#include "rpg/battle.hpp"

#include <iostream>

int main() {
    rpg::Battle battle(rpg::reference_roster());
    std::cout << "RPG Urbano - demonstracao automatica 3x3 (sem entrada interativa)\n"
              << "A3 usa defesa na primeira acao; depois ambas equipes atacam por menor vida.\n";
    int actions = 0;
    constexpr int safety_limit = 256;
    while (battle.phase() == rpg::Phase::Running && actions < safety_limit) {
        auto action = rpg::choose_basic_attack(battle);
        if (!action) { std::cerr << "SIMULATION_FAIL no_action\n"; return 1; }
        if (actions == 0) { *action = {"A3", rpg::ActionType::Guard, {}}; }
        const auto round = battle.round();
        const auto result = battle.apply(*action);
        if (!result.accepted()) { std::cerr << "SIMULATION_FAIL rejected_action\n"; return 2; }
        ++actions;
        std::cout << "R" << round << " " << action->actor;
        if (action->type == rpg::ActionType::Guard) {
            std::cout << " defesa\n";
        } else {
            for (const auto& event : result.events) {
                if (event.type == rpg::EventType::Attack) {
                    std::cout << " -> " << event.target << " dano=" << event.amount
                              << " vida=" << battle.find(event.target)->health << '\n';
                }
            }
        }
    }
    if (battle.phase() != rpg::Phase::Finished) { std::cerr << "SIMULATION_FAIL safety_limit\n"; return 3; }
    std::cout << "SIMULATION_OK winner=" << (*battle.winner() == rpg::Team::A ? 'A' : 'B')
              << " rounds=" << battle.round() << " actions=" << actions << '\n';
    return 0;
}
