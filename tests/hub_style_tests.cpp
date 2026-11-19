#include "rpg/hub_style.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace rpg::presentation;
void check(bool ok){if(!ok){throw std::runtime_error("Hub style contract failed");}}
bool near(double a,double b){return std::abs(a-b)<0.000001;}
void timings(){auto a=pulse_frame(0,false),b=pulse_frame(1.2,false),c=pulse_frame(2.4,false);check(near(a.icon_scale,1) && near(a.ring_opacity,0.7));check(near(b.icon_scale,1.06) && near(b.ring_scale,1.175) && near(b.ring_opacity,0.35));check(near(c.icon_scale,1) && near(c.ring_opacity,0.7));auto end=pulse_frame(2.4-0.00001,false);check(end.ring_scale>1.3499 && end.ring_opacity<0.00001);}
void reduced(){for(double t:{0.0,0.6,1.2,2.4,1000.0}){auto p=pulse_frame(t,true);check(p.icon_scale==1 && p.ring_scale==1 && p.ring_opacity==0 && p.static_double_border);}}
void target(){auto r=minimum_target({20,30,8,10},{0,0,200,200});check(r && r->width==48 && r->height==48);auto edge=minimum_target({190,190,10,10},{0,0,200,200});check(edge && edge->x==152 && edge->y==152);check(!minimum_target({0,0,10,10},{0,0,40,200}));}
void bounds(){auto a=minimum_target({-5,10,20,20},{0,0,100,100});check(a && a->x==0 && a->width==48);check(!minimum_target({200,200,20,20},{0,0,100,100}));auto big=minimum_target({0,0,80,90},{0,0,100,100});check(big && big->width==80 && big->height==90);}
void invalid(){bool caught=false;try{(void)pulse_frame(std::numeric_limits<double>::quiet_NaN(),false);}catch(const std::invalid_argument&){caught=true;}check(caught);caught=false;try{PulseSettings s;s.period=0;(void)pulse_frame(0,false,s);}catch(const std::invalid_argument&){caught=true;}check(caught);}
int main(){try{timings();reduced();target();bounds();invalid();std::cout<<"HUB_STYLE_TESTS_RESULT passed=5 total=5\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
