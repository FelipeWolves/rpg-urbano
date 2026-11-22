#pragma once
#include "rpg/visual_session.hpp"
#include "rpg/pve.hpp"
#include "rpg/stage.hpp"
#include <memory>
namespace rpg::presentation {
enum class PveLabKind { Ritual, Phased, Score, Survival };
[[nodiscard]] PveDefinition pve_lab_definition(PveLabKind kind);
[[nodiscard]] PveDefinition ritual_lab_definition();
[[nodiscard]] PveDefinition score_lab_definition();
[[nodiscard]] PveDefinition survival_lab_definition();
[[nodiscard]] PveDefinition phased_ritual_lab_definition();
// Owns its ledger on the heap: moving the adapter cannot invalidate PveEncounter's ledger pointer.
class PveVisualSession {
public:
 explicit PveVisualSession(PveDefinition definition=ritual_lab_definition());
 [[nodiscard]] VisualState state()const;
 [[nodiscard]] PveSnapshot observation()const{return encounter_->snapshot();}
 [[nodiscard]] std::optional<ScoreReceipt> receipt()const;
 [[nodiscard]] std::size_t receipt_count()const noexcept{return ledger_->receipts().size();}
 [[nodiscard]] bool finished()const noexcept{return encounter_->finished();}
 [[nodiscard]] const std::vector<PvePhaseEvent>& phase_events()const noexcept{return phase_events_;}
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
 std::vector<PvePhaseEvent> phase_events_;
 std::uint64_t revision_=0,attempt_=1;
 std::uint32_t automatic_count_=0;
 bool stopped_=false;
 void check_capacity()const;
 [[nodiscard]] VisualResult preflight(std::uint64_t revision,bool automatic)const;
 [[nodiscard]] VisualResult accept(PveActionResult result,bool automatic);
};
struct WaveVisualDefinition {
 StageDefinition stage;
 std::vector<std::vector<CharacterLoadout>> loadouts;
};
[[nodiscard]] WaveVisualDefinition wave_lab_definition();
struct WaveHud {
 std::string stage_id,wave_id,status;
 std::size_t wave_number,wave_count;
 std::uint64_t global_actions;
 std::optional<std::uint64_t> global_goal,global_remaining;
 std::uint64_t local_actions;
 std::optional<std::uint64_t> local_remaining;
 bool finished,objective_met;
 std::optional<std::string> previous_wave;
};
class WaveVisualSession {
public:
 explicit WaveVisualSession(WaveVisualDefinition definition=wave_lab_definition());
 [[nodiscard]] VisualState state()const;
 [[nodiscard]] WaveHud hud()const;
 [[nodiscard]] StageSnapshot observation()const{return stage_.snapshot();}
 [[nodiscard]] bool finished()const{return stage_.snapshot().phase==Phase::Finished;}
 [[nodiscard]] std::size_t wave_index()const{return stage_.snapshot().encounter_index;}
 [[nodiscard]] const std::vector<Event>& transition_events()const noexcept{return transition_events_;}
 [[nodiscard]] VisualResult submit(std::uint64_t revision,std::uint32_t slot,std::string target);
 [[nodiscard]] VisualResult submit_action(std::uint64_t revision,const Action& action);
 [[nodiscard]] VisualResult step_automatic();
 [[nodiscard]] std::optional<VisualCue> next_visual_cue(){return cues_.pop();}
 void reset();
 void abandon()noexcept{stopped_=true;cues_.clear();}
private:
 WaveVisualDefinition definition_;
 Stage stage_;
 VisualEventQueue cues_;
 std::vector<Event> transition_events_;
 std::optional<std::string> previous_wave_;
 std::uint64_t revision_=0,attempt_=1;
 std::uint32_t automatic_count_=0;
 bool stopped_=false;
 [[nodiscard]] const std::vector<CharacterLoadout>& loadouts()const{return definition_.loadouts[wave_index()];}
 void check_capacity()const;
 [[nodiscard]] VisualResult preflight(std::uint64_t revision,bool automatic)const;
 [[nodiscard]] VisualResult accept(StageActionResult result,bool automatic);
};}