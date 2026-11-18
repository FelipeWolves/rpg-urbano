#pragma once
#include "rpg/battle.hpp"
#include <array>

namespace rpg {
struct FrontDefinition {
    std::string id;
    std::vector<Definition> fighters;
    BattleRules rules;
};
struct ParallelDefinition {
    std::string id;
    std::uint32_t config_version;
    std::vector<FrontDefinition> fronts;
    std::array<std::size_t,3> schedule{0,1,2};
    std::optional<ActionBudget> global_budget=std::nullopt;
};
enum class ParallelEndReason { AllFrontsFinished, TechnicalFailure, GlobalActionLimit, Cancelled };
enum class FrontState { Running, Finished, Cancelled };
enum class ParallelError { None, StageFinished, UnknownFront, NotScheduled, BattleRejected };
struct ParallelSnapshot {
    std::string stage_id;
    std::uint32_t config_version;
    std::vector<std::string> front_ids;
    std::vector<Snapshot> fronts;
    std::array<std::size_t,3> schedule;
    std::optional<std::size_t> current_front;
    std::uint64_t accepted_actions;
    Phase phase;
    std::optional<Team> winner;
    std::optional<ParallelEndReason> end_reason;
    std::optional<ActionBudget> global_budget;
    std::vector<FrontState> front_states;
    bool operator==(const ParallelSnapshot&) const = default;
};
struct ParallelActionResult {
    std::string stage_id;
    std::string front_id;
    ParallelError error=ParallelError::None;
    ActionResult action;
    std::uint64_t stage_action_id=0;
    bool front_finished=false;
    bool stage_finished=false;
    std::vector<std::string> cancelled_fronts;
    [[nodiscard]] bool accepted() const noexcept { return error==ParallelError::None && action.accepted(); }
};
// Cooperative round-robin: no threads, shared actors or shared boss state.
class ParallelFronts {
public:
    explicit ParallelFronts(ParallelDefinition definition);
    [[nodiscard]] const Battle& battle(std::size_t index) const { return battles_.at(index); }
    [[nodiscard]] std::optional<std::size_t> current_front() const noexcept;
    [[nodiscard]] ParallelSnapshot snapshot() const;
    [[nodiscard]] ParallelActionResult apply(std::string_view front,const Action& action);
    [[nodiscard]] std::optional<std::uint64_t> remaining_actions() const noexcept;
    // Cancels the phase between commands; preserves all underlying Battle evidence.
    [[nodiscard]] bool cancel();
    void reset();
private:
    ParallelDefinition definition_;
    std::vector<Battle> battles_;
    std::size_t cursor_=0;
    std::uint64_t actions_=0;
    Phase phase_=Phase::Running;
    std::optional<Team> winner_;
    std::optional<ParallelEndReason> reason_;
    std::vector<FrontState> front_states_;
    void finish(ParallelEndReason reason,std::optional<Team> winner);
    void advance();
};
} // namespace rpg
