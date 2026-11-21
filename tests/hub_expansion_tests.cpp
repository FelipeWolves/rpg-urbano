#include "rpg/startup_presentation.hpp"
#include "rpg/hub_style.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace rpg::presentation;
void check(bool value){if(!value){throw std::runtime_error("Hub expansion contract failed");}}
template<class F> void rejects(F f){bool caught=false;try{f();}catch(const std::invalid_argument&){caught=true;}check(caught);}
int main(){try{
    check(parse_presentation_mode("Auto")==PresentationMode::Auto && parse_presentation_mode("Desktop")==PresentationMode::Desktop && parse_presentation_mode("Mobile")==PresentationMode::Mobile && !parse_presentation_mode("mobile") && !parse_presentation_mode(""));
    for(auto actual:{Device::Desktop,Device::Mobile}){check(presentation_device(PresentationMode::Auto,actual)==actual);check(presentation_device(PresentationMode::Desktop,actual)==Device::Desktop);check(presentation_device(PresentationMode::Mobile,actual)==Device::Mobile);}
    rejects([]{(void)presentation_device(static_cast<PresentationMode>(99),Device::Desktop);});
    rejects([]{(void)presentation_device(PresentationMode::Desktop,static_cast<Device>(99));});
    const auto forced=plan_hub({1670,942},{400,700},presentation_device(PresentationMode::Mobile,Device::Desktop));check(forced.view==HubView::Focused);
    for(auto route:{HubRoute::Terminal,HubRoute::Unavailable}){HubHotspot h{"test","Local",route,.55,.4,{.5,.35,.1,.1}};validate_hotspot(h);check(project_hotspot(plan_hub({1670,942},{1280,720},Device::Desktop),h).has_value());}
    for(double time:{0.0,1.2,2.4,3.6,4.8,10000.0}){auto m=label_motion(time,false);check(std::abs(m.offset_y)<=2.00001 && m.glow_opacity>=.14999 && m.glow_opacity<=.35001);auto fixed=label_motion(time,true,1.2);check(fixed.offset_y==0 && fixed.glow_opacity==.25);}
    rejects([]{(void)label_motion(-1,false);});rejects([]{(void)label_motion(std::numeric_limits<double>::infinity(),false);});rejects([]{(void)label_motion(0,true,std::numeric_limits<double>::quiet_NaN());});
    std::cout<<"HUB_EXPANSION_TESTS_RESULT passed=6 total=6\n";return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
