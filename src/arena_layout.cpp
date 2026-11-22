#include "rpg/arena_layout.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>
namespace rpg::presentation {
std::optional<ArenaLayout> parse_arena_layout(std::string_view name){
    if(name=="Standard"){return ArenaLayout::StandardDepth;}
    if(name=="Classic"){return ArenaLayout::ClassicLateral;}
    return std::nullopt;
}
std::vector<ArenaPlacement> arena_placements(const std::vector<Definition>& roster,ArenaLayout layout){
    if(layout!=ArenaLayout::StandardDepth && layout!=ArenaLayout::ClassicLateral){throw std::invalid_argument("Unknown arena layout");}
    std::array<std::size_t,2> counts{};
    std::vector<std::string> ids;
    for(const auto& fighter:roster){
        if((fighter.team!=Team::A && fighter.team!=Team::B) || fighter.id.empty() ||
           std::ranges::find(ids,fighter.id)!=ids.end()){throw std::invalid_argument("Invalid arena roster identity");}
        ++counts[fighter.team==Team::A?0:1];ids.push_back(fighter.id);
    }
    if(counts[0]<1 || counts[0]>5 || counts[1]<1 || counts[1]>5){throw std::invalid_argument("Arena supports 1..5 units per side");}
    constexpr std::array<float,5> offsets={0.f,260.f,-260.f,140.f,-140.f};
    constexpr std::array<float,5> depths={520.f,400.f,380.f,180.f,160.f};
    std::array<std::size_t,2> index{};
    std::vector<ArenaPlacement> result;result.reserve(roster.size());
    for(const auto& fighter:roster){
        const bool ally=fighter.team==Team::A;
        const std::size_t side=ally?0:1;
        const auto slot=index[side]++;
        if(layout==ArenaLayout::ClassicLateral){
            // Exact three-unit positions of the previously observed lateral scene.
            result.push_back({fighter.id,fighter.team,ally?230.f:-230.f,
                (static_cast<float>(slot)-(static_cast<float>(counts[side])-1.f)/2.f)*170.f,60.f,ally?180.f:0.f});
        }else{
            // Staggered fan: three foreground slots and two further-forward slots.
            const float depth=depths[slot];
            result.push_back({fighter.id,fighter.team,ally?offsets[slot]:-offsets[slot],
                ally?-depth:depth,60.f,ally?90.f:-90.f});
        }
    }
    return result;
}
std::vector<ArenaPlacement> shared_arena_placements(const SharedBossSnapshot& s){
 if(s.squad_ids.size()!=3 || s.members.size()!=3 || s.boss_id.empty()){throw std::invalid_argument("Invalid shared visual formation");}
 constexpr std::array<float,5> offsets={0.f,75.f,-75.f,40.f,-40.f};constexpr std::array<float,5> depths={500.f,400.f,380.f,240.f,220.f};
 std::vector<ArenaPlacement> out;std::vector<std::string> seen;
 for(std::size_t i=0;i<3;++i){if(s.members[i].empty()||s.members[i].size()>5){throw std::invalid_argument("Shared visual front supports 1..5");}for(std::size_t j=0;j<s.members[i].size();++j){
  const auto& id=s.members[i][j];const auto f=std::ranges::find(s.battle.fighters,id,[](const Fighter& unit){return unit.definition.id;});
  if(f==s.battle.fighters.end()||f->definition.team!=Team::A||std::ranges::find(seen,id)!=seen.end()){throw std::invalid_argument("Invalid shared visual member");}seen.push_back(id);out.push_back({id,Team::A,(static_cast<float>(i)-1)*340+offsets[j],-depths[j],60,90});
 }}
 const auto boss=std::ranges::find(s.battle.fighters,s.boss_id,[](const Fighter& unit){return unit.definition.id;});if(boss==s.battle.fighters.end()||boss->definition.team!=Team::B){throw std::invalid_argument("Missing shared boss");}
 out.push_back({s.boss_id,Team::B,0,600,60,-90});return out;
}
float marker_radius(float x,float y) noexcept {
    if(!std::isfinite(x) || !std::isfinite(y) || x<=0 || y<=0){return 75.f;}
    return std::clamp(std::max(x,y),1.f,10000.f/1.5f)*1.5f;
}
std::vector<MarkerEdge> marker_edges(float radius,std::uint32_t segments){
    if(!std::isfinite(radius) || radius<=0 || radius>10000 || segments<3 || segments>64){throw std::invalid_argument("Invalid target marker geometry");}
    constexpr double pi=3.14159265358979323846;
    std::vector<MarkerEdge> result;result.reserve(segments);
    for(std::uint32_t i=0;i<segments;++i){
        const double a=2*pi*i/segments,b=2*pi*(i+1)/segments;
        const double x1=radius*std::cos(a),y1=radius*std::sin(a),x2=radius*std::cos(b),y2=radius*std::sin(b);
        result.push_back({static_cast<float>((x1+x2)/2),static_cast<float>((y1+y2)/2),static_cast<float>(std::hypot(x2-x1,y2-y1)),static_cast<float>(std::atan2(y2-y1,x2-x1)*180/pi)});
    }
    return result;
}
} // namespace rpg::presentation
