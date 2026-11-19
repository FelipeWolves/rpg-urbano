#include "rpg/arena_layout.hpp"
#include <algorithm>
#include <array>
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
} // namespace rpg::presentation
