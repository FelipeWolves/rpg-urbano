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
