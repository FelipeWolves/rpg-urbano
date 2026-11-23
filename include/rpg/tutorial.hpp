#pragma once
#include "rpg/pve_visual.hpp"
#include "rpg/persistence.hpp"
namespace rpg::presentation {
struct TutorialCompletion {bool standard=false,classic=false;bool operator==(const TutorialCompletion&)const=default;};
std::string encode_tutorial_completion(const TutorialCompletion& value);
TutorialCompletion decode_tutorial_completion(std::string_view text);
TutorialCompletion load_tutorial_completion(storage::Files& files,const storage::Keys& keys);
TutorialCompletion record_tutorial_completion(storage::Files& files,const storage::Keys& keys,bool classic);
[[nodiscard]] WaveVisualDefinition standard_tutorial_definition();
class TutorialSession {
 WaveVisualSession session_;
 bool intro_=true;std::size_t lesson_=0;std::vector<std::string> notes_;
 VisualResult observe(VisualResult result);
public:
 TutorialSession():session_(standard_tutorial_definition()){}
 VisualState state()const{return session_.state();}
 WaveHud hud()const{return session_.hud();}
 bool finished()const{return session_.finished();}
 const std::vector<Event>& transition_events()const{return session_.transition_events();}
 std::optional<VisualCue> next_visual_cue(){return session_.next_visual_cue();}
 std::size_t wave_index()const{return session_.wave_index();}
 bool awaiting_lesson()const{return intro_ || !notes_.empty();}
 bool allowed_slot(std::uint32_t slot)const{return lesson_!=0 || slot==1;}
 std::vector<std::string> lesson()const;
 void acknowledge(){intro_=false;notes_.clear();}
 VisualResult submit(std::uint64_t revision,std::uint32_t slot,std::string target);
 VisualResult submit_action(std::uint64_t revision,const Action& action);
 VisualResult step_automatic();
 void reset(){session_.reset();intro_=true;lesson_=0;notes_.clear();}
 void abandon(){session_.abandon();notes_.clear();intro_=false;}
};
enum class SearchAction { Reconnaissance,Concentrate,Advance };
enum class SearchOutcome { Running,Recovered,Storm,ActionLimit,Abandoned };
struct SearchSettings {int search_gain=25,concentration_bonus=25,storm_gain=15,storm_reduction=20;std::uint32_t action_limit=200;};
struct SearchState {std::uint64_t revision=0,attempt=1;int search=0,storm=0;bool concentrated=false,dialogue=false,dialogue_seen=false;std::uint32_t actions=0;SearchOutcome outcome=SearchOutcome::Running;bool operator==(const SearchState&)const=default;};
class ClassicSearchSession {
 SearchSettings settings_;SearchState state_;
public:
 explicit ClassicSearchSession(SearchSettings settings={});
 const SearchState& state()const{return state_;}
 bool act(std::uint64_t revision,SearchAction action);
 bool acknowledge();
 void reset();
 void abandon();
};
std::string hub_uppercase(std::string text);
} // namespace rpg::presentation
