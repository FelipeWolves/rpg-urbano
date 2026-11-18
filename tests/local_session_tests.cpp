#include "rpg/local_session.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>
namespace {
using namespace rpg;
void check(bool value,const char* message){if(!value){throw std::runtime_error(message);}}
std::vector<Definition> duel(){return {{"A",Team::A,10,10,200},{"B",Team::B,10,10,100}};}
void victory_once(){
    LocalSession s(duel(),{});check(s.apply({"A",ActionType::Attack,"B"}).accepted(),"victory setup failed");
    const auto before=s.progress();check(before.credits==10 && before.completions.size()==1 && before.completions[0].attempt==1 &&
        before.completions[0].winner==Team::A && before.completions[0].reason==EndReason::Elimination &&
        before.completions[0].actions==1,"victory ledger wrong");
    check(!s.settle() && !s.settle() && !s.apply({"A",ActionType::Attack,"B"}).accepted() &&
        s.progress()==before,"duplicate settlement or post-finish command paid again");
}
void nonrewarding_outcomes(){
    auto roster=duel();roster[1].speed=300;LocalSession lost(roster,{});
    check(lost.apply({"B",ActionType::Attack,"A"}).accepted() && lost.progress().credits==0 &&
        lost.progress().completions[0].winner==Team::B,"defeat rewarded or missing result");
    for(const auto outcome:{LimitOutcome::Draw,LimitOutcome::DefeatA,LimitOutcome::DefeatB}){
        BattleRules rules;rules.action_budget=ActionBudget{1,outcome};LocalSession timeout(duel(),rules);
        check(timeout.apply({"A",ActionType::Guard,{}}).accepted() && timeout.progress().credits==0 &&
            timeout.progress().completions.size()==1 && timeout.progress().completions[0].reason==EndReason::ActionLimit,
            "timeout incorrectly rewarded");}
}
void technical_no_reward(){
    BattleRules rules;rules.max_derived_actions=1;
    rules.skills={{"extra","A",0,TargetRule::Self,{{EffectKind::ExtraTurn},{EffectKind::ExtraTurn}}}};
    LocalSession s(duel(),rules);check(s.apply({"A",ActionType::Skill,"A","extra"}).accepted() &&
        s.progress().credits==0 && s.progress().completions.size()==1 &&
        s.progress().completions[0].reason==EndReason::TechnicalLimit && !s.progress().completions[0].winner,
        "technical result created victory/reward");
}
void reset_and_abandon(){
    LocalSession s(duel(),{});check(s.apply({"A",ActionType::Attack,"B"}).accepted(),"first win failed");s.reset();
    check(s.progress().attempt==2 && s.progress().credits==10 && s.progress().completions.size()==1,"reset lost progress");
    check(s.apply({"A",ActionType::Attack,"B"}).accepted() && s.progress().credits==20 && s.progress().completions[1].attempt==2,
        "second attempt reused identity or not rewarded");
    s.reset();s.reset();check(s.progress().attempt==4 && s.progress().credits==20 && s.progress().completions.size()==2 && !s.settle(),
        "abandon created reward or reused ID");
}
void running_invalid_no_reward(){
    LocalSession s(duel(),{});const auto progress=s.progress();const auto battle=s.battle().snapshot();
    check(!s.settle() && !s.apply({"A",ActionType::Attack,"missing"}).accepted() && s.progress()==progress &&
        s.battle().snapshot()==battle,"running or rejected action changed progress");
}
void team_and_causal_victory(){
    BattleRules rules;rules.skills={{"a","A",0,TargetRule::Enemy,{{EffectKind::Damage}}},
        {"b","B",0,TargetRule::Enemy,{{EffectKind::Damage}}}};
    std::vector<CharacterLoadout> sets{{"A",{{1,"Basic","a",{}}}},{"B",{{1,"Basic","b",{}}}}};
    LocalSession team(duel(),rules,CombatProfile::Team,sets);ProfileBattle direct(duel(),rules,CombatProfile::Team,sets);
    check(team.apply_slot(1,"B")==direct.apply_slot(1,"B") && team.battle().snapshot()==direct.snapshot() &&
        team.progress().credits==10,"ledger changed profile result/events");
    rules=BattleRules{};rules.skills={{"reflect","B",0,TargetRule::Self,{{EffectKind::Reflect,10,3}}}};
    LocalSession causal(duel(),rules);check(causal.apply({"A",ActionType::Guard,{}}).accepted() &&
        causal.apply({"B",ActionType::Skill,"B","reflect"}).accepted() && causal.apply({"A",ActionType::Attack,"B"}).accepted() &&
        causal.battle().battle().find("A")->health==0 && causal.battle().battle().find("B")->health==0 &&
        causal.progress().credits==10 && causal.progress().completions[0].winner==Team::A,"ledger reinterpreted R04 causal victory");
}
void replay_isolation(){
    LocalSession a(duel(),{}),b(duel(),{}),untouched(duel(),{});const auto empty=untouched.progress();
    for(int i=0;i<4;++i){check(a.apply({"A",ActionType::Attack,"B"})==b.apply({"A",ActionType::Attack,"B"}) &&
        a.progress()==b.progress() && a.battle().snapshot()==b.battle().snapshot(),"progress replay diverged");a.reset();b.reset();}
    auto copy=a.progress();copy.credits=999;
    check(a.progress().credits==40 && untouched.progress()==empty && untouched.progress().attempt==1,"wallet/snapshot isolation failed");
}
}
int main(int argc,char** argv){
    const std::pair<const char*,void(*)()> tests[]={{"victory_once",victory_once},{"other_outcomes",nonrewarding_outcomes},
        {"technical_zero",technical_no_reward},{"reset_abandon",reset_and_abandon},{"running_invalid",running_invalid_no_reward},
        {"team_causal",team_and_causal_victory},{"replay_isolation",replay_isolation}};
    int passed=0;for(const auto& [name,test]:tests){try{test();++passed;std::cout<<"PASS local_reward "<<name<<'\n';}
        catch(const std::exception& e){std::cerr<<"FAIL local_reward "<<name<<": "<<e.what()<<'\n';}}
    std::cout<<"REWARD_TESTS_RESULT passed="<<passed<<" total="<<std::size(tests)<<'\n';
    if(argc==2 && passed==static_cast<int>(std::size(tests))){std::ofstream out(argv[1]);out<<"test,status,scope\n";
        for(const auto& [name,test]:tests){static_cast<void>(test);out<<name<<",pass,session_scoped_one_time_rewards\n";}if(!out){return 2;}}
    return passed==static_cast<int>(std::size(tests))?0:1;
}
