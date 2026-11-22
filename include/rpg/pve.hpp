#pragma once
#include "rpg/score.hpp"
#include "rpg/sequential.hpp"
namespace rpg {
enum class PveMode { Ritual, ScoreAttack, Survival };
enum class PveEnd { BossDefeated, TeamDefeated, WindowClosed, SurvivalReached, TechnicalLimit };
struct PveDefinition {
 std::string id,attempt_id,boss_id;
 std::vector<Definition> roster;
 BattleRules rules;
 PveMode mode=PveMode::Ritual;
 bool score_enabled=false;
 std::optional<std::uint64_t> boss_turn_goal;
 TickPolicy boss_turn_clock=TickPolicy::NaturalOnly;
 ScorePolicy score;
 std::vector<ScoreTier> tiers;
};
struct PveActionResult {
 ActionResult action;
 bool attempt_finished=false,objective_met=false;
 std::optional<PveEnd> end;
 numeric::Quantity score=0;
};
class PveEncounter {
public:
 PveEncounter(PveDefinition definition,ScoreLedger& ledger);
 [[nodiscard]] const Battle& battle()const noexcept{return *battle_;}
 [[nodiscard]] bool finished()const noexcept{return end_.has_value();}
 [[nodiscard]] std::optional<PveEnd> end_reason()const noexcept{return end_;}
 [[nodiscard]] std::uint64_t boss_turns()const noexcept{return boss_turns_;}
 [[nodiscard]] numeric::Quantity score()const{return (definition_.score_enabled||definition_.mode==PveMode::ScoreAttack)?score_.value(alive()):0;}
 [[nodiscard]] PveActionResult apply(const Action& action);
private:
 PveDefinition definition_;ScoreLedger* ledger_;ScoreAccumulator score_;std::optional<Battle> battle_;
 std::optional<PveEnd> end_;std::uint64_t boss_turns_=0;bool objective_=false;
 [[nodiscard]] bool alive()const;
 void finish(PveEnd reason,bool objective);
};
// Score/survival result wrapper over unchanged three-squad scheduling, no cyclic return.
class ScoreSequence {
public:
 ScoreSequence(SequentialDefinition definition,std::string attempt_id,ScoreLedger& ledger,ScorePolicy policy={},std::vector<ScoreTier> tiers={});
 [[nodiscard]] const SequentialSquads& encounter()const{return encounter_;}
 [[nodiscard]] numeric::Quantity score()const{return score_.value(survived_);}
 [[nodiscard]] bool finished()const noexcept{return finished_;}
 [[nodiscard]] SequentialActionResult apply(const Action& action);
private:
 SequentialSquads encounter_;std::string attempt_;ScoreLedger* ledger_;ScoreAccumulator score_;std::vector<ScoreTier> tiers_;
 bool finished_=false,survived_=false;
};
}
