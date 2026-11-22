#include "rpg/battle_presentation.hpp"
#include "rpg/stage.hpp"
#include "rpg/sequential.hpp"
#include "rpg/shared_boss.hpp"
#include "rpg/save_game.hpp"
#include <iostream>
#include <limits>
#include <algorithm>
using namespace rpg;using namespace rpg::presentation;
void check(bool v){if(!v){throw std::runtime_error("Wide quantity contract failed");}}
template<class F>void rejected(F f){bool caught=false;try{f();}catch(const std::invalid_argument&){caught=true;}check(caught);}
int main(){try{
 constexpr auto cap=numeric::quantity_max;constexpr std::int64_t damage=5000000000LL;
 Battle b({{"A",Team::A,cap,damage,100},{"B",Team::B,cap,1,90}});auto hit=b.apply({"A",ActionType::Attack,"B"});check(hit.accepted() && b.find("B")->health==cap-damage);check(std::ranges::any_of(hit.events,[](const Event& e){return e.type==EventType::Attack && e.amount==damage;}));
 check(b.apply({"B",ActionType::Guard,{}}).accepted());check(b.apply({"A",ActionType::Attack,"B"}).accepted() && b.find("B")->health==cap-damage-damage/2);
 BattleRules rules;rules.skills={{"shield","A",0,TargetRule::Ally,{{EffectKind::StandardShield}}}};
 Battle shield({{"A",Team::A,cap,1,100},{"C",Team::A,100,1,10},{"B",Team::B,cap,damage,90}},rules);check(shield.apply({"A",ActionType::Skill,"C","shield"}).accepted());auto protection=std::ranges::find(shield.find("C")->statuses,EffectKind::StandardShield,&Status::kind);check(protection!=shield.find("C")->statuses.end() && protection->magnitude==10000000000LL);check(shield.apply({"B",ActionType::Attack,"C"}).accepted() && shield.find("C")->health==100 && shield_display(*shield.find("C")).points==damage);
 InitialRuntime reflection{"B",90,{{EffectKind::Reflect,2,TickPolicy::NaturalOnly,0,3000000000LL}}, {}};
 Battle reflected({{"A",Team::A,cap,damage,100},{"B",Team::B,cap,1,90}},{},{},{reflection});check(reflected.apply({"A",ActionType::Attack,"B"}).accepted() && reflected.find("A")->health==cap-3000000000LL);
 Battle capped({{"A",Team::A,std::numeric_limits<std::int64_t>::max(),std::numeric_limits<std::int64_t>::max(),100},{"B",Team::B,cap,1,90}});check(capped.find("A")->health==cap && capped.find("A")->definition.attack==cap);check(capped.apply({"A",ActionType::Attack,"B"}).accepted() && capped.find("B")->health==0);
 rejected([]{Battle invalid({{"A",Team::A,-1,1,2},{"B",Team::B,1,1,1}});});auto bad=rules;bad.numeric_model=1;rejected([&]{Battle invalid({{"A",Team::A,100,1,100},{"B",Team::B,100,1,90}},bad);});
 StageDefinition stageDef{"wide-stage",1,{{"A",Team::A,cap,damage,100}},{{"first",{{"B1",Team::B,1,1,90}},{}},{"second",{{"B2",Team::B,cap,1,90}},{}}},CarryPolicy::PreserveHealth};Stage stage(stageDef);check(stage.apply({"A",ActionType::Attack,"B1"}).encounter_advanced && stage.battle().find("A")->health==cap);
 auto setup=team_reference_setup();for(auto& unit:setup.roster){unit.max_health=cap;unit.attack=damage;}setup.loadouts[1].slots[1].passive->shield=cap+1;VisualSession visual(setup);check(visual.state().combat.battle.fighters.front().health==cap);const auto bytes=visual.export_save();check(bytes.starts_with("RPG_URBANO_SAVE 1"));VisualSession restored(setup);restored.import_save(bytes);check(restored.state().combat.battle.fighters.front().health==cap && restored.state().progress.credits==0);
 InitialRuntime revive{"B",90,{{EffectKind::AutoRevive,2,TickPolicy::NaturalOnly,0,damage}}, {}};
 Battle revived({{"A",Team::A,cap,cap,100},{"B",Team::B,cap,1,90}},{},{},{revive});auto revivedHit=revived.apply({"A",ActionType::Attack,"B"});check(revivedHit.accepted() && revived.find("B")->health==damage && revived.phase()==Phase::Running);
 std::vector<SquadDefinition> squads;std::vector<SharedBossSquad> fronts;
 for(int i=1;i<=3;++i){const auto id="A"+std::to_string(i);BattleRules r;r.timing=TimingProfile::ActionBar;squads.push_back({"s"+std::to_string(i),{{"char"+id,{id,Team::A,cap,damage,200}}},r});fronts.push_back({"front"+std::to_string(i),{{"char"+id,{id,Team::A,cap,damage,200}}}});}
 SequentialSquads sequence({"wide-seq",1,squads,{"boss",Team::B,cap,1,100},BossRuntimeCarry::KeepCooldownsClearStatuses});check(sequence.apply({"A1",ActionType::Attack,"boss"}).action.accepted() && sequence.battle().find("boss")->health==cap-damage);
 BattleRules sharedRules;sharedRules.timing=TimingProfile::ActionBar;SharedBoss shared({"wide-shared",1,fronts,{"boss",Team::B,cap,1,100},sharedRules});check(shared.apply({"A1",ActionType::Attack,"boss"}).accepted() && shared.battle().find("boss")->health==cap-damage);
 auto f=*shield.find("C");f.statuses={{EffectKind::Shield,1,TickPolicy::NaturalOnly,0,cap},{EffectKind::StructuralShield,0,TickPolicy::NaturalOnly,0,cap,1,StatusCategory::Structural}};check(shield_display(f).points==2*cap && shield_display(f).overlay_fraction==1.0);
 std::cout<<"WIDE_NUMERIC_TESTS_RESULT passed=9 total=9\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
