#include "rpg/ability_profile.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace {
using namespace rpg;
void check(bool ok,const char* message){if(!ok){throw std::runtime_error(message);}}
template<class F>void invalid(F f){bool caught=false;try{f();}catch(const std::invalid_argument&){caught=true;}check(caught,"invalid profile accepted");}
struct Setup {
    std::vector<Definition> roster{{"A",Team::A,100,5,200},{"B",Team::B,100,3,100}};
    BattleRules rules;
    std::vector<CharacterLoadout> sets;
    Setup(){
        rules.skills={{"basic-a","A",0,TargetRule::Enemy,{{EffectKind::Damage}}},
            {"special-a","A",3,TargetRule::Enemy,{{EffectKind::Damage}}},
            {"basic-b","B",0,TargetRule::Enemy,{{EffectKind::Damage}}}};
        sets={{"A",{{1,"Basica","basic-a",{}},{2,"Passiva",std::nullopt,"Nao implementada"},
                    {3,"Hibrida","special-a","Gatilho futuro"}}},
              {"B",{{1,"Basica","basic-b",{}}}}};
    }
    ProfileBattle make()const{return {roster,rules,CombatProfile::Team,sets};}
};
void configuration(){
    Setup d;d.sets[0].slots.erase(d.sets[0].slots.begin());invalid([&]{auto s=d.make();});
    d=Setup{};d.rules.skills[0].cooldown=1;invalid([&]{auto s=d.make();});
    d=Setup{};d.rules.skills[0].target=TargetRule::Self;invalid([&]{auto s=d.make();});
    d=Setup{};d.sets[0].slots[2].position=4;invalid([&]{auto s=d.make();});
    d=Setup{};d.sets[0].slots[2].position=1;invalid([&]{auto s=d.make();});
    d=Setup{};d.sets[0].slots[0].active_skill="basic-b";invalid([&]{auto s=d.make();});
    d=Setup{};d.rules.skills.push_back({"hidden","A",0,TargetRule::Self,{{EffectKind::Marker,0,2}}});invalid([&]{auto s=d.make();});
    d=Setup{};d.sets.pop_back();invalid([&]{auto s=d.make();});
    invalid([&]{ProfileBattle s(d.roster,d.rules,static_cast<CombatProfile>(99));});
}
void canonical_basic_no_defense(){
    Setup d;auto a=d.make(),b=d.make(),c=d.make();
    const auto chosen=a.choose_basic_action();check(chosen && chosen->type==ActionType::Skill && chosen->skill=="basic-a","AI bypassed basic definition");
    const auto x=a.apply_slot(1,"B"),y=b.apply({"A",ActionType::Attack,"B"}),z=c.apply(*chosen);
    check(x.accepted() && x==y && x==z && a.snapshot()==b.snapshot() && b.snapshot()==c.snapshot(),"basic paths differ");
    const auto before=a.snapshot();
    check(!a.apply({"B",ActionType::Guard,{}}).accepted() && a.snapshot()==before && !before.battle.rules.guard_enabled,
          "team allowed generic defense");
    ProfileBattle classic(d.roster,BattleRules{});check(classic.apply({"A",ActionType::Guard,{}}).accepted(),"classic defense lost");
}
void passive_and_hybrid(){
    Setup d;auto s=d.make();const auto before=s.snapshot();const auto views=s.abilities("A");
    check(views.size()==3 && views[1].availability==AbilityAvailability::PassiveOnly && !views[1].passive_implemented &&
          views[2].slot.active_skill && !views[2].slot.passive_note.empty() && !views[2].passive_implemented,"component classification wrong");
    check(!s.apply_slot(2,"B").accepted() && s.snapshot()==before,"passive selectable or changed state");
    check(s.apply_slot(3,"B").accepted() && s.battle().find("B")->health==95 && s.battle().find("A")->statuses.empty(),
          "hybrid invented passive effect or lost active");
}
void cooldown_and_basic_fallback(){
    Setup d;auto s=d.make();check(s.apply_slot(3,"B").accepted(),"special failed");
    check(s.apply(*s.choose_basic_action()).accepted(),"enemy basic failed");
    const auto views=s.abilities("A");check(views[2].cooldown==2 && views[2].availability==AbilityAvailability::Cooldown &&
        views[0].availability==AbilityAvailability::Ready,"cooldown blocked basic or count wrong");
    const auto before=s.snapshot();check(!s.apply_slot(3,"B").accepted() && s.snapshot()==before,"cooldown rejection mutated");
    check(s.apply_slot(1,"B").accepted() && s.battle().find("B")->health==90,"basic unavailable during special cooldown");
}
void action_block_is_separate(){
    Setup d;d.rules.skills[1].target=TargetRule::Self;d.rules.skills[1].effects={{EffectKind::Block,0,1}};
    auto s=d.make();check(s.apply_slot(3,"A").accepted(),"block setup failed");
    check(s.apply(*s.choose_basic_action()).accepted(),"enemy setup failed");
    const auto views=s.abilities("A");check(views[0].availability==AbilityAvailability::Incapacitated && views[2].cooldown==2,
                                          "action block confused with cooldown");
    const auto before=s.snapshot();const auto blocked=s.apply_slot(1,"B");
    check(blocked.error==ActionError::ActorBlocked && s.snapshot()==before,"blocked basic executed");
    const auto skip=s.choose_basic_action();check(skip && skip->type==ActionType::Skip && s.apply(*skip).accepted(),"blocked actor failed Skip");
}
void reset_replay_and_classic(){
    Setup d;d.rules.action_budget=ActionBudget{8,LimitOutcome::Draw};auto a=d.make(),b=d.make();const auto initial=a.snapshot();
    int finishes=0;
    while(a.battle().phase()==Phase::Running){const auto command=*a.choose_basic_action();const auto x=a.apply(command),y=b.apply(command);
        check(x==y && a.snapshot()==b.snapshot(),"profile replay diverged");
        finishes+=static_cast<int>(std::ranges::count(x.events,EventType::BattleFinished,&Event::type));}
    const auto final=b.snapshot();check(finishes==1 && !a.apply_slot(1,"B").accepted() && a.snapshot()==final,"post-finish changed");
    a.reset();check(a.snapshot()==initial && b.snapshot()==final,"profile reset leaked");
    d=Setup{};ProfileBattle classic(d.roster,d.rules);Battle direct(d.roster,d.rules);
    const Action commands[]={{"A",ActionType::Guard,{}},{"B",ActionType::Attack,"A"},{"A",ActionType::Skill,"B","special-a"}};
    for(const auto& cmd:commands){check(classic.apply(cmd)==direct.apply(cmd) && classic.battle().snapshot()==direct.snapshot(),"classic contract changed");}
}
}
int main(int argc,char** argv){
    const std::pair<const char*,void(*)()> tests[]={{"configuration",configuration},{"canonical_basic",canonical_basic_no_defense},
        {"passive_hybrid",passive_and_hybrid},{"cooldown_basic",cooldown_and_basic_fallback},
        {"action_block",action_block_is_separate},{"replay_classic",reset_replay_and_classic}};
    int passed=0;for(const auto& [name,test]:tests){try{test();++passed;std::cout<<"PASS ability_profile "<<name<<'\n';}
        catch(const std::exception& e){std::cerr<<"FAIL ability_profile "<<name<<": "<<e.what()<<'\n';}}
    std::cout<<"PROFILE_TESTS_RESULT passed="<<passed<<" total="<<std::size(tests)<<'\n';
    if(argc==2 && passed==static_cast<int>(std::size(tests))){std::ofstream out(argv[1]);out<<"test,status,scope\n";
        for(const auto& [name,test]:tests){static_cast<void>(test);out<<name<<",pass,team_slots_classic_preserved\n";}if(!out){return 2;}}
    return passed==static_cast<int>(std::size(tests))?0:1;
}
