#pragma once
#include "rpg/battle.hpp"
#include <deque>
#include <optional>
#include <stdexcept>
namespace rpg::presentation {
enum class CuePhase { Begin, Impact, State, End };
struct VisualCue {
    std::uint64_t revision=0;
    std::size_t index=0;
    CuePhase phase=CuePhase::State;
    std::optional<Event> event;
};
// Consumes committed results only. Cues have no command/reward/HP mutation API.
class VisualEventQueue {
public:
    explicit VisualEventQueue(std::size_t capacity=1024):capacity_(capacity){if(capacity<2 || capacity>4096){throw std::invalid_argument("Invalid visual queue capacity");}}
    bool observe(std::uint64_t revision,const ActionResult& result){
        if(!result.accepted() || revision==0 || revision<=last_revision_){return false;}
        last_revision_=revision;
        if(result.events.size()>capacity_-2 || queue_.size()>capacity_-result.events.size()-2){++dropped_;return false;}
        queue_.push_back({revision,0,CuePhase::Begin,{}});
        for(std::size_t i=0;i<result.events.size();++i){const auto& event=result.events[i];
            const bool impact=event.type==EventType::PeriodicDamage || event.type==EventType::Attack || event.type==EventType::ReflectedDamage || event.type==EventType::DeathExplosion || event.type==EventType::ShieldAbsorbed;
            queue_.push_back({revision,i+1,impact?CuePhase::Impact:CuePhase::State,event});
        }
        queue_.push_back({revision,result.events.size()+1,CuePhase::End,{}});return true;
    }
    [[nodiscard]] std::size_t size() const noexcept {return queue_.size();}
    [[nodiscard]] std::uint64_t dropped_batches() const noexcept {return dropped_;}
    std::optional<VisualCue> pop(){if(queue_.empty()){return {};}auto cue=std::move(queue_.front());queue_.pop_front();return cue;}
    void clear() noexcept {queue_.clear();} // Does not permit replaying a consumed revision.
private:
    std::size_t capacity_;
    std::uint64_t last_revision_=0,dropped_=0;
    std::deque<VisualCue> queue_;
};
} // namespace rpg::presentation
