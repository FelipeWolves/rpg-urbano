#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
namespace rpg::presentation {
struct PixelSize { std::uint32_t width; std::uint32_t height; };
struct Rect { double x; double y; double width; double height; };
enum class PointerSource { None, Mouse, Touch };
struct PointerSample { bool down=false; bool valid=false; double x=0; double y=0; };
struct PointerContext { std::uint64_t revision=0; std::uint8_t screen=0; bool operator==(const PointerContext&) const = default; };
struct PointerFrame {
    PointerSource source=PointerSource::None;
    double x=0,y=0,origin_x=0,origin_y=0;
    bool pressed=false,down=false,released=false,tap=false,cancelled=false;
};
// One owner per gesture. Touch wins simultaneous starts; suppresses synthetic mouse
// until a fresh mouse press after a short guard. Cancelled gestures require all-up.
class PointerInput {
public:
    [[nodiscard]] PointerFrame update(PointerSample mouse,PointerSample touch,bool focused,
        double seconds,PointerContext context);
private:
    PointerSource owner_=PointerSource::None;
    PointerContext context_;
    bool previous_mouse_=false,previous_touch_=false,blocked_=false;
    double started_=0,last_time_=0,origin_x_=0,origin_y_=0,touch_guard_=0;
};
[[nodiscard]] bool pointer_clicked(const PointerFrame& frame,const Rect& target) noexcept;
struct SafeInsets { double left=0; double top=0; double right=0; double bottom=0; };
struct HubFrame {
    Rect safe;
    Rect header;
    Rect scene;
    std::vector<Rect> destinations;
    std::vector<Rect> navigation;
};
// Pure pixel geometry; no platform notch discovery or input handling.
// Valid but insufficient space returns nullopt; invalid metadata throws.
[[nodiscard]] std::optional<HubFrame> plan_hub_frame(PixelSize viewport,std::uint32_t destinations,
    SafeInsets insets={});
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
