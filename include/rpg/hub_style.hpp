#pragma once
#include "rpg/startup_presentation.hpp"
namespace rpg::presentation {
struct PulseSettings {
    double period=2.4;
    double minimum_scale=1.0;
    double maximum_scale=1.06;
    double ring_maximum_scale=1.35;
    double ring_opacity=0.7;
    double ring_stroke=1.5;
    double minimum_target=48;
};
struct PulseFrame {
    double icon_scale;
    double ring_scale;
    double ring_opacity;
    bool static_double_border;
};
struct LabelMotion { double offset_y; double glow_opacity; };
[[nodiscard]] LabelMotion label_motion(double seconds,bool reduced_motion,double phase=0);
void validate_pulse(const PulseSettings& settings);
[[nodiscard]] PulseFrame pulse_frame(double seconds,bool reduced_motion,const PulseSettings& settings={});
// Returns no target if the visible content cannot hold the required minimum.
[[nodiscard]] std::optional<Rect> minimum_target(const Rect& area,const Rect& visible_content,double pixels=48);
} // namespace rpg::presentation
