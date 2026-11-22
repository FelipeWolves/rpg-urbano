#include "rpg/startup_presentation.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace rpg::presentation;
void check(bool ok){if(!ok){throw std::runtime_error("Canvas coordinates failed");}}
int main(){try{
 auto p=viewport_to_canvas(30,40,{});check(p && p->x==30 && p->y==40);
 CanvasMapping offset{0,24,1,0,0,1};p=viewport_to_canvas(110,134,offset);check(p && p->y==110 && contains({100,100,48,48},p->x,p->y));auto above=viewport_to_canvas(110,110,offset);check(above && !contains({100,100,48,48},above->x,above->y));
 for(double s:{1.,1.25,1.5,2.}){CanvasMapping m{12*s,24*s,s,0,0,s};auto a=canvas_to_viewport(123,87,m);auto b=viewport_to_canvas(a->x,a->y,m);check(b && std::abs(b->x-123)<1e-6 && std::abs(b->y-87)<1e-6);}
 for(auto m:{CanvasMapping{120,40,0,-1,1,0},CanvasMapping{8,9,2,.2,.1,1.5}}){auto a=canvas_to_viewport(150,90,m);auto b=viewport_to_canvas(a->x,a->y,m);check(b && std::abs(b->x-150)<1e-6 && std::abs(b->y-90)<1e-6);}
 p=viewport_to_canvas(0,0,offset);check(p && p->y==-24 && !contains({0,0,100,100},p->x,p->y));
 check(!viewport_to_canvas(1,1,{0,0,0,0,0,0}) && !canvas_to_viewport(std::numeric_limits<double>::quiet_NaN(),0,{}));
 check(same_canvas_mapping({},{}));check(!same_canvas_mapping({},offset));PointerInput latched;(void)latched.update({true,true,110,110},{},true,0,{});latched.cancel_until_release();check(!latched.update({true,true,110,110},{},true,.1,{}).pressed);check(!latched.update({false,true,110,110},{},true,.2,{}).tap);
 for(bool touch:{false,true}){PointerInput input;auto local=viewport_to_canvas(110,134,offset);PointerSample down{true,true,local->x,local->y},up{false,true,local->x,local->y};(void)input.update(touch?PointerSample{}:down,touch?down:PointerSample{},true,0,{});check(pointer_clicked(input.update(touch?PointerSample{}:up,touch?up:PointerSample{},true,.1,{}),{100,100,48,48}));}
 std::cout<<"CANVAS_COORDINATE_TESTS_RESULT passed=8 total=8\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
