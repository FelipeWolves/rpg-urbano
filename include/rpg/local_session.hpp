#pragma once
#include "rpg/ability_profile.hpp"

namespace rpg {
struct LocalCompletion {
    std::uint64_t attempt;
    std::optional<Team> winner;
    EndReason reason;
    std::uint64_t actions;
    std::uint64_t credits;
    bool operator==(const LocalCompletion&) const = default;
};
struct LocalProgress {
    std::uint64_t attempt=1;
    std::uint64_t credits=0;
    std::vector<LocalCompletion> completions;
    bool operator==(const LocalProgress&) const = default;
};
void validate_progress(const LocalProgress& progress);
// Owns attempts and their in-memory ledger; accepts no external result or claim ID.
class LocalSession {
public:
    LocalSession(std::vector<Definition> roster,BattleRules rules,CombatProfile profile=CombatProfile::Classic,
                 std::vector<CharacterLoadout> loadouts={});
    [[nodiscard]] const ProfileBattle& battle() const noexcept {return battle_;}
    [[nodiscard]] const LocalProgress& progress() const noexcept {return progress_;}
    [[nodiscard]] ActionResult apply(const Action& action);
    [[nodiscard]] ActionResult apply_slot(std::uint32_t position,std::string target);
    [[nodiscard]] bool settle();
    void reset();
    // Only on a pristine session; resumes BETWEEN battles with a fresh attempt ID.
    void restore_progress(const LocalProgress& progress);
private:
    ProfileBattle battle_;
    LocalProgress progress_;
};
} // namespace rpg
