#include "rpg/team_selection.hpp"
#include <algorithm>
#include <stdexcept>

namespace rpg {
TeamSelection::TeamSelection(EncounterSetup catalog,Team side):catalog_(std::move(catalog)),side_(side){
    if(side!=Team::A && side!=Team::B){throw std::invalid_argument("Invalid selection side");}
    const ProfileBattle validated(catalog_.roster,catalog_.rules,CombatProfile::Team,catalog_.loadouts);
    for(const auto& fighter:validated.battle().fighters()){
        if(fighter.definition.team==side_){available_.push_back(fighter.definition.id);}
    }
    if(available_.empty()){throw std::invalid_argument("Selection catalog requires available members");}
    selected_.assign(available_.begin(),available_.begin()+std::min<std::size_t>(5,available_.size()));
}
SelectionError TeamSelection::select(std::vector<std::string> ids){
    if(ids.empty()){return SelectionError::Empty;}
    if(ids.size()>5){return SelectionError::TooMany;}
    std::vector<std::string> seen;
    for(const auto& id:ids){
        if(std::ranges::find(available_,id)==available_.end()){return SelectionError::UnknownMember;}
        if(std::ranges::find(seen,id)!=seen.end()){return SelectionError::DuplicateMember;}
        seen.push_back(id);
    }
    selected_=std::move(ids);return SelectionError::None;
}
EncounterSetup TeamSelection::compose() const {
    EncounterSetup result=catalog_;
    std::erase_if(result.roster,[this](const Definition& f){
        return f.team==side_ && std::ranges::find(selected_,f.id)==selected_.end();});
    const auto absent=[&result](std::string_view id){return std::ranges::find(result.roster,id,&Definition::id)==result.roster.end();};
    std::erase_if(result.loadouts,[&absent](const CharacterLoadout& set){return absent(set.fighter_id);});
    std::erase_if(result.rules.skills,[&absent](const SkillDefinition& skill){return absent(skill.owner);});
    return result;
}
} // namespace rpg
