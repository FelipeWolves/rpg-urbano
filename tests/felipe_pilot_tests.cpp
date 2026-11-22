#include "rpg/reference_encounter.hpp"
#include "rpg/visual_session.hpp"
#include "rpg/battle_presentation.hpp"
#include <iostream>
#include <stdexcept>
using namespace rpg;
void check(bool v,const char* m){if(!v){throw std::runtime_error(m);}}
int main(){try{
 check(team_reference_setup().rules.combat_model==resolution::CombatModel::FunnelV1 && felipe_pilot_setup().rules.combat_model==team_reference_setup().rules.combat_model,"shared general funnel");
 check(classic_reference_setup().rules.combat_model==resolution::CombatModel::Legacy,"classic baseline preserved");
 const auto terminalLab=terminal_lab_setup();check(terminalLab.roster[0].max_health==10000 && terminalLab.roster[0].attack==500 && terminalLab.rules.combat_model==resolution::CombatModel::FunnelV1,"terminal visual health scale");
 check(team_reference_setup().roster[0].max_health==100,"small baseline preserved");
 check(terminal_lab_setup().rules.action_budget==felipe_pilot_setup().rules.action_budget,"shared laboratory action limit");
 // Hub scenario switch preserves both sessions rather than importing their saves.
 presentation::VisualSession hubReference(team_reference_setup()),hubPilot(felipe_pilot_setup());
 const auto referenceBefore=hubReference.state();const auto pilotBefore=hubPilot.state();
 std::optional<presentation::VisualSession> alternate(std::move(hubPilot));
 std::swap(hubReference,*alternate);
 check(hubReference.state().combat==pilotBefore.combat && alternate->state().combat==referenceBefore.combat,"hub switch isolates encounters");
 const auto pilotStep=hubReference.submit(hubReference.state().revision,1,"ECO-1");check(pilotStep.accepted(),"hub pilot executable after move");
 const auto progressedPilot=hubReference.state();std::swap(hubReference,*alternate);
 check(hubReference.state().combat==referenceBefore.combat && alternate->state().combat==progressedPilot.combat,"hub return preserves both battles");
 auto shieldFixture=team_reference_setup();for(auto& unit:shieldFixture.roster){unit.speed=1;}shieldFixture.roster[0].speed=200;
 ProfileBattle shieldLab(shieldFixture.roster,shieldFixture.rules,CombatProfile::Team,shieldFixture.loadouts);
 check(shieldLab.battle().current_actor()->definition.id=="A1","shield fixture actor");
 const auto shieldBefore=shieldLab.snapshot();check(!shieldLab.apply_slot(3,"B1").accepted() && shieldLab.snapshot()==shieldBefore,"reference A3 rejects enemy");
 check(shieldLab.apply_slot(3,"A1").accepted(),"reference A3 accepts self");
 check(presentation::shield_display(*shieldLab.battle().find("A1")).points==1,"reference A3 gives one shield point");
 std::cout<<"HUB_LAB_SWITCH_TESTS_RESULT passed=2 total=2\n";
 auto setup=felipe_pilot_setup();ProfileBattle profile(setup.roster,setup.rules,CombatProfile::Team,setup.loadouts);const auto abilities=profile.abilities("FELIPE");check(abilities.size()==3&&abilities[1].availability==AbilityAvailability::PassiveOnly&&abilities[1].passive_implemented,"passive slot");const auto before=profile.snapshot();check(!profile.apply_slot(2,"ECO-1").accepted()&&profile.snapshot()==before,"A2 not selectable");check(profile.battle().find("FELIPE")->vigil_charges==0,"normal initial charges");
 auto tutorial=felipe_pilot_setup(true);ProfileBattle lesson(tutorial.roster,tutorial.rules,CombatProfile::Team,tutorial.loadouts);check(lesson.battle().find("FELIPE")->vigil_charges==4,"tutorial preload explicit");
 Battle a1(setup.roster,setup.rules);auto result=a1.apply({"FELIPE",ActionType::Skill,"ECO-1","FELIPE-A1"});check(result.accepted()&&a1.find("ECO-1")->health==9250&&!has_status(*a1.find("ECO-1"),EffectKind::Bleed),"110 percent without condition");
 auto conditionSetup=setup;auto& bleed=conditionSetup.rules.skills[0].effects[1];bleed.irresistible=true;InitialRuntime marked{"ECO-1",100,{{EffectKind::Silence,3,TickPolicy::NaturalOnly,0}},{{"ECO-1-A1",0}}};Battle conditional(conditionSetup.roster,conditionSetup.rules,{}, {marked});result=conditional.apply({"FELIPE",ActionType::Skill,"ECO-1","FELIPE-A1"});check(result.accepted()&&has_status(*conditional.find("ECO-1"),EffectKind::Bleed),"conditional bleed");check(std::ranges::any_of(result.events,[](const Event& e){return e.type==EventType::EffectApplied&&e.actor=="FELIPE"&&e.target=="FELIPE"&&e.amount==static_cast<int>(EffectKind::ChangeAP);}),"conditional self AP");check(conditional.find("ECO-1")->health==8000,"snapshotted 1250 tick");
 Battle ultimate(setup.roster,setup.rules);result=ultimate.apply({"FELIPE",ActionType::Skill,"ECO-1","FELIPE-A3"});check(result.accepted()&&ultimate.find("ECO-1")->health==5907&&ultimate.find("ECO-2")->health==1907,"three AoE hits 65 percent plus 25 percent");check(std::ranges::count_if(result.events,[](const Event& e){return e.type==EventType::Attack;})==6,"six hit reports");check(ultimate.find("FELIPE")->cooldowns[1].remaining==3,"cooldown four includes use turn");
 InitialRuntime buffed{"ECO-1",100,{{EffectKind::Shield,3,TickPolicy::NaturalOnly,0,5000},{EffectKind::Counter,3,TickPolicy::NaturalOnly,0},{EffectKind::Reflect,3,TickPolicy::NaturalOnly,0,1}},{{"ECO-1-A1",0}}};Battle dispelled(setup.roster,setup.rules,{}, {buffed});result=dispelled.apply({"FELIPE",ActionType::Skill,"ECO-1","FELIPE-A3"});check(result.accepted()&&!has_status(*dispelled.find("ECO-1"),EffectKind::Shield)&&!has_status(*dispelled.find("ECO-1"),EffectKind::Counter)&&has_status(*dispelled.find("ECO-1"),EffectKind::Reflect),"dispel up to two common buffs");check(dispelled.find("ECO-1")->health==7125,"remaining buff prevents bonus");
 presentation::VisualSession session(setup);check(presentation::target_candidates(session.state(),3).size()==2,"area selection candidates");auto original=session.state();check(!session.submit(original.revision,2,"ECO-1").accepted()&&session.state().revision==original.revision,"passive rejection keeps revision");
 for(int i=0;i<200&&session.state().combat.battle.phase==Phase::Running;++i){const auto state=session.state();if(state.automatic){check(session.step_automatic().accepted(),"enemy automated step");}else{auto targets=presentation::target_candidates(state,3);std::uint32_t slot=3;if(targets.empty()){slot=1;targets=presentation::target_candidates(state,1);}check(!targets.empty()&&session.submit(state.revision,slot,targets.front()).accepted(),"player action");}}
 check(session.state().combat.battle.phase==Phase::Finished&&session.state().progress.completions.size()==1,"pilot full session and settlement");const auto bytes=session.export_save();presentation::VisualSession restored(setup);restored.import_save(bytes);check(restored.state().progress.completions==session.state().progress.completions&&restored.team().selected==session.team().selected,"save roundtrip");const auto pristine=restored.state();bool rejected=false;try{restored.import_save("invalid");}catch(const SaveError&){rejected=true;}check(rejected&&restored.state().combat==pristine.combat,"invalid save atomic");
 Battle replayA(setup.roster,setup.rules),replayB(setup.roster,setup.rules);for(int i=0;i<200&&replayA.phase()==Phase::Running;++i){const auto action=choose_basic_attack(replayA);check(action&&replayA.apply(*action)==replayB.apply(*action)&&replayA.snapshot()==replayB.snapshot(),"exact replay");}
 check(!presentation::effect_details(*replayA.find("FELIPE")).empty(),"charge details");
 std::cout<<"REG_PILOT_TESTS_RESULT passed=10 total=10\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
