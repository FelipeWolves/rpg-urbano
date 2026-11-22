#include "rpg/startup_presentation.hpp"
#include <iostream>
#include <stdexcept>
using namespace rpg::presentation;
void check(bool ok,const char* message="Media lifecycle contract failed"){if(!ok){throw std::runtime_error(message);}}
int main(){try{
 Asset asset{"intro","Movies/intro.mp4",{1920,1080}};MediaFlow f;
 check(f.state()==MediaState::Idle && f.accepts_gameplay() && !f.layout({1080,1920},Device::Mobile));
 auto ticket=f.begin(asset);check(f.state()==MediaState::Preparing && !f.accepts_gameplay() && !f.ended(ticket) && !f.skip(800));
 auto plan=f.layout({1080,1920},Device::Mobile);check(plan.has_value());const auto reference=plan_video(asset.size,{1080,1920},Device::Mobile);check(plan->content.width==reference.content.width && plan->request==reference.request);
 check(f.prepared(ticket) && !f.prepared(ticket) && !f.skip(799));check(f.skip(800) && f.watched() && f.return_to_hub() && !f.ended(ticket));
 ticket=f.begin(asset,true);check(f.prepared(ticket) && f.skip(0) && f.watched());
 ticket=f.begin(asset);check(f.prepared(ticket));f.background(true);check(f.state()==MediaState::Paused && !f.resume() && !f.skip(800));f.background(false);check(f.state()==MediaState::Paused && f.resume() && f.ended(ticket) && f.watched());
 ticket=f.begin(asset);f.background(true);check(f.prepared(ticket) && f.state()==MediaState::Paused);f.background(false);check(f.state()==MediaState::Paused && f.resume());check(f.pause() && !f.ended(ticket) && f.resume());
 const auto old=ticket;ticket=f.begin(asset);check(!f.prepared(old) && !f.ended(old) && !f.fail(old,MediaFailure::DecodeFailed));check(f.prepared(ticket) && f.fail(ticket,MediaFailure::DecodeFailed) && !f.watched() && f.return_to_hub());
 ticket=f.begin(std::nullopt);check(f.failure()==MediaFailure::MissingAsset && f.return_to_hub() && !f.watched() && !f.prepared(ticket));
 auto invalid=asset;invalid.relative_path="https://example.invalid/video.mp4";ticket=f.begin(invalid);check(f.failure()==MediaFailure::InvalidAsset && f.return_to_hub() && !f.layout({800,600},Device::Desktop));
 ticket=f.begin(asset);check(!f.fail(ticket,MediaFailure::None) && f.fail(ticket,MediaFailure::Timeout) && !f.watched());ticket=f.begin(asset,true);check(f.cancel() && f.watched() && f.return_to_hub() && !f.cancel());
 ticket=f.begin(asset);check(f.cancel() && !f.watched() && !f.ended(ticket));ticket=f.begin(asset);check(f.prepared(ticket) && f.ended(ticket) && !f.ended(ticket) && !f.cancel());
 MediaStartGate gate;MediaFlow pending;ticket=pending.begin(asset);
 check(!gate.should_request(pending.state(),false,false,true,0),"unready backend waits");
 check(gate.should_request(pending.state(),true,false,true,.1),"first request without second click");
 check(pending.state()==MediaState::Preparing&&!pending.watched(),"request is not playback proof");
 check(!gate.should_request(pending.state(),true,false,true,.15)&&gate.should_request(pending.state(),true,false,true,.21),"bounded retry after asynchronous refusal");
 check(!gate.should_request(pending.state(),true,true,true,.4)&&pending.prepared(ticket),"actual playback completes preparation");
 check(!gate.should_request(pending.state(),true,false,true,.6),"playing flow cannot restart from gate");
 gate.reset();check(!gate.should_request(MediaState::Preparing,true,false,false,.7)&&gate.attempts()==0,"background cannot start");
 check(gate.should_request(MediaState::Preparing,true,false,true,.8)&&gate.attempts()==1,"fresh playback resets attempts");
 std::cout<<"MEDIA_FLOW_TESTS_RESULT passed=16 total=16\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
