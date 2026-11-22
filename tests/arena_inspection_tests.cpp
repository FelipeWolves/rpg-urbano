#include "rpg/reference_encounter.hpp"
#include "rpg/arena_layout.hpp"
#include "rpg/battle_presentation.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <source_location>
#include <stdexcept>
using namespace rpg;
using namespace rpg::presentation;
void check(bool ok,std::source_location where=std::source_location::current()){if(!ok){throw std::runtime_error("Inspection contract failed at line "+std::to_string(where.line()));}}
const InspectionFrame& find(const std::vector<InspectionFrame>& frames,const char* id){auto it=std::ranges::find(frames,id,&InspectionFrame::id);check(it!=frames.end());return *it;}
const Fighter& focus(const InspectionFrame& frame){auto it=std::ranges::find(frame.battle.fighters,frame.focus,[](const Fighter& f){return f.definition.id;});check(it!=frame.battle.fighters.end());return *it;}
const Status* shield(const InspectionFrame& frame){const auto& f=focus(frame);auto it=std::ranges::find(f.statuses,EffectKind::StandardShield,&Status::kind);return it==f.statuses.end()?nullptr:&*it;}
int main(){try{
    const auto frames=arena_inspection_frames();check(frames.size()==13);std::set<std::string> ids;for(const auto& f:frames){check(ids.insert(f.id).second && !f.notes.empty());(void)focus(f);}
    const std::pair<int,int> sizes[]={{1,1},{2,1},{3,3},{5,5},{1,5},{5,1}};
    for(std::size_t i=0;i<6;++i){const auto& f=frames[i];check(std::ranges::count(f.battle.fighters,Team::A,[](const Fighter& unit){return unit.definition.team;})==sizes[i].first);check(std::ranges::count(f.battle.fighters,Team::B,[](const Fighter& unit){return unit.definition.team;})==sizes[i].second);check(f.battle.accepted_actions==0);}
    for(const auto& frame:frames){std::vector<Definition> roster;for(const auto& f:frame.battle.fighters){roster.push_back(f.definition);}for(auto layout:{ArenaLayout::StandardDepth,ArenaLayout::ClassicLateral}){const auto placements=arena_placements(roster,layout);check(placements.size()==roster.size());std::set<std::pair<float,float>> positions;for(const auto& p:placements){check(std::isfinite(p.x) && std::isfinite(p.y) && positions.emplace(p.x,p.y).second);}}}
    check(!shield(find(frames,"ShieldBefore")));const auto& applied=find(frames,"ShieldApplied");check(shield(applied) && shield(applied)->magnitude==10 && shield(applied)->remaining==3 && focus(applied).health==50);const auto visual=shield_display(focus(applied));check(visual.points==10 && visual.overlay_fraction==.1 && visual.health_fraction==.5);
    const auto& consumed=find(frames,"ShieldConsumed");check(shield(consumed) && shield(consumed)->magnitude==2 && focus(consumed).health==50);
    const auto& reapplied=find(frames,"ShieldReapplied");check(shield(reapplied) && shield(reapplied)->magnitude==10 && shield(reapplied)->remaining==3);
    const auto& exhausted=find(frames,"ShieldExhausted");check(!shield(exhausted) && focus(exhausted).health==42 && shield_display(focus(exhausted)).overlay_fraction==0);
    const auto& expired=find(frames,"ShieldExpired");check(!shield(expired) && focus(expired).health==50);
    const auto& dead=find(frames,"ShieldDeath");check(!shield(dead) && focus(dead).health==0 && shield_display(focus(dead)).points==0);
    check(frames==arena_inspection_frames());check(team_reference_setup().roster==reference_roster() && classic_reference_setup().roster==reference_roster());
    for(const auto& frame:frames){const auto& f=focus(frame);std::cout<<"CASE,"<<frame.id<<','<<f.health<<','<<shield_display(f).points<<','<<(shield(frame)?shield(frame)->remaining:0)<<'\n';}
    std::cout<<"ARENA_INSPECTION_TESTS_RESULT passed=10 total=10 cases=13\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
