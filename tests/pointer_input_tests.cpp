#include "rpg/startup_presentation.hpp"
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace rpg::presentation;
void check(bool ok){if(!ok){throw std::runtime_error("Pointer input contract failed");}}
PointerSample sample(bool down,double x=20,double y=20){return {down,true,x,y};}
PointerContext c{7,0};
void mouse(){PointerInput p;auto f=p.update(sample(true),{},true,0,c);check(f.pressed && f.down);f=p.update(sample(false),{},true,.1,c);check(f.released && f.tap && pointer_clicked(f,{0,0,48,48}));check(!p.update(sample(false),{},true,.2,c).tap);}
void touch(){PointerInput p;auto f=p.update(sample(true),sample(true),true,0,c);check(f.source==PointerSource::Touch && f.pressed);f=p.update(sample(false),sample(false),true,.1,c);check(f.source==PointerSource::Touch && f.tap);check(!p.update(sample(false),sample(false),true,.2,c).tap);}
void guard(){PointerInput p;(void)p.update({},sample(true),true,0,c);check(p.update({},sample(false),true,.1,c).tap);check(!p.update(sample(true),{},true,.2,c).pressed);check(!p.update(sample(true),{},true,.6,c).pressed);(void)p.update(sample(false),{},true,.7,c);check(p.update(sample(true),{},true,.8,c).pressed);}
void drag(){PointerInput p;(void)p.update(sample(true),{},true,0,c);check(p.update(sample(true,40),{},true,.1,c).cancelled);check(!p.update(sample(false),{},true,.2,c).tap);check(p.update(sample(true),{},true,.3,c).pressed);}
void revision(){PointerInput p;(void)p.update(sample(true),{},true,0,c);check(p.update(sample(true),{},true,.1,{8,0}).cancelled);check(!p.update(sample(false),{},true,.2,{8,0}).tap);PointerInput q;(void)q.update(sample(true),{},true,0,c);check(q.update(sample(true),{},true,.1,{7,1}).cancelled);PointerInput r;(void)r.update({},sample(true),true,0,c);check(r.update({},sample(false),true,.1,{7,1}).cancelled);check(!r.update(sample(true),{},true,.2,{7,1}).pressed);}
void focus(){PointerInput p;(void)p.update(sample(true),{},true,0,c);check(p.update(sample(true),{},false,.1,c).cancelled);check(!p.update(sample(true),{},true,.2,c).pressed);check(!p.update(sample(false),{},true,.3,c).tap);check(p.update(sample(true),{},true,.4,c).pressed);PointerInput q;(void)q.update(sample(false),{},false,0,c);check(!q.update(sample(true),{},true,.1,c).pressed);check(!q.update(sample(false),{},true,.2,c).tap);check(q.update(sample(true),{},true,.3,c).pressed);}
void invalid(){PointerInput p;(void)p.update(sample(true),{},true,1,c);check(p.update(sample(false),{},true,.5,c).cancelled);PointerInput q;(void)q.update(sample(true),{},true,0,c);check(q.update({true,true,std::numeric_limits<double>::quiet_NaN(),0},{},true,.1,c).cancelled);check(!q.update(sample(false),{},true,.2,c).tap);PointerInput r;check(!r.update(sample(false),{},true,0,c).tap);}
void hold(){PointerInput p;(void)p.update(sample(true),{},true,0,c);auto f=p.update(sample(true),{},true,.5,c);check(f.down && !f.tap && !f.released);f=p.update(sample(false),{},true,.6,c);check(f.released && !f.tap);}
void handoff(){PointerInput p;(void)p.update(sample(true),{},true,0,c);auto f=p.update(sample(true),sample(true),true,.1,c);check(f.cancelled && f.pressed && f.source==PointerSource::Touch);f=p.update(sample(false),sample(false),true,.2,c);check(f.tap && f.source==PointerSource::Touch);}
void origin(){PointerInput p;(void)p.update(sample(true,47),{},true,0,c);auto f=p.update(sample(false,50),{},true,.1,c);check(f.tap && !pointer_clicked(f,{48,0,48,48}));auto hover=p.update(sample(false,90,70),{},true,.2,c);check(!hover.tap && hover.x==90 && hover.y==70);}
void panorama_drag(){
    const PointerContext drag_context{0,0,true};
    PointerInput pointer;
    (void)pointer.update(sample(true,50),{},true,0,drag_context);
    auto moved=pointer.update(sample(true,125),{},true,0.1,drag_context);
    check(moved.down && !moved.cancelled && moved.origin_x==50 && moved.x==125);
    auto released=pointer.update(sample(false,50),{},true,0.2,drag_context);
    check(released.released && !released.tap);
}
int main(){try{panorama_drag();mouse();touch();guard();drag();revision();focus();invalid();hold();handoff();origin();std::cout<<"POINTER_INPUT_TESTS_RESULT passed=11 total=11\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
