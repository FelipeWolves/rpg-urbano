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
Rect contain(PixelSize media,PixelSize viewport){
    valid_size(media);return fit(media.width,media.height,viewport);
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
} // namespace rpg::presentation
