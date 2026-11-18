#pragma once
#include "rpg/battle.hpp"

namespace rpg {
struct RegisteredFighter {
    std::string character_id;
    Definition fighter;
};
struct SquadDefinition {
    std::string id;
    std::vector<RegisteredFighter> members;
    BattleRules rules;
};
// Both base policies clear statuses and reset AP/guard/effective speed at a squad boundary.
enum class BossRuntimeCarry { KeepCooldownsClearStatuses, ResetRuntime };
enum class SequentialEndReason { BossDefeated, SquadsExhausted, GlobalActionLimit, TechnicalLimit, MutualElimination };
struct SequentialDefinition {
    std::string id;
    std::uint32_t config_version;
    std::vector<SquadDefinition> squads;
    Definition boss;
    BossRuntimeCarry boss_carry;
    std::optional<ActionBudget> global_budget;
    SequentialDefinition(std::string stage_id, std::uint32_t version, std::vector<SquadDefinition> teams,
                         Definition boss_definition, BossRuntimeCarry carry,
                         std::optional<ActionBudget> budget = std::nullopt);
};
struct SequentialSnapshot {
    std::string stage_id;
    std::uint32_t config_version;
    std::string squad_id;
    std::size_t squad_index;
    std::string boss_id;
    Phase phase;
    std::uint64_t accepted_actions;
    std::optional<Team> winner;
    std::optional<SequentialEndReason> end_reason;
    BossRuntimeCarry boss_carry;
    Snapshot battle;
    bool operator==(const SequentialSnapshot&) const = default;
};
struct SequentialActionResult {
    ActionResult action;
    std::string stage_id;
    std::string squad_id;
    std::size_t squad_index;
    bool squad_advanced = false;
    bool stage_finished = false;
};
class SequentialSquads {
public:
    explicit SequentialSquads(SequentialDefinition definition);
    [[nodiscard]] const Battle& battle() const noexcept { return *battle_; }
    [[nodiscard]] SequentialSnapshot snapshot() const;
    [[nodiscard]] std::optional<std::uint64_t> remaining_actions() const noexcept;
    [[nodiscard]] SequentialActionResult apply(const Action& action);
    void reset();
private:
    SequentialDefinition definition_;
    std::optional<Battle> battle_;
    std::size_t index_ = 0;
    std::uint64_t actions_ = 0;
    Phase phase_ = Phase::Running;
    std::optional<Team> winner_;
    std::optional<SequentialEndReason> reason_;
    void start_squad(const Fighter* previous_boss);
    void finish(SequentialEndReason reason, std::optional<Team> winner);
};
} // namespace rpg
