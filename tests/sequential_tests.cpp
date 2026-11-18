#include "rpg/sequential.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace {
using namespace rpg;
void check(bool value,const char* message){if(!value){throw std::runtime_error(message);}}
template<class F>void invalid(F f){bool caught=false;try{f();}catch(const std::invalid_argument&){caught=true;}check(caught,"invalid sequence accepted");}
SequentialDefinition setup(int boss_hp=20,BossRuntimeCarry carry=BossRuntimeCarry::KeepCooldownsClearStatuses){
    std::vector<SquadDefinition> squads;
    for(int i=1;i<=3;++i){BattleRules r;r.timing=TimingProfile::ActionBar;
        squads.push_back({"team-"+std::to_string(i),{{"character-"+std::to_string(i),
            {"A"+std::to_string(i),Team::A,3,2,200}}},r});}
    return {"sequence",1,squads,{"boss",Team::B,boss_hp,3,100},carry};
}
SequentialActionResult step(SequentialSquads& stage){
    const auto command=choose_basic_attack(stage.battle());check(command.has_value(),"no command");
    auto result=stage.apply(*command);check(result.action.accepted(),"valid command rejected");return result;
}
void run(SequentialSquads& stage){for(int i=0;i<256 && stage.snapshot().phase==Phase::Running;++i){step(stage);}
    check(stage.snapshot().phase==Phase::Finished,"sequence did not terminate");}
int cd(const SequentialSquads& stage,const std::string& skill){
    for(const auto& cooldown:stage.battle().find("boss")->cooldowns){if(cooldown.skill==skill){return cooldown.remaining;}}
    throw std::runtime_error("boss cooldown missing");
}
void composition(){
    for(int size=1;size<=5;++size){
        auto valid=setup();
        for(auto& squad:valid.squads){
            for(int i=1;i<size;++i){
                const auto suffix=squad.id+"-"+std::to_string(i);
                squad.members.push_back({"character-extra-"+suffix,{"instance-extra-"+suffix,Team::A,3,2,200}});
            }
        }
        SequentialSquads stage(valid);
        check(stage.battle().fighters().size()==static_cast<std::size_t>(size+1),"valid squad size rejected");
    }
    auto d=setup();d.squads.pop_back();invalid([&]{SequentialSquads stage(d);});
    d=setup();d.squads[2].id=d.squads[0].id;invalid([&]{SequentialSquads stage(d);});
    d=setup();d.squads[2].members[0].character_id=d.squads[0].members[0].character_id;invalid([&]{SequentialSquads stage(d);});
    d=setup();d.squads[2].members[0].fighter.id=d.squads[0].members[0].fighter.id;invalid([&]{SequentialSquads stage(d);});
    d=setup();d.squads[0].members.clear();invalid([&]{SequentialSquads stage(d);});
    d=setup();d.squads[0].members.resize(6,d.squads[0].members[0]);invalid([&]{SequentialSquads stage(d);});
    d=setup();d.boss.team=Team::A;invalid([&]{SequentialSquads stage(d);});
    d=setup();d.squads[2].members[0].fighter.attack=0;invalid([&]{SequentialSquads stage(d);});
    d=setup();d.global_budget=ActionBudget{0,LimitOutcome::Draw};invalid([&]{SequentialSquads stage(d);});
    d=setup();d.boss_carry=static_cast<BossRuntimeCarry>(99);invalid([&]{SequentialSquads stage(d);});
    d=setup();d.squads[2].rules.skills={{"foreign","A1",0,TargetRule::Self,{{EffectKind::Marker,0,1}}}};
    invalid([&]{SequentialSquads stage(d);});
    d=setup();d.squads[2].rules.skills={{"different","boss",0,TargetRule::Self,{{EffectKind::Marker,0,1}}}};
    invalid([&]{SequentialSquads stage(d);});
}
void wipes_in_order(){
    SequentialSquads stage(setup());int advances=0,finishes=0;
    while(stage.snapshot().phase==Phase::Running){
        const auto before=stage.snapshot();const auto result=step(stage);
        check(result.squad_id==before.squad_id && result.squad_index==before.squad_index,"response crossed squad boundary");
        advances+=result.squad_advanced?1:0;finishes+=result.stage_finished?1:0;
        if(result.squad_advanced){
            const auto now=stage.snapshot();check(now.squad_index==before.squad_index+1 && now.battle.accepted_actions==0 &&
                stage.battle().find("boss")->definition.max_health==20,"skipped squad or changed max health");
        }
    }
    check(advances==2 && finishes==1 && stage.snapshot().end_reason==SequentialEndReason::SquadsExhausted &&
          stage.snapshot().winner==Team::B && stage.battle().find("boss")->health==8 &&
          stage.snapshot().accepted_actions==9,"boss HP reset or squad wipe ended too early");
    const auto before=stage.snapshot();const auto rejected=stage.apply({"A3",ActionType::Guard,{}});
    check(!rejected.action.accepted() && !rejected.stage_finished && stage.snapshot()==before,"completion duplicated");
}
void early_victory(){
    SequentialSquads stage(setup(7));run(stage);
    check(stage.snapshot().squad_index==1 && stage.snapshot().end_reason==SequentialEndReason::BossDefeated &&
          stage.snapshot().winner==Team::A && stage.battle().find("boss")->health==0 &&
          stage.battle().find("boss")->definition.max_health==7,"early win started third squad or altered boss attributes");
}
void local_and_global_budgets(){
    auto d=setup();for(auto& squad:d.squads){squad.rules.action_budget=ActionBudget{1,LimitOutcome::Draw};}
    SequentialSquads local(d);run(local);
    check(local.snapshot().accepted_actions==3 && local.snapshot().end_reason==SequentialEndReason::SquadsExhausted &&
          local.battle().find("boss")->health==14,"local deadline did not advance squads");
    d.global_budget=ActionBudget{2,LimitOutcome::DefeatA};SequentialSquads global(d);run(global);
    check(global.snapshot().squad_index==1 && global.snapshot().end_reason==SequentialEndReason::GlobalActionLimit &&
          global.snapshot().accepted_actions==2 && global.remaining_actions()==0,"global budget restarted or started another squad");
    d=setup(2);d.global_budget=ActionBudget{1,LimitOutcome::DefeatA};SequentialSquads final(d);run(final);
    check(final.snapshot().end_reason==SequentialEndReason::BossDefeated && final.snapshot().winner==Team::A,
          "last-action boss elimination lost to deadline");
}
void boss_runtime_policy(){
    for(const auto carry:{BossRuntimeCarry::KeepCooldownsClearStatuses,BossRuntimeCarry::ResetRuntime}){
        auto d=setup(20,carry);d.boss.speed=600;
        for(auto& squad:d.squads){squad.rules.skills={{"pulse","boss",3,TargetRule::Self,{{EffectKind::Marker,0,5}}}};}
        SequentialSquads stage(d);
        check(stage.apply({"boss",ActionType::Skill,"boss","pulse"}).action.accepted(),"boss setup rejected");
        check(stage.apply({"boss",ActionType::Attack,"A1"}).squad_advanced,"boss wipe did not advance");
        const auto* boss=stage.battle().find("boss");
        check(boss->statuses.empty() && !boss->guarding && boss->effective_speed==600 && cd(stage,"pulse")==
              (carry==BossRuntimeCarry::KeepCooldownsClearStatuses?1:0),"boss cooldown policy or status cleanup wrong");
        const auto before=stage.snapshot();const auto attempt=stage.apply({"boss",ActionType::Skill,"boss","pulse"});
        if(carry==BossRuntimeCarry::KeepCooldownsClearStatuses){
            check(attempt.action.error==ActionError::SkillOnCooldown && stage.snapshot()==before,"blocked reuse changed sequence");
        }else{check(attempt.action.accepted(),"reset policy did not release cooldown");}
    }
}
void fresh_squad_state(){
    auto d=setup();for(auto& squad:d.squads){squad.rules.action_budget=ActionBudget{1,LimitOutcome::Draw};
        const auto id=squad.members[0].fighter.id;
        squad.rules.skills={{"skill-"+id,id,3,TargetRule::Self,{{EffectKind::Marker,0,5}}}};
    }
    SequentialSquads stage(d);const auto result=stage.apply({"A1",ActionType::Skill,"A1","skill-A1"});
    check(result.squad_advanced && result.squad_id=="team-1" && stage.snapshot().squad_id=="team-2", "scope not stable");
    const auto* incoming=stage.battle().find("A2");
    check(incoming->health==3 && incoming->statuses.empty() && incoming->cooldowns[0].remaining==0 &&
          stage.battle().accepted_actions()==0 && stage.battle().snapshot().next_action_id==1,"old team state leaked");
    const auto before=stage.snapshot();const auto rejected=stage.apply({"A1",ActionType::Guard,{}});
    check(rejected.action.error==ActionError::UnknownActor && stage.snapshot()==before,"retired fighter controlled next squad");
}
void r04_before_transition(){
    auto d=setup(2);d.boss.speed=600;
    for(auto& squad:d.squads){squad.rules.action_budget=ActionBudget{3,LimitOutcome::DefeatA};
        squad.rules.skills={{"recover","boss",3,TargetRule::Self,{{EffectKind::AutoRevive,2,10}}},
                           {"slow","boss",0,TargetRule::Self,{{EffectKind::SetSpeed,100}}}};}
    SequentialSquads revive(d);
    check(revive.apply({"boss",ActionType::Skill,"boss","recover"}).action.accepted(),"revive setup rejected");
    check(revive.apply({"boss",ActionType::Skill,"boss","slow"}).action.accepted(),"slow setup rejected");
    const auto result=revive.apply({"A1",ActionType::Attack,"boss"});
    check(result.squad_advanced && !result.stage_finished && revive.battle().find("boss")->health==2 &&
          cd(revive,"recover")==1,"transition ran before boss revive or restored spent cooldown");
    auto mutual=setup(5);mutual.boss.attack=5;
    for(auto& squad:mutual.squads){squad.members[0].fighter.max_health=5;
        const auto id=squad.members[0].fighter.id;
        squad.rules.skills={{"shield-"+id,id,0,TargetRule::Self,{{EffectKind::Reflect,5,10},{EffectKind::SetSpeed,100}}}};}
    SequentialSquads both(mutual);
    check(both.apply({"A1",ActionType::Skill,"A1","shield-A1"}).action.accepted(),"reflection setup rejected");
    const auto ended=both.apply({"boss",ActionType::Attack,"A1"});
    check(ended.stage_finished && !ended.squad_advanced && both.snapshot().end_reason==SequentialEndReason::MutualElimination &&
          both.snapshot().winner==Team::B,"boss HP zero overrode R04 attacker victory");
}
void technical_reset_and_replay(){
    auto d=setup();d.boss.speed=300;
    for(auto& squad:d.squads){squad.rules.max_derived_actions=1;
        squad.rules.skills={{"loop","boss",0,TargetRule::Self,{{EffectKind::ExtraTurn}}}};}
    SequentialSquads technical(d);const auto initial=technical.snapshot();
    check(technical.apply({"boss",ActionType::Skill,"boss","loop"}).action.accepted(),"loop setup rejected");
    const auto stopped=technical.apply({"boss",ActionType::Skill,"boss","loop"});
    check(stopped.stage_finished && !stopped.squad_advanced && technical.snapshot().end_reason==SequentialEndReason::TechnicalLimit &&
          !technical.snapshot().winner,"technical error treated as ordinary squad swap");
    technical.reset();check(technical.snapshot()==initial,"reset retained runtime");
    auto replay_def=setup();replay_def.global_budget=ActionBudget{30,LimitOutcome::Draw};
    SequentialSquads a(replay_def),b(replay_def);const auto start=a.snapshot();
    for(int i=0;i<128 && a.snapshot().phase==Phase::Running;++i){
        const auto command=choose_basic_attack(a.battle());check(command.has_value(),"no replay command");
        const auto first=a.apply(*command),second=b.apply(*command);
        check(first.action==second.action && first.squad_id==second.squad_id && first.squad_advanced==second.squad_advanced &&
              a.snapshot()==b.snapshot(),"sequential replay diverged");
    }
    check(a.snapshot().phase==Phase::Finished,"replay incomplete");a.reset();check(a.snapshot()==start,"completed reset failed");
    step(a);a.reset();check(a.snapshot()==start && b.snapshot().phase==Phase::Finished,"running reset leaked to other instance");
}
}
int main(int argc,char** argv){
    const std::pair<const char*,void(*)()> tests[]={{"composition",composition},{"wipe_order",wipes_in_order},
        {"early_win",early_victory},{"budgets",local_and_global_budgets},{"boss_runtime",boss_runtime_policy},
        {"fresh_squad",fresh_squad_state},{"r04_transition",r04_before_transition},{"technical_reset_replay",technical_reset_and_replay}};
    int passed=0;
    for(const auto& [name,test]:tests){try{test();++passed;std::cout<<"PASS sequential "<<name<<'\n';}
        catch(const std::exception& e){std::cerr<<"FAIL sequential "<<name<<": "<<e.what()<<'\n';}}
    std::cout<<"SEQUENTIAL_TESTS_RESULT passed="<<passed<<" total="<<std::size(tests)<<'\n';
    if(argc==2 && passed==static_cast<int>(std::size(tests))){std::ofstream out(argv[1]);out<<"test,status,scope\n";
        for(const auto& [name,test]:tests){static_cast<void>(test);out<<name<<",pass,three_sequential_squads_base\n";}if(!out){return 2;}}
    return passed==static_cast<int>(std::size(tests))?0:1;
}
