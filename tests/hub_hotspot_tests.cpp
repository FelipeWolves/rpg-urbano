#include "rpg/startup_presentation.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace rpg::presentation;
void check(bool ok){if(!ok){throw std::runtime_error("Hub hotspot contract failed");}}
bool near(double a,double b){return std::abs(a-b)<0.00001;}
HubHotspot link(){return {"battle","Batalha",HubRoute::Battle,0.8,0.6,{0.75,0.5,0.1,0.2}};}
void wide(){auto plan=plan_hub({3000,1500},{1200,600},Device::Desktop);auto p=project_hotspot(plan,link());check(p && near(p->hit_area.x,900) && near(p->hit_area.y,300) && near(p->hit_area.width,120) && near(p->label_x,960));check(contains(p->hit_area,960,360));}
void hidden(){auto plan=plan_hub({3000,1500},{600,900},Device::Mobile,HubArea::Center);check(!project_hotspot(plan,link()));plan=plan_hub({3000,1500},{600,900},Device::Mobile,HubArea::Right);check(project_hotspot(plan,link()).has_value());auto h=link();h.visible=false;check(!project_hotspot(plan,h));}
void clipped(){auto h=link();h.anchor_x=0.8;HubPlan plan{HubView::Focused,{0.78,0,0.1,1},{0,0,100,1000}};auto p=project_hotspot(plan,h);check(p && near(p->hit_area.x,0) && near(p->hit_area.width,70));check(!contains(p->hit_area,-1,600) && !contains(p->hit_area,75,600));}
void bars_resize(){auto plan=plan_hub({3000,1500},{1000,1000},Device::Desktop,HubArea::Right,HubView::Panoramic);auto p=project_hotspot(plan,link());check(p && near(p->hit_area.y,500) && !contains(p->hit_area,800,100));auto bigger=project_hotspot(plan_hub({3000,1500},{2000,1000},Device::Desktop),link());check(bigger && near(bigger->hit_area.width,200));}
void invalid(){auto h=link();h.source_area.width=-1;bool rejected=false;try{validate_hotspot(h);}catch(const std::invalid_argument&){rejected=true;}check(rejected);check(!contains({0,0,1,1},std::numeric_limits<double>::quiet_NaN(),0));check(!contains({0,0,1,1},1,0));}
int main(){try{wide();hidden();clipped();bars_resize();invalid();std::cout<<"HUB_HOTSPOT_TESTS_RESULT passed=5 total=5\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
