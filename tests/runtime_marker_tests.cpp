#include "rpg/arena_layout.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <limits>
using namespace rpg::presentation;
void check(bool v){if(!v){throw std::runtime_error("Marker contract failed");}}
int main(){try{
 for(const auto n:{4u,28u}){auto edges=marker_edges(75,n);check(edges.size()==n);double x=0,y=0;for(const auto& e:edges){check(std::isfinite(e.x) && e.length>0);x+=e.length*std::cos(e.yaw*3.141592653589793/180);y+=e.length*std::sin(e.yaw*3.141592653589793/180);}check(std::abs(x)<0.001 && std::abs(y)<0.001);}
 check(std::abs(marker_edges(20,4)[0].length-std::sqrt(800.0))<0.001);
 for(const auto radius:{0.f,-1.f,std::numeric_limits<float>::infinity()}){bool caught=false;try{static_cast<void>(marker_edges(radius,4));}catch(const std::invalid_argument&){caught=true;}check(caught);}
 for(const auto n:{0u,2u,65u}){bool caught=false;try{static_cast<void>(marker_edges(10,n));}catch(const std::invalid_argument&){caught=true;}check(caught);}
 std::cout<<"AUT12_TESTS_RESULT passed=5 total=5\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
