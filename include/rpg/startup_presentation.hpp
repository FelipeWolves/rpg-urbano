#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
namespace rpg::presentation {
struct PixelSize { std::uint32_t width; std::uint32_t height; };
struct Rect { double x; double y; double width; double height; };
enum class Device { Desktop, Mobile };
enum class HubArea { Left, Center, Right };
enum class HubView { Auto, Focused, Panoramic };
enum class Orientation { Keep, Portrait, Landscape };
enum class WindowMode { Windowed, Fullscreen };
struct StartupDisplay {
    PixelSize resolution{1280,720};
    WindowMode mode=WindowMode::Windowed;
};
struct Asset {
    std::string id;
    std::string relative_path;
    PixelSize size;
};
struct Variants { std::optional<Asset> desktop; std::optional<Asset> mobile; };
struct VideoPlan {
    Rect content;
    Rect left;
    Rect right;
    Rect top;
    Rect bottom;
    Orientation request;
    bool use_side_art;
};
struct HubRegions {
    Rect left{0,0,1.0/3,1};
    Rect center{1.0/3,0,1.0/3,1};
    Rect right{2.0/3,0,1.0/3,1};
};
struct HubPlan {
    HubView view;
    Rect source_uv;
    Rect content;
};
enum class HubRoute { SpecialEvent, Battle, Terminal, Unavailable };
enum class PresentationMode { Auto, Desktop, Mobile };
[[nodiscard]] std::optional<PresentationMode> parse_presentation_mode(std::string_view name) noexcept;
[[nodiscard]] Device presentation_device(PresentationMode mode,Device actual);
struct HubHotspot {
    std::string id;
    std::string label;
    HubRoute route;
    double anchor_x;
    double anchor_y;
    Rect source_area;
    bool visible=true;
};
struct ProjectedHotspot {
    Rect hit_area;
    double label_x;
    double label_y;
};
void validate_hotspot(const HubHotspot& hotspot);
[[nodiscard]] std::optional<ProjectedHotspot> project_hotspot(const HubPlan& plan,const HubHotspot& hotspot);
[[nodiscard]] bool contains(const Rect& rect,double x,double y) noexcept;
// Geometry/policy only: never opens a file, window, player, account or platform service.
[[nodiscard]] Rect contain(PixelSize media,PixelSize viewport);
[[nodiscard]] HubPlan plan_hub(PixelSize panorama,PixelSize viewport,Device device,
    HubArea area=HubArea::Center,HubView preference=HubView::Auto,
    double panoramic_min_aspect=1.5,const HubRegions& regions={});
[[nodiscard]] VideoPlan plan_video(PixelSize video,PixelSize viewport,Device device,
    bool orientation_change_allowed=true);
[[nodiscard]] const Asset* select_variant(const Variants& assets,Device device) noexcept;
[[nodiscard]] bool valid_asset_path(std::string_view path) noexcept;
void validate_asset(const Asset& asset);
void validate_display(const StartupDisplay& display);
} // namespace rpg::presentation
