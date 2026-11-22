#include "rpg/stage.hpp"
#include <iostream>
#include <stdexcept>
using namespace rpg;
void check(bool x,const char* m){if(!x){throw std::runtime_error(m);}}
StageDefinition setup(int enemyPower=100){
 Definition a{"A",Team::A,1000,100,100},p{"P",Team::A,150,1,1},b{"B",Team::B,100,enemyPower,90},c{"C",Team::B,1000,1,90};a.stats.block=0;p.stats.block=0;b.stats.block=0;c.stats.block=0;a.vigil=VigilDefinition{"basic",5,2};
 EffectDefinition gold{EffectKind::PreventDeath,0,8};gold.border=StatusBorder::Golden;EffectDefinition mark{EffectKind::Marker,0,8};mark.debuff=true;
 SkillDefinition basic{"basic","A",0,TargetRule::Enemy,{{EffectKind::Damage}}};basic.basic=true;
 BattleRules first;first.combat_model=resolution::CombatModel::FunnelV1;first.skills={basic,{"setup","A",3,TargetRule::Self,{{EffectKind::SilverShield},{EffectKind::StandardShield},gold,mark}},{"area","B",0,TargetRule::EnemyTeam,{{EffectKind::Damage}}}};
 BattleRules next=first;next.skills.pop_back();next.skills.push_back({"area","C",0,TargetRule::EnemyTeam,{{EffectKind::Damage}}});
 return StageDefinition{"waves",2,{a,p},{{"one",{b},first},{"two",{c},next}},CarryPolicy::PreserveHealth,ActionBudget{100,LimitOutcome::DefeatA},RuntimeCarry::WaveV2};
}
int main(){try{
 auto d=setup();Stage stage(d);check(stage.apply({"A",ActionType::Skill,"A","setup"}).action.accepted(),"setup shields");check(stage.apply({"B",ActionType::Skill,"A","area"}).action.accepted(),"enemy area");check(stage.apply({"P",ActionType::Attack,"B"}).action.accepted(),"ally turn");auto r=stage.apply({"A",ActionType::Skill,"B","basic"});check(r.action.accepted()&&r.encounter_advanced,"next wave");const auto* a=stage.battle().find("A");check(has_status(*a,EffectKind::SilverShield)&&has_status(*a,EffectKind::PreventDeath)&&!has_status(*a,EffectKind::StandardShield)&&!has_status(*a,EffectKind::Marker),"category cleaning");check(a->vigil_charges==4,"golden charges preserved");check(a->cooldowns[1].remaining==1,"cooldown preserved");check(a->ap==0,"AP reset fixed clock");
 Stage hurt(setup(800));check(hurt.apply({"A",ActionType::Skill,"A","setup"}).action.accepted(),"hurt setup");check(hurt.apply({"B",ActionType::Skill,"A","area"}).action.accepted(),"hurt area");r=hurt.apply({"A",ActionType::Skill,"B","basic"});check(r.encounter_advanced&&hurt.battle().find("A")->health==660&&hurt.battle().find("P")->health==0,"30 percent heal; no revive");bool heal=false;for(const auto& event:r.transition_events){if(event.status_kind==EffectKind::Heal){heal=true;}else{check(!heal,"all cleanup precedes healing");}}
 bool rejected=false;try{auto invalid=setup();invalid.config_version=1;Stage bad(invalid);}catch(const std::invalid_argument&){rejected=true;}check(rejected,"explicit version required");
 auto old=setup();old.runtime_carry=RuntimeCarry::Preserve;old.squad[0].vigil.reset();Stage legacy(old);check(legacy.snapshot().config_version==2,"old policy remains selectable");
 std::cout<<"REG_WAVE_TESTS_RESULT passed=7 total=7\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
