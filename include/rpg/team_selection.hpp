#pragma once
#include "rpg/ability_profile.hpp"

namespace rpg {
struct EncounterSetup {
    std::vector<Definition> roster;
    BattleRules rules;
    std::vector<CharacterLoadout> loadouts;
    bool operator==(const EncounterSetup&) const = default;
};
enum class SelectionError { None, Empty, TooMany, UnknownMember, DuplicateMember };
struct TeamSelectionSnapshot {
    Team side;
    std::vector<std::string> available;
    std::vector<std::string> selected;
    bool operator==(const TeamSelectionSnapshot&) const = default;
};
// Pre-battle composition only: no runtime, rewards, inventory or file I/O.
class TeamSelection {
public:
    explicit TeamSelection(EncounterSetup catalog,Team side=Team::A);
    [[nodiscard]] SelectionError select(std::vector<std::string> ids);
    [[nodiscard]] TeamSelectionSnapshot snapshot() const {return {side_,available_,selected_};}
    [[nodiscard]] EncounterSetup compose() const;
private:
    EncounterSetup catalog_;
    Team side_;
    std::vector<std::string> available_,selected_;
};
} // namespace rpg
