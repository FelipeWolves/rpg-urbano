#include "rpg/team_selection.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>
namespace {
using namespace rpg;
void check(bool value,const char* message){if(!value){throw std::runtime_error(message);}}
template<class F>void invalid(F f){bool caught=false;try{f();}catch(const std::invalid_argument&){caught=true;}check(caught,"invalid catalog accepted");}
EncounterSetup catalog(){
    EncounterSetup d;d.rules.action_budget=ActionBudget{10,LimitOutcome::Draw};
    for(int i=1;i<=7;++i){const auto id=i<=6?"A"+std::to_string(i):std::string{"B1"};
        d.roster.push_back({id,i<=6?Team::A:Team::B,30,5,200-i});
        d.rules.skills.push_back({id+"-basic",id,0,TargetRule::Enemy,{{EffectKind::Damage}}});
        CharacterLoadout set{id,{{1,"Basic",id+"-basic",{}}}};
        if(i==2){set.slots[0].passive=PassiveDefinition{"A2-opening"};}
        d.loadouts.push_back(std::move(set));}
    return d;
}
void default_and_detached_catalog(){
    auto d=catalog();TeamSelection s(d);const auto state=s.snapshot();
    check(state.available.size()==6 && state.selected.size()==5 && state.selected.front()=="A1" &&
        state.selected.back()=="A5","default selection not canonical/bounded");
    d.roster[0].attack=999;check(s.compose().roster[0].attack==5,"catalog aliases external state");
}
void invalid_atomic(){
    TeamSelection s(catalog());check(s.select({"A6"})==SelectionError::None,"available sixth member not selectable");const auto old=s.snapshot();
    check(s.select({})==SelectionError::Empty && s.snapshot()==old,"empty selection mutated");
    check(s.select({"A1","A1"})==SelectionError::DuplicateMember && s.snapshot()==old,"duplicate selection mutated");
    check(s.select({"B1"})==SelectionError::UnknownMember && s.snapshot()==old,"opponent accepted");
    check(s.select({"missing"})==SelectionError::UnknownMember && s.snapshot()==old,"unknown member accepted");
    check(s.select({"A1","A2","A3","A4","A5","A6"})==SelectionError::TooMany && s.snapshot()==old,"oversized selection accepted");
}
void sizes_and_playability(){
    for(int count=1;count<=5;++count){TeamSelection s(catalog());std::vector<std::string> ids;
        for(int i=1;i<=count;++i){ids.push_back("A"+std::to_string(i));}
        check(s.select(ids)==SelectionError::None,"valid size rejected");const auto before=s.snapshot();const auto d=s.compose();
        check(d.roster.size()==static_cast<std::size_t>(count+1) && d.rules.skills.size()==d.roster.size() &&
            d.loadouts.size()==d.roster.size(),"composition left missing or orphan definitions");
        ProfileBattle a(d.roster,d.rules,CombatProfile::Team,d.loadouts),b(d.roster,d.rules,CombatProfile::Team,d.loadouts);
        while(a.battle().phase()==Phase::Running){const auto cmd=a.choose_automatic_action();check(cmd &&
            a.apply(*cmd)==b.apply(*b.choose_automatic_action()) && a.snapshot()==b.snapshot(),"selected team replay diverged");}
        check(a.battle().accepted_actions()<=10 && s.snapshot()==before,"battle leaked into selection/budget");}
}
void order_not_initiative(){
    TeamSelection a(catalog()),b(catalog());check(a.select({"A3","A1"})==SelectionError::None &&
        b.select({"A1","A3"})==SelectionError::None,"order setup failed");
    check(a.snapshot().selected.front()=="A3" && a.compose()==b.compose(),"selection order changed canonical battle data");
    const auto d=a.compose();ProfileBattle battle(d.roster,d.rules,CombatProfile::Team,d.loadouts);
    check(battle.battle().current_actor()->definition.id=="A1","selection order overrode initiative");
}
void prune_and_preserve_rules(){
    TeamSelection s(catalog());check(s.select({"A2"})==SelectionError::None,"prune setup failed");const auto d=s.compose();
    check(d.rules.action_budget==catalog().rules.action_budget && d.roster.back().id=="B1" && d.rules.skills[0].owner=="A2",
        "rules/opponents not preserved or skills not pruned");
    ProfileBattle b(d.roster,d.rules,CombatProfile::Team,d.loadouts);
    check(b.passive_activations().size()==1 && b.passive_activations()[0].owner=="A2" && !b.battle().find("A1"),"removed owner retained effects");
}
void isolation_and_later_selection(){
    TeamSelection s(catalog());check(s.select({"A1"})==SelectionError::None,"isolation setup failed");auto first=s.compose();
    ProfileBattle live(first.roster,first.rules,CombatProfile::Team,first.loadouts);const auto initial=live.snapshot();
    first.roster[0].attack=999;check(s.compose().roster[0].attack==5,"composition aliases selection");
    check(s.select({"A2"})==SelectionError::None && live.snapshot()==initial,"selection edited running battle");
    const auto next=s.compose();ProfileBattle other(next.roster,next.rules,CombatProfile::Team,next.loadouts);const auto untouched=other.snapshot();
    check(live.apply(*live.choose_automatic_action()).accepted() && other.snapshot()==untouched,"runtime leaked between encounters");
}
void invalid_catalog_and_side(){
    auto d=catalog();d.roster[0].id="B1";invalid([&]{TeamSelection s(d);});
    invalid([&]{TeamSelection s(catalog(),static_cast<Team>(99));});
    TeamSelection enemy(catalog(),Team::B);check(enemy.select({"B1"})==SelectionError::None &&
        enemy.select({"A1"})==SelectionError::UnknownMember,"controlled side ignored");
}
}
int main(int argc,char** argv){
    const std::pair<const char*,void(*)()> tests[]={{"default_detached",default_and_detached_catalog},{"invalid_atomic",invalid_atomic},
        {"sizes_playability",sizes_and_playability},{"order_initiative",order_not_initiative},{"prune_rules",prune_and_preserve_rules},
        {"isolation_later_selection",isolation_and_later_selection},{"invalid_catalog_side",invalid_catalog_and_side}};
    int passed=0;for(const auto& [name,test]:tests){try{test();++passed;std::cout<<"PASS team_selection "<<name<<'\n';}
        catch(const std::exception& e){std::cerr<<"FAIL team_selection "<<name<<": "<<e.what()<<'\n';}}
    std::cout<<"SELECTION_TESTS_RESULT passed="<<passed<<" total="<<std::size(tests)<<'\n';
    if(argc==2 && passed==static_cast<int>(std::size(tests))){std::ofstream out(argv[1]);out<<"test,status,scope\n";
        for(const auto& [name,test]:tests){static_cast<void>(test);out<<name<<",pass,in_memory_selection_1_to_5\n";}if(!out){return 2;}}
    return passed==static_cast<int>(std::size(tests))?0:1;
}
