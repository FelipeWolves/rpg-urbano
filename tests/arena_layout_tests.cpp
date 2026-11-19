#include "rpg/arena_layout.hpp"
#include <iostream>
#include <stdexcept>
using namespace rpg;
using namespace rpg::presentation;
void check(bool ok){if(!ok){throw std::runtime_error("Arena layout contract failed");}}
std::vector<Definition> roster(int count){std::vector<Definition> out;for(int i=0;i<count;++i){out.push_back({"A"+std::to_string(i+1),Team::A,100,10,100});out.push_back({"B"+std::to_string(i+1),Team::B,100,10,100});}return out;}
void depth_default(){auto r=roster(5);auto p=arena_placements(r);check(default_arena_layout==ArenaLayout::StandardDepth && p.size()==10);check(p[2].x>0 && p[4].x<0 && p[6].x>0 && p[8].x<0);check(p[0].y<p[6].y && p[0].y<p[8].y);for(std::size_t i=0;i<p.size();++i){check(p[i].fighter_id==r[i].id && p[i].side==r[i].team);check(p[i].side==Team::A?p[i].y<0:p[i].y>0);if(i>=2){check(p[i].side==Team::A?p[i].y>p[i-2].y:p[i].y<p[i-2].y);}}}
void classic_preserved(){auto p=arena_placements(roster(3),ArenaLayout::ClassicLateral);for(std::size_t i=0;i<p.size();++i){check(p[i].x==(i%2==0?230.f:-230.f));check(p[i].y==(static_cast<float>(i/2)-1.f)*170.f);}}
void formations(){for(int n=1;n<=5;++n){for(auto mode:{ArenaLayout::StandardDepth,ArenaLayout::ClassicLateral}){auto r=roster(n);auto before=r;auto a=arena_placements(r,mode);check(a==arena_placements(r,mode) && r==before);for(std::size_t i=0;i<a.size();++i){for(std::size_t j=i+1;j<a.size();++j){check(a[i].x!=a[j].x || a[i].y!=a[j].y);}}}}}
void asymmetric(){auto r=roster(1);for(int i=2;i<=5;++i){r.push_back({"A"+std::to_string(i),Team::A,100,10,100});}check(arena_placements(r).size()==6 && arena_placements(r,ArenaLayout::ClassicLateral).size()==6);}
void invalid(){for(int kind=0;kind<4;++kind){auto r=roster(2);if(kind==0){r=roster(6);}if(kind==1){r[1].id=r[0].id;}if(kind==2){r.clear();}if(kind==3){r[0].team=static_cast<Team>(99);}bool rejected=false;try{static_cast<void>(arena_placements(r));}catch(const std::invalid_argument&){rejected=true;}check(rejected);}}
void names(){check(parse_arena_layout("Standard")==ArenaLayout::StandardDepth);check(parse_arena_layout("Classic")==ArenaLayout::ClassicLateral);check(!parse_arena_layout("Unknown"));}
int main(){try{depth_default();classic_preserved();formations();asymmetric();invalid();names();std::cout<<"ARENA_LAYOUT_TESTS_RESULT passed=6 total=6\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
