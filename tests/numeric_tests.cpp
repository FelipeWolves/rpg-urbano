#include "rpg/numeric.hpp"
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <algorithm>
using namespace rpg::numeric;
void check(bool ok){if(!ok){throw std::runtime_error("Numeric arithmetic contract failed");}}
template<class F>void rejected(F f){bool caught=false;try{f();}catch(const std::invalid_argument&){caught=true;}check(caught);}
int main(int argc,char** argv){try{
 check(quantity(0)==0 && quantity(quantity_max+1)==quantity_max && quantity(std::numeric_limits<std::int64_t>::max())==quantity_max);rejected([]{static_cast<void>(quantity(-1));});
 check(add(quantity_max-1,1)==quantity_max && add(quantity_max,quantity_max)==quantity_max && add(3,4)==7);rejected([]{static_cast<void>(add(-1,1));});
 check(percent_floor(55,1000)==5 && percent_floor(100,15000)==150 && percent_floor(1,5000)==0);
 check(mul_div_floor(quantity_max,quantity_max,quantity_max)==quantity_max && mul_div_floor(quantity_max,quantity_max,quantity_max,99)==99);
 check(mul_div_floor(0,999,1)==0 && mul_div_floor(9,9,2)==40 && mul_div_floor(1,1,2,0)==0);
 rejected([]{static_cast<void>(mul_div_floor(1,1,0));});rejected([]{static_cast<void>(mul_div_floor(-1,1,1));});rejected([]{static_cast<void>(mul_div_floor(1,-1,1));});rejected([]{static_cast<void>(mul_div_floor(1,1,1,quantity_max+1));});
 check(compare_ratio(1,3,2,6)==0 && compare_ratio(0,1,0,99)==0 && compare_ratio(quantity_max-1,quantity_max,quantity_max-2,quantity_max-1)>0);
 rejected([]{static_cast<void>(compare_ratio(1,0,1,1));});rejected([]{static_cast<void>(compare_ratio(-1,1,1,1));});
 for(std::int64_t a=0;a<=40;++a){for(std::int64_t b=0;b<=40;++b){for(std::int64_t d=1;d<=40;++d){check(mul_div_floor(a,b,d,1000)==std::min<std::int64_t>(1000,a*b/d));check(compare_ratio(a,d,b,41)==(a*41<b*d?-1:a*41>b*d?1:0));}}}
 if(argc!=2){throw std::runtime_error("BigInteger oracle vector path required");}
 std::ifstream file(argv[1]);std::int64_t a,b,d,cap,expected;int count=0,ratio_expected=0;
 while(file>>a>>b>>d>>cap>>expected>>ratio_expected){check(mul_div_floor(a,b,d,cap)==expected);check(compare_ratio(a,d,b,quantity_max)==ratio_expected);++count;}
 check(file.eof() && count>=256);
 std::cout<<"NUMERIC_TESTS_RESULT passed=10 total=10 oracle="<<count<<" exhaustive=67240\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
