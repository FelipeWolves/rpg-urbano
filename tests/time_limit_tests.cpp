#include "rpg/battle.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

namespace {
using namespace rpg;
void check(bool value, const char* message) {
    if (!value) { throw std::runtime_error(message); }
}
BattleRules rules(std::int64_t limit, LimitOutcome outcome = LimitOutcome::Draw) {
    return {ActionBudget{limit, outcome}};
}
std::vector<Definition> duel() {
    return {{"A", Team::A, 100, 1, 2}, {"B", Team::B, 100, 1, 1}};
}
ActionResult apply(Battle& battle, Action action) {
    auto result = battle.apply(action);
    check(result.accepted(), "valid action rejected");
    return result;
}
void rejected_unchanged(Battle& battle, Action action) {
    const auto before = battle.snapshot();
    const auto result = battle.apply(action);
    check(!result.accepted() && result.events.empty() && battle.snapshot() == before,
          "rejected action changed state or budget");
}
void finished(const Battle& battle, EndReason reason, std::optional<Team> winner) {
    check(battle.phase() == Phase::Finished && !battle.current_actor() &&
          battle.end_reason() == reason && battle.winner() == winner, "wrong finish reason/result");
}

void validates_configuration() {
    for (const auto limit : {std::int64_t{0}, std::int64_t{-1}, std::int64_t{-100}}) {
        bool rejected = false;
        try { Battle battle(duel(), rules(limit)); }
        catch (const std::invalid_argument&) { rejected = true; }
        check(rejected, "nonpositive budget accepted");
    }
    bool rejected = false;
    try { Battle battle(duel(), rules(1, static_cast<LimitOutcome>(99))); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected, "unknown timeout policy accepted");
    Battle maximum(duel(), rules(std::numeric_limits<std::int64_t>::max()));
    check(maximum.remaining_actions() == static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max()),
          "large budget narrowed or overflowed");
}

void default_has_no_budget() {
    Battle battle(duel());
    check(!battle.remaining_actions() && battle.accepted_actions() == 0 && !battle.end_reason(), "wrong default budget");
    for (int i = 0; i < 10; ++i) { apply(battle, {battle.current_actor()->definition.id, ActionType::Guard, {}}); }
    check(battle.phase() == Phase::Running && battle.accepted_actions() == 10 && !battle.remaining_actions(),
          "default battle unexpectedly capped");
}

void boundaries_one_two_and_n() {
    for (const auto limit : {std::int64_t{1}, std::int64_t{2}, std::int64_t{7}, std::int64_t{10}}) {
        Battle battle(duel(), rules(limit));
        for (std::int64_t i = 0; i < limit; ++i) {
            check(battle.phase() == Phase::Running, "budget ended early");
            const auto result = apply(battle, {battle.current_actor()->definition.id, ActionType::Guard, {}});
            check(battle.accepted_actions() == static_cast<std::uint64_t>(i + 1), "wrong accepted count");
            check(battle.remaining_actions() == static_cast<std::uint64_t>(limit - i - 1), "wrong remaining count");
            if (i + 1 == limit) {
                finished(battle, EndReason::ActionLimit, std::nullopt);
                check(result.events.back().end_reason == EndReason::ActionLimit, "finish event lacks timeout reason");
                check(std::ranges::count_if(result.events, [](const Event& e) { return e.type == EventType::BattleFinished; }) == 1,
                      "finish event duplicated");
                check(!std::ranges::any_of(result.events, [](const Event& e) {
                    return e.type == EventType::TurnStarted || e.type == EventType::RoundStarted || e.type == EventType::GuardExpired;
                }), "advanced turn or expired next guard after timeout");
            }
        }
        rejected_unchanged(battle, {"A", ActionType::Attack, "B"});
        check(!choose_basic_attack(battle), "policy returned action after timeout");
    }
}

void invalid_actions_do_not_spend() {
    Battle battle(duel(), rules(3));
    rejected_unchanged(battle, {"A", ActionType::Attack, "missing"});
    rejected_unchanged(battle, {"B", ActionType::Guard, {}});
    check(battle.remaining_actions() == 3, "invalid initial action spent budget");
    apply(battle, {"A", ActionType::Guard, {}});
    rejected_unchanged(battle, {"B", ActionType::Attack, "B"});
    rejected_unchanged(battle, {"B", ActionType::Guard, "A"});
    check(battle.remaining_actions() == 2 && battle.find("A")->guarding, "rejection spent budget or guard");
    apply(battle, {"B", ActionType::Attack, "A"});
    check(battle.accepted_actions() == 2, "derived guard-consumed event spent another unit");
    apply(battle, {"A", ActionType::Guard, {}});
    finished(battle, EndReason::ActionLimit, std::nullopt);
}

void elimination_on_last_action_wins() {
    Battle battle({{"A", Team::A, 10, 10, 2}, {"B", Team::B, 1, 1, 1}}, rules(1, LimitOutcome::DefeatA));
    const auto result = apply(battle, {"A", ActionType::Attack, "B"});
    finished(battle, EndReason::Elimination, Team::A);
    check(battle.remaining_actions() == 0 && battle.accepted_actions() == 1, "last action count wrong");
    check(result.events.back().end_reason == EndReason::Elimination, "timeout overrode elimination");
    check(result.events.size() == 3 && result.events.back().type == EventType::BattleFinished, "extra end/turn event");
}

void early_elimination_keeps_remaining_budget() {
    Battle battle({{"A", Team::A, 10, 10, 2}, {"B", Team::B, 1, 1, 1}}, rules(9));
    apply(battle, {"A", ActionType::Attack, "B"});
    finished(battle, EndReason::Elimination, Team::A);
    check(battle.accepted_actions() == 1 && battle.remaining_actions() == 8, "early elimination spent extra budget");
    rejected_unchanged(battle, {"A", ActionType::Guard, {}});
}

void deadline_prevents_next_lethal_action() {
    Battle battle({{"A", Team::A, 100, 1, 2}, {"B", Team::B, 2, 1, 1}}, rules(2));
    apply(battle, {"A", ActionType::Attack, "B"});
    apply(battle, {"B", ActionType::Attack, "A"});
    finished(battle, EndReason::ActionLimit, std::nullopt);
    check(battle.find("B")->health == 1 && battle.round() == 1, "timeout allowed extra action or round");
    rejected_unchanged(battle, {"A", ActionType::Attack, "B"});
}

void explicit_defeat_policies() {
    for (const auto outcome : {LimitOutcome::DefeatA, LimitOutcome::DefeatB}) {
        Battle battle(duel(), rules(1, outcome));
        apply(battle, {"A", ActionType::Guard, {}});
        finished(battle, EndReason::ActionLimit, outcome == LimitOutcome::DefeatA ? Team::B : Team::A);
        check(battle.find("A")->health == 100 && battle.find("B")->health == 100, "timeout falsely killed fighters");
    }
}

void dead_turn_skip_does_not_spend() {
    Battle battle({{"A", Team::A, 100, 10, 30}, {"B1", Team::B, 1, 1, 20}, {"B2", Team::B, 100, 1, 10}}, rules(3));
    apply(battle, {"A", ActionType::Attack, "B1"});
    check(battle.current_actor()->definition.id == "B2" && battle.accepted_actions() == 1, "dead skipped turn counted");
    rejected_unchanged(battle, {"B1", ActionType::Guard, {}});
    apply(battle, {"B2", ActionType::Guard, {}});
    check(battle.round() == 2 && battle.accepted_actions() == 2, "round transition counted as action");
    apply(battle, {"A", ActionType::Guard, {}});
    finished(battle, EndReason::ActionLimit, std::nullopt);
}

void budgets_are_local_and_fresh_battle_resets() {
    const auto config = rules(1);
    Battle first(duel(), config);
    Battle second(duel(), config);
    const auto untouched = second.snapshot();
    apply(first, {"A", ActionType::Guard, {}});
    check(second.snapshot() == untouched, "one battle spent another's budget");
    Battle fresh(duel(), config);
    check(fresh.snapshot() == second.snapshot() && fresh.accepted_actions() == 0, "new battle inherited old counters");
}

void full_reference_deadline_and_replay() {
    for (const auto limit : {std::int64_t{34}, std::int64_t{35}}) {
        auto execute = [limit](std::vector<Definition> definitions) {
            Battle battle(std::move(definitions), rules(limit));
            std::vector<ActionResult> trace;
            while (battle.phase() == Phase::Running) {
                check(trace.size() < 36, "bounded reference did not finish");
                const auto action = choose_basic_attack(battle);
                check(action.has_value(), "missing reference action");
                trace.push_back(apply(battle, *action));
                check(battle.accepted_actions() == trace.size(), "event count confused with actions");
            }
            finished(battle, limit == 35 ? EndReason::Elimination : EndReason::ActionLimit,
                     limit == 35 ? std::optional<Team>{Team::A} : std::nullopt);
            check(trace.size() == static_cast<std::size_t>(limit), "reference ended on wrong boundary");
            return std::pair{battle.snapshot(), trace};
        };
        const auto first = execute(reference_roster());
        auto reversed = reference_roster();
        std::ranges::reverse(reversed);
        check(first == execute(std::move(reversed)), "budgeted replay changed with input order");
    }
}
} // namespace

int main() {
    struct Test { const char* name; void (*run)(); };
    const std::vector<Test> tests{
        {"validates_configuration", validates_configuration},
        {"default_has_no_budget", default_has_no_budget},
        {"boundaries_one_two_and_n", boundaries_one_two_and_n},
        {"invalid_actions_do_not_spend", invalid_actions_do_not_spend},
        {"elimination_on_last_action_wins", elimination_on_last_action_wins},
        {"early_elimination_keeps_remaining_budget", early_elimination_keeps_remaining_budget},
        {"deadline_prevents_next_lethal_action", deadline_prevents_next_lethal_action},
        {"explicit_defeat_policies", explicit_defeat_policies},
        {"dead_turn_skip_does_not_spend", dead_turn_skip_does_not_spend},
        {"budgets_are_local_and_fresh_battle_resets", budgets_are_local_and_fresh_battle_resets},
        {"full_reference_deadline_and_replay", full_reference_deadline_and_replay}
    };
    std::size_t passed = 0;
    for (const auto& test : tests) {
        try { test.run(); ++passed; std::cout << "PASS limit_" << test.name << '\n'; }
        catch (const std::exception& error) { std::cerr << "FAIL limit_" << test.name << ": " << error.what() << '\n'; }
    }
    std::cout << "LIMIT_TESTS_RESULT passed=" << passed << " total=" << tests.size() << '\n';
    return passed == tests.size() ? 0 : 1;
}
