#include "rpg/stage.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <fstream>

namespace {
using namespace rpg;
void check(bool value, const char* message) { if (!value) { throw std::runtime_error(message); } }
template<class F> void invalid(F f) {
    bool caught = false; try { f(); } catch (const std::invalid_argument&) { caught = true; }
    check(caught, "invalid configuration accepted");
}
std::vector<Definition> duel(int a_speed = 200, int b_speed = 100) {
    return {{"A", Team::A, 10000, 10, a_speed}, {"B", Team::B, 10000, 10, b_speed}};
}
BattleRules bar() { BattleRules r; r.timing = TimingProfile::ActionBar; return r; }
SkillDefinition skill(std::string id, std::string owner, TargetRule target,
                      std::vector<EffectDefinition> effects, int cooldown = 0,
                      TickPolicy clock = TickPolicy::NaturalOnly) {
    return {std::move(id), std::move(owner), cooldown, target, std::move(effects), clock};
}
ActionResult act(Battle& battle, Action action) {
    auto result = battle.apply(action); check(result.accepted(), "valid command rejected"); return result;
}
ActionResult guard(Battle& battle) { return act(battle, {battle.current_actor()->definition.id, ActionType::Guard, {}}); }
void reject(Battle& battle, Action action, ActionError error) {
    const auto before = battle.snapshot(); const auto result = battle.apply(action);
    check(result.error == error && result.events.empty() && battle.snapshot() == before, "invalid command mutated state");
}
int cd(const Battle& battle, const std::string& owner, const std::string& id) {
    for (const auto& cooldown : battle.find(owner)->cooldowns) { if (cooldown.skill == id) { return cooldown.remaining; } }
    throw std::runtime_error("cooldown missing");
}
void configuration() {
    auto r = bar(); r.timing = static_cast<TimingProfile>(99); invalid([&] { Battle b(duel(), r); });
    r = bar(); r.max_derived_actions = 0; invalid([&] { Battle b(duel(), r); });
    invalid([] { Battle b(duel(10001), bar()); });
    r = bar(); r.skills = {skill("s", "missing", TargetRule::Self, {{EffectKind::ExtraTurn}})};
    invalid([&] { Battle b(duel(), r); });
    r.skills = {skill("s", "A", TargetRule::Self, {{EffectKind::SetSpeed, 0}})};
    invalid([&] { Battle b(duel(), r); });
    r.skills = {skill("s", "A", TargetRule::Self, {{EffectKind::Marker, 0, 0}})};
    invalid([&] { Battle b(duel(), r); });
    r.skills = {skill("s", "A", TargetRule::Self, {{EffectKind::Damage}})};
    invalid([&] { Battle b(duel(), r); });
    invalid([&] { Battle b(duel(), bar(), {}, {{"unknown", 100, {}, {}}}); });
    invalid([&] { Battle b(duel(), bar(), {}, {{"A", 0, {}, {}}}); });
    invalid([&] { Battle b(duel(), bar(), {}, {{"A", 100, {{EffectKind::Damage, 1, TickPolicy::NaturalOnly, 0}}, {}}}); });
}
void frequency_and_replay() {
    auto roster = duel(); auto reversed = roster; std::ranges::reverse(reversed);
    Battle a(roster, bar()), b(reversed, bar());
    int fast = 0; int slow = 0;
    for (int i = 0; i < 30; ++i) {
        if (a.current_actor()->definition.id == "A") { ++fast; } else { ++slow; }
        check(guard(a) == guard(b) && a.snapshot() == b.snapshot(), "AP replay diverged");
    }
    check(fast == 20 && slow == 10 && a.snapshot().natural_turns == 30 && a.snapshot().extra_turns == 0,
          "AP acts like one turn per round");
    Battle tie(duel(100,100), bar());
    check(tie.current_actor()->definition.id == "A", "ordinal tie wrong"); guard(tie);
    check(tie.current_actor()->definition.id == "B", "ready fighter skipped");
}
void ap_and_speed() {
    auto r = bar();
    r.skills = {skill("boost", "A", TargetRule::Enemy, {{EffectKind::ChangeAP, 1000000}})};
    Battle push(duel(), r); const auto time = push.snapshot().logical_time;
    act(push, {"A", ActionType::Skill, "B", "boost"});
    check(push.current_actor()->definition.id == "B" && push.snapshot().logical_time == time, "AP boost did not reorder");
    r.skills = {skill("drain", "A", TargetRule::Enemy, {{EffectKind::ChangeAP, -1000000}})};
    Battle drain(duel(), r); act(drain, {"A", ActionType::Skill, "B", "drain"});
    check(drain.current_actor()->definition.id == "A" && drain.find("B")->ap == 500000, "AP drain wrong");
    r.skills = {skill("speed", "A", TargetRule::Enemy, {{EffectKind::SetSpeed, 1000}})};
    Battle speed(duel(), r); act(speed, {"A", ActionType::Skill, "B", "speed"});
    check(speed.current_actor()->definition.id == "B" && speed.find("B")->definition.speed == 100 &&
          speed.find("B")->effective_speed == 1000, "dynamic speed changed base or ignored");
}
void extras_and_lifetimes() {
    auto r = bar();
    r.skills = {skill("extra", "A", TargetRule::Self,
        {{EffectKind::Marker, 0, 2, TickPolicy::EveryTurn}, {EffectKind::ExtraTurn}}, 2)};
    Battle b(duel(), r); const auto before = b.snapshot();
    act(b, {"A", ActionType::Skill, "A", "extra"});
    check(b.snapshot().turn_kind == TurnKind::Extra && b.snapshot().logical_time == before.logical_time &&
          b.find("B")->ap == before.fighters[1].ap && b.find("A")->statuses[0].remaining == 2 && cd(b,"A","extra") == 1,
          "new effect ticked or extra advanced AP");
    reject(b, {"A", ActionType::Skill, "A", "extra"}, ActionError::SkillOnCooldown);
    guard(b);
    check(b.find("A")->statuses[0].remaining == 1 && cd(b,"A","extra") == 1 &&
          b.snapshot().natural_turns == 1 && b.snapshot().extra_turns == 1, "extra clocks conflated");
    guard(b);
    check(b.find("A")->statuses.empty() && cd(b,"A","extra") == 0, "owner clock not advanced");
    guard(b); check(cd(b,"A","extra") == 0, "enemy changed ready cooldown");
    guard(b); check(cd(b,"A","extra") == 0, "ready cooldown went below zero");
    r.skills[0].cooldown_clock = TickPolicy::EveryTurn;
    Battle every(duel(), r); act(every, {"A", ActionType::Skill, "A", "extra"}); guard(every);
    check(cd(every,"A","extra") == 0, "every-turn cooldown ignored extra");
}
void blocking_and_skip() {
    auto r = bar(); r.action_budget = ActionBudget{4, LimitOutcome::Draw};
    r.skills = {skill("block", "A", TargetRule::Enemy,
        {{EffectKind::ChangeAP, 1000000}, {EffectKind::Block, 0, 1}})};
    Battle b(duel(), r);
    reject(b, {"A", ActionType::Skip, {}}, ActionError::NotBlocked);
    act(b, {"A", ActionType::Skill, "B", "block"});
    check(b.current_actor_blocked(), "target not blocked");
    reject(b, {"B", ActionType::Attack, "A"}, ActionError::ActorBlocked);
    const auto automatic = choose_basic_attack(b);
    check(automatic && automatic->type == ActionType::Skip, "policy did not resolve blocked opportunity");
    act(b, *automatic);
    check(b.find("B")->statuses.empty() && b.accepted_actions() == 2 && b.snapshot().natural_turns == 2 &&
          b.find("A")->health == 10000, "blocked opportunity not counted/expired correctly");
}
void guard_toggle_and_extra() {
    auto r = bar(); r.guard_enabled = false;
    Battle disabled(duel(), r); reject(disabled, {"A", ActionType::Guard, {}}, ActionError::GuardDisabled);
    auto roster = duel(100,200);
    r = bar(); r.skills = {skill("grant", "B", TargetRule::Enemy, {{EffectKind::ExtraTurn}})};
    Battle b(roster, r);
    act(b, {"B", ActionType::Skill, "A", "grant"});
    guard(b); // A guards in an extra turn, then B's natural turn is next.
    check(b.find("A")->guarding && b.current_actor()->definition.id == "B", "guard lost too early");
    auto result = act(b, {"B", ActionType::Attack, "A"});
    check(b.find("A")->health == 9995 && std::ranges::any_of(result.events, [](const Event& e) {
        return e.type == EventType::GuardConsumed;
    }), "guard did not halve damage");
    // A guard expires at next own opportunity even when that opportunity is extra.
    r = bar(); r.skills = {skill("grant", "B", TargetRule::Enemy, {{EffectKind::ExtraTurn}})};
    Battle expire(duel(100,100), r); guard(expire);
    act(expire, {"B", ActionType::Skill, "A", "grant"});
    check(!expire.find("A")->guarding && expire.snapshot().turn_kind == TurnKind::Extra, "guard leaked into extra");
}
void reaction_is_not_turn() {
    auto r = bar();
    r.skills = {skill("counter", "A", TargetRule::Self, {{EffectKind::Counter, 0, 3}}, 2),
                skill("push", "A", TargetRule::Enemy, {{EffectKind::ChangeAP, 1000000}})};
    Battle b(duel(100,100), r); act(b, {"A", ActionType::Skill, "A", "counter"});
    const auto before = b.snapshot();
    auto result = act(b, {"B", ActionType::Attack, "A"});
    check(b.snapshot().derived_actions == 1 && b.accepted_actions() == 2 && b.find("A")->natural_turns == 1 &&
          cd(b,"A","counter") == 1 && b.find("A")->statuses[0].remaining == 3, "reaction consumed owner clocks");
    const auto reaction = std::ranges::find(result.events, EventType::Reaction, &Event::type);
    check(reaction != result.events.end() && reaction->actor == "A" && reaction->derived &&
          reaction->source_actor == "B" && reaction->turn_id == before.turn_id, "reaction origin lost");
}
void reaction_lethal_and_budget() {
    auto r = bar(); r.action_budget = ActionBudget{2, LimitOutcome::DefeatA};
    r.skills = {skill("counter", "A", TargetRule::Self, {{EffectKind::Counter, 0, 3}})};
    auto roster = duel(100,100); roster[1].max_health = 10;
    Battle b(roster,r); act(b, {"A", ActionType::Skill, "A", "counter"});
    const auto result = act(b, {"B", ActionType::Attack, "A"});
    check(b.winner() == Team::A && b.end_reason() == EndReason::Elimination && b.accepted_actions() == 2 &&
          result.events.back().type == EventType::BattleFinished, "last-action reaction not fully resolved");
    reject(b, {"A", ActionType::Attack, "B"}, ActionError::BattleFinished);
}
void technical_limits() {
    auto r = bar(); r.max_derived_actions = 2;
    r.skills = {skill("chain", "A", TargetRule::Self, {{EffectKind::ExtraTurn}})};
    Battle extra(duel(),r);
    for (int i=0;i<3;++i) { act(extra, {"A", ActionType::Skill, "A", "chain"}); }
    check(extra.end_reason() == EndReason::TechnicalLimit && !extra.winner() && extra.accepted_actions() == 3,
          "extra chain unbounded or treated as defeat");
    r.skills = {skill("counter-a", "A", TargetRule::Self, {{EffectKind::Counter,0,10}}),
                skill("counter-b", "B", TargetRule::Self, {{EffectKind::Counter,0,10}})};
    Battle counters(duel(100,100),r);
    act(counters, {"A", ActionType::Skill, "A", "counter-a"});
    act(counters, {"B", ActionType::Skill, "B", "counter-b"});
    act(counters, {"A", ActionType::Attack, "B"});
    check(counters.end_reason() == EndReason::TechnicalLimit && !counters.winner() &&
          counters.snapshot().derived_actions == 2 && counters.accepted_actions() == 3, "reaction cap failed");
}
void skill_validation_and_atomics() {
    auto r=bar(); r.skills={skill("s","A",TargetRule::Enemy,{{EffectKind::Damage}},2)};
    Battle b(duel(),r);
    reject(b,{"A",ActionType::Skill,"B","missing"},ActionError::UnknownSkill);
    reject(b,{"A",ActionType::Skill,"A","s"},ActionError::InvalidSkillTarget);
    reject(b,{"A",ActionType::Attack,"B","s"},ActionError::UnknownSkill);
    act(b,{"A",ActionType::Skill,"B","s"});
    check(b.find("B")->health == 9990 && cd(b,"A","s") == 1,"skill damage or initial cooldown wrong");
    reject(b,{"A",ActionType::Skill,"B","s"},ActionError::SkillOnCooldown);
}
StageDefinition waves(int count, RuntimeCarry policy) {
    auto r=bar(); r.skills={skill("state","A",TargetRule::Self,{{EffectKind::Marker,0,5},{EffectKind::SetSpeed,250}},3)};
    std::vector<EncounterDefinition> encounters;
    for(int i=0;i<count;++i) { encounters.push_back({"wave-"+std::to_string(i),{{"B",Team::B,10,1,100}},r}); }
    return {"stage-ap",2,{{"A",Team::A,100,10,200}},encounters,CarryPolicy::PreserveHealth,
            ActionBudget{30,LimitOutcome::Draw},policy};
}
void stage_runtime() {
    for(const auto policy : {RuntimeCarry::Preserve,RuntimeCarry::Reset}) {
        Stage stage(waves(2,policy));
        check(stage.apply({"A",ActionType::Skill,"A","state"}).action.accepted(),"stage skill rejected");
        check(stage.apply({"A",ActionType::Attack,"B"}).encounter_advanced,"stage did not advance");
        const auto* hero=stage.battle().find("A");
        check(hero->definition.max_health == 100 && !hero->guarding && hero->natural_turns == 0,"stage runtime clocks leaked");
        check(cd(stage.battle(),"A","state") == (policy==RuntimeCarry::Preserve ? 1 : 0),"stage reset cooldown implicitly");
        check(hero->statuses.size() == (policy==RuntimeCarry::Preserve ? 1u : 0u) &&
              hero->effective_speed == (policy==RuntimeCarry::Preserve ? 250 : 200),"stage lost preserved runtime");
        const auto initial=Stage(waves(2,policy)).snapshot(); stage.reset();
        check(stage.snapshot()==initial,"extended stage reset failed");
    }
    auto d=waves(2,RuntimeCarry::Preserve); d.runtime_carry.reset(); invalid([&]{Stage stage(d);});
    d=waves(2,RuntimeCarry::Preserve); d.encounters[1].rules.skills.clear(); invalid([&]{Stage stage(d);});
}
void stage_budgets_and_replay() {
    for(const int count : {1,2,5}) {
        auto d=waves(count,RuntimeCarry::Preserve);
        Stage first(d),second(d); int completed=0;
        for(int i=0;i<100 && first.snapshot().phase==Phase::Running;++i) {
            const auto action=choose_basic_attack(first.battle()); check(action.has_value(),"no stage action");
            const auto a=first.apply(*action),b=second.apply(*action);
            completed+=a.stage_finished?1:0;
            check(a.action==b.action && first.snapshot()==second.snapshot(),"AP stage replay diverged");
        }
        check(first.snapshot().end_reason==StageEndReason::Completed && completed==1 &&
              first.snapshot().accepted_actions==static_cast<std::uint64_t>(count),"AP stage completion wrong");
    }
    auto d=waves(5,RuntimeCarry::Preserve); d.global_budget=ActionBudget{2,LimitOutcome::DefeatA};
    Stage limited(d); auto a=limited.apply({"A",ActionType::Attack,"B"});
    check(a.encounter_advanced,"wave one not won");
    a=limited.apply({"A",ActionType::Attack,"B"});
    check(a.stage_finished && !a.encounter_advanced && limited.snapshot().end_reason==StageEndReason::GlobalActionLimit &&
          limited.snapshot().accepted_actions==2,"AP stage reset global budget");
}
void formation_matrix() {
    for(int allies=1;allies<=5;++allies) { for(int enemies=1;enemies<=5;++enemies) {
        std::vector<Definition> roster;
        for(int i=0;i<allies;++i) {roster.push_back({"A"+std::to_string(i),Team::A,30,10,150+i*10});}
        for(int i=0;i<enemies;++i) {roster.push_back({"B"+std::to_string(i),Team::B,30,10,100+i*10});}
        auto reversed=roster; std::ranges::reverse(reversed);
        auto r=bar();r.action_budget=ActionBudget{200,LimitOutcome::Draw};
        Battle first(roster,r),second(reversed,r);
        while(first.phase()==Phase::Running) {
            const auto action=choose_basic_attack(first); check(action.has_value(),"no AP action");
            check(act(first,*action)==act(second,*action) && first.snapshot()==second.snapshot(),"AP formation replay mismatch");
        }
        check(first.end_reason()==EndReason::Elimination && first.accepted_actions()<200,"AP formation did not finish");
        for(const auto& f:first.fighters()) {check(f.health>=0 && f.health<=f.definition.max_health,"HP invariant failed");}
    }}
}
void fixed_profile_extras() {
    BattleRules r;
    r.skills={skill("extra","A",TargetRule::Self,{{EffectKind::ExtraTurn}})};
    Battle b(duel(),r);
    act(b,{"A",ActionType::Skill,"A","extra"});
    check(b.current_actor()->definition.id=="A" && b.snapshot().turn_kind==TurnKind::Extra,"fixed extra not selected");
    guard(b);
    check(b.current_actor()->definition.id=="B" && b.round()==1,"extra skipped fixed natural cursor");
    guard(b); check(b.current_actor()->definition.id=="A" && b.round()==2,"fixed round not resumed");
}
void pending_death_and_blocked_reactions() {
    auto r=bar();r.skills={skill("cancel","A",TargetRule::Enemy,{{EffectKind::ExtraTurn},{EffectKind::Damage}})};
    auto roster=duel();roster[1].max_health=10;roster.push_back({"B2",Team::B,10000,1,50});
    Battle b(roster,r);act(b,{"A",ActionType::Skill,"B","cancel"});
    check(b.find("B")->health==0 && b.snapshot().pending_extras.empty() && b.snapshot().turn_kind==TurnKind::Natural,
          "dead fighter kept queued extra");
    r=bar();r.skills={skill("stop","A",TargetRule::Enemy,{{EffectKind::Block,0,1},{EffectKind::Damage}})};
    Battle blocked(duel(),r,{},{{"B",100,{{EffectKind::Counter,3,TickPolicy::NaturalOnly,0}}, {}}});
    act(blocked,{"A",ActionType::Skill,"B","stop"});
    check(blocked.snapshot().derived_actions==0 && blocked.find("A")->health==10000,"blocked target counterattacked");
}
void deadline_and_numeric_edges() {
    auto r=bar();r.action_budget=ActionBudget{1,LimitOutcome::Draw};
    r.skills={skill("extra","A",TargetRule::Self,{{EffectKind::ExtraTurn}})};
    Battle b(duel(10000,1),r);const auto time=b.snapshot().logical_time;
    const auto result=act(b,{"A",ActionType::Skill,"A","extra"});
    check(b.end_reason()==EndReason::ActionLimit && b.snapshot().extra_turns==0 &&
          b.snapshot().logical_time==time && result.events.back().type==EventType::BattleFinished,
          "deadline started queued extra");
    auto d=waves(1,RuntimeCarry::Preserve);d.global_budget=ActionBudget{1,LimitOutcome::DefeatA};
    Stage final(d);const auto ended=final.apply({"A",ActionType::Attack,"B"});
    check(ended.stage_finished && final.snapshot().end_reason==StageEndReason::Completed,
          "AP final elimination lost to global budget");
    d=waves(1,RuntimeCarry::Preserve);d.encounters[0].rules.max_derived_actions=1;
    d.encounters[0].rules.skills={skill("loop","A",TargetRule::Self,{{EffectKind::ExtraTurn}})};
    Stage technical(d);
    check(technical.apply({"A",ActionType::Skill,"A","loop"}).action.accepted(),"initial extra rejected");
    const auto stopped=technical.apply({"A",ActionType::Skill,"A","loop"});
    check(stopped.stage_finished && technical.snapshot().end_reason==StageEndReason::TechnicalLimit &&
          !technical.snapshot().winner,"stage converted technical failure into game defeat");
}
void cooldown_cycle(int maximum) {
    for (const auto profile : {TimingProfile::FixedRounds, TimingProfile::ActionBar}) {
        auto r = bar(); r.timing = profile;
        r.skills = {skill("pulse", "A", TargetRule::Enemy, {{EffectKind::Damage}}, maximum)};
        Battle b(duel(100,100), r);
        const auto next_owner = [&b] {
            if (b.current_actor()->definition.id == "B") { act(b, {"B",ActionType::Attack,"A"}); }
            check(b.current_actor()->definition.id == "A", "owner opportunity missing");
        };
        reject(b, {"A", ActionType::Skill, "A", "pulse"}, ActionError::InvalidSkillTarget);
        check(cd(b,"A","pulse") == 0, "invalid use set cooldown");
        act(b, {"A", ActionType::Skill, "B", "pulse"});
        check(cd(b,"A","pulse") == std::max(0,maximum-1), "use did not decrement at end");
        for (int blocked = maximum-1; blocked > 0; --blocked) {
            next_owner();
            check(cd(b,"A","pulse") == blocked, "enemy turn consumed cooldown");
            reject(b, {"A", ActionType::Skill, "B", "pulse"}, ActionError::SkillOnCooldown);
            guard(b);
            check(cd(b,"A","pulse") == blocked-1, "owner completion did not decrement");
        }
        next_owner();
        check(cd(b,"A","pulse") == 0, "skill unavailable at contracted opportunity");
        act(b, {"A", ActionType::Skill, "B", "pulse"});
        check(b.find("B")->health == 9980 && cd(b,"A","pulse") == std::max(0,maximum-1),
              "reuse failed or zero underflowed");
    }
}
void cooldown_zero() { cooldown_cycle(0); }
void cooldown_one() { cooldown_cycle(1); }
void cooldown_three() { cooldown_cycle(3); }
void cooldown_replay() {
    auto r = bar();
    r.skills = {skill("pulse", "A", TargetRule::Enemy, {{EffectKind::Damage}}, 3)};
    auto roster = duel(100,100); auto reversed = roster; std::ranges::reverse(reversed);
    Battle first(roster,r), second(reversed,r);
    for (int i=0;i<24;++i) {
        const auto owner = first.current_actor()->definition.id;
        const Action command = owner == "A" && cd(first,"A","pulse") == 0 ?
            Action{"A",ActionType::Skill,"B","pulse"} :
            (owner == "B" ? Action{"B",ActionType::Attack,"A"} : Action{owner,ActionType::Guard,{}});
        check(act(first,command) == act(second,command) && first.snapshot() == second.snapshot(),
              "new cooldown cycle changed replay");
        check(cd(first,"A","pulse") >= 0 && cd(first,"A","pulse") <= 2, "cooldown left cycle bounds");
    }
    check(first.find("B")->health == 9960 && first.accepted_actions() == 24,
          "cooldown 3 did not repeat every three owner turns");
}
EffectDefinition extra_from(std::string id, ExtraOrigin origin, bool cooldowns, bool durations,
                            std::uint32_t cap = 0) {
    EffectDefinition effect{EffectKind::ExtraTurn};
    effect.extra_source = ExtraTurnSource{std::move(id), origin, cooldowns, durations, cap};
    return effect;
}
void extra_flags_independent() {
    for (const auto profile : {TimingProfile::FixedRounds, TimingProfile::ActionBar}) {
        for (const bool cooldowns : {false,true}) { for (const bool durations : {false,true}) {
            auto r=bar();r.timing=profile;
            r.skills={skill("grant","A",TargetRule::Self,{extra_from("custom",ExtraOrigin::Custom,cooldowns,durations)}),
                      skill("watched","A",TargetRule::Enemy,{{EffectKind::Damage}},3)};
            Battle b(duel(),r,{},{{"A",200,{{EffectKind::Marker,3,TickPolicy::NaturalOnly,0}},{{"grant",0},{"watched",3}}}});
            act(b,{"A",ActionType::Skill,"A","grant"});
            const auto before=b.snapshot();
            check(cd(b,"A","watched")==2 && b.find("A")->statuses[0].remaining==2,"natural ticks changed");
            guard(b);
            check(cd(b,"A","watched")== (cooldowns?1:2) &&
                  b.find("A")->statuses[0].remaining==(durations?1:2),"extra flags not independent");
            check(before.current_extra && before.current_extra->source &&
                  before.current_extra->source->advances_cooldowns==cooldowns &&
                  before.current_extra->source->advances_effect_durations==durations,"flags missing from snapshot");
        }}
    }
}
void extra_fifo_and_provenance() {
    auto r=bar();
    r.skills={skill("grant","A",TargetRule::Self,
        {extra_from("equipment",ExtraOrigin::Equipment,false,true,1),extra_from("ability",ExtraOrigin::Ability,true,true)}),
        skill("watched","A",TargetRule::Enemy,{{EffectKind::Damage}},3)};
    Battle b(duel(),r,{},{{"A",200,{{EffectKind::Marker,4,TickPolicy::NaturalOnly,0}},{{"grant",0},{"watched",3}}}});
    const auto initial=b.snapshot();act(b,{"A",ActionType::Skill,"A","grant"});
    const auto first=b.snapshot();
    check(first.current_extra && first.current_extra->source->id=="equipment" &&
          first.current_extra->source_actor=="A" && first.current_extra->granting_skill=="grant" &&
          first.current_extra->granting_turn==1 && first.current_extra->originating_natural_turn==1 &&
          first.pending_extra_contexts.size()==1 && first.pending_extra_contexts[0].source->id=="ability",
          "FIFO or provenance lost");
    const auto result=guard(b);const auto second=b.snapshot();
    check(cd(b,"A","watched")==2 && b.find("A")->statuses[0].remaining==2 &&
          second.current_extra->source->id=="ability" && second.logical_time==initial.logical_time &&
          second.fighters[1].ap==initial.fighters[1].ap,"equipment extra changed CD/AP or lost duration tick");
    const auto ended=std::ranges::find(result.events,EventType::TurnEnded,&Event::type);
    const auto started=std::ranges::find(result.events,EventType::TurnStarted,&Event::type);
    check(ended!=result.events.end() && ended->extra_context && ended->extra_context->source->id=="equipment" &&
          started!=result.events.end() && started->extra_context && started->extra_context->source->id=="ability",
          "event context crossed turn boundary");
    guard(b);
    check(cd(b,"A","watched")==1 && b.find("A")->statuses[0].remaining==1,"ability extra not full clock");
}
void extra_source_caps() {
    auto r=bar();r.skills={skill("grant","A",TargetRule::Self,{extra_from("equipment",ExtraOrigin::Equipment,false,true,1)})};
    Battle b(duel(),r);act(b,{"A",ActionType::Skill,"A","grant"});
    check(b.snapshot().extra_proc_counts.size()==1 && b.snapshot().extra_proc_counts[0].count==1,"proc not recorded");
    const auto before=b.snapshot();
    reject(b,{"A",ActionType::Skill,"B","grant"},ActionError::InvalidSkillTarget);
    check(b.snapshot()==before,"rejected action consumed proc");
    const auto suppressed=act(b,{"A",ActionType::Skill,"A","grant"});
    check(std::ranges::any_of(suppressed.events,[](const Event& e){return e.type==EventType::ExtraTurnSuppressed;}) &&
          b.phase()==Phase::Running && b.snapshot().turn_kind==TurnKind::Natural &&
          b.snapshot().extra_turns==1 && b.snapshot().extra_proc_counts.empty(),"source cap treated as technical failure");
    check(b.snapshot().originating_natural_turn!=before.originating_natural_turn,"natural origin not renewed");
    act(b,{"A",ActionType::Skill,"A","grant"});
    check(b.snapshot().turn_kind==TurnKind::Extra && b.snapshot().extra_proc_counts[0].count==1,"source cap not reset");
    // Two different skills of one owner share the same source limit, rather than bypassing it.
    r.skills.push_back(skill("second","A",TargetRule::Self,{extra_from("equipment",ExtraOrigin::Equipment,false,true,1)}));
    Battle shared(duel(),r);act(shared,{"A",ActionType::Skill,"A","grant"});
    const auto same=act(shared,{"A",ActionType::Skill,"A","second"});
    check(std::ranges::any_of(same.events,[](const Event& e){return e.type==EventType::ExtraTurnSuppressed;}),"skill alias bypassed cap");
}
void extra_source_validation() {
    auto r=bar();auto effect=extra_from("",ExtraOrigin::Ability,true,true);
    r.skills={skill("s","A",TargetRule::Self,{effect})};invalid([&]{Battle b(duel(),r);});
    effect=extra_from("x",static_cast<ExtraOrigin>(99),true,true);
    r.skills[0].effects={effect};invalid([&]{Battle b(duel(),r);});
    effect=extra_from("x",ExtraOrigin::Ability,true,true,1025);
    r.skills[0].effects={effect};invalid([&]{Battle b(duel(),r);});
    effect=extra_from("x",ExtraOrigin::Ability,true,true);effect.kind=EffectKind::Marker;effect.duration=2;
    r.skills[0].effects={effect};invalid([&]{Battle b(duel(),r);});
    r.skills[0].effects={extra_from("x",ExtraOrigin::Ability,true,true),extra_from("x",ExtraOrigin::Equipment,false,true,1)};
    invalid([&]{Battle b(duel(),r);});
}
void sourced_extra_death_and_budget() {
    auto r=bar();r.skills={skill("kill","A",TargetRule::Enemy,
        {extra_from("source",ExtraOrigin::Ability,true,true),{EffectKind::Damage}})};
    auto roster=duel();roster[1].max_health=10;roster.push_back({"B2",Team::B,100,1,50});
    Battle dead(roster,r);act(dead,{"A",ActionType::Skill,"B","kill"});
    check(dead.snapshot().pending_extra_contexts.empty() && !dead.snapshot().current_extra &&
          dead.find("B")->health==0,"sourced dead extra survived queue pruning");
    r.action_budget=ActionBudget{1,LimitOutcome::Draw};
    r.skills={skill("grant","A",TargetRule::Self,{extra_from("source",ExtraOrigin::Ability,true,true)})};
    Battle budget(duel(),r);act(budget,{"A",ActionType::Skill,"A","grant"});
    check(budget.end_reason()==EndReason::ActionLimit && budget.snapshot().extra_turns==0 &&
          !budget.snapshot().current_extra,"sourced extra bypassed primary budget");
    reject(budget,{"A",ActionType::Guard,{}},ActionError::BattleFinished);
}
void sourced_extra_stage_reset() {
    auto d=waves(2,RuntimeCarry::Preserve);
    for(auto& wave:d.encounters) {
        wave.rules.skills={skill("kill","A",TargetRule::Enemy,
            {extra_from("equipment",ExtraOrigin::Equipment,false,true,1),{EffectKind::Damage}})};
    }
    Stage stage(d);const auto initial=stage.snapshot();
    check(stage.apply({"A",ActionType::Skill,"B","kill"}).encounter_advanced,"sourced stage did not advance");
    const auto after=stage.battle().snapshot();
    check(after.pending_extra_contexts.empty() && !after.current_extra && after.extra_proc_counts.empty() &&
          after.originating_natural_turn==1 && after.accepted_actions==0 && stage.snapshot().accepted_actions==1,
          "extra metadata leaked across encounter");
    stage.reset();check(stage.snapshot()==initial,"sourced stage reset failed");
}
void sourced_extra_replay() {
    auto r=bar();r.skills={skill("grant","A",TargetRule::Self,
        {extra_from("equipment",ExtraOrigin::Equipment,false,true,1),extra_from("ability",ExtraOrigin::Ability,true,true)})};
    auto roster=duel();auto reversed=roster;std::ranges::reverse(reversed);
    Battle a(roster,r),b(reversed,r);
    for(int i=0;i<18;++i) {
        const auto owner=a.current_actor()->definition.id;
        const Action command=owner=="A" && a.snapshot().turn_kind==TurnKind::Natural ?
            Action{"A",ActionType::Skill,"A","grant"}:Action{owner,ActionType::Guard,{}};
        check(act(a,command)==act(b,command) && a.snapshot()==b.snapshot(),"source context replay diverged");
    }
    check(a.snapshot().extra_turns>0 && a.snapshot().natural_turns>0,"replay did not exercise both opportunities");
}
int duration(const Battle& b, const std::string& owner, EffectKind kind) {
    for(const auto& status:b.find(owner)->statuses) {if(status.kind==kind){return status.remaining;}}
    return 0;
}
void control_freeze_independent() {
    for(const auto profile:{TimingProfile::FixedRounds,TimingProfile::ActionBar}) {
        for(const bool blocked:{false,true}) {for(const bool frozen:{false,true}) {
            auto r=bar();r.timing=profile;
            r.skills={skill("watched","A",TargetRule::Enemy,{{EffectKind::Damage}},3)};
            std::vector<Status> statuses;
            if(blocked){statuses.push_back({EffectKind::Block,1,TickPolicy::NaturalOnly,0});}
            if(frozen){statuses.push_back({EffectKind::FreezeCooldowns,1,TickPolicy::NaturalOnly,0});}
            Battle b(duel(100,100),r,{},{{"A",100,statuses,{{"watched",2}}}});
            check(b.current_actor_blocked()==blocked && b.current_actor_cooldowns_frozen()==frozen,"control properties coupled");
            if(blocked){
                reject(b,{"A",ActionType::Attack,"B"},ActionError::ActorBlocked);
                act(b,{"A",ActionType::Skip,{}});
            } else {act(b,{"A",ActionType::Attack,"B"});}
            check(cd(b,"A","watched")== (frozen?2:1),"block and freeze use same cooldown rule");
            check(b.find("B")->health==(blocked?10000:9990) && b.accepted_actions()==1 &&
                  duration(b,"A",EffectKind::Block)==0 && duration(b,"A",EffectKind::FreezeCooldowns)==0,
                  "control expiration or command budget wrong");
        }}
    }
}
void freeze_applied_and_expiration() {
    auto r=bar();r.skills={skill("freeze","A",TargetRule::Self,{{EffectKind::FreezeCooldowns,0,1}},3)};
    Battle b(duel(100,100),r);act(b,{"A",ActionType::Skill,"A","freeze"});
    check(cd(b,"A","freeze")==3 && duration(b,"A",EffectKind::FreezeCooldowns)==1,
          "self freeze failed to block use decrement or expired immediately");
    act(b,{"B",ActionType::Attack,"A"});
    check(b.current_actor_cooldowns_frozen() && !b.current_actor_blocked(),"freeze incorrectly blocks action");
    reject(b,{"A",ActionType::Skill,"A","freeze"},ActionError::SkillOnCooldown);
    guard(b);
    check(cd(b,"A","freeze")==3 && duration(b,"A",EffectKind::FreezeCooldowns)==0,"last frozen completion decremented cooldown");
    act(b,{"B",ActionType::Attack,"A"});guard(b);
    check(cd(b,"A","freeze")==2,"cooldown did not resume after freeze expired");
}
void freeze_overrides_extra_flags() {
    for(const bool cooldowns:{false,true}) {for(const bool durations:{false,true}) {
        auto r=bar();r.skills={skill("grant","A",TargetRule::Self,
            {extra_from("custom",ExtraOrigin::Custom,cooldowns,durations)}),
            skill("watched","A",TargetRule::Enemy,{{EffectKind::Damage}},3)};
        Battle b(duel(),r,{},{{"A",200,{{EffectKind::FreezeCooldowns,2,TickPolicy::NaturalOnly,0}},
                              {{"grant",0},{"watched",2}}}});
        act(b,{"A",ActionType::Skill,"A","grant"});
        check(duration(b,"A",EffectKind::FreezeCooldowns)==1 && cd(b,"A","watched")==2,"natural freeze tick wrong");
        const auto before=b.snapshot();guard(b);
        check(cd(b,"A","watched")==2 && duration(b,"A",EffectKind::FreezeCooldowns)==(durations?0:1),
              "extra bypassed freeze or froze status duration implicitly");
        check(before.current_extra && before.current_extra->source->advances_cooldowns==cooldowns,
              "test did not exercise configured extra");
    }}
}
void frozen_stage_carry_and_reset() {
    for(const int count:{1,2,5}) {for(const auto policy:{RuntimeCarry::Preserve,RuntimeCarry::Reset}) {
        auto d=waves(count,policy);
        for(auto& wave:d.encounters){
            wave.rules.skills={skill("watched","A",TargetRule::Self,{{EffectKind::Marker,0,9}},3),
                               skill("freeze","A",TargetRule::Self,{{EffectKind::FreezeCooldowns,0,3}})};
        }
        Stage stage(d);const auto initial=stage.snapshot();
        check(stage.apply({"A",ActionType::Skill,"A","watched"}).action.accepted(),"stage watch use rejected");
        check(stage.apply({"A",ActionType::Skill,"A","freeze"}).action.accepted(),"stage freeze use rejected");
        while(stage.battle().current_actor()->definition.id!="A"){
            const auto command=choose_basic_attack(stage.battle());check(command.has_value(),"no enemy command");
            check(stage.apply(*command).action.accepted(),"stage enemy action rejected");
        }
        const auto result=stage.apply({"A",ActionType::Attack,"B"});
        check(result.action.accepted() && cd(stage.battle(),"A","watched")==
              (count>1 && policy==RuntimeCarry::Reset?0:2),"stage lost frozen cooldown");
        check(duration(stage.battle(),"A",EffectKind::FreezeCooldowns)==
              (count>1 && policy==RuntimeCarry::Reset?0:2),"stage lost frozen duration");
        while(stage.snapshot().phase==Phase::Running){
            const auto command=choose_basic_attack(stage.battle());check(command.has_value(),"no stage continuation");
            check(stage.apply(*command).action.accepted(),"stage continuation rejected");
        }
        check(stage.snapshot().end_reason==StageEndReason::Completed,"freeze stage did not complete");
        if(count==5 && policy==RuntimeCarry::Preserve){
            check(cd(stage.battle(),"A","watched")==0 && duration(stage.battle(),"A",EffectKind::FreezeCooldowns)==0,
                  "preserved freeze never expired or cooldown never resumed");
        }
        stage.reset();check(stage.snapshot()==initial,"reset retained control/freeze state");
    }}
}
void frozen_final_budget() {
    auto r=bar();r.action_budget=ActionBudget{1,LimitOutcome::Draw};
    r.skills={skill("watched","A",TargetRule::Enemy,{{EffectKind::Damage}},3)};
    Battle b(duel(),r,{},{{"A",200,{{EffectKind::Block,1,TickPolicy::NaturalOnly,0},
        {EffectKind::FreezeCooldowns,1,TickPolicy::NaturalOnly,0}},{{"watched",2}}}});
    const auto result=act(b,{"A",ActionType::Skip,{}});
    check(b.end_reason()==EndReason::ActionLimit && b.accepted_actions()==1 && cd(b,"A","watched")==2 &&
          duration(b,"A",EffectKind::FreezeCooldowns)==0 && result.events.back().type==EventType::BattleFinished,
          "last blocked frozen turn violated budget/expiration");
    reject(b,{"A",ActionType::Guard,{}},ActionError::BattleFinished);
}
void frozen_replay() {
    auto r=bar();r.skills={skill("watched","A",TargetRule::Enemy,{{EffectKind::Damage}},3)};
    const std::vector<InitialRuntime> state={{"A",100,
        {{EffectKind::Block,1,TickPolicy::NaturalOnly,0},{EffectKind::FreezeCooldowns,2,TickPolicy::NaturalOnly,0}},{{"watched",2}}}};
    auto roster=duel(100,100);auto reverse=roster;std::ranges::reverse(reverse);
    Battle a(roster,r,{},state),b(reverse,r,{},state);
    for(int i=0;i<16;++i){
        const auto command=choose_basic_attack(a);check(command.has_value(),"no frozen replay command");
        check(act(a,*command)==act(b,*command) && a.snapshot()==b.snapshot(),"frozen replay mismatch");
    }
    check(cd(a,"A","watched")==0 && duration(a,"A",EffectKind::FreezeCooldowns)==0,"replay never thawed");
}
void freeze_validation() {
    auto r=bar();r.skills={skill("bad","A",TargetRule::Self,{{EffectKind::FreezeCooldowns,0,0}})};
    invalid([&]{Battle b(duel(),r);});
    r.skills.clear();
    const std::vector<Status> all={{EffectKind::Block,1,TickPolicy::NaturalOnly,0},
        {EffectKind::Marker,1,TickPolicy::NaturalOnly,0},{EffectKind::Counter,1,TickPolicy::NaturalOnly,0},
        {EffectKind::FreezeCooldowns,1,TickPolicy::NaturalOnly,0}};
    Battle b(duel(),r,{},{{"A",200,all,{}}});
    check(b.current_actor_blocked() && b.current_actor_cooldowns_frozen(),"four status types rejected");
    invalid([&]{Battle bad(duel(),r,{},{{"A",200,{{EffectKind::FreezeCooldowns,0,TickPolicy::NaturalOnly,0}}, {}}});});
    invalid([&]{Battle bad(duel(),r,{},{{"A",200,{{EffectKind::FreezeCooldowns,1,TickPolicy::NaturalOnly,0},
        {EffectKind::FreezeCooldowns,2,TickPolicy::NaturalOnly,0}}, {}}});});
}
}
int main(int argc, char** argv) {
    const std::pair<const char*,void(*)()> tests[] = {
        {"configuration",configuration},{"frequency_replay",frequency_and_replay},{"ap_speed",ap_and_speed},
        {"extras_lifetimes",extras_and_lifetimes},{"blocking_skip",blocking_and_skip},{"guard_toggle",guard_toggle_and_extra},
        {"reaction_clocks",reaction_is_not_turn},{"reaction_lethal",reaction_lethal_and_budget},
        {"technical_limits",technical_limits},{"skill_atomics",skill_validation_and_atomics},
        {"stage_runtime",stage_runtime},{"stage_budgets_replay",stage_budgets_and_replay},{"formations_25",formation_matrix},
        {"fixed_extras",fixed_profile_extras},{"pending_death_block",pending_death_and_blocked_reactions},
        {"deadline_edges",deadline_and_numeric_edges},{"cooldown_zero",cooldown_zero},
        {"cooldown_one",cooldown_one},{"cooldown_three",cooldown_three},{"cooldown_replay",cooldown_replay},
        {"extra_flags",extra_flags_independent},{"extra_fifo_origin",extra_fifo_and_provenance},
        {"extra_source_caps",extra_source_caps},{"extra_source_validation",extra_source_validation},
        {"sourced_death_budget",sourced_extra_death_and_budget},{"sourced_stage_reset",sourced_extra_stage_reset},
        {"sourced_replay",sourced_extra_replay},{"control_freeze_independent",control_freeze_independent},
        {"freeze_apply_expire",freeze_applied_and_expiration},{"freeze_extra_flags",freeze_overrides_extra_flags},
        {"frozen_stage_carry_reset",frozen_stage_carry_and_reset},{"frozen_budget",frozen_final_budget},
        {"frozen_replay",frozen_replay},{"freeze_validation",freeze_validation}};
    int passed=0;
    for(const auto& [name,test]:tests) {
        try {test();++passed;std::cout<<"PASS timing "<<name<<'\n';}
        catch(const std::exception& e) {std::cerr<<"FAIL timing "<<name<<": "<<e.what()<<'\n';}
    }
    std::cout<<"TIMING_TESTS_RESULT passed="<<passed<<" total="<<std::size(tests)<<'\n';
    if(argc==2 && passed==static_cast<int>(std::size(tests))) {
        std::ofstream output(argv[1]);
        output<<"test,status,scope\n";
        for(const auto& [name,test]:tests) {static_cast<void>(test);output<<name<<",pass,own_cpp_timing_profile\n";}
        if(!output) {std::cerr<<"Cannot save timing matrix\n";return 2;}
    }
    return passed==static_cast<int>(std::size(tests))?0:1;
}
