#pragma once
#include "rpg/team_selection.hpp"
namespace rpg {
[[nodiscard]] BattleRules text_reference_rules();
[[nodiscard]] BattleRules team_reference_rules();
[[nodiscard]] EncounterSetup team_reference_setup();
[[nodiscard]] EncounterSetup classic_reference_setup();
[[nodiscard]] EncounterSetup felipe_pilot_setup(bool tutorial_preload=false);
} // namespace rpg

namespace rpg::presentation {
struct InspectionFrame {
    std::string id;
    std::string description;
    Snapshot battle;
    std::string focus;
    std::vector<std::string> notes;
    bool operator==(const InspectionFrame&) const = default;
};
// Read-only fixtures built with the real Battle executor; not a reward-bearing session.
[[nodiscard]] std::vector<InspectionFrame> arena_inspection_frames();
}
