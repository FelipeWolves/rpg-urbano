#include "rpg/shared_boss.hpp"
#include <algorithm>
#include <stdexcept>
#include <utility>

namespace rpg {
SharedBoss::SharedBoss(SharedBossDefinition definition):definition_(std::move(definition)) {
    if(definition_.id.empty() || definition_.config_version==0 || definition_.squads.size()!=3 ||
       definition_.boss.team!=Team::B || definition_.rules.timing!=TimingProfile::ActionBar) {
        throw std::invalid_argument("Invalid shared boss profile"); }
    std::vector<std::string> squads,characters,instances{definition_.boss.id};
    for(const auto& squad:definition_.squads){
        if(squad.id.empty() || squad.members.empty() || squad.members.size()>5 ||
           std::ranges::find(squads,squad.id)!=squads.end()){throw std::invalid_argument("Invalid shared squad");}
        squads.push_back(squad.id);
        for(const auto& member:squad.members){
            if(member.character_id.empty() || member.fighter.team!=Team::A ||
               std::ranges::find(characters,member.character_id)!=characters.end() ||
               std::ranges::find(instances,member.fighter.id)!=instances.end()){
                throw std::invalid_argument("Invalid shared member identity");
            }
            characters.push_back(member.character_id);instances.push_back(member.fighter.id);
        }
    }
    if(definition_.skill_scopes.size()!=definition_.rules.skills.size()){throw std::invalid_argument("Each skill needs one scope");}
    std::vector<std::string> scoped;
    for(const auto& scope:definition_.skill_scopes){
        const auto skill=std::ranges::find(definition_.rules.skills,scope.skill_id,&SkillDefinition::id);
        if(skill==definition_.rules.skills.end() || std::ranges::find(scoped,scope.skill_id)!=scoped.end()){
            throw std::invalid_argument("Invalid or duplicate skill scope");}
        scoped.push_back(scope.skill_id);const bool boss=skill->owner==definition_.boss.id;
        const bool valid=(scope.target==SharedSkillTarget::Self && skill->target==TargetRule::Self) ||
            (scope.target==SharedSkillTarget::OwnSquad && !boss && skill->target==TargetRule::Ally) ||
            (scope.target==SharedSkillTarget::Boss && !boss && skill->target==TargetRule::Enemy) ||
            (scope.target==SharedSkillTarget::RotatingFront && boss && skill->target==TargetRule::Enemy);
        if(!valid){throw std::invalid_argument("Skill scope does not match owner/target");}
        if(boss && std::ranges::any_of(skill->effects,[](const EffectDefinition& effect){
            return effect.hit_selection!=HitSelection::FixedTarget;
        })){throw std::invalid_argument("Boss dynamic retargeting is not front-scoped yet");}
    }
    reset();
}
void SharedBoss::reset(){
    std::vector<Definition> roster;
    for(const auto& squad:definition_.squads){for(const auto& member:squad.members){roster.push_back(member.fighter);}}
    roster.push_back(definition_.boss);
    battle_.emplace(std::move(roster),definition_.rules);boss_cursor_=0;
}
bool SharedBoss::active(std::size_t squad) const {
    for(const auto& member:definition_.squads[squad].members){if(battle_->find(member.fighter.id)->health>0){return true;}}
    return false;
}
std::optional<std::size_t> SharedBoss::next_front() const {
    if(battle_->phase()==Phase::Finished){return std::nullopt;}
    for(std::size_t offset=0;offset<3;++offset){const auto index=(boss_cursor_+offset)%3;if(active(index)){return index;}}
    return std::nullopt;
}
std::optional<std::size_t> SharedBoss::squad_of(std::string_view fighter) const {
    for(std::size_t i=0;i<3;++i){for(const auto& member:definition_.squads[i].members){
        if(member.fighter.id==fighter){return i;}
    }}
    return std::nullopt;
}
SharedBossSnapshot SharedBoss::snapshot() const {
    std::vector<std::string> ids;std::vector<std::vector<std::string>> members;std::vector<bool> alive;
    for(std::size_t i=0;i<3;++i){ids.push_back(definition_.squads[i].id);alive.push_back(active(i));
        std::vector<std::string> local;for(const auto& member:definition_.squads[i].members){local.push_back(member.fighter.id);}
        members.push_back(std::move(local));}
    const auto front=next_front();
    return {definition_.id,definition_.config_version,definition_.boss.id,ids,members,alive,
        front?std::optional<std::string>{ids[*front]}:std::nullopt,battle_->snapshot(),definition_.skill_scopes};
}
SharedBossActionResult SharedBoss::apply(const Action& action){
    SharedBossActionResult result;result.stage_id=definition_.id;
    const SkillDefinition* skill=nullptr;const SharedSkillScope* scope=nullptr;
    if(action.type==ActionType::Skill){
        const auto found=std::ranges::find(definition_.rules.skills,action.skill,&SkillDefinition::id);
        if(found!=definition_.rules.skills.end() && found->owner==action.actor){skill=&*found;
            scope=&*std::ranges::find(definition_.skill_scopes,action.skill,&SharedSkillScope::skill_id);}
    }
    const bool boss_attack=action.actor==definition_.boss.id && (action.type==ActionType::Attack ||
        (scope && scope->target==SharedSkillTarget::RotatingFront));
    const auto front=boss_attack?next_front():squad_of(action.actor);
    if(front){result.squad_id=definition_.squads[*front].id;}
    if(battle_->phase()==Phase::Running && boss_attack && front && squad_of(action.target)!=front){
        result.error=SharedBossError::WrongFront;return result;
    }
    if(battle_->phase()==Phase::Running && skill && scope->target==SharedSkillTarget::OwnSquad &&
       squad_of(action.actor)!=squad_of(action.target)){
        result.error=SharedBossError::WrongFront;return result;
    }
    result.action=battle_->apply(action);
    if(!result.action.accepted()){result.error=SharedBossError::BattleRejected;return result;}
    if(boss_attack && front){boss_cursor_=(*front+1)%3;}
    result.stage_finished=battle_->phase()==Phase::Finished;return result;
}
std::optional<Action> SharedBoss::choose_basic_action() const {
    const auto* actor=battle_->current_actor();if(!actor){return std::nullopt;}
    if(actor->definition.id!=definition_.boss.id){return choose_basic_attack(*battle_);}
    if(battle_->current_actor_blocked()){return Action{definition_.boss.id,ActionType::Skip,{}};}
    const auto front=next_front();if(!front){return std::nullopt;}
    const Fighter* target=nullptr;
    for(const auto& member:definition_.squads[*front].members){const auto* candidate=battle_->find(member.fighter.id);
        if(candidate->health>0 && (!target || candidate->health<target->health ||
           (candidate->health==target->health && candidate->definition.id<target->definition.id))){target=candidate;}}
    if(!target){return std::nullopt;}
    return Action{definition_.boss.id,ActionType::Attack,target->definition.id};
}
} // namespace rpg
