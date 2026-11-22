#include "rpg/visual_session.hpp"
#include <limits>
#include <stdexcept>
namespace rpg::presentation {
namespace {
LocalSession make_session(EncounterSetup setup,CombatProfile profile){
    return LocalSession(std::move(setup.roster),std::move(setup.rules),profile,std::move(setup.loadouts));
}
}
VisualSession::VisualSession(EncounterSetup setup,CombatProfile profile):selection_(std::move(setup),Team::A,profile),profile_(profile),session_(make_session(selection_.compose(),profile)){}
VisualState VisualSession::state() const {
    const auto& profile=session_.battle();const auto* actor=profile.battle().current_actor();
    return {revision_,profile.snapshot(),session_.progress(),actor?profile.abilities(actor->definition.id):std::vector<AbilityView>{},
        profile.battle().phase()==Phase::Running && actor && actor->definition.team==Team::B,stopped_};
}
void VisualSession::check_revision_capacity() const {
    if(revision_==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("Visual revision exhausted");}
}
VisualResult VisualSession::submit(std::uint64_t revision,std::uint32_t slot,std::string target){
    if(revision!=revision_){return {VisualError::StaleCommand,{}};}
    if(stopped_){return {VisualError::SafetyStop,{}};}
    const auto* actor=session_.battle().battle().current_actor();
    if(session_.battle().battle().phase()==Phase::Finished){return {VisualError::Finished,{}};}
    if(!actor || actor->definition.team!=Team::A){return {VisualError::NotPlayerTurn,{}};}
    check_revision_capacity();
    auto result=session_.apply_slot(slot,std::move(target));
    if(!result.accepted()){return {VisualError::CoreRejected,std::move(result)};}
    ++revision_;automatic_count_=0;static_cast<void>(visual_events_.observe(revision_,result));return {VisualError::None,std::move(result)};
}
VisualResult VisualSession::submit_action(std::uint64_t revision,const Action& action){
    if(revision!=revision_){return {VisualError::StaleCommand,{}};}
    if(stopped_){return {VisualError::SafetyStop,{}};}
    const auto* actor=session_.battle().battle().current_actor();
    if(session_.battle().battle().phase()==Phase::Finished){return {VisualError::Finished,{}};}
    if(!actor || actor->definition.team!=Team::A){return {VisualError::NotPlayerTurn,{}};}
    check_revision_capacity();auto result=session_.apply(action);
    if(!result.accepted()){return {VisualError::CoreRejected,std::move(result)};}
    ++revision_;automatic_count_=0;static_cast<void>(visual_events_.observe(revision_,result));return {VisualError::None,std::move(result)};
}
VisualResult VisualSession::step_automatic(){
    if(stopped_){return {VisualError::SafetyStop,{}};}
    const auto* actor=session_.battle().battle().current_actor();
    if(session_.battle().battle().phase()==Phase::Finished){return {VisualError::Finished,{}};}
    if(!actor || actor->definition.team!=Team::B){return {VisualError::NotPlayerTurn,{}};}
    if(automatic_count_==256){stopped_=true;return {VisualError::SafetyStop,{}};}
    check_revision_capacity();const auto action=session_.battle().choose_automatic_action();
    if(!action){stopped_=true;return {VisualError::SafetyStop,{}};}
    auto result=session_.apply(*action);
    if(!result.accepted()){stopped_=true;return {VisualError::CoreRejected,std::move(result)};}
    ++revision_;++automatic_count_;static_cast<void>(visual_events_.observe(revision_,result));return {VisualError::None,std::move(result)};
}
void VisualSession::reset(){check_revision_capacity();session_.reset();++revision_;automatic_count_=0;stopped_=false;visual_events_.clear();}
void VisualSession::require_between_battles() const {
    const auto& battle=session_.battle().battle();
    if(battle.phase()==Phase::Running && battle.accepted_actions()!=0){
        throw SaveError("Equipe/save indisponivel durante combate em andamento.");
    }
}
SelectionError VisualSession::select_team(std::vector<std::string> ids){
    require_between_battles();check_revision_capacity();
    auto candidate=selection_;
    const auto error=candidate.select(std::move(ids));
    if(error!=SelectionError::None){return error;}
    auto setup=candidate.compose();
    LocalSession replacement(std::move(setup.roster),std::move(setup.rules),profile_,std::move(setup.loadouts));
    replacement.restore_progress(session_.progress());
    selection_=std::move(candidate);session_=std::move(replacement);
    ++revision_;automatic_count_=0;stopped_=false;visual_events_.clear();return SelectionError::None;
}
std::string VisualSession::export_save() const {
    require_between_battles();
    return encode_save({1,profile_,selection_.snapshot().selected,session_.progress()});
}
void VisualSession::import_save(std::string_view bytes){
    require_between_battles();check_revision_capacity();
    const auto saved=decode_save(bytes);
    if(saved.profile!=profile_){throw SaveError("Save pertence a outro perfil de combate.");}
    auto candidate=selection_;
    if(candidate.select(saved.selected)!=SelectionError::None){throw SaveError("Equipe do save ausente no catalogo atual.");}
    auto setup=candidate.compose();
    LocalSession replacement(std::move(setup.roster),std::move(setup.rules),profile_,std::move(setup.loadouts));
    replacement.restore_progress(saved.progress);
    selection_=std::move(candidate);session_=std::move(replacement);
    ++revision_;automatic_count_=0;stopped_=false;visual_events_.clear();
}
} // namespace rpg::presentation
