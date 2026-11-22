#include "rpg/battle_presentation.hpp"
#include "rpg/stage.hpp"
#include "rpg/sequential.hpp"
#include "rpg/shared_boss.hpp"
#include <algorithm>
#include <climits>
#include <tuple>
#include <iostream>
#include <stdexcept>
using namespace rpg;
using namespace rpg::presentation;
void check(bool ok,const char* message){if(!ok){throw std::runtime_error(message);}}
const Status* shield(const Fighter* f){auto it=std::ranges::find(f->statuses,EffectKind::StandardShield,&Status::kind);return it==f->statuses.end()?nullptr:&*it;}
BattleRules rules(TargetRule target=TargetRule::Ally){BattleRules r;r.skills={{"shield","A",0,target,{{EffectKind::StandardShield}}}};return r;}
std::vector<Definition> roster(int maximum=1000){return {{"A",Team::A,maximum,1,300},{"C",Team::A,1000,1,200},{"B",Team::B,10000,1,100}};}
void cast(Battle& b,const char* id="C"){check(b.apply({"A",ActionType::Skill,id,"shield"}).accepted(),"cast failed");}
void until(Battle& b,const char* id){for(int i=0;i<30 && b.current_actor()->definition.id!=id;++i){auto action=choose_basic_attack(b);check(action && b.apply(*action).accepted(),"advance failed");}check(b.current_actor()->definition.id==id,"actor missing");}
void shield_numeric(){for(const auto& [hp,expected]:std::vector<std::pair<int,int>>{{1,1},{9,1},{10,1},{55,1},{99,1},{100,1},{550,5},{1000,10},{INT_MAX,21474836}}){Battle b(roster(hp),rules());auto result=b.apply({"A",ActionType::Skill,"C","shield"});check(result.accepted(),"numeric cast");const auto* s=shield(b.find("C"));check(s && s->magnitude==expected && s->remaining==3,"floor/minimum incorrect");check(std::ranges::any_of(result.events,[](const Event& e){return e.type==EventType::EffectApplied;}),"valid minimum shield missing event");}}
void independent(){auto r=rules();r.skills.push_back({"hurt","B",0,TargetRule::Enemy,{{EffectKind::Damage}}});auto units=roster();units[2].attack=20;Battle b(units,r,{{"A",500},{"C",50},{"B",10000}});cast(b);check(shield(b.find("C"))->magnitude==10,"uses recipient/current caster HP");until(b,"B");check(b.apply({"B",ActionType::Attack,"A",{}}).accepted(),"damage caster");check(shield(b.find("C"))->magnitude==10,"caster damage recalculated");}
void self_clock(){Battle b(roster(),rules(TargetRule::Self));cast(b,"A");check(shield(b.find("A"))->remaining==3,"self immediate tick");for(int remaining=2;remaining>=0;--remaining){until(b,"A");check(b.apply({"A",ActionType::Attack,"B",{}}).accepted(),"natural action");auto s=shield(b.find("A"));check(remaining==0?!s:s && s->remaining==remaining,"natural clock");}}
void recipient_clock(){Battle b(roster(),rules());cast(b);check(b.current_actor()->definition.id=="C","target order");check(b.apply({"C",ActionType::Attack,"B",{}}).accepted(),"recipient action");check(shield(b.find("C"))->remaining==2,"target did not tick");}
void reapplied(){auto r=rules();r.skills.push_back({"shield-c","C",0,TargetRule::Ally,{{EffectKind::StandardShield}}});for(const auto& [oldPoints,oldDuration,newMax]:std::vector<std::tuple<int,int,int>>{{20,1,1000},{10,5,2000},{20,5,1000},{10,1,2000}}){Battle b(roster(newMax),r,{},{{"C",200,{{EffectKind::StandardShield,oldDuration,TickPolicy::NaturalOnly,0,oldPoints}},{{"shield-c",0}}}});cast(b);const auto* s=shield(b.find("C"));check(s && s->magnitude==std::max(oldPoints,newMax/100) && s->remaining==std::max(oldDuration,3),"max properties");}}
void minimum_and_dead(){Battle b(roster(55),rules(),{},{{"C",200,{{EffectKind::StandardShield,1,TickPolicy::NaturalOnly,0,10}}, {}}});cast(b);check(shield(b.find("C"))->magnitude==10 && shield(b.find("C"))->remaining==3,"minimum application must preserve stronger points and renew duration");Battle dead(roster(),rules(),{{"A",1000},{"B",10000},{"C",0}});const auto before=dead.snapshot();check(dead.apply({"A",ActionType::Skill,"C","shield"}).error==ActionError::TargetDefeated && dead.snapshot()==before,"dead target accepted");}
void extra_and_incapacitated(){auto r=rules(TargetRule::Self);r.skills[0].effects.push_back({EffectKind::ExtraTurn,0,0,TickPolicy::NaturalOnly,ExtraTurnSource{"external",ExtraOrigin::Equipment,true,true}});r.timing=TimingProfile::ActionBar;Battle b(roster(),r);cast(b,"A");check(b.snapshot().turn_kind==TurnKind::Extra,"extra not scheduled");check(b.apply({"A",ActionType::Attack,"B",{}}).accepted(),"extra action");check(shield(b.find("A"))->remaining==3,"extra policy ticked shield");auto fixed=rules();Battle blocked(roster(),fixed,{},{{"C",200,{{EffectKind::Block,2,TickPolicy::NaturalOnly,0}}, {}}});cast(blocked);check(blocked.apply({"C",ActionType::Skip,{},{}}).accepted(),"skip failed");check(shield(blocked.find("C"))->remaining==2,"incapacitated natural missing tick");}
void hits_and_defense(){auto r=rules(TargetRule::Self);EffectDefinition hit{EffectKind::Damage};hit.hit_count=2;r.skills.push_back({"multi","B",0,TargetRule::Enemy,{hit}});auto units=roster();units[2].attack=6;Battle b(units,r);cast(b,"A");until(b,"B");auto result=b.apply({"B",ActionType::Skill,"A","multi"});check(result.accepted() && !shield(b.find("A")) && b.find("A")->health==998,"multi hit excess");check(std::ranges::count(result.events,EventType::ShieldAbsorbed,&Event::type)==2,"not per hit");Battle guarded(units,r,{},{{"A",300,{{EffectKind::StandardShield,3,TickPolicy::NaturalOnly,0,5}},{{"shield",0}}}});check(guarded.apply({"A",ActionType::Guard,{},{}}).accepted(),"guard");until(guarded,"B");check(guarded.apply({"B",ActionType::Attack,"A",{}}).accepted(),"hit guarded");check(guarded.find("A")->health==1000 && shield(guarded.find("A"))->magnitude==2,"mitigation order");}
void death_revive(){auto r=rules();r.skills.push_back({"fatal","C",0,TargetRule::Self,{{EffectKind::Damage}}});r.skills.pop_back();auto units=roster();units[2].attack=2000;Battle b(units,r,{},{{"C",200,{{EffectKind::AutoRevive,5,TickPolicy::NaturalOnly,0,100}}, {}}});cast(b);until(b,"B");check(b.apply({"B",ActionType::Attack,"C",{}}).accepted(),"fatal");check(b.find("C")->health==100 && !shield(b.find("C")),"revive restored shield");auto other=roster();other[2].attack=2000;Battle caster(other,r);cast(caster);until(caster,"B");check(caster.apply({"B",ActionType::Attack,"A",{}}).accepted(),"caster death");check(caster.find("A")->health==0 && shield(caster.find("C")),"caster dependency");}
void transition(){for(auto carry:{RuntimeCarry::Preserve,RuntimeCarry::Reset}){auto r=rules(TargetRule::Self);r.skills.push_back({"kill","A",0,TargetRule::Enemy,{{EffectKind::Damage}}});StageDefinition d{"stage",1,{{"A",Team::A,1000,5,300}},{{"one",{{"B",Team::B,1,1,100}},r},{"two",{{"B",Team::B,10,1,100}},r}},CarryPolicy::PreserveHealth,std::nullopt,carry};Stage stage(d);check(stage.apply({"A",ActionType::Skill,"A","shield"}).action.accepted(),"stage cast");while(stage.battle().current_actor()->definition.id!="A"){auto a=choose_basic_attack(stage.battle());check(a && stage.apply(*a).action.accepted(),"stage advance");}check(shield(stage.battle().find("A")),"missing pretransition");check(stage.apply({"A",ActionType::Skill,"B","kill"}).action.accepted(),"stage kill");check(stage.snapshot().encounter_index==1 && !shield(stage.battle().find("A")),"shield carried");}}
void presentation_contract(){Fighter f{{"A",Team::A,100,1,1},50};f.statuses={{EffectKind::StandardShield,3,TickPolicy::NaturalOnly,0,10}};check(shield_display(f).points==10 && shield_display(f).overlay_fraction==0.1,"presentation shield");check(effect_details(f)[0].find("Standard shield")!=std::string::npos,"effect label");auto setup=team_reference_setup();VisualSession v(setup);auto info=ability_details(v.state(),3);check(std::ranges::any_of(info,[](const std::string& line){return line.find("1% caster max HP")!=std::string::npos;}),"ability description");}
void validation_replay(){auto r=rules();r.skills[0].effects[0].amount=10;bool rejected=false;try{Battle b(roster(),r);}catch(const std::invalid_argument&){rejected=true;}check(rejected,"invalid params");Battle a(roster(),rules()),b(roster(),rules());cast(a);cast(b);check(a.snapshot()==b.snapshot(),"replay");}
void sequential_transition(){
    for(const auto carry:{BossRuntimeCarry::KeepCooldownsClearStatuses,BossRuntimeCarry::ResetRuntime,BossRuntimeCarry::PreserveClassifiedStates}){
        std::vector<SquadDefinition> squads;
        for(int i=1;i<=3;++i){BattleRules r;r.skills={{"boss-shield","boss",0,TargetRule::Self,{{EffectKind::StandardShield}}}};squads.push_back({"s"+std::to_string(i),{{"c"+std::to_string(i),{"A"+std::to_string(i),Team::A,1,1,200}}},r});}
        SequentialDefinition definition{"seq",1,squads,{"boss",Team::B,10000,5,100},carry};
        if(carry==BossRuntimeCarry::PreserveClassifiedStates){
            for(auto& squad:definition.squads){squad.rules.skills.clear();}
            for(int i=0;i<3;++i){definition.boss_phases.emplace_back("p"+std::to_string(i),std::vector<SkillDefinition>{{"boss-shield","boss",0,TargetRule::Self,{{EffectKind::StandardShield}}}},BossCooldownTransition::KeepMatching);}
        }
        SequentialSquads stage(definition);
        auto first=choose_basic_attack(stage.battle());check(first && stage.apply(*first).action.accepted(),"sequence opener");
        check(stage.apply({"boss",ActionType::Skill,"boss","boss-shield"}).action.accepted(),"sequence boss cast");
        check(shield(stage.battle().find("boss")),"sequence shield");
        for(int i=0;i<10 && stage.snapshot().squad_index==0;++i){auto action=choose_basic_attack(stage.battle());check(action && stage.apply(*action).action.accepted(),"sequence advance");}
        check(stage.snapshot().squad_index==1 && !shield(stage.battle().find("boss")),"sequence carried standard shield");
    }
}
void shared_front(){
    SharedBossDefinition d{"shared",1,{{"s1",{{"c1",{"A1",Team::A,1000,1,300}}}},{"s2",{{"c2",{"A2",Team::A,1000,1,200}}}},{"s3",{{"c3",{"A3",Team::A,1000,1,100}}}}},{"boss",Team::B,10000,1,50},{},{{"protect",SharedSkillTarget::Self}}};
    d.rules.timing=TimingProfile::ActionBar;d.rules.skills={{"protect","A1",0,TargetRule::Self,{{EffectKind::StandardShield}}}};
    SharedBoss stage(d);check(stage.apply({"A1",ActionType::Skill,"A1","protect"}).accepted(),"shared cast");
    for(int i=0;i<20 && stage.battle().current_actor()->definition.id!="A2";++i){auto action=stage.choose_basic_action();check(action && stage.apply(*action).accepted(),"shared advance");}
    check(stage.battle().current_actor()->definition.id=="A2" && shield(stage.battle().find("A1")),"shared actor/shield");
    auto before=*shield(stage.battle().find("A1"));check(stage.apply({"A2",ActionType::Attack,"boss",{}}).accepted(),"other front action");
    check(shield(stage.battle().find("A1")) && *shield(stage.battle().find("A1"))==before,"front scheduling removed shield");
    stage.reset();check(!shield(stage.battle().find("A1")),"reset retained shield");
}
int main(){try{shield_numeric();independent();self_clock();recipient_clock();reapplied();minimum_and_dead();extra_and_incapacitated();hits_and_defense();death_revive();transition();presentation_contract();validation_replay();sequential_transition();shared_front();std::cout<<"STANDARD_SHIELD_TESTS_RESULT passed=14 total=14\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
