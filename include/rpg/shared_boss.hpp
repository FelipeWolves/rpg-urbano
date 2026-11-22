#pragma once
#include "rpg/battle.hpp"

namespace rpg {
struct SharedBossMember { std::string character_id; Definition fighter; };
struct SharedBossSquad { std::string id; std::vector<SharedBossMember> members; };
enum class SharedSkillTarget { Self, OwnSquad, Boss, RotatingFront, AllFronts };
struct SharedSkillScope {
    std::string skill_id;
    SharedSkillTarget target;
    bool operator==(const SharedSkillScope&) const = default;
};
struct SharedFrontEnemy { std::string squad_id;Definition fighter; };
struct SharedBossDefinition {
    std::string id;
    std::uint32_t config_version;
    std::vector<SharedBossSquad> squads;
    Definition boss;
    BattleRules rules;
    std::vector<SharedSkillScope> skill_scopes{};
    std::vector<SharedFrontEnemy> front_enemies{};
};
enum class SharedBossError { None, WrongFront, BattleRejected };
struct SharedBossSnapshot {
    std::string stage_id;
    std::uint32_t config_version;
    std::string boss_id;
    std::vector<std::string> squad_ids;
    std::vector<std::vector<std::string>> members;
    std::vector<bool> active_squads;
    std::optional<std::string> next_boss_front;
    Snapshot battle;
    std::vector<SharedSkillScope> skill_scopes;
    bool operator==(const SharedBossSnapshot&) const = default;
};
struct SharedBossActionResult {
    std::string stage_id;
    std::optional<std::string> squad_id;
    SharedBossError error=SharedBossError::None;
    ActionResult action;
    bool stage_finished=false;
    [[nodiscard]] bool accepted() const noexcept { return error==SharedBossError::None && action.accepted(); }
};
// One authoritative Battle, one boss/AP clock. Explicit single-target skill scopes.
class SharedBoss {
public:
    explicit SharedBoss(SharedBossDefinition definition);
    [[nodiscard]] const Battle& battle() const noexcept { return *battle_; }
    [[nodiscard]] SharedBossSnapshot snapshot() const;
    [[nodiscard]] SharedBossActionResult apply(const Action& action);
    [[nodiscard]] std::optional<Action> choose_basic_action() const;
    void reset();
private:
    SharedBossDefinition definition_;
    std::optional<Battle> battle_;
    std::size_t boss_cursor_=0;
    [[nodiscard]] bool active(std::size_t squad) const;
    [[nodiscard]] std::optional<std::size_t> enemy_front(std::string_view id)const;
    [[nodiscard]] std::optional<std::size_t> next_front() const;
    [[nodiscard]] std::optional<std::size_t> squad_of(std::string_view fighter) const;
};
} // namespace rpg
