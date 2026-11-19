#pragma once
#include "rpg/local_session.hpp"
#include "rpg/reference_encounter.hpp"
namespace rpg::presentation {
enum class VisualError { None, StaleCommand, NotPlayerTurn, Finished, SafetyStop, CoreRejected };
struct VisualResult {
    VisualError error=VisualError::None;
    ActionResult action;
    [[nodiscard]] bool accepted() const noexcept {return error==VisualError::None;}
};
struct VisualState {
    std::uint64_t revision;
    ProfileSnapshot combat;
    LocalProgress progress;
    std::vector<AbilityView> abilities;
    bool automatic;
    bool stopped;
};
// Owns one session; views cannot mutate it. Revision expires after every accepted action/reset.
class VisualSession {
public:
    explicit VisualSession(EncounterSetup setup=team_reference_setup(),CombatProfile profile=CombatProfile::Team);
    [[nodiscard]] VisualState state() const;
    [[nodiscard]] VisualResult submit(std::uint64_t revision,std::uint32_t slot,std::string target);
    [[nodiscard]] VisualResult submit_action(std::uint64_t revision,const Action& action);
    [[nodiscard]] VisualResult step_automatic();
    void reset();
private:
    LocalSession session_;
    std::uint64_t revision_=0;
    std::uint32_t automatic_count_=0;
    bool stopped_=false;
    void check_revision_capacity() const;
};
} // namespace rpg::presentation
