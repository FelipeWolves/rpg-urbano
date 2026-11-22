#include "rpg/visual_events.hpp"
#include <iostream>
using namespace rpg;using namespace rpg::presentation;
void check(bool v){if(!v){throw std::runtime_error("Visual event contract failed");}}
int main(){try{
 ActionResult action;Event hit{EventType::Attack,"A1","B1",10};hit.hit_index=2;hit.planned_hits=3;action.events.push_back(hit);
 VisualEventQueue q;check(q.observe(1,action) && q.size()==3);check(q.pop()->phase==CuePhase::Begin);auto impact=q.pop();check(impact->phase==CuePhase::Impact && impact->event==hit && impact->revision==1);check(q.pop()->phase==CuePhase::End && !q.pop());
 check(!q.observe(1,action) && !q.observe(0,action));q.clear();check(!q.observe(1,action));
 auto rejected=action;rejected.error=ActionError::UnknownActor;check(!q.observe(2,rejected) && q.size()==0);check(q.observe(2,action));
 VisualEventQueue small(3);check(small.observe(1,action));check(!small.observe(2,action) && small.size()==3 && small.dropped_batches()==1);small.clear();check(!small.observe(2,action) && small.observe(3,action));
 ActionResult many;many.events.assign(5,hit);VisualEventQueue limited(4);check(!limited.observe(1,many) && limited.size()==0);
 ActionResult state;state.events.push_back({EventType::FighterDefeated,"A1","B1"});check(q.observe(3,state));q.pop();q.pop();q.pop();q.pop();check(q.pop()->phase==CuePhase::State);
 bool caught=false;try{VisualEventQueue invalid(1);}catch(const std::invalid_argument&){caught=true;}check(caught);
 std::cout<<"AUT13_TESTS_RESULT passed=7 total=7\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
