#include "rpg/battle.hpp"

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <limits>

namespace rpg {
namespace {
constexpr std::int64_t bar_max = 1000000;
bool quantity_effect(EffectKind kind){return kind==EffectKind::Reflect || kind==EffectKind::DeathExplosion || kind==EffectKind::AutoRevive || kind==EffectKind::Shield || kind==EffectKind::StructuralShield || kind==EffectKind::StandardShield;}
bool valid_clock(TickPolicy clock) {
    return clock == TickPolicy::NaturalOnly || clock == TickPolicy::EveryTurn;
}
bool ticks(TickPolicy clock, TurnKind kind) {
    return kind == TurnKind::Natural || clock == TickPolicy::EveryTurn;
}
bool valid_duration(StatusCategory category,int remaining) {
    if(category==StatusCategory::Temporary){return remaining>=1 && remaining<=1000000;}
    return (category==StatusCategory::Permanent || category==StatusCategory::Structural) && remaining==0;
}
}

Battle::Battle(std::vector<Definition> definitions, BattleRules rules,
               std::vector<InitialHealth> initial_health,
               std::vector<InitialRuntime> initial_runtime) : rules_(std::move(rules)) {
    if ((rules_.timing != TimingProfile::FixedRounds && rules_.timing != TimingProfile::ActionBar) ||
        rules_.max_derived_actions == 0 || rules_.max_derived_actions > 1024 || rules_.skills.size() > 256 ||
        rules_.max_consequence_events == 0 || rules_.max_consequence_events > 4096 || rules_.numeric_model != 2 ||
        (rules_.combat_model!=resolution::CombatModel::Legacy && rules_.combat_model!=resolution::CombatModel::FunnelV1)) {
        throw std::invalid_argument("Invalid timing configuration");
    }
    if (rules_.action_budget) {
        const auto& budget = *rules_.action_budget;
        if (budget.max_actions <= 0 ||
            (budget.outcome != LimitOutcome::Draw && budget.outcome != LimitOutcome::DefeatA &&
             budget.outcome != LimitOutcome::DefeatB)) {
            throw std::invalid_argument("Invalid action budget");
        }
    }
    // Normalize quantity domains only. AP, speed, counters, durations and IDs keep their contracts.
    for(auto& definition:definitions){if(definition.max_health>0){definition.max_health=numeric::quantity(definition.max_health);}if(definition.attack>0){definition.attack=numeric::quantity(definition.attack);}}
    for(auto& skill:rules_.skills){for(auto& effect:skill.effects){if(quantity_effect(effect.kind) && effect.amount>0){effect.amount=numeric::quantity(effect.amount);}}}
    for(auto& state:initial_health){if(state.health>0){state.health=numeric::quantity(state.health);}}
    for(auto& state:initial_runtime){for(auto& status:state.statuses){if(quantity_effect(status.kind) && status.magnitude>0){status.magnitude=numeric::quantity(status.magnitude);}}}
    std::ranges::sort(definitions, {}, &Definition::id);
    bool has_a = false;
    bool has_b = false;
    for (std::size_t i = 0; i < definitions.size(); ++i) {
        const auto& definition = definitions[i];
        if (definition.id.empty() || definition.max_health <= 0 ||
            definition.attack <= 0 || definition.speed <= 0 ||
            (rules_.timing == TimingProfile::ActionBar && definition.speed > 10000) ||
            (definition.team != Team::A && definition.team != Team::B)) {
            throw std::invalid_argument("Invalid fighter definition");
        }
        if (i > 0 && definitions[i - 1].id == definition.id) {
            throw std::invalid_argument("Duplicate fighter ID");
        }
        resolution::validate_stats(definition.stats);
        has_a = has_a || definition.team == Team::A;
        has_b = has_b || definition.team == Team::B;
    }
    if (!has_a || !has_b) {
        throw std::invalid_argument("Both teams require at least one fighter");
    }
    fighters_.reserve(definitions.size());
    for (auto& definition : definitions) {
        const numeric::Quantity health = definition.max_health;
        fighters_.push_back(Fighter{std::move(definition), health, false});
        fighters_.back().effective_speed = fighters_.back().definition.speed;
    }
    std::vector<std::pair<std::string, ExtraTurnSource>> sources;
    for (std::size_t i = 0; i < rules_.skills.size(); ++i) {
        const auto& skill = rules_.skills[i];
        const auto owner = index_of(skill.owner);
        if (skill.id.empty() || !owner || skill.cooldown < 0 || skill.cooldown > 1000000 ||
            !valid_clock(skill.cooldown_clock) || skill.effects.empty() || skill.effects.size() > 16 ||
            (skill.target != TargetRule::Enemy && skill.target != TargetRule::Ally && skill.target != TargetRule::Self)) {
            throw std::invalid_argument("Invalid skill");
        }
        for (std::size_t j = 0; j < i; ++j) {
            if (rules_.skills[j].id == skill.id) { throw std::invalid_argument("Duplicate skill ID"); }
        }
        std::uint32_t planned_hits = 0;
        for (const auto& effect : skill.effects) {
            if(effect.power_ratio<0 || effect.power_ratio>numeric::quantity_max ||
               (effect.damage_kind!=resolution::DamageKind::Normal && effect.damage_kind!=resolution::DamageKind::True && effect.damage_kind!=resolution::DamageKind::Dot)){
                throw std::invalid_argument("Invalid damage effect metadata");
            }
            if (!valid_clock(effect.clock)) { throw std::invalid_argument("Invalid effect clock"); }
            if (effect.hit_count == 0 || effect.hit_count > 64 ||
                (effect.kind != EffectKind::Damage && effect.hit_count != 1)) {
                throw std::invalid_argument("Invalid hit count");
            }
            if ((effect.hit_selection != HitSelection::FixedTarget && effect.hit_selection != HitSelection::LowestHealthEnemy) ||
                (effect.hit_selection != HitSelection::FixedTarget &&
                 (effect.kind != EffectKind::Damage || skill.target != TargetRule::Enemy))) {
                throw std::invalid_argument("Invalid dynamic target selection");
            }
            if (effect.kind == EffectKind::Damage) { planned_hits += effect.hit_count; }
            if (planned_hits > 256) { throw std::invalid_argument("Too many hits per skill"); }
            if (effect.extra_source) {
                const auto& source = *effect.extra_source;
                if (effect.kind != EffectKind::ExtraTurn || source.id.empty() ||
                    (source.origin != ExtraOrigin::Ability && source.origin != ExtraOrigin::Equipment && source.origin != ExtraOrigin::Custom) ||
                    source.max_procs_per_natural_turn > 1024) {
                    throw std::invalid_argument("Invalid extra source");
                }
                for (const auto& [owner_id, known] : sources) {
                    if (owner_id == skill.owner && known.id == source.id && known != source) {
                        throw std::invalid_argument("Inconsistent extra source policy");
                    }
                }
                sources.emplace_back(skill.owner, source);
            }
            switch (effect.kind) {
            case EffectKind::StandardShield:
                if(effect.amount!=0 || effect.duration!=0 || effect.clock!=TickPolicy::NaturalOnly ||
                   effect.category!=StatusCategory::Temporary || effect.charges!=1 ||
                   (skill.target!=TargetRule::Self && skill.target!=TargetRule::Ally)) {
                    throw std::invalid_argument("Standard shield requires fixed parameters and living ally target");
                }
                break;
            case EffectKind::ChangeAP:
                if (rules_.timing != TimingProfile::ActionBar || effect.amount < -bar_max || effect.amount > bar_max) {
                    throw std::invalid_argument("Invalid AP effect");
                }
                break;
            case EffectKind::SetSpeed:
                if (rules_.timing != TimingProfile::ActionBar || effect.amount < 1 || effect.amount > 10000) {
                    throw std::invalid_argument("Invalid speed effect");
                }
                break;
            case EffectKind::Block: case EffectKind::Marker: case EffectKind::Counter: case EffectKind::FreezeCooldowns:
            case EffectKind::Reflect: case EffectKind::DeathExplosion: case EffectKind::PreventDeath: case EffectKind::AutoRevive:
            case EffectKind::Shield: case EffectKind::StructuralShield:
                if (!valid_duration(effect.category,effect.duration)) { throw std::invalid_argument("Invalid duration/category"); }
                if ((effect.kind == EffectKind::Reflect || effect.kind == EffectKind::DeathExplosion ||
                     effect.kind == EffectKind::AutoRevive || effect.kind == EffectKind::Shield ||
                     effect.kind == EffectKind::StructuralShield) && effect.amount <= 0) {
                    throw std::invalid_argument("Consequence damage must be positive");
                }
                if ((effect.kind == EffectKind::PreventDeath || effect.kind == EffectKind::AutoRevive) &&
                    (effect.charges == 0 || effect.charges > 1024)) { throw std::invalid_argument("Invalid survival charges"); }
                break;
            case EffectKind::ExtraTurn: case EffectKind::Damage:
                if(effect.category!=StatusCategory::Temporary){throw std::invalid_argument("Instant effect cannot persist");}
                break;
            default: throw std::invalid_argument("Unknown effect");
            }
            if ((effect.kind==EffectKind::Shield && effect.category!=StatusCategory::Temporary) ||
                (effect.kind==EffectKind::StructuralShield && effect.category!=StatusCategory::Structural) ||
                ((effect.kind==EffectKind::SetSpeed || effect.kind==EffectKind::ChangeAP) &&
                 effect.category!=StatusCategory::Temporary)) { throw std::invalid_argument("Invalid category for effect"); }
            if (effect.kind == EffectKind::Damage && skill.target != TargetRule::Enemy) {
                throw std::invalid_argument("Damage requires enemy target in this profile");
            }
        }
        fighters_[*owner].cooldowns.push_back({skill.id, 0});
    }
    if (!initial_health.empty()) {
        std::ranges::sort(initial_health, {}, &InitialHealth::id);
        if (initial_health.size() != fighters_.size()) { throw std::invalid_argument("Incomplete initial health"); }
        bool alive_a = false;
        bool alive_b = false;
        for (std::size_t i = 0; i < fighters_.size(); ++i) {
            auto& fighter = fighters_[i];
            const auto& state = initial_health[i];
            if (state.id != fighter.definition.id || state.health < 0 || state.health > fighter.definition.max_health) {
                throw std::invalid_argument("Invalid initial health");
            }
            fighter.health = state.health;
            alive_a = alive_a || (state.health > 0 && fighter.definition.team == Team::A);
            alive_b = alive_b || (state.health > 0 && fighter.definition.team == Team::B);
        }
        if (!alive_a || !alive_b) { throw std::invalid_argument("Both sides need living fighters"); }
    }
    std::ranges::sort(initial_runtime, {}, &InitialRuntime::id);
    for (std::size_t i = 0; i < initial_runtime.size(); ++i) {
        const auto& state = initial_runtime[i];
        const auto index = index_of(state.id);
        if (!index || (i > 0 && initial_runtime[i - 1].id == state.id) || state.effective_speed < 1 ||
            (rules_.timing == TimingProfile::ActionBar && state.effective_speed > 10000)) {
            throw std::invalid_argument("Invalid runtime identity/speed");
        }
        auto& fighter = fighters_[*index];
        if (state.cooldowns.size() != fighter.cooldowns.size()) { throw std::invalid_argument("Incomplete cooldowns"); }
        auto cooldowns = state.cooldowns;
        std::ranges::sort(cooldowns, {}, &Cooldown::skill);
        for (std::size_t j = 0; j < cooldowns.size(); ++j) {
            const auto skill = std::ranges::find(rules_.skills, cooldowns[j].skill, &SkillDefinition::id);
            if (skill == rules_.skills.end() || skill->owner != state.id || cooldowns[j].remaining < 0 ||
                cooldowns[j].remaining > 1000000 || (j > 0 && cooldowns[j-1].skill == cooldowns[j].skill)) {
                throw std::invalid_argument("Invalid cooldown state");
            }
        }
        if (state.statuses.size() > 10) { throw std::invalid_argument("Too many status types"); }
        auto statuses = state.statuses;
        for (std::size_t j = 0; j < statuses.size(); ++j) {
            auto& status = statuses[j];
            if ((status.kind != EffectKind::Block && status.kind != EffectKind::Marker && status.kind != EffectKind::Counter &&
                 status.kind != EffectKind::FreezeCooldowns && status.kind != EffectKind::Reflect &&
                 status.kind != EffectKind::DeathExplosion && status.kind != EffectKind::PreventDeath &&
                 status.kind != EffectKind::AutoRevive && status.kind != EffectKind::Shield &&
                 status.kind != EffectKind::StructuralShield && status.kind != EffectKind::StandardShield) ||
                !valid_duration(status.category,status.remaining) || !valid_clock(status.clock)) {
                throw std::invalid_argument("Invalid status state");
            }
            if ((status.kind == EffectKind::Reflect || status.kind == EffectKind::DeathExplosion ||
                 status.kind == EffectKind::AutoRevive || status.kind==EffectKind::Shield ||
                 status.kind==EffectKind::StructuralShield || status.kind==EffectKind::StandardShield) && status.magnitude <= 0) {
                throw std::invalid_argument("Invalid consequence magnitude");
            }
            if(status.kind==EffectKind::StandardShield && (fighter.health==0 || status.clock!=TickPolicy::NaturalOnly || status.category!=StatusCategory::Temporary)){throw std::invalid_argument("Invalid standard shield state");}
            if ((status.kind==EffectKind::Shield && status.category!=StatusCategory::Temporary) ||
                (status.kind==EffectKind::StructuralShield && status.category!=StatusCategory::Structural)) {
                throw std::invalid_argument("Invalid shield category");
            }
            if ((status.kind == EffectKind::PreventDeath || status.kind == EffectKind::AutoRevive) &&
                (status.charges == 0 || status.charges > 1024)) { throw std::invalid_argument("Invalid survival state"); }
            for (std::size_t k = 0; k < j; ++k) {
                if (statuses[k].kind == status.kind) { throw std::invalid_argument("Duplicate status type"); }
            }
            status.applied_turn = 0; // Entered from the preceding encounter, never a newly-applied local status.
        }
        fighter.effective_speed = state.effective_speed;
        fighter.cooldowns = std::move(cooldowns);
        fighter.statuses = std::move(statuses);
    }
    for (std::size_t i = 0; i < fighters_.size(); ++i) {
        if (fighters_[i].health > 0) { order_.push_back(i); }
    }
    std::ranges::sort(order_, [this](std::size_t left, std::size_t right) {
        const auto& a = fighters_[left].definition;
        const auto& b = fighters_[right].definition;
        if (a.speed != b.speed) { return a.speed > b.speed; }
        return a.id < b.id;
    });
    active_ = order_.front();
    if (rules_.timing == TimingProfile::ActionBar) { select_bar_actor(); }
}

std::optional<std::size_t> Battle::index_of(std::string_view id) const noexcept {
    for (std::size_t i = 0; i < fighters_.size(); ++i) {
        if (fighters_[i].definition.id == id) { return i; }
    }
    return std::nullopt;
}

const Fighter* Battle::find(std::string_view id) const noexcept {
    const auto index = index_of(id);
    return index ? &fighters_[*index] : nullptr;
}

const Fighter* Battle::current_actor() const noexcept {
    return phase_ == Phase::Running ? &fighters_[active_] : nullptr;
}

std::optional<std::uint64_t> Battle::remaining_actions() const noexcept {
    if (!rules_.action_budget) { return std::nullopt; }
    return static_cast<std::uint64_t>(rules_.action_budget->max_actions) - accepted_actions_;
}

Snapshot Battle::snapshot() const {
    std::vector<std::string> living_order;
    for (const auto index : order_) {
        if (fighters_[index].health > 0) {
            living_order.push_back(fighters_[index].definition.id);
        }
    }
    const auto* actor = current_actor();
    std::vector<std::string> pending;
    for (const auto& extra : extras_) { pending.push_back(extra.beneficiary); }
    return Snapshot{fighters_, std::move(living_order), cursor_, round_, phase_, winner_,
                    actor ? std::optional<std::string>{actor->definition.id} : std::nullopt,
                    accepted_actions_, rules_, end_reason_, logical_time_, turn_id_, turn_kind_,
                    natural_turns_, extra_turns_, derived_actions_, std::move(pending), extra_chain_,
                    current_extra_, extras_, originating_natural_turn_, extra_proc_counts_, next_action_id_, last_consequence_events_};
}

bool Battle::current_actor_blocked() const noexcept {
    const auto* actor = current_actor();
    return actor && std::ranges::any_of(actor->statuses, [](const Status& status) {
        return status.kind == EffectKind::Block;
    });
}

bool Battle::current_actor_cooldowns_frozen() const noexcept {
    const auto* actor = current_actor();
    return actor && std::ranges::any_of(actor->statuses, [](const Status& status) {
        return status.kind == EffectKind::FreezeCooldowns;
    });
}

void Battle::stamp(std::vector<Event>& events, std::size_t begin, std::uint64_t turn, TurnKind kind) const {
    // Keep the original fixed-profile event contract unchanged for regression.
    if (rules_.timing == TimingProfile::FixedRounds && rules_.skills.empty()) { return; }
    for (std::size_t i = begin; i < events.size(); ++i) {
        if (events[i].turn_id == 0) {
            events[i].turn_id = turn; events[i].turn_kind = kind;
            events[i].extra_context = current_extra_;
        }
    }
}

ActionContext Battle::make_context(std::size_t actor, ActionType type, const std::string& target,
                                   const std::string& skill, std::uint64_t parent, std::uint64_t root) {
    if (next_action_id_ == std::numeric_limits<std::uint64_t>::max()) { throw std::overflow_error("Action ID overflow"); }
    const auto id = next_action_id_++;
    return {id, parent, root == 0 ? id : root, fighters_[actor].definition.id,
            fighters_[actor].definition.team, type, target, skill};
}

void Battle::stamp_action(std::vector<Event>& events, std::size_t begin, const ActionContext& context) {
    for (std::size_t i=begin; i<events.size(); ++i) {
        auto& event = events[i];
        if (!event.action_context && event.type != EventType::TurnStarted && event.type != EventType::RoundStarted &&
            event.type != EventType::GuardExpired) {
            event.action_context = context;
            event.derived = context.parent_id != 0;
        }
    }
}

void Battle::close_action(const ActionContext& context, const std::vector<std::size_t>& guarded,
                          const std::vector<PendingRevive>& revives, std::vector<Event>& events) {
    std::optional<Event> terminal;
    if (phase_ == Phase::Finished && !events.empty() && events.back().type == EventType::BattleFinished) {
        terminal = events.back(); events.pop_back();
    }
    const auto begin = events.size();
    for (const auto index : guarded) {
        auto& target = fighters_[index];
        target.guarding = false;
        events.push_back(Event{EventType::GuardConsumed, target.definition.id, {}, 0, round_, {}});
    }
    // Hits and mandatory death consequences have finished. Never resume cancelled hits/reactions.
    for (const auto& pending : revives) {
        if (phase_ != Phase::Running) { break; }
        auto& fighter = fighters_[pending.fighter];
        if (fighter.health > 0) { continue; }
        if (!spend_consequence(context, events)) { break; }
        fighter.health = pending.health;
        fighter.ap = 0;
        fighter.guarding = false;
        events.push_back(Event{EventType::FighterRevived, fighter.definition.id, fighter.definition.id, fighter.health, round_, {}});
    }
    const bool enemy_alive = std::ranges::any_of(fighters_, [&context](const Fighter& candidate) {
        return candidate.health > 0 && candidate.definition.team != context.actor_side;
    });
    const bool own_alive = std::ranges::any_of(fighters_, [&context](const Fighter& candidate) {
        return candidate.health > 0 && candidate.definition.team == context.actor_side;
    });
    if (phase_ == Phase::Running && !enemy_alive) {
        finish(EndReason::Elimination, context.actor_side, context.actor, events);
    } else if (phase_ == Phase::Running && !own_alive) {
        finish(EndReason::Elimination, context.actor_side == Team::A ? Team::B : Team::A, context.actor, events);
    }
    if (terminal) { events.push_back(*terminal); }
    stamp_action(events, begin, context);
}

bool Battle::spend_consequence(const ActionContext& context, std::vector<Event>& events) {
    if (phase_ != Phase::Running) { return false; }
    if (last_consequence_events_ == rules_.max_consequence_events) {
        finish(EndReason::TechnicalLimit, std::nullopt, context.actor, events);
        stamp_action(events, events.size()-1, context);
        return false;
    }
    ++last_consequence_events_;
    return true;
}

std::optional<std::size_t> Battle::lowest_health_enemy(Team side) const noexcept {
    std::optional<std::size_t> selected;
    for (std::size_t i=0;i<fighters_.size();++i) {
        const auto& f=fighters_[i];
        if (f.health>0 && f.definition.team!=side && (!selected || f.health<fighters_[*selected].health)) {selected=i;}
    }
    return selected; // Equal HP uses canonical ordinal ID.
}

void Battle::apply_damage(std::size_t emitter, std::size_t target_index, numeric::Quantity power, EventType type,
                          const ActionContext& context, std::uint32_t hit_index, std::uint32_t planned_hits,
                          std::vector<Event>& events, std::vector<PendingReaction>& reactions,
                          std::vector<std::size_t>& deaths, std::vector<PendingRevive>& revives,std::optional<resolution::HitReport> report) {
    auto& target=fighters_[target_index];
    if (target.health==0 || phase_!=Phase::Running) {return;}
    const auto begin=events.size();
    const auto health_before=target.health;numeric::Quantity absorbed_total=0;
    // Guard has already mitigated the hit. Ordinary shield precedes the structural layer.
    for(const auto kind:{EffectKind::StandardShield,EffectKind::Shield,EffectKind::StructuralShield}) {
        auto shield=std::ranges::find(target.statuses,kind,&Status::kind);
        if(shield==target.statuses.end() || power==0){continue;}
        const numeric::Quantity absorbed=std::min(power,shield->magnitude);
        power-=absorbed;shield->magnitude-=absorbed;absorbed_total+=absorbed;
        events.push_back(Event{EventType::ShieldAbsorbed,target.definition.id,target.definition.id,absorbed,round_,{}});
        events.back().status_kind=kind;
        if(shield->magnitude==0){
            target.statuses.erase(shield);
            events.push_back(Event{EventType::ShieldBroken,target.definition.id,target.definition.id,0,round_,{}});
            events.back().status_kind=kind;
        }
    }
    auto prevention=std::ranges::find(target.statuses,EffectKind::PreventDeath,&Status::kind);
    const bool prevented=power>=target.health && prevention!=target.statuses.end();
    if (prevented && !spend_consequence(context,events)) {return;}
    const numeric::Quantity applied=prevented ? target.health-1 : std::min(power,target.health);
    target.health-=applied;
    events.push_back(Event{type,fighters_[emitter].definition.id,target.definition.id,applied,round_,{}});
    if(report){report->absorbed=absorbed_total;report->health_damage=applied;report->overkill=target.health==0?std::max<numeric::Quantity>(0,power-health_before):0;events.back().hit=report;}
    if (prevented) {
        --prevention->charges;
        if (prevention->charges==0) {target.statuses.erase(prevention);}
        events.push_back(Event{EventType::DeathPrevented,target.definition.id,target.definition.id,1,round_,{}});
    }
    if (target.health==0) {
        std::erase_if(target.statuses,[](const Status& status){return status.kind==EffectKind::StandardShield;});
        target.ap=0;
        events.push_back(Event{EventType::FighterDefeated,target.definition.id,{},0,round_,{}});
        deaths.push_back(target_index);
        auto revive=std::ranges::find(target.statuses,EffectKind::AutoRevive,&Status::kind);
        if (revive!=target.statuses.end()) {
            const numeric::Quantity health=std::min(revive->magnitude,target.definition.max_health);
            --revive->charges;
            if (revive->charges==0) {target.statuses.erase(revive);}
            revives.push_back({target_index,health});
            events.push_back(Event{EventType::ReviveScheduled,target.definition.id,target.definition.id,health,round_,{}});
        }
        std::erase_if(reactions,[target_index](const PendingReaction& pending) {
            return pending.from==target_index || pending.to==target_index;
        });
        std::erase_if(extras_,[&target](const ExtraTurnContext& extra) {return extra.beneficiary==target.definition.id;});
    }
    for(std::size_t i=begin;i<events.size();++i) {
        events[i].hit_index=hit_index;events[i].planned_hits=planned_hits;
        events[i].source_actor=context.actor;
    }
    stamp_action(events,begin,context);
}

void Battle::hit(std::size_t attacker, std::size_t target_index, bool derived, const std::string& source,
                 const ActionContext& context, std::uint32_t hit_index, std::uint32_t planned_hits,
                 std::vector<Event>& events, std::vector<PendingReaction>& reactions, std::vector<std::size_t>& guarded,
                 std::vector<PendingRevive>& revives,const EffectDefinition* attack_effect) {
    auto& target=fighters_[target_index];
    const auto& actor=fighters_[attacker];
    const auto begin=events.size();
    const numeric::Quantity power=actor.definition.attack;
    numeric::Quantity damage=target.guarding?power/2:power;
    std::optional<resolution::HitReport> report;
    bool can_react=true,used_guard=target.guarding;
    if(rules_.combat_model==resolution::CombatModel::FunnelV1){
        resolution::DamageInput input;input.base=power;input.ratio=attack_effect?attack_effect->power_ratio:10000;input.kind=attack_effect?attack_effect->damage_kind:resolution::DamageKind::Normal;
        input.attacker=actor.definition.stats;input.defender=target.definition.stats;input.guarding=target.guarding;
        const resolution::RollKey key{rules_.random_seed,context.id,hit_index,resolution::identity(target.definition.id),0};
        const auto calculated=resolution::calculate_damage(input,key);damage=calculated.amount;can_react=calculated.outcome.counts_for_hit_passive();
        used_guard=target.guarding && input.kind!=resolution::DamageKind::True && calculated.outcome.contact!=resolution::Contact::Invincible && calculated.outcome.contact!=resolution::Contact::Blocked;
        report=resolution::HitReport{calculated.outcome,input.kind,damage,0,0,0};
    }
    numeric::Quantity reflection=0;
    for(const auto& status:target.statuses) {if(status.kind==EffectKind::Reflect){reflection=status.magnitude;}}
    if(used_guard && std::ranges::find(guarded,target_index)==guarded.end()){guarded.push_back(target_index);}
    std::vector<std::size_t> pending_deaths;
    apply_damage(attacker,target_index,damage,derived?EventType::Reaction:EventType::Attack,
                 context,hit_index,planned_hits,events,reactions,pending_deaths,revives,report);
    for(std::size_t i=begin;i<events.size();++i){events[i].source_actor=source;}
    // Reflection still resolves if its carrier died in this hit. It precedes death explosions.
    if(phase_==Phase::Running && reflection>0 && (rules_.combat_model==resolution::CombatModel::Legacy || damage>0) && fighters_[attacker].health>0 && spend_consequence(context,events)) {
        apply_damage(target_index,attacker,reflection,EventType::ReflectedDamage,
                     context,hit_index,planned_hits,events,reactions,pending_deaths,revives);
    }
    for(std::size_t cursor=0;cursor<pending_deaths.size() && phase_==Phase::Running;++cursor) {
        if(!spend_consequence(context,events)){break;}
        const auto corpse=pending_deaths[cursor];
        numeric::Quantity blast=0;
        for(const auto& status:fighters_[corpse].statuses){if(status.kind==EffectKind::DeathExplosion){blast=status.magnitude;}}
        if(blast==0){continue;}
        const auto start=events.size();
        events.push_back(Event{EventType::DeathEffectTriggered,fighters_[corpse].definition.id,{},blast,round_,{}});
        events.back().hit_index=hit_index;events.back().planned_hits=planned_hits;
        events.back().source_actor=context.actor;
        stamp_action(events,start,context);
        // Canonical ID order for all live enemies; nested deaths append to the same FIFO.
        for(std::size_t victim=0;victim<fighters_.size() && phase_==Phase::Running;++victim) {
            if(fighters_[victim].health==0 || fighters_[victim].definition.team==fighters_[corpse].definition.team){continue;}
            if(!spend_consequence(context,events)){break;}
            apply_damage(corpse,victim,blast,EventType::DeathExplosion,
                         context,hit_index,planned_hits,events,reactions,pending_deaths,revives);
        }
    }
    // Consequence damage does not reflect or create counters. Only the original live pair can react.
    if(phase_==Phase::Running && can_react && target.health>0 && actor.health>0 && actor.definition.team!=target.definition.team &&
       !std::ranges::any_of(target.statuses,[](const Status& status){return status.kind==EffectKind::Block;}) &&
        std::ranges::any_of(target.statuses,[](const Status& status){return status.kind==EffectKind::Counter;})) {
        reactions.push_back({target_index,attacker,context.id});
    }
}
void Battle::end_turn(std::vector<Event>& events) {
    auto& actor = fighters_[active_];
    if (turn_kind_ == TurnKind::Natural) { ++natural_turns_; ++actor.natural_turns; }
    else { ++extra_turns_; ++actor.extra_turns; }
    const ExtraTurnSource* source = current_extra_ && current_extra_->source ? &*current_extra_->source : nullptr;
    // Inspect before expiration: the final frozen opportunity still cannot reduce cooldowns.
    const bool frozen = std::ranges::any_of(actor.statuses, [](const Status& status) {
        return status.kind == EffectKind::FreezeCooldowns;
    });
    for (auto& cooldown : actor.cooldowns) {
        const auto skill = std::ranges::find(rules_.skills, cooldown.skill, &SkillDefinition::id);
        // The use turn belongs to the cooldown cycle, including a newly-set cooldown.
        const bool advance = source ? source->advances_cooldowns : ticks(skill->cooldown_clock, turn_kind_);
        if (!frozen && advance && cooldown.remaining > 0) {
            --cooldown.remaining;
        }
    }
    for (auto& status : actor.statuses) {
        const bool advance = status.kind==EffectKind::StandardShield ? turn_kind_==TurnKind::Natural :
            (source ? source->advances_effect_durations : ticks(status.clock, turn_kind_));
        if (status.category==StatusCategory::Temporary && status.applied_turn != turn_id_ && advance) { --status.remaining; }
    }
    std::erase_if(actor.statuses, [&events, this, &actor](const Status& status) {
        if (status.category!=StatusCategory::Temporary || status.remaining > 0) { return false; }
        events.push_back(Event{EventType::EffectExpired, actor.definition.id, {}, static_cast<int>(status.kind), round_, {}});
        return true;
    });
    if (rules_.timing != TimingProfile::FixedRounds || !rules_.skills.empty()) {
        events.push_back(Event{EventType::TurnEnded, actor.definition.id, {}, 0, round_, {}});
    }
}

ActionResult Battle::apply(const Action& action) {
    const auto reject = [](ActionError error) { return ActionResult{error, {}}; };
    if (phase_ == Phase::Finished) { return reject(ActionError::BattleFinished); }
    const auto actor_index = index_of(action.actor);
    if (!actor_index) { return reject(ActionError::UnknownActor); }
    const auto& actor = fighters_[*actor_index];
    if (actor.health == 0) { return reject(ActionError::ActorDefeated); }
    if (*actor_index != active_) { return reject(ActionError::NotActorsTurn); }
    if (action.type != ActionType::Attack && action.type != ActionType::Guard &&
        action.type != ActionType::Skill && action.type != ActionType::Skip) { return reject(ActionError::UnknownAction); }
    if (current_actor_blocked() && action.type != ActionType::Skip) { return reject(ActionError::ActorBlocked); }
    if (action.type == ActionType::Skip && !current_actor_blocked()) { return reject(ActionError::NotBlocked); }
    if (action.type == ActionType::Guard && !rules_.guard_enabled) { return reject(ActionError::GuardDisabled); }
    if ((action.type == ActionType::Guard || action.type == ActionType::Skip) && !action.target.empty()) {
        return reject(ActionError::UnexpectedGuardTarget);
    }
    if (action.type != ActionType::Skill && !action.skill.empty()) { return reject(ActionError::UnknownSkill); }
    const SkillDefinition* skill = nullptr;
    if (action.type == ActionType::Skill) {
        const auto found = std::ranges::find(rules_.skills, action.skill, &SkillDefinition::id);
        if (found == rules_.skills.end() || found->owner != action.actor) { return reject(ActionError::UnknownSkill); }
        skill = &*found;
        const auto cooldown = std::ranges::find(actor.cooldowns, skill->id, &Cooldown::skill);
        if (cooldown->remaining > 0) { return reject(ActionError::SkillOnCooldown); }
    }
    std::optional<std::size_t> target_index;
    if (action.type == ActionType::Attack || skill) {
        target_index = index_of(action.target);
        if (!target_index) { return reject(ActionError::UnknownTarget); }
        const auto& target = fighters_[*target_index];
        if (target.health == 0) { return reject(ActionError::TargetDefeated); }
        if (!skill && target.definition.team == actor.definition.team) { return reject(ActionError::FriendlyTarget); }
        if (skill && ((skill->target == TargetRule::Enemy && target.definition.team == actor.definition.team) ||
            (skill->target == TargetRule::Ally && target.definition.team != actor.definition.team) ||
            (skill->target == TargetRule::Self && *target_index != *actor_index))) {
            return reject(ActionError::InvalidSkillTarget);
        }
    }

    // Only accepted primary commands (including blocked opportunity resolution) spend a budget unit.
    ActionResult result;
    const auto root_context = make_context(active_, action.type, action.target, action.skill);
    result.action_context = root_context;
    last_consequence_events_ = 0;
    const auto completed_turn = turn_id_;
    const auto completed_kind = turn_kind_;
    ++accepted_actions_;
    std::vector<PendingReaction> reactions;
    std::vector<std::size_t> guarded;
    std::vector<PendingRevive> revives;
    if (action.type == ActionType::Guard) {
        fighters_[active_].guarding = true;
        result.events.push_back(Event{EventType::GuardSet, action.actor, {}, 0, round_, {}});
    } else if (action.type == ActionType::Skip) {
        result.events.push_back(Event{EventType::TurnSkipped, action.actor, {}, 0, round_, {}});
    } else if (!skill) {
        hit(active_, *target_index, false, {}, root_context, 1, 1, result.events, reactions, guarded, revives);
    } else {
        auto& cooldowns = fighters_[active_].cooldowns;
        std::ranges::find(cooldowns, skill->id, &Cooldown::skill)->remaining = skill->cooldown;
        result.events.push_back(Event{EventType::SkillUsed, action.actor, skill->id, 0, round_, {}});
        std::uint32_t planned_hits = 0;
        for (const auto& effect : skill->effects) {
            if (effect.kind == EffectKind::Damage) { planned_hits += effect.hit_count; }
        }
        std::uint32_t hit_index = 0;
        for (const auto& effect : skill->effects) {
            if (phase_ == Phase::Finished || fighters_[active_].health == 0) { break; }
            if (fighters_[*target_index].health == 0 &&
                (effect.kind != EffectKind::Damage || effect.hit_selection == HitSelection::FixedTarget)) { break; }
            auto& target = fighters_[*target_index];
            switch (effect.kind) {
            case EffectKind::Damage:
                for (std::uint32_t i=0; i<effect.hit_count && phase_==Phase::Running && fighters_[active_].health>0; ++i) {
                    const auto chosen = effect.hit_selection == HitSelection::LowestHealthEnemy ?
                        lowest_health_enemy(actor.definition.team) :
                        (target.health>0 ? target_index : std::nullopt);
                    if (!chosen) { break; }
                    hit(active_, *chosen, false, {}, root_context, ++hit_index, planned_hits, result.events, reactions, guarded, revives,&effect);
                }
                break;
            case EffectKind::StandardShield: {
                // Integer division computes floor(1% max HP) without multiplication overflow.
                const numeric::Quantity points=std::max<numeric::Quantity>(1,actor.definition.max_health/100);
                auto old=std::ranges::find(target.statuses,EffectKind::StandardShield,&Status::kind);
                if(old==target.statuses.end()){
                    target.statuses.push_back({EffectKind::StandardShield,3,TickPolicy::NaturalOnly,turn_id_,points});
                }else{
                    old->magnitude=std::max(old->magnitude,points);
                    old->remaining=std::max(old->remaining,3);
                    old->applied_turn=turn_id_;
                }
                break;
            }
            case EffectKind::ChangeAP:
                target.ap = std::clamp(target.ap + effect.amount, std::int64_t{0}, bar_max); break;
            case EffectKind::SetSpeed: target.effective_speed = static_cast<int>(effect.amount); break; // Validated to 1..10000.
            case EffectKind::ExtraTurn: {
                auto proc = extra_proc_counts_.end();
                if (effect.extra_source && effect.extra_source->max_procs_per_natural_turn > 0) {
                    const auto& policy = *effect.extra_source;
                    proc = std::ranges::find_if(extra_proc_counts_, [&](const ExtraProcCount& known) {
                        return known.source_actor == action.actor && known.beneficiary == action.target && known.source_id == policy.id;
                    });
                    if (proc != extra_proc_counts_.end() && proc->count >= policy.max_procs_per_natural_turn) {
                        result.events.push_back(Event{EventType::ExtraTurnSuppressed, action.actor, action.target, 0, round_, {}});
                        continue; // Source cap is a game rule; it does not terminate the battle.
                    }
                }
                if (extras_.size() >= rules_.max_derived_actions) {
                    finish(EndReason::TechnicalLimit, std::nullopt, action.actor, result.events);
                } else {
                    extras_.push_back({action.target, action.actor, skill->id, turn_id_, originating_natural_turn_, effect.extra_source});
                    if (effect.extra_source && effect.extra_source->max_procs_per_natural_turn > 0) {
                        if (proc == extra_proc_counts_.end()) {
                            extra_proc_counts_.push_back({action.actor, action.target, effect.extra_source->id, 1});
                        } else { ++proc->count; }
                    }
                }
                break;
            }
            case EffectKind::Block: case EffectKind::Marker: case EffectKind::Counter: case EffectKind::FreezeCooldowns:
            case EffectKind::Reflect: case EffectKind::DeathExplosion: case EffectKind::PreventDeath: case EffectKind::AutoRevive:
            case EffectKind::Shield: case EffectKind::StructuralShield: {
                const auto old = std::ranges::find(target.statuses, effect.kind, &Status::kind);
                const Status status{effect.kind, effect.duration, effect.clock, turn_id_, effect.amount, effect.charges,effect.category};
                if (old == target.statuses.end()) { target.statuses.push_back(status); } else { *old = status; }
                break;
            }
            }
            if (effect.kind != EffectKind::Damage) {
                result.events.push_back(Event{EventType::EffectApplied, action.actor, target.definition.id,
                                              static_cast<int>(effect.kind), round_, {}});
            }
        }
    }
    close_action(root_context, guarded, revives, result.events);
    stamp_action(result.events, 0, root_context);
    std::size_t processed = 0;
    while (!reactions.empty() && phase_ == Phase::Running) {
        const auto [from, to, parent] = reactions.front();
        reactions.erase(reactions.begin());
        if (fighters_[from].health == 0 || fighters_[to].health == 0) { continue; }
        if (processed == rules_.max_derived_actions) {
            finish(EndReason::TechnicalLimit, std::nullopt, action.actor, result.events); break;
        }
        ++processed; ++derived_actions_;
        const auto context = make_context(from, ActionType::Attack, fighters_[to].definition.id, {}, parent, root_context.id);
        std::vector<std::size_t> reaction_guarded;
        std::vector<PendingRevive> reaction_revives;
        hit(from, to, true, action.actor, context, 1, 1, result.events, reactions, reaction_guarded, reaction_revives);
        close_action(context, reaction_guarded, reaction_revives, result.events);
    }
    std::optional<Event> terminal;
    if (phase_ == Phase::Finished) { terminal = result.events.back(); result.events.pop_back(); }
    end_turn(result.events);
    if (terminal) { result.events.push_back(*terminal); }
    stamp_action(result.events, 0, root_context);
    stamp(result.events, 0, completed_turn, completed_kind);
    if (phase_ == Phase::Finished) { return result; }
    if (remaining_actions() == std::optional<std::uint64_t>{0}) {
        std::optional<Team> winner;
        switch (rules_.action_budget->outcome) {
        case LimitOutcome::Draw: break;
        case LimitOutcome::DefeatA: winner = Team::B; break;
        case LimitOutcome::DefeatB: winner = Team::A; break;
        }
        finish(EndReason::ActionLimit, winner, action.actor, result.events);
        stamp_action(result.events, 0, root_context);
        stamp(result.events, 0, completed_turn, completed_kind);
        return result;
    }
    advance(result.events);
    stamp_action(result.events, 0, root_context);
    return result;
}
void Battle::finish(EndReason reason, std::optional<Team> winner, const std::string& actor, std::vector<Event>& events) {
    phase_ = Phase::Finished;
    winner_ = winner;
    end_reason_ = reason;
    events.push_back(Event{EventType::BattleFinished, actor, {}, 0, round_, winner_, reason});
}

void Battle::advance(std::vector<Event>& events) {
    const auto begin = events.size();
    std::erase_if(extras_, [this](const ExtraTurnContext& extra) { return find(extra.beneficiary)->health == 0; });
    if (!extras_.empty()) {
        if (extra_chain_ == rules_.max_derived_actions) {
            finish(EndReason::TechnicalLimit, std::nullopt, fighters_[active_].definition.id, events);
            stamp(events, begin, turn_id_, turn_kind_);
            return;
        }
        current_extra_ = extras_.front();
        active_ = *index_of(current_extra_->beneficiary);
        extras_.erase(extras_.begin());
        ++extra_chain_;
        turn_kind_ = TurnKind::Extra;
    } else {
        extra_chain_ = 0;
        turn_kind_ = TurnKind::Natural;
        current_extra_.reset();
        extra_proc_counts_.clear();
        if (rules_.timing == TimingProfile::ActionBar) { select_bar_actor(); }
        else {
            do {
                ++cursor_;
                if (cursor_ == order_.size()) {
                    cursor_ = 0;
                    ++round_;
                    events.push_back(Event{EventType::RoundStarted, {}, {}, 0, round_, {}});
                }
            } while (fighters_[order_[cursor_]].health == 0);
            active_ = order_[cursor_];
        }
    }
    ++turn_id_;
    if (turn_kind_ == TurnKind::Natural) { originating_natural_turn_ = turn_id_; }
    auto& next = fighters_[active_];
    if (next.guarding) {
        next.guarding = false;
        events.push_back(Event{EventType::GuardExpired, next.definition.id, {}, 0, round_, {}});
    }
    events.push_back(Event{EventType::TurnStarted, next.definition.id, {}, 0, round_, {}});
    stamp(events, begin, turn_id_, turn_kind_);
}

void Battle::select_bar_actor() {
    std::int64_t wait = bar_max;
    bool ready = false;
    for (const auto& fighter : fighters_) {
        if (fighter.health == 0) { continue; }
        if (fighter.ap >= bar_max) { ready = true; break; }
        const auto distance = bar_max - fighter.ap;
        const auto delta = (distance + fighter.effective_speed - 1) / fighter.effective_speed;
        wait = std::min(wait, delta);
    }
    if (!ready) {
        if (logical_time_ > std::numeric_limits<std::uint64_t>::max() - static_cast<std::uint64_t>(wait)) {
            throw std::overflow_error("Logical clock overflow");
        }
        logical_time_ += static_cast<std::uint64_t>(wait);
        for (auto& fighter : fighters_) {
            if (fighter.health > 0) { fighter.ap += fighter.effective_speed * wait; }
        }
    }
    std::optional<std::size_t> selected;
    for (std::size_t i = 0; i < fighters_.size(); ++i) {
        const auto& fighter = fighters_[i];
        if (fighter.health == 0 || fighter.ap < bar_max) { continue; }
        if (!selected || fighter.ap > fighters_[*selected].ap ||
            (fighter.ap == fighters_[*selected].ap && fighter.effective_speed > fighters_[*selected].effective_speed)) {
            selected = i;
        }
    }
    active_ = *selected; // Definitions are canonical by ordinal ID: final tie is stable.
    fighters_[active_].ap = 0;
}

std::vector<Definition> reference_roster() {
    return {{"A1", Team::A, 100, 20, 12}, {"A2", Team::A, 140, 15, 8},
            {"A3", Team::A, 80, 25, 16}, {"B1", Team::B, 100, 20, 12},
            {"B2", Team::B, 140, 15, 8}, {"B3", Team::B, 80, 25, 16}};
}

std::optional<Action> choose_basic_attack(const Battle& battle) {
    const auto* actor = battle.current_actor();
    if (!actor) { return std::nullopt; }
    if (battle.current_actor_blocked()) { return Action{actor->definition.id, ActionType::Skip, {}}; }
    const Fighter* selected = nullptr;
    for (const auto& candidate : battle.fighters()) {
        if (candidate.health == 0 || candidate.definition.team == actor->definition.team) { continue; }
        if (!selected || candidate.health < selected->health ||
            (candidate.health == selected->health && candidate.definition.id < selected->definition.id)) {
            selected = &candidate;
        }
    }
    if (!selected) { return std::nullopt; }
    return Action{actor->definition.id, ActionType::Attack, selected->definition.id};
}

} // namespace rpg
