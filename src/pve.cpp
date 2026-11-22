#include "rpg/pve.hpp"
#include <algorithm>
#include <stdexcept>
namespace rpg {
PveEncounter::PveEncounter(PveDefinition definition,ScoreLedger& ledger):definition_(std::move(definition)),ledger_(&ledger),score_(definition_.score){
 if(definition_.id.empty()||definition_.attempt_id.empty()||definition_.boss_id.empty()||definition_.attempt_id.size()>128||definition_.rules.timing!=TimingProfile::ActionBar||
    (definition_.mode!=PveMode::Ritual&&definition_.mode!=PveMode::ScoreAttack&&definition_.mode!=PveMode::Survival)||
    (definition_.boss_turn_clock!=TickPolicy::NaturalOnly&&definition_.boss_turn_clock!=TickPolicy::EveryTurn)||
    (definition_.mode==PveMode::ScoreAttack&&!definition_.rules.action_budget)||
    (definition_.mode==PveMode::Survival&&(!definition_.boss_turn_goal||*definition_.boss_turn_goal==0))){throw std::invalid_argument("Invalid PvE mode contract");}
 ScoreLedger validation;static_cast<void>(validation.settle(definition_.attempt_id,score_,false,definition_.tiers));
 if(std::ranges::find(ledger.receipts(),definition_.attempt_id,&ScoreReceipt::attempt_id)!=ledger.receipts().end()){throw std::invalid_argument("Attempt already settled");}
 if(std::ranges::count_if(definition_.roster,[](const Definition& f){return f.team==Team::A&&f.role!=EntityRole::Summon;})>5 || definition_.roster.size()>128){throw std::invalid_argument("PvE formation/resource bound");}
 const auto boss=std::ranges::find(definition_.roster,definition_.boss_id,&Definition::id);
 if(boss==definition_.roster.end()||boss->team!=Team::B){throw std::invalid_argument("PvE requires a declared enemy boss");}
 boss->role=EntityRole::Boss;boss->is_boss=true;
 definition_.rules.required_enemy=definition_.boss_id;definition_.rules.guard_enabled=false;
 std::vector<InitialHealth> health;std::vector<std::string> reserves;
 for(const auto& f:definition_.roster){const bool reserved=std::ranges::find(definition_.reserve_summons,f.id)!=definition_.reserve_summons.end();if(reserved){if(f.role!=EntityRole::Summon){throw std::invalid_argument("Only summons are reserves");}reserves.push_back(f.id);}health.push_back({f.id,reserved?0:f.max_health});}
 if(reserves.size()!=definition_.reserve_summons.size()){throw std::invalid_argument("Unknown/duplicate reserve");}
 battle_.emplace(definition_.roster,definition_.rules,health);
 if(definition_.turn_cycle&&(!definition_.phases.empty()||definition_.turn_cycle->every==0||definition_.turn_cycle->states.size()<2)){throw std::invalid_argument("Cycle needs period/states and separate phase family");}
 std::vector<std::string> phases;auto phase_validation=*battle_;std::uint64_t last_hp=10000,last_turn=0;
 for(const auto& p:definition_.phases){if(p.id.empty()||std::ranges::find(phases,p.id)!=phases.end()||p.threshold==0||
    (p.trigger!=PvePhaseTrigger::HealthThreshold&&p.trigger!=PvePhaseTrigger::BossTurnCount)||
    (p.trigger==PvePhaseTrigger::HealthThreshold&&p.threshold>=10000)){throw std::invalid_argument("Invalid phase trigger");}if(p.trigger==PvePhaseTrigger::HealthThreshold){if(p.threshold>=last_hp){throw std::invalid_argument("HP thresholds must descend");}last_hp=p.threshold;}else{if(p.threshold<=last_turn){throw std::invalid_argument("Turn thresholds must ascend");}last_turn=p.threshold;}phases.push_back(p.id);auto mutation=p.mutation;for(const auto& id:mutation.activate_summons){const auto* f=battle_->find(id);if(!f||f->definition.role!=EntityRole::Summon){throw std::invalid_argument("Invalid declared reserve");}}mutation.activate_summons.clear();static_cast<void>(phase_validation.transition_phase(definition_.boss_id,mutation));}
 if(definition_.turn_cycle){for(const auto& p:definition_.turn_cycle->states){if(p.first.empty()){throw std::invalid_argument("Empty cycle phase");}auto check=*battle_;static_cast<void>(check.transition_phase(definition_.boss_id,p.second));}}
}
PveSnapshot PveEncounter::snapshot()const{
 return {definition_.id,definition_.attempt_id,definition_.boss_id,definition_.mode,battle_->snapshot(),phase_id_,boss_turns_,definition_.boss_turn_goal,definition_.boss_turn_clock,battle_->remaining_actions(),
  (definition_.score_enabled||definition_.mode==PveMode::ScoreAttack)?std::optional<numeric::Quantity>{score()}:std::nullopt,finished(),objective_,end_};
}
void PveEncounter::update_phases(std::vector<PvePhaseEvent>& events){
 while(next_phase_<definition_.phases.size()){
  const auto& boss=*battle_->find(definition_.boss_id);
  const auto& p=definition_.phases[next_phase_];const bool ready=p.trigger==PvePhaseTrigger::HealthThreshold?
    numeric::compare_ratio(boss.health,boss.definition.max_health,static_cast<std::int64_t>(p.threshold),10000)<=0:boss_turns_>=p.threshold;
  if(!ready){break;}events.push_back({p.id,battle_->transition_phase(definition_.boss_id,p.mutation)});phase_id_=p.id;++next_phase_;
 }
 if(definition_.turn_cycle){
  const auto& cycle=*definition_.turn_cycle;const auto reached=boss_turns_/cycle.every;
  while(cycle_steps_<reached){const auto& p=cycle.states[static_cast<std::size_t>(cycle_steps_%cycle.states.size())];events.push_back({p.first,battle_->transition_phase(definition_.boss_id,p.second)});phase_id_=p.first;++cycle_steps_;}
 }
}
bool PveEncounter::alive()const{return std::ranges::any_of(battle_->fighters(),[](const Fighter& f){return f.definition.team==Team::A&&f.definition.role!=EntityRole::Summon&&f.health>0;});}
void PveEncounter::finish(PveEnd reason,bool objective){
 if(end_){return;}objective_=objective;end_=reason;
 if(definition_.score_enabled||definition_.mode==PveMode::ScoreAttack){static_cast<void>(ledger_->settle(definition_.attempt_id,score_,alive(),reason==PveEnd::TechnicalLimit?std::vector<ScoreTier>{}:definition_.tiers));}
}
PveActionResult PveEncounter::apply(const Action& action){
 PveActionResult result;
 if(end_){result.action.error=ActionError::BattleFinished;result.attempt_finished=true;result.end=end_;result.objective_met=objective_;result.score=score();return result;}
 result.action=battle_->apply(action);if(!result.action.accepted()){return result;}
 try{if(definition_.score_enabled||definition_.mode==PveMode::ScoreAttack){static_cast<void>(score_.observe(1,result.action,battle_->fighters()));}}catch(const std::length_error&){finish(PveEnd::TechnicalLimit,false);}
 for(const auto& event:result.action.events){if(event.type==EventType::TurnEnded&&event.actor==definition_.boss_id&&(event.turn_kind==TurnKind::Natural||definition_.boss_turn_clock==TickPolicy::EveryTurn)){++boss_turns_;}}
 if(!end_ && battle_->phase()==Phase::Running){update_phases(result.phase_events);}
 if(!end_){
  if(battle_->phase()==Phase::Finished){
   if(battle_->end_reason()==EndReason::TechnicalLimit){finish(PveEnd::TechnicalLimit,false);}
   else if(battle_->end_reason()==EndReason::ActionLimit){finish(PveEnd::WindowClosed,false);}
   else{const bool won=battle_->winner()==Team::A;finish(won?PveEnd::BossDefeated:PveEnd::TeamDefeated,definition_.mode==PveMode::Ritual?won:false);}
  }else if(definition_.mode==PveMode::Survival && boss_turns_>=*definition_.boss_turn_goal){finish(PveEnd::SurvivalReached,alive());}
 }
 result.attempt_finished=finished();result.end=end_;result.objective_met=objective_;result.score=score();return result;
}
ScoreSequence::ScoreSequence(SequentialDefinition definition,std::string attempt_id,ScoreLedger& ledger,ScorePolicy policy,std::vector<ScoreTier> tiers)
 :encounter_(std::move(definition)),attempt_(std::move(attempt_id)),ledger_(&ledger),score_(policy),tiers_(std::move(tiers)){
 if(attempt_.empty()||std::ranges::find(ledger.receipts(),attempt_,&ScoreReceipt::attempt_id)!=ledger.receipts().end()){throw std::invalid_argument("Invalid/reused sequence attempt");}
}
SequentialActionResult ScoreSequence::apply(const Action& action){
 const auto before=encounter_.snapshot();const auto roster=encounter_.battle().fighters();
 auto result=encounter_.apply(action);if(!result.action.accepted()){return result;}
 static_cast<void>(score_.observe(before.squad_index+1,result.action,roster));
 if(result.stage_finished&&!finished_){
  finished_=true;survived_=std::ranges::any_of(encounter_.battle().fighters(),[](const Fighter& f){return f.definition.team==Team::A&&f.definition.role!=EntityRole::Summon&&f.health>0;});
  static_cast<void>(ledger_->settle(attempt_,score_,survived_,encounter_.snapshot().end_reason==SequentialEndReason::TechnicalLimit?std::vector<ScoreTier>{}:tiers_));
 }
 return result;
}
}
