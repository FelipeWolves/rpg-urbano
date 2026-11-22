#include "rpg/status_operations.hpp"
#include "rpg/battle_presentation.hpp"
#include <iostream>
#include <stdexcept>
using namespace rpg;
void check(bool x,const char* m){if(!x){throw std::runtime_error(m);}}
const Status* status(const Battle& b,EffectKind k){const auto& list=b.find("A")->statuses;auto s=std::ranges::find(list,k,&Status::kind);return s==list.end()?nullptr:&*s;}
int main(){try{
 BattleRules r;r.combat_model=resolution::CombatModel::FunnelV1;Definition a{"A",Team::A,1000,1,100},b{"B",Team::B,1000,20,90};a.stats.block=0;b.stats.block=0;
 r.skills={{"silver","A",0,TargetRule::Self,{{EffectKind::SilverShield}}},{"standard","A",0,TargetRule::Self,{{EffectKind::StandardShield}}}};
 Battle apply({a,b},r);check(apply.apply({"A",ActionType::Skill,"A","silver"}).accepted()&&status(apply,EffectKind::SilverShield)->magnitude==150&&status(apply,EffectKind::SilverShield)->remaining==2,"silver formula and duration");
 Status silver{EffectKind::SilverShield,2,TickPolicy::NaturalOnly,0,150};silver.border=StatusBorder::Silver;Status standard{EffectKind::StandardShield,3,TickPolicy::NaturalOnly,0,10};
 Battle layers({a,b},r,{},{{"A",100,{standard,silver},{{"silver",0},{"standard",0}}}});check(layers.apply({"A",ActionType::Attack,"B"}).accepted(),"first action");auto hit=layers.apply({"B",ActionType::Attack,"A"});check(hit.accepted()&&!status(layers,EffectKind::StandardShield)&&status(layers,EffectKind::SilverShield)->magnitude==140&&layers.find("A")->health==1000,"standard consumed before silver");
 check(presentation::shield_display(*layers.find("A")).points==140,"display includes silver");
 Status common{EffectKind::Shield,2,TickPolicy::NaturalOnly,0,1};check(permits_operation(common,StatusOperation::Steal,OperationScope::Removable),"common movable");check(permits_operation(silver,StatusOperation::Remove,OperationScope::Removable)&&!permits_operation(silver,StatusOperation::Steal,OperationScope::Removable)&&!permits_operation(silver,StatusOperation::Transfer,OperationScope::Removable),"silver only removable");common.border=StatusBorder::Golden;check(!permits_operation(common,StatusOperation::Remove,OperationScope::Removable),"golden protected");common.border=StatusBorder::Common;common.category=StatusCategory::Structural;check(!permits_operation(common,StatusOperation::Remove,OperationScope::Removable),"structural category independent");
 EffectDefinition cleanse{EffectKind::CleanseDebuffs,3};cleanse.operation_scope=OperationScope::Removable;r.skills={{"cleanse","A",0,TargetRule::Self,{cleanse}}};Status dot{EffectKind::Bleed,3,TickPolicy::EveryTurn,0,10};dot.source_id="B";Battle purified({a,b},r,{},{{"A",100,{dot},{{"cleanse",0}}}});check(purified.apply({"A",ActionType::Skill,"A","cleanse"}).accepted()&&!has_status(*purified.find("A"),EffectKind::Bleed),"cleanse periodic");
 EffectDefinition transfer{EffectKind::TransferDebuffs,3};transfer.irresistible=true;r.skills={{"transfer","A",0,TargetRule::Enemy,{transfer}}};Battle moved({a,b},r,{},{{"A",100,{dot},{{"transfer",0}}}});check(moved.apply({"A",ActionType::Skill,"B","transfer"}).accepted()&&!has_status(*moved.find("A"),EffectKind::Bleed)&&has_status(*moved.find("B"),EffectKind::Bleed),"transfer retains periodic state");
 EffectDefinition remove{EffectKind::DispelBuffs,2};remove.irresistible=true;remove.operation_scope=OperationScope::Removable;r.skills={{"dispel","A",0,TargetRule::Enemy,{remove}}};Battle stripped({a,b},r,{},{{"B",90,{silver},{}}});check(stripped.apply({"A",ActionType::Skill,"B","dispel"}).accepted()&&!has_status(*stripped.find("B"),EffectKind::SilverShield),"silver dispel explicit");
 EffectDefinition heal{EffectKind::Heal,500};r.skills={{"heal","A",0,TargetRule::Ally,{heal}}};Battle restored({a,b},r,{{"A",900},{"B",1000}});check(restored.apply({"A",ActionType::Skill,"A","heal"}).accepted()&&restored.find("A")->health==1000,"heal caps at max");
 std::cout<<"REG_SPECIAL_SHIELD_TESTS_RESULT passed=8 total=8\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
