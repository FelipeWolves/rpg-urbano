#pragma once
#include "rpg/visual_session.hpp"
#include "rpg/pve.hpp"
namespace rpg::presentation {
struct PveCounter {
 std::uint64_t completed;
 std::optional<std::uint64_t> goal;
 bool operator==(const PveCounter&) const=default;
};
// Read-only HUD model, never a reward grant or permission to bypass command validation.
struct PveHud {
 std::string encounter_id,attempt_id,boss_id,mode_label,objective_label,status_label;
 std::optional<std::string> phase_id;
 numeric::Quantity boss_health,boss_max_health;
 PveCounter actions,boss_turns;
 TickPolicy boss_turn_clock;
 std::optional<std::uint64_t> remaining_actions;
 std::optional<numeric::Quantity> score;
 bool encounter_finished,objective_met,commands_open;
 std::optional<PveEnd> end;
 bool operator==(const PveHud&) const=default;
};
[[nodiscard]] PveHud pve_hud(const PveSnapshot& snapshot);
enum class AbilityBadge { Ready, Cooldown, Passive, Locked, Unavailable };
struct AbilityCard {
    std::string kind;
    std::string status;
    std::string reason;
    AbilityBadge badge=AbilityBadge::Unavailable;
    int cooldown=0;
    bool selectable=false;
};
// Presentation only. Never enables an action rejected by the core.
[[nodiscard]] AbilityCard ability_card(const VisualState& state,std::uint32_t position);
struct ShieldDisplay {
    std::int64_t points;
    double health_fraction;
    double overlay_fraction;
};
[[nodiscard]] ShieldDisplay shield_display(const Fighter& fighter);
[[nodiscard]] std::string effect_name(EffectKind kind);
[[nodiscard]] std::string action_error_message(ActionError error);
[[nodiscard]] std::vector<std::string> effect_details(const Fighter& fighter);
[[nodiscard]] std::vector<std::string> ability_details(const VisualState& state,std::uint32_t position);
[[nodiscard]] std::vector<std::string> target_candidates(const VisualState& state,std::uint32_t position);
// 0.5 seconds is a provisional input threshold, not battle time.
class AbilityGesture {
public:
    void press(std::uint32_t position,double seconds,std::uint64_t revision);
    [[nodiscard]] std::optional<std::uint32_t> hold(double seconds,std::uint64_t revision);
    [[nodiscard]] std::optional<std::uint32_t> release(double seconds,std::uint64_t revision);
    void cancel() noexcept;
    [[nodiscard]] bool active() const noexcept {return position_!=0;}
private:
    std::uint32_t position_=0;
    double started_=0;
    std::uint64_t revision_=0;
    bool held_=false;
};
} // namespace rpg::presentation
