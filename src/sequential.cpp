#include "rpg/sequential.hpp"
#include <algorithm>
#include <stdexcept>
#include <utility>

namespace rpg {
namespace {
std::optional<Team> budget_winner(LimitOutcome outcome) {
    if(outcome==LimitOutcome::DefeatA){return Team::B;}
    if(outcome==LimitOutcome::DefeatB){return Team::A;}
    return std::nullopt;
}
std::vector<SkillDefinition> boss_skills(const SquadDefinition& squad,const std::string& id) {
    std::vector<SkillDefinition> skills;
    for(const auto& skill:squad.rules.skills){if(skill.owner==id){skills.push_back(skill);}}
    std::ranges::sort(skills,{},&SkillDefinition::id);
    return skills;
}
}
SequentialDefinition::SequentialDefinition(std::string stage_id,std::uint32_t version,
    std::vector<SquadDefinition> teams,Definition boss_definition,BossRuntimeCarry carry,
    std::optional<ActionBudget> budget,std::vector<BossPhaseDefinition> phases,std::vector<Status> initial_statuses)
    :id(std::move(stage_id)),config_version(version),squads(std::move(teams)),boss(std::move(boss_definition)),
     boss_carry(carry),global_budget(budget),boss_phases(std::move(phases)),boss_initial_statuses(std::move(initial_statuses)) {}

SequentialSquads::SequentialSquads(SequentialDefinition definition):definition_(std::move(definition)) {
    if(definition_.id.empty() || definition_.config_version==0 || definition_.squads.size()!=3 ||
       definition_.boss.team!=Team::B || (definition_.boss_carry!=BossRuntimeCarry::KeepCooldownsClearStatuses &&
       definition_.boss_carry!=BossRuntimeCarry::ResetRuntime && definition_.boss_carry!=BossRuntimeCarry::PreserveClassifiedStates && definition_.boss_carry!=BossRuntimeCarry::PreserveSpecialBordersV2)){
        throw std::invalid_argument("Invalid sequential configuration");}
    const bool profiled=!definition_.boss_phases.empty();
    if((definition_.boss_carry==BossRuntimeCarry::PreserveSpecialBordersV2 && definition_.config_version<2) || (definition_.boss_carry!=BossRuntimeCarry::PreserveSpecialBordersV2 && profiled!=(definition_.boss_carry==BossRuntimeCarry::PreserveClassifiedStates)) ||
       (profiled && definition_.boss_phases.size()!=3)){throw std::invalid_argument("Profiles require explicit classified-state policy");}
    for(std::size_t i=0;i<definition_.boss_phases.size();++i){
        const auto& phase=definition_.boss_phases[i];
        if(phase.id.empty() || (phase.cooldowns!=BossCooldownTransition::KeepMatching &&
            phase.cooldowns!=BossCooldownTransition::ResetAll && phase.cooldowns!=BossCooldownTransition::MapExplicit)){
            throw std::invalid_argument("Invalid boss phase");
        }
        for(std::size_t j=0;j<i;++j){if(definition_.boss_phases[j].id==phase.id){throw std::invalid_argument("Duplicate phase ID");}}
        for(const auto& skill:phase.skills){if(skill.owner!=definition_.boss.id){throw std::invalid_argument("Phase skill owner must be boss");}}
        if((phase.cooldowns!=BossCooldownTransition::MapExplicit || i==0) && !phase.mappings.empty()){
            throw std::invalid_argument("Mapping requires preceding phase and explicit policy");
        }
        for(std::size_t j=0;j<phase.mappings.size();++j){const auto& mapping=phase.mappings[j];
            if(std::ranges::find(phase.skills,mapping.to_skill,&SkillDefinition::id)==phase.skills.end() ||
               std::ranges::find(definition_.boss_phases[i-1].skills,mapping.from_skill,&SkillDefinition::id)==
               definition_.boss_phases[i-1].skills.end()){throw std::invalid_argument("Unknown cooldown mapping");}
            for(std::size_t k=0;k<j;++k){if(phase.mappings[k].to_skill==mapping.to_skill || phase.mappings[k].from_skill==mapping.from_skill){
                throw std::invalid_argument("Cooldown mapping must be one-to-one");}}
        }
    }
    std::vector<std::string> characters,instances,squad_ids;
    for(std::size_t index=0;index<definition_.squads.size();++index){
        const auto& squad=definition_.squads[index];
        if(squad.id.empty() || squad.members.empty() || squad.members.size()>5 ||
           std::ranges::find(squad_ids,squad.id)!=squad_ids.end()){throw std::invalid_argument("Invalid squad");}
        squad_ids.push_back(squad.id);
        std::vector<Definition> roster;
        for(const auto& member:squad.members){
            if(member.character_id.empty() || member.fighter.team!=Team::A || member.fighter.id==definition_.boss.id ||
               std::ranges::find(characters,member.character_id)!=characters.end() ||
               std::ranges::find(instances,member.fighter.id)!=instances.end()){
                throw std::invalid_argument("Duplicate/invalid character or fighter identity");
            }
            characters.push_back(member.character_id);instances.push_back(member.fighter.id);roster.push_back(member.fighter);
        }
        roster.push_back(definition_.boss);
        if(profiled && !boss_skills(squad,definition_.boss.id).empty()){
            throw std::invalid_argument("Profile owns the boss kit, not squad rules");
        }
        const auto rules=effective_rules(index);
        std::vector<InitialRuntime> runtime;
        if(index==0 && !definition_.boss_initial_statuses.empty()){
            std::vector<Cooldown> cooldowns;
            for(const auto& skill:rules.skills){if(skill.owner==definition_.boss.id){cooldowns.push_back({skill.id,0});}}
            runtime.push_back({definition_.boss.id,definition_.boss.speed,definition_.boss_initial_statuses,cooldowns});
        }
        const Battle validate_local(roster,rules,{},runtime);
        if(definition_.global_budget && (definition_.global_budget->max_actions<1 || (definition_.global_budget->outcome!=LimitOutcome::Draw && definition_.global_budget->outcome!=LimitOutcome::DefeatA && definition_.global_budget->outcome!=LimitOutcome::DefeatB))){throw std::invalid_argument("Invalid sequence global budget");}
        if((!profiled && boss_skills(squad,definition_.boss.id)!=boss_skills(definition_.squads.front(),definition_.boss.id)) ||
           squad.rules.timing!=definition_.squads.front().rules.timing){
            throw std::invalid_argument("Base coordinator requires stable boss kit and timing");
        }
    }
    reset();
}
BattleRules SequentialSquads::effective_rules(std::size_t index) const {
    auto rules=definition_.squads[index].rules;
    if(!definition_.boss_phases.empty()){
        const auto& kit=definition_.boss_phases[index].skills;
        rules.skills.insert(rules.skills.end(),kit.begin(),kit.end());
    }
    return rules;
}
std::string SequentialSquads::phase_id(std::size_t index) const {
    return definition_.boss_phases.empty()?"stable":definition_.boss_phases[index].id;
}
std::optional<std::uint64_t> SequentialSquads::remaining_actions() const noexcept {
    if(!definition_.global_budget){return std::nullopt;}
    return static_cast<std::uint64_t>(definition_.global_budget->max_actions)-actions_;
}
void SequentialSquads::start_squad(const Fighter* previous_boss) {
    const auto& squad=definition_.squads[index_];
    std::vector<Definition> roster;std::vector<InitialHealth> health;std::vector<InitialRuntime> runtime;
    for(const auto& member:squad.members){roster.push_back(member.fighter);health.push_back({member.fighter.id,member.fighter.max_health});}
    roster.push_back(definition_.boss);
    health.push_back({definition_.boss.id,previous_boss?previous_boss->health:definition_.boss.max_health});
    auto rules=effective_rules(index_);
    if(!definition_.boss_phases.empty()){
        const auto& phase=definition_.boss_phases[index_];
        std::vector<Cooldown> cooldowns;
        for(const auto& skill:phase.skills){
            int remaining=0;
            if(previous_boss && phase.cooldowns!=BossCooldownTransition::ResetAll){
                std::string from=skill.id;
                if(phase.cooldowns==BossCooldownTransition::MapExplicit){
                    const auto mapping=std::ranges::find(phase.mappings,skill.id,&CooldownMapping::to_skill);
                    from=mapping==phase.mappings.end()?std::string{}:mapping->from_skill;
                }
                const auto old=std::ranges::find(previous_boss->cooldowns,from,&Cooldown::skill);
                if(old!=previous_boss->cooldowns.end()){remaining=old->remaining;}
            }
            cooldowns.push_back({skill.id,remaining});
        }
        std::vector<Status> statuses;
        if(previous_boss){
            for(const auto& status:previous_boss->statuses){if(status.category!=StatusCategory::Temporary || (definition_.boss_carry==BossRuntimeCarry::PreserveSpecialBordersV2 && (status.border!=StatusBorder::Common || status.native_passive))){statuses.push_back(status);}}
        }else{statuses=definition_.boss_initial_statuses;}
        runtime.push_back({definition_.boss.id,definition_.boss.speed,statuses,cooldowns});
    }else if(previous_boss && definition_.boss_carry==BossRuntimeCarry::PreserveSpecialBordersV2){
        auto statuses=previous_boss->statuses;std::erase_if(statuses,[](const Status& state){return state.category!=StatusCategory::Structural && state.border==StatusBorder::Common && !state.native_passive;});
        runtime.push_back({definition_.boss.id,definition_.boss.speed,statuses,previous_boss->cooldowns,definition_.boss.vigil?std::optional<std::uint32_t>{previous_boss->vigil_charges}:std::nullopt});
    }else if(previous_boss && definition_.boss_carry==BossRuntimeCarry::KeepCooldownsClearStatuses){
        runtime.push_back({definition_.boss.id,definition_.boss.speed,{},previous_boss->cooldowns});
    }else if(!previous_boss && !definition_.boss_initial_statuses.empty()){
        std::vector<Cooldown> cooldowns;
        for(const auto& skill:rules.skills){if(skill.owner==definition_.boss.id){cooldowns.push_back({skill.id,0});}}
        runtime.push_back({definition_.boss.id,definition_.boss.speed,definition_.boss_initial_statuses,cooldowns});
    }
    const auto remaining=remaining_actions();
    if(remaining && (!rules.action_budget || *remaining<=static_cast<std::uint64_t>(rules.action_budget->max_actions))){
        rules.action_budget=ActionBudget{static_cast<std::int64_t>(*remaining),definition_.global_budget->outcome};
    }
    battle_.emplace(std::move(roster),rules,std::move(health),std::move(runtime));
}
void SequentialSquads::reset(){index_=0;actions_=0;phase_=Phase::Running;winner_.reset();reason_.reset();start_squad(nullptr);}
SequentialSnapshot SequentialSquads::snapshot() const {
    return {definition_.id,definition_.config_version,definition_.squads[index_].id,index_,definition_.boss.id,
            phase_,actions_,winner_,reason_,definition_.boss_carry,battle_->snapshot(),phase_id(index_)};
}
void SequentialSquads::finish(SequentialEndReason reason,std::optional<Team> winner){
    phase_=Phase::Finished;reason_=reason;winner_=winner;
}
SequentialActionResult SequentialSquads::apply(const Action& action){
    SequentialActionResult result{{},definition_.id,definition_.squads[index_].id,index_};
    result.boss_phase_id=phase_id(index_);
    if(phase_==Phase::Finished){result.action.error=ActionError::BattleFinished;return result;}
    result.action=battle_->apply(action);if(!result.action.accepted()){return result;}
    ++actions_;
    const auto* boss=battle_->find(definition_.boss.id);
    if(battle_->end_reason()==EndReason::TechnicalLimit){finish(SequentialEndReason::TechnicalLimit,std::nullopt);}
    else if(boss->health==0){
        if(battle_->winner()==Team::A){finish(SequentialEndReason::BossDefeated,Team::A);}
        else{finish(SequentialEndReason::MutualElimination,battle_->winner());}
    }
    else if(remaining_actions()==std::optional<std::uint64_t>{0}){
        finish(SequentialEndReason::GlobalActionLimit,budget_winner(definition_.global_budget->outcome));
    }
    else if(battle_->phase()==Phase::Finished){
        if(index_+1==definition_.squads.size()){finish(SequentialEndReason::SquadsExhausted,Team::B);}
        else{const Fighter previous=*boss;++index_;start_squad(&previous);result.squad_advanced=true;
            result.next_boss_phase_id=phase_id(index_);result.boss_phase_changed=result.boss_phase_id!=*result.next_boss_phase_id;}
    }
    result.stage_finished=phase_==Phase::Finished;return result;
}
} // namespace rpg
