#include "rpg/ability_profile.hpp"
#include <algorithm>
#include <stdexcept>

namespace rpg {
namespace {
ActionResult reject(ActionError error){ActionResult result;result.error=error;return result;}
}
ProfileBattle::ProfileBattle(std::vector<Definition> roster,BattleRules rules,CombatProfile profile,
    std::vector<CharacterLoadout> loadouts):roster_(std::move(roster)),rules_(std::move(rules)),
    profile_(profile),loadouts_(std::move(loadouts)) {
    if(profile_!=CombatProfile::Classic && profile_!=CombatProfile::Team){throw std::invalid_argument("Unknown combat profile");}
    if(profile_==CombatProfile::Classic){
        if(!loadouts_.empty()){throw std::invalid_argument("Classic profile does not use team slots");}
    }else{
        rules_.guard_enabled=false;
        if(loadouts_.size()!=roster_.size()){throw std::invalid_argument("Every fighter requires a loadout");}
        std::vector<std::string> owners,assigned,passive_ids;
        for(auto& set:loadouts_){
            if(std::ranges::find(roster_,set.fighter_id,&Definition::id)==roster_.end() ||
               std::ranges::find(owners,set.fighter_id)!=owners.end() || set.slots.empty() || set.slots.size()>3){
                throw std::invalid_argument("Invalid loadout");}
            owners.push_back(set.fighter_id);std::ranges::sort(set.slots,{},&AbilitySlot::position);
            std::uint32_t previous=0;
            bool opening_shield=false;
            for(const auto& entry:set.slots){
                if(entry.position<1 || entry.position>3 || entry.position==previous || entry.name.empty() ||
                   (!entry.active_skill && !entry.passive && entry.passive_note.empty())){throw std::invalid_argument("Invalid ability slot");}
                previous=entry.position;
                if(entry.requirement && (!entry.active_skill || entry.position==1 ||
                   entry.requirement->at_most_percent<1 || entry.requirement->at_most_percent>100)){
                    throw std::invalid_argument("Health requirement needs a non-basic active slot and percentage 1..100");}
                if(entry.passive){
                    const auto& passive=*entry.passive;
                    if(passive.id.empty() || passive.trigger!=PassiveTrigger::BattleStart || passive.shield<=0 ||
                       passive.duration<1 || passive.duration>1000 || opening_shield ||
                       std::ranges::find(passive_ids,passive.id)!=passive_ids.end()){
                        throw std::invalid_argument("Invalid or conflicting opening passive");}
                    opening_shield=true;passive_ids.push_back(passive.id);
                }
                if(entry.active_skill){
                    const auto skill=std::ranges::find(rules_.skills,*entry.active_skill,&SkillDefinition::id);
                    if(skill==rules_.skills.end() || skill->owner!=set.fighter_id ||
                       std::ranges::find(assigned,*entry.active_skill)!=assigned.end()){
                        throw std::invalid_argument("Slot skill missing, repeated or owned by another fighter");}
                    assigned.push_back(*entry.active_skill);
                    if(entry.position==1 && (skill->cooldown!=0 || skill->target!=TargetRule::Enemy ||
                       !std::ranges::any_of(skill->effects,[](const EffectDefinition& e){return e.kind==EffectKind::Damage;}))){
                        throw std::invalid_argument("Basic slot needs available enemy attack with zero cooldown");}
                }
            }
            if(set.slots.front().position!=1 || !set.slots.front().active_skill){throw std::invalid_argument("Basic slot 1 required");}
        }
        if(assigned.size()!=rules_.skills.size()){throw std::invalid_argument("Unassigned hidden skill");}
        std::ranges::sort(loadouts_,{},&CharacterLoadout::fighter_id);
    }
    reset(); // Validates definitions and full skill configuration in the existing resolver.
}
const CharacterLoadout* ProfileBattle::loadout(std::string_view fighter) const {
    const auto found=std::ranges::find(loadouts_,fighter,&CharacterLoadout::fighter_id);
    return found==loadouts_.end()?nullptr:&*found;
}
const AbilitySlot* ProfileBattle::slot(std::string_view fighter,std::uint32_t position) const {
    const auto* set=loadout(fighter);if(!set){return nullptr;}
    const auto found=std::ranges::find(set->slots,position,&AbilitySlot::position);
    return found==set->slots.end()?nullptr:&*found;
}
void ProfileBattle::reset(){
    // Opening passives become initial runtime, before the first opportunity. No action is synthesized.
    Battle fresh(roster_,rules_);
    std::vector<InitialRuntime> runtime;
    std::vector<PassiveActivation> activations;
    for(const auto& fighter:fresh.fighters()){
        InitialRuntime state{fighter.definition.id,fighter.effective_speed,{},fighter.cooldowns};
        if(const auto* set=loadout(fighter.definition.id)){
            for(const auto& entry:set->slots){if(entry.passive){
                const auto& passive=*entry.passive;
                state.statuses.push_back({EffectKind::Shield,passive.duration,TickPolicy::NaturalOnly,0,passive.shield});
                activations.push_back({set->fighter_id,entry.position,passive});
            }}
        }
        runtime.push_back(std::move(state));
    }
    if(!activations.empty()){fresh=Battle(roster_,rules_,{},std::move(runtime));}
    battle_=std::move(fresh);passive_activations_=std::move(activations);
}
ProfileSnapshot ProfileBattle::snapshot() const {return {profile_,loadouts_,battle_->snapshot(),passive_activations_};}
std::vector<AbilityView> ProfileBattle::abilities(std::string_view fighter) const {
    std::vector<AbilityView> views;const auto* set=loadout(fighter);const auto* unit=battle_->find(fighter);
    if(!set || !unit){return views;}
    for(const auto& entry:set->slots){
        const bool condition_met=!entry.requirement ||
            static_cast<std::int64_t>(unit->health)*100 <=
            static_cast<std::int64_t>(unit->definition.max_health)*entry.requirement->at_most_percent;
        int cd=0;if(entry.active_skill){for(const auto& known:unit->cooldowns){if(known.skill==*entry.active_skill){cd=known.remaining;}}}
        auto availability=entry.active_skill?AbilityAvailability::Ready:AbilityAvailability::PassiveOnly;
        if(entry.active_skill){
            if(battle_->phase()==Phase::Finished){availability=AbilityAvailability::Finished;}
            else if(unit->health==0 || std::ranges::any_of(unit->statuses,[](const Status& s){return s.kind==EffectKind::Block;})){
                availability=AbilityAvailability::Incapacitated;
            }else if(cd>0){availability=AbilityAvailability::Cooldown;}
            else if(!condition_met){availability=AbilityAvailability::ConditionBlocked;}
        }
        views.push_back({entry,availability,cd,entry.passive.has_value(),condition_met});
    }
    return views;
}
ActionResult ProfileBattle::apply(const Action& action){
    if(profile_==CombatProfile::Classic){return battle_->apply(action);}
    if(battle_->phase()==Phase::Finished){return reject(ActionError::BattleFinished);}
    if(action.type==ActionType::Attack){
        const auto* basic=slot(action.actor,1);
        if(!basic){return reject(ActionError::UnknownActor);}
        return battle_->apply({action.actor,ActionType::Skill,action.target,*basic->active_skill});
    }
    if(action.type==ActionType::Skill){
        const auto* set=loadout(action.actor);
        if(!set){return reject(ActionError::UnknownActor);}
        if(!std::ranges::any_of(set->slots,[&](const AbilitySlot& entry){return entry.active_skill==action.skill;})){
            return reject(ActionError::UnknownSkill);}
        const auto* actor=battle_->current_actor();
        if(actor && actor->definition.id==action.actor){
            for(const auto& view:abilities(action.actor)){
                if(view.slot.active_skill==action.skill && view.availability==AbilityAvailability::ConditionBlocked){
                    return reject(ActionError::SkillConditionNotMet);}
            }
        }
    }
    return battle_->apply(action);
}
ActionResult ProfileBattle::apply_slot(std::uint32_t position,std::string target){
    if(battle_->phase()==Phase::Finished){return reject(ActionError::BattleFinished);}
    const auto* actor=battle_->current_actor();
    const auto* entry=actor?slot(actor->definition.id,position):nullptr;
    if(!entry || !entry->active_skill){return reject(ActionError::UnknownSkill);}
    return apply({actor->definition.id,ActionType::Skill,std::move(target),*entry->active_skill});
}
std::optional<Action> ProfileBattle::choose_basic_action() const {
    auto action=choose_basic_attack(*battle_);
    if(action && profile_==CombatProfile::Team && action->type==ActionType::Attack){
        const auto* basic=slot(action->actor,1);action->type=ActionType::Skill;action->skill=*basic->active_skill;
    }
    return action;
}
} // namespace rpg
