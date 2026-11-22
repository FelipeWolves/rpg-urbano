#include "rpg/pve_visual.hpp"
#include <limits>
#include <stdexcept>
namespace rpg::presentation {
PveDefinition ritual_lab_definition(){
 auto setup=felipe_pilot_setup();PveDefinition d;d.id="ritual-lab";d.attempt_id="ritual-lab-1";d.boss_id="ECO-1";
 d.roster=std::move(setup.roster);d.rules=std::move(setup.rules);d.loadouts=std::move(setup.loadouts);
 for(auto& f:d.roster){if(f.id=="ECO-2"){f.role=EntityRole::Auxiliary;}}
 return d;
}
PveDefinition phased_ritual_lab_definition(){
 auto d=ritual_lab_definition();d.id="ritual-phases-lab";d.attempt_id="ritual-phases-1";
 const auto skill=std::ranges::find(d.rules.skills,std::string("ECO-1-A1"),&SkillDefinition::id);
 PhaseMutation fury;fury.kit={*skill};fury.kit[0].effects[0].power_ratio=15000;
 d.phases={{"Vigilia-90",PvePhaseTrigger::HealthThreshold,9000,{}},{"Furia-60",PvePhaseTrigger::HealthThreshold,6000,std::move(fury)}};
 return d;
}
PveDefinition score_lab_definition(){
 auto d=ritual_lab_definition();d.id="score-lab";d.attempt_id="score-lab-1";d.mode=PveMode::ScoreAttack;d.rules.action_budget=ActionBudget{12,LimitOutcome::Draw};
 auto boss=std::ranges::find(d.roster,d.boss_id,&Definition::id);boss->max_health=1000000;return d;
}
PveDefinition survival_lab_definition(){
 auto d=ritual_lab_definition();d.id="survival-lab";d.attempt_id="survival-lab-1";d.mode=PveMode::Survival;d.boss_turn_goal=3;d.boss_turn_clock=TickPolicy::NaturalOnly;d.score_enabled=true;
 auto boss=std::ranges::find(d.roster,d.boss_id,&Definition::id);boss->max_health=1000000;return d;
}
PveDefinition pve_lab_definition(PveLabKind kind){
 switch(kind){case PveLabKind::Ritual:return ritual_lab_definition();case PveLabKind::Phased:return phased_ritual_lab_definition();case PveLabKind::Score:return score_lab_definition();case PveLabKind::Survival:return survival_lab_definition();}
 throw std::invalid_argument("Unknown local PvE variant");
}
std::optional<ScoreReceipt> PveVisualSession::receipt()const{
 if(!finished()){return std::nullopt;}
 const auto id=encounter_->snapshot().attempt_id;const auto& receipts=ledger_->receipts();const auto found=std::ranges::find(receipts,id,&ScoreReceipt::attempt_id);
 return found==receipts.end()?std::nullopt:std::optional<ScoreReceipt>{*found};
}
PveVisualSession::PveVisualSession(PveDefinition definition):definition_(std::move(definition)),ledger_(std::make_unique<ScoreLedger>()),encounter_(std::make_unique<PveEncounter>(definition_,*ledger_)){
 if(definition_.loadouts.empty()){throw std::invalid_argument("Visual PvE requires declared team slots");}
}
VisualState PveVisualSession::state()const{
 auto snapshot=encounter_->snapshot();const auto* actor=encounter_->battle().current_actor();
 return {revision_,{CombatProfile::Team,encounter_->loadouts(),std::move(snapshot.battle),{}},{attempt_,0,{}},actor?encounter_->abilities(actor->definition.id):std::vector<AbilityView>{},
  !finished()&&actor&&actor->definition.team==Team::B,stopped_||finished()};
}
void PveVisualSession::check_capacity()const{if(revision_==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("PvE revision exhausted");}}
VisualResult PveVisualSession::preflight(std::uint64_t revision,bool automatic)const{
 if(revision!=revision_){return {VisualError::StaleCommand,{}};}if(finished()){return {VisualError::Finished,{}};}if(stopped_){return {VisualError::SafetyStop,{}};}
 const auto* actor=encounter_->battle().current_actor();if(!actor||(actor->definition.team==Team::B)!=automatic){return {VisualError::NotPlayerTurn,{}};}return {};
}
VisualResult PveVisualSession::accept(PveActionResult result,bool automatic){
 if(!result.action.accepted()){return {VisualError::CoreRejected,std::move(result.action)};}
 if(!result.phase_events.empty()){phase_events_=std::move(result.phase_events);}
 ++revision_;automatic_count_=automatic?automatic_count_+1:0;static_cast<void>(cues_.observe(revision_,result.action));return {VisualError::None,std::move(result.action)};
}
VisualResult PveVisualSession::submit(std::uint64_t revision,std::uint32_t slot,std::string target){
 const auto check=preflight(revision,false);if(!check.accepted()){return check;}check_capacity();return accept(encounter_->apply_slot(slot,std::move(target)),false);
}
VisualResult PveVisualSession::submit_action(std::uint64_t revision,const Action& action){
 const auto check=preflight(revision,false);if(!check.accepted()){return check;}check_capacity();return accept(encounter_->apply(action),false);
}
VisualResult PveVisualSession::step_automatic(){
 const auto check=preflight(revision_,true);if(!check.accepted()){return check;}if(automatic_count_>=256){stopped_=true;return {VisualError::SafetyStop,{}};}
 check_capacity();const auto action=encounter_->choose_automatic_action();if(!action){stopped_=true;return {VisualError::SafetyStop,{}};}
 auto result=accept(encounter_->apply(*action),true);if(!result.accepted()){stopped_=true;}return result;
}
void PveVisualSession::reset(){
 check_capacity();if(attempt_==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("PvE attempt exhausted");}
 auto next=definition_;next.attempt_id=definition_.attempt_id+"-retry-"+std::to_string(attempt_+1);
 auto replacement=std::make_unique<PveEncounter>(std::move(next),*ledger_);encounter_=std::move(replacement);++attempt_;++revision_;automatic_count_=0;stopped_=false;cues_.clear();phase_events_.clear();
}
namespace {
WaveVisualDefinition validate_waves(WaveVisualDefinition d){
 if(d.loadouts.size()!=d.stage.encounters.size()){throw std::invalid_argument("Wave loadouts must match encounters");}
 for(std::size_t i=0;i<d.loadouts.size();++i){
  auto roster=d.stage.squad;roster.insert(roster.end(),d.stage.encounters[i].enemies.begin(),d.stage.encounters[i].enemies.end());
  for(const auto& set:d.loadouts[i]){for(const auto& entry:set.slots){if(entry.passive&&entry.passive->trigger==PassiveTrigger::BattleStart){throw std::invalid_argument("Wave opening passives require explicit stage runtime initialization");}}}
  ProfileBattle profile(roster,d.stage.encounters[i].rules,CombatProfile::Team,d.loadouts[i]);
  d.stage.encounters[i].rules=profile.snapshot().battle.rules;d.loadouts[i]=profile.snapshot().loadouts;
 }
 return d;
}
}
WaveVisualDefinition wave_lab_definition(){
 const auto source=felipe_pilot_setup();const auto ally=*std::ranges::find(source.roster,std::string("FELIPE"),&Definition::id);
 const auto ally_set=*std::ranges::find(source.loadouts,std::string("FELIPE"),&CharacterLoadout::fighter_id);
 std::vector<EncounterDefinition> waves;std::vector<std::vector<CharacterLoadout>> sets;
 for(const auto& id:{std::string("ECO-1"),std::string("ECO-2")}){
  const auto enemy=*std::ranges::find(source.roster,id,&Definition::id);auto rules=source.rules;
  std::erase_if(rules.skills,[&](const SkillDefinition& s){return s.owner!="FELIPE"&&s.owner!=id;});
  waves.push_back({"onda-"+id,{enemy},std::move(rules)});
  sets.push_back({ally_set,*std::ranges::find(source.loadouts,id,&CharacterLoadout::fighter_id)});
 }
 return {StageDefinition{"waves-lab",2,{ally},std::move(waves),CarryPolicy::PreserveHealth,ActionBudget{200,LimitOutcome::Draw},RuntimeCarry::WaveV2},std::move(sets)};
}
WaveVisualSession::WaveVisualSession(WaveVisualDefinition definition):definition_(validate_waves(std::move(definition))),stage_(definition_.stage){}
VisualState WaveVisualSession::state()const{
 const auto* actor=stage_.battle().current_actor();return {revision_,{CombatProfile::Team,loadouts(),stage_.battle().snapshot(),{}},{attempt_,0,{}},actor?team_abilities(stage_.battle(),loadouts(),actor->definition.id):std::vector<AbilityView>{},!finished()&&actor&&actor->definition.team==Team::B,stopped_||finished()};
}
WaveHud WaveVisualSession::hud()const{
 const auto s=stage_.snapshot();std::string status="Em andamento";
 if(s.end_reason){switch(*s.end_reason){case StageEndReason::Completed:status="Todas as ondas concluidas";break;case StageEndReason::GlobalActionLimit:status="Limite global de acoes atingido";break;case StageEndReason::TechnicalLimit:status="Interrupcao por limite tecnico";break;case StageEndReason::NoSurvivors:status="Sem sobreviventes";break;case StageEndReason::EncounterFailure:status="Encontro encerrado sem completar as ondas";break;}}
 return {s.stage_id,s.encounter_id,std::move(status),s.encounter_index+1,definition_.stage.encounters.size(),s.accepted_actions,definition_.stage.global_budget?std::optional<std::uint64_t>{static_cast<std::uint64_t>(definition_.stage.global_budget->max_actions)}:std::nullopt,stage_.remaining_actions(),s.battle.accepted_actions,stage_.battle().remaining_actions(),finished(),s.end_reason==StageEndReason::Completed,previous_wave_};
}
void WaveVisualSession::check_capacity()const{if(revision_==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("Wave revision exhausted");}}
VisualResult WaveVisualSession::preflight(std::uint64_t revision,bool automatic)const{
 if(revision!=revision_){return {VisualError::StaleCommand,{}};}if(finished()){return {VisualError::Finished,{}};}if(stopped_){return {VisualError::SafetyStop,{}};}
 const auto* actor=stage_.battle().current_actor();if(!actor||(actor->definition.team==Team::B)!=automatic){return {VisualError::NotPlayerTurn,{}};}return {};
}
VisualResult WaveVisualSession::accept(StageActionResult result,bool automatic){
 if(!result.action.accepted()){return {VisualError::CoreRejected,std::move(result.action)};}
 if(result.encounter_advanced){previous_wave_=result.encounter_id;transition_events_=std::move(result.transition_events);}
 ++revision_;automatic_count_=automatic?automatic_count_+1:0;static_cast<void>(cues_.observe(revision_,result.action));return {VisualError::None,std::move(result.action)};
}
VisualResult WaveVisualSession::submit(std::uint64_t revision,std::uint32_t slot,std::string target){
 const auto check=preflight(revision,false);if(!check.accepted()){return check;}check_capacity();auto prepared=prepare_team_slot(stage_.battle(),loadouts(),slot,std::move(target));if(!prepared.action){return {VisualError::CoreRejected,ActionResult{prepared.error}};}return accept(stage_.apply(*prepared.action),false);
}
VisualResult WaveVisualSession::submit_action(std::uint64_t revision,const Action& action){
 const auto check=preflight(revision,false);if(!check.accepted()){return check;}check_capacity();auto prepared=prepare_team_action(stage_.battle(),loadouts(),action);if(!prepared.action){return {VisualError::CoreRejected,ActionResult{prepared.error}};}return accept(stage_.apply(*prepared.action),false);
}
VisualResult WaveVisualSession::step_automatic(){
 const auto check=preflight(revision_,true);if(!check.accepted()){return check;}if(automatic_count_>=256){stopped_=true;return {VisualError::SafetyStop,{}};}
 check_capacity();const auto action=choose_team_action(stage_.battle(),loadouts());if(!action){stopped_=true;return {VisualError::SafetyStop,{}};}auto result=accept(stage_.apply(*action),true);if(!result.accepted()){stopped_=true;}return result;
}
void WaveVisualSession::reset(){check_capacity();if(attempt_==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("Wave attempt exhausted");}stage_.reset();++revision_;++attempt_;automatic_count_=0;stopped_=false;cues_.clear();transition_events_.clear();previous_wave_.reset();}}