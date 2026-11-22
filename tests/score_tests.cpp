#include "rpg/score.hpp"
#include <iostream>
using namespace rpg;
void check(bool x,const char* m){if(!x){throw std::runtime_error(m);}}
ActionResult hit(std::uint64_t id,numeric::Quantity hp,numeric::Quantity shield,numeric::Quantity over){ActionResult r;r.action_context=ActionContext{id,0,id,"A",Team::A,ActionType::Attack,"B",{}};Event e{EventType::Attack,"A","B",hp};e.hit=resolution::HitReport{{},resolution::DamageKind::Normal,hp+shield+over,shield,hp,over};r.events.push_back(e);return r;}
int main(){try{
 Battle b({{"A",Team::A,1000,1,100},{"B",Team::B,1000,1,90}});ScoreAccumulator score;
 for(int i=1;i<=4;++i){check(score.observe(1,hit(i,1,0,1),b.fighters()),"observe hit");}check(score.value()==5,"floor once across four quarter overkills");check(!score.observe(1,hit(1,1,0,1),b.fighters())&&score.value()==5,"duplicate batch");
 bool conflict=false;try{static_cast<void>(score.observe(1,hit(1,2,0,1),b.fighters()));}catch(const std::invalid_argument&){conflict=true;}check(conflict&&score.value()==5,"conflicting batch atomic");
 ScoreAccumulator shield;check(shield.observe(1,hit(1,0,10,0),b.fighters())&&shield.value()==10,"absorption is effective damage");
 check(shield.observe(2,hit(1,1,0,0),b.fighters())&&shield.value()==11,"new encounter epoch");
 ScoreAccumulator cap;for(int i=1;i<=5;++i){check(cap.observe(1,hit(i,0,0,numeric::quantity_max),b.fighters()),"raw overkill");}check(cap.value()==numeric::quantity_max,"raw overkill not capped prematurely");
 ScoreAccumulator difficulty(ScorePolicy{Team::A,10,15000,ScoreModifierOrder::BonusBeforeDifficulty});check(difficulty.observe(1,hit(1,10,0,1),b.fighters())&&difficulty.value(false)==15&&difficulty.value(true)==30,"explicit bonus and difficulty final floor");
 ScoreLedger ledger;check(ledger.settle("attempt-1",score,false,{{0,"base"},{5,"five"}})&&!ledger.settle("attempt-1",score,false,{{0,"base"},{5,"five"}})&&ledger.receipts().size()==1&&ledger.receipts()[0].reward_tier=="five","idempotent attempt receipt and tier");
 auto invalid=hit(9,1,0,0);invalid.error=ActionError::UnknownActor;check(!score.observe(1,invalid,b.fighters())&&score.value()==5,"rejected command no score");
 BattleRules funnel;funnel.combat_model=resolution::CombatModel::FunnelV1;Definition hero{"A",Team::A,1000,1,100},foe{"B",Team::B,1000,10,200};hero.stats.block=0;foe.stats.block=0;
 InitialRuntime reflected{"A",100,{{EffectKind::Reflect,3,TickPolicy::NaturalOnly,0,25}}, {}};Battle reflection({hero,foe},funnel,{}, {reflected});auto reflectionResult=reflection.apply({"B",ActionType::Attack,"A"});ScoreAccumulator reflects;check(reflects.observe(1,reflectionResult,reflection.fighters())&&reflects.value()==25,"reflection attributed to reflecting side");
 hero.stats.invincible=true;InitialRuntime mirror{"B",200,{{EffectKind::Reflect,3,TickPolicy::NaturalOnly,0,100}}, {}};hero.speed=300;Battle immune({hero,foe},funnel,{}, {mirror});auto immuneHit=immune.apply({"A",ActionType::Attack,"B"});check(immuneHit.accepted()&&immune.find("A")->health==1000,"fixed consequences respect invincibility");
 std::cout<<"REG_SCORE_TESTS_RESULT passed=10 total=10\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
