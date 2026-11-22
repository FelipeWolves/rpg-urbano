#include "rpg/startup_presentation.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
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
        owner_=PointerSource::Touch;context_=context;started_=seconds;origin_x_=touch.x;origin_y_=touch.y;frame.pressed=true;
    }else if(owner_==PointerSource::None && mouseStart && !touch.down && seconds>=touch_guard_ && valid(mouse)){
        owner_=PointerSource::Mouse;context_=context;started_=seconds;origin_x_=mouse.x;origin_y_=mouse.y;frame.pressed=true;
    }
    if(owner_==PointerSource::None){return frame;}
    const auto sample=owner_==PointerSource::Touch?touch:mouse;
    frame.source=owner_;frame.x=sample.x;frame.y=sample.y;frame.origin_x=origin_x_;frame.origin_y=origin_y_;
    if(!valid(sample) || std::hypot(sample.x-origin_x_,sample.y-origin_y_)>12){
        if(owner_==PointerSource::Touch){touch_guard_=seconds+0.35;}
        cancel();return frame;
    }
    frame.down=sample.down;
    if(!sample.down){
        frame.released=true;frame.tap=seconds-started_<0.5;
        if(owner_==PointerSource::Touch){touch_guard_=seconds+0.35;}
        owner_=PointerSource::None;
    }
    return frame;
}
bool pointer_clicked(const PointerFrame& frame,const Rect& target) noexcept {
    return frame.tap && !frame.cancelled && contains(target,frame.origin_x,frame.origin_y) && contains(target,frame.x,frame.y);
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
}} // namespace rpg::presentation
