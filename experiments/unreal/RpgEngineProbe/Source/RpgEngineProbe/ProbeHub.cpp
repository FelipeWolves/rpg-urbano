#include "ProbeArena.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "Dom/JsonObject.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Engine/World.h"
#include "Engine/Texture2D.h"
#include "GameFramework/PlayerController.h"
#include "HAL/FileManager.h"
#include "ImageUtils.h"
#include "InputCoreTypes.h"
#include "Misc/FileHelper.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
namespace {
using namespace rpg::presentation;
FString Text(const std::string& value){return UTF8_TO_TCHAR(value.c_str());}
std::string Utf8(const FString& value){return TCHAR_TO_UTF8(*value);}
FString String(const TSharedPtr<FJsonObject>& object,const TCHAR* key){
    FString value;if(!object || !object->TryGetStringField(key,value)){throw std::invalid_argument("Missing string field");}return value;
}
double Number(const TSharedPtr<FJsonObject>& object,const TCHAR* key){
    double value=0;if(!object || !object->TryGetNumberField(key,value) || !std::isfinite(value)){throw std::invalid_argument("Missing finite numeric field");}return value;
}
TSharedPtr<FJsonObject> Object(const TSharedPtr<FJsonObject>& object,const TCHAR* key){
    const TSharedPtr<FJsonObject>* value=nullptr;if(!object || !object->TryGetObjectField(key,value) || !value || !value->IsValid()){throw std::invalid_argument("Missing object field");}return *value;
}
Rect ReadRect(const TSharedPtr<FJsonObject>& value){return {Number(value,TEXT("x")),Number(value,TEXT("y")),Number(value,TEXT("width")),Number(value,TEXT("height"))};}
const TArray<TSharedPtr<FJsonValue>>& Array(const TSharedPtr<FJsonObject>& object,const TCHAR* key){
    const TArray<TSharedPtr<FJsonValue>>* value=nullptr;if(!object || !object->TryGetArrayField(key,value) || !value){throw std::invalid_argument("Missing array field");}return *value;
}
Device DeviceProfile(){
#if PLATFORM_ANDROID || PLATFORM_IOS
    return Device::Mobile;
#else
    return Device::Desktop;
#endif
}
}
void AProbeArenaHUD::LoadHubCatalog(){
    HubTexture=nullptr;HubLogo=nullptr;HubTitleFont=nullptr;HubBodyFont=nullptr;HubLinks.clear();
    try{
        const FString root=FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()/TEXT("Presentation"));
        const FString path=root/TEXT("data/presentation/startup.json");
        const int64 bytes=IFileManager::Get().FileSize(*path);
        if(bytes<1 || bytes>1024*1024){throw std::invalid_argument("Hub catalog missing or too large");}
        FString json;if(!FFileHelper::LoadFileToString(json,*path)){throw std::invalid_argument("Cannot read local hub catalog");}
        TSharedPtr<FJsonObject> catalog;
        if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(json),catalog) || !catalog.IsValid() || Number(catalog,TEXT("schemaVersion"))!=1){throw std::invalid_argument("Invalid hub catalog schema");}
        const auto hub=Object(catalog,TEXT("hub"));const auto regions=Object(hub,TEXT("regions"));
        HubRegions={ReadRect(Object(regions,TEXT("Left"))),ReadRect(Object(regions,TEXT("Center"))),ReadRect(Object(regions,TEXT("Right")))};
        HubAspectThreshold=Number(hub,TEXT("panoramicMinAspect"));
        const FString area=String(hub,TEXT("initialArea")),view=String(hub,TEXT("view"));
        if(area!=TEXT("Left") && area!=TEXT("Center") && area!=TEXT("Right")){throw std::invalid_argument("Unknown initial hub area");}
        if(view!=TEXT("Auto") && view!=TEXT("Focused") && view!=TEXT("Panoramic")){throw std::invalid_argument("Unknown hub view");}
        HubArea=area==TEXT("Left")?rpg::presentation::HubArea::Left:area==TEXT("Right")?rpg::presentation::HubArea::Right:rpg::presentation::HubArea::Center;
        HubView=view==TEXT("Focused")?rpg::presentation::HubView::Focused:view==TEXT("Panoramic")?rpg::presentation::HubView::Panoramic:rpg::presentation::HubView::Auto;
        const TSharedPtr<FJsonObject>* visualPtr=nullptr;
        if(hub->TryGetObjectField(TEXT("visual"),visualPtr) && visualPtr && visualPtr->IsValid()){
            const auto visual=*visualPtr,palette=Object(visual,TEXT("palette"));
            HubTitle=String(visual,TEXT("title"));HubCodename=String(visual,TEXT("codename"));
            const auto readColor=[&](const TCHAR* name){
                const FString hex=String(palette,name);
                if(hex.Len()!=7 || hex[0]!=TEXT('#')){throw std::invalid_argument("Expected RGB hex color");}
                for(int32 index=1;index<hex.Len();++index){if(!FChar::IsHexDigit(hex[index])){throw std::invalid_argument("Invalid RGB hex color");}}
                return FLinearColor::FromSRGBColor(FColor::FromHex(hex));
            };
            HubMagenta=readColor(TEXT("magenta"));HubCyan=readColor(TEXT("cyanBright"));
            HubAmber=readColor(TEXT("amber"));HubCrimson=readColor(TEXT("crimson"));HubPanel=readColor(TEXT("panel"));
            const double opacity=Number(palette,TEXT("panelOpacity"));if(opacity<0 || opacity>1){throw std::invalid_argument("Invalid panel opacity");}
            HubPanel.A=static_cast<float>(opacity);
            const auto pulse=Object(visual,TEXT("battlePulse"));
            HubPulse={Number(pulse,TEXT("period")),Number(pulse,TEXT("minimumScale")),Number(pulse,TEXT("maximumScale")),
                Number(pulse,TEXT("ringMaximumScale")),Number(pulse,TEXT("ringOpacity")),Number(pulse,TEXT("ringStroke")),Number(pulse,TEXT("minimumTarget"))};
            validate_pulse(HubPulse);
            if(!visual->TryGetBoolField(TEXT("reducedMotion"),HubReducedMotion)){throw std::invalid_argument("Missing reduced motion preference");}
            const auto fonts=Object(visual,TEXT("fonts"));
            const auto loadFont=[&](const TCHAR* key){
                const FString filename=FPaths::ConvertRelativePathToFull(root/String(fonts,key));
                const int64 size=IFileManager::Get().FileSize(*filename);
                if(!FPaths::IsUnderDirectory(filename,root) || size<1 || size>4*1024*1024){throw std::invalid_argument("Local font missing or invalid");}
                auto* font=NewObject<UFont>(this);
                font->FontCacheType=EFontCacheType::Runtime;
                font->LegacyFontSize=18;
                font->GetMutableInternalCompositeFont()=FCompositeFont(FName(TEXT("Regular")),filename,EFontHinting::Default,EFontLoadingPolicy::LazyLoad);
                return font;
            };
            HubTitleFont=loadFont(TEXT("primaryPath"));HubBodyFont=loadFont(TEXT("secondaryPath"));HubBodyFont->LegacyFontSize=14;
            const auto logo=Object(visual,TEXT("logo"));
            const FString logoFile=FPaths::ConvertRelativePathToFull(root/String(logo,TEXT("runtimePath")));
            const int64 logoBytes=IFileManager::Get().FileSize(*logoFile);
            if(!FPaths::IsUnderDirectory(logoFile,root) || logoBytes<1 || logoBytes>1024*1024){throw std::invalid_argument("Local logo missing or invalid");}
            HubLogo=FImageUtils::ImportFileAsTexture2D(logoFile);
            if(!HubLogo || HubLogo->GetSizeX()!=132 || HubLogo->GetSizeY()!=132){throw std::invalid_argument("Logo decode/dimensions mismatch");}
            const auto panel=Object(visual,TEXT("eventPanel"));
            HubEventTitle=String(panel,TEXT("title"));HubEventBody=String(panel,TEXT("body"));
        }
        HubReducedMotionForced=FParse::Param(FCommandLine::Get(),TEXT("ReducedMotion"));
        HubReducedMotion=HubReducedMotion || HubReducedMotionForced;
        std::optional<Asset> panorama;
        for(const auto& entry:Array(catalog,TEXT("assets"))){
            const auto asset=entry->AsObject();
            if(String(asset,TEXT("role"))!=TEXT("HubPanorama")){continue;}
            if(panorama || String(asset,TEXT("status"))!=TEXT("Ready")){throw std::invalid_argument("Hub panorama must be uniquely ready");}
            const double width=Number(asset,TEXT("width")),height=Number(asset,TEXT("height"));
            if(width<1 || height<1 || width>8192 || height>8192 || std::floor(width)!=width || std::floor(height)!=height){throw std::invalid_argument("Invalid hub image dimensions");}
            panorama=Asset{Utf8(String(asset,TEXT("id"))),Utf8(String(asset,TEXT("path"))),{static_cast<uint32>(width),static_cast<uint32>(height)}};
            validate_asset(*panorama);
        }
        if(!panorama){throw std::invalid_argument("Hub panorama not declared");}
        const auto& links=Array(hub,TEXT("hotspots"));
        if(links.Num()>16){throw std::invalid_argument("Too many provisional hub links");}
        std::vector<HubHotspot> parsed;
        for(const auto& entry:links){
            const auto object=entry->AsObject();const auto anchor=Object(object,TEXT("anchor"));
            const FString route=String(object,TEXT("route"));bool visible=false;
            if(!object->TryGetBoolField(TEXT("visible"),visible) || (route!=TEXT("SpecialEventPlaceholder") && route!=TEXT("ReferenceBattle"))){throw std::invalid_argument("Unknown hub link route");}
            HubHotspot hotspot{Utf8(String(object,TEXT("id"))),Utf8(String(object,TEXT("label"))),route==TEXT("ReferenceBattle")?HubRoute::Battle:HubRoute::SpecialEvent,
                Number(anchor,TEXT("x")),Number(anchor,TEXT("y")),ReadRect(Object(object,TEXT("hitArea"))),visible};
            validate_hotspot(hotspot);
            if(std::ranges::any_of(parsed,[&](const HubHotspot& known){return known.id==hotspot.id;})){throw std::invalid_argument("Duplicate hub link");}
            parsed.push_back(std::move(hotspot));
        }
        (void)plan_hub(panorama->size,{1280,720},DeviceProfile(),HubArea,HubView,HubAspectThreshold,HubRegions);
        const FString image=FPaths::ConvertRelativePathToFull(root/Text(panorama->relative_path));
        const int64 imageBytes=IFileManager::Get().FileSize(*image);
        if(!FPaths::IsUnderDirectory(image,root) || imageBytes<1 || imageBytes>32*1024*1024){throw std::invalid_argument("Hub image missing, outside root or too large");}
        auto* texture=FImageUtils::ImportFileAsTexture2D(image);
        if(!texture || texture->GetSizeX()!=static_cast<int32>(panorama->size.width) || texture->GetSizeY()!=static_cast<int32>(panorama->size.height)){throw std::invalid_argument("Hub image decode/dimensions mismatch");}
        HubTexture=texture;HubLinks=std::move(parsed);HubError.Empty();
        UE_LOG(LogTemp,Display,TEXT("HUB_LOADED links=%d image=%dx%d"),static_cast<int32>(HubLinks.size()),texture->GetSizeX(),texture->GetSizeY());
    }catch(const std::exception& error){
        HubLinks.clear();HubTexture=nullptr;HubError=Text(error.what());
        UE_LOG(LogTemp,Warning,TEXT("HUB_UNAVAILABLE %s"),*HubError);
    }
}
void AProbeArenaHUD::ReturnToHub(){
    InHub=true;HubEventOpen=false;Gesture.cancel();PopupLines.Empty();SelectedSlot=0;SelectedTarget.Empty();
    UE_LOG(LogTemp,Display,TEXT("HUB_RETURN attempt=%llu"),static_cast<unsigned long long>(Session.state().progress.attempt));
}
void AProbeArenaHUD::DrawHub(){
    if(Canvas->SizeX<120 || Canvas->SizeY<120){return;}
    float mx=0,my=0;PlayerOwner->GetMousePosition(mx,my);
    const bool clicked=PlayerOwner->WasInputKeyJustReleased(EKeys::LeftMouseButton);
    const auto button=[&](const Rect& rect,const FString& label){
        const bool hover=contains(rect,mx,my);
        DrawRect(hover?FLinearColor(HubCyan.R*0.15f,HubCyan.G*0.15f,HubCyan.B*0.15f,0.92f):HubPanel,
            static_cast<float>(rect.x),static_cast<float>(rect.y),static_cast<float>(rect.width),static_cast<float>(rect.height));
        DrawText(label,FColor::White,static_cast<float>(rect.x+8),static_cast<float>(rect.y+8));
        return clicked && hover;
    };
    const auto enterBattle=[&](){
        InHub=false;HubEventOpen=false;NextAutomatic=GetWorld()->GetTimeSeconds()+0.5;
        Gesture.cancel();SelectedSlot=0;SelectedTarget.Empty();
        UE_LOG(LogTemp,Display,TEXT("HUB_BATTLE attempt=%llu"),static_cast<unsigned long long>(Session.state().progress.attempt));
    };
    DrawRect(FLinearColor(0.015f,0.02f,0.03f),0,0,Canvas->SizeX,Canvas->SizeY);
    if(!HubTexture){
        DrawText(TEXT("Hub indisponível. A batalha continua acessível."),FColor::White,20,40);
        DrawText(HubError,FColor::Orange,20,65);
        if(button({20,110,180,38},TEXT("Abrir batalha"))){enterBattle();}return;
    }
    const auto plan=plan_hub({static_cast<uint32>(HubTexture->GetSizeX()),static_cast<uint32>(HubTexture->GetSizeY())},
        {static_cast<uint32>(Canvas->SizeX),static_cast<uint32>(Canvas->SizeY)},DeviceProfile(),HubArea,HubView,HubAspectThreshold,HubRegions);
    const auto& r=plan.content;const auto& uv=plan.source_uv;
    DrawTexture(HubTexture,static_cast<float>(r.x),static_cast<float>(r.y),static_cast<float>(r.width),static_cast<float>(r.height),
        static_cast<float>(uv.x),static_cast<float>(uv.y),static_cast<float>(uv.width),static_cast<float>(uv.height),FLinearColor::White,BLEND_Opaque);
    const auto line=[&](double x1,double y1,double x2,double y2,FLinearColor color,double stroke){
        DrawLine(static_cast<float>(x1),static_cast<float>(y1),static_cast<float>(x2),static_cast<float>(y2),color,static_cast<float>(stroke));
    };
    const auto frame=[&](const Rect& area,FLinearColor color){
        auto glow=color;glow.A=0.12f;
        for(double stroke:{5.0,1.5}){
            const auto c=stroke>2?glow:color;
            line(area.x,area.y,area.x+area.width,area.y,c,stroke);
            line(area.x+area.width,area.y,area.x+area.width,area.y+area.height,c,stroke);
            line(area.x+area.width,area.y+area.height,area.x,area.y+area.height,c,stroke);
            line(area.x,area.y+area.height,area.x,area.y,c,stroke);
        }
    };
    if(HubLogo){DrawTexture(HubLogo,12,8,48,48,0,0,1,1,FLinearColor::White,BLEND_Translucent);}
    DrawText(HubTitle,HubCyan.ToFColorSRGB(),68,8,HubTitleFont);
    DrawText(TEXT("CODINOME: ")+HubCodename,FColor::White,68,32,HubBodyFont,0.75f);
    if(HubEventOpen){
        const double width=FMath::Min(660.0,Canvas->SizeX-30.0),height=FMath::Min(360.0,Canvas->SizeY-30.0);
        const double x=(Canvas->SizeX-width)/2,y=(Canvas->SizeY-height)/2;
        DrawRect(HubPanel,static_cast<float>(x),static_cast<float>(y),static_cast<float>(width),static_cast<float>(height));
        frame({x,y,width,height},HubMagenta);
        double cursor=y+18;
        float lineWidth=0,lineHeight=0;GetTextSize(TEXT("Ágj"),lineWidth,lineHeight,HubBodyFont);
        const double lineStep=std::max(20.0,static_cast<double>(lineHeight)+4);
        const auto wrapped=[&](const FString& text,FColor color){
            TArray<FString> words;text.ParseIntoArray(words,TEXT(" "),true);FString current;
            for(const auto& word:words){
                const FString candidate=current.IsEmpty()?word:current+TEXT(" ")+word;
                float textWidth=0,textHeight=0;GetTextSize(candidate,textWidth,textHeight,HubBodyFont);
                if(textWidth>width-32 && !current.IsEmpty()){
                    if(cursor+lineStep>y+height-60){return;}
                    DrawText(current,color,static_cast<float>(x+16),static_cast<float>(cursor),HubBodyFont);cursor+=lineStep;current=word;
                }else{current=candidate;}
            }
            if(!current.IsEmpty() && cursor+lineStep<=y+height-60){DrawText(current,color,static_cast<float>(x+16),static_cast<float>(cursor),HubBodyFont);cursor+=lineStep;}
        };
        wrapped(HubEventTitle,HubCyan.ToFColorSRGB());cursor+=12;wrapped(HubEventBody,FColor::White);
        if(button({x+16,y+height-48,120,36},TEXT("Voltar")) || PlayerOwner->WasInputKeyJustPressed(EKeys::Escape)){HubEventOpen=false;}return;
    }
    const auto pulse=pulse_frame(GetWorld()->GetTimeSeconds(),HubReducedMotion,HubPulse);
    for(const auto& link:HubLinks){
        const auto projected=project_hotspot(plan,link);if(!projected){continue;}
        const bool battle=link.route==HubRoute::Battle;
        const FString labelText=battle?Text(link.label).ToUpper():Text(link.label);
        float tw=0,th=0;GetTextSize(labelText,tw,th,HubBodyFont);
        const double width=std::min(static_cast<double>(tw+24),r.width),height=std::min(std::max(28.0,static_cast<double>(th)+12.0),r.height);
        const Rect label{std::clamp(projected->label_x-width/2,r.x,r.x+r.width-width),
            std::clamp(projected->label_y+(battle?30:-height/2),r.y,r.y+r.height-height),width,height};
        const double left=std::min(label.x,projected->hit_area.x),top=std::min(label.y,projected->hit_area.y);
        const double right=std::max(label.x+label.width,projected->hit_area.x+projected->hit_area.width);
        const double bottom=std::max(label.y+label.height,projected->hit_area.y+projected->hit_area.height);
        const auto hit=minimum_target({left,top,right-left,bottom-top},r,HubPulse.minimum_target);
        if(!hit){continue;}
        const bool hover=contains(*hit,mx,my);
        DrawRect(HubPanel,static_cast<float>(label.x),static_cast<float>(label.y),static_cast<float>(label.width),static_cast<float>(label.height));
        DrawText(labelText,(battle?HubAmber:HubCyan).ToFColorSRGB(),static_cast<float>(label.x+10),static_cast<float>(label.y+6),HubBodyFont);
        if(!battle){frame(label,hover?HubCyan:HubMagenta);}
        else{
            const double cx=projected->label_x,cy=projected->label_y;
            const auto octagon=[&](double radius,FLinearColor color,double stroke){
                constexpr double points[8][2]={{-.55,-1},{.55,-1},{1,-.55},{1,.55},{.55,1},{-.55,1},{-1,.55},{-1,-.55}};
                for(int32 index=0;index<8;++index){const int32 next=(index+1)%8;line(cx+points[index][0]*radius,cy+points[index][1]*radius,cx+points[next][0]*radius,cy+points[next][1]*radius,color,stroke);}
            };
            const double radius=20*pulse.icon_scale;
            auto glow=HubAmber;glow.A=hover?0.35f:0.15f;octagon(radius,glow,6);octagon(radius,HubAmber,1.5);
            if(pulse.static_double_border){octagon(16,HubCrimson,1.5);}
            else{
                auto ring=HubAmber;ring.A=static_cast<float>(pulse.ring_opacity);
                for(int32 i=0;i<32;++i){
                    const double a=2*PI*i/32,b=2*PI*(i+1)/32,ringRadius=20*pulse.ring_scale;
                    line(cx+ringRadius*std::cos(a),cy+ringRadius*std::sin(a),cx+ringRadius*std::cos(b),cy+ringRadius*std::sin(b),ring,HubPulse.ring_stroke);
                }
            }
            // Original geometric paired fangs, no imported game icon.
            for(double sign:{-1.0,1.0}){
                line(cx+sign*12*pulse.icon_scale,cy-8*pulse.icon_scale,cx+sign*5*pulse.icon_scale,cy+11*pulse.icon_scale,HubAmber,2);
                line(cx+sign*5*pulse.icon_scale,cy+11*pulse.icon_scale,cx+sign*3*pulse.icon_scale,cy-5*pulse.icon_scale,HubAmber,2);
            }
        }
        if(clicked && hover){if(battle){enterBattle();}else{HubEventOpen=true;}return;}
    }
    const double w=std::min(130.0,(Canvas->SizeX-48.0)/5),y=Canvas->SizeY-46.0;
    if(button({8,y,w,34},TEXT("Esquerda")) || PlayerOwner->WasInputKeyJustPressed(EKeys::Left)){HubArea=rpg::presentation::HubArea::Left;HubView=rpg::presentation::HubView::Focused;}
    if(button({16+w,y,w,34},TEXT("Centro"))){HubArea=rpg::presentation::HubArea::Center;HubView=rpg::presentation::HubView::Focused;}
    if(button({24+2*w,y,w,34},TEXT("Direita")) || PlayerOwner->WasInputKeyJustPressed(EKeys::Right)){HubArea=rpg::presentation::HubArea::Right;HubView=rpg::presentation::HubView::Focused;}
    if(button({32+3*w,y,w,34},plan.view==rpg::presentation::HubView::Panoramic?TEXT("Focar área"):TEXT("Panorama"))){
        HubView=plan.view==rpg::presentation::HubView::Panoramic?rpg::presentation::HubView::Focused:rpg::presentation::HubView::Panoramic;
    }
    if(button({40+4*w,y,w,34},HubReducedMotion?TEXT("Sem pulso"):TEXT("Com pulso")) && !HubReducedMotionForced){HubReducedMotion=!HubReducedMotion;}
}



