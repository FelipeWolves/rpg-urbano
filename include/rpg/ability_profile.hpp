#pragma once
#include "rpg/battle.hpp"

namespace rpg {
enum class CombatProfile { Classic, Team };
enum class PassiveTrigger { BattleStart };
struct PassiveDefinition {
    std::string id;
    PassiveTrigger trigger = PassiveTrigger::BattleStart;
    int shield = 10;
    int duration = 2;
    bool operator==(const PassiveDefinition&) const = default;
};
struct PassiveActivation {
    std::string owner;
    std::uint32_t position;
    PassiveDefinition passive;
    bool operator==(const PassiveActivation&) const = default;
};
struct AbilitySlot {
    std::uint32_t position;
    std::string name;
    std::optional<std::string> active_skill;
    // Notes without an executable definition remain informational.
    std::string passive_note;
    std::optional<PassiveDefinition> passive = std::nullopt;
    bool operator==(const AbilitySlot&) const = default;
};
struct CharacterLoadout {
    std::string fighter_id;
    std::vector<AbilitySlot> slots;
    bool operator==(const CharacterLoadout&) const = default;
};
enum class AbilityAvailability { Ready, Cooldown, PassiveOnly, Incapacitated, Finished };
struct AbilityView {
    AbilitySlot slot;
    AbilityAvailability availability;
    int cooldown;
    bool passive_implemented=false;
};
struct ProfileSnapshot {
    CombatProfile profile;
    std::vector<CharacterLoadout> loadouts;
    Snapshot battle;
    std::vector<PassiveActivation> passive_activations;
    bool operator==(const ProfileSnapshot&) const = default;
};
// Validates commands before the unchanged Battle resolver; owns all mutable state.
class ProfileBattle {
public:
    ProfileBattle(std::vector<Definition> roster,BattleRules rules,CombatProfile profile=CombatProfile::Classic,
                  std::vector<CharacterLoadout> loadouts={});
    [[nodiscard]] const Battle& battle() const noexcept { return *battle_; }
    [[nodiscard]] CombatProfile profile() const noexcept { return profile_; }
    [[nodiscard]] const std::vector<PassiveActivation>& passive_activations() const noexcept { return passive_activations_; }
    [[nodiscard]] ProfileSnapshot snapshot() const;
    [[nodiscard]] std::vector<AbilityView> abilities(std::string_view fighter) const;
    [[nodiscard]] ActionResult apply(const Action& action);
    [[nodiscard]] ActionResult apply_slot(std::uint32_t position,std::string target);
    [[nodiscard]] std::optional<Action> choose_basic_action() const;
    void reset();
private:
    std::vector<Definition> roster_;
    BattleRules rules_;
    CombatProfile profile_;
    std::vector<CharacterLoadout> loadouts_;
    std::optional<Battle> battle_;
    std::vector<PassiveActivation> passive_activations_;
    [[nodiscard]] const CharacterLoadout* loadout(std::string_view fighter) const;
    [[nodiscard]] const AbilitySlot* slot(std::string_view fighter,std::uint32_t position) const;
};
} // namespace rpg
