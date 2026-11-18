#include "rpg/shared_boss.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace {
using namespace rpg;
void check(bool ok,const char* message){if(!ok){throw std::runtime_error(message);}}
template<class F>void invalid(F f){bool caught=false;try{f();}catch(const std::invalid_argument&){caught=true;}check(caught,"invalid shared profile accepted");}
SharedBossDefinition setup(){
    BattleRules rules;rules.timing=TimingProfile::ActionBar;
    SharedBossDefinition d{"shared",1,{}, {"boss",Team::B,30,3,100},rules};
    for(int i=0;i<3;++i){const auto id=std::to_string(i);d.squads.push_back({"team-"+id,{{"char-"+id,{"A"+id,Team::A,10,2,300}}}});}
    return d;
}
SharedBossActionResult step(SharedBoss& stage){const auto action=stage.choose_basic_action();check(action.has_value(),"no shared command");
    auto r=stage.apply(*action);check(r.accepted(),"valid shared command rejected");return r;}
void composition(){
    for(int size=1;size<=5;++size){auto d=setup();for(auto& squad:d.squads){for(int i=1;i<size;++i){
        const auto id=squad.id+std::to_string(i);squad.members.push_back({"char"+id,{id,Team::A,10,2,300}});}}
        SharedBoss s(d);check(s.battle().fighters().size()==static_cast<std::size_t>(size*3+1),"boss duplicated or wrong roster");}
    auto d=setup();d.squads.pop_back();invalid([&]{SharedBoss s(d);});
    d=setup();d.squads[2].members[0].character_id=d.squads[0].members[0].character_id;invalid([&]{SharedBoss s(d);});
    d=setup();d.squads[2].members[0].fighter.id="boss";invalid([&]{SharedBoss s(d);});
    d=setup();d.squads[2].id=d.squads[0].id;invalid([&]{SharedBoss s(d);});
    d=setup();d.squads[2].members.clear();invalid([&]{SharedBoss s(d);});
    d=setup();d.squads[2].members[0].fighter.speed=0;invalid([&]{SharedBoss s(d);});
    d=setup();d.rules.skills={{"later","boss",0,TargetRule::Self,{{EffectKind::Marker,0,1}}}};invalid([&]{SharedBoss s(d);});
}
void one_hp_and_cadence(){
    SharedBoss s(setup());for(int i=0;i<3;++i){check(s.battle().current_actor()->definition.id=="A"+std::to_string(i),"unexpected initial AP order");step(s);}
    check(s.battle().find("boss")->health==24 && s.battle().find("boss")->definition.max_health==30 &&
          s.battle().snapshot().accepted_actions==3,"three teams did not damage single HP");
    auto d=setup();d.boss.max_health=1000;d.boss.speed=900;SharedBoss fast(d);int attacks=0;
    while(attacks<3){const auto action=*fast.choose_basic_action();const auto r=fast.apply(action);check(r.accepted(),"fast boss rejected");
        if(action.actor=="boss"){check(action.target=="A"+std::to_string(attacks) && r.squad_id=="team-"+std::to_string(attacks),"boss did not rotate front");++attacks;}}
    for(int i=0;i<3;++i){check(fast.battle().find("A"+std::to_string(i))->health==7,"boss action tripled across teams");}
}
void target_atomicity_and_guard(){
    auto d=setup();d.boss.speed=900;SharedBoss s(d);const auto before=s.snapshot();
    check(s.apply({"boss",ActionType::Attack,"A1"}).error==SharedBossError::WrongFront && s.snapshot()==before,"cross-front attack mutated");
    check(!s.apply({"unknown",ActionType::Attack,"boss"}).accepted() && s.snapshot()==before,"unknown actor mutated");
    check(s.apply({"boss",ActionType::Guard,{}}).accepted() && s.snapshot().next_boss_front=="team-0","guard advanced attack rotation");
    const auto guarded=s.snapshot();check(!s.apply({"boss",ActionType::Skill,"boss","missing"}).accepted() && s.snapshot()==guarded,"unknown skill mutated");
}
void skip_dead_fronts_and_defeat(){
    auto d=setup();d.boss.speed=900;d.boss.max_health=1000;d.boss.attack=10;SharedBoss s(d);int attacks=0,finishes=0;
    for(int i=0;i<100 && s.battle().phase()==Phase::Running;++i){const auto command=*s.choose_basic_action();
        const auto r=s.apply(command);check(r.accepted(),"dead-front flow rejected");finishes+=r.stage_finished?1:0;
        if(command.actor=="boss"){check(command.target=="A"+std::to_string(attacks++),"dead front repeated");}}
    check(attacks==3 && finishes==1 && s.battle().winner()==Team::B && !s.snapshot().next_boss_front &&
          s.snapshot().active_squads==std::vector<bool>(3,false),"defeat ignored surviving teams or repeated conclusion");
    d=setup();d.boss.speed=900;d.boss.max_health=1000;d.boss.attack=10;d.squads[1].members[0].fighter.max_health=100;
    d.squads[2].members[0].fighter.max_health=100;SharedBoss skipped(d);int seen=0;
    for(int i=0;i<100 && seen<4;++i){const auto action=*skipped.choose_basic_action();const auto r=skipped.apply(action);check(r.accepted(),"skip flow rejected");
        if(action.actor=="boss"){const std::string expected=seen==0?"A0":seen==2?"A2":"A1";
            check(action.target==expected,"rotation did not skip wiped team");++seen;}}
    check(seen==4,"did not exercise skipped team");
}
void victory_budget_replay_reset(){
    auto d=setup();d.boss.max_health=6;d.rules.action_budget=ActionBudget{3,LimitOutcome::DefeatA};SharedBoss a(d),b(d);const auto initial=a.snapshot();
    int finishes=0;
    while(a.battle().phase()==Phase::Running){const auto command=*a.choose_basic_action();const auto x=a.apply(command),y=b.apply(command);
        check(x.action==y.action && x.squad_id==y.squad_id && a.snapshot()==b.snapshot(),"shared replay diverged");finishes+=x.stage_finished?1:0;}
    check(finishes==1 && a.battle().winner()==Team::A && a.battle().end_reason()==EndReason::Elimination &&
          a.battle().find("boss")->health==0,"final kill lost to timeout");
    const auto final=a.snapshot();check(!a.apply({"A0",ActionType::Attack,"boss"}).accepted() && a.snapshot()==final,"post-victory mutation");
    a.reset();check(a.snapshot()==initial && b.snapshot()==final,"shared reset leaked");
    d=setup();d.rules.action_budget=ActionBudget{1,LimitOutcome::Draw};SharedBoss limited(d);step(limited);
    check(limited.battle().end_reason()==EndReason::ActionLimit && !limited.battle().winner(),"global budget not applied");
}
SharedBossActionResult act(SharedBoss& s,const Action& action){auto r=s.apply(action);check(r.accepted(),"skill command rejected");return r;}
std::size_t count(const SharedBossActionResult& r,EventType type){return static_cast<std::size_t>(std::ranges::count(r.action.events,type,&Event::type));}
int cooldown(const SharedBoss& s,const std::string& id){for(const auto& cd:s.battle().find("boss")->cooldowns){if(cd.skill==id){return cd.remaining;}}
    throw std::runtime_error("missing cooldown");}
void skill_scope_validation(){
    auto d=setup();d.rules.skills={{"self","boss",0,TargetRule::Self,{{EffectKind::Marker,0,3}}}};
    invalid([&]{SharedBoss s(d);});d.skill_scopes={{"self",SharedSkillTarget::Self}};SharedBoss valid(d);
    d.skill_scopes[0].target=SharedSkillTarget::OwnSquad;invalid([&]{SharedBoss s(d);});
    d.skill_scopes={{"missing",SharedSkillTarget::Self}};invalid([&]{SharedBoss s(d);});
    d.skill_scopes={{"self",SharedSkillTarget::Self},{"self",SharedSkillTarget::Self}};invalid([&]{SharedBoss s(d);});
    d=setup();EffectDefinition damage{EffectKind::Damage};damage.hit_selection=HitSelection::LowestHealthEnemy;
    d.rules.skills={{"bounce","boss",0,TargetRule::Enemy,{damage}}};d.skill_scopes={{"bounce",SharedSkillTarget::RotatingFront}};
    invalid([&]{SharedBoss s(d);});d.rules.skills[0].effects[0].hit_selection=HitSelection::FixedTarget;SharedBoss fixed(d);
    d.skill_scopes[0].target=static_cast<SharedSkillTarget>(99);invalid([&]{SharedBoss s(d);});
}
void squad_and_rotating_skills(){
    auto d=setup();d.squads[0].members.push_back({"buddy",{"buddy",Team::A,10,2,50}});
    d.rules.skills={{"buff","A0",2,TargetRule::Ally,{{EffectKind::Marker,0,5}}}};
    d.skill_scopes={{"buff",SharedSkillTarget::OwnSquad}};SharedBoss s(d);const auto before=s.snapshot();
    check(s.apply({"A0",ActionType::Skill,"A1","buff"}).error==SharedBossError::WrongFront && s.snapshot()==before,
          "ally skill crossed squad or rejection consumed turn");
    const auto buff=act(s,{"A0",ActionType::Skill,"buddy","buff"});
    check(count(buff,EventType::EffectApplied)==1 && s.battle().find("buddy")->statuses.size()==1 &&
          s.battle().find("A1")->statuses.empty(),"buff multiplied or wrong squad");
    d=setup();d.boss.speed=900;d.boss.max_health=1000;
    d.rules.skills={{"strike","boss",0,TargetRule::Enemy,{{EffectKind::Damage}}}};
    d.skill_scopes={{"strike",SharedSkillTarget::RotatingFront}};SharedBoss boss(d);
    for(int i=0;i<3;++i){while(boss.battle().current_actor()->definition.id!="boss"){step(boss);}
        const auto snap=boss.snapshot();const auto wrong="A"+std::to_string((i+1)%3);
        check(!boss.apply({"boss",ActionType::Skill,wrong,"strike"}).accepted() && boss.snapshot()==snap,"boss skill ignored front");
        const auto hit=act(boss,{"boss",ActionType::Skill,"A"+std::to_string(i),"strike"});
        check(hit.squad_id=="team-"+std::to_string(i) && count(hit,EventType::Attack)==1,"boss skill duplicated hit or scope");}
}
void one_boss_clock_and_blocked_skip(){
    auto d=setup();d.boss.speed=900;d.boss.max_health=1000;
    d.rules.skills={{"clock","boss",3,TargetRule::Self,{{EffectKind::Marker,0,5}}}};
    d.skill_scopes={{"clock",SharedSkillTarget::Self}};SharedBoss s(d);
    const auto applied=act(s,{"boss",ActionType::Skill,"boss","clock"});
    check(cooldown(s,"clock")==2 && count(applied,EventType::EffectApplied)==1 && s.battle().find("boss")->statuses.size()==1 &&
          s.snapshot().next_boss_front=="team-0","self skill tripled or advanced front");
    const auto before=s.snapshot();check(!s.apply({"boss",ActionType::Skill,"boss","clock"}).accepted() && s.snapshot()==before,"cooldown bypass");
    while(s.battle().current_actor()->definition.id!="boss"){step(s);check(cooldown(s,"clock")==2,"ally turn ticked boss cooldown");}
    act(s,{"boss",ActionType::Guard,{}});check(cooldown(s,"clock")==1 && s.battle().find("boss")->statuses[0].remaining==4,
                                            "boss clock advanced per front");
    int ally_turns=0;
    while(s.battle().current_actor()->definition.id!="boss"){step(s);++ally_turns;
        check(cooldown(s,"clock")==1 && s.battle().find("boss")->statuses[0].remaining==4,"ally advanced boss clocks");}
    check(ally_turns>=3,"boss clock case did not cover all squads");
    d=setup();d.rules.skills={{"block","A0",0,TargetRule::Enemy,{{EffectKind::Block,0,3}}}};
    d.skill_scopes={{"block",SharedSkillTarget::Boss}};SharedBoss blocked(d);act(blocked,{"A0",ActionType::Skill,"boss","block"});
    for(int i=0;i<40 && blocked.battle().current_actor()->definition.id!="boss";++i){
        const auto* actor=blocked.battle().current_actor();act(blocked,{actor->definition.id,ActionType::Guard,{}});}
    const auto skip=blocked.choose_basic_action();check(skip && skip->actor=="boss" && skip->type==ActionType::Skip,"blocked boss did not select Skip");
    const auto next=blocked.snapshot().next_boss_front;act(blocked,*skip);check(blocked.snapshot().next_boss_front==next,"Skip advanced attack rotation");
}
void shared_survival_charges(){
    auto d=setup();d.boss.speed=301;d.boss.max_health=4;d.squads[0].members[0].fighter.attack=4;
    EffectDefinition shield{EffectKind::StructuralShield,3};shield.category=StatusCategory::Structural;
    EffectDefinition prevent{EffectKind::PreventDeath,0,10};prevent.charges=1;
    EffectDefinition multi{EffectKind::Damage};multi.hit_count=3;
    d.rules.skills={{"survival","boss",0,TargetRule::Self,{shield,prevent,{EffectKind::AutoRevive,2,10}}},
        {"multi","A0",0,TargetRule::Enemy,{multi}}};
    d.skill_scopes={{"survival",SharedSkillTarget::Self},{"multi",SharedSkillTarget::Boss}};SharedBoss s(d);
    act(s,{"boss",ActionType::Skill,"boss","survival"});
    check(s.battle().current_actor()->definition.id=="A0","survival setup did not hand off to attacker");
    const auto result=act(s,{"A0",ActionType::Skill,"boss","multi"});
    check(count(result,EventType::ShieldBroken)==1 && count(result,EventType::DeathPrevented)==1 &&
          count(result,EventType::FighterRevived)==1 && s.battle().find("boss")->health==2 &&
          s.battle().find("boss")->statuses.empty() && !result.stage_finished,"shared survival duplicated charges or declared victory too soon");
    while(s.battle().current_actor()->definition.id=="boss"){act(s,{"boss",ActionType::Guard,{}});}
    const auto last=step(s);check(last.stage_finished && s.battle().winner()==Team::A,"consumed shared revive reappeared");
}
void shared_global_death_consequences(){
    auto d=setup();d.boss.max_health=10;d.squads[0].members[0].fighter.attack=4;
    EffectDefinition multi{EffectKind::Damage};multi.hit_count=3;
    d.rules.skills={{"arm","A0",0,TargetRule::Enemy,{{EffectKind::Reflect,1,10},{EffectKind::DeathExplosion,30,10},
        {EffectKind::Counter,0,10}}},{"multi","A0",0,TargetRule::Enemy,{multi}}};
    d.skill_scopes={{"arm",SharedSkillTarget::Boss},{"multi",SharedSkillTarget::Boss}};SharedBoss s(d);
    act(s,{"A0",ActionType::Skill,"boss","arm"});act(s,{"A1",ActionType::Guard,{}});act(s,{"A2",ActionType::Guard,{}});
    const auto r=act(s,{"A0",ActionType::Skill,"boss","multi"});
    check(count(r,EventType::Attack)==3 && count(r,EventType::ReflectedDamage)==3 &&
          count(r,EventType::DeathEffectTriggered)==1 && count(r,EventType::DeathExplosion)==3 &&
          count(r,EventType::BattleFinished)==1 && s.battle().winner()==Team::A &&
          s.snapshot().active_squads==std::vector<bool>(3,false) && s.battle().snapshot().derived_actions==0,
          "shared death was duplicated, missed global targets or violated causal victory");
}
void scoped_skill_replay_reset(){
    auto d=setup();d.boss.speed=900;d.rules.action_budget=ActionBudget{12,LimitOutcome::Draw};
    d.rules.skills={{"mark","boss",3,TargetRule::Self,{{EffectKind::Marker,0,5}}},
        {"hit","boss",0,TargetRule::Enemy,{{EffectKind::Damage}}}};
    d.skill_scopes={{"mark",SharedSkillTarget::Self},{"hit",SharedSkillTarget::RotatingFront}};
    SharedBoss a(d),b(d);const auto initial=a.snapshot();int finishes=0;
    while(a.battle().phase()==Phase::Running){auto action=*a.choose_basic_action();
        if(action.actor=="boss"){action=cooldown(a,"mark")==0?Action{"boss",ActionType::Skill,"boss","mark"}:
            Action{"boss",ActionType::Skill,action.target,"hit"};}
        const auto x=act(a,action),y=act(b,action);finishes+=x.stage_finished?1:0;
        check(x.action==y.action && x.squad_id==y.squad_id && a.snapshot()==b.snapshot(),"scoped replay diverged");}
    const auto final=b.snapshot();check(finishes==1 && final.battle.accepted_actions==12,"skill global budget wrong");
    a.reset();check(a.snapshot()==initial && b.snapshot()==final,"scope/clock/status reset leaked");
}
void counter_before_timeout(){
    auto d=setup();d.rules.action_budget=ActionBudget{2,LimitOutcome::DefeatA};
    d.rules.skills={{"arm","A0",0,TargetRule::Enemy,{{EffectKind::Counter,0,10}}}};
    d.skill_scopes={{"arm",SharedSkillTarget::Boss}};SharedBoss s(d);
    act(s,{"A0",ActionType::Skill,"boss","arm"});const auto r=act(s,{"A1",ActionType::Attack,"boss"});
    check(count(r,EventType::Reaction)==1 && s.battle().find("A1")->health==7 && s.battle().find("A0")->health==10 &&
          s.battle().snapshot().derived_actions==1 && s.battle().accepted_actions()==2 &&
          s.battle().end_reason()==EndReason::ActionLimit && s.battle().winner()==Team::B,
          "counter missed author, spent primary budget or failed timeout precedence");
    // The reaction targets its attacker even though the ordinary rotation still points at team-0.
    for(const auto& e:r.action.events){if(e.type==EventType::Reaction){check(e.target=="A1","counter used ordinary rotation");}}
}
void lethal_overkill_single_finish(){
    auto d=setup();d.boss.max_health=3;d.squads[0].members[0].fighter.attack=1000;
    d.rules.action_budget=ActionBudget{1,LimitOutcome::DefeatA};EffectDefinition multi{EffectKind::Damage};multi.hit_count=3;
    d.rules.skills={{"multi","A0",0,TargetRule::Enemy,{multi}}};d.skill_scopes={{"multi",SharedSkillTarget::Boss}};
    SharedBoss s(d);const auto r=act(s,{"A0",ActionType::Skill,"boss","multi"});
    check(r.stage_finished && count(r,EventType::Attack)==1 && count(r,EventType::FighterDefeated)==1 &&
          count(r,EventType::BattleFinished)==1 && s.battle().find("boss")->health==0 &&
          s.battle().winner()==Team::A && s.battle().end_reason()==EndReason::Elimination &&
          s.snapshot().active_squads==std::vector<bool>(3,true),"overkill duplicated damage/death or lost final-action victory");
    for(const auto& e:r.action.events){if(e.type==EventType::Attack){check(e.amount==3,"overkill reported as effective HP loss");}}
    const auto final=s.snapshot();for(const auto* actor:{"A1","A2"}){
        const auto rejected=s.apply({actor,ActionType::Attack,"boss"});
        check(!rejected.accepted() && !rejected.stage_finished && rejected.action.events.empty() && s.snapshot()==final,
              "another team damaged or concluded dead boss again");}
}
void reflected_actor_death_not_global_defeat(){
    auto d=setup();d.squads[0].members[0].fighter.max_health=1;d.rules.action_budget=ActionBudget{5,LimitOutcome::Draw};
    EffectDefinition multi{EffectKind::Damage};multi.hit_count=3;
    d.rules.skills={{"reflect","A0",0,TargetRule::Enemy,{{EffectKind::Reflect,1,10}}},
        {"multi","A0",0,TargetRule::Enemy,{multi}}};
    d.skill_scopes={{"reflect",SharedSkillTarget::Boss},{"multi",SharedSkillTarget::Boss}};SharedBoss s(d);
    act(s,{"A0",ActionType::Skill,"boss","reflect"});act(s,{"A1",ActionType::Guard,{}});act(s,{"A2",ActionType::Guard,{}});
    const auto r=act(s,{"A0",ActionType::Skill,"boss","multi"});
    check(count(r,EventType::Attack)==1 && count(r,EventType::ReflectedDamage)==1 && !r.stage_finished &&
          s.battle().find("boss")->health==28 && s.snapshot().active_squads==std::vector<bool>{false,true,true},
          "reflect did not stop hits or isolated death ended global phase");
    const auto last=step(s);check(last.stage_finished && s.battle().end_reason()==EndReason::ActionLimit &&
        !s.battle().winner() && s.battle().find("boss")->health==26,"remaining team could not act before timeout");
}
void revive_on_last_global_action(){
    auto d=setup();d.boss.speed=301;d.boss.max_health=2;d.rules.action_budget=ActionBudget{2,LimitOutcome::Draw};
    EffectDefinition multi{EffectKind::Damage};multi.hit_count=3;
    d.rules.skills={{"revive","boss",0,TargetRule::Self,{{EffectKind::AutoRevive,2,10}}},
        {"multi","A0",0,TargetRule::Enemy,{multi}}};
    d.skill_scopes={{"revive",SharedSkillTarget::Self},{"multi",SharedSkillTarget::Boss}};SharedBoss s(d);
    act(s,{"boss",ActionType::Skill,"boss","revive"});const auto r=act(s,{"A0",ActionType::Skill,"boss","multi"});
    check(count(r,EventType::Attack)==1 && count(r,EventType::FighterRevived)==1 && count(r,EventType::BattleFinished)==1 &&
          s.battle().find("boss")->health==2 && !s.battle().winner() && s.battle().end_reason()==EndReason::ActionLimit,
          "revive lost to victory/timeout or resumed cancelled hits");
}
void technical_failure_priority_replay(){
    for(const bool consequence:{false,true}){
        auto d=setup();d.rules.action_budget=ActionBudget{consequence?4:1,LimitOutcome::DefeatA};
        std::vector<Action> commands;
        if(consequence){d.boss.max_health=2;d.rules.max_consequence_events=1;
            d.rules.skills={{"blast","A0",0,TargetRule::Enemy,{{EffectKind::DeathExplosion,30,10}}}};
            d.skill_scopes={{"blast",SharedSkillTarget::Boss}};
            commands={{"A0",ActionType::Skill,"boss","blast"},{"A1",ActionType::Guard,{}},
                {"A2",ActionType::Guard,{}},{"A0",ActionType::Attack,"boss"}};
        }else{d.boss.speed=900;d.rules.max_derived_actions=1;
            d.rules.skills={{"loop","boss",0,TargetRule::Self,{{EffectKind::ExtraTurn},{EffectKind::ExtraTurn}}}};
            d.skill_scopes={{"loop",SharedSkillTarget::Self}};
            commands={{"boss",ActionType::Skill,"boss","loop"}};
        }
        SharedBoss a(d),b(d);const auto initial=a.snapshot();int finishes=0;
        for(const auto& cmd:commands){const auto x=act(a,cmd),y=act(b,cmd);finishes+=x.stage_finished?1:0;
            check(x.action==y.action && a.snapshot()==b.snapshot(),"technical shared replay diverged");}
        check(finishes==1 && a.battle().end_reason()==EndReason::TechnicalLimit && !a.battle().winner() &&
              a.battle().accepted_actions()==commands.size(),"technical failure lost to timeout/elimination or repeated finish");
        const auto final=b.snapshot();check(!a.apply(commands.back()).accepted() && a.snapshot()==final,"post-technical mutation");
        a.reset();check(a.snapshot()==initial && b.snapshot()==final,"technical reset leaked state");
    }
}
void extra_turn_timeout_no_followup(){
    auto d=setup();d.rules.action_budget=ActionBudget{1,LimitOutcome::Draw};
    d.rules.skills={{"extra","A0",3,TargetRule::Self,{{EffectKind::ExtraTurn},{EffectKind::Marker,0,5}}}};
    d.skill_scopes={{"extra",SharedSkillTarget::Self}};SharedBoss s(d);const auto initial=s.snapshot();
    const auto r=act(s,{"A0",ActionType::Skill,"A0","extra"});
    check(r.stage_finished && count(r,EventType::BattleFinished)==1 && s.battle().accepted_actions()==1 &&
          s.battle().end_reason()==EndReason::ActionLimit && s.snapshot().next_boss_front==std::nullopt,
          "extra bypassed global deadline");
    const auto final=s.snapshot();check(!s.apply({"A0",ActionType::Attack,"boss"}).accepted() && s.snapshot()==final,
                                      "extra executed after terminal action");
    s.reset();check(s.snapshot()==initial,"extra/status/cooldown retained after reset");
    // A future scheduling cap is not a failure of the chain that already completed.
    d=setup();d.boss.speed=900;d.rules.max_derived_actions=1;d.rules.action_budget=ActionBudget{2,LimitOutcome::DefeatA};
    d.rules.skills={{"loop","boss",0,TargetRule::Self,{{EffectKind::ExtraTurn}}}};
    d.skill_scopes={{"loop",SharedSkillTarget::Self}};SharedBoss scheduled(d);
    act(scheduled,{"boss",ActionType::Skill,"boss","loop"});const auto last=act(scheduled,{"boss",ActionType::Skill,"boss","loop"});
    check(last.stage_finished && scheduled.battle().end_reason()==EndReason::ActionLimit && scheduled.battle().winner()==Team::B,
          "future extra scheduling cap replaced completed-action timeout");
}
}
int main(int argc,char** argv){
    const std::pair<const char*,void(*)()> tests[]={{"composition",composition},{"one_hp_cadence",one_hp_and_cadence},
        {"target_atomics_guard",target_atomicity_and_guard},{"dead_fronts_defeat",skip_dead_fronts_and_defeat},
        {"victory_budget_replay_reset",victory_budget_replay_reset},{"scope_validation",skill_scope_validation},
        {"squad_rotating_skills",squad_and_rotating_skills},{"boss_clock_skip",one_boss_clock_and_blocked_skip},
        {"shared_survival",shared_survival_charges},{"global_death_consequences",shared_global_death_consequences},
        {"scoped_replay_reset",scoped_skill_replay_reset},{"counter_timeout",counter_before_timeout},
        {"lethal_overkill",lethal_overkill_single_finish},{"reflected_actor_death",reflected_actor_death_not_global_defeat},
        {"revive_last_action",revive_on_last_global_action},{"technical_priority_replay",technical_failure_priority_replay},
        {"extra_timeout",extra_turn_timeout_no_followup}};
    int passed=0;for(const auto& [name,test]:tests){try{test();++passed;std::cout<<"PASS shared_boss "<<name<<'\n';}
        catch(const std::exception& e){std::cerr<<"FAIL shared_boss "<<name<<": "<<e.what()<<'\n';}}
    std::cout<<"SHARED_BOSS_TESTS_RESULT passed="<<passed<<" total="<<std::size(tests)<<'\n';
    if(argc==2 && passed==static_cast<int>(std::size(tests))){std::ofstream out(argv[1]);out<<"test,status,scope\n";
        for(const auto& [name,test]:tests){static_cast<void>(test);out<<name<<",pass,single_hp_scoped_skills_shared_boss\n";}if(!out){return 2;}}
    return passed==static_cast<int>(std::size(tests))?0:1;
}
