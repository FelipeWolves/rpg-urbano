#include "rpg/parallel.hpp"
#include <algorithm>
#include <stdexcept>
#include <utility>

namespace rpg {
ParallelFronts::ParallelFronts(ParallelDefinition definition):definition_(std::move(definition)) {
    if(definition_.id.empty() || definition_.config_version==0 || definition_.fronts.size()!=3){
        throw std::invalid_argument("Three identified fronts required");
    }
    auto order=definition_.schedule;std::ranges::sort(order);
    if(order!=std::array<std::size_t,3>{0,1,2}){throw std::invalid_argument("Schedule must be a permutation");}
    if(definition_.global_budget){
        const auto& budget=*definition_.global_budget;
        if(budget.max_actions<=0 || (budget.outcome!=LimitOutcome::Draw && budget.outcome!=LimitOutcome::DefeatA &&
                                    budget.outcome!=LimitOutcome::DefeatB)){throw std::invalid_argument("Invalid global budget");}
    }
    std::vector<std::string> ids;
    for(const auto& front:definition_.fronts){
        if(front.id.empty() || std::ranges::find(ids,front.id)!=ids.end()){
            throw std::invalid_argument("Front IDs must be unique");
        }
        ids.push_back(front.id);
    }
    reset(); // Battle validates every roster/rule before construction completes.
}
void ParallelFronts::reset(){
    std::vector<Battle> fresh;
    for(const auto& front:definition_.fronts){fresh.emplace_back(front.fighters,front.rules);}
    battles_=std::move(fresh);cursor_=0;actions_=0;phase_=Phase::Running;winner_.reset();reason_.reset();
    front_states_.assign(3,FrontState::Running);
}
std::optional<std::uint64_t> ParallelFronts::remaining_actions() const noexcept {
    if(!definition_.global_budget){return std::nullopt;}
    return static_cast<std::uint64_t>(definition_.global_budget->max_actions)-actions_;
}
void ParallelFronts::finish(ParallelEndReason reason,std::optional<Team> winner){
    phase_=Phase::Finished;reason_=reason;winner_=winner;
    for(auto& state:front_states_){if(state==FrontState::Running){state=FrontState::Cancelled;}}
}
bool ParallelFronts::cancel(){
    if(phase_==Phase::Finished){return false;}
    finish(ParallelEndReason::Cancelled,std::nullopt);return true;
}
std::optional<std::size_t> ParallelFronts::current_front() const noexcept {
    if(phase_==Phase::Finished){return std::nullopt;}
    return definition_.schedule[cursor_];
}
ParallelSnapshot ParallelFronts::snapshot() const {
    std::vector<std::string> ids;std::vector<Snapshot> states;
    for(std::size_t i=0;i<battles_.size();++i){ids.push_back(definition_.fronts[i].id);states.push_back(battles_[i].snapshot());}
    return {definition_.id,definition_.config_version,ids,states,definition_.schedule,current_front(),actions_,phase_,winner_,reason_,
            definition_.global_budget,front_states_};
}
void ParallelFronts::advance(){
    for(std::size_t offset=1;offset<=3;++offset){
        const auto next=(cursor_+offset)%3;
        if(battles_[definition_.schedule[next]].phase()==Phase::Running){cursor_=next;return;}
    }
    phase_=Phase::Finished;reason_=ParallelEndReason::AllFrontsFinished;
    bool all_a=true,any_b=false,technical=false;
    for(const auto& battle:battles_){
        all_a=all_a && battle.winner()==Team::A;any_b=any_b || battle.winner()==Team::B;
        technical=technical || battle.end_reason()==EndReason::TechnicalLimit;
    }
    if(technical){reason_=ParallelEndReason::TechnicalFailure;}
    else if(all_a){winner_=Team::A;}
    else if(any_b){winner_=Team::B;}
}
ParallelActionResult ParallelFronts::apply(std::string_view front,const Action& action){
    ParallelActionResult result;result.stage_id=definition_.id;result.front_id=front;
    if(phase_==Phase::Finished){result.error=ParallelError::StageFinished;return result;}
    const auto found=std::ranges::find(definition_.fronts,front,&FrontDefinition::id);
    if(found==definition_.fronts.end()){result.error=ParallelError::UnknownFront;return result;}
    const auto index=static_cast<std::size_t>(found-definition_.fronts.begin());
    if(index!=*current_front()){result.error=ParallelError::NotScheduled;return result;}
    result.action=battles_[index].apply(action);
    if(!result.action.accepted()){result.error=ParallelError::BattleRejected;return result;}
    result.stage_action_id=++actions_;
    result.front_finished=battles_[index].phase()==Phase::Finished;
    if(result.front_finished){front_states_[index]=FrontState::Finished;}
    bool technical=false,all_eliminated=true;
    for(const auto& battle:battles_){
        technical=technical || battle.end_reason()==EndReason::TechnicalLimit;
        all_eliminated=all_eliminated && battle.phase()==Phase::Finished && battle.end_reason()==EndReason::Elimination;
    }
    if(technical){finish(ParallelEndReason::TechnicalFailure,std::nullopt);}
    else if(remaining_actions()==0 && !all_eliminated){
        std::optional<Team> winner;
        if(definition_.global_budget->outcome==LimitOutcome::DefeatA){winner=Team::B;}
        else if(definition_.global_budget->outcome==LimitOutcome::DefeatB){winner=Team::A;}
        finish(ParallelEndReason::GlobalActionLimit,winner);
    }else{advance();}
    for(std::size_t i=0;i<front_states_.size();++i){
        if(front_states_[i]==FrontState::Cancelled){result.cancelled_fronts.push_back(definition_.fronts[i].id);}
    }
    result.stage_finished=phase_==Phase::Finished;return result;
}
} // namespace rpg
