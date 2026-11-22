#include "rpg/hit_resolution.hpp"
#include <iostream>
using namespace rpg::resolution;
void check(bool v){if(!v){throw std::runtime_error("Hit outcome contract failed");}}
template<class F>void rejected(F f){bool caught=false;try{f();}catch(const std::invalid_argument&){caught=true;}check(caught);}
int main(){try{
 check(mix(0)==0xe220a8397b1dcdafULL && identity("A1")==identity("A1") && identity("A1")!=identity("B1"));RollKey key{42,1,1,identity("B1"),0};
 for(std::uint64_t i=0;i<10000;++i){key.action=i;auto a=roll(key,RollChannel::Block);check(a<10000 && a==roll(key,RollChannel::Block));}key.action=1;
 HitChances input;input.invincible=true;input.miss=10000;input.block=10000;input.critical=10000;check(classify_hit(input,key).contact==Contact::Invincible);
 input.invincible=false;check(classify_hit(input,key).contact==Contact::Miss && !classify_hit(input,key).counts_for_hit_passive());input.miss=0;input.allow_special_block_cap=true;check(classify_hit(input,key).contact==Contact::Blocked && classify_hit(input,key).counts_for_hit_passive());
 input.kind=DamageKind::True;check(classify_hit(input,key).contact==Contact::Connected && !classify_hit(input,key).critical);input.kind=DamageKind::Dot;input.miss=10000;check(classify_hit(input,key).contact==Contact::Connected && !classify_hit(input,key).critical);
 HitOutcome connected;check(debuff_applies(connected,true,false,true,10000,0,10000,key));check(!debuff_applies(connected,true,true,true,10000,10000,0,key));check(!debuff_applies({Contact::Miss,false},true,false,true,10000,0,0,key));check(!debuff_applies({Contact::Blocked,false},true,false,true,10000,0,0,key));
 check(debuff_applies({Contact::Miss,false},false,false,true,10000,0,0,key));check(!debuff_applies(connected,false,false,false,10000,0,10000,key));check(!debuff_applies(connected,false,false,true,0,0,0,key));
 int passed=0;for(std::uint64_t i=0;i<10000;++i){key.action=i;if(debuff_applies(connected,true,false,false,10000,999999,0,key)){++passed;}}check(passed>7700 && passed<8300);
 input.kind=DamageKind::Normal;input.miss=0;input.block=-1;rejected([&]{static_cast<void>(classify_hit(input,key));});rejected([&]{static_cast<void>(debuff_applies(connected,true,false,false,10001,0,0,key));});
 std::cout<<"REG_OUTCOME_TESTS_RESULT passed=9 total=9\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
