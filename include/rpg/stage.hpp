#pragma once
#include "rpg/battle.hpp"

namespace rpg {

enum class CarryPolicy { PreserveHealth, RestoreTeam };
enum class RuntimeCarry { Preserve, Reset, WaveV2 };
enum class StageEndReason { Completed, EncounterFailure, GlobalActionLimit, TechnicalLimit, NoSurvivors };

struct EncounterDefinition {
    std::string id;
    std::vector<Definition> enemies;
    BattleRules rules;
};

struct StageDefinition {
    std::string id;
    std::uint32_t config_version;
    std::vector<Definition> squad;
    std::vector<EncounterDefinition> encounters;
    CarryPolicy carry;
    std::optional<ActionBudget> global_budget;
    // Required for profiles with AP/skills. AP/guard and encounter clocks always reset.
    std::optional<RuntimeCarry> runtime_carry;
    StageDefinition(std::string stage_id, std::uint32_t version, std::vector<Definition> team,
                    std::vector<EncounterDefinition> waves, CarryPolicy policy,
                    std::optional<ActionBudget> budget = std::nullopt,
                    std::optional<RuntimeCarry> runtime_policy = std::nullopt);
};

struct StageSnapshot {
    std::string stage_id;
    std::string encounter_id;
    std::uint32_t config_version;
    std::size_t encounter_index;
    std::uint64_t accepted_actions;
    Phase phase;
    std::optional<Team> winner;
    std::optional<StageEndReason> end_reason;
    Snapshot battle;
    bool operator==(const StageSnapshot&) const = default;
};

// Events belong to the encounter that accepted the action, even after transition.
struct StageActionResult {
    ActionResult action;
    std::string stage_id;
    std::string encounter_id;
    bool encounter_advanced = false;
    bool stage_finished = false;
    std::vector<Event> transition_events;
};

class Stage {
public:
    explicit Stage(StageDefinition definition);
    [[nodiscard]] const Battle& battle() const noexcept { return *battle_; }
    [[nodiscard]] StageSnapshot snapshot() const;
    [[nodiscard]] std::optional<std::uint64_t> remaining_actions() const noexcept;
    [[nodiscard]] StageActionResult apply(const Action& action);
    void reset();
private:
    StageDefinition definition_;
    std::optional<Battle> battle_;
    std::size_t index_ = 0;
    std::uint64_t actions_ = 0;
    Phase phase_ = Phase::Running;
    std::optional<Team> winner_;
    std::optional<StageEndReason> reason_;
    void start_encounter(const std::vector<Fighter>& previous,std::vector<Event>* transition=nullptr);
    void finish(StageEndReason reason, std::optional<Team> winner);
};
} // namespace rpg
