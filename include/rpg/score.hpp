#pragma once
#include "rpg/battle.hpp"
#include <algorithm>
#include <stdexcept>
namespace rpg {
enum class ScoreModifierOrder { BonusBeforeDifficulty, BonusAfterDifficulty };
struct ScorePolicy {
 Team credited_team=Team::A;
 numeric::Quantity survival_bonus=0;
 numeric::BasisPoints difficulty=10000;
 ScoreModifierOrder order=ScoreModifierOrder::BonusBeforeDifficulty;
 bool operator==(const ScorePolicy&) const=default;
};
struct ScoreDamage {
 std::size_t ordinal;
 std::string source,target;
 numeric::Quantity effective,overkill;
 bool operator==(const ScoreDamage&) const=default;
};
struct ScoreBatch {
 std::uint64_t epoch,action;
 std::vector<ScoreDamage> damage;
 bool operator==(const ScoreBatch&) const=default;
};
class ScoreAccumulator {
public:
 explicit ScoreAccumulator(ScorePolicy policy={}):policy_(policy){
  if((policy.credited_team!=Team::A&&policy.credited_team!=Team::B)||policy.survival_bonus<0||policy.survival_bonus>numeric::quantity_max||policy.difficulty<0||policy.difficulty>numeric::quantity_max||
     (policy.order!=ScoreModifierOrder::BonusBeforeDifficulty&&policy.order!=ScoreModifierOrder::BonusAfterDifficulty)){throw std::invalid_argument("Invalid score policy");}
 }
 // Epoch separates encounters/squads whose Battle action IDs restart. Rejected commands never score.
 bool observe(std::uint64_t epoch,const ActionResult& result,const std::vector<Fighter>& roster){
  if(!result.accepted()){return false;}if(epoch==0||!result.action_context){throw std::invalid_argument("Score requires epoch and accepted action identity");}
  ScoreBatch batch{epoch,result.action_context->id,{}};
  for(std::size_t i=0;i<result.events.size();++i){const auto& event=result.events[i];if(!event.hit){continue;}
   const auto source=std::ranges::find(roster,event.actor,[](const Fighter& f){return f.definition.id;});
   const auto target=std::ranges::find(roster,event.target,[](const Fighter& f){return f.definition.id;});
   if((source==roster.end() && (!event.action_context || event.actor!=event.action_context->actor))||target==roster.end()){throw std::invalid_argument("Unknown score damage identity");}
   const auto source_team=source==roster.end()?event.action_context->actor_side:source->definition.team;
   if(source_team!=policy_.credited_team||target->definition.team==policy_.credited_team){continue;}
   const auto& h=*event.hit;
   for(auto v:{h.attempted,h.absorbed,h.health_damage,h.overkill}){if(v<0||v>numeric::quantity_max){throw std::invalid_argument("Invalid damage report quantity");}}
   if(h.absorbed+h.health_damage+h.overkill>h.attempted){throw std::invalid_argument("Inconsistent damage report");}
   batch.damage.push_back({i,event.actor,event.target,h.absorbed+h.health_damage,h.overkill});
  }
  const auto old=std::ranges::find_if(batches_,[&](const ScoreBatch& b){return b.epoch==epoch&&b.action==batch.action;});
  if(old!=batches_.end()){if(*old!=batch){throw std::invalid_argument("Conflicting score replay");}return false;}
  if(batches_.size()>=4096){throw std::length_error("Score batch safety bound reached");}
  auto effective=effective_,overkill=overkill_;
  for(const auto& hit:batch.damage){effective=bounded_add(effective,hit.effective,10000000000000000LL);overkill=bounded_add(overkill,hit.overkill,40000000000000000LL);}
  batches_.push_back(std::move(batch));effective_=effective;overkill_=overkill;return true;
 }
 [[nodiscard]] numeric::Quantity value(bool survived=false) const {
  const auto bonus=survived?policy_.survival_bonus:0;
  const auto raw=effective_*4+overkill_+(policy_.order==ScoreModifierOrder::BonusBeforeDifficulty?bonus*4:0);
  const auto scaled=numeric::mul_div_floor(raw,policy_.difficulty,40000);
  return policy_.order==ScoreModifierOrder::BonusAfterDifficulty?numeric::add(scaled,bonus):scaled;
 }
 [[nodiscard]] std::int64_t effective_total() const noexcept{return effective_;}
 [[nodiscard]] std::int64_t overkill_total() const noexcept{return overkill_;}
 [[nodiscard]] const ScorePolicy& policy()const noexcept{return policy_;}
 [[nodiscard]] std::size_t observed_batches()const noexcept{return batches_.size();}
private:
 ScorePolicy policy_;std::int64_t effective_=0,overkill_=0;std::vector<ScoreBatch> batches_;
 static std::int64_t bounded_add(std::int64_t a,std::int64_t b,std::int64_t cap){return a>cap-b?cap:a+b;}
};
struct ScoreReceipt {
 std::string attempt_id;
 numeric::Quantity score;
 bool survived;
 std::optional<std::string> reward_tier;
 bool operator==(const ScoreReceipt&)const=default;
};
struct ScoreTier {numeric::Quantity minimum;std::string id;};
class ScoreLedger {
public:
 bool settle(std::string attempt,const ScoreAccumulator& score,bool survived,const std::vector<ScoreTier>& tiers={}){
  if(attempt.empty()||attempt.size()>128){throw std::invalid_argument("Invalid attempt identity");}
  ScoreReceipt receipt{std::move(attempt),score.value(survived),survived,{}};
  numeric::Quantity previous=-1;
  for(const auto& tier:tiers){if(tier.minimum<=previous||tier.minimum>numeric::quantity_max||tier.id.empty()){throw std::invalid_argument("Invalid score tier table");}previous=tier.minimum;if(receipt.score>=tier.minimum){receipt.reward_tier=tier.id;}}
  const auto old=std::ranges::find(receipts_,receipt.attempt_id,&ScoreReceipt::attempt_id);
  if(old!=receipts_.end()){if(*old!=receipt){throw std::invalid_argument("Conflicting attempt settlement");}return false;}
  if(receipts_.size()>=4096){throw std::length_error("Score receipt safety bound reached");}
  receipts_.push_back(std::move(receipt));return true;
 }
 [[nodiscard]] const std::vector<ScoreReceipt>& receipts()const noexcept{return receipts_;}
private:std::vector<ScoreReceipt> receipts_;
};
}
