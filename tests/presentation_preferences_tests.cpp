#include "rpg/startup_presentation.hpp"
#include <iostream>
#include <stdexcept>
using namespace rpg::presentation;
void check(bool ok){if(!ok){throw std::runtime_error("Preferences contract failed");}}
template<class F> void rejects(F f){bool caught=false;try{f();}catch(const std::invalid_argument&){caught=true;}check(caught);}
int main(){try{
    for(auto mode:{PresentationMode::Auto,PresentationMode::Desktop,PresentationMode::Mobile}){for(bool motion:{false,true}){for(bool contrast:{false,true}){for(unsigned text:{100U,125U,150U}){for(auto area:{HubArea::Left,HubArea::Center,HubArea::Right}){for(auto view:{HubView::Auto,HubView::Focused,HubView::Panoramic}){PresentationPreferences p{mode,motion,contrast,text,area,view};check(decode_preferences(encode_preferences(p))==p);}}}}}}
    auto defaults=read_preferences({},{});check(defaults.source==PreferenceSource::Defaults && !defaults.primary_invalid && defaults.values==PresentationPreferences{});
    PresentationPreferences primary;primary.reduced_motion=true;primary.text_percent=150;PresentationPreferences backup;backup.high_contrast=true;const auto p=encode_preferences(primary),b=encode_preferences(backup);
    auto loaded=read_preferences(p,b);check(loaded.source==PreferenceSource::Primary && loaded.values==primary && !loaded.primary_invalid);
    loaded=read_preferences("broken",b);check(loaded.source==PreferenceSource::Backup && loaded.primary_invalid && loaded.values==backup);
    loaded=read_preferences("broken","also broken",primary);check(loaded.source==PreferenceSource::Defaults && loaded.primary_invalid && loaded.values==primary);
    loaded=read_preferences({},b);check(loaded.source==PreferenceSource::Backup && !loaded.primary_invalid);
    auto invalid=p;invalid.replace(invalid.find("RPG_PRESENTATION 1"),18,"RPG_PRESENTATION 2");rejects([&]{(void)decode_preferences(invalid);});rejects([&]{(void)decode_preferences(p+"credits 999\n");});rejects([]{(void)decode_preferences("RPG_URBANO_SAVE 1\n");});
    invalid=p;invalid.replace(invalid.find("text 150"),8,"text 0");rejects([&]{(void)decode_preferences(invalid);});invalid=p;invalid.replace(invalid.find("motion 1"),8,"motion true");rejects([&]{(void)decode_preferences(invalid);});
    rejects([]{PresentationPreferences x;x.text_percent=101;(void)encode_preferences(x);});rejects([]{PresentationPreferences x;x.mode=static_cast<PresentationMode>(99);(void)encode_preferences(x);});rejects([]{PresentationPreferences x;x.area=static_cast<HubArea>(99);(void)read_preferences({}, {},x);});
    rejects([]{(void)decode_preferences(std::string(4097,'x'));});rejects([]{(void)decode_preferences("");});rejects([&]{(void)decode_preferences(p.substr(0,p.size()/2));});check(decode_preferences(p+" \r\n")==primary);
    std::cout<<"PREFERENCES_TESTS_RESULT passed=10 total=10 combinations=324\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
