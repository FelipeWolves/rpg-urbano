#pragma once
#include "rpg/visual_session.hpp"
namespace rpg::presentation {
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
