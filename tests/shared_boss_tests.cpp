#include "rpg/shared_boss.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace {
using namespace rpg;
void check(bool ok,const char* message){if(!ok){throw std::runtime_error(message);}}
template<class F>void invalid(F f){bool caught=false;try{f();}catch(const std::invalid_argument&){caught=true;}check(caught,"invalid shared profile accepted");}
SharedBossDefinition setup(){
    BattleRules rules;rules.timing=TimingProfile::ActionBar;
    SharedBossDefinition d{"shared",1,{}, {"boss",Team::B,30,3,100},rules};
    for(int i=0;i<3;++i){const auto id=std::to_string(i);d.squads.push_back({"team-"+id,{{"char-"+id,{"A"+id,Team::A,10,2,300}}}});}
    return d;
}
SharedBossActionResult step(SharedBoss& stage){const auto action=stage.choose_basic_action();check(action.has_value(),"no shared command");
    auto r=stage.apply(*action);check(r.accepted(),"valid shared command rejected");return r;}
void composition(){
    for(int size=1;size<=5;++size){auto d=setup();for(auto& squad:d.squads){for(int i=1;i<size;++i){
        const auto id=squad.id+std::to_string(i);squad.members.push_back({"char"+id,{id,Team::A,10,2,300}});}}
        SharedBoss s(d);check(s.battle().fighters().size()==static_cast<std::size_t>(size*3+1),"boss duplicated or wrong roster");}
    auto d=setup();d.squads.pop_back();invalid([&]{SharedBoss s(d);});
    d=setup();d.squads[2].members[0].character_id=d.squads[0].members[0].character_id;invalid([&]{SharedBoss s(d);});
    d=setup();d.squads[2].members[0].fighter.id="boss";invalid([&]{SharedBoss s(d);});
    d=setup();d.squads[2].id=d.squads[0].id;invalid([&]{SharedBoss s(d);});
    d=setup();d.squads[2].members.clear();invalid([&]{SharedBoss s(d);});
    d=setup();d.squads[2].members[0].fighter.speed=0;invalid([&]{SharedBoss s(d);});
    d=setup();d.rules.skills={{"later","boss",0,TargetRule::Self,{{EffectKind::Marker,0,1}}}};invalid([&]{SharedBoss s(d);});
}
void one_hp_and_cadence(){
    SharedBoss s(setup());for(int i=0;i<3;++i){check(s.battle().current_actor()->definition.id=="A"+std::to_string(i),"unexpected initial AP order");step(s);}
    check(s.battle().find("boss")->health==24 && s.battle().find("boss")->definition.max_health==30 &&
          s.battle().snapshot().accepted_actions==3,"three teams did not damage single HP");
    auto d=setup();d.boss.max_health=1000;d.boss.speed=900;SharedBoss fast(d);int attacks=0;
    while(attacks<3){const auto action=*fast.choose_basic_action();const auto r=fast.apply(action);check(r.accepted(),"fast boss rejected");
        if(action.actor=="boss"){check(action.target=="A"+std::to_string(attacks) && r.squad_id=="team-"+std::to_string(attacks),"boss did not rotate front");++attacks;}}
    for(int i=0;i<3;++i){check(fast.battle().find("A"+std::to_string(i))->health==7,"boss action tripled across teams");}
}
void target_atomicity_and_guard(){
    auto d=setup();d.boss.speed=900;SharedBoss s(d);const auto before=s.snapshot();
    check(s.apply({"boss",ActionType::Attack,"A1"}).error==SharedBossError::WrongFront && s.snapshot()==before,"cross-front attack mutated");
    check(!s.apply({"unknown",ActionType::Attack,"boss"}).accepted() && s.snapshot()==before,"unknown actor mutated");
    check(s.apply({"boss",ActionType::Guard,{}}).accepted() && s.snapshot().next_boss_front=="team-0","guard advanced attack rotation");
    const auto guarded=s.snapshot();check(!s.apply({"boss",ActionType::Skill,"boss","missing"}).accepted() && s.snapshot()==guarded,"unknown skill mutated");
}
void skip_dead_fronts_and_defeat(){
    auto d=setup();d.boss.speed=900;d.boss.max_health=1000;d.boss.attack=10;SharedBoss s(d);int attacks=0,finishes=0;
    for(int i=0;i<100 && s.battle().phase()==Phase::Running;++i){const auto command=*s.choose_basic_action();
        const auto r=s.apply(command);check(r.accepted(),"dead-front flow rejected");finishes+=r.stage_finished?1:0;
        if(command.actor=="boss"){check(command.target=="A"+std::to_string(attacks++),"dead front repeated");}}
    check(attacks==3 && finishes==1 && s.battle().winner()==Team::B && !s.snapshot().next_boss_front &&
          s.snapshot().active_squads==std::vector<bool>(3,false),"defeat ignored surviving teams or repeated conclusion");
    d=setup();d.boss.speed=900;d.boss.max_health=1000;d.boss.attack=10;d.squads[1].members[0].fighter.max_health=100;
    d.squads[2].members[0].fighter.max_health=100;SharedBoss skipped(d);int seen=0;
    for(int i=0;i<100 && seen<4;++i){const auto action=*skipped.choose_basic_action();const auto r=skipped.apply(action);check(r.accepted(),"skip flow rejected");
        if(action.actor=="boss"){const std::string expected=seen==0?"A0":seen==2?"A2":"A1";
            check(action.target==expected,"rotation did not skip wiped team");++seen;}}
    check(seen==4,"did not exercise skipped team");
}
void victory_budget_replay_reset(){
    auto d=setup();d.boss.max_health=6;d.rules.action_budget=ActionBudget{3,LimitOutcome::DefeatA};SharedBoss a(d),b(d);const auto initial=a.snapshot();
    int finishes=0;
    while(a.battle().phase()==Phase::Running){const auto command=*a.choose_basic_action();const auto x=a.apply(command),y=b.apply(command);
        check(x.action==y.action && x.squad_id==y.squad_id && a.snapshot()==b.snapshot(),"shared replay diverged");finishes+=x.stage_finished?1:0;}
    check(finishes==1 && a.battle().winner()==Team::A && a.battle().end_reason()==EndReason::Elimination &&
          a.battle().find("boss")->health==0,"final kill lost to timeout");
    const auto final=a.snapshot();check(!a.apply({"A0",ActionType::Attack,"boss"}).accepted() && a.snapshot()==final,"post-victory mutation");
    a.reset();check(a.snapshot()==initial && b.snapshot()==final,"shared reset leaked");
    d=setup();d.rules.action_budget=ActionBudget{1,LimitOutcome::Draw};SharedBoss limited(d);step(limited);
    check(limited.battle().end_reason()==EndReason::ActionLimit && !limited.battle().winner(),"global budget not applied");
}
}
int main(int argc,char** argv){
    const std::pair<const char*,void(*)()> tests[]={{"composition",composition},{"one_hp_cadence",one_hp_and_cadence},
        {"target_atomics_guard",target_atomicity_and_guard},{"dead_fronts_defeat",skip_dead_fronts_and_defeat},
        {"victory_budget_replay_reset",victory_budget_replay_reset}};
    int passed=0;for(const auto& [name,test]:tests){try{test();++passed;std::cout<<"PASS shared_boss "<<name<<'\n';}
        catch(const std::exception& e){std::cerr<<"FAIL shared_boss "<<name<<": "<<e.what()<<'\n';}}
    std::cout<<"SHARED_BOSS_TESTS_RESULT passed="<<passed<<" total="<<std::size(tests)<<'\n';
    if(argc==2 && passed==static_cast<int>(std::size(tests))){std::ofstream out(argv[1]);out<<"test,status,scope\n";
        for(const auto& [name,test]:tests){static_cast<void>(test);out<<name<<",pass,single_hp_basic_shared_boss\n";}if(!out){return 2;}}
    return passed==static_cast<int>(std::size(tests))?0:1;
}
