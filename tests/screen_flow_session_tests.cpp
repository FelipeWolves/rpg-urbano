#include "rpg/startup_presentation.hpp"
#include "rpg/battle_presentation.hpp"
#include <iostream>
#include <stdexcept>
using namespace rpg;
using namespace rpg::presentation;
void check(bool ok){if(!ok){throw std::runtime_error("Screen/session boundary failed");}}
int main(){try{
 VisualSession session;ScreenFlow f(UiScreen::Arena);const auto before=session.state();check(f.pause());check(session.state().combat==before.combat && session.state().progress==before.progress);check(f.resume() && session.state().revision==before.revision);
 check(f.pause() && f.request_abandon() && f.confirm_abandon());check(session.state().combat==before.combat && session.state().progress==before.progress);check(f.new_attempt_required());session.reset();check(f.enter_arena());const auto next=session.state();check(next.progress.attempt==before.progress.attempt+1 && next.progress.credits==before.progress.credits && next.progress.completions==before.progress.completions);
 for(int i=0;i<1000 && session.state().combat.battle.phase==Phase::Running;++i){auto s=session.state();if(s.automatic){check(session.step_automatic().accepted());}else{const auto targets=target_candidates(s,1);if(targets.empty()){throw std::runtime_error("No basic target");}check(session.submit(s.revision,1,targets.front()).accepted());}}
 check(session.state().combat.battle.phase==Phase::Finished);const auto completed=session.state().progress;check(f.finish() && f.return_hub());check(f.enter_arena(true));check(session.state().progress==completed);check(f.restart_result());session.reset();check(session.state().progress.credits==completed.credits && session.state().progress.completions==completed.completions);
 std::cout<<"SCREEN_SESSION_TESTS_RESULT passed=3 total=3\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
