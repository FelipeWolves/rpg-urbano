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
 const auto boss=std::ranges::find(definition_.roster,definition_.boss_id,&Definition::id);
 if(boss==definition_.roster.end()||boss->team!=Team::B){throw std::invalid_argument("PvE requires a declared enemy boss");}
 boss->role=EntityRole::Boss;boss->is_boss=true;
 definition_.rules.required_enemy=definition_.boss_id;definition_.rules.guard_enabled=false;
 battle_.emplace(definition_.roster,definition_.rules);
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
