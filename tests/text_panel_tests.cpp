#include "rpg/startup_presentation.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace rpg::presentation;
void check(bool ok){if(!ok){throw std::runtime_error("Text panel contract failed");}}
double measure(std::string_view text){double width=0;for(unsigned char c:text){if((c&0xC0)!=0x80){width+=10;}}return width;}
template<class F> void rejects(F f){bool caught=false;try{f();}catch(const std::invalid_argument&){caught=true;}check(caught);}
bool inside(Rect a,Rect b){return a.x>=b.x && a.y>=b.y && a.x+a.width<=b.x+b.width+1e-6 && a.y+a.height<=b.y+b.height+1e-6;}
bool overlaps(Rect a,Rect b){return a.x<b.x+b.width && b.x<a.x+a.width && a.y<b.y+b.height && b.y<a.y+a.height;}
int main(){try{
    check(wrap_text("alpha beta fim",60,measure)==std::vector<std::string>({"alpha","beta","fim"}));
    check(wrap_text("ação única",50,measure)==std::vector<std::string>({"ação","única"}));
    check(wrap_text("abcdef",20,measure)==std::vector<std::string>({"ab","cd","ef"}));check(wrap_text("áéí",10,measure)==std::vector<std::string>({"á","é","í"}));
    check(wrap_text("a\r\n\nb\n",100,measure)==std::vector<std::string>({"a","","b",""}));check(wrap_text("  a\t b  ",100,measure)==std::vector<std::string>({"a b"}));check(wrap_text("",100,measure)==std::vector<std::string>({""}));
    const auto variable=[](std::string_view s){double result=0;for(char c:s){result+=c=='W'?9:2;}return result;};check(wrap_text("Wi Wi",22,variable)==std::vector<std::string>({"Wi","Wi"}));
    rejects([]{(void)wrap_text("abc",5,measure);});rejects([]{(void)wrap_text("a",100,[](std::string_view){return -1.0;});});rejects([]{(void)wrap_text("a",std::numeric_limits<double>::infinity(),measure);});rejects([]{(void)wrap_text("a",100,{});});
    for(auto bad:{std::string("\xC0\xAF"),std::string("\xED\xA0\x80"),std::string("\xE2"),std::string("a\0b",3)}){rejects([&]{(void)wrap_text(bad,100,measure);});}
    auto p=text_page(7,3,99);check(p.page==2 && p.pages==3 && p.begin==6 && p.end==7);check(text_page(0,3,0).pages==1);rejects([]{(void)text_page(1,0,0);});auto large=text_page(std::numeric_limits<std::size_t>::max(),2,std::numeric_limits<std::size_t>::max());check(large.end==std::numeric_limits<std::size_t>::max());
    for(Rect safe:{Rect{0,0,280,360},Rect{20,80,410,700},Rect{0,0,1280,720}}){for(bool action:{false,true}){auto l=plan_info_panel(safe,22,action);check(l.has_value());check(inside(l->box,safe) && inside(l->body,l->box));std::vector<Rect> areas{l->body,l->previous,l->next,l->close};if(l->action){areas.push_back(*l->action);}for(std::size_t i=0;i<areas.size();++i){check(inside(areas[i],l->box));if(i){check(areas[i].width>=48 && areas[i].height==48);}for(std::size_t j=i+1;j<areas.size();++j){check(!overlaps(areas[i],areas[j]));}}check(l->capacity*22<=l->body.height);}}
    check(!plan_info_panel({0,0,100,100},22));rejects([]{(void)plan_info_panel({0,0,500,500},1e-300);});rejects([]{(void)plan_info_panel({0,0,500,500},std::numeric_limits<double>::quiet_NaN());});
    std::cout<<"TEXT_PANEL_TESTS_RESULT passed=10 total=10\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
