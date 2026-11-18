#include "rpg/battle.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using namespace rpg;

void require(bool condition, const char* message) {
    if (!condition) { throw std::runtime_error(message); }
}

ActionResult accepted(Battle& battle, Action action) {
    auto result = battle.apply(action);
    require(result.accepted(), "expected accepted action");
    return result;
}

void rejected_unchanged(Battle& battle, Action action, ActionError expected) {
    const auto before = battle.snapshot();
    const auto result = battle.apply(action);
    require(result.error == expected, "unexpected rejection reason");
    require(result.events.empty(), "rejection emitted events");
    require(battle.snapshot() == before, "rejection mutated state or consumed turn");
}

bool has_event(const ActionResult& result, EventType type) {
    return std::ranges::any_of(result.events, [type](const Event& event) { return event.type == type; });
}

void invariants(const Battle& battle) {
    for (const auto& fighter : battle.fighters()) {
        require(fighter.health >= 0 && fighter.health <= fighter.definition.max_health, "health out of bounds");
    }
    if (battle.phase() == Phase::Running) {
        require(!battle.winner(), "winner before finish");
        require(battle.current_actor() && battle.current_actor()->health > 0, "dead or absent current actor");
    } else {
        require(battle.winner().has_value() && !battle.current_actor(), "invalid finished state");
        for (const auto& fighter : battle.fighters()) {
            require(fighter.definition.team == *battle.winner() || fighter.health == 0, "living loser");
        }
    }
}

void rejects_bad_definitions() {
    auto must_throw = [](std::vector<Definition> definitions) {
        bool threw = false;
        try { Battle invalid(std::move(definitions)); }
        catch (const std::invalid_argument&) { threw = true; }
        require(threw, "bad roster accepted");
    };
    must_throw({});
    must_throw({{"A", Team::A, 1, 1, 1}});
    must_throw({{"same", Team::A, 1, 1, 1}, {"same", Team::B, 1, 1, 1}});
    for (int field = 0; field < 5; ++field) {
        auto roster = reference_roster();
        if (field == 0) { roster[0].id = ""; }
        if (field == 1) { roster[0].max_health = 0; }
        if (field == 2) { roster[0].attack = -1; }
        if (field == 3) { roster[0].speed = 0; }
        if (field == 4) { roster[0].team = static_cast<Team>(99); }
        must_throw(std::move(roster));
    }
}

void initial_order_and_canonical_input() {
    Battle first(reference_roster());
    const std::vector<std::string> expected{"A3", "B3", "A1", "B1", "A2", "B2"};
    require(first.snapshot().living_order == expected && first.round() == 1, "wrong initial order");
    auto reversed = reference_roster();
    std::ranges::reverse(reversed);
    Battle second(std::move(reversed));
    require(first.snapshot() == second.snapshot(), "input order changed battle state");
}

void invalid_actions_are_atomic() {
    Battle battle(reference_roster());
    rejected_unchanged(battle, {"missing", ActionType::Attack, "B3"}, ActionError::UnknownActor);
    rejected_unchanged(battle, {"A1", ActionType::Attack, "B3"}, ActionError::NotActorsTurn);
    rejected_unchanged(battle, {"A3", static_cast<ActionType>(99), "B3"}, ActionError::UnknownAction);
    rejected_unchanged(battle, {"A3", ActionType::Guard, "A3"}, ActionError::UnexpectedGuardTarget);
    rejected_unchanged(battle, {"A3", ActionType::Attack, "missing"}, ActionError::UnknownTarget);
    rejected_unchanged(battle, {"A3", ActionType::Attack, ""}, ActionError::UnknownTarget);
    rejected_unchanged(battle, {"A3", ActionType::Attack, "A1"}, ActionError::FriendlyTarget);
    rejected_unchanged(battle, {"A3", ActionType::Attack, "A3"}, ActionError::FriendlyTarget);
}

void basic_attack_event_and_next_turn() {
    Battle battle(reference_roster());
    const auto result = accepted(battle, {"A3", ActionType::Attack, "B3"});
    require(battle.find("B3")->health == 55, "wrong basic damage");
    Event expected{EventType::Attack, "A3", "B3", 25, 1, {}};
    expected.action_context = ActionContext{1,0,1,"A3",Team::A,ActionType::Attack,"B3",{}};
    expected.hit_index = 1; expected.planned_hits = 1;
    require(result.events.front() == expected, "wrong attack event");
    require(result.events.back().type == EventType::TurnStarted, "missing turn event");
    require(battle.current_actor()->definition.id == "B3", "wrong next actor");
}

void round_and_one_action_per_actor() {
    Battle battle(reference_roster());
    const std::vector<std::string> actors{"A3", "B3", "A1", "B1", "A2", "B2"};
    ActionResult last;
    for (const auto& actor : actors) {
        require(battle.current_actor()->definition.id == actor, "turn repeated or skipped");
        last = accepted(battle, {actor, ActionType::Guard, {}});
    }
    require(battle.round() == 2 && battle.current_actor()->definition.id == "A3", "round not advanced");
    require(has_event(last, EventType::RoundStarted) && has_event(last, EventType::GuardExpired), "missing round/expiry event");
}

void guard_rounds_up_and_only_one_hit() {
    Battle battle({{"A1", Team::A, 100, 1, 30}, {"B1", Team::B, 100, 5, 20}, {"B2", Team::B, 100, 5, 10}});
    accepted(battle, {"A1", ActionType::Guard, {}});
    rejected_unchanged(battle, {"B1", ActionType::Attack, "B2"}, ActionError::FriendlyTarget);
    require(battle.find("A1")->guarding, "invalid command consumed guard");
    const auto first_hit = accepted(battle, {"B1", ActionType::Attack, "A1"});
    require(battle.find("A1")->health == 97 && !battle.find("A1")->guarding, "guard odd rounding/consumption wrong");
    require(has_event(first_hit, EventType::GuardConsumed), "missing guard consumed event");
    accepted(battle, {"B2", ActionType::Attack, "A1"});
    require(battle.find("A1")->health == 92, "guard applied more than once");
}

void guard_even_damage() {
    Battle battle({{"A", Team::A, 100, 1, 2}, {"B", Team::B, 100, 6, 1}});
    accepted(battle, {"A", ActionType::Guard, {}});
    accepted(battle, {"B", ActionType::Attack, "A"});
    require(battle.find("A")->health == 97, "even damage not halved");
}

void guard_expires_at_next_turn() {
    Battle battle({{"A1", Team::A, 100, 1, 30}, {"B1", Team::B, 100, 5, 20}, {"A2", Team::A, 100, 1, 10}});
    accepted(battle, {"A1", ActionType::Guard, {}});
    accepted(battle, {"B1", ActionType::Attack, "A2"});
    require(battle.find("A1")->guarding, "guard expired before defender's turn");
    const auto result = accepted(battle, {"A2", ActionType::Attack, "B1"});
    require(battle.find("A1")->health == 100 && !battle.find("A1")->guarding, "unhit guard did not expire");
    require(has_event(result, EventType::GuardExpired), "missing expiry event");
    accepted(battle, {"A1", ActionType::Attack, "B1"});
    accepted(battle, {"B1", ActionType::Attack, "A1"});
    require(battle.find("A1")->health == 95, "expired guard still reduced damage");
}

void defeat_clamp_skip_and_invalid_dead_target() {
    Battle battle({{"A1", Team::A, 100, 100, 30}, {"B1", Team::B, 10, 1, 20}, {"B2", Team::B, 200, 1, 10}});
    const auto result = accepted(battle, {"A1", ActionType::Attack, "B1"});
    require(battle.find("B1")->health == 0 && result.events.front().amount == 10, "overkill not clamped");
    require(has_event(result, EventType::FighterDefeated), "missing defeated event");
    require(battle.current_actor()->definition.id == "B2", "defeated actor not skipped");
    rejected_unchanged(battle, {"B1", ActionType::Guard, {}}, ActionError::ActorDefeated);
    accepted(battle, {"B2", ActionType::Attack, "A1"});
    rejected_unchanged(battle, {"A1", ActionType::Attack, "B1"}, ActionError::TargetDefeated);
}

void victory_immediate_and_finished_immutable() {
    Battle battle({{"A", Team::A, 10, 20, 2}, {"B", Team::B, 1, 1, 1}});
    const auto result = accepted(battle, {"A", ActionType::Attack, "B"});
    require(battle.winner() == Team::A && battle.phase() == Phase::Finished, "victory missing");
    require(battle.round() == 1 && !battle.current_actor(), "extra turn after victory");
    require(result.events.back().type == EventType::BattleFinished && !has_event(result, EventType::TurnStarted), "finish event order wrong");
    rejected_unchanged(battle, {"A", ActionType::Guard, {}}, ActionError::BattleFinished);
    require(!choose_basic_attack(battle), "policy generated action after finish");
    invariants(battle);
}

void max_integer_damage_without_overflow() {
    constexpr int maximum = std::numeric_limits<int>::max();
    Battle battle({{"A", Team::A, maximum, 1, 2}, {"B", Team::B, maximum, maximum, 1}});
    accepted(battle, {"A", ActionType::Guard, {}});
    accepted(battle, {"B", ActionType::Attack, "A"});
    require(battle.find("A")->health == maximum / 2, "INT_MAX guard rounding overflow");
    invariants(battle);
}

void opponent_target_tie_and_lowest_health() {
    Battle battle({{"B", Team::B, 100, 1, 30}, {"A2", Team::A, 50, 1, 10}, {"A1", Team::A, 50, 1, 20}});
    require(choose_basic_attack(battle)->target == "A1", "policy target ID tie wrong");
    accepted(battle, *choose_basic_attack(battle));
    accepted(battle, {"A1", ActionType::Guard, {}});
    accepted(battle, {"A2", ActionType::Guard, {}});
    require(choose_basic_attack(battle)->target == "A1", "policy did not choose lowest current health");
}

struct Run { Snapshot state; std::vector<ActionResult> trace; };
Run run_reference(std::vector<Definition> roster) {
    Battle battle(std::move(roster));
    std::vector<ActionResult> trace;
    while (battle.phase() == Phase::Running && trace.size() < 256) {
        const auto action = choose_basic_attack(battle);
        require(action.has_value(), "missing policy action");
        trace.push_back(accepted(battle, *action));
        invariants(battle);
    }
    require(battle.phase() == Phase::Finished, "reference simulation exceeded safety limit");
    return {battle.snapshot(), std::move(trace)};
}

void complete_battle_golden_and_replay() {
    const auto first = run_reference(reference_roster());
    require(first.state.winner == Team::A && first.state.round == 12 && first.trace.size() == 35, "reference battle differs from hand-checked outcome");
    for (const auto& fighter : first.state.fighters) {
        require(fighter.health == (fighter.definition.id == "A2" ? 50 : 0), "reference final health wrong");
    }
    auto reversed = reference_roster();
    std::ranges::reverse(reversed);
    const auto replay = run_reference(std::move(reversed));
    require(first.state == replay.state && first.trace == replay.trace, "replay not deterministic");
}
} // namespace

int main() {
    struct Test { const char* name; void (*run)(); };
    const std::vector<Test> tests{
        {"rejects_bad_definitions", rejects_bad_definitions},
        {"initial_order_and_canonical_input", initial_order_and_canonical_input},
        {"invalid_actions_are_atomic", invalid_actions_are_atomic},
        {"basic_attack_event_and_next_turn", basic_attack_event_and_next_turn},
        {"round_and_one_action_per_actor", round_and_one_action_per_actor},
        {"guard_rounds_up_and_only_one_hit", guard_rounds_up_and_only_one_hit},
        {"guard_even_damage", guard_even_damage},
        {"guard_expires_at_next_turn", guard_expires_at_next_turn},
        {"defeat_clamp_skip_and_invalid_dead_target", defeat_clamp_skip_and_invalid_dead_target},
        {"victory_immediate_and_finished_immutable", victory_immediate_and_finished_immutable},
        {"max_integer_damage_without_overflow", max_integer_damage_without_overflow},
        {"opponent_target_tie_and_lowest_health", opponent_target_tie_and_lowest_health},
        {"complete_battle_golden_and_replay", complete_battle_golden_and_replay}
    };
    std::size_t passed = 0;
    for (const auto& test : tests) {
        try { test.run(); ++passed; std::cout << "PASS " << test.name << '\n'; }
        catch (const std::exception& error) { std::cerr << "FAIL " << test.name << ": " << error.what() << '\n'; }
    }
    std::cout << "TESTS_RESULT passed=" << passed << " total=" << tests.size() << '\n';
    return passed == tests.size() ? 0 : 1;
}
