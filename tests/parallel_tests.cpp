#include "rpg/parallel.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace {
using namespace rpg;
void check(bool ok,const char* message){if(!ok){throw std::runtime_error(message);}}
template<class F>void invalid(F f){bool caught=false;try{f();}catch(const std::invalid_argument&){caught=true;}check(caught,"invalid front configuration accepted");}
ParallelDefinition setup(){
    ParallelDefinition d{"parallel",1,{}};
    for(int i=0;i<3;++i){BattleRules r;r.timing=TimingProfile::ActionBar;
        d.fronts.push_back({"front-"+std::to_string(i),{{"A",Team::A,20,4,200},{"boss",Team::B,8+i*4,1,100}},r});}
    return d;
}
ParallelActionResult step(ParallelFronts& stage){
    const auto index=stage.current_front();check(index.has_value(),"no scheduled front");
    const auto action=choose_basic_attack(stage.battle(*index));check(action.has_value(),"no action");
    const auto result=stage.apply(stage.snapshot().front_ids[*index],*action);check(result.accepted(),"scheduled action rejected");return result;
}
void run(ParallelFronts& stage){for(int i=0;i<200 && stage.current_front();++i){step(stage);}check(!stage.current_front(),"nonterminating fronts");}
void configuration(){
    auto d=setup();d.fronts.pop_back();invalid([&]{ParallelFronts p(d);});
    d=setup();d.fronts[2].id=d.fronts[0].id;invalid([&]{ParallelFronts p(d);});
    d=setup();d.schedule={0,0,2};invalid([&]{ParallelFronts p(d);});
    d=setup();d.schedule={0,1,3};invalid([&]{ParallelFronts p(d);});
    d=setup();d.fronts[2].fighters[0].speed=0;invalid([&]{ParallelFronts p(d);});
    d=setup();d.config_version=0;invalid([&]{ParallelFronts p(d);});
}
void atomicity_and_isolation(){
    ParallelFronts p(setup());const auto before=p.snapshot();
    check(p.apply("missing",{"A",ActionType::Attack,"boss"}).error==ParallelError::UnknownFront,"unknown front accepted");
    check(p.apply("front-1",{"A",ActionType::Attack,"boss"}).error==ParallelError::NotScheduled,"schedule bypassed");
    check(p.apply("front-0",{"boss",ActionType::Attack,"A"}).error==ParallelError::BattleRejected,"wrong actor accepted");
    check(p.snapshot()==before,"rejection mutated coordinator");
    const auto result=step(p);const auto after=p.snapshot();
    check(result.stage_action_id==1 && result.front_id=="front-0" && after.fronts[1]==before.fronts[1] &&
          after.fronts[2]==before.fronts[2] && p.battle(0).find("boss")->health==4 && p.current_front()==1,
          "front action leaked or scheduling failed");
}
void fairness_and_completion(){
    ParallelFronts p(setup());int finishes=0,fronts=0;std::uint64_t actions=0;
    for(int i=0;i<3;++i){check(p.current_front()==static_cast<std::size_t>(i),"front starved on first cycle");step(p);++actions;}
    while(p.current_front()){
        const auto index=*p.current_front();check(p.battle(index).phase()==Phase::Running,"finished front scheduled");
        const auto r=step(p);++actions;check(r.stage_action_id==actions,"global action identity repeated");
        finishes+=r.stage_finished?1:0;fronts+=r.front_finished?1:0;
    }
    const auto final=p.snapshot();check(finishes==1 && fronts==3 && final.winner==Team::A &&
        final.end_reason==ParallelEndReason::AllFrontsFinished && final.accepted_actions==actions,"aggregate completion wrong");
    check(!p.apply("front-0",{"A",ActionType::Attack,"boss"}).accepted() && p.snapshot()==final,"finished phase mutated");
}
void schedule_independence_and_reset(){
    auto d=setup();ParallelFronts a(d);d.schedule={2,1,0};ParallelFronts b(d);run(a);run(b);
    check(a.snapshot().fronts==b.snapshot().fronts && a.snapshot().winner==b.snapshot().winner &&
          a.snapshot().accepted_actions==b.snapshot().accepted_actions,"schedule altered independent outcomes");
    ParallelFronts replay(setup());run(replay);check(a.snapshot()==replay.snapshot(),"replay diverged");
    const auto final=b.snapshot();a.reset();ParallelFronts fresh(setup());
    check(a.snapshot()==fresh.snapshot() && b.snapshot()==final,"reset retained state or leaked");
}
void aggregate_outcomes(){
    auto d=setup();d.fronts[1].fighters[0].max_health=1;d.fronts[1].fighters[0].speed=50;
    ParallelFronts lost(d);run(lost);check(lost.snapshot().winner==Team::B,"one defeated front did not lose aggregate");
    d=setup();d.fronts[1].rules.action_budget=ActionBudget{1,LimitOutcome::Draw};
    ParallelFronts draw(d);run(draw);check(!draw.snapshot().winner && draw.battle(1).end_reason()==EndReason::ActionLimit,
                                        "draw plus victories aggregated incorrectly");
}
void global_budget_validation_and_atomicity(){
    auto d=setup();d.global_budget=ActionBudget{1,LimitOutcome::Draw};
    d.global_budget->max_actions=0;invalid([&]{ParallelFronts p(d);});
    d.global_budget->max_actions=-1;invalid([&]{ParallelFronts p(d);});
    d.global_budget=ActionBudget{1,static_cast<LimitOutcome>(99)};invalid([&]{ParallelFronts p(d);});
    for(const auto policy:{LimitOutcome::Draw,LimitOutcome::DefeatA,LimitOutcome::DefeatB}){
        d=setup();d.global_budget=ActionBudget{1,policy};ParallelFronts p(d);const auto before=p.snapshot();
        check(!p.apply("front-1",{"A",ActionType::Attack,"boss"}).accepted() && p.snapshot()==before &&
              p.remaining_actions()==1,"invalid action consumed global budget");
        const auto r=step(p);const auto final=p.snapshot();
        check(r.stage_finished && r.cancelled_fronts.size()==3 && final.accepted_actions==1 && p.remaining_actions()==0 &&
              final.end_reason==ParallelEndReason::GlobalActionLimit,"global budget tripled or failed cancellation");
        const std::optional<Team> expected=policy==LimitOutcome::Draw?std::nullopt:
            std::optional<Team>{policy==LimitOutcome::DefeatA?Team::B:Team::A};
        check(final.winner==expected && final.fronts[1]==before.fronts[1] && final.fronts[2]==before.fronts[2],
              "timeout changed idle battle or wrong winner");
        check(!p.apply("front-0",{"A",ActionType::Attack,"boss"}).accepted() && !p.cancel() && p.snapshot()==final,
              "post-timeout mutation accepted");
    }
}
void local_global_boundaries(){
    auto d=setup();d.global_budget=ActionBudget{2,LimitOutcome::DefeatA};
    d.fronts[0].rules.action_budget=ActionBudget{1,LimitOutcome::Draw};ParallelFronts p(d);
    check(step(p).front_finished && p.current_front()==1 && p.remaining_actions()==1,"local limit ended global phase");
    const auto r=step(p);check(r.stage_finished && r.cancelled_fronts.size()==2 &&
        p.snapshot().front_states[0]==FrontState::Finished && p.snapshot().front_states[1]==FrontState::Cancelled,
        "finished front cancelled or global counter reset");
    d=setup();for(auto& f:d.fronts){f.rules.action_budget=ActionBudget{1,LimitOutcome::Draw};}
    d.global_budget=ActionBudget{3,LimitOutcome::DefeatA};ParallelFronts equal(d);run(equal);
    check(equal.snapshot().end_reason==ParallelEndReason::GlobalActionLimit && equal.snapshot().winner==Team::B &&
          equal.snapshot().front_states==std::vector<FrontState>(3,FrontState::Finished),"global tie did not precede local draws");
}
void last_action_elimination_priority(){
    auto d=setup();for(auto& f:d.fronts){f.fighters[1].max_health=4;}
    d.global_budget=ActionBudget{3,LimitOutcome::DefeatA};ParallelFronts p(d);run(p);
    check(p.snapshot().end_reason==ParallelEndReason::AllFrontsFinished && p.snapshot().winner==Team::A &&
          p.remaining_actions()==0,"final elimination lost to global timeout");
    d.global_budget=ActionBudget{1,LimitOutcome::DefeatA};ParallelFronts partial(d);const auto r=step(partial);
    check(r.front_finished && r.cancelled_fronts.size()==2 && partial.snapshot().winner==Team::B &&
          partial.snapshot().end_reason==ParallelEndReason::GlobalActionLimit,"one local kill granted global victory");
}
void manual_cancel_and_reset(){
    ParallelFronts p(setup());step(p);const auto before=p.snapshot();
    check(p.cancel(),"running cancellation rejected");const auto after=p.snapshot();
    check(after.fronts==before.fronts && after.accepted_actions==before.accepted_actions && !after.winner &&
          after.end_reason==ParallelEndReason::Cancelled && after.front_states==std::vector<FrontState>(3,FrontState::Cancelled),
          "cancel altered battle evidence or fabricated winner");
    check(!p.cancel() && p.snapshot()==after && !p.apply("front-1",{"A",ActionType::Attack,"boss"}).accepted(),
          "cancellation not idempotent");
    p.reset();ParallelFronts fresh(setup());check(p.snapshot()==fresh.snapshot(),"reset retained cancellation");
    auto d=setup();d.fronts[0].fighters[1].max_health=4;ParallelFronts finished(d);step(finished);
    check(finished.cancel() && finished.snapshot().front_states[0]==FrontState::Finished,"cancel erased local completion");
}
void bounded_replay_schedules(){
    for(const auto order:{std::array<std::size_t,3>{0,1,2},std::array<std::size_t,3>{2,1,0}}){
        auto d=setup();d.schedule=order;d.global_budget=ActionBudget{5,LimitOutcome::Draw};
        ParallelFronts a(d),b(d);int finishes=0;
        while(a.current_front()){
            const auto x=step(a),y=step(b);finishes+=x.stage_finished?1:0;
            check(x.action==y.action && x.cancelled_fronts==y.cancelled_fronts && a.snapshot()==b.snapshot(),"bounded replay diverged");
        }
        check(finishes==1 && a.snapshot().accepted_actions==5 && a.remaining_actions()==0,"bounded schedule exceeded budget");
        const auto final=b.snapshot();a.reset();ParallelFronts fresh(d);
        check(a.snapshot()==fresh.snapshot() && b.snapshot()==final,"bounded reset leaked");
    }
}
}
int main(int argc,char** argv){
    const std::pair<const char*,void(*)()> tests[]={{"configuration",configuration},{"atomicity_isolation",atomicity_and_isolation},
        {"fairness_completion",fairness_and_completion},{"schedule_replay_reset",schedule_independence_and_reset},{"aggregate_outcomes",aggregate_outcomes},
        {"global_validation_atomics",global_budget_validation_and_atomicity},{"local_global_boundaries",local_global_boundaries},
        {"last_elimination_priority",last_action_elimination_priority},{"manual_cancel_reset",manual_cancel_and_reset},
        {"bounded_replay_schedules",bounded_replay_schedules}};
    int passed=0;
    for(const auto& [name,test]:tests){try{test();++passed;std::cout<<"PASS parallel "<<name<<'\n';}
        catch(const std::exception& e){std::cerr<<"FAIL parallel "<<name<<": "<<e.what()<<'\n';}}
    std::cout<<"PARALLEL_TESTS_RESULT passed="<<passed<<" total="<<std::size(tests)<<'\n';
    if(argc==2 && passed==static_cast<int>(std::size(tests))){std::ofstream out(argv[1]);out<<"test,status,scope\n";
        for(const auto& [name,test]:tests){static_cast<void>(test);out<<name<<",pass,independent_cooperative_fronts\n";}if(!out){return 2;}}
    return passed==static_cast<int>(std::size(tests))?0:1;
}
