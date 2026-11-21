#include "rpg/startup_presentation.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace rpg::presentation;
void check(bool value){if(!value){throw std::runtime_error("Safe frame contract failed");}}
bool inside(const Rect& a,const Rect& b){return a.x>=b.x-1e-6 && a.y>=b.y-1e-6 && a.x+a.width<=b.x+b.width+1e-6 && a.y+a.height<=b.y+b.height+1e-6;}
bool overlaps(const Rect& a,const Rect& b){return a.x<b.x+b.width-1e-6 && b.x<a.x+a.width-1e-6 && a.y<b.y+b.height-1e-6 && b.y<a.y+a.height-1e-6;}
template<class F> void rejects(F f){bool caught=false;try{f();}catch(const std::invalid_argument&){caught=true;}check(caught);}
void verify(const HubFrame& f,unsigned count){
    check(f.destinations.size()==count && f.navigation.size()==5);
    check(inside(f.header,f.safe) && inside(f.scene,f.safe) && f.scene.width>0 && f.scene.height>=64);
    std::vector<Rect> all{f.header,f.scene};
    for(auto r:f.destinations){check(r.width>=48 && r.height>=48 && inside(r,f.safe));all.push_back(r);}
    for(auto r:f.navigation){check(r.width>=48 && r.height>=48 && inside(r,f.safe));all.push_back(r);}
    for(std::size_t i=0;i<all.size();++i){for(std::size_t j=i+1;j<all.size();++j){check(!overlaps(all[i],all[j]));}}
}
int main(){try{
    auto desktop=plan_hub_frame({1280,720},7);check(desktop.has_value());verify(*desktop,7);check(desktop->destinations[0].y==desktop->destinations[6].y);
    auto portrait=plan_hub_frame({450,800},7);check(portrait.has_value());verify(*portrait,7);check(portrait->destinations[0].y<portrait->destinations[3].y && portrait->destinations[3].y<portrait->destinations[6].y);
    auto margins=plan_hub_frame({1080,2400},7,{30,90,45,120});check(margins.has_value());verify(*margins,7);check(margins->safe.x==30 && margins->safe.y==90 && margins->safe.width==1005 && margins->safe.height==2190);
    for(auto size:{PixelSize{280,360},PixelSize{599,720},PixelSize{600,720},PixelSize{800,450},PixelSize{2560,1080},PixelSize{1920,1200}}){for(unsigned count:{0U,1U,5U,7U,16U}){auto frame=plan_hub_frame(size,count);if(frame){verify(*frame,count);}}}
    check(!plan_hub_frame({279,800},7) && !plan_hub_frame({800,359},7) && !plan_hub_frame({280,360},16));
    check(!plan_hub_frame({450,800},7,{300,0,300,0}));check(!plan_hub_frame({450,800},7,{0,1e300,0,0}));
    rejects([]{(void)plan_hub_frame({450,800},7,{-1,0,0,0});});rejects([]{(void)plan_hub_frame({450,800},7,{0,std::numeric_limits<double>::quiet_NaN(),0,0});});rejects([]{(void)plan_hub_frame({0,800},7);});rejects([]{(void)plan_hub_frame({450,800},17);});
    const auto first=plan_hub_frame({800,450},7,{4,8,12,16});const auto again=plan_hub_frame({800,450},7,{4,8,12,16});check(first && again && first->scene.x==again->scene.x && first->scene.height==again->scene.height);verify(*first,7);
    std::cout<<"HUB_FRAME_TESTS_RESULT passed=8 total=8\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
