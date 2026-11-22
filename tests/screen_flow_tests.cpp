#include "rpg/startup_presentation.hpp"
#include <iostream>
#include <stdexcept>
using namespace rpg::presentation;
void check(bool ok){if(!ok){throw std::runtime_error("Screen flow contract failed");}}
int main(){try{
 ScreenFlow f;check(f.in_hub() && !f.accepts_actions() && !f.confirm_abandon());
 check(f.open_location() && f.screen()==UiScreen::Location && !f.open_options());check(f.back(false,false)==BackEffect::Transition && f.screen()==UiScreen::Hub);
 check(f.open_options() && !f.enter_arena() && !f.accepts_actions());check(f.back(false,false)==BackEffect::Transition);
 check(f.enter_arena() && f.accepts_actions() && !f.accepts_actions(true));check(f.back(true,true)==BackEffect::CloseDetails && f.screen()==UiScreen::Arena);check(f.back(false,true)==BackEffect::CancelSelection && f.screen()==UiScreen::Arena);
 check(f.back(false,false)==BackEffect::Transition && f.screen()==UiScreen::Pause && !f.accepts_actions());check(f.resume() && f.screen()==UiScreen::Arena);
 check(f.pause() && f.request_abandon() && f.screen()==UiScreen::AbandonConfirm && !f.accepts_actions());check(f.back(false,false)==BackEffect::Transition && f.screen()==UiScreen::Pause && !f.new_attempt_required());
 check(f.request_abandon() && f.confirm_abandon() && f.new_attempt_required() && f.in_hub());check(!f.confirm_abandon() && f.new_attempt_required());
 check(f.open_options());(void)f.back(false,false);check(f.new_attempt_required());check(f.enter_arena() && !f.new_attempt_required());check(f.finish() && f.screen()==UiScreen::Result && !f.accepts_actions());
 check(!f.pause() && !f.request_abandon() && f.restart_result() && f.screen()==UiScreen::Arena);check(!f.restart_result());check(f.finish() && f.return_hub() && !f.return_hub());
 check(f.enter_arena(true) && f.screen()==UiScreen::Result);bool caught=false;try{ScreenFlow invalid(UiScreen::Pause);}catch(const std::invalid_argument&){caught=true;}check(caught);
 std::cout<<"SCREEN_FLOW_TESTS_RESULT passed=10 total=10\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
