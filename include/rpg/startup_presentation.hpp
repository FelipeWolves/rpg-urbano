#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <functional>
namespace rpg::presentation {
struct PixelSize { std::uint32_t width; std::uint32_t height; };
struct Rect { double x; double y; double width; double height; };
using TextMeasure=std::function<double(std::string_view)>;
// UTF-8 words/newlines, measured by the caller's real font. No silent truncation.
[[nodiscard]] std::vector<std::string> wrap_text(std::string_view text,double width,const TextMeasure& measure);
struct PageSlice { std::size_t begin,end,page,pages; };
[[nodiscard]] PageSlice text_page(std::size_t lines,std::size_t capacity,std::size_t requested);
struct InfoPanelLayout { Rect box,body,previous,next,close; std::optional<Rect> action; std::size_t capacity; };
[[nodiscard]] std::optional<InfoPanelLayout> plan_info_panel(Rect safe,double line_height,bool action=false);
enum class UiScreen { Hub, Location, Options, Arena, Pause, AbandonConfirm, Result, TeamSetup };
enum class BackEffect { None, CloseDetails, CancelSelection, Transition };
class ScreenFlow {
public:
    explicit ScreenFlow(UiScreen start=UiScreen::Hub);
    [[nodiscard]] UiScreen screen() const noexcept {return screen_;}
    [[nodiscard]] bool in_hub() const noexcept;
    [[nodiscard]] bool accepts_actions(bool details=false) const noexcept {return screen_==UiScreen::Arena && !details;}
    [[nodiscard]] bool new_attempt_required() const noexcept {return new_attempt_;}
    bool open_location() noexcept;
    bool open_options() noexcept;
    bool open_team() noexcept;
    void acknowledge_new_attempt() noexcept {new_attempt_=false;}
    bool enter_arena(bool finished=false) noexcept;
    bool pause() noexcept;
    bool resume() noexcept;
    bool request_abandon() noexcept;
    bool confirm_abandon() noexcept;
    bool finish() noexcept;
    bool restart_result() noexcept;
    bool return_hub() noexcept;
    BackEffect back(bool details,bool selection) noexcept;
private:
    UiScreen screen_;
    bool new_attempt_=false;
};
enum class PointerSource { None, Mouse, Touch };
struct PointerSample { bool down=false; bool valid=false; double x=0; double y=0; };
struct CanvasMapping {
    double origin_x=0,origin_y=0,xx=1,xy=0,yx=0,yy=1;
};
struct Point2 { double x,y; };
[[nodiscard]] std::optional<Point2> viewport_to_canvas(double x,double y,const CanvasMapping& mapping) noexcept;
[[nodiscard]] std::optional<Point2> canvas_to_viewport(double x,double y,const CanvasMapping& mapping) noexcept;
[[nodiscard]] bool same_canvas_mapping(const CanvasMapping& a,const CanvasMapping& b) noexcept;
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
    void cancel_until_release() noexcept {owner_=PointerSource::None;blocked_=true;}
    [[nodiscard]] PointerFrame update(PointerSample mouse,PointerSample touch,bool focused,
        double seconds,PointerContext context);
private:
    PointerSource owner_=PointerSource::None;
    PointerContext context_;
    bool previous_mouse_=false,previous_touch_=false,blocked_=false;
    double started_=0,last_time_=0,origin_x_=0,origin_y_=0,touch_guard_=0;
};
[[nodiscard]] bool pointer_clicked(const PointerFrame& frame,const Rect& target) noexcept;
struct TargetHitRegion { std::string id;Rect bounds;double depth=0;bool label=false; };
[[nodiscard]] bool canvas_input_active(bool window_foreground,bool viewport_widget_focused) noexcept;
[[nodiscard]] std::optional<std::string> pick_target(const PointerFrame& input,const std::vector<TargetHitRegion>& regions);
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
struct PresentationPreferences {
    PresentationMode mode=PresentationMode::Auto;
    bool reduced_motion=false;
    bool high_contrast=false;
    std::uint32_t text_percent=100;
    HubArea area=HubArea::Center;
    HubView view=HubView::Auto;
    bool operator==(const PresentationPreferences&) const = default;
};
enum class PreferenceSource { Defaults, Primary, Backup };
struct PreferenceRead { PresentationPreferences values; PreferenceSource source; bool primary_invalid; };
[[nodiscard]] std::string encode_preferences(const PresentationPreferences& values);
[[nodiscard]] PresentationPreferences decode_preferences(std::string_view data);
[[nodiscard]] PreferenceRead read_preferences(std::optional<std::string_view> primary,
    std::optional<std::string_view> backup,PresentationPreferences defaults={});
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
// Local media lifecycle only. The backend resolves validated relative assets in AUT-11.
enum class MediaState { Idle, Preparing, Playing, Paused, Completed, Skipped, Failed, Cancelled };
enum class MediaFailure { None, MissingAsset, InvalidAsset, OpenFailed, Timeout, DecodeFailed };
class MediaStartGate {
public:
    void reset() noexcept {next_attempt_=0;attempts_=0;}
    [[nodiscard]] bool should_request(MediaState state,bool ready,bool playing,bool foreground,double seconds) noexcept;
    [[nodiscard]] std::uint32_t attempts()const noexcept{return attempts_;}
private:
    double next_attempt_=0;
    std::uint32_t attempts_=0;
};
class MediaFlow {
public:
    std::uint64_t begin(std::optional<Asset> asset,bool previously_watched=false);
    [[nodiscard]] MediaState state() const noexcept {return state_;}
    [[nodiscard]] MediaFailure failure() const noexcept {return failure_;}
    [[nodiscard]] bool watched() const noexcept {return watched_;}
    [[nodiscard]] bool return_to_hub() const noexcept;
    [[nodiscard]] bool accepts_gameplay() const noexcept {return state_==MediaState::Idle || return_to_hub();}
    bool prepared(std::uint64_t ticket) noexcept;
    bool ended(std::uint64_t ticket) noexcept;
    bool fail(std::uint64_t ticket,MediaFailure reason) noexcept;
    bool pause() noexcept;
    bool resume() noexcept;
    void background(bool hidden) noexcept;
    bool skip(std::uint64_t held_milliseconds) noexcept;
    bool cancel() noexcept;
    [[nodiscard]] std::optional<VideoPlan> layout(PixelSize viewport,Device device,bool allow_rotation=true) const;
private:
    std::uint64_t ticket_=0;
    MediaState state_=MediaState::Idle;
    MediaFailure failure_=MediaFailure::None;
    std::optional<Asset> asset_;
    bool watched_=false,background_=false;
    [[nodiscard]] bool active() const noexcept;
};
} // namespace rpg::presentation
