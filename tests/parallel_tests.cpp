#include "rpg/parallel.hpp"
#include <algorithm>
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
std::size_t events(const ParallelActionResult& r,EventType type){
    return static_cast<std::size_t>(std::ranges::count(r.action.events,type,&Event::type));
}
ParallelActionResult command(ParallelFronts& p,const Action& action){
    const auto index=p.current_front();check(index.has_value(),"no integration front");
    auto r=p.apply(p.snapshot().front_ids[*index],action);check(r.accepted(),"integration command rejected");return r;
}
void r04_final_elimination_replay(){
    for(const auto order:{std::array<std::size_t,3>{0,1,2},std::array<std::size_t,3>{2,1,0}}){
        auto d=setup();d.schedule=order;d.global_budget=ActionBudget{9,LimitOutcome::DefeatA};
        for(auto& f:d.fronts){f.rules.timing=TimingProfile::FixedRounds;f.fighters[1].max_health=4;
            EffectDefinition multi{EffectKind::Damage};multi.hit_count=3;
            f.rules.skills={{"arm","A",0,TargetRule::Enemy,{{EffectKind::Reflect,1,10},
                {EffectKind::DeathExplosion,30,10},{EffectKind::Counter,0,10}}},
                {"multi","A",0,TargetRule::Enemy,{multi}}};}
        ParallelFronts a(d),b(d);int finishes=0;
        for(int turn=0;turn<9;++turn){
            const Action action=turn<3?Action{"A",ActionType::Skill,"boss","arm"}:
                turn<6?Action{"boss",ActionType::Attack,"A"}:Action{"A",ActionType::Skill,"boss","multi"};
            const auto index=*a.current_front();const auto before=a.snapshot();
            const auto x=command(a,action),y=command(b,action);finishes+=x.stage_finished?1:0;
            check(x.action==y.action && x.stage_action_id==y.stage_action_id && x.cancelled_fronts==y.cancelled_fronts &&
                  a.snapshot()==b.snapshot(),"R04 replay diverged");
            for(std::size_t other=0;other<3;++other){if(other!=index){
                check(a.snapshot().fronts[other]==before.fronts[other],"R04 consequences crossed front scope");}}
            if(turn>=6){check(events(x,EventType::Attack)==1 && events(x,EventType::ReflectedDamage)==1 &&
                events(x,EventType::DeathExplosion)==1 && a.battle(index).find("A")->health==0 &&
                a.battle(index).find("boss")->health==0 && a.battle(index).winner()==Team::A &&
                a.battle(index).snapshot().derived_actions==0,"R04 chain, author tie or dead counter wrong");}
        }
        check(finishes==1 && a.snapshot().winner==Team::A && a.snapshot().end_reason==ParallelEndReason::AllFrontsFinished &&
              a.remaining_actions()==0,"R04 final victory lost to timeout");
    }
}
void revive_before_timeout(){
    auto d=setup();d.global_budget=ActionBudget{5,LimitOutcome::Draw};
    auto& f=d.fronts[0];f.rules.timing=TimingProfile::FixedRounds;f.fighters[1].max_health=4;
    EffectDefinition multi{EffectKind::Damage};multi.hit_count=3;
    f.rules.skills={{"revive","A",0,TargetRule::Enemy,{{EffectKind::AutoRevive,2,10}}},
                    {"multi","A",0,TargetRule::Enemy,{multi}}};
    for(std::size_t i=1;i<3;++i){d.fronts[i].rules.action_budget=ActionBudget{1,LimitOutcome::Draw};}
    ParallelFronts p(d);command(p,{"A",ActionType::Skill,"boss","revive"});step(p);step(p);
    command(p,{"boss",ActionType::Attack,"A"});const auto result=command(p,{"A",ActionType::Skill,"boss","multi"});
    check(events(result,EventType::FighterRevived)==1 && events(result,EventType::Attack)==1 &&
          p.battle(0).find("boss")->health==2 && p.battle(0).phase()==Phase::Running &&
          p.snapshot().front_states[0]==FrontState::Cancelled && result.cancelled_fronts==std::vector<std::string>{"front-0"} &&
          p.snapshot().end_reason==ParallelEndReason::GlobalActionLimit,"timeout preceded revive or fabricated local victory");
}
void skip_global_accounting(){
    auto d=setup();d.global_budget=ActionBudget{9,LimitOutcome::Draw};
    for(auto& f:d.fronts){f.rules.timing=TimingProfile::FixedRounds;
        f.rules.skills={{"block","A",0,TargetRule::Self,{{EffectKind::Block,0,3}}}};}
    ParallelFronts p(d);
    for(int i=0;i<3;++i){command(p,{"A",ActionType::Skill,"A","block"});}
    for(int i=0;i<3;++i){command(p,{"boss",ActionType::Attack,"A"});}
    for(int i=0;i<3;++i){const auto before=p.snapshot();const auto index=*p.current_front();
        check(!p.apply(before.front_ids[index],{"A",ActionType::Attack,"boss"}).accepted() && p.snapshot()==before,
              "blocked attack mutated state");
        const auto r=command(p,{"A",ActionType::Skip,{}});
        check(events(r,EventType::TurnSkipped)==1 && p.battle(index).accepted_actions()==3,"Skip not counted once");}
    check(p.snapshot().accepted_actions==9 && p.remaining_actions()==0 && p.snapshot().end_reason==ParallelEndReason::GlobalActionLimit,
          "Skip bypassed global budget");
}
void reactions_before_global_limit(){
    auto d=setup();d.global_budget=ActionBudget{6,LimitOutcome::Draw};
    for(auto& f:d.fronts){f.rules.timing=TimingProfile::FixedRounds;
        f.rules.skills={{"counter","A",0,TargetRule::Self,{{EffectKind::Counter,0,10}}}};}
    ParallelFronts p(d);
    for(int i=0;i<3;++i){command(p,{"A",ActionType::Skill,"A","counter"});}
    for(int i=0;i<3;++i){const auto index=*p.current_front();const auto r=command(p,{"boss",ActionType::Attack,"A"});
        check(events(r,EventType::Reaction)==1 && p.battle(index).snapshot().derived_actions==1 &&
              p.battle(index).accepted_actions()==2 && p.battle(index).find("boss")->health==d.fronts[index].fighters[1].max_health-4,
              "reaction interrupted or spent primary budget");}
    check(p.snapshot().accepted_actions==6 && p.snapshot().end_reason==ParallelEndReason::GlobalActionLimit,
          "reaction changed global accounting");
}
void technical_priority_and_reset(){
    for(const bool bounded:{false,true}){
        auto d=setup();if(bounded){d.global_budget=ActionBudget{6,LimitOutcome::DefeatA};}
        d.fronts[1].fighters[1].max_health=4;auto& r=d.fronts[0].rules;r.max_derived_actions=2;
        r.skills={{"loop","A",0,TargetRule::Self,{{EffectKind::ExtraTurn}}}};
        ParallelFronts p(d);const auto initial=p.snapshot();
        command(p,{"A",ActionType::Skill,"A","loop"});step(p);step(p);
        command(p,{"A",ActionType::Skill,"A","loop"});step(p);
        const auto before=p.snapshot();const auto last=command(p,{"A",ActionType::Skill,"A","loop"});
        const auto final=p.snapshot();
        check(last.stage_finished && final.end_reason==ParallelEndReason::TechnicalFailure && !final.winner &&
              final.accepted_actions==6 && final.front_states[0]==FrontState::Finished &&
              final.front_states[1]==FrontState::Finished && final.front_states[2]==FrontState::Cancelled &&
              last.cancelled_fronts==std::vector<std::string>{"front-2"} && final.fronts[2]==before.fronts[2],
              "technical failure lost priority or cancelled completed front");
        check(!p.cancel() && !p.apply("front-2",{"A",ActionType::Attack,"boss"}).accepted() && p.snapshot()==final,
              "technical completion mutated");
        p.reset();check(p.snapshot()==initial,"technical reset retained failure");
    }
}
void extras_cancellation_boundary(){
    auto d=setup();d.global_budget=ActionBudget{1,LimitOutcome::Draw};
    d.fronts[0].rules.skills={{"extra","A",3,TargetRule::Self,{{EffectKind::ExtraTurn},{EffectKind::Marker,0,5}}}};
    ParallelFronts p(d);const auto r=command(p,{"A",ActionType::Skill,"A","extra"});const auto final=p.snapshot();
    check(r.stage_finished && p.battle(0).snapshot().turn_kind==TurnKind::Extra &&
          p.battle(0).accepted_actions()==1 && !p.battle(0).find("A")->statuses.empty() &&
          final.front_states[0]==FrontState::Cancelled,"extra/status not resolved before cancellation");
    check(!p.apply("front-0",{"A",ActionType::Attack,"boss"}).accepted() && p.snapshot()==final,
          "pending extra executed after cancellation");
    p.reset();ParallelFronts fresh(d);check(p.snapshot()==fresh.snapshot(),"reset retained extra/status/cooldown");
}
}
int main(int argc,char** argv){
    const std::pair<const char*,void(*)()> tests[]={{"configuration",configuration},{"atomicity_isolation",atomicity_and_isolation},
        {"fairness_completion",fairness_and_completion},{"schedule_replay_reset",schedule_independence_and_reset},{"aggregate_outcomes",aggregate_outcomes},
        {"global_validation_atomics",global_budget_validation_and_atomicity},{"local_global_boundaries",local_global_boundaries},
        {"last_elimination_priority",last_action_elimination_priority},{"manual_cancel_reset",manual_cancel_and_reset},
        {"bounded_replay_schedules",bounded_replay_schedules},{"r04_final_replay",r04_final_elimination_replay},
        {"revive_before_timeout",revive_before_timeout},{"skip_global",skip_global_accounting},
        {"reaction_global_boundary",reactions_before_global_limit},{"technical_priority_reset",technical_priority_and_reset},
        {"extra_cancel_boundary",extras_cancellation_boundary}};
    int passed=0;
    for(const auto& [name,test]:tests){try{test();++passed;std::cout<<"PASS parallel "<<name<<'\n';}
        catch(const std::exception& e){std::cerr<<"FAIL parallel "<<name<<": "<<e.what()<<'\n';}}
    std::cout<<"PARALLEL_TESTS_RESULT passed="<<passed<<" total="<<std::size(tests)<<'\n';
    if(argc==2 && passed==static_cast<int>(std::size(tests))){std::ofstream out(argv[1]);out<<"test,status,scope\n";
        for(const auto& [name,test]:tests){static_cast<void>(test);out<<name<<",pass,independent_cooperative_fronts\n";}if(!out){return 2;}}
    return passed==static_cast<int>(std::size(tests))?0:1;
}
