#include "rpg/local_session.hpp"
#include <limits>
#include <stdexcept>

namespace rpg {
void validate_progress(const LocalProgress& progress){
    if(progress.attempt==0 || progress.attempt==std::numeric_limits<std::uint64_t>::max() ||
       progress.completions.size()>4096){throw std::invalid_argument("Invalid progress bounds");}
    std::uint64_t previous=0,total=0;
    for(const auto& result:progress.completions){
        const bool valid_winner=!result.winner || result.winner==Team::A || result.winner==Team::B;
        if(!valid_winner || result.attempt<=previous || result.attempt>progress.attempt || result.actions==0 ||
           (result.reason!=EndReason::Elimination && result.reason!=EndReason::ActionLimit && result.reason!=EndReason::TechnicalLimit) ||
           (result.reason==EndReason::Elimination && !result.winner) ||
           (result.reason==EndReason::TechnicalLimit && result.winner)){
            throw std::invalid_argument("Invalid completion ledger");}
        const std::uint64_t expected=result.reason==EndReason::Elimination && result.winner==Team::A?10:0;
        if(result.credits!=expected || result.credits>std::numeric_limits<std::uint64_t>::max()-total){
            throw std::invalid_argument("Invalid credit ledger");}
        total+=result.credits;previous=result.attempt;
    }
    if(total!=progress.credits){throw std::invalid_argument("Credit total mismatch");}
}
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
void LocalSession::restore_progress(const LocalProgress& progress){
    validate_progress(progress);
    if(progress.attempt>=std::numeric_limits<std::uint64_t>::max()-1){
        throw std::invalid_argument("No room for a new persistable attempt");}
    if(progress_.attempt!=1 || progress_.credits!=0 || !progress_.completions.empty() ||
       battle_.battle().accepted_actions()!=0){throw std::invalid_argument("Restore requires a pristine session");}
    auto restored=progress;++restored.attempt;progress_=std::move(restored);
}
} // namespace rpg
