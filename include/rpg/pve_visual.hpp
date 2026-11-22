#pragma once
#include "rpg/visual_session.hpp"
#include "rpg/pve.hpp"
#include <memory>
namespace rpg::presentation {
[[nodiscard]] PveDefinition ritual_lab_definition();
// Owns its ledger on the heap: moving the adapter cannot invalidate PveEncounter's ledger pointer.
class PveVisualSession {
public:
 explicit PveVisualSession(PveDefinition definition=ritual_lab_definition());
 [[nodiscard]] VisualState state()const;
 [[nodiscard]] PveSnapshot observation()const{return encounter_->snapshot();}
 [[nodiscard]] bool finished()const noexcept{return encounter_->finished();}
 [[nodiscard]] VisualResult submit(std::uint64_t revision,std::uint32_t slot,std::string target);
 [[nodiscard]] VisualResult submit_action(std::uint64_t revision,const Action& action);
 [[nodiscard]] VisualResult step_automatic();
 [[nodiscard]] std::optional<VisualCue> next_visual_cue(){return cues_.pop();}
 void reset();
 void abandon()noexcept{stopped_=true;cues_.clear();}
private:
 PveDefinition definition_;
 std::unique_ptr<ScoreLedger> ledger_;
 std::unique_ptr<PveEncounter> encounter_;
 VisualEventQueue cues_;
 std::uint64_t revision_=0,attempt_=1;
 std::uint32_t automatic_count_=0;
 bool stopped_=false;
 void check_capacity()const;
 [[nodiscard]] VisualResult preflight(std::uint64_t revision,bool automatic)const;
 [[nodiscard]] VisualResult accept(PveActionResult result,bool automatic);
};
}