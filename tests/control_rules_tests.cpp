#include "rpg/battle.hpp"
#include <iostream>
#include <stdexcept>
using namespace rpg;
void check(bool ok){if(!ok){throw std::runtime_error("Control contract failed");}}
int main(){try{
 BattleRules rules;rules.combat_model=resolution::CombatModel::FunnelV1;
 Definition a{"A",Team::A,100,10,100},b{"B",Team::B,100,10,90};a.stats.block=0;b.stats.block=0;
 SkillDefinition basic{"basic","A",0,TargetRule::Enemy,{{EffectKind::Damage}}};basic.basic=true;
 rules.skills={basic,{"special","A",0,TargetRule::Enemy,{{EffectKind::Damage}}}};
 InitialRuntime silence{"A",100,{{EffectKind::Silence,2,TickPolicy::NaturalOnly,0}}, {{"basic",0},{"special",0}}};
 Battle silenced({a,b},rules,{}, {silence});const auto before=silenced.snapshot();check(silenced.apply({"A",ActionType::Skill,"B","special"}).error==ActionError::ActorSilenced);check(silenced.snapshot()==before);check(silenced.apply({"A",ActionType::Skill,"B","basic"}).accepted());
 for(auto kind:{EffectKind::Stun,EffectKind::Freeze,EffectKind::Petrify}){
  InitialRuntime state{"A",100,{{kind,1,TickPolicy::NaturalOnly,0}}, {{"basic",0},{"special",0}}};Battle battle({a,b},rules,{}, {state});check(battle.current_actor_blocked());check(battle.apply({"A",ActionType::Attack,"B"}).error==ActionError::ActorBlocked);check(battle.apply({"A",ActionType::Skip}).accepted());check(!has_status(*battle.find("A"),kind));
 }
 Fighter unit{a,100};unit.statuses={{EffectKind::Suppression,2,TickPolicy::NaturalOnly,0}};Status native{EffectKind::PreventDeath,2,TickPolicy::NaturalOnly,0};native.native_passive=true;check(!status_native_enabled(unit,native));native.border=StatusBorder::Golden;check(status_native_enabled(unit,native));native.native_passive=false;native.border=StatusBorder::Common;check(status_native_enabled(unit,native));
 EffectDefinition stun{EffectKind::Stun,0,2};stun.irresistible=true;rules.skills={{"stun","A",0,TargetRule::Enemy,{stun}}};Battle controlled({a,b},rules);check(controlled.apply({"A",ActionType::Skill,"B","stun"}).accepted());check(has_status(*controlled.find("B"),EffectKind::Stun));
 b.stats.debuff_immunity=true;Battle immune({a,b},rules);auto result=immune.apply({"A",ActionType::Skill,"B","stun"});check(result.accepted()&&!has_status(*immune.find("B"),EffectKind::Stun));check(std::ranges::any_of(result.events,[](const Event& e){return e.type==EventType::EffectResisted;}));
 b.stats.debuff_immunity=false;b.immune_effects={EffectKind::Stun};Battle boss({a,b},rules);check(boss.apply({"A",ActionType::Skill,"B","stun"}).accepted()&&!has_status(*boss.find("B"),EffectKind::Stun));
 b.immune_effects.clear();a.stats.miss=10000;rules.skills={{"stun","A",0,TargetRule::Enemy,{{EffectKind::Damage},stun}}};Battle missed({a,b},rules);check(missed.apply({"A",ActionType::Skill,"B","stun"}).accepted()&&!has_status(*missed.find("B"),EffectKind::Stun));
 std::cout<<"REG_CONTROLS_TESTS_RESULT passed=8 total=8\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
