#include "rpg/battle.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {
using namespace rpg;
constexpr std::size_t safety_limit = 4096;
void check(bool condition, const char* message) {
    if (!condition) { throw std::runtime_error(message); }
}

std::vector<Definition> roster(int count_a, int count_b, int profile) {
    std::vector<Definition> definitions;
    for (int side = 0; side < 2; ++side) {
        const int count = side == 0 ? count_a : count_b;
        for (int index = 0; index < count; ++index) {
            const int multiplier = ((profile == 1 && side == 0) || (profile == 2 && side == 1)) ? 3 : 1;
            definitions.push_back({std::string(side == 0 ? "A" : "B") + std::to_string(index + 1),
                side == 0 ? Team::A : Team::B,
                60 + 9 * index, (6 + 3 * index) * multiplier, 7 + 2 * (index % 3)});
        }
    }
    return definitions;
}

void invariants(const Battle& battle) {
    const auto state = battle.snapshot();
    std::size_t alive = 0;
    for (const auto& fighter : state.fighters) {
        check(fighter.health >= 0 && fighter.health <= fighter.definition.max_health, "health outside bounds");
        if (fighter.health == 0) { check(!fighter.guarding, "dead fighter kept guard"); }
        else {
            ++alive;
            check(std::ranges::count(state.living_order, fighter.definition.id) == 1, "living order missing/duplicated ID");
        }
    }
    check(alive == state.living_order.size(), "living order contains dead fighter");
    if (battle.phase() == Phase::Running) {
        check(battle.current_actor() && battle.current_actor()->health > 0 && !battle.winner(), "invalid active battle");
    } else {
        check(!battle.current_actor() && battle.winner().has_value(), "invalid finished battle");
        bool winner_alive = false;
        for (const auto& fighter : state.fighters) {
            check(fighter.definition.team == *battle.winner() || fighter.health == 0, "losing team remains alive");
            winner_alive = winner_alive || (fighter.definition.team == *battle.winner() && fighter.health > 0);
        }
        check(winner_alive, "winner has no survivors");
    }
}

Action choose(const Battle& battle, std::size_t accepted_actions, bool guards) {
    auto action = choose_basic_attack(battle);
    check(action.has_value(), "policy has no valid action");
    if (guards && accepted_actions % 7 == 0) { return {action->actor, ActionType::Guard, {}}; }
    return *action;
}

void invalid_does_not_mutate(Battle& battle) {
    const auto before = battle.snapshot();
    const auto invalid = battle.apply({"not_registered", ActionType::Attack, "missing"});
    check(!invalid.accepted() && invalid.events.empty() && battle.snapshot() == before, "invalid command changed battle");
}

struct Run { Snapshot state; std::vector<ActionResult> trace; };
Run run(std::vector<Definition> definitions, bool guards) {
    Battle battle(std::move(definitions));
    std::vector<ActionResult> trace;
    invalid_does_not_mutate(battle);
    invariants(battle);
    while (battle.phase() == Phase::Running && trace.size() < safety_limit) {
        auto result = battle.apply(choose(battle, trace.size(), guards));
        check(result.accepted(), "scenario rejected valid policy command");
        trace.push_back(std::move(result));
        invariants(battle);
    }
    check(battle.phase() == Phase::Finished, "runner safety limit; not a game timeout");
    invalid_does_not_mutate(battle);
    return {battle.snapshot(), std::move(trace)};
}

void independent_interleaved_contexts() {
    // Repeated fighter IDs across contexts must never imply shared state.
    const std::array formations{roster(5, 1, 0), roster(5, 1, 1), roster(5, 1, 2)};
    std::vector<Run> baselines;
    for (const auto& definitions : formations) { baselines.push_back(run(definitions, true)); }
    for (const bool reverse_schedule : {false, true}) {
        std::array contexts{Battle(formations[0]), Battle(formations[1]), Battle(formations[2])};
        std::array<std::vector<ActionResult>, 3> traces;
        std::size_t total_actions = 0;
        while (std::ranges::any_of(contexts, [](const Battle& b) { return b.phase() == Phase::Running; })) {
            for (std::size_t slot = 0; slot < contexts.size(); ++slot) {
                const std::size_t index = reverse_schedule ? contexts.size() - 1 - slot : slot;
                auto& current = contexts[index];
                if (current.phase() == Phase::Finished) { continue; }
                const std::array before{contexts[0].snapshot(), contexts[1].snapshot(), contexts[2].snapshot()};
                auto result = current.apply(choose(current, traces[index].size(), true));
                check(result.accepted(), "interleaving rejected command");
                traces[index].push_back(std::move(result));
                for (std::size_t other = 0; other < contexts.size(); ++other) {
                    if (other != index) { check(contexts[other].snapshot() == before[other], "battle mutated another context"); }
                }
                invariants(current);
                check(++total_actions < 3 * safety_limit, "interleaving safety limit");
            }
        }
        for (std::size_t i = 0; i < contexts.size(); ++i) {
            check(contexts[i].snapshot() == baselines[i].state && traces[i] == baselines[i].trace,
                  "schedule changed independent battle state/events");
        }
    }
}
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "expected CSV output filename");
        std::ofstream output(argv[1]);
        check(output.is_open(), "cannot open CSV output");
        output << "case_id,count_a,count_b,profile,policy,winner,actions,rounds,replay,invariants\n";
        const std::array<const char*, 3> profiles{"balanced", "stronger_A", "stronger_B"};
        std::size_t cases = 0;
        std::size_t maximum_actions = 0;
        auto verify_case = [&](int a, int b, int profile, bool guards) {
            auto definitions = roster(a, b, profile);
            const auto first = run(definitions, guards);
            std::ranges::reverse(definitions);
            const auto replay = run(std::move(definitions), guards);
            check(first.state == replay.state && first.trace == replay.trace, "scenario replay differs");
            if (a == 1 && b == 1 && !guards && profile != 0) {
                check(first.state.winner == (profile == 1 ? Team::A : Team::B), "asymmetric duel winner wrong");
            }
            maximum_actions = std::max(maximum_actions, first.trace.size());
            output << ++cases << ',' << a << ',' << b << ',' << profiles[static_cast<std::size_t>(profile)] << ','
                   << (guards ? "guard_every_7" : "attack") << ',' << (*first.state.winner == Team::A ? 'A' : 'B') << ','
                   << first.trace.size() << ',' << first.state.round << ",pass,pass\n";
        };
        for (int a = 1; a <= 5; ++a) {
            for (int b = 1; b <= 5; ++b) {
                for (int profile = 0; profile < 3; ++profile) {
                    for (const bool guards : {false, true}) { verify_case(a, b, profile, guards); }
                }
            }
        }
        // Extended probes keep formation policy out of the generic battle kernel.
        for (const auto& [a, b] : {std::pair{1, 6}, std::pair{6, 1}, std::pair{6, 6}}) {
            for (int profile = 0; profile < 3; ++profile) {
                for (const bool guards : {false, true}) { verify_case(a, b, profile, guards); }
            }
        }
        output.flush();
        check(output.good(), "failed writing complete matrix");
        independent_interleaved_contexts();
        constexpr int batch_runs = 1000;
        std::size_t batch_actions = 0;
        const auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < batch_runs; ++i) {
            const auto result = run(roster(5, 5, i % 3), i % 2 == 0);
            batch_actions += result.trace.size();
        }
        const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
        std::cout << "SCENARIOS_OK cases=" << cases << " replays=" << cases << " max_actions=" << maximum_actions << '\n';
        std::cout << "ISOLATION_OK contexts=3 schedules=2 model=interleaved_single_thread independent_bosses_only\n";
        std::cout << "BATCH_OK battles=" << batch_runs << " actions=" << batch_actions << " elapsed_ms=" << milliseconds
                  << " build=Od scope=local_functional_not_server_benchmark\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "SCENARIOS_FAIL " << error.what() << '\n';
        return 1;
    }
}
