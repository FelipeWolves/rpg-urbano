#include "rpg/damage_pipeline.hpp"
#include "rpg/battle.hpp"
#include <array>
#include <iostream>
using namespace rpg::resolution;
void check(bool v){if(!v){throw std::runtime_error("Damage pipeline contract failed");}}
int main(){try{
 RollKey key{1,1,1,identity("B"),0};DamageInput input;input.base=3000;input.ratio=15000;input.attacker.critical_chance=10000;input.attacker.critical_total=20000;input.attacker.damage_bonus=1000;input.defender.block=0;input.defender.defense=1430;check(calculate_damage(input,key).amount==8910);
 input.kind=DamageKind::True;input.defender.invincible=false;input.defender.block=10000;input.defender.special_block_cap=true;input.defender.mitigation=5000;input.guarding=true;check(calculate_damage(input,key).amount==4950);input.defender.invincible=true;check(calculate_damage(input,key).amount==0);
 input=DamageInput{};input.base=1;input.defender.block=0;input.guarding=true;check(calculate_damage(input,key).amount==0);input.guarding=false;input.defender.mitigation=5000;check(calculate_damage(input,key).amount==1);
 input=DamageInput{};input.base=100;input.defender.block=0;input.attacker.miss=10000;auto miss=calculate_damage(input,key);check(miss.amount==75 && miss.outcome.contact==Contact::Miss && !miss.outcome.counts_for_hit_passive());
 std::array<Factor,2> factors{{{2,1},{1,10}}};check(product_floor(1000000000000LL,factors)==200000000000LL);std::array<Factor,2> tiny{{{1,2},{2,1}}};check(product_floor(1,tiny)==1);
 input=DamageInput{};input.base=1000000000000LL;input.ratio=20000;input.defender.block=0;input.guarding=true;check(calculate_damage(input,key).amount==1000000000000LL);
 std::vector<Factor> wide(14,{9000000000000000000LL,9000000000000000000LL});wide.push_back({1,2});wide.push_back({1,1});check(product_floor(1000000000000LL,wide)==500000000000LL);
 for(std::int64_t a=0;a<=50;++a){for(std::int64_t b=0;b<=20;++b){for(std::int64_t d=1;d<=20;++d){std::array<Factor,2> f{{{b,d},{7,3}}};check(product_floor(a,f)==a*b*7/(d*3));}}}
 rpg::BattleRules rules;rules.combat_model=CombatModel::FunnelV1;
 rpg::Definition a{"A",rpg::Team::A,1000,20,100},b{"B",rpg::Team::B,1000,20,90};b.stats.block=0;
 rpg::Battle battle({a,b},rules);auto hit=battle.apply({"A",rpg::ActionType::Attack,"B"});check(hit.accepted() && battle.find("B")->health==980);auto event=std::ranges::find(hit.events,rpg::EventType::Attack,&rpg::Event::type);check(event!=hit.events.end() && event->hit && event->hit->attempted==20 && event->hit->health_damage==20);
 rpg::InitialRuntime shield{"B",90,{{rpg::EffectKind::Shield,2,rpg::TickPolicy::NaturalOnly,0,30}}, {}};rpg::Battle protectedBattle({a,b},rules,{}, {shield});hit=protectedBattle.apply({"A",rpg::ActionType::Attack,"B"});event=std::ranges::find(hit.events,rpg::EventType::Attack,&rpg::Event::type);check(event->hit && event->hit->absorbed==20 && event->hit->health_damage==0 && protectedBattle.find("B")->health==1000);
 b.stats.block=10000;b.stats.special_block_cap=true;rpg::Battle blocked({a,b},rules);hit=blocked.apply({"A",rpg::ActionType::Attack,"B"});event=std::ranges::find(hit.events,rpg::EventType::Attack,&rpg::Event::type);check(event->hit->outcome.contact==Contact::Blocked && event->hit->health_damage==0 && event->hit->outcome.counts_for_hit_passive());
 b.stats.invincible=true;rpg::Battle immune({a,b},rules);hit=immune.apply({"A",rpg::ActionType::Attack,"B"});event=std::ranges::find(hit.events,rpg::EventType::Attack,&rpg::Event::type);check(event->hit->outcome.contact==Contact::Invincible && !event->hit->outcome.counts_for_hit_passive());
 rpg::Battle legacy({a,b});hit=legacy.apply({"A",rpg::ActionType::Attack,"B"});event=std::ranges::find(hit.events,rpg::EventType::Attack,&rpg::Event::type);check(!event->hit && legacy.find("B")->health==980);
 b.stats.invincible=false;b.stats.block=300;rpg::Battle first({a,b},rules),second({a,b},rules);check(!first.apply({"missing",rpg::ActionType::Attack,"B"}).accepted());for(int i=0;i<10;++i){auto action=rpg::choose_basic_attack(first);check(action && first.apply(*action)==second.apply(*action) && first.snapshot()==second.snapshot());}
 std::cout<<"REG_PIPELINE_TESTS_RESULT passed=10 total=10\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
