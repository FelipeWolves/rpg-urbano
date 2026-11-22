#include "rpg/startup_presentation.hpp"
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace rpg::presentation;
void check(bool x,const char* m){if(!x){throw std::runtime_error(m);}}
PointerFrame tap(double x,double y){PointerFrame f;f.tap=true;f.released=true;f.x=f.origin_x=x;f.y=f.origin_y=y;return f;}
int main(){try{
 PointerInput first;PointerContext context{1,0};check(first.update({true,true,10,10},{},canvas_input_active(true,false),0,context).pressed,"first click not eaten by widget focus");auto released=first.update({false,true,10,10},{},canvas_input_active(true,false),.1,context);check(pointer_clicked(released,{0,0,48,48}),"one click routes");
 PointerInput inactive;(void)inactive.update({true,true,10,10},{},canvas_input_active(false,true),0,context);check(!inactive.update({false,true,10,10},{},canvas_input_active(true,true),.1,context).tap,"inactive window cannot leak focus click");
 std::vector<TargetHitRegion> regions{{"A",{0,0,48,48},10,false},{"B",{60,0,48,48},20,false}};check(pick_target(tap(10,10),regions)=="A","choose A");check(pick_target(tap(70,10),regions)=="B","change to B without new ability");check(!pick_target(tap(120,10),regions),"invalid target does not select");
 regions.push_back({"near",{0,0,48,48},1,false});check(pick_target(tap(10,10),regions)=="near","overlapping body prefers nearest");
 regions.push_back({"label",{0,0,48,48},100,true});check(pick_target(tap(10,10),regions)=="label","label has priority");
 auto drag=tap(70,10);drag.origin_x=10;check(!pick_target(drag,{{"B",{60,0,48,48},20,false}}),"drag into target does not click");auto stale=tap(70,10);stale.cancelled=true;check(!pick_target(stale,regions),"cancelled gesture cannot select");
 regions.push_back({"invalid",{60,0,48,48},std::numeric_limits<double>::quiet_NaN(),true});check(pick_target(tap(70,10),regions)=="B","invalid projected region ignored");
 std::cout<<"INTERACTION_ROUTING_TESTS_RESULT passed=8 total=8\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
