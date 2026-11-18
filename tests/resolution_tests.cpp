#include "rpg/stage.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

namespace {
using namespace rpg;
void check(bool value,const char* message){if(!value){throw std::runtime_error(message);}}
template<class F> void invalid(F f){bool caught=false;try{f();}catch(const std::invalid_argument&){caught=true;}check(caught,"invalid data accepted");}
std::vector<Definition> roster(){return {{"A",Team::A,100,5,200},{"B",Team::B,100,5,100}};}
EffectDefinition damage(std::uint32_t count){EffectDefinition d{EffectKind::Damage};d.hit_count=count;return d;}
BattleRules rules(std::string owner="B",std::uint32_t hits=3){
    BattleRules r;r.timing=TimingProfile::ActionBar;
    r.skills={{"multi",std::move(owner),0,TargetRule::Enemy,{damage(hits)}}};return r;
}
ActionResult act(Battle& b,Action action){auto r=b.apply(action);check(r.accepted(),"valid command rejected");return r;}
std::vector<Event> hits(const ActionResult& r,EventType type=EventType::Attack){
    std::vector<Event> out;for(const auto& e:r.events){if(e.type==type){out.push_back(e);}}return out;
}
void rejection(Battle& b,Action action){
    const auto before=b.snapshot();const auto r=b.apply(action);
    check(!r.accepted() && !r.action_context && r.events.empty() && b.snapshot()==before,"rejection consumed action ID/state");
}
void identity_and_atomicity(){
    Battle b(roster(),rules("A"));
    rejection(b,{"A",ActionType::Skill,"A","multi"});
    const auto first=act(b,{"A",ActionType::Guard,{}});
    check(first.action_context && first.action_context->id==1 && first.action_context->root_id==1 &&
          first.action_context->parent_id==0 && b.snapshot().next_action_id==2,"primary identity wrong");
    rejection(b,{"B",ActionType::Attack,"A"}); // A is ready again first because of its higher SPD.
}
void guarded_whole_skill(){
    auto defs=roster();defs[0].speed=100;defs[1].speed=90;
    auto r=rules();r.timing=TimingProfile::FixedRounds;
    Battle b(defs,r);act(b,{"A",ActionType::Guard,{}});
    const auto result=act(b,{"B",ActionType::Skill,"A","multi"});const auto attacks=hits(result);
    check(attacks.size()==3 && b.find("A")->health==91 && !b.find("A")->guarding,"guard did not protect all hits");
    int consumed=0;std::size_t last_damage=0,consume_index=0;
    for(std::size_t i=0;i<result.events.size();++i){
        if(result.events[i].type==EventType::Attack){last_damage=i;}
        if(result.events[i].type==EventType::GuardConsumed){++consumed;consume_index=i;}
    }
    check(consumed==1 && consume_index>last_damage,"guard consumed before skill finished or twice");
    for(std::size_t i=0;i<attacks.size();++i){
        check(attacks[i].amount==3 && attacks[i].hit_index==i+1 && attacks[i].planned_hits==3 &&
              attacks[i].action_context==result.action_context,"hit lost action identity/index");
    }
}
void split_effects_and_plain_damage(){
    auto r=rules("A");r.skills[0].effects={damage(2),damage(1)};
    Battle b(roster(),r);const auto result=act(b,{"A",ActionType::Skill,"B","multi"});
    const auto attacks=hits(result);
    check(b.find("B")->health==85 && attacks.size()==3 && attacks[2].hit_index==3 &&
          attacks[2].planned_hits==3 && b.accepted_actions()==1,"damage effects became separate turns");
}
void lethal_stops_hits_and_dead_reactions(){
    auto defs=roster();defs[1].max_health=7;auto r=rules("A");
    Battle b(defs,r,{},{{"B",100,{{EffectKind::Counter,5,TickPolicy::NaturalOnly,0}}, {}}});
    const auto result=act(b,{"A",ActionType::Skill,"B","multi"});const auto attacks=hits(result);
    check(attacks.size()==2 && attacks[0].amount==5 && attacks[1].amount==2 && b.find("B")->health==0 &&
          b.snapshot().derived_actions==0 && b.winner()==Team::A && result.events.back().type==EventType::BattleFinished,
          "dead target took extra hits/reacted or HP underflowed");
    check(b.snapshot().next_action_id==2,"cancelled reaction reserved an ID");
    rejection(b,{"A",ActionType::Guard,{}});
}
void derived_identity_and_guards(){
    auto defs=roster();defs[0].speed=100;defs[1].speed=90;auto r=rules();r.timing=TimingProfile::FixedRounds;
    Battle b(defs,r,{},{{"A",100,{{EffectKind::Counter,5,TickPolicy::NaturalOnly,0}}, {}}});
    act(b,{"A",ActionType::Guard,{}});const auto result=act(b,{"B",ActionType::Skill,"A","multi"});
    const auto reactions=hits(result,EventType::Reaction);
    check(reactions.size()==3 && b.find("A")->health==91 && b.find("B")->health==85 &&
          b.accepted_actions()==2 && b.snapshot().next_action_id==6,"reaction counted as primary or skipped");
    for(std::size_t i=0;i<reactions.size();++i){
        const auto& ctx=*reactions[i].action_context;
        check(ctx.id==i+3 && ctx.parent_id==2 && ctx.root_id==2 && ctx.actor=="A" && ctx.actor_side==Team::A &&
              reactions[i].hit_index==1 && reactions[i].planned_hits==1 && reactions[i].derived,"reaction identity incorrect");
    }
    // Guard belongs to the defender, independent of the active actor's earlier guard.
    defs=roster();defs[0].speed=100;defs[1].speed=90;r=rules("A");r.timing=TimingProfile::FixedRounds;
    Battle other(defs,r,{},{{"A",100,{{EffectKind::Counter,5,TickPolicy::NaturalOnly,0}}, {{"multi",0}}}});
    act(other,{"A",ActionType::Guard,{}});act(other,{"B",ActionType::Guard,{}});
    // A's next natural opportunity expires its own guard. Its guarded target remains B.
    act(other,{"A",ActionType::Skill,"B","multi"});
    check(other.find("B")->health==91,"offensive action consumed actor guard rather than target guard");
}
void reaction_ancestry(){
    auto r=rules("A");r.max_derived_actions=3;
    Battle b(roster(),r,{},{{"A",200,{{EffectKind::Counter,10,TickPolicy::NaturalOnly,0}},{{"multi",0}}},
                          {"B",100,{{EffectKind::Counter,10,TickPolicy::NaturalOnly,0}}, {}}});
    const auto result=act(b,{"A",ActionType::Attack,"B"});const auto reactions=hits(result,EventType::Reaction);
    check(reactions.size()==3 && b.end_reason()==EndReason::TechnicalLimit,"reaction chain unbounded");
    for(std::size_t i=0;i<reactions.size();++i){
        const auto& ctx=*reactions[i].action_context;
        check(ctx.id==i+2 && ctx.parent_id==i+1 && ctx.root_id==1,"reaction ancestry lost immediate parent");
    }
}
void bounds_and_large_damage(){
    for(const auto count:{0u,65u}){auto r=rules("A",count);invalid([&]{Battle b(roster(),r);});}
    auto r=rules("A");r.skills[0].effects.assign(5,damage(64));invalid([&]{Battle b(roster(),r);});
    r=rules("A");EffectDefinition bad{EffectKind::Marker,0,2};bad.hit_count=2;
    r.skills[0].effects={bad};invalid([&]{Battle b(roster(),r);});
    auto defs=roster();defs[0].attack=std::numeric_limits<int>::max();defs[1].max_health=std::numeric_limits<int>::max();
    Battle b(defs,rules("A",64));const auto result=act(b,{"A",ActionType::Skill,"B","multi"});
    check(hits(result).size()==1 && b.find("B")->health==0,"multi-hit multiplied int damage or overflowed");
}
void last_budget_and_replay(){
    auto r=rules("A");r.action_budget=ActionBudget{1,LimitOutcome::DefeatA};
    auto defs=roster();defs[1].max_health=12;auto reverse=defs;std::ranges::reverse(reverse);
    Battle a(defs,r),b(reverse,r);const Action command{"A",ActionType::Skill,"B","multi"};
    const auto result=act(a,command);
    check(result==act(b,command) && a.snapshot()==b.snapshot() && a.winner()==Team::A &&
          a.end_reason()==EndReason::Elimination && a.accepted_actions()==1 && hits(result).size()==3,
          "multi-hit budget cut an allowed skill or replay changed");
}
void stage_scoped_ids(){
    for(const int count:{1,2,5}){
        std::vector<EncounterDefinition> waves;
        for(int i=0;i<count;++i){waves.push_back({"wave-"+std::to_string(i),{{"B",Team::B,12,1,100}},rules("A")});}
        Stage stage({"stage",3,{{"A",Team::A,100,5,200}},waves,CarryPolicy::PreserveHealth,
                     ActionBudget{count,LimitOutcome::DefeatA},RuntimeCarry::Preserve});
        const auto initial=stage.snapshot();
        for(int i=0;i<count;++i){
            const auto result=stage.apply({"A",ActionType::Skill,"B","multi"});
            check(result.action.accepted() && result.action.action_context->id==1 &&
                  result.encounter_id=="wave-"+std::to_string(i) && hits(result.action).size()==3,
                  "action identity lost encounter scope");
        }
        check(stage.snapshot().end_reason==StageEndReason::Completed &&
              stage.snapshot().accepted_actions==static_cast<std::uint64_t>(count),"stage counted hits as commands");
        stage.reset();check(stage.snapshot()==initial,"reset did not reset action identity");
    }
}
Status consequence(EffectKind kind,int power){return {kind,10,TickPolicy::NaturalOnly,0,power};}
void reflection_inline_and_interrupt(){
    auto defs=roster();auto r=rules("A");
    Battle alive(defs,r,{},{{"B",100,{consequence(EffectKind::Reflect,2)}, {}}});
    const auto result=act(alive,{"A",ActionType::Skill,"B","multi"});
    check(alive.find("A")->health==94 && alive.find("B")->health==85,"reflection flat profile wrong");
    const auto reflected=hits(result,EventType::ReflectedDamage);
    check(reflected.size()==3,"reflection not per hit");
    for(std::size_t i=0;i<reflected.size();++i){check(reflected[i].hit_index==i+1 && reflected[i].actor=="B" &&
        reflected[i].action_context->actor=="A","emitter replaced action author");}
    std::vector<EventType> damage_order;
    for(const auto& e:result.events){if(e.type==EventType::Attack || e.type==EventType::ReflectedDamage){damage_order.push_back(e.type);}}
    check(damage_order==std::vector<EventType>{EventType::Attack,EventType::ReflectedDamage,EventType::Attack,
          EventType::ReflectedDamage,EventType::Attack,EventType::ReflectedDamage},"reflection deferred until after hits");
    defs[0].max_health=2;
    Battle dead(defs,r,{},{{"B",100,{consequence(EffectKind::Reflect,2)}, {}}});
    const auto stopped=act(dead,{"A",ActionType::Skill,"B","multi"});
    check(hits(stopped).size()==1 && dead.find("A")->health==0 && dead.winner()==Team::B &&
          dead.snapshot().next_action_id==2,"dead author continued hits or consequence allocated action ID");
}
void double_death_current_actor(){
    auto defs=roster();defs[0].max_health=5;defs[1].max_health=5;
    Battle a(defs,rules("A"),{},{{"B",100,{consequence(EffectKind::Reflect,5)}, {}}});
    const auto direct=act(a,{"A",ActionType::Skill,"B","multi"});
    check(a.winner()==Team::A && a.find("A")->health==0 && a.find("B")->health==0 &&
          direct.events.back().type==EventType::BattleFinished,"dead reflecting carrier or double-death author rule failed");
    defs=roster();defs[0].max_health=5;defs[0].attack=1;defs[1].max_health=6;
    Battle reaction(defs,rules("A"),{},{{"A",200,{consequence(EffectKind::Reflect,5)},{{"multi",0}}},
                                      {"B",100,{{EffectKind::Counter,5,TickPolicy::NaturalOnly,0}}, {}}});
    const auto result=act(reaction,{"A",ActionType::Attack,"B"});
    check(reaction.winner()==Team::B && reaction.find("A")->health==0 && reaction.find("B")->health==0 &&
          result.events.back().action_context->actor=="B" && result.action_context->actor=="A",
          "reaction author confused with original opportunity owner");
}
void death_fifo_and_order(){
    std::vector<Definition> defs={{"A1",Team::A,20,5,200},{"A2",Team::A,3,1,50},
        {"B1",Team::B,5,1,100},{"B2",Team::B,3,1,50}};
    auto r=rules("A1");
    Battle b(defs,r,{},{{"A1",200,{consequence(EffectKind::DeathExplosion,3)},{{"multi",0}}},
                       {"A2",50,{consequence(EffectKind::DeathExplosion,3)}, {}},
                       {"B1",100,{consequence(EffectKind::Reflect,20),consequence(EffectKind::DeathExplosion,3)}, {}},
                       {"B2",50,{consequence(EffectKind::DeathExplosion,1)}, {}}});
    const auto result=act(b,{"A1",ActionType::Skill,"B1","multi"});
    std::vector<std::string> fifo;std::size_t reflection=0,first_blast=result.events.size();int deaths=0;
    for(std::size_t i=0;i<result.events.size();++i){const auto& e=result.events[i];
        if(e.type==EventType::ReflectedDamage){reflection=i;}
        if(e.type==EventType::DeathEffectTriggered){fifo.push_back(e.actor);first_blast=std::min(first_blast,i);}
        if(e.type==EventType::FighterDefeated){++deaths;}
    }
    check(fifo==std::vector<std::string>{"B1","A1","A2","B2"} && reflection<first_blast && deaths==4,
          "deaths not FIFO, reflection late, or death duplicated");
    check(hits(result).size()==1 && b.winner()==Team::A && b.snapshot().derived_actions==0 &&
          b.snapshot().last_consequence_events==7,"mandatory FIFO stopped when author died or counted as counters");
}
void dynamic_selection_after_explosion(){
    std::vector<Definition> defs={{"A1",Team::A,100,5,200},{"A2",Team::A,3,1,50},
        {"B1",Team::B,5,1,100},{"B2",Team::B,6,1,50},{"B3",Team::B,100,1,50}};
    auto r=rules("A1");r.skills[0].effects[0].hit_selection=HitSelection::LowestHealthEnemy;
    Battle b(defs,r,{},{{"A2",50,{consequence(EffectKind::DeathExplosion,6)}, {}},
                       {"B1",100,{consequence(EffectKind::DeathExplosion,3)}, {}}});
    const auto result=act(b,{"A1",ActionType::Skill,"B1","multi"});const auto attacks=hits(result);
    check(attacks.size()==3 && attacks[0].target=="B1" && attacks[1].target=="B3" && attacks[2].target=="B3" &&
          b.find("B2")->health==0 && b.find("B3")->health==84,"dynamic selector retained victim killed in FIFO");
    std::size_t death_b2=0,second_hit=result.events.size();
    for(std::size_t i=0;i<result.events.size();++i){
        if(result.events[i].type==EventType::FighterDefeated && result.events[i].actor=="B2"){death_b2=i;}
        if(result.events[i].type==EventType::Attack && result.events[i].hit_index==2){second_hit=i;}
    }
    check(death_b2<second_hit && b.phase()==Phase::Running,"FIFO did not drain before dynamic next hit");
}
void explosion_interrupt_and_last_budget(){
    auto defs=roster();defs[0].max_health=3;defs[1].max_health=5;defs.push_back({"B2",Team::B,100,1,50});
    auto r=rules("A");r.action_budget=ActionBudget{1,LimitOutcome::DefeatB};
    Battle b(defs,r,{},{{"B",100,{consequence(EffectKind::DeathExplosion,3)}, {}}});
    const auto result=act(b,{"A",ActionType::Skill,"B","multi"});
    check(hits(result).size()==1 && b.winner()==Team::B && b.end_reason()==EndReason::Elimination &&
          b.find("B2")->health==100 && result.events.back().type==EventType::BattleFinished,
          "death explosion did not interrupt author or timeout overrode it");
    rejection(b,{"A",ActionType::Guard,{}});
}
void consequence_budget_and_validation(){
    auto r=rules("A");r.max_consequence_events=0;invalid([&]{Battle b(roster(),r);});
    r.max_consequence_events=4097;invalid([&]{Battle b(roster(),r);});
    r=rules("A");r.skills[0].effects={{EffectKind::Reflect,0,2}};invalid([&]{Battle b(roster(),r);});
    r=rules("A");r.skills[0].effects[0].hit_selection=static_cast<HitSelection>(99);invalid([&]{Battle b(roster(),r);});
    r=rules("A");invalid([&]{Battle b(roster(),r,{},{{"B",100,{consequence(EffectKind::DeathExplosion,-1)}, {}}});});
    r=rules("A");r.max_consequence_events=1;auto defs=roster();defs[1].max_health=5;
    Battle b(defs,r,{},{{"B",100,{consequence(EffectKind::Reflect,1),consequence(EffectKind::DeathExplosion,1)}, {}}});
    const auto result=act(b,{"A",ActionType::Skill,"B","multi"});
    check(b.end_reason()==EndReason::TechnicalLimit && !b.winner() && b.snapshot().last_consequence_events==1 &&
          hits(result,EventType::DeathExplosion).empty() && result.events.back().type==EventType::BattleFinished,
          "consequence budget unbounded or converted to game victory");
}
void configured_consequences_and_replay(){
    auto r=rules("B");r.skills={{"shield","A",0,TargetRule::Self,{{EffectKind::Reflect,2,3},{EffectKind::DeathExplosion,3,3}}}};
    auto defs=roster();defs[0].speed=100;auto reverse=defs;std::ranges::reverse(reverse);
    Battle a(defs,r),b(reverse,r);
    for(const Action& command:std::vector<Action>{{"A",ActionType::Skill,"A","shield"},{"B",ActionType::Attack,"A"},
                                                  {"A",ActionType::Attack,"B"},{"B",ActionType::Attack,"A"}}){
        check(act(a,command)==act(b,command) && a.snapshot()==b.snapshot(),"configured consequence replay diverged");
    }
    check(a.find("B")->health==91 && a.find("A")->health==90,"configured Reflect not applied or reflected recursively");
    defs=roster();defs[0].max_health=std::numeric_limits<int>::max();defs[0].attack=std::numeric_limits<int>::max();
    defs[1].max_health=std::numeric_limits<int>::max();
    Battle extreme(defs,rules("A"),{},{{"B",100,{consequence(EffectKind::Reflect,std::numeric_limits<int>::max())}, {}}});
    act(extreme,{"A",ActionType::Skill,"B","multi"});
    check(extreme.find("A")->health==0 && extreme.find("B")->health==0 && extreme.winner()==Team::A,
          "large consequence damage overflowed");
}
void stage_without_survivors(){
    for(const auto carry:{CarryPolicy::PreserveHealth,CarryPolicy::RestoreTeam}){
        BattleRules r;r.timing=TimingProfile::ActionBar;
        r.skills={{"reflect","B",0,TargetRule::Self,{{EffectKind::Reflect,5,3}}}};
        Stage stage({"stage-double",4,{{"A",Team::A,5,5,100}},
            {{"one",{{"B",Team::B,5,1,101}},r},{"two",{{"B",Team::B,5,1,101}},r}},carry,
            ActionBudget{10,LimitOutcome::Draw},RuntimeCarry::Reset});
        check(stage.apply({"B",ActionType::Skill,"B","reflect"}).action.accepted(),"stage setup rejected");
        const auto result=stage.apply({"A",ActionType::Attack,"B"});
        check(stage.battle().winner()==(carry==CarryPolicy::PreserveHealth?std::optional<Team>{Team::A}:std::nullopt),
              "double-death encounter result lost");
        if(carry==CarryPolicy::PreserveHealth){
            check(result.stage_finished && !result.encounter_advanced && stage.snapshot().end_reason==StageEndReason::NoSurvivors &&
                  !stage.snapshot().winner,"phase attempted to start with dead squad");
        }else{
            check(result.encounter_advanced && stage.battle().find("A")->health==5,"explicit restoration did not allow continuation");
            check(stage.apply({"B",ActionType::Skill,"B","reflect"}).action.accepted(),"second setup rejected");
            check(stage.apply({"A",ActionType::Attack,"B"}).stage_finished && stage.snapshot().end_reason==StageEndReason::Completed,
                  "last double-death win did not complete phase");
        }
    }
}
Status survival(EffectKind kind,int health=0,std::uint32_t charges=1){
    return {kind,20,TickPolicy::NaturalOnly,0,health,charges};
}
std::size_t event_count(const ActionResult& result,EventType type){
    return static_cast<std::size_t>(std::ranges::count(result.events,type,&Event::type));
}
void prevention_inline_and_priority(){
    auto defs=roster();defs[1].max_health=5;
    Battle b(defs,rules("A"),{},{{"B",100,{survival(EffectKind::PreventDeath,0,2),
        consequence(EffectKind::DeathExplosion,2)}, {}}});
    const auto result=act(b,{"A",ActionType::Skill,"B","multi"});const auto attacks=hits(result);
    check(attacks.size()==3 && attacks[0].amount==4 && attacks[1].amount==0 && attacks[2].amount==1 &&
          event_count(result,EventType::DeathPrevented)==2 && event_count(result,EventType::FighterDefeated)==1 &&
          event_count(result,EventType::DeathEffectTriggered)==1 && b.find("A")->health==98,"prevention confirmed death too soon");
    Battle priority(defs,rules("A"),{},{{"B",100,{survival(EffectKind::PreventDeath),survival(EffectKind::AutoRevive,3)}, {}}});
    const auto both=act(priority,{"A",ActionType::Skill,"B","multi"});
    check(event_count(both,EventType::DeathPrevented)==1 && event_count(both,EventType::ReviveScheduled)==1 &&
          event_count(both,EventType::FighterRevived)==1 && hits(both).size()==2 && priority.find("B")->health==3,
          "prevention consumed revive before real death");
}
void prevention_on_reflection(){
    auto defs=roster();defs[0].max_health=5;
    Battle b(defs,rules("A"),{},{{"A",200,{survival(EffectKind::PreventDeath,0,3)},{{"multi",0}}},
                                {"B",100,{consequence(EffectKind::Reflect,5)}, {}}});
    const auto result=act(b,{"A",ActionType::Skill,"B","multi"});
    check(hits(result).size()==3 && b.find("A")->health==1 && b.find("B")->health==85 &&
          event_count(result,EventType::DeathPrevented)==3 && event_count(result,EventType::FighterDefeated)==0 &&
          b.phase()==Phase::Running,"prevented reflection death interrupted living author");
}
void target_revives_before_victory(){
    auto defs=roster();defs[1].max_health=5;
    Battle b(defs,rules("A"),{},{{"B",100,{survival(EffectKind::AutoRevive,3),
        {EffectKind::Counter,10,TickPolicy::NaturalOnly,0}}, {}}});
    const auto result=act(b,{"A",ActionType::Skill,"B","multi"});
    check(hits(result).size()==1 && b.find("B")->health==3 && b.phase()==Phase::Running && !b.winner() &&
          event_count(result,EventType::BattleFinished)==0 && event_count(result,EventType::FighterRevived)==1 &&
          b.snapshot().derived_actions==0 && b.snapshot().next_action_id==2,"target revival happened after victory or restored old actions");
    check(std::ranges::none_of(b.find("B")->statuses,[](const Status& s){return s.kind==EffectKind::AutoRevive;}),
          "single revival charge was not consumed");
}
void author_revive_does_not_resume_hits(){
    auto defs=roster();defs[0].max_health=5;
    Battle b(defs,rules("A"),{},{{"A",200,{survival(EffectKind::AutoRevive,5)},{{"multi",0}}},
        {"B",100,{consequence(EffectKind::Reflect,5)}, {}}});
    const auto result=act(b,{"A",ActionType::Skill,"B","multi"});
    check(hits(result).size()==1 && b.find("B")->health==95 && b.find("A")->health==5 &&
          b.phase()==Phase::Running && event_count(result,EventType::FighterRevived)==1 && b.accepted_actions()==1,
          "revival resumed interrupted multi-hit or created turn");
}
void fifo_before_all_revives(){
    std::vector<Definition> defs={{"A1",Team::A,20,5,200},{"A2",Team::A,3,1,50},
        {"B1",Team::B,5,1,100},{"B2",Team::B,3,1,50}};
    Battle b(defs,rules("A1"),{},{{"A1",200,{consequence(EffectKind::DeathExplosion,3),survival(EffectKind::AutoRevive,2)},{{"multi",0}}},
        {"A2",50,{consequence(EffectKind::DeathExplosion,3)}, {}},
        {"B1",100,{consequence(EffectKind::Reflect,20),consequence(EffectKind::DeathExplosion,3),survival(EffectKind::AutoRevive,2)}, {}},
        {"B2",50,{consequence(EffectKind::DeathExplosion,1)}, {}}});
    const auto result=act(b,{"A1",ActionType::Skill,"B1","multi"});
    std::vector<std::string> triggered,revived;std::size_t last_blast=0,first_revive=result.events.size();
    for(std::size_t i=0;i<result.events.size();++i){const auto& e=result.events[i];
        if(e.type==EventType::DeathEffectTriggered){triggered.push_back(e.actor);last_blast=i;}
        if(e.type==EventType::FighterRevived){revived.push_back(e.actor);first_revive=std::min(first_revive,i);}
    }
    check(triggered==std::vector<std::string>{"B1","A1","A2","B2"} && revived==std::vector<std::string>{"B1","A1"} &&
          last_blast<first_revive && b.phase()==Phase::Running && hits(result).size()==1,
          "revive interleaved with death FIFO or prematurely ended battle");
}
void dynamic_dead_target_waits_for_revive(){
    std::vector<Definition> defs={{"A1",Team::A,100,5,200},{"A2",Team::A,3,1,50},
        {"B1",Team::B,5,1,100},{"B2",Team::B,6,1,50},{"B3",Team::B,100,1,50}};
    auto r=rules("A1");r.skills[0].effects[0].hit_selection=HitSelection::LowestHealthEnemy;
    Battle b(defs,r,{},{{"A2",50,{consequence(EffectKind::DeathExplosion,6)}, {}},
        {"B1",100,{consequence(EffectKind::DeathExplosion,3)}, {}},
        {"B2",50,{survival(EffectKind::AutoRevive,3)}, {}}});
    const auto result=act(b,{"A1",ActionType::Skill,"B1","multi"});const auto attacks=hits(result);
    check(attacks.size()==3 && attacks[1].target=="B3" && attacks[2].target=="B3" && b.find("B2")->health==3,
          "dynamic target revived during hits instead of at action boundary");
}
void revival_never_restores_cancelled_work(){
    auto defs=roster();defs[1].max_health=7;
    Battle b(defs,rules("A"),{},{{"B",100,{survival(EffectKind::AutoRevive,3),
        {EffectKind::Counter,10,TickPolicy::NaturalOnly,0}}, {}}});
    const auto result=act(b,{"A",ActionType::Skill,"B","multi"});
    check(hits(result).size()==2 && b.find("B")->health==3 && b.snapshot().derived_actions==0 &&
          b.snapshot().next_action_id==2 && event_count(result,EventType::Reaction)==0,"revival restored counter queued by previous hit");
    auto r=rules("A");r.skills[0].effects={{EffectKind::ExtraTurn},damage(3)};defs[1].max_health=5;
    Battle extras(defs,r,{},{{"B",100,{survival(EffectKind::AutoRevive,3)}, {}}});
    act(extras,{"A",ActionType::Skill,"B","multi"});
    check(extras.snapshot().pending_extras.empty() && !extras.snapshot().current_extra && extras.find("B")->health==3,
          "revival restored cancelled extra");
}
void revive_changes_result_and_timeout(){
    auto defs=roster();defs[0].max_health=5;defs[1].max_health=5;
    Battle b(defs,rules("A"),{},{{"B",100,{consequence(EffectKind::Reflect,5),survival(EffectKind::AutoRevive,3)}, {}}});
    const auto result=act(b,{"A",ActionType::Skill,"B","multi"});
    check(b.winner()==Team::B && b.find("A")->health==0 && b.find("B")->health==3 &&
          result.events.back().type==EventType::BattleFinished,"causal double-death priority ignored surviving revived side");
    defs=roster();defs[1].max_health=5;auto r=rules("A");r.action_budget=ActionBudget{1,LimitOutcome::DefeatA};
    Battle limited(defs,r,{},{{"B",100,{survival(EffectKind::AutoRevive,3)}, {}}});
    const auto ended=act(limited,{"A",ActionType::Skill,"B","multi"});
    check(limited.end_reason()==EndReason::ActionLimit && limited.winner()==Team::B &&
          limited.find("B")->health==3 && event_count(ended,EventType::FighterRevived)==1 &&
          ended.events.back().type==EventType::BattleFinished,"timeout ran before revival or granted premature elimination");
    rejection(limited,{"A",ActionType::Attack,"B"});
}
void survival_validation_and_technical_budget(){
    for(const auto kind:{EffectKind::PreventDeath,EffectKind::AutoRevive}){
        auto r=rules("A");EffectDefinition e{kind,kind==EffectKind::AutoRevive?5:0,3};
        for(const auto charges:{0u,1025u}){e.charges=charges;r.skills[0].effects={e};invalid([&]{Battle b(roster(),r);});}
        auto state=survival(kind,kind==EffectKind::AutoRevive?5:0,0);
        invalid([&]{Battle b(roster(),rules("A"),{},{{"B",100,{state}, {}}});});
    }
    auto defs=roster();defs[1].max_health=5;auto r=rules("A");r.max_consequence_events=1;
    Battle budget(defs,r,{},{{"B",100,{survival(EffectKind::AutoRevive,3)}, {}}});
    const auto result=act(budget,{"A",ActionType::Skill,"B","multi"});
    check(budget.end_reason()==EndReason::TechnicalLimit && !budget.winner() &&
          event_count(result,EventType::FighterRevived)==0,"unprocessed revive budget produced victory");
    Battle capped(defs,rules("A"),{},{{"B",100,{survival(EffectKind::AutoRevive,std::numeric_limits<int>::max())}, {}}});
    act(capped,{"A",ActionType::Skill,"B","multi"});check(capped.find("B")->health==5,"revive exceeded max HP");
    defs[0].attack=std::numeric_limits<int>::max();defs[1].max_health=1;
    Battle prevent(defs,rules("A",1),{},{{"B",100,{survival(EffectKind::PreventDeath)}, {}}});
    const auto saved=act(prevent,{"A",ActionType::Skill,"B","multi"});
    check(prevent.find("B")->health==1 && hits(saved)[0].amount==0 && prevent.phase()==Phase::Running,
          "prevention at one HP overflowed or confirmed death");
}
void survival_persists_across_stage(){
    BattleRules r;r.timing=TimingProfile::ActionBar;
    EffectDefinition gift{EffectKind::AutoRevive,5,20};gift.charges=2;
    r.skills={{"gift","A",0,TargetRule::Self,{gift}},{"reflect","B",0,TargetRule::Self,{{EffectKind::Reflect,5,10}}}};
    Stage stage({"revive-stage",5,{{"A",Team::A,5,5,200}},
        {{"one",{{"B",Team::B,5,1,100}},r},{"two",{{"B",Team::B,5,1,100}},r}},
        CarryPolicy::PreserveHealth,ActionBudget{20,LimitOutcome::Draw},RuntimeCarry::Preserve});
    const auto initial=stage.snapshot();check(stage.apply({"A",ActionType::Skill,"A","gift"}).action.accepted(),"gift rejected");
    for(int wave=0;wave<2;++wave){
        while(stage.battle().current_actor()->definition.id=="A"){
            check(stage.apply({"A",ActionType::Guard,{}}).action.accepted(),"stage guard rejected");
        }
        check(stage.apply({"B",ActionType::Skill,"B","reflect"}).action.accepted(),"stage reflect rejected");
        const auto ended=stage.apply({"A",ActionType::Attack,"B"});check(ended.action.accepted(),"stage hit rejected");
        check(stage.battle().find("A")->health==5,"phase restored through implicit heal instead of revive");
        const auto& statuses=stage.battle().find("A")->statuses;
        const auto revive=std::ranges::find(statuses,EffectKind::AutoRevive,&Status::kind);
        if(wave==0){check(ended.encounter_advanced && revive!=statuses.end() && revive->charges==1,"carry restored spent charge");}
        else{check(ended.stage_finished && stage.snapshot().end_reason==StageEndReason::Completed && revive==statuses.end(),
                   "last charge not consumed or stage ended before revive");}
    }
    stage.reset();check(stage.snapshot()==initial,"stage reset retained survival charges");
}
void survival_replay(){
    auto defs=roster();defs[0].max_health=9;defs[1].max_health=7;auto reversed=defs;std::ranges::reverse(reversed);
    const std::vector<InitialRuntime> state={{"A",200,{survival(EffectKind::PreventDeath),survival(EffectKind::AutoRevive,5)},{{"multi",0}}},
        {"B",100,{consequence(EffectKind::Reflect,3),survival(EffectKind::AutoRevive,3)}, {}}};
    Battle a(defs,rules("A"),{},state),b(reversed,rules("A"),{},state);
    bool prevented=false,revived=false,reflected=false;
    for(int i=0;i<100 && a.phase()==Phase::Running;++i){
        const auto owner=a.current_actor()->definition.id;
        const Action command=owner=="A"?Action{"A",ActionType::Skill,"B","multi"}:Action{"B",ActionType::Attack,"A"};
        const auto result=act(a,command);check(result==act(b,command) && a.snapshot()==b.snapshot(),"survival replay diverged");
        prevented=prevented || event_count(result,EventType::DeathPrevented)>0;
        revived=revived || event_count(result,EventType::FighterRevived)>0;
        reflected=reflected || event_count(result,EventType::ReflectedDamage)>0;
    }
    check(a.phase()==Phase::Finished && a.end_reason()==EndReason::Elimination && prevented && revived && reflected,
          "survival replay did not exercise contract or did not terminate");
}
Status barrier(EffectKind kind,int power){
    return {kind,kind==EffectKind::Shield?10:0,TickPolicy::NaturalOnly,0,power,1,
            kind==EffectKind::Shield?StatusCategory::Temporary:StatusCategory::Structural};
}
void shield_layers_and_guard(){
    auto defs=roster();defs[0].speed=100;defs[1].speed=90;auto r=rules();r.timing=TimingProfile::FixedRounds;
    Battle b(defs,r,{},{{"A",100,{barrier(EffectKind::Shield,4),barrier(EffectKind::StructuralShield,6)}, {}}});
    act(b,{"A",ActionType::Guard,{}});const auto result=act(b,{"B",ActionType::Skill,"A","multi"});
    const auto& statuses=b.find("A")->statuses;
    const auto structural=std::ranges::find(statuses,EffectKind::StructuralShield,&Status::kind);
    check(b.find("A")->health==100 && structural!=statuses.end() && structural->magnitude==1 &&
          std::ranges::find(statuses,EffectKind::Shield,&Status::kind)==statuses.end() &&
          event_count(result,EventType::ShieldAbsorbed)==4 && event_count(result,EventType::ShieldBroken)==1 &&
          event_count(result,EventType::GuardConsumed)==1,"guard/shield layers order wrong");
    for(const auto& e:hits(result)){check(e.amount==0,"full absorption lost HP");}
    defs=roster();defs[0].attack=std::numeric_limits<int>::max();defs[1].max_health=1;
    Battle extreme(defs,rules("A",64),{},{{"B",100,{barrier(EffectKind::Shield,std::numeric_limits<int>::max()),
          barrier(EffectKind::StructuralShield,std::numeric_limits<int>::max())}, {}}});
    const auto ended=act(extreme,{"A",ActionType::Skill,"B","multi"});
    check(hits(ended).size()==3 && event_count(ended,EventType::ShieldBroken)==2 && extreme.winner()==Team::A,
          "shield totals overflowed or bypassed layers");
}
void shields_before_survival(){
    auto defs=roster();defs[1].max_health=5;
    Battle b(defs,rules("A"),{},{{"B",100,{barrier(EffectKind::Shield,3),barrier(EffectKind::StructuralShield,2),
        survival(EffectKind::PreventDeath),survival(EffectKind::AutoRevive,3),consequence(EffectKind::Reflect,2)}, {}}});
    const auto result=act(b,{"A",ActionType::Skill,"B","multi"});const auto attacks=hits(result);
    check(attacks.size()==3 && attacks[0].amount==0 && attacks[1].amount==4 && attacks[2].amount==1 &&
          event_count(result,EventType::DeathPrevented)==1 && event_count(result,EventType::FighterRevived)==1 &&
          b.find("B")->health==3 && b.find("A")->health==94 && b.phase()==Phase::Running,
          "shields consumed survival too early or changed R04 order");
}
void categories_validation_and_expiration(){
    auto permanent=consequence(EffectKind::Reflect,2);permanent.category=StatusCategory::Permanent;permanent.remaining=0;
    Status structural{EffectKind::Counter,0,TickPolicy::NaturalOnly,0,0,1,StatusCategory::Structural};
    Battle b(roster(),rules("A"),{},{{"A",200,{permanent,structural,{EffectKind::Marker,1,TickPolicy::NaturalOnly,0}},{{"multi",0}}}});
    const auto result=act(b,{"A",ActionType::Guard,{}});
    check(b.find("A")->statuses.size()==2 && event_count(result,EventType::EffectExpired)==1,"persistent category expired with turns");
    auto r=rules("A");r.skills[0].target=TargetRule::Self;r.skills[0].effects={{EffectKind::StructuralShield,5,1}};
    invalid([&]{Battle bad(roster(),r);});
    auto bad=permanent;bad.remaining=1;invalid([&]{Battle x(roster(),rules("A"),{},{{"B",100,{bad}, {}}});});
    bad=barrier(EffectKind::Shield,2);bad.category=StatusCategory::Structural;bad.remaining=0;
    invalid([&]{Battle x(roster(),rules("A"),{},{{"B",100,{bad}, {}}});});
    bad=barrier(EffectKind::StructuralShield,0);invalid([&]{Battle x(roster(),rules("A"),{},{{"B",100,{bad}, {}}});});
}
}
int main(int argc,char** argv){
    const std::pair<const char*,void(*)()> tests[]={
        {"identity_atomics",identity_and_atomicity},{"guard_whole_skill",guarded_whole_skill},
        {"split_effects",split_effects_and_plain_damage},{"lethal_cancel",lethal_stops_hits_and_dead_reactions},
        {"derived_identity",derived_identity_and_guards},{"reaction_ancestry",reaction_ancestry},
        {"bounds_integer",bounds_and_large_damage},{"last_budget_replay",last_budget_and_replay},
        {"stage_identity_reset",stage_scoped_ids},{"reflection_inline",reflection_inline_and_interrupt},
        {"double_death_author",double_death_current_actor},{"death_fifo_order",death_fifo_and_order},
        {"dynamic_after_blast",dynamic_selection_after_explosion},{"blast_interrupt_budget",explosion_interrupt_and_last_budget},
        {"consequence_limits",consequence_budget_and_validation},{"consequence_config_replay",configured_consequences_and_replay},
        {"stage_no_survivors",stage_without_survivors},{"prevention_inline",prevention_inline_and_priority},
        {"prevention_reflection",prevention_on_reflection},{"revive_target_window",target_revives_before_victory},
        {"revive_author_no_resume",author_revive_does_not_resume_hits},{"fifo_before_revive",fifo_before_all_revives},
        {"dynamic_wait_revive",dynamic_dead_target_waits_for_revive},{"revive_cancelled_work",revival_never_restores_cancelled_work},
        {"revive_victory_timeout",revive_changes_result_and_timeout},{"survival_validation_budget",survival_validation_and_technical_budget},
        {"survival_stage_carry",survival_persists_across_stage},{"survival_replay",survival_replay},
        {"shield_layers_guard",shield_layers_and_guard},{"shield_survival",shields_before_survival},
        {"categories_expiration",categories_validation_and_expiration}};
    int passed=0;
    for(const auto& [name,test]:tests){try{test();++passed;std::cout<<"PASS resolution "<<name<<'\n';}
        catch(const std::exception& e){std::cerr<<"FAIL resolution "<<name<<": "<<e.what()<<'\n';}}
    std::cout<<"RESOLUTION_TESTS_RESULT passed="<<passed<<" total="<<std::size(tests)<<'\n';
    if(argc==2 && passed==static_cast<int>(std::size(tests))){
        std::ofstream out(argv[1]);out<<"test,status,scope\n";
        for(const auto& [name,test]:tests){static_cast<void>(test);out<<name<<",pass,incremental_action_resolution\n";}
        if(!out){return 2;}
    }
    return passed==static_cast<int>(std::size(tests))?0:1;
}
