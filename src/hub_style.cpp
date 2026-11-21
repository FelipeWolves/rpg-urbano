#include "rpg/hub_style.hpp"
#include <algorithm>
#include <cmath>
#include <numbers>
#include <stdexcept>
namespace rpg::presentation {
LabelMotion label_motion(double seconds,bool reduced_motion,double phase){
    if(!std::isfinite(seconds) || seconds<0 || !std::isfinite(phase)){throw std::invalid_argument("Invalid label motion time");}
    if(reduced_motion){return {0,0.25};}
    const double wave=std::sin(2*std::numbers::pi*std::fmod(seconds,4.8)/4.8+phase);
    return {2*wave,0.25+0.1*wave};
}
void validate_pulse(const PulseSettings& s){
    for(const double value:{s.period,s.minimum_scale,s.maximum_scale,s.ring_maximum_scale,s.ring_opacity,s.ring_stroke,s.minimum_target}){
        if(!std::isfinite(value)){throw std::invalid_argument("Finite pulse settings required");}
    }
    if(s.period<=0 || s.minimum_scale<=0 || s.maximum_scale<s.minimum_scale || s.ring_maximum_scale<1 ||
       s.ring_opacity<0 || s.ring_opacity>1 || s.ring_stroke<=0 || s.minimum_target<48){throw std::invalid_argument("Invalid pulse settings");}
}
PulseFrame pulse_frame(double seconds,bool reduced_motion,const PulseSettings& s){
    validate_pulse(s);
    if(!std::isfinite(seconds) || seconds<0){throw std::invalid_argument("Invalid presentation time");}
    if(reduced_motion){return {1,1,0,true};}
    const double phase=std::fmod(seconds,s.period)/s.period;
    const double wave=0.5-0.5*std::cos(2*std::numbers::pi*phase);
    const double ease=0.5-0.5*std::cos(std::numbers::pi*phase);
    return {s.minimum_scale+(s.maximum_scale-s.minimum_scale)*wave,
        1+(s.ring_maximum_scale-1)*ease,s.ring_opacity*(1-ease),false};
}
std::optional<Rect> minimum_target(const Rect& a,const Rect& b,double pixels){
    for(const double value:{a.x,a.y,a.width,a.height,b.x,b.y,b.width,b.height,pixels}){
        if(!std::isfinite(value)){throw std::invalid_argument("Finite target geometry required");}
    }
    if(a.width<=0 || a.height<=0 || b.width<=0 || b.height<=0 || pixels<1){throw std::invalid_argument("Positive target geometry required");}
    const double x=std::max(a.x,b.x),y=std::max(a.y,b.y),right=std::min(a.x+a.width,b.x+b.width),bottom=std::min(a.y+a.height,b.y+b.height);
    if(right<=x || bottom<=y || b.width<pixels || b.height<pixels){return {};}
    const double width=std::min(b.width,std::max(pixels,right-x)),height=std::min(b.height,std::max(pixels,bottom-y));
    return Rect{std::clamp((x+right-width)/2,b.x,b.x+b.width-width),
        std::clamp((y+bottom-height)/2,b.y,b.y+b.height-height),width,height};
}
} // namespace rpg::presentation
