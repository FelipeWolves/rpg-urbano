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
    for(auto& squad:definition_.squads){
        if(squad.id.empty() || squad.members.empty() || squad.members.size()>5 ||
           std::ranges::find(squads,squad.id)!=squads.end()){throw std::invalid_argument("Invalid shared squad");}
        squads.push_back(squad.id);
        for(auto& member:squad.members){
            if(member.character_id.empty() || member.fighter.team!=Team::A ||
               std::ranges::find(characters,member.character_id)!=characters.end() ||
               std::ranges::find(instances,member.fighter.id)!=instances.end()){
                throw std::invalid_argument("Invalid shared member identity");
            }
            if(definition_.config_version>=2){member.fighter.front_id=squad.id;}
            characters.push_back(member.character_id);instances.push_back(member.fighter.id);
        }
    }
    if(definition_.config_version<2 && (!definition_.front_enemies.empty() || std::ranges::any_of(definition_.rules.skills,[](const SkillDefinition& s){return s.target==TargetRule::EnemyTeam;}))){throw std::invalid_argument("Area/front enemies require profile v2");}
    if(definition_.front_enemies.size()>64){throw std::invalid_argument("Front enemy safety bound");}
    for(auto& extra:definition_.front_enemies){
        if(extra.fighter.team!=Team::B || std::ranges::find(squads,extra.squad_id)==squads.end() || std::ranges::find(instances,extra.fighter.id)!=instances.end()){throw std::invalid_argument("Invalid front enemy");}
        if(extra.fighter.role==EntityRole::Fighter){extra.fighter.role=EntityRole::Auxiliary;}instances.push_back(extra.fighter.id);
    }
    if(definition_.config_version>=2){definition_.boss.role=EntityRole::Boss;definition_.boss.is_boss=true;definition_.rules.required_enemy=definition_.boss.id;definition_.rules.allow_scoped_aoe=true;}
    if(definition_.skill_scopes.size()!=definition_.rules.skills.size()){throw std::invalid_argument("Each skill needs one scope");}
    std::vector<std::string> scoped;
    for(const auto& scope:definition_.skill_scopes){
        const auto skill=std::ranges::find(definition_.rules.skills,scope.skill_id,&SkillDefinition::id);
        if(skill==definition_.rules.skills.end() || std::ranges::find(scoped,scope.skill_id)!=scoped.end()){throw std::invalid_argument("Invalid scope identity");}
        scoped.push_back(scope.skill_id);const bool boss=skill->owner==definition_.boss.id;const bool enemy=boss||enemy_front(skill->owner).has_value();
        const bool valid=(scope.target==SharedSkillTarget::Self && skill->target==TargetRule::Self) ||
          (scope.target==SharedSkillTarget::OwnSquad&&!enemy&&skill->target==TargetRule::Ally) ||
          (scope.target==SharedSkillTarget::Boss&&!enemy&&(skill->target==TargetRule::Enemy||skill->target==TargetRule::EnemyTeam)) ||
          (scope.target==SharedSkillTarget::RotatingFront&&enemy&&(skill->target==TargetRule::Enemy||skill->target==TargetRule::EnemyTeam)) ||
          (scope.target==SharedSkillTarget::AllFronts&&boss&&definition_.config_version>=2&&skill->target==TargetRule::EnemyTeam);
        if(!valid){throw std::invalid_argument("Skill scope/target mismatch");}
    }
    reset();
}
void SharedBoss::reset(){
    std::vector<Definition> roster;
    for(auto& squad:definition_.squads){for(auto& member:squad.members){roster.push_back(member.fighter);}}
    roster.push_back(definition_.boss);
    for(const auto& extra:definition_.front_enemies){roster.push_back(extra.fighter);}
    battle_.emplace(std::move(roster),definition_.rules);boss_cursor_=0;
}
std::optional<std::size_t> SharedBoss::enemy_front(std::string_view id)const{
    const auto extra=std::ranges::find_if(definition_.front_enemies,[&](const SharedFrontEnemy& e){return e.fighter.id==id;});
    if(extra==definition_.front_enemies.end()){return std::nullopt;}
    for(std::size_t i=0;i<definition_.squads.size();++i){if(definition_.squads[i].id==extra->squad_id){return i;}}return std::nullopt;
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
    const auto auxiliary=enemy_front(action.actor);
    const auto front=boss_attack?next_front():auxiliary?auxiliary:squad_of(action.actor);
    if(front){result.squad_id=definition_.squads[*front].id;}
    if(battle_->phase()==Phase::Running && (boss_attack||auxiliary.has_value()) && front && (action.type==ActionType::Attack||(scope&&scope->target==SharedSkillTarget::RotatingFront)) && squad_of(action.target)!=front){
        result.error=SharedBossError::WrongFront;return result;
    }
    if(battle_->phase()==Phase::Running && skill && scope->target==SharedSkillTarget::OwnSquad &&
       squad_of(action.actor)!=squad_of(action.target)){
        result.error=SharedBossError::WrongFront;return result;
    }
    if(!action.target_scope.empty()){result.error=SharedBossError::WrongFront;return result;}
    if(definition_.config_version>=2 && squad_of(action.actor) && (action.type==ActionType::Attack || (scope&&scope->target==SharedSkillTarget::Boss)) && action.target!=definition_.boss.id && enemy_front(action.target)!=squad_of(action.actor)){result.error=SharedBossError::WrongFront;return result;}
    auto scoped_action=action;
    if(skill&&skill->target==TargetRule::EnemyTeam){
        for(const auto& f:battle_->fighters()){if(f.health==0){continue;}
            bool include=false;
            if(scope->target==SharedSkillTarget::AllFronts){include=f.definition.team==Team::A;}
            else if(scope->target==SharedSkillTarget::RotatingFront){include=f.definition.team==Team::A && squad_of(f.definition.id)==front;}
            else if(scope->target==SharedSkillTarget::Boss){include=f.definition.id==definition_.boss.id || (front && enemy_front(f.definition.id)==front);}
            if(include){scoped_action.target_scope.push_back(f.definition.id);}
        }
    }
    result.action=battle_->apply(scoped_action);
    if(!result.action.accepted()){result.error=SharedBossError::BattleRejected;return result;}
    if(boss_attack && front){boss_cursor_=(*front+1)%3;}
    result.stage_finished=battle_->phase()==Phase::Finished;return result;
}
std::optional<Action> SharedBoss::choose_basic_action()const{
 const auto* actor=battle_->current_actor();if(!actor){return std::nullopt;}
 if(battle_->current_actor_blocked()){return Action{actor->definition.id,ActionType::Skip,{}};}
 const auto front=actor->definition.id==definition_.boss.id?next_front():actor->definition.team==Team::A?squad_of(actor->definition.id):enemy_front(actor->definition.id);
 const Fighter* selected=nullptr;
 for(const auto& f:battle_->fighters()){if(f.health==0 || f.definition.team==actor->definition.team){continue;}
  const bool valid=actor->definition.team==Team::A?(f.definition.id==definition_.boss.id||(front&&enemy_front(f.definition.id)==front)):(front&&squad_of(f.definition.id)==front);
  if(valid&&(!selected||f.health<selected->health||(f.health==selected->health&&f.definition.id<selected->definition.id))){selected=&f;}
 }
 if(!selected){return std::nullopt;}return Action{actor->definition.id,ActionType::Attack,selected->definition.id};
}
}
