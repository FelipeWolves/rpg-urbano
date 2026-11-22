#include "rpg/battle.hpp"
#include "rpg/status_operations.hpp"

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <limits>

namespace rpg {
namespace {
constexpr std::int64_t bar_max = 1000000;
bool quantity_effect(EffectKind kind){return kind==EffectKind::Reflect || kind==EffectKind::DeathExplosion || kind==EffectKind::AutoRevive || kind==EffectKind::Shield || kind==EffectKind::StructuralShield || kind==EffectKind::StandardShield || kind==EffectKind::SilverShield || kind==EffectKind::Heal;}
bool control_kind(EffectKind kind){return kind==EffectKind::Stun || kind==EffectKind::Freeze || kind==EffectKind::Petrify || kind==EffectKind::Silence || kind==EffectKind::Suppression || kind==EffectKind::Invincible || kind==EffectKind::DebuffImmunity;}
bool active_buff(const Status& s){return !s.native_passive && (s.kind==EffectKind::Shield || s.kind==EffectKind::StandardShield || s.kind==EffectKind::SilverShield || s.kind==EffectKind::Counter || s.kind==EffectKind::Reflect || s.kind==EffectKind::PreventDeath || s.kind==EffectKind::AutoRevive || s.kind==EffectKind::Invincible || s.kind==EffectKind::DebuffImmunity);}
bool valid_border(StatusBorder border){return border==StatusBorder::Common || border==StatusBorder::Silver || border==StatusBorder::Golden || border==StatusBorder::Structural;}
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

bool has_status(const Fighter& unit,EffectKind kind) noexcept {return std::ranges::any_of(unit.statuses,[kind](const Status& s){return s.kind==kind;});}
bool incapacitated(const Fighter& unit) noexcept {return has_status(unit,EffectKind::Block) || has_status(unit,EffectKind::Stun) || has_status(unit,EffectKind::Freeze) || has_status(unit,EffectKind::Petrify);}
bool native_suppressed(const Fighter& unit) noexcept {return has_status(unit,EffectKind::Suppression);}
bool status_native_enabled(const Fighter& unit,const Status& status) noexcept {return !status.native_passive || !native_suppressed(unit) || status.border==StatusBorder::Golden || status.border==StatusBorder::Structural;}
bool periodic_effect(EffectKind kind) noexcept {return kind==EffectKind::Bleed || kind==EffectKind::Poison;}
bool common_debuff(EffectKind kind) noexcept {return periodic_effect(kind) || kind==EffectKind::Block || kind==EffectKind::Stun || kind==EffectKind::Freeze || kind==EffectKind::Petrify || kind==EffectKind::Silence || kind==EffectKind::Suppression || kind==EffectKind::FreezeCooldowns;}
Battle::Battle(std::vector<Definition> definitions, BattleRules rules,
               std::vector<InitialHealth> initial_health,
               std::vector<InitialRuntime> initial_runtime) : rules_(std::move(rules)) {
    if ((rules_.timing != TimingProfile::FixedRounds && rules_.timing != TimingProfile::ActionBar) ||
        rules_.max_derived_actions == 0 || rules_.max_derived_actions > 1024 || rules_.skills.size() > 256 ||
        rules_.periodic_stack_limit>4096 || rules_.max_consequence_events == 0 || rules_.max_consequence_events > 4096 || rules_.numeric_model != 2 ||
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
    if(rules_.combat_model==resolution::CombatModel::FunnelV1){
        std::uint32_t a_slot=0,b_slot=0;
        for(auto& d:definitions){auto& next=d.team==Team::A?a_slot:b_slot;if(d.formation_slot==0){d.formation_slot=++next;}else{next=std::max(next,d.formation_slot);}}
    }
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
        if(fighters_.back().definition.vigil){fighters_.back().vigil_charges=fighters_.back().definition.vigil->initial_charges;}
    }
    for(auto& fighter:fighters_){
        auto& d=fighter.definition;
        if(d.role!=EntityRole::Fighter && d.role!=EntityRole::Boss && d.role!=EntityRole::Auxiliary && d.role!=EntityRole::Summon){throw std::invalid_argument("Invalid entity role");}
        resolution::chance_domain(d.ap_reduction_resistance);
        if(d.role==EntityRole::Boss){d.is_boss=true;for(auto kind:{EffectKind::Block,EffectKind::Stun,EffectKind::Freeze,EffectKind::Petrify}){if(std::ranges::find(d.immune_effects,kind)==d.immune_effects.end()){d.immune_effects.push_back(kind);}}}
        if(d.role==EntityRole::Summon){const auto owner=index_of(d.summoner_id);if(!owner || *owner==static_cast<std::size_t>(&fighter-fighters_.data()) || fighters_[*owner].definition.team!=d.team || fighters_[*owner].definition.role==EntityRole::Summon){throw std::invalid_argument("Summon requires a non-summon same-team owner");}}
        else if(!d.summoner_id.empty()){throw std::invalid_argument("Only summon declares owner");}
    }
    if(rules_.required_enemy){const auto enemy=index_of(*rules_.required_enemy);if(!enemy || fighters_[*enemy].definition.team!=Team::B || fighters_[*enemy].definition.role!=EntityRole::Boss){throw std::invalid_argument("Required enemy must be a B boss");}}
    std::vector<std::pair<std::string, ExtraTurnSource>> sources;
    for (std::size_t i = 0; i < rules_.skills.size(); ++i) {
        const auto& skill = rules_.skills[i];
        const auto owner = index_of(skill.owner);
        if (skill.id.empty() || !owner || skill.cooldown < 0 || skill.cooldown > 1000000 ||
            !valid_clock(skill.cooldown_clock) || skill.effects.empty() || skill.effects.size() > 16 ||
            (skill.target != TargetRule::Enemy && skill.target != TargetRule::Ally && skill.target != TargetRule::Self && skill.target != TargetRule::EnemyTeam)) {
            throw std::invalid_argument("Invalid skill");
        }
        for (std::size_t j = 0; j < i; ++j) {
            if (rules_.skills[j].id == skill.id) { throw std::invalid_argument("Duplicate skill ID"); }
        }
        std::uint32_t planned_hits = 0;
        for (const auto& effect : skill.effects) {
            if(effect.power_ratio<0 || effect.power_ratio>numeric::quantity_max ||
               (effect.damage_kind!=resolution::DamageKind::Normal && effect.damage_kind!=resolution::DamageKind::True && effect.damage_kind!=resolution::DamageKind::Dot && effect.damage_kind!=resolution::DamageKind::TrueDot && effect.damage_kind!=resolution::DamageKind::Consequence)){
                throw std::invalid_argument("Invalid damage effect metadata");
            }
            if(periodic_effect(effect.kind) && (effect.category!=StatusCategory::Temporary || effect.border!=StatusBorder::Common)){throw std::invalid_argument("Periodic effects require common temporary debuffs");}
            if(effect.operation_scope!=OperationScope::CommonOnly && effect.operation_scope!=OperationScope::Removable){throw std::invalid_argument("Invalid operation scope");}
            if((effect.kind==EffectKind::StealBuffs || effect.kind==EffectKind::TransferDebuffs) && skill.target!=TargetRule::Enemy){throw std::invalid_argument("Transfer/steal requires an enemy");}
            if(effect.kind==EffectKind::CleanseDebuffs && skill.target!=TargetRule::Ally && skill.target!=TargetRule::Self){throw std::invalid_argument("Cleanse requires ally target");}
            resolution::chance_domain(effect.proc_chance);if(!valid_border(effect.border)){throw std::invalid_argument("Invalid effect border");}
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
            if(effect.unbuffed_damage_bonus<0 || effect.unbuffed_damage_bonus>numeric::quantity_max){throw std::invalid_argument("Invalid buffless bonus");}
            switch (effect.kind) {
            case EffectKind::CleanseDebuffs:case EffectKind::StealBuffs:case EffectKind::TransferDebuffs:
            case EffectKind::DispelBuffs: if(effect.amount<1 || effect.amount>32){throw std::invalid_argument("Invalid dispel count");}break;
            case EffectKind::Heal:
                if(effect.amount<0 || (skill.target!=TargetRule::Ally && skill.target!=TargetRule::Self)){throw std::invalid_argument("Heal requires nonnegative explicit points and ally target");}break;
            case EffectKind::SilverShield:
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
            case EffectKind::Bleed: case EffectKind::Poison:
            case EffectKind::Stun: case EffectKind::Freeze: case EffectKind::Petrify: case EffectKind::Silence: case EffectKind::Suppression: case EffectKind::Invincible: case EffectKind::DebuffImmunity:
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
            if (effect.kind == EffectKind::Damage && skill.target != TargetRule::Enemy && skill.target != TargetRule::EnemyTeam) {
                throw std::invalid_argument("Damage requires enemy target in this profile");
            }
        }
        fighters_[*owner].cooldowns.push_back({skill.id, 0});
    }
        for(const auto& f:fighters_){if(f.definition.vigil){
        const auto& v=*f.definition.vigil;const auto skill=std::ranges::find(rules_.skills,v.counter_skill,&SkillDefinition::id);
        if(rules_.combat_model!=resolution::CombatModel::FunnelV1 || (v.max_party_size!=5 && v.max_party_size!=6) || v.initial_charges>v.max_party_size || !valid_border(v.charge_border) || skill==rules_.skills.end() || skill->owner!=f.definition.id || !skill->basic || skill->cooldown!=0 || skill->target!=TargetRule::Enemy){throw std::invalid_argument("Invalid Vigil passive");}
    }}
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
            alive_a = alive_a || (state.health > 0 && fighter.definition.team == Team::A && fighter.definition.role!=EntityRole::Summon);
            alive_b = alive_b || (state.health > 0 && fighter.definition.team == Team::B && fighter.definition.role!=EntityRole::Summon);
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
        if (state.statuses.size() > (rules_.combat_model==resolution::CombatModel::Legacy?10u:4096u)) { throw std::invalid_argument("Too many status types"); }
        auto statuses = state.statuses;
        for (std::size_t j = 0; j < statuses.size(); ++j) {
            auto& status = statuses[j];
            if ((status.kind != EffectKind::Block && status.kind != EffectKind::Marker && status.kind != EffectKind::Counter &&
                 status.kind != EffectKind::FreezeCooldowns && status.kind != EffectKind::Reflect &&
                 status.kind != EffectKind::DeathExplosion && status.kind != EffectKind::PreventDeath &&
                 status.kind != EffectKind::AutoRevive && status.kind != EffectKind::Shield &&
                 status.kind != EffectKind::StructuralShield && status.kind != EffectKind::StandardShield && status.kind!=EffectKind::SilverShield && !control_kind(status.kind) && !periodic_effect(status.kind)) ||
                !valid_duration(status.category,status.remaining) || !valid_clock(status.clock) || !valid_border(status.border)) {
                throw std::invalid_argument("Invalid status state");
            }
            if ((status.kind == EffectKind::Reflect || status.kind == EffectKind::DeathExplosion ||
                 status.kind == EffectKind::AutoRevive || status.kind==EffectKind::Shield ||
                 status.kind==EffectKind::StructuralShield || status.kind==EffectKind::StandardShield || status.kind==EffectKind::SilverShield) && status.magnitude <= 0) {
                throw std::invalid_argument("Invalid consequence magnitude");
            }
            if((status.kind==EffectKind::StandardShield || status.kind==EffectKind::SilverShield) && (fighter.health==0 || status.clock!=TickPolicy::NaturalOnly || status.category!=StatusCategory::Temporary)){throw std::invalid_argument("Invalid standard shield state");}
            if ((status.kind==EffectKind::Shield && status.category!=StatusCategory::Temporary) ||
                (status.kind==EffectKind::StructuralShield && status.category!=StatusCategory::Structural)) {
                throw std::invalid_argument("Invalid shield category");
            }
            if ((status.kind == EffectKind::PreventDeath || status.kind == EffectKind::AutoRevive) &&
                (status.charges == 0 || status.charges > 1024)) { throw std::invalid_argument("Invalid survival state"); }
            if(status.kind==EffectKind::SilverShield && status.border!=StatusBorder::Silver){throw std::invalid_argument("SilverShield requires silver border");}
            for (std::size_t k = 0; k < j; ++k) {
                if (statuses[k].kind == status.kind && !periodic_effect(status.kind)) { throw std::invalid_argument("Duplicate status type"); }
            }
            if(periodic_effect(status.kind)){
                status.instance_id=j+1;
                const auto source=index_of(status.source_id);
                if(source){if(status.source_team && *status.source_team!=fighters_[*source].definition.team){throw std::invalid_argument("Conflicting periodic source team");}status.source_team=fighters_[*source].definition.team;}
                if(status.source_id.empty() || (!source && !status.source_team) || (status.source_team && *status.source_team!=Team::A && *status.source_team!=Team::B) || status.magnitude<1 || status.magnitude>numeric::quantity_max || status.clock!=TickPolicy::EveryTurn || status.category!=StatusCategory::Temporary || status.border!=StatusBorder::Common){throw std::invalid_argument("Invalid periodic state");}
                const auto count=std::ranges::count(statuses,status.kind,&Status::kind);
                if(rules_.periodic_stack_limit && count>rules_.periodic_stack_limit){throw std::invalid_argument("Periodic stack cap exceeded");}
            }
            status.applied_turn = 0; // Entered from the preceding encounter, never a newly-applied local status.
        }
        fighter.effective_speed = state.effective_speed;
        fighter.cooldowns = std::move(cooldowns);
        fighter.statuses = std::move(statuses);
        if(state.vigil_charges){if(!fighter.definition.vigil || *state.vigil_charges>fighter.definition.vigil->max_party_size){throw std::invalid_argument("Invalid carried charges");}fighter.vigil_charges=*state.vigil_charges;}
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
    tick_periodics(initial_events_);
    if(phase_==Phase::Running && fighters_[active_].health==0){advance(initial_events_);}
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
    const auto* actor=current_actor();return actor && incapacitated(*actor);
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
    const auto living=[&](Team team){return std::ranges::any_of(fighters_,[&](const Fighter& f){return f.health>0 && f.definition.team==team && f.definition.role!=EntityRole::Summon;});};
    const bool a_alive=living(Team::A);
    const bool b_alive=rules_.required_enemy?find(*rules_.required_enemy)->health>0:living(Team::B);
    if(phase_==Phase::Running && (!a_alive || !b_alive)){
        const auto winner=!a_alive&&!b_alive?context.actor_side:!b_alive?Team::A:Team::B;
        finish(EndReason::Elimination,winner,context.actor,events);
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

void Battle::apply_damage(std::optional<std::size_t> emitter, std::size_t target_index, numeric::Quantity power, EventType type,
                          const ActionContext& context, std::uint32_t hit_index, std::uint32_t planned_hits,
                          std::vector<Event>& events, std::vector<PendingReaction>& reactions,
                          std::vector<std::size_t>& deaths, std::vector<PendingRevive>& revives,std::optional<resolution::HitReport> report) {
    auto& target=fighters_[target_index];
    if (target.health==0 || phase_!=Phase::Running) {return;}
    const auto begin=events.size();
    if(!report && rules_.combat_model==resolution::CombatModel::FunnelV1){
        resolution::DamageInput input;input.base=power;input.kind=resolution::DamageKind::Consequence;input.defender=target.definition.stats;input.defender.invincible=input.defender.invincible||has_status(target,EffectKind::Invincible);
        const auto damage=resolution::calculate_damage(input,{rules_.random_seed,context.id,hit_index,resolution::identity(target.definition.id),0});power=damage.amount;
        report=resolution::HitReport{damage.outcome,input.kind,power,0,0,0};
    }
    const auto health_before=target.health;numeric::Quantity absorbed_total=0;
    // Guard has already mitigated the hit. Ordinary shield precedes the structural layer.
    for(const auto kind:{EffectKind::StandardShield,EffectKind::SilverShield,EffectKind::Shield,EffectKind::StructuralShield}) {
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
    const bool prevented=power>=target.health && prevention!=target.statuses.end() && status_native_enabled(target,*prevention);
    if (prevented && !spend_consequence(context,events)) {return;}
    const numeric::Quantity applied=prevented ? target.health-1 : std::min(power,target.health);
    target.health-=applied;
    events.push_back(Event{type,emitter?fighters_[*emitter].definition.id:context.actor,target.definition.id,applied,round_,{}});
    if(report){report->absorbed=absorbed_total;report->health_damage=applied;report->overkill=target.health==0?std::max<numeric::Quantity>(0,power-health_before):0;events.back().hit=report;}
    if (prevented) {
        --prevention->charges;
        if (prevention->charges==0) {target.statuses.erase(prevention);}
        events.push_back(Event{EventType::DeathPrevented,target.definition.id,target.definition.id,1,round_,{}});
    }
    if (target.health==0) {
        std::erase_if(target.statuses,[](const Status& status){return status.kind==EffectKind::StandardShield || status.kind==EffectKind::SilverShield;});
        for(std::size_t i=0;i<fighters_.size();++i){auto& summon=fighters_[i];
            if(summon.health>0 && summon.definition.role==EntityRole::Summon && summon.definition.summoner_id==target.definition.id){
                summon.health=0;summon.ap=0;summon.guarding=false;
                events.push_back({EventType::SummonRemoved,target.definition.id,summon.definition.id,0,round_,{}});
                std::erase_if(reactions,[&](const PendingReaction& r){return r.from==i || r.to==i;});
                std::erase_if(extras_,[&](const ExtraTurnContext& e){return e.beneficiary==summon.definition.id;});
            }
        }
        target.ap=0;
        events.push_back(Event{EventType::FighterDefeated,target.definition.id,{},0,round_,{}});
        deaths.push_back(target_index);
        auto revive=std::ranges::find(target.statuses,EffectKind::AutoRevive,&Status::kind);
        if (revive!=target.statuses.end() && status_native_enabled(target,*revive)) {
            const numeric::Quantity health=std::min(revive->magnitude,target.definition.max_health);
            --revive->charges;
            if (revive->charges==0) {target.statuses.erase(revive);}
            revives.push_back({target_index,health});
            events.push_back(Event{EventType::ReviveScheduled,target.definition.id,target.definition.id,health,round_,{}});
        }
        std::erase_if(reactions,[target_index](const PendingReaction& pending) {
            return pending.from==target_index || (pending.to==target_index && !pending.vigil);
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
        input.attacker=actor.definition.stats;if(attack_effect && attack_effect->unbuffed_damage_bonus>0 && !std::ranges::any_of(target.statuses,active_buff)){input.attacker.damage_bonus=std::min<numeric::BasisPoints>(numeric::quantity_max,input.attacker.damage_bonus+attack_effect->unbuffed_damage_bonus);}input.defender=target.definition.stats;input.defender.invincible=input.defender.invincible || has_status(target,EffectKind::Invincible);input.guarding=target.guarding;
        const resolution::RollKey key{rules_.random_seed,context.id,hit_index,resolution::identity(target.definition.id),0};
        const auto calculated=resolution::calculate_damage(input,key);damage=calculated.amount;can_react=calculated.outcome.counts_for_hit_passive();
        used_guard=target.guarding && input.kind!=resolution::DamageKind::True && input.kind!=resolution::DamageKind::TrueDot && calculated.outcome.contact!=resolution::Contact::Invincible && calculated.outcome.contact!=resolution::Contact::Blocked;
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
    drain_deaths(pending_deaths,context,hit_index,planned_hits,events,reactions,revives);
    // Consequence damage does not reflect or create counters. Only the original live pair can react.
    if(phase_==Phase::Running && can_react && target.health>0 && actor.health>0 && actor.definition.team!=target.definition.team &&
       !std::ranges::any_of(target.statuses,[](const Status& status){return status.kind==EffectKind::Block;}) &&
        std::ranges::any_of(target.statuses,[&](const Status& status){return status.kind==EffectKind::Counter && status_native_enabled(target,status) && !incapacitated(target);})) {
        reactions.push_back({target_index,attacker,context.id});
    }
}
void Battle::drain_deaths(std::vector<std::size_t>& pending_deaths,const ActionContext& context,std::uint32_t hit_index,std::uint32_t planned_hits,std::vector<Event>& events,std::vector<PendingReaction>& reactions,std::vector<PendingRevive>& revives) {
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
        const bool advance = (status.kind==EffectKind::StandardShield || status.kind==EffectKind::SilverShield) ? turn_kind_==TurnKind::Natural :
            (source ? source->advances_effect_durations : ticks(status.clock, turn_kind_));
        if (!periodic_effect(status.kind) && status.category==StatusCategory::Temporary && status.applied_turn != turn_id_ && advance) { --status.remaining; }
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

void Battle::standard_shield(std::size_t source,std::size_t target_index,std::vector<Event>& events){
    auto& target=fighters_[target_index];if(target.health==0 || fighters_[source].health==0){return;}
    const auto points=std::max<numeric::Quantity>(1,fighters_[source].definition.max_health/100);
    auto old=std::ranges::find(target.statuses,EffectKind::StandardShield,&Status::kind);
    if(old==target.statuses.end()){target.statuses.push_back({EffectKind::StandardShield,3,TickPolicy::NaturalOnly,turn_id_,points});}
    else{old->magnitude=std::max(old->magnitude,points);old->remaining=std::max(old->remaining,3);old->applied_turn=turn_id_;}
    events.push_back({EventType::EffectApplied,fighters_[source].definition.id,target.definition.id,static_cast<int>(EffectKind::StandardShield),round_,{}});if(rules_.combat_model==resolution::CombatModel::FunnelV1){events.back().status_kind=EffectKind::StandardShield;}
}
void Battle::observe_vigil(const ActionContext& context,std::vector<Event>& events,std::vector<PendingReaction>& reactions){
    const auto aggressor=*index_of(context.actor);
    std::vector<std::size_t> touched;
    for(const auto& event:events){
        if((event.type!=EventType::Attack && event.type!=EventType::Reaction) || !event.hit || !event.action_context || event.action_context->id!=context.id || !event.hit->outcome.counts_for_hit_passive()){continue;}
        const auto target=index_of(event.target);
        if(target && std::ranges::find(touched,*target)==touched.end()){touched.push_back(*target);}
    }
    const auto begin=events.size();
    for(std::size_t index=0;index<fighters_.size();++index){
        auto& watcher=fighters_[index];
        if(!watcher.definition.vigil || watcher.health==0 || native_suppressed(watcher) || watcher.definition.team==context.actor_side){continue;}
        const auto& passive=*watcher.definition.vigil;
        const auto added=static_cast<std::uint32_t>(std::ranges::count_if(touched,[&](std::size_t target){return fighters_[target].definition.team==watcher.definition.team;}));
        if(added==0){continue;}
        watcher.vigil_charges=std::min(passive.max_party_size,watcher.vigil_charges+added);
        events.push_back({EventType::ChargeGained,watcher.definition.id,context.actor,watcher.vigil_charges,round_,{}});
        if(watcher.vigil_charges<passive.max_party_size){continue;}
        if(fighters_[aggressor].health==0){
            if(passive.charge_border==StatusBorder::Common || passive.charge_border==StatusBorder::Silver){watcher.vigil_charges=0;}
            events.push_back({EventType::PassiveCancelled,watcher.definition.id,context.actor,watcher.vigil_charges,round_,{}});continue;
        }
        reactions.push_back({index,aggressor,context.id,true,true});
    }
    stamp_action(events,begin,context);
}
void Battle::operate_statuses(std::size_t from,std::size_t to,const EffectDefinition& effect,const ActionContext& context,std::uint32_t ordinal,std::vector<Event>& events){
    auto& actor=fighters_[from];auto& target=fighters_[to];
    const bool hostile=actor.definition.team!=target.definition.team;
    const bool immunity=effect.kind==EffectKind::TransferDebuffs && !effect.bypass_immunity && (target.definition.stats.debuff_immunity || has_status(target,EffectKind::DebuffImmunity));
    if(hostile && !resolution::debuff_applies({},false,immunity,effect.irresistible,effect.proc_chance,actor.definition.stats.accuracy,target.definition.stats.resistance,{rules_.random_seed,context.id,0,resolution::identity(target.definition.id),ordinal})){
        events.push_back({EventType::EffectResisted,actor.definition.id,target.definition.id,0,round_,{}});return;
    }
    const bool transfer=effect.kind==EffectKind::TransferDebuffs,steal=effect.kind==EffectKind::StealBuffs;
    auto& source=transfer?actor:target;auto& receiver=transfer?target:actor;
    const auto op=transfer?StatusOperation::Transfer:steal?StatusOperation::Steal:StatusOperation::Remove;
    numeric::Quantity changed=0;
    for(std::size_t i=0;i<source.statuses.size() && changed<effect.amount;){
        const auto state=source.statuses[i];
        const bool match=(transfer || effect.kind==EffectKind::CleanseDebuffs)?status_is_debuff(state):status_is_buff(state);
        if((transfer && !effect.bypass_immunity && std::ranges::find(target.definition.immune_effects,state.kind)!=target.definition.immune_effects.end()) || !match || !permits_operation(state,op,effect.operation_scope)){++i;continue;}
        if(transfer || steal){
            if(periodic_effect(state.kind) && rules_.periodic_stack_limit && std::ranges::count(receiver.statuses,state.kind,&Status::kind)>=rules_.periodic_stack_limit){++i;continue;}
            if(receiver.statuses.size()>=4096){finish(EndReason::TechnicalLimit,std::nullopt,actor.definition.id,events);return;}
            auto incoming=state;incoming.applied_turn=turn_id_;
            incoming.instance_id=0;for(const auto& s:receiver.statuses){incoming.instance_id=std::max(incoming.instance_id,s.instance_id);}++incoming.instance_id;
            auto old=std::ranges::find(receiver.statuses,state.kind,&Status::kind);
            if(periodic_effect(state.kind) || old==receiver.statuses.end()){receiver.statuses.push_back(incoming);}
            else if(state.kind==EffectKind::StandardShield){old->magnitude=std::max(old->magnitude,incoming.magnitude);old->remaining=std::max(old->remaining,incoming.remaining);old->applied_turn=turn_id_;}
            else{*old=incoming;}
        }
        source.statuses.erase(source.statuses.begin()+static_cast<std::ptrdiff_t>(i));++changed;
    }
    events.push_back({EventType::EffectApplied,actor.definition.id,target.definition.id,changed,round_,{}});events.back().status_kind=effect.kind;
}
void Battle::execute_skill(std::size_t from,std::size_t target_index,const SkillDefinition& definition,const ActionContext& context,std::vector<Event>& events,std::vector<PendingReaction>& reactions,std::vector<std::size_t>& guarded,std::vector<PendingRevive>& revives,bool derived) {
    const auto* skill=&definition;const auto& actor=fighters_[from];
    const auto primary_target=target_index;
    const bool target_had_debuff=std::ranges::any_of(fighters_[target_index].statuses,[](const Status& s){return common_debuff(s.kind) || s.debuff;});
        std::uint32_t planned_hits = 0;
        for (const auto& effect : skill->effects) {
            if (effect.kind == EffectKind::Damage) { planned_hits += effect.hit_count; }
        }
        if(skill->target==TargetRule::EnemyTeam){planned_hits*=static_cast<std::uint32_t>(std::ranges::count_if(fighters_,[&](const Fighter& f){return f.health>0 && f.definition.team!=actor.definition.team;}));}
        std::uint32_t hit_index = 0,effect_ordinal=0;
        const bool has_attack=std::ranges::any_of(skill->effects,[](const EffectDefinition& e){return e.kind==EffectKind::Damage;});
        for (const auto& effect : skill->effects) {
            ++effect_ordinal;
            target_index=effect.self_target?from:primary_target;
            if(effect.requires_target_debuff && !target_had_debuff){continue;}
            if(effect.requires_connected_hit){bool connected=false;for(const auto& e:events){if(e.action_context && e.action_context->id==context.id && e.target==fighters_[primary_target].definition.id && e.hit && e.hit->outcome.contact!=resolution::Contact::Miss && e.hit->outcome.contact!=resolution::Contact::Blocked){connected=true;}}if(!connected){continue;}}
            if (phase_ == Phase::Finished || fighters_[from].health == 0) { break; }
            if (fighters_[target_index].health == 0 &&
                (effect.kind != EffectKind::Damage || effect.hit_selection == HitSelection::FixedTarget)) { if(rules_.combat_model==resolution::CombatModel::FunnelV1){continue;}break; }
            auto& target = fighters_[target_index];
            if(rules_.combat_model==resolution::CombatModel::FunnelV1 && target.definition.team!=actor.definition.team && (common_debuff(effect.kind) || effect.debuff)){
                resolution::HitOutcome outcome;bool found=!has_attack;
                for(auto it=events.rbegin();it!=events.rend();++it){if(it->target==target.definition.id && it->hit && it->action_context && it->action_context->id==context.id){outcome=it->hit->outcome;found=true;break;}}
                const bool immunity=(!effect.bypass_immunity) && (target.definition.stats.debuff_immunity || has_status(target,EffectKind::DebuffImmunity) || std::ranges::find(target.definition.immune_effects,effect.kind)!=target.definition.immune_effects.end());
                const resolution::RollKey key{rules_.random_seed,context.id,hit_index,resolution::identity(target.definition.id),effect_ordinal};
                if(!found || !resolution::debuff_applies(outcome,has_attack,immunity,effect.irresistible,effect.proc_chance,actor.definition.stats.accuracy,target.definition.stats.resistance,key)){
                    events.push_back({EventType::EffectResisted,actor.definition.id,target.definition.id,0,round_,{}});events.back().status_kind=effect.kind;continue;
                }
            }
            if(effect.unbuffed_damage_bonus<0 || effect.unbuffed_damage_bonus>numeric::quantity_max){throw std::invalid_argument("Invalid buffless bonus");}
            switch (effect.kind) {
            case EffectKind::CleanseDebuffs:case EffectKind::StealBuffs:case EffectKind::TransferDebuffs:
            case EffectKind::DispelBuffs: {
                if(skill->target==TargetRule::EnemyTeam){for(std::size_t i=0;i<fighters_.size();++i){if(fighters_[i].health>0 && fighters_[i].definition.team!=actor.definition.team){operate_statuses(from,i,effect,context,effect_ordinal,events);}}}
                else{operate_statuses(from,target_index,effect,context,effect_ordinal,events);}break;
            }
            case EffectKind::Damage:
                for(std::uint32_t i=0;i<effect.hit_count && phase_==Phase::Running && fighters_[from].health>0;++i){
                    if(skill->target==TargetRule::EnemyTeam){
                        for(std::size_t selected=0;selected<fighters_.size() && phase_==Phase::Running && fighters_[from].health>0;++selected){
                            if(fighters_[selected].health>0 && fighters_[selected].definition.team!=actor.definition.team){
                                hit(from,selected,derived,context.actor,context,++hit_index,planned_hits,events,reactions,guarded,revives,&effect);
                            }
                        }
                    }else{
                        const auto chosen=effect.hit_selection==HitSelection::LowestHealthEnemy?lowest_health_enemy(actor.definition.team):(target.health>0?std::optional<std::size_t>{target_index}:std::nullopt);
                        if(!chosen){break;}
                        hit(from,*chosen,derived,context.actor,context,++hit_index,planned_hits,events,reactions,guarded,revives,&effect);
                    }
                }
                break;
            case EffectKind::Heal: {const auto healed=std::min(effect.amount,target.definition.max_health-target.health);target.health+=healed;events.push_back({EventType::EffectApplied,actor.definition.id,target.definition.id,healed,round_,{}});events.back().status_kind=EffectKind::Heal;break;}
            case EffectKind::SilverShield: {
                const auto points=std::max<numeric::Quantity>(1,numeric::mul_div_floor(actor.definition.max_health,1500,10000));
                auto old=std::ranges::find(target.statuses,EffectKind::SilverShield,&Status::kind);
                if(old==target.statuses.end()){Status s{EffectKind::SilverShield,2,TickPolicy::NaturalOnly,turn_id_,points};s.border=StatusBorder::Silver;target.statuses.push_back(s);}
                else{old->magnitude=std::max(old->magnitude,points);old->remaining=std::max(old->remaining,2);old->applied_turn=turn_id_;}break;
            }
            case EffectKind::StandardShield: standard_shield(from,target_index,events);break;
            case EffectKind::ChangeAP:
                target.ap = std::clamp(target.ap + (effect.amount<0?-numeric::mul_div_floor(-effect.amount,10000-target.definition.ap_reduction_resistance,10000):effect.amount), std::int64_t{0}, bar_max); break;
            case EffectKind::SetSpeed: target.effective_speed = static_cast<int>(effect.amount); break; // Validated to 1..10000.
            case EffectKind::ExtraTurn: {
                auto proc = extra_proc_counts_.end();
                if (effect.extra_source && effect.extra_source->max_procs_per_natural_turn > 0) {
                    const auto& policy = *effect.extra_source;
                    proc = std::ranges::find_if(extra_proc_counts_, [&](const ExtraProcCount& known) {
                        return known.source_actor == actor.definition.id && known.beneficiary == fighters_[target_index].definition.id && known.source_id == policy.id;
                    });
                    if (proc != extra_proc_counts_.end() && proc->count >= policy.max_procs_per_natural_turn) {
                        events.push_back(Event{EventType::ExtraTurnSuppressed, actor.definition.id, fighters_[target_index].definition.id, 0, round_, {}});
                        continue; // Source cap is a game rule; it does not terminate the battle.
                    }
                }
                if (extras_.size() >= rules_.max_derived_actions) {
                    finish(EndReason::TechnicalLimit, std::nullopt, actor.definition.id, events);
                } else {
                    extras_.push_back({fighters_[target_index].definition.id, actor.definition.id, skill->id, turn_id_, originating_natural_turn_, effect.extra_source});
                    if (effect.extra_source && effect.extra_source->max_procs_per_natural_turn > 0) {
                        if (proc == extra_proc_counts_.end()) {
                            extra_proc_counts_.push_back({actor.definition.id, fighters_[target_index].definition.id, effect.extra_source->id, 1});
                        } else { ++proc->count; }
                    }
                }
                break;
            }
            case EffectKind::Bleed: case EffectKind::Poison: {
                const auto count=std::ranges::count(target.statuses,effect.kind,&Status::kind);
                if(rules_.periodic_stack_limit && count>=rules_.periodic_stack_limit){events.push_back({EventType::StackLimitReached,actor.definition.id,target.definition.id,0,round_,{}});continue;}
                if(target.statuses.size()>=4096){finish(EndReason::TechnicalLimit,std::nullopt,actor.definition.id,events);break;}
                Status status{effect.kind,effect.duration,TickPolicy::EveryTurn,turn_id_,effect.kind==EffectKind::Bleed?std::max<numeric::Quantity>(1,numeric::mul_div_floor(actor.definition.attack,5000,10000)):actor.definition.attack};
                for(const auto& old:target.statuses){status.instance_id=std::max(status.instance_id,old.instance_id);}++status.instance_id;
                status.source_id=actor.definition.id;status.source_team=actor.definition.team;target.statuses.push_back(std::move(status));break;
            }
            case EffectKind::Stun: case EffectKind::Freeze: case EffectKind::Petrify: case EffectKind::Silence: case EffectKind::Suppression: case EffectKind::Invincible: case EffectKind::DebuffImmunity:
            case EffectKind::Block: case EffectKind::Marker: case EffectKind::Counter: case EffectKind::FreezeCooldowns:
            case EffectKind::Reflect: case EffectKind::DeathExplosion: case EffectKind::PreventDeath: case EffectKind::AutoRevive:
            case EffectKind::Shield: case EffectKind::StructuralShield: {
                const auto old = std::ranges::find(target.statuses, effect.kind, &Status::kind);
                Status status{effect.kind, effect.duration, effect.clock, turn_id_, effect.amount, effect.charges,effect.category};status.border=effect.border;status.debuff=effect.debuff;
                if (old == target.statuses.end()) { target.statuses.push_back(status); } else { *old = status; }
                break;
            }
            }
            if (effect.kind != EffectKind::Damage && effect.kind != EffectKind::StandardShield && effect.kind!=EffectKind::Heal) {
                events.push_back(Event{EventType::EffectApplied, actor.definition.id, target.definition.id,
                                              static_cast<int>(effect.kind), round_, {}});
            }
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
        if (skill && (((skill->target == TargetRule::Enemy || skill->target==TargetRule::EnemyTeam) && target.definition.team == actor.definition.team) ||
            (skill->target == TargetRule::Ally && target.definition.team != actor.definition.team) ||
            (skill->target == TargetRule::Self && *target_index != *actor_index))) {
            return reject(ActionError::InvalidSkillTarget);
        }
    }

    // Only accepted primary commands (including blocked opportunity resolution) spend a budget unit.
    ActionResult result;
    if(skill && !skill->basic && has_status(actor,EffectKind::Silence)){return reject(ActionError::ActorSilenced);}
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
        execute_skill(active_,*target_index,*skill,root_context,result.events,reactions,guarded,revives,false);
    }
    observe_vigil(root_context,result.events,reactions);
    close_action(root_context, guarded, revives, result.events);
    stamp_action(result.events, 0, root_context);
    std::size_t processed = 0;
    while (!reactions.empty() && phase_ == Phase::Running) {
        const auto reaction=reactions.front();const auto from=reaction.from,to=reaction.to;const auto parent=reaction.parent_id;
        reactions.erase(reactions.begin());
        if(reaction.vigil && fighters_[to].health==0){
            const auto border=fighters_[from].definition.vigil->charge_border;
            if(border==StatusBorder::Common || border==StatusBorder::Silver){fighters_[from].vigil_charges=0;}
            result.events.push_back({EventType::PassiveCancelled,fighters_[from].definition.id,fighters_[to].definition.id,fighters_[from].vigil_charges,round_,{}});continue;
        }
        if (fighters_[from].health == 0 || fighters_[to].health == 0 || incapacitated(fighters_[from]) || (reaction.native && native_suppressed(fighters_[from]))) { continue; }
        if (processed == rules_.max_derived_actions) {
            finish(EndReason::TechnicalLimit, std::nullopt, action.actor, result.events); break;
        }
        ++processed; ++derived_actions_;
        const auto reaction_skill=reaction.vigil?fighters_[from].definition.vigil->counter_skill:std::string{};
        const auto context = make_context(from,reaction.vigil?ActionType::Skill:ActionType::Attack,fighters_[to].definition.id,reaction_skill,parent,root_context.id);
        const auto begin=result.events.size();
        std::vector<std::size_t> reaction_guarded;
        std::vector<PendingRevive> reaction_revives;
        if(reaction.vigil){
            fighters_[from].vigil_charges=0;
            result.events.push_back({EventType::PassiveTriggered,fighters_[from].definition.id,fighters_[to].definition.id,0,round_,{}});
            const auto definition=std::ranges::find(rules_.skills,reaction_skill,&SkillDefinition::id);
            result.events.push_back({EventType::SkillUsed,fighters_[from].definition.id,reaction_skill,0,round_,{}});
            execute_skill(from,to,*definition,context,result.events,reactions,reaction_guarded,reaction_revives,true);
        }else{hit(from,to,true,action.actor,context,1,1,result.events,reactions,reaction_guarded,reaction_revives);}
        if(reaction.vigil && fighters_[from].health>0 && phase_==Phase::Running){
            std::optional<std::size_t> selected;
            for(std::size_t i=0;i<fighters_.size();++i){
                const auto& candidate=fighters_[i];if(candidate.health==0 || candidate.definition.team!=fighters_[from].definition.team){continue;}
                int comparison=selected?numeric::compare_ratio(candidate.health,candidate.definition.max_health,fighters_[*selected].health,fighters_[*selected].definition.max_health):-1;
                if(!selected || comparison<0 || (comparison==0 && (i==from || (*selected!=from && candidate.definition.formation_slot<fighters_[*selected].definition.formation_slot)))){selected=i;}
            }
            if(selected){standard_shield(from,*selected,result.events);}
        }
        observe_vigil(context,result.events,reactions);
        close_action(context,reaction_guarded,reaction_revives,result.events);
        stamp_action(result.events,begin,context);
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

void Battle::tick_periodics(std::vector<Event>& events) {
    if(phase_!=Phase::Running || fighters_[active_].health==0){return;}
    auto& owner=fighters_[active_];
    const auto pending=owner.statuses;
    for(const auto& status:pending){
        if(phase_!=Phase::Running || owner.health==0){break;}
        if(!periodic_effect(status.kind)){continue;}
        const auto found=std::ranges::find(owner.statuses,status.instance_id,&Status::instance_id);
        if(found==owner.statuses.end()){continue;}
        const auto source=index_of(status.source_id);
        if(--found->remaining==0){owner.statuses.erase(found);}
        const auto context=source?make_context(*source,ActionType::Skill,owner.definition.id,status.kind==EffectKind::Bleed?"Bleed-tick":"Poison-tick"):ActionContext{next_action_id_++,0,next_action_id_-1,status.source_id,*status.source_team,ActionType::Skill,owner.definition.id,status.kind==EffectKind::Bleed?"Bleed-tick":"Poison-tick"};
        last_consequence_events_=0;
        auto points=status.kind==EffectKind::Bleed?status.magnitude:std::max<numeric::Quantity>(1,numeric::mul_div_floor(owner.definition.max_health,400,10000));
        if(status.kind==EffectKind::Poison && owner.definition.is_boss){points=std::min(points,status.magnitude);}
        resolution::DamageInput input;input.base=points;input.kind=resolution::DamageKind::TrueDot;input.defender=owner.definition.stats;input.defender.invincible=input.defender.invincible || has_status(owner,EffectKind::Invincible);
        const auto damage=resolution::calculate_damage(input,{rules_.random_seed,context.id,1,resolution::identity(owner.definition.id),0});
        std::vector<PendingReaction> reactions;std::vector<PendingRevive> revives;std::vector<std::size_t> deaths;
        const auto begin=events.size();
        apply_damage(source,active_,damage.amount,EventType::PeriodicDamage,context,1,1,events,reactions,deaths,revives,resolution::HitReport{damage.outcome,input.kind,damage.amount,0,0,0});
        for(std::size_t e=begin;e<events.size();++e){if(events[e].type==EventType::PeriodicDamage){events[e].status_kind=status.kind;}}
        if(status.remaining==1){events.push_back({EventType::EffectExpired,owner.definition.id,owner.definition.id,static_cast<int>(status.kind),round_,{}});events.back().status_kind=status.kind;stamp_action(events,events.size()-1,context);}
        drain_deaths(deaths,context,1,1,events,reactions,revives);
        close_action(context,{},revives,events);
        stamp(events,begin,turn_id_,turn_kind_);
    }
}
void Battle::advance(std::vector<Event>& events) {
    do {
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
    tick_periodics(events);
    } while(phase_==Phase::Running && fighters_[active_].health==0);
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
