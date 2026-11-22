#pragma once
#include "rpg/local_session.hpp"
#include "rpg/reference_encounter.hpp"
#include "rpg/save_game.hpp"
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
    [[nodiscard]] TeamSelectionSnapshot team() const {return selection_.snapshot();}
    // Between battles only. Success starts a fresh attempt and expires old commands.
    [[nodiscard]] SelectionError select_team(std::vector<std::string> ids);
    [[nodiscard]] std::string export_save() const;
    void import_save(std::string_view bytes);
private:
    TeamSelection selection_;
    CombatProfile profile_;
    LocalSession session_;
    std::uint64_t revision_=0;
    std::uint32_t automatic_count_=0;
    bool stopped_=false;
    void check_revision_capacity() const;
    void require_between_battles() const;
};
} // namespace rpg::presentation
