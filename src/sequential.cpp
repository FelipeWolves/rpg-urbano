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
    std::optional<ActionBudget> budget)
    :id(std::move(stage_id)),config_version(version),squads(std::move(teams)),boss(std::move(boss_definition)),
     boss_carry(carry),global_budget(budget) {}

SequentialSquads::SequentialSquads(SequentialDefinition definition):definition_(std::move(definition)) {
    if(definition_.id.empty() || definition_.config_version==0 || definition_.squads.size()!=3 ||
       definition_.boss.team!=Team::B || (definition_.boss_carry!=BossRuntimeCarry::KeepCooldownsClearStatuses &&
       definition_.boss_carry!=BossRuntimeCarry::ResetRuntime)){throw std::invalid_argument("Invalid sequential configuration");}
    std::vector<std::string> characters,instances,squad_ids;
    for(const auto& squad:definition_.squads){
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
        const Battle validate_local(roster,squad.rules);
        const Battle validate_global(roster,BattleRules{definition_.global_budget});
        if(boss_skills(squad,definition_.boss.id)!=boss_skills(definition_.squads.front(),definition_.boss.id) ||
           squad.rules.timing!=definition_.squads.front().rules.timing){
            throw std::invalid_argument("Base coordinator requires stable boss kit and timing");
        }
    }
    reset();
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
    if(previous_boss && definition_.boss_carry==BossRuntimeCarry::KeepCooldownsClearStatuses){
        runtime.push_back({definition_.boss.id,definition_.boss.speed,{},previous_boss->cooldowns});
    }
    auto rules=squad.rules;const auto remaining=remaining_actions();
    if(remaining && (!rules.action_budget || *remaining<=static_cast<std::uint64_t>(rules.action_budget->max_actions))){
        rules.action_budget=ActionBudget{static_cast<std::int64_t>(*remaining),definition_.global_budget->outcome};
    }
    battle_.emplace(std::move(roster),rules,std::move(health),std::move(runtime));
}
void SequentialSquads::reset(){index_=0;actions_=0;phase_=Phase::Running;winner_.reset();reason_.reset();start_squad(nullptr);}
SequentialSnapshot SequentialSquads::snapshot() const {
    return {definition_.id,definition_.config_version,definition_.squads[index_].id,index_,definition_.boss.id,
            phase_,actions_,winner_,reason_,definition_.boss_carry,battle_->snapshot()};
}
void SequentialSquads::finish(SequentialEndReason reason,std::optional<Team> winner){
    phase_=Phase::Finished;reason_=reason;winner_=winner;
}
SequentialActionResult SequentialSquads::apply(const Action& action){
    SequentialActionResult result{{},definition_.id,definition_.squads[index_].id,index_};
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
        else{const Fighter previous=*boss;++index_;start_squad(&previous);result.squad_advanced=true;}
    }
    result.stage_finished=phase_==Phase::Finished;return result;
}
} // namespace rpg
