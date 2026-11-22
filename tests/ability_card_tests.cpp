#include "rpg/battle_presentation.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
using namespace rpg;
using namespace rpg::presentation;
void check(bool ok){if(!ok){throw std::runtime_error("Ability card contract failed");}}
VisualState state(const char* actor="A1"){auto setup=team_reference_setup();for(auto& f:setup.roster){if(f.id==actor){f.speed=1000;}}return VisualSession(setup).state();}
AbilityView& slot(VisualState& s,unsigned position){auto it=std::ranges::find(s.abilities,position,[](const AbilityView& a){return a.slot.position;});check(it!=s.abilities.end());return *it;}
int main(){try{
    auto v=state();auto c=ability_card(v,1);check(c.selectable && c.badge==AbilityBadge::Ready && c.kind=="BASICA" && c.cooldown==0);
    auto hybrid=state("A3");check(ability_card(hybrid,2).selectable && ability_card(hybrid,2).kind=="HABILIDADE");
    auto passive=state("A2");c=ability_card(passive,2);check(!c.selectable && c.badge==AbilityBadge::Passive && c.kind=="PASSIVA");slot(passive,2).passive_implemented=false;check(ability_card(passive,2).reason.find("não implementado")!=std::string::npos);
    auto cooldown=state();slot(cooldown,2).availability=AbilityAvailability::Cooldown;slot(cooldown,2).cooldown=2;c=ability_card(cooldown,2);check(!c.selectable && c.badge==AbilityBadge::Cooldown && c.cooldown==2);
    c=ability_card(hybrid,3);check(!c.selectable && c.badge==AbilityBadge::Locked && c.reason.find("50%")!=std::string::npos);
    slot(cooldown,2).availability=AbilityAvailability::Incapacitated;c=ability_card(cooldown,2);check(c.badge==AbilityBadge::Unavailable && !c.selectable && c.cooldown==2);auto blocked=state();auto it=std::ranges::find(blocked.combat.battle.fighters,std::string("A1"),[](const Fighter& f){return f.definition.id;});it->statuses.push_back({EffectKind::Block,1,TickPolicy::NaturalOnly,0});check(!ability_card(blocked,1).selectable);
    auto ended=state();ended.combat.battle.phase=Phase::Finished;check(!ability_card(ended,1).selectable);ended=state();ended.stopped=true;check(!ability_card(ended,1).selectable);
    auto opponent=state();opponent.automatic=true;check(!ability_card(opponent,1).selectable);opponent=state();opponent.combat.battle.current_actor="B1";check(!ability_card(opponent,1).selectable);
    check(!ability_card(v,9).selectable);auto missing=state();slot(missing,2).slot.active_skill="not-defined";check(!ability_card(missing,2).selectable);missing=state();missing.combat.profile=CombatProfile::Classic;check(!ability_card(missing,1).selectable);
    const auto before=v.combat;slot(v,1).slot.level=4;const auto details=ability_details(v,1);check(details[1]=="Level: 4" && std::ranges::any_of(details,[](const std::string& line){return line.find("Estado:")==0;}));v=state();(void)ability_card(v,1);(void)ability_details(v,1);check(v.combat==before);
    std::cout<<"ABILITY_CARD_TESTS_RESULT passed=10 total=10\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
