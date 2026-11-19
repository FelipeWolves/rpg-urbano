#pragma once
#include "rpg/team_selection.hpp"
namespace rpg {
[[nodiscard]] BattleRules text_reference_rules();
[[nodiscard]] EncounterSetup team_reference_setup();
[[nodiscard]] EncounterSetup classic_reference_setup();
} // namespace rpg
