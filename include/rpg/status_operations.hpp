#pragma once
#include "rpg/battle.hpp"
namespace rpg {
enum class StatusOperation { Remove, Steal, Transfer };
[[nodiscard]] inline bool status_is_buff(const Status& s){return !s.native_passive && (s.kind==EffectKind::Shield || s.kind==EffectKind::StandardShield || s.kind==EffectKind::SilverShield || s.kind==EffectKind::Counter || s.kind==EffectKind::Reflect || s.kind==EffectKind::PreventDeath || s.kind==EffectKind::AutoRevive || s.kind==EffectKind::Invincible || s.kind==EffectKind::DebuffImmunity || s.kind==EffectKind::AttackUp);}
[[nodiscard]] inline bool status_is_debuff(const Status& s){return common_debuff(s.kind)||s.debuff;}
[[nodiscard]] inline bool permits_operation(const Status& s,StatusOperation op,OperationScope scope){
 if(s.native_passive || s.category==StatusCategory::Structural || s.border==StatusBorder::Structural || s.border==StatusBorder::Golden){return false;}
 if(s.border==StatusBorder::Silver){return scope==OperationScope::Removable && op==StatusOperation::Remove;}
 return true;
}
}
