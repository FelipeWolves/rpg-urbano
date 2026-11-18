#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <utility>

namespace rpg {

enum class Team { A, B };
enum class Phase { Running, Finished };
enum class ActionType { Attack, Guard, Skill, Skip };
enum class EndReason { Elimination, ActionLimit, TechnicalLimit };
enum class LimitOutcome { Draw, DefeatA, DefeatB };
enum class TimingProfile { FixedRounds, ActionBar };
enum class TurnKind { Natural, Extra };
enum class TickPolicy { NaturalOnly, EveryTurn };
enum class TargetRule { Enemy, Ally, Self };
enum class EffectKind { Damage, ChangeAP, SetSpeed, ExtraTurn, Block, Marker, Counter, FreezeCooldowns,
                        Reflect, DeathExplosion, PreventDeath, AutoRevive };
enum class ExtraOrigin { Ability, Equipment, Custom };
enum class HitSelection { FixedTarget, LowestHealthEnemy };

struct ExtraTurnSource {
    std::string id;
    ExtraOrigin origin;
    bool advances_cooldowns;
    bool advances_effect_durations;
    std::uint32_t max_procs_per_natural_turn;
    ExtraTurnSource(std::string source_id, ExtraOrigin kind, bool cooldowns, bool durations,
                    std::uint32_t maximum = 0)
        : id(std::move(source_id)), origin(kind), advances_cooldowns(cooldowns),
          advances_effect_durations(durations), max_procs_per_natural_turn(maximum) {}
    bool operator==(const ExtraTurnSource&) const = default;
};

struct ExtraTurnContext {
    std::string beneficiary;
    std::string source_actor;
    std::string granting_skill;
    std::uint64_t granting_turn;
    std::uint64_t originating_natural_turn;
    std::optional<ExtraTurnSource> source;
    bool operator==(const ExtraTurnContext&) const = default;
};

struct ExtraProcCount {
    std::string source_actor;
    std::string beneficiary;
    std::string source_id;
    std::uint32_t count;
    bool operator==(const ExtraProcCount&) const = default;
};

struct EffectDefinition {
    EffectKind kind;
    int amount = 0;
    int duration = 0;
    TickPolicy clock = TickPolicy::NaturalOnly;
    std::optional<ExtraTurnSource> extra_source;
    std::uint32_t hit_count = 1;
    HitSelection hit_selection = HitSelection::FixedTarget;
    std::uint32_t charges = 1;
    bool operator==(const EffectDefinition&) const = default;
};
struct SkillDefinition {
    std::string id;
    std::string owner;
    int cooldown;
    TargetRule target;
    std::vector<EffectDefinition> effects;
    TickPolicy cooldown_clock = TickPolicy::NaturalOnly;
    bool operator==(const SkillDefinition&) const = default;
};

struct ActionBudget {
    std::int64_t max_actions;
    LimitOutcome outcome;
    // Configuring a budget requires explicitly choosing its outcome.
    ActionBudget(std::int64_t maximum, LimitOutcome result) : max_actions(maximum), outcome(result) {}
    bool operator==(const ActionBudget&) const = default;
};

struct BattleRules {
    std::optional<ActionBudget> action_budget;
    TimingProfile timing = TimingProfile::FixedRounds;
    bool guard_enabled = true;
    std::vector<SkillDefinition> skills;
    std::uint32_t max_derived_actions = 64;
    std::uint32_t max_consequence_events = 256;
    bool operator==(const BattleRules&) const = default;
};
enum class ActionError {
    None, BattleFinished, UnknownActor, ActorDefeated, NotActorsTurn,
    UnknownAction, UnexpectedGuardTarget, UnknownTarget, FriendlyTarget, TargetDefeated,
    GuardDisabled, UnknownSkill, SkillOnCooldown, InvalidSkillTarget, ActorBlocked, NotBlocked
};
enum class EventType {
    Attack, GuardSet, GuardConsumed, FighterDefeated,
    RoundStarted, TurnStarted, GuardExpired, BattleFinished,
    SkillUsed, EffectApplied, EffectExpired, Reaction, TurnSkipped, TurnEnded, ExtraTurnSuppressed,
    ReflectedDamage, DeathExplosion, DeathEffectTriggered, DeathPrevented, ReviveScheduled, FighterRevived
};

struct Definition {
    std::string id;
    Team team;
    int max_health;
    int attack;
    int speed;
    bool operator==(const Definition&) const = default;
};

struct Status {
    EffectKind kind;
    int remaining;
    TickPolicy clock;
    std::uint64_t applied_turn;
    int magnitude = 0;
    std::uint32_t charges = 1;
    bool operator==(const Status&) const = default;
};

struct Cooldown {
    std::string skill;
    int remaining;
    bool operator==(const Cooldown&) const = default;
};

struct Fighter {
    Definition definition;
    int health;
    bool guarding = false;
    std::int64_t ap = 0;
    int effective_speed = 0;
    std::uint64_t natural_turns = 0;
    std::uint64_t extra_turns = 0;
    std::vector<Status> statuses;
    std::vector<Cooldown> cooldowns;
    bool operator==(const Fighter&) const = default;
};

struct InitialHealth {
    std::string id;
    int health;
};

struct InitialRuntime {
    std::string id;
    int effective_speed;
    std::vector<Status> statuses;
    std::vector<Cooldown> cooldowns;
};

struct Action {
    std::string actor;
    ActionType type;
    std::string target;
    std::string skill;
};

struct ActionContext {
    std::uint64_t id;
    std::uint64_t parent_id;
    std::uint64_t root_id;
    std::string actor;
    Team actor_side;
    ActionType type;
    std::string target;
    std::string skill;
    bool operator==(const ActionContext&) const = default;
};

struct Event {
    EventType type;
    std::string actor;
    std::string target;
    int amount = 0;
    std::uint64_t round = 0;
    std::optional<Team> winner;
    std::optional<EndReason> end_reason;
    std::uint64_t turn_id = 0;
    TurnKind turn_kind = TurnKind::Natural;
    bool derived = false;
    std::string source_actor;
    std::optional<ExtraTurnContext> extra_context;
    std::optional<ActionContext> action_context;
    std::uint32_t hit_index = 0;
    std::uint32_t planned_hits = 0;
    bool operator==(const Event&) const = default;
};

struct ActionResult {
    ActionError error = ActionError::None;
    std::vector<Event> events;
    std::optional<ActionContext> action_context;
    [[nodiscard]] bool accepted() const noexcept { return error == ActionError::None; }
    bool operator==(const ActionResult&) const = default;
};

struct Snapshot {
    std::vector<Fighter> fighters;
    std::vector<std::string> living_order;
    std::size_t cursor;
    std::uint64_t round;
    Phase phase;
    std::optional<Team> winner;
    std::optional<std::string> current_actor;
    std::uint64_t accepted_actions;
    BattleRules rules;
    std::optional<EndReason> end_reason;
    std::uint64_t logical_time = 0;
    std::uint64_t turn_id = 1;
    TurnKind turn_kind = TurnKind::Natural;
    std::uint64_t natural_turns = 0;
    std::uint64_t extra_turns = 0;
    std::uint64_t derived_actions = 0;
    std::vector<std::string> pending_extras;
    std::uint32_t extra_chain = 0;
    std::optional<ExtraTurnContext> current_extra;
    std::vector<ExtraTurnContext> pending_extra_contexts;
    std::uint64_t originating_natural_turn = 1;
    std::vector<ExtraProcCount> extra_proc_counts;
    std::uint64_t next_action_id = 1;
    std::uint32_t last_consequence_events = 0;
    bool operator==(const Snapshot&) const = default;
};

// Owns all state. Presentation receives only const views or copied snapshots.
class Battle {
public:
    explicit Battle(std::vector<Definition> definitions, BattleRules rules = {},
                    std::vector<InitialHealth> initial_health = {},
                    std::vector<InitialRuntime> initial_runtime = {});
    [[nodiscard]] const std::vector<Fighter>& fighters() const noexcept { return fighters_; }
    [[nodiscard]] const Fighter* find(std::string_view id) const noexcept;
    [[nodiscard]] const Fighter* current_actor() const noexcept;
    [[nodiscard]] Phase phase() const noexcept { return phase_; }
    [[nodiscard]] std::optional<Team> winner() const noexcept { return winner_; }
    [[nodiscard]] std::uint64_t round() const noexcept { return round_; }
    [[nodiscard]] std::uint64_t accepted_actions() const noexcept { return accepted_actions_; }
    [[nodiscard]] std::optional<EndReason> end_reason() const noexcept { return end_reason_; }
    [[nodiscard]] std::optional<std::uint64_t> remaining_actions() const noexcept;
    [[nodiscard]] Snapshot snapshot() const;
    [[nodiscard]] ActionResult apply(const Action& action);
    [[nodiscard]] bool current_actor_blocked() const noexcept;
    [[nodiscard]] bool current_actor_cooldowns_frozen() const noexcept;

private:
    std::vector<Fighter> fighters_;
    std::vector<std::size_t> order_;
    std::size_t cursor_ = 0;
    std::uint64_t round_ = 1;
    Phase phase_ = Phase::Running;
    std::optional<Team> winner_;
    BattleRules rules_;
    std::uint64_t accepted_actions_ = 0;
    std::optional<EndReason> end_reason_;
    std::size_t active_ = 0;
    std::uint64_t logical_time_ = 0;
    std::uint64_t turn_id_ = 1;
    TurnKind turn_kind_ = TurnKind::Natural;
    std::uint64_t natural_turns_ = 0;
    std::uint64_t extra_turns_ = 0;
    std::uint64_t derived_actions_ = 0;
    std::vector<ExtraTurnContext> extras_;
    std::uint32_t extra_chain_ = 0;
    std::optional<ExtraTurnContext> current_extra_;
    std::uint64_t originating_natural_turn_ = 1;
    std::vector<ExtraProcCount> extra_proc_counts_;
    std::uint64_t next_action_id_ = 1;
    std::uint32_t last_consequence_events_ = 0;
    struct PendingReaction {
        std::size_t from;
        std::size_t to;
        std::uint64_t parent_id;
    };
    struct PendingRevive {
        std::size_t fighter;
        int health;
    };

    [[nodiscard]] std::optional<std::size_t> index_of(std::string_view id) const noexcept;
    void advance(std::vector<Event>& events);
    void finish(EndReason reason, std::optional<Team> winner, const std::string& actor, std::vector<Event>& events);
    void select_bar_actor();
    void end_turn(std::vector<Event>& events);
    void hit(std::size_t attacker, std::size_t target, bool derived, const std::string& source,
             const ActionContext& context, std::uint32_t hit_index, std::uint32_t planned_hits,
             std::vector<Event>& events, std::vector<PendingReaction>& reactions,
             std::vector<std::size_t>& guarded, std::vector<PendingRevive>& revives);
    [[nodiscard]] ActionContext make_context(std::size_t actor, ActionType type, const std::string& target,
                                            const std::string& skill, std::uint64_t parent = 0, std::uint64_t root = 0);
    void close_action(const ActionContext& context, const std::vector<std::size_t>& guarded,
                      const std::vector<PendingRevive>& revives, std::vector<Event>& events);
    static void stamp_action(std::vector<Event>& events, std::size_t begin, const ActionContext& context);
    void apply_damage(std::size_t emitter, std::size_t target, int power, EventType type,
                      const ActionContext& context, std::uint32_t hit_index, std::uint32_t planned_hits,
                      std::vector<Event>& events, std::vector<PendingReaction>& reactions,
                      std::vector<std::size_t>& deaths, std::vector<PendingRevive>& revives);
    [[nodiscard]] bool spend_consequence(const ActionContext& context, std::vector<Event>& events);
    [[nodiscard]] std::optional<std::size_t> lowest_health_enemy(Team side) const noexcept;
    void stamp(std::vector<Event>& events, std::size_t begin, std::uint64_t turn, TurnKind kind) const;
};

[[nodiscard]] std::vector<Definition> reference_roster();
// Baseline opponent policy, deterministic and independent of clocks/network.
[[nodiscard]] std::optional<Action> choose_basic_attack(const Battle& battle);

} // namespace rpg
