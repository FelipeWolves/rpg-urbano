#pragma once
#include "rpg/team_selection.hpp"
#include "rpg/local_session.hpp"
#include "rpg/save_game.hpp"
#include <iosfwd>

namespace rpg::presentation {
enum class TextExit { Quit, InputClosed, AutomaticSafetyStop };
struct TextReport {
    Snapshot final_state;
    TextExit exit;
    std::uint64_t player_actions;
    std::uint64_t automatic_actions;
    std::uint64_t rejected_commands;
    std::uint64_t restarts;
    LocalProgress progress={};
};
using TeamReferenceSetup = EncounterSetup;
struct TextSessionOptions {
    std::optional<std::filesystem::path> save_path;
    std::optional<LocalProgress> restored;
    std::vector<std::string> selected;
};
[[nodiscard]] TeamReferenceSetup team_reference_setup();
[[nodiscard]] BattleRules text_reference_rules();
// Presentation only; streams support terminal use and deterministic scripted checks.
[[nodiscard]] TextReport run_text_session(std::istream& input,std::ostream& output,
    std::vector<Definition> roster=reference_roster(),BattleRules rules=text_reference_rules(),
    CombatProfile profile=CombatProfile::Classic,std::vector<CharacterLoadout> loadouts={},TextSessionOptions options={});
[[nodiscard]] TextReport run_team_reference_session(std::istream& input,std::ostream& output,TextSessionOptions options={});
[[nodiscard]] TextReport run_selected_team_reference_session(std::istream& input,std::ostream& output,
    std::vector<std::string> selected,TextSessionOptions options={});
} // namespace rpg::presentation
