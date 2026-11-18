#include "rpg/local_session.hpp"
#include <limits>
#include <stdexcept>

namespace rpg {
LocalSession::LocalSession(std::vector<Definition> roster,BattleRules rules,CombatProfile profile,
    std::vector<CharacterLoadout> loadouts):battle_(std::move(roster),std::move(rules),profile,std::move(loadouts)){}
bool LocalSession::settle(){
    const auto& battle=battle_.battle();
    if(battle.phase()!=Phase::Finished ||
       (!progress_.completions.empty() && progress_.completions.back().attempt==progress_.attempt)){return false;}
    const auto reason=*battle.end_reason();
    const std::uint64_t credits=reason==EndReason::Elimination && battle.winner()==Team::A?10:0;
    if(credits>std::numeric_limits<std::uint64_t>::max()-progress_.credits){throw std::overflow_error("Local credit limit");}
    progress_.completions.push_back({progress_.attempt,battle.winner(),reason,battle.accepted_actions(),credits});
    progress_.credits+=credits;return true;
}
ActionResult LocalSession::apply(const Action& action){
    auto result=battle_.apply(action);
    if(result.accepted()){static_cast<void>(settle());}
    return result;
}
ActionResult LocalSession::apply_slot(std::uint32_t position,std::string target){
    auto result=battle_.apply_slot(position,std::move(target));
    if(result.accepted()){static_cast<void>(settle());}
    return result;
}
void LocalSession::reset(){
    if(progress_.attempt==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("Local attempt limit");}
    battle_.reset();++progress_.attempt;
}
} // namespace rpg
