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
 auto replacement=std::make_unique<PveEncounter>(std::move(next),*ledger_);encounter_=std::move(replacement);++attempt_;++revision_;automatic_count_=0;stopped_=false;cues_.clear();
}
}