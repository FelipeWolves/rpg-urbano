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
}
int main(int argc,char** argv){
    const std::pair<const char*,void(*)()> tests[]={{"configuration",configuration},{"atomicity_isolation",atomicity_and_isolation},
        {"fairness_completion",fairness_and_completion},{"schedule_replay_reset",schedule_independence_and_reset},{"aggregate_outcomes",aggregate_outcomes}};
    int passed=0;
    for(const auto& [name,test]:tests){try{test();++passed;std::cout<<"PASS parallel "<<name<<'\n';}
        catch(const std::exception& e){std::cerr<<"FAIL parallel "<<name<<": "<<e.what()<<'\n';}}
    std::cout<<"PARALLEL_TESTS_RESULT passed="<<passed<<" total="<<std::size(tests)<<'\n';
    if(argc==2 && passed==static_cast<int>(std::size(tests))){std::ofstream out(argv[1]);out<<"test,status,scope\n";
        for(const auto& [name,test]:tests){static_cast<void>(test);out<<name<<",pass,independent_cooperative_fronts\n";}if(!out){return 2;}}
    return passed==static_cast<int>(std::size(tests))?0:1;
}
