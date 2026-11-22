#pragma once
#include "rpg/battle.hpp"
#include <optional>
#include <string_view>
namespace rpg::presentation {
// Presentation only; neither layout selects combat rules nor changes roster order.
enum class ArenaLayout { StandardDepth, ClassicLateral };
inline constexpr ArenaLayout default_arena_layout=ArenaLayout::StandardDepth;
struct ArenaPlacement {
    std::string fighter_id;
    Team side;
    float x;
    float y;
    float z;
    float facing_yaw;
    bool operator==(const ArenaPlacement&) const = default;
};
[[nodiscard]] std::optional<ArenaLayout> parse_arena_layout(std::string_view name);
[[nodiscard]] std::vector<ArenaPlacement> arena_placements(const std::vector<Definition>& roster,
    ArenaLayout layout=default_arena_layout);
struct MarkerEdge { float x,y,length,yaw; };
// Local XY geometry, anchored at feet by the engine adapter. No debug renderer.
[[nodiscard]] std::vector<MarkerEdge> marker_edges(float radius,std::uint32_t segments);
} // namespace rpg::presentation
