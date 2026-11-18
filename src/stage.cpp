#include "rpg/stage.hpp"
#include <stdexcept>
#include <utility>
#include <algorithm>

namespace rpg {
namespace {
std::optional<Team> outcome_winner(LimitOutcome outcome) {
    if (outcome == LimitOutcome::DefeatA) { return Team::B; }
    if (outcome == LimitOutcome::DefeatB) { return Team::A; }
    return std::nullopt;
}
}

StageDefinition::StageDefinition(std::string stage_id, std::uint32_t version,
    std::vector<Definition> team, std::vector<EncounterDefinition> waves,
    CarryPolicy policy, std::optional<ActionBudget> budget, std::optional<RuntimeCarry> runtime_policy)
    : id(std::move(stage_id)), config_version(version), squad(std::move(team)),
      encounters(std::move(waves)), carry(policy), global_budget(budget), runtime_carry(runtime_policy) {}

Stage::Stage(StageDefinition definition) : definition_(std::move(definition)) {
    if (definition_.id.empty() || definition_.config_version == 0 ||
        definition_.encounters.empty() || definition_.encounters.size() > 5 ||
        (definition_.carry != CarryPolicy::PreserveHealth && definition_.carry != CarryPolicy::RestoreTeam)) {
        throw std::invalid_argument("Invalid stage configuration");
    }
    for (const auto& ally : definition_.squad) {
        if (ally.team != Team::A) { throw std::invalid_argument("Stage squad must be A"); }
    }
    // Validate every encounter up front, including IDs and budgets of later waves.
    for (std::size_t i = 0; i < definition_.encounters.size(); ++i) {
        const auto& encounter = definition_.encounters[i];
        if (encounter.id.empty()) { throw std::invalid_argument("Empty encounter ID"); }
        for (std::size_t j = 0; j < i; ++j) {
            if (definition_.encounters[j].id == encounter.id) { throw std::invalid_argument("Duplicate encounter ID"); }
        }
        auto roster = definition_.squad;
        for (const auto& enemy : encounter.enemies) {
            if (enemy.team != Team::B) { throw std::invalid_argument("Stage enemy must be B"); }
            roster.push_back(enemy);
        }
        if ((encounter.rules.timing == TimingProfile::ActionBar || !encounter.rules.skills.empty()) &&
            (!definition_.runtime_carry || (*definition_.runtime_carry != RuntimeCarry::Preserve &&
                                           *definition_.runtime_carry != RuntimeCarry::Reset))) {
            throw std::invalid_argument("Extended stages require runtime carry policy");
        }
        if (definition_.runtime_carry == RuntimeCarry::Preserve && i > 0) {
            std::vector<SkillDefinition> initial_skills;
            std::vector<SkillDefinition> current_skills;
            for (const auto& ally : definition_.squad) {
                for (const auto& skill : definition_.encounters.front().rules.skills) {
                    if (skill.owner == ally.id) { initial_skills.push_back(skill); }
                }
                for (const auto& skill : encounter.rules.skills) {
                    if (skill.owner == ally.id) { current_skills.push_back(skill); }
                }
            }
            std::ranges::sort(initial_skills, {}, &SkillDefinition::id);
            std::ranges::sort(current_skills, {}, &SkillDefinition::id);
            if (initial_skills != current_skills || encounter.rules.timing != definition_.encounters.front().rules.timing) {
                throw std::invalid_argument("Preserved squad requires consistent skill/timing configuration");
            }
        }
        const Battle validate_local(roster, encounter.rules);
        const Battle validate_global(roster, BattleRules{definition_.global_budget});
    }
    reset();
}

std::optional<std::uint64_t> Stage::remaining_actions() const noexcept {
    if (!definition_.global_budget) { return std::nullopt; }
    return static_cast<std::uint64_t>(definition_.global_budget->max_actions) - actions_;
}

void Stage::start_encounter(const std::vector<Fighter>& previous) {
    const auto& encounter = definition_.encounters[index_];
    auto roster = definition_.squad;
    roster.insert(roster.end(), encounter.enemies.begin(), encounter.enemies.end());
    std::vector<InitialHealth> health;
    for (const auto& fighter : roster) {
        int current = fighter.max_health;
        if (fighter.team == Team::A && !previous.empty() && definition_.carry == CarryPolicy::PreserveHealth) {
            for (const auto& old : previous) {
                if (old.definition.team == Team::A && old.definition.id == fighter.id) { current = old.health; break; }
            }
        }
        health.push_back({fighter.id, current});
    }
    auto rules = encounter.rules;
    const auto remaining = remaining_actions();
    // Equal limits use global outcome; final elimination is still resolved first.
    if (remaining && (!rules.action_budget || *remaining <= static_cast<std::uint64_t>(rules.action_budget->max_actions))) {
        rules.action_budget = ActionBudget{static_cast<std::int64_t>(*remaining), definition_.global_budget->outcome};
    }
    std::vector<InitialRuntime> runtime;
    if (definition_.runtime_carry == RuntimeCarry::Preserve) {
        for (const auto& fighter : previous) {
            if (fighter.definition.team == Team::A) {
                runtime.push_back({fighter.definition.id, fighter.effective_speed, fighter.statuses, fighter.cooldowns});
            }
        }
    }
    battle_.emplace(std::move(roster), rules, std::move(health), std::move(runtime));
}

void Stage::reset() {
    index_ = 0;
    actions_ = 0;
    phase_ = Phase::Running;
    winner_.reset();
    reason_.reset();
    start_encounter({});
}

StageSnapshot Stage::snapshot() const {
    return {definition_.id, definition_.encounters[index_].id, definition_.config_version,
            index_, actions_, phase_, winner_, reason_, battle_->snapshot()};
}

void Stage::finish(StageEndReason reason, std::optional<Team> winner) {
    phase_ = Phase::Finished;
    reason_ = reason;
    winner_ = winner;
}

StageActionResult Stage::apply(const Action& action) {
    StageActionResult result{{}, definition_.id, definition_.encounters[index_].id};
    if (phase_ == Phase::Finished) {
        result.action.error = ActionError::BattleFinished;
        return result;
    }
    result.action = battle_->apply(action);
    if (!result.action.accepted()) { return result; }
    ++actions_;
    const bool won_encounter = battle_->phase() == Phase::Finished && battle_->winner() == Team::A &&
        (battle_->end_reason() == EndReason::Elimination || remaining_actions() != std::optional<std::uint64_t>{0});
    if (battle_->end_reason() == EndReason::TechnicalLimit) {
        finish(StageEndReason::TechnicalLimit, std::nullopt);
    } else if (won_encounter && index_ + 1 == definition_.encounters.size()) {
        finish(StageEndReason::Completed, Team::A);
    } else if (battle_->end_reason() == EndReason::Elimination && battle_->winner() == Team::B) {
        finish(StageEndReason::EncounterFailure, Team::B);
    } else if (remaining_actions() == std::optional<std::uint64_t>{0}) {
        finish(StageEndReason::GlobalActionLimit, outcome_winner(definition_.global_budget->outcome));
    } else if (won_encounter) {
        const bool squad_alive = std::ranges::any_of(battle_->fighters(), [](const Fighter& fighter) {
            return fighter.definition.team == Team::A && fighter.health > 0;
        });
        if (!squad_alive && definition_.carry == CarryPolicy::PreserveHealth) {
            finish(StageEndReason::NoSurvivors, std::nullopt);
            result.stage_finished = true;
            return result;
        }
        const auto previous = battle_->fighters();
        ++index_;
        start_encounter(previous);
        result.encounter_advanced = true;
    } else if (battle_->phase() == Phase::Finished) {
        finish(StageEndReason::EncounterFailure, battle_->winner());
    }
    result.stage_finished = phase_ == Phase::Finished;
    return result;
}
} // namespace rpg
