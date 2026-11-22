#include "rpg/battle_presentation.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
namespace rpg::presentation {
AbilityCard ability_card(const VisualState& state,std::uint32_t position){
    AbilityCard card{"HABILIDADE","Indisponível","Esta posição não possui habilidade."};
    const auto found=std::ranges::find(state.abilities,position,[](const AbilityView& a){return a.slot.position;});
    if(found==state.abilities.end()){return card;}
    const auto& ability=*found;
    card.kind=ability.slot.active_skill?(position==1?"BASICA":"HABILIDADE"):"PASSIVA";
    if(!ability.slot.active_skill){
        card.badge=AbilityBadge::Passive;card.status="Passiva";
        card.reason=ability.passive_implemented?"Passiva automática; não é uma ação selecionável.":"Informação passiva; gatilho ainda não implementado.";
        return card;
    }
    card.cooldown=std::max(0,ability.cooldown);
    if(state.combat.profile==CombatProfile::Classic){card.reason="O perfil clássico utiliza seus comandos próprios.";return card;}
    if(state.combat.battle.phase!=Phase::Running){card.reason="O combate terminou.";return card;}
    if(state.stopped){card.reason="Interação parada para segurança ou inspeção.";return card;}
    const auto actor=std::ranges::find(state.combat.battle.fighters,state.combat.battle.current_actor.value_or(""),[](const Fighter& f){return f.definition.id;});
    if(actor==state.combat.battle.fighters.end()){card.reason="Não há personagem ativo.";return card;}
    if(state.automatic || actor->definition.team!=Team::A){card.reason="Aguarde o turno da sua equipe.";return card;}
    if(actor->health<=0 || ability.availability==AbilityAvailability::Incapacitated || std::ranges::any_of(actor->statuses,[](const Status& s){return s.kind==EffectKind::Block;})){card.reason="Personagem impossibilitado de agir.";return card;}
    const auto skill=std::ranges::find(state.combat.battle.rules.skills,*ability.slot.active_skill,&SkillDefinition::id);
    if(skill==state.combat.battle.rules.skills.end() || skill->owner!=actor->definition.id){card.reason="Habilidade não disponível para o personagem ativo.";return card;}
    if(ability.availability==AbilityAvailability::Finished){card.reason="O combate terminou.";return card;}
    if(ability.availability==AbilityAvailability::Cooldown || card.cooldown>0){
        card.badge=AbilityBadge::Cooldown;card.status="Em recarga";card.reason="Aguarde a recarga: "+std::to_string(card.cooldown)+" turno(s).";return card;
    }
    if(ability.availability==AbilityAvailability::ConditionBlocked || !ability.condition_met){
        card.badge=AbilityBadge::Locked;card.status="Condição";
        card.reason=ability.slot.requirement?"Requer PV em até "+std::to_string(ability.slot.requirement->at_most_percent)+"% do máximo.":"A condição de uso não foi atendida.";
        return card;
    }
    if(ability.availability!=AbilityAvailability::Ready){card.reason="Estado da habilidade indisponível.";return card;}
    card.badge=AbilityBadge::Ready;card.status="Pronta";card.reason="Selecione a habilidade e um alvo válido; confirme a ação.";card.selectable=true;
    return card;
}
ShieldDisplay shield_display(const Fighter& fighter){
    std::int64_t shield=0;
    for(const auto& status:fighter.statuses){if(status.kind==EffectKind::StandardShield || status.kind==EffectKind::Shield || status.kind==EffectKind::StructuralShield){shield+=std::max<numeric::Quantity>(0,status.magnitude);}}
    if(fighter.definition.max_health<=0){throw std::invalid_argument("Positive max HP required");}
    const numeric::Quantity health=std::clamp<numeric::Quantity>(fighter.health,0,fighter.definition.max_health);
    return {shield,static_cast<double>(health)/fighter.definition.max_health,
        static_cast<double>(std::min<std::int64_t>(shield,health))/fighter.definition.max_health};
}
std::string effect_name(EffectKind kind){
    switch(kind){
    case EffectKind::StandardShield:return "Standard shield";case EffectKind::Shield:return "Shield";case EffectKind::StructuralShield:return "Structural shield";
    case EffectKind::Block:return "Unable to act";case EffectKind::FreezeCooldowns:return "Cooldowns frozen";
    case EffectKind::Reflect:return "Reflect";case EffectKind::DeathExplosion:return "On-death explosion";
    case EffectKind::PreventDeath:return "Death prevention";case EffectKind::AutoRevive:return "Auto-revive";
    case EffectKind::Counter:return "Counter";case EffectKind::Marker:return "Marker";
    case EffectKind::Damage:return "Damage";case EffectKind::ChangeAP:return "AP change";
    case EffectKind::SetSpeed:return "Speed";case EffectKind::ExtraTurn:return "Extra turn";
    }
    return "Effect";
}
std::vector<std::string> effect_details(const Fighter& fighter){
    std::vector<std::string> lines;
    if(fighter.guarding){lines.push_back("Defending: incoming hit damage is halved, rounded down.");}
    for(const auto& effect:fighter.statuses){
        lines.push_back(effect_name(effect.kind)+" | value "+std::to_string(effect.magnitude)+" | charges "+std::to_string(effect.charges));
        if(effect.category==StatusCategory::Permanent){lines.push_back("  Permanent");}
        else {lines.push_back("  Remaining: "+std::to_string(effect.remaining)+(effect.clock==TickPolicy::NaturalOnly?" natural turns":" all owner turns"));}
    }
    if(lines.empty()){lines.push_back("No active effects.");}
    return lines;
}
std::vector<std::string> target_candidates(const VisualState& state,std::uint32_t position){
    const auto& battle=state.combat.battle;
    if(state.automatic || state.stopped || battle.phase!=Phase::Running || !battle.current_actor){return {};}
    if(state.combat.profile==CombatProfile::Classic){
        if(position!=1){return {};}
        const auto actor=std::ranges::find(battle.fighters,*battle.current_actor,[](const Fighter& f){return f.definition.id;});
        if(actor==battle.fighters.end() || actor->health<=0 || std::ranges::any_of(actor->statuses,[](const Status& s){return s.kind==EffectKind::Block;})){return {};}
        std::vector<std::string> ids;
        for(const auto& fighter:battle.fighters){if(fighter.health>0 && fighter.definition.team!=actor->definition.team){ids.push_back(fighter.definition.id);}}
        return ids;
    }
    const auto ability=std::ranges::find(state.abilities,position,[](const AbilityView& v){return v.slot.position;});
    if(ability==state.abilities.end() || ability->availability!=AbilityAvailability::Ready || !ability->slot.active_skill){return {};}
    const auto actor=std::ranges::find(battle.fighters,*battle.current_actor,[](const Fighter& f){return f.definition.id;});
    const auto skill=std::ranges::find(battle.rules.skills,*ability->slot.active_skill,&SkillDefinition::id);
    if(actor==battle.fighters.end() || actor->health<=0 || skill==battle.rules.skills.end()){return {};}
    std::vector<std::string> ids;
    for(const auto& fighter:battle.fighters){
        if(fighter.health<=0){continue;}
        const bool same=fighter.definition.team==actor->definition.team;
        if((skill->target==TargetRule::Self && fighter.definition.id==actor->definition.id) ||
           (skill->target==TargetRule::Enemy && !same) || (skill->target==TargetRule::Ally && same)){ids.push_back(fighter.definition.id);}
    }
    return ids;
}
std::vector<std::string> ability_details(const VisualState& state,std::uint32_t position){
    if(state.combat.profile==CombatProfile::Classic){
        if(position==1){return {"Attack","Choose a living enemy. Uses the existing attack rules."};}
        if(position==2){return {"Defense","Existing defense: halves each incoming hit, rounded down.","Expires at the next owner opportunity according to the core."};}
        if(position==4){return {"Pass","Only legal while the current actor is unable to act."};}
        std::vector<std::string> out={"Classic skills"};
        for(const auto& skill:state.combat.battle.rules.skills){if(state.combat.battle.current_actor==skill.owner){out.push_back(skill.id+" | cooldown "+std::to_string(skill.cooldown));}}
        if(out.size()==1){out.push_back("No special skills registered in this reference encounter.");}
        return out;
    }
    const auto ability=std::ranges::find(state.abilities,position,[](const AbilityView& v){return v.slot.position;});
    if(ability==state.abilities.end()){return {"Ability unavailable in this view."};}
    const auto& slot=ability->slot;
    std::vector<std::string> out={slot.name};
    if(slot.level){out.push_back("Level: "+std::to_string(*slot.level));}
    if(!slot.description.empty()){out.push_back(slot.description);}
    if(slot.active_skill){
        const auto skill=std::ranges::find(state.combat.battle.rules.skills,*slot.active_skill,&SkillDefinition::id);
        if(skill!=state.combat.battle.rules.skills.end()){
            out.push_back(std::string("Target: ")+(skill->target==TargetRule::Self?"self":skill->target==TargetRule::Ally?"living ally":"living enemy"));
            out.push_back("Cooldown: "+std::to_string(skill->cooldown)+" | remaining "+std::to_string(ability->cooldown));
            for(const auto& effect:skill->effects){
                auto detail=effect_name(effect.kind);
                if(effect.kind==EffectKind::StandardShield){detail+=": 1% caster max HP, rounded down, minimum 1 point; 3 recipient natural turns; keep greater points/duration.";}
                else if(effect.kind==EffectKind::Damage){detail+=": attack power, hits "+std::to_string(effect.hit_count);}
                else{detail+=" | value "+std::to_string(effect.amount)+" | duration "+std::to_string(effect.duration);}
                out.push_back(detail);
            }
        }
    }
    if(slot.passive){out.push_back("Battle start: shield "+std::to_string(slot.passive->shield)+" points, duration "+std::to_string(slot.passive->duration));}
    if(!slot.passive_note.empty()){out.push_back(slot.passive_note);}
    if(slot.requirement){out.push_back("Requires owner HP <= "+std::to_string(slot.requirement->at_most_percent)+"%.");}
    const auto card=ability_card(state,position);
    out.push_back("Estado: "+card.status+". "+card.reason);
    if(card.cooldown>0){out.push_back("Recarga restante: "+std::to_string(card.cooldown));}
    return out;
}
std::string action_error_message(ActionError error){
    switch(error){
    case ActionError::None:return "Action accepted.";
    case ActionError::BattleFinished:return "The battle has ended.";
    case ActionError::UnknownActor:case ActionError::ActorDefeated:case ActionError::NotActorsTurn:return "The acting unit changed or cannot act.";
    case ActionError::UnknownTarget:return "Target not found.";
    case ActionError::FriendlyTarget:return "Choose a living enemy.";
    case ActionError::TargetDefeated:return "The target is defeated.";
    case ActionError::GuardDisabled:return "Defense is not available in this combat profile.";
    case ActionError::UnknownSkill:return "This unit does not have that skill.";
    case ActionError::SkillOnCooldown:return "The skill is still on cooldown.";
    case ActionError::InvalidSkillTarget:return "This skill requires a different target type.";
    case ActionError::ActorBlocked:return "This unit is unable to act; only pass is available.";
    case ActionError::NotBlocked:return "Pass is only legal while unable to act.";
    case ActionError::SkillConditionNotMet:return "The skill condition is not satisfied.";
    case ActionError::UnexpectedGuardTarget:return "Defense does not take a target.";
    case ActionError::UnknownAction:return "Unknown action.";
    }
    return "Action rejected.";
}
void AbilityGesture::press(std::uint32_t position,double seconds,std::uint64_t revision){
    cancel();if(position<1 || position>3 || !std::isfinite(seconds)){return;}
    position_=position;started_=seconds;revision_=revision;
}
std::optional<std::uint32_t> AbilityGesture::hold(double seconds,std::uint64_t revision){
    if(revision!=revision_ || !std::isfinite(seconds) || seconds<started_){cancel();return {};}
    if(position_ && !held_ && seconds-started_>=0.5){held_=true;return position_;}
    return {};
}
std::optional<std::uint32_t> AbilityGesture::release(double seconds,std::uint64_t revision){
    const auto position=position_;
    const bool click=position && !held_ && revision==revision_ && std::isfinite(seconds) && seconds>=started_ && seconds-started_<0.5;
    cancel();return click?std::optional<std::uint32_t>(position):std::nullopt;
}
void AbilityGesture::cancel() noexcept {position_=0;held_=false;}
} // namespace rpg::presentation
