#include "rpg/stage.hpp"
#include <iostream>
#include <stdexcept>
#include <utility>

namespace {
using namespace rpg;
void check(bool value, const char* message) { if (!value) { throw std::runtime_error(message); } }
template<class F> void invalid(F function) {
    bool caught = false;
    try { function(); } catch (const std::invalid_argument&) { caught = true; }
    check(caught, "invalid configuration accepted");
}
StageDefinition setup(int count, CarryPolicy carry = CarryPolicy::PreserveHealth,
                      std::optional<ActionBudget> budget = std::nullopt) {
    std::vector<EncounterDefinition> waves;
    for (int i = 0; i < count; ++i) {
        waves.push_back({"wave-" + std::to_string(i), {{"enemy", Team::B, 2, 3, 3}}, {}});
    }
    return {"stage", 1, {{"hero", Team::A, 100, 2, 2}}, waves, carry, budget};
}
StageActionResult step(Stage& stage) {
    const auto action = choose_basic_attack(stage.battle());
    check(action.has_value(), "no valid action");
    auto result = stage.apply(*action);
    check(result.action.accepted(), "valid stage action rejected");
    return result;
}
void run(Stage& stage) {
    for (int i = 0; i < 100 && stage.snapshot().phase == Phase::Running; ++i) { step(stage); }
    check(stage.snapshot().phase == Phase::Finished, "stage did not finish");
}
void configurations() {
    for (const int count : {0, 6}) { invalid([&] { Stage stage(setup(count)); }); }
    auto d = setup(2); d.encounters[1].id = d.encounters[0].id;
    invalid([&] { Stage stage(d); });
    d = setup(2); d.encounters[1].enemies[0].id = "hero";
    invalid([&] { Stage stage(d); });
    d = setup(2); d.encounters[1].rules.action_budget = ActionBudget{0, LimitOutcome::Draw};
    invalid([&] { Stage stage(d); });
    invalid([] { Stage stage(setup(1, CarryPolicy::PreserveHealth, ActionBudget{-1, LimitOutcome::Draw})); });
    d = setup(1); d.config_version = 0;
    invalid([&] { Stage stage(d); });
    d = setup(1); d.carry = static_cast<CarryPolicy>(99);
    invalid([&] { Stage stage(d); });
    d = setup(1); d.squad.clear();
    invalid([&] { Stage stage(d); });
}
void health_validation() {
    const auto d = setup(1);
    auto roster = d.squad; roster.push_back(d.encounters[0].enemies[0]);
    for (const auto& state : std::vector<std::vector<InitialHealth>>{
        {{"hero", 100}}, {{"hero", 101}, {"enemy", 2}}, {{"hero", -1}, {"enemy", 2}},
        {{"hero", 0}, {"enemy", 2}}, {{"hero", 100}, {"enemy", 0}},
        {{"hero", 100}, {"hero", 2}}}) {
        invalid([&] { Battle battle(roster, {}, state); });
    }
}
void sequences() {
    for (const int count : {1, 2, 5}) {
        Stage stage(setup(count));
        int finishes = 0; int advances = 0;
        while (stage.snapshot().phase == Phase::Running) {
            const auto before = stage.snapshot();
            const auto result = step(stage);
            check(result.stage_id == "stage" && result.encounter_id == before.encounter_id, "event scope lost");
            finishes += result.stage_finished ? 1 : 0;
            advances += result.encounter_advanced ? 1 : 0;
            if (result.encounter_advanced) {
                check(stage.snapshot().encounter_index == before.encounter_index + 1, "skipped wave");
                check(stage.battle().accepted_actions() == 0 && stage.battle().round() == 1, "local clock not reset");
            }
        }
        const auto state = stage.snapshot();
        check(finishes == 1 && advances == count - 1 && state.end_reason == StageEndReason::Completed &&
              state.winner == Team::A && state.accepted_actions == static_cast<std::uint64_t>(count * 2), "bad completion");
        check(stage.battle().find("hero")->health == 100 - count * 3 &&
              stage.battle().find("hero")->definition.max_health == 100, "health persistence failed");
        const auto result = stage.apply({"hero", ActionType::Guard, {}});
        check(!result.action.accepted() && !result.stage_finished && result.action.events.empty() && stage.snapshot() == state,
              "duplicate completion or mutation after finish");
    }
}
void fallen_and_restore() {
    for (const auto policy : {CarryPolicy::PreserveHealth, CarryPolicy::RestoreTeam}) {
        auto d = setup(2, policy);
        d.squad.push_back({"fragile", Team::A, 1, 1, 1});
        Stage stage(d);
        check(stage.apply({"enemy", ActionType::Attack, "fragile"}).action.accepted(), "kill rejected");
        check(step(stage).encounter_advanced, "no transition");
        const auto* fallen = stage.battle().find("fragile");
        check(fallen->health == (policy == CarryPolicy::PreserveHealth ? 0 : 1) && fallen->definition.max_health == 1,
              "resurrection policy incorrect");
        check(stage.battle().snapshot().living_order.size() == (policy == CarryPolicy::PreserveHealth ? 2u : 3u),
              "dead fighter scheduled");
    }
    Stage restored(setup(2, CarryPolicy::RestoreTeam));
    step(restored); step(restored);
    check(restored.battle().find("hero")->health == 100, "explicit restoration failed");
}
void guard_cleared() {
    auto d = setup(2);
    d.squad = {{"guard", Team::A, 100, 1, 5}, {"killer", Team::A, 100, 2, 4}};
    Stage stage(d);
    check(stage.apply({"guard", ActionType::Guard, {}}).action.accepted(), "guard rejected");
    check(step(stage).encounter_advanced, "no guarded transition");
    check(!stage.battle().find("guard")->guarding, "guard leaked to next wave");
}
void defeat_middle() {
    auto d = setup(5); d.encounters[1].enemies[0].attack = 100;
    Stage stage(d); run(stage);
    check(stage.snapshot().encounter_index == 1 && stage.snapshot().winner == Team::B &&
          stage.snapshot().end_reason == StageEndReason::EncounterFailure, "middle defeat skipped");
}
void global_boundaries() {
    for (const int budget : {1, 2, 3, 4}) {
        Stage stage(setup(2, CarryPolicy::PreserveHealth, ActionBudget{budget, LimitOutcome::Draw}));
        run(stage);
        const auto state = stage.snapshot();
        check(state.accepted_actions == static_cast<std::uint64_t>(budget) && stage.remaining_actions() == 0, "global budget reset");
        check(state.end_reason == (budget == 4 ? StageEndReason::Completed : StageEndReason::GlobalActionLimit),
              "wrong global boundary precedence");
        check(state.encounter_index == (budget <= 2 ? 0u : 1u), "started wave without budget");
    }
    auto d = setup(1, CarryPolicy::PreserveHealth, ActionBudget{1, LimitOutcome::DefeatB});
    d.encounters[0].enemies[0].attack = 100;
    Stage stage(d); run(stage);
    check(stage.snapshot().winner == Team::B && stage.snapshot().end_reason == StageEndReason::EncounterFailure,
          "global policy overrode actual elimination");
}
void local_global() {
    for (const auto policy : {LimitOutcome::Draw, LimitOutcome::DefeatA, LimitOutcome::DefeatB}) {
        auto d = setup(2, CarryPolicy::PreserveHealth, ActionBudget{3, policy});
        d.encounters[0].rules.action_budget = ActionBudget{1, LimitOutcome::DefeatB};
        Stage stage(d); run(stage);
        check(stage.snapshot().end_reason == StageEndReason::Completed && stage.snapshot().accepted_actions == 3,
              "local victory did not advance or final victory lost");
        d = setup(2, CarryPolicy::PreserveHealth, ActionBudget{1, policy});
        d.encounters[0].rules.action_budget = ActionBudget{1, LimitOutcome::DefeatA};
        Stage tied(d); run(tied);
        const auto winner = policy == LimitOutcome::Draw ? std::optional<Team>{} :
            std::optional<Team>{policy == LimitOutcome::DefeatA ? Team::B : Team::A};
        check(tied.snapshot().end_reason == StageEndReason::GlobalActionLimit && tied.snapshot().winner == winner,
              "equal budget precedence wrong");
    }
    auto d = setup(5); d.encounters[0].rules.action_budget = ActionBudget{1, LimitOutcome::Draw};
    Stage stage(d); run(stage);
    check(stage.snapshot().encounter_index == 0 && !stage.snapshot().winner, "local draw advanced");
}
void invalid_reset_replay() {
    Stage first(setup(5, CarryPolicy::PreserveHealth, ActionBudget{20, LimitOutcome::Draw}));
    Stage second(setup(5, CarryPolicy::PreserveHealth, ActionBudget{20, LimitOutcome::Draw}));
    const auto initial = first.snapshot();
    check(!first.apply({"missing", ActionType::Attack, "enemy"}).action.accepted() && first.snapshot() == initial,
          "invalid action consumed budget");
    while (first.snapshot().phase == Phase::Running) {
        const auto a = step(first); const auto b = step(second);
        check(a.action == b.action && a.encounter_id == b.encounter_id && first.snapshot() == second.snapshot(), "replay mismatch");
    }
    first.reset(); check(first.snapshot() == initial, "reset did not restore initial state");
    step(first); first.reset(); check(first.snapshot() == initial, "running reset failed");
    check(second.snapshot().phase == Phase::Finished, "reset leaked between stages");
}
}
int main() {
    const std::pair<const char*, void(*)()> tests[] = {
        {"configuration", configurations}, {"initial_health", health_validation}, {"sequences_1_2_5", sequences},
        {"fallen_restore", fallen_and_restore}, {"guard_reset", guard_cleared}, {"middle_defeat", defeat_middle},
        {"global_boundaries", global_boundaries}, {"local_global", local_global}, {"reset_replay", invalid_reset_replay}};
    int passed = 0;
    for (const auto& [name, test] : tests) {
        try { test(); ++passed; std::cout << "PASS stage " << name << '\n'; }
        catch (const std::exception& error) { std::cerr << "FAIL stage " << name << ": " << error.what() << '\n'; }
    }
    std::cout << "STAGE_TESTS_RESULT passed=" << passed << " total=" << std::size(tests) << '\n';
    return passed == static_cast<int>(std::size(tests)) ? 0 : 1;
}
