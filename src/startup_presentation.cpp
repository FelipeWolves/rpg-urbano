#include "rpg/startup_presentation.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <locale>
#include <limits>
namespace rpg::presentation {
namespace {
void valid_size(PixelSize size){
    if(!size.width || !size.height){throw std::invalid_argument("Positive pixel dimensions required");}
}
void valid_device(Device device){
    if(device!=Device::Desktop && device!=Device::Mobile){throw std::invalid_argument("Unknown device profile");}
}
Rect fit(double width,double height,PixelSize viewport){
    valid_size(viewport);
    const double scale=std::min(viewport.width/width,viewport.height/height);
    const double w=width*scale,h=height*scale;
    return {(viewport.width-w)/2,(viewport.height-h)/2,w,h};
}
}
ScreenFlow::ScreenFlow(UiScreen start):screen_(start){if(start!=UiScreen::Hub && start!=UiScreen::Arena){throw std::invalid_argument("Invalid initial screen");}}
bool ScreenFlow::in_hub() const noexcept {return screen_==UiScreen::Hub || screen_==UiScreen::Location || screen_==UiScreen::Options || screen_==UiScreen::TeamSetup;}
bool ScreenFlow::open_location() noexcept {if(screen_!=UiScreen::Hub){return false;}screen_=UiScreen::Location;return true;}
bool ScreenFlow::open_team() noexcept {if(screen_!=UiScreen::Hub){return false;}screen_=UiScreen::TeamSetup;return true;}
bool ScreenFlow::open_options() noexcept {if(screen_!=UiScreen::Hub){return false;}screen_=UiScreen::Options;return true;}
bool ScreenFlow::enter_arena(bool finished) noexcept {if(screen_!=UiScreen::Hub && screen_!=UiScreen::Location){return false;}screen_=finished?UiScreen::Result:UiScreen::Arena;new_attempt_=false;return true;}
bool ScreenFlow::pause() noexcept {if(screen_!=UiScreen::Arena){return false;}screen_=UiScreen::Pause;return true;}
bool ScreenFlow::resume() noexcept {if(screen_!=UiScreen::Pause){return false;}screen_=UiScreen::Arena;return true;}
bool ScreenFlow::request_abandon() noexcept {if(screen_!=UiScreen::Pause){return false;}screen_=UiScreen::AbandonConfirm;return true;}
bool ScreenFlow::confirm_abandon() noexcept {if(screen_!=UiScreen::AbandonConfirm){return false;}screen_=UiScreen::Hub;new_attempt_=true;return true;}
bool ScreenFlow::finish() noexcept {if(screen_!=UiScreen::Arena){return false;}screen_=UiScreen::Result;return true;}
bool ScreenFlow::restart_result() noexcept {if(screen_!=UiScreen::Result){return false;}screen_=UiScreen::Arena;return true;}
bool ScreenFlow::return_hub() noexcept {if(screen_!=UiScreen::Result){return false;}screen_=UiScreen::Hub;return true;}
BackEffect ScreenFlow::back(bool details,bool selection) noexcept {
    if(details){return BackEffect::CloseDetails;}
    if(selection){return BackEffect::CancelSelection;}
    switch(screen_){
    case UiScreen::Hub:return BackEffect::None;
    case UiScreen::TeamSetup:case UiScreen::Location:case UiScreen::Options:case UiScreen::Result:screen_=UiScreen::Hub;break;
    case UiScreen::Arena:screen_=UiScreen::Pause;break;
    case UiScreen::Pause:screen_=UiScreen::Arena;break;
    case UiScreen::AbandonConfirm:screen_=UiScreen::Pause;break;
    }
    return BackEffect::Transition;
}
std::vector<std::string> wrap_text(std::string_view text,double width,const TextMeasure& measure){
    if(!std::isfinite(width) || width<=0 || !measure || text.size()>1024*1024){throw std::invalid_argument("Invalid text layout request");}
    std::vector<std::string_view> scalars;
    for(std::size_t i=0;i<text.size();){
        const auto first=static_cast<unsigned char>(text[i]);
        const std::size_t n=first<0x80?1:first>=0xC2 && first<=0xDF?2:first>=0xE0 && first<=0xEF?3:first>=0xF0 && first<=0xF4?4:0;
        if(!n || n>text.size()-i){throw std::invalid_argument("Invalid UTF-8 text");}
        std::uint32_t value=first & (n==1?0x7FU:n==2?0x1FU:n==3?0x0FU:0x07U);
        for(std::size_t j=1;j<n;++j){const auto b=static_cast<unsigned char>(text[i+j]);if((b&0xC0)!=0x80){throw std::invalid_argument("Invalid UTF-8 continuation");}value=(value<<6)|(b&0x3F);}
        if((n==2 && value<0x80) || (n==3 && value<0x800) || (n==4 && value<0x10000) || value>0x10FFFF || (value>=0xD800 && value<=0xDFFF) || value==0){throw std::invalid_argument("Invalid Unicode scalar");}
        scalars.push_back(text.substr(i,n));i+=n;
    }
    const auto fits=[&](std::string_view value){const double size=measure(value);if(!std::isfinite(size) || size<0){throw std::invalid_argument("Invalid font measurement");}return size<=width;};
    std::vector<std::string> lines;std::string line,word;
    const auto flushWord=[&](){
        if(word.empty()){return;}
        const auto candidate=line.empty()?word:line+" "+word;
        if(fits(candidate)){line=candidate;word.clear();return;}
        if(!line.empty()){lines.push_back(line);line.clear();}
        for(std::size_t i=0;i<word.size();){
            const auto first=static_cast<unsigned char>(word[i]);const std::size_t n=first<0x80?1:first<0xE0?2:first<0xF0?3:4;
            const auto scalar=word.substr(i,n);
            if(!fits(line+scalar)){
                if(line.empty() || !fits(scalar)){throw std::invalid_argument("Panel too narrow for a glyph");}
                lines.push_back(line);line=scalar;
            }else{line+=scalar;}
            i+=n;
        }
        word.clear();
    };
    bool afterCr=false;
    for(const auto scalar:scalars){
        if(scalar=="\n" && afterCr){afterCr=false;continue;}
        afterCr=false;
        if(scalar=="\n" || scalar=="\r"){flushWord();lines.push_back(line);line.clear();afterCr=scalar=="\r";}
        else if(scalar==" " || scalar=="\t"){flushWord();}
        else{word+=scalar;}
    }
    flushWord();lines.push_back(line);return lines;
}
PageSlice text_page(std::size_t lines,std::size_t capacity,std::size_t requested){
    if(!capacity){throw std::invalid_argument("Positive page capacity required");}
    const auto pages=std::max<std::size_t>(1,lines/capacity+(lines%capacity!=0));
    const auto page=std::min(requested,pages-1),begin=page*capacity;
    return {begin,begin+std::min(capacity,lines-begin),page,pages};
}
std::optional<InfoPanelLayout> plan_info_panel(Rect safe,double line_height,bool action){
    for(double value:{safe.x,safe.y,safe.width,safe.height,line_height}){if(!std::isfinite(value)){throw std::invalid_argument("Finite panel geometry required");}}
    if(safe.width<=0 || safe.height<=0 || line_height<1){throw std::invalid_argument("Positive panel geometry required");}
    const double width=std::min(760.0,safe.width-24),height=std::min(520.0,safe.height-24);
    const double footer=action?112:56;
    if(width<256 || height<line_height+footer+32){return {};}
    const Rect box{safe.x+(safe.width-width)/2,safe.y+(safe.height-height)/2,width,height};
    const Rect body{box.x+16,box.y+16,width-32,height-footer-24};
    const auto capacity=static_cast<std::size_t>(std::floor(body.height/line_height));
    if(!capacity){return {};}
    const double cell=(width-40)/3,y=box.y+height-56;
    InfoPanelLayout layout{box,body,{box.x+16,y,cell,48},{box.x+20+cell,y,cell,48},{box.x+24+2*cell,y,cell,48},{},capacity};
    if(action){layout.action=Rect{box.x+16,y-56,width-32,48};}
    return layout;
}
namespace {
bool valid_mapping(const CanvasMapping& m) noexcept {
    for(double v:{m.origin_x,m.origin_y,m.xx,m.xy,m.yx,m.yy}){if(!std::isfinite(v)){return false;}}
    const double det=m.xx*m.yy-m.xy*m.yx;return std::isfinite(det) && std::abs(det)>1e-12;
}
}
std::optional<Point2> viewport_to_canvas(double x,double y,const CanvasMapping& m) noexcept {
    if(!valid_mapping(m) || !std::isfinite(x) || !std::isfinite(y)){return {};}
    x-=m.origin_x;y-=m.origin_y;const double det=m.xx*m.yy-m.xy*m.yx;
    const Point2 result{(m.yy*x-m.xy*y)/det,(-m.yx*x+m.xx*y)/det};
    if(!std::isfinite(result.x) || !std::isfinite(result.y)){return {};}return result;
}
std::optional<Point2> canvas_to_viewport(double x,double y,const CanvasMapping& m) noexcept {
    if(!valid_mapping(m) || !std::isfinite(x) || !std::isfinite(y)){return {};}
    const Point2 result{m.origin_x+m.xx*x+m.xy*y,m.origin_y+m.yx*x+m.yy*y};
    if(!std::isfinite(result.x) || !std::isfinite(result.y)){return {};}return result;
}
bool same_canvas_mapping(const CanvasMapping& a,const CanvasMapping& b) noexcept {
    if(!valid_mapping(a) || !valid_mapping(b)){return false;}
    return std::abs(a.origin_x-b.origin_x)<1e-6 && std::abs(a.origin_y-b.origin_y)<1e-6 &&
        std::abs(a.xx-b.xx)<1e-6 && std::abs(a.xy-b.xy)<1e-6 && std::abs(a.yx-b.yx)<1e-6 && std::abs(a.yy-b.yy)<1e-6;
}
PointerFrame PointerInput::update(PointerSample mouse,PointerSample touch,bool focused,double seconds,PointerContext context){
    PointerFrame frame;
    const bool mouseStart=mouse.down && !previous_mouse_,touchStart=touch.down && !previous_touch_;
    if(!touch.down && previous_touch_ && std::isfinite(seconds) && seconds>=0){touch_guard_=std::max(seconds,last_time_)+0.35;}
    previous_mouse_=mouse.down;previous_touch_=touch.down;
    const auto valid=[](const PointerSample& p){return p.valid && std::isfinite(p.x) && std::isfinite(p.y);};
    if(valid(mouse)){frame.x=mouse.x;frame.y=mouse.y;}
    const auto cancel=[&](){
        frame.cancelled=owner_!=PointerSource::None;
        owner_=PointerSource::None;blocked_=mouse.down || touch.down;
    };
    if(!focused){cancel();blocked_=true;return frame;}
    if(!std::isfinite(seconds) || seconds<last_time_){cancel();return frame;}
    last_time_=seconds;
    if(blocked_){if(!mouse.down && !touch.down){blocked_=false;}return frame;}
    if(owner_!=PointerSource::None && context!=context_){cancel();return frame;}
    if(touchStart && valid(touch)){
        if(owner_==PointerSource::Mouse){frame.cancelled=true;}
        owner_=PointerSource::Touch;context_=context;started_=seconds;moved_=false;origin_x_=touch.x;origin_y_=touch.y;frame.pressed=true;
    }else if(owner_==PointerSource::None && mouseStart && !touch.down && seconds>=touch_guard_ && valid(mouse)){
        owner_=PointerSource::Mouse;context_=context;started_=seconds;moved_=false;origin_x_=mouse.x;origin_y_=mouse.y;frame.pressed=true;
    }
    if(owner_==PointerSource::None){return frame;}
    const auto sample=owner_==PointerSource::Touch?touch:mouse;
    frame.source=owner_;frame.x=sample.x;frame.y=sample.y;frame.origin_x=origin_x_;frame.origin_y=origin_y_;
    moved_=moved_ || std::hypot(sample.x-origin_x_,sample.y-origin_y_)>12;
    if(!valid(sample) || (moved_ && !context.allow_drag)){
        if(owner_==PointerSource::Touch){touch_guard_=seconds+0.35;}
        cancel();return frame;
    }
    frame.down=sample.down;
    if(!sample.down){
        frame.released=true;frame.tap=!moved_ && seconds-started_<0.5;
        if(owner_==PointerSource::Touch){touch_guard_=seconds+0.35;}
        owner_=PointerSource::None;
    }
    return frame;
}
bool pointer_clicked(const PointerFrame& frame,const Rect& target) noexcept {
    return frame.tap && !frame.cancelled && contains(target,frame.origin_x,frame.origin_y) && contains(target,frame.x,frame.y);
}
bool canvas_input_active(bool window_foreground,[[maybe_unused]] bool viewport_widget_focused) noexcept {
    // Canvas has no focusable Slate control. Internal widget focus must not eat a fresh click.
    return window_foreground;
}
std::optional<std::string> pick_target(const PointerFrame& input,const std::vector<TargetHitRegion>& regions){
    const TargetHitRegion* chosen=nullptr;
    for(const auto& region:regions){
        if(region.id.empty() || !std::isfinite(region.depth) || region.depth<0 || !pointer_clicked(input,region.bounds)){continue;}
        if(!chosen || (region.label&&!chosen->label) || (region.label==chosen->label && (region.depth<chosen->depth || (region.depth==chosen->depth && region.id<chosen->id)))){chosen=&region;}
    }
    return chosen?std::optional<std::string>{chosen->id}:std::nullopt;
}
std::optional<HubFrame> plan_hub_frame(PixelSize viewport,std::uint32_t count,SafeInsets insets){
    valid_size(viewport);
    if(count>16){throw std::invalid_argument("At most 16 hub destinations supported");}
    for(double value:{insets.left,insets.top,insets.right,insets.bottom}){
        if(!std::isfinite(value) || value<0){throw std::invalid_argument("Finite nonnegative safe margins required");}
    }
    const double width=viewport.width-insets.left-insets.right;
    const double height=viewport.height-insets.top-insets.bottom;
    if(width<280 || height<360){return {};}
    const std::uint32_t columns=std::min(count==0?1U:count,width<600?3U:7U);
    const std::uint32_t rows=(count+columns-1)/columns;
    const double sceneHeight=height-60-52-static_cast<double>(rows)*52;
    if(sceneHeight<64){return {};}
    HubFrame frame{{insets.left,insets.top,width,height},
        {insets.left,insets.top,width,56},
        {insets.left,insets.top+60,width,sceneHeight},{},{}};
    const double dockTop=insets.top+height-52-static_cast<double>(rows)*52;
    const double cell=width/columns;
    for(std::uint32_t i=0;i<count;++i){
        frame.destinations.push_back({insets.left+(i%columns)*cell+3,dockTop+(i/columns)*52,cell-6,48});
    }
    const double navWidth=(width-24)/5;
    for(std::uint32_t i=0;i<5;++i){
        frame.navigation.push_back({insets.left+4+i*(navWidth+4),insets.top+height-48,navWidth,48});
    }
    return frame;
}
Rect contain(PixelSize media,PixelSize viewport){
    valid_size(media);return fit(media.width,media.height,viewport);
}
std::string encode_preferences(const PresentationPreferences& p){
    if((p.mode!=PresentationMode::Auto && p.mode!=PresentationMode::Desktop && p.mode!=PresentationMode::Mobile) ||
       (p.text_percent!=100 && p.text_percent!=125 && p.text_percent!=150) ||
       (p.area!=HubArea::Left && p.area!=HubArea::Center && p.area!=HubArea::Right) ||
       (p.view!=HubView::Auto && p.view!=HubView::Focused && p.view!=HubView::Panoramic)){
        throw std::invalid_argument("Invalid presentation preferences");
    }
    std::ostringstream out;out.imbue(std::locale::classic());
    out<<"RPG_PRESENTATION 1\nmode "<<(p.mode==PresentationMode::Auto?"Auto":p.mode==PresentationMode::Desktop?"Desktop":"Mobile")
       <<"\nmotion "<<(p.reduced_motion?1:0)<<"\ncontrast "<<(p.high_contrast?1:0)<<"\ntext "<<p.text_percent
       <<"\narea "<<(p.area==HubArea::Left?"Left":p.area==HubArea::Center?"Center":"Right")
       <<"\nview "<<(p.view==HubView::Auto?"Auto":p.view==HubView::Focused?"Focused":"Panoramic")<<"\nend\n";
    return out.str();
}
PresentationPreferences decode_preferences(std::string_view data){
    if(data.empty() || data.size()>4096){throw std::invalid_argument("Invalid preferences size");}
    std::istringstream in(std::string{data});in.imbue(std::locale::classic());
    const auto token=[&](){std::string value;if(!(in>>value)){throw std::invalid_argument("Truncated preferences");}return value;};
    const auto expect=[&](std::string_view key){if(token()!=key){throw std::invalid_argument("Unexpected preference field");}};
    expect("RPG_PRESENTATION");expect("1");PresentationPreferences p;
    expect("mode");const auto mode=parse_presentation_mode(token());if(!mode){throw std::invalid_argument("Invalid mode");}p.mode=*mode;
    const auto boolean=[&](){const auto value=token();if(value!="0" && value!="1"){throw std::invalid_argument("Invalid boolean");}return value=="1";};
    expect("motion");p.reduced_motion=boolean();expect("contrast");p.high_contrast=boolean();
    expect("text");const auto scale=token();if(scale!="100" && scale!="125" && scale!="150"){throw std::invalid_argument("Invalid text scale");}p.text_percent=scale=="100"?100:scale=="125"?125:150;
    expect("area");const auto area=token();if(area!="Left" && area!="Center" && area!="Right"){throw std::invalid_argument("Invalid area");}p.area=area=="Left"?HubArea::Left:area=="Center"?HubArea::Center:HubArea::Right;
    expect("view");const auto view=token();if(view!="Auto" && view!="Focused" && view!="Panoramic"){throw std::invalid_argument("Invalid view");}p.view=view=="Auto"?HubView::Auto:view=="Focused"?HubView::Focused:HubView::Panoramic;
    expect("end");std::string extra;if(in>>extra){throw std::invalid_argument("Trailing preferences data");}
    return p;
}
PreferenceRead read_preferences(std::optional<std::string_view> primary,std::optional<std::string_view> backup,PresentationPreferences defaults){
    (void)encode_preferences(defaults);
    bool invalid=false;
    if(primary){try{return {decode_preferences(*primary),PreferenceSource::Primary,false};}catch(const std::invalid_argument&){invalid=true;}}
    if(backup){try{return {decode_preferences(*backup),PreferenceSource::Backup,invalid};}catch(const std::invalid_argument&){}}
    return {defaults,PreferenceSource::Defaults,invalid};
}
std::optional<PresentationMode> parse_presentation_mode(std::string_view name) noexcept {
    if(name=="Auto"){return PresentationMode::Auto;}
    if(name=="Desktop"){return PresentationMode::Desktop;}
    if(name=="Mobile"){return PresentationMode::Mobile;}
    return {};
}
Device presentation_device(PresentationMode mode,Device actual){
    valid_device(actual);
    if(mode==PresentationMode::Auto){return actual;}
    if(mode==PresentationMode::Desktop){return Device::Desktop;}
    if(mode==PresentationMode::Mobile){return Device::Mobile;}
    throw std::invalid_argument("Unknown presentation mode");
}
HubPlan plan_hub(PixelSize panorama,PixelSize viewport,Device device,HubArea area,
                 HubView preference,double panoramic_min_aspect,const HubRegions& regions){
    valid_size(panorama);valid_size(viewport);valid_device(device);
    if(!std::isfinite(panoramic_min_aspect) || panoramic_min_aspect<=0 ||
       (preference!=HubView::Auto && preference!=HubView::Focused && preference!=HubView::Panoramic)){
        throw std::invalid_argument("Invalid hub framing policy");
    }
    const int index=area==HubArea::Left?0:area==HubArea::Center?1:area==HubArea::Right?2:-1;
    if(index<0){throw std::invalid_argument("Unknown hub area");}
    const double aspect=static_cast<double>(viewport.width)/viewport.height;
    const auto view=preference==HubView::Auto?
        (device==Device::Desktop && aspect>=panoramic_min_aspect?HubView::Panoramic:HubView::Focused):preference;
    const Rect selected=index==0?regions.left:index==1?regions.center:regions.right;
    for(const auto& region:{regions.left,regions.center,regions.right}){
        if(!std::isfinite(region.x) || !std::isfinite(region.y) || !std::isfinite(region.width) || !std::isfinite(region.height) ||
           region.x<0 || region.y<0 || region.width<=0 || region.height<=0 || region.x+region.width>1 || region.y+region.height>1){
            throw std::invalid_argument("Hub region outside image");
        }
    }
    const Rect uv=view==HubView::Panoramic?Rect{0,0,1,1}:selected;
    return {view,uv,fit(panorama.width*uv.width,panorama.height*uv.height,viewport)};
}
VideoPlan plan_video(PixelSize video,PixelSize viewport,Device device,bool orientation_change_allowed){
    valid_size(video);valid_size(viewport);valid_device(device);
    const auto content=contain(video,viewport);
    auto request=Orientation::Keep;
    if(device==Device::Mobile && orientation_change_allowed){
        if(video.width>video.height && viewport.width<viewport.height){request=Orientation::Landscape;}
        else if(video.width<video.height && viewport.width>viewport.height){request=Orientation::Portrait;}
    }
    const Rect left{0,0,content.x,static_cast<double>(viewport.height)};
    const Rect right{content.x+content.width,0,content.x,static_cast<double>(viewport.height)};
    const Rect top{content.x,0,content.width,content.y};
    const Rect bottom{content.x,content.y+content.height,content.width,content.y};
    return {content,left,right,top,bottom,request,
        device==Device::Desktop && video.width<video.height && content.x>0};
}
const Asset* select_variant(const Variants& assets,Device device) noexcept {
    if(device==Device::Desktop){return assets.desktop?&*assets.desktop:assets.mobile?&*assets.mobile:nullptr;}
    if(device==Device::Mobile){return assets.mobile?&*assets.mobile:assets.desktop?&*assets.desktop:nullptr;}
    return nullptr;
}
bool valid_asset_path(std::string_view path) noexcept {
    if(path.empty() || path.front()=='/' || path.front()=='\\' || path.back()=='/'){return false;}
    std::size_t start=0;
    for(std::size_t i=0;i<=path.size();++i){
        if(i<path.size()){
            const char c=path[i];
            if(c=='\\' || c==':' || c=='?' || c=='#' || c=='*' || c=='"' || c=='<' || c=='>' || c=='|' ||
               static_cast<unsigned char>(c)<32){return false;}
        }
        if(i==path.size() || path[i]=='/'){
            const auto part=path.substr(start,i-start);
            if(part.empty() || part=="." || part==".." || part.back()==' ' || part.back()=='.'){return false;}
            start=i+1;
        }
    }
    return true;
}
void validate_asset(const Asset& asset){
    valid_size(asset.size);
    if(asset.id.empty() || !valid_asset_path(asset.relative_path)){throw std::invalid_argument("Invalid local asset metadata");}
}
void validate_display(const StartupDisplay& display){
    valid_size(display.resolution);
    if(display.mode!=WindowMode::Windowed && display.mode!=WindowMode::Fullscreen){throw std::invalid_argument("Unknown window mode");}
}
void validate_hotspot(const HubHotspot& h){
    const auto& r=h.source_area;
    if(h.id.empty() || h.label.empty() || (h.route!=HubRoute::Battle && h.route!=HubRoute::SpecialEvent && h.route!=HubRoute::Terminal && h.route!=HubRoute::Unavailable) ||
       !std::isfinite(h.anchor_x) || !std::isfinite(h.anchor_y) ||
       !std::isfinite(r.x) || !std::isfinite(r.y) || !std::isfinite(r.width) || !std::isfinite(r.height) ||
       r.x<0 || r.y<0 || r.width<=0 || r.height<=0 || r.x+r.width>1 || r.y+r.height>1 ||
       h.anchor_x<r.x || h.anchor_y<r.y || h.anchor_x>r.x+r.width || h.anchor_y>r.y+r.height){
        throw std::invalid_argument("Invalid hub hotspot");
    }
}
bool contains(const Rect& r,double x,double y) noexcept {
    return std::isfinite(x) && std::isfinite(y) && r.width>0 && r.height>0 &&
        x>=r.x && y>=r.y && x<r.x+r.width && y<r.y+r.height;
}
std::optional<ProjectedHotspot> project_hotspot(const HubPlan& plan,const HubHotspot& h){
    validate_hotspot(h);
    const auto& uv=plan.source_uv;const auto& content=plan.content;
    if(!std::isfinite(uv.x) || !std::isfinite(uv.y) || !std::isfinite(uv.width) || !std::isfinite(uv.height) ||
       uv.x<0 || uv.y<0 || uv.width<=0 || uv.height<=0 || uv.x+uv.width>1 || uv.y+uv.height>1 ||
       !std::isfinite(content.x) || !std::isfinite(content.y) || !std::isfinite(content.width) || !std::isfinite(content.height) ||
       content.width<=0 || content.height<=0){throw std::invalid_argument("Invalid hub projection");}
    if(!h.visible){return {};}
    const double x=std::max(uv.x,h.source_area.x),y=std::max(uv.y,h.source_area.y);
    const double right=std::min(uv.x+uv.width,h.source_area.x+h.source_area.width);
    const double bottom=std::min(uv.y+uv.height,h.source_area.y+h.source_area.height);
    if(right<=x || bottom<=y){return {};}
    const auto px=[&](double value){return content.x+(value-uv.x)*content.width/uv.width;};
    const auto py=[&](double value){return content.y+(value-uv.y)*content.height/uv.height;};
    return ProjectedHotspot{{px(x),py(y),px(right)-px(x),py(bottom)-py(y)},
        px(std::clamp(h.anchor_x,x,right)),py(std::clamp(h.anchor_y,y,bottom))};
}bool MediaStartGate::should_request(MediaState state,bool ready,bool playing,bool foreground,double seconds) noexcept {
    if(state!=MediaState::Preparing || !ready || playing || !foreground || !std::isfinite(seconds) || seconds<0 || seconds<next_attempt_){return false;}
    next_attempt_=seconds+0.1;++attempts_;return true;
}
bool MediaFlow::active() const noexcept {return state_==MediaState::Preparing || state_==MediaState::Playing || state_==MediaState::Paused;}
bool MediaFlow::return_to_hub() const noexcept {return state_==MediaState::Completed || state_==MediaState::Skipped || state_==MediaState::Failed || state_==MediaState::Cancelled;}
std::uint64_t MediaFlow::begin(std::optional<Asset> asset,bool previously_watched){
    if(ticket_==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("Media ticket exhausted");}
    ++ticket_;asset_.reset();watched_=previously_watched;failure_=MediaFailure::None;state_=MediaState::Preparing;
    if(!asset){state_=MediaState::Failed;failure_=MediaFailure::MissingAsset;return ticket_;}
    try{validate_asset(*asset);}catch(const std::invalid_argument&){state_=MediaState::Failed;failure_=MediaFailure::InvalidAsset;return ticket_;}
    asset_=std::move(asset);return ticket_;
}
bool MediaFlow::prepared(std::uint64_t ticket) noexcept {
    if(ticket!=ticket_ || state_!=MediaState::Preparing){return false;}
    state_=background_?MediaState::Paused:MediaState::Playing;return true;
}
bool MediaFlow::ended(std::uint64_t ticket) noexcept {
    if(ticket!=ticket_ || state_!=MediaState::Playing){return false;}
    state_=MediaState::Completed;watched_=true;return true;
}
bool MediaFlow::fail(std::uint64_t ticket,MediaFailure reason) noexcept {
    if(ticket!=ticket_ || !active() || (reason!=MediaFailure::OpenFailed && reason!=MediaFailure::Timeout && reason!=MediaFailure::DecodeFailed)){return false;}
    state_=MediaState::Failed;failure_=reason;return true;
}
bool MediaFlow::pause() noexcept {if(state_!=MediaState::Playing){return false;}state_=MediaState::Paused;return true;}
bool MediaFlow::resume() noexcept {if(state_!=MediaState::Paused || background_){return false;}state_=MediaState::Playing;return true;}
void MediaFlow::background(bool hidden) noexcept {background_=hidden;if(hidden){pause();}}
bool MediaFlow::skip(std::uint64_t held_milliseconds) noexcept {
    if((state_!=MediaState::Playing && state_!=MediaState::Paused) || background_ || (!watched_ && held_milliseconds<800)){return false;}
    state_=MediaState::Skipped;watched_=true;return true;
}
bool MediaFlow::cancel() noexcept {if(!active()){return false;}state_=MediaState::Cancelled;return true;}
std::optional<VideoPlan> MediaFlow::layout(PixelSize viewport,Device device,bool allow_rotation) const {
    if(!asset_ || !active()){return std::nullopt;}
    return plan_video(asset_->size,viewport,device,allow_rotation);
}
} // namespace rpg::presentation
