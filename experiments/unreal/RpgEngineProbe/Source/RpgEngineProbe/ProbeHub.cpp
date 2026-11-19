#include "ProbeArena.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "Dom/JsonObject.h"
#include "Engine/Canvas.h"
#include "Engine/World.h"
#include "Engine/Texture2D.h"
#include "GameFramework/PlayerController.h"
#include "HAL/FileManager.h"
#include "ImageUtils.h"
#include "InputCoreTypes.h"
#include "Misc/FileHelper.h"
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
    HubTexture=nullptr;HubLinks.clear();
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
        DrawRect(hover?FLinearColor(0.08f,0.4f,0.45f,0.95f):FLinearColor(0.035f,0.08f,0.12f,0.92f),
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
    if(HubEventOpen){
        const double width=FMath::Min(580.0,Canvas->SizeX-30.0),x=(Canvas->SizeX-width)/2,y=FMath::Max(20.0,(Canvas->SizeY-220.0)/2);
        DrawRect(FLinearColor(0.025f,0.04f,0.07f),static_cast<float>(x),static_cast<float>(y),static_cast<float>(width),220);
        DrawText(TEXT("Evento Especial"),FColor::Cyan,static_cast<float>(x+16),static_cast<float>(y+20));
        DrawText(TEXT("Conteúdo em preparação."),FColor::White,static_cast<float>(x+16),static_cast<float>(y+65));
        DrawText(TEXT("Nenhum evento disponível nesta versão."),FColor::White,static_cast<float>(x+16),static_cast<float>(y+90));
        if(button({x+16,y+155,120,38},TEXT("Voltar")) || PlayerOwner->WasInputKeyJustPressed(EKeys::Escape)){HubEventOpen=false;}return;
    }
    for(const auto& link:HubLinks){
        const auto projected=project_hotspot(plan,link);if(!projected){continue;}
        float tw=0,th=0;GetTextSize(Text(link.label),tw,th);
        const double width=std::min(static_cast<double>(tw+24),r.width),height=std::min(34.0,r.height);
        const Rect label{std::clamp(projected->label_x-width/2,r.x,r.x+r.width-width),
            std::clamp(projected->label_y-height/2,r.y,r.y+r.height-height),width,height};
        const bool labelClicked=button(label,Text(link.label));
        if(labelClicked || (clicked && contains(projected->hit_area,mx,my))){
            if(link.route==HubRoute::Battle){enterBattle();}else{HubEventOpen=true;}return;
        }
    }
    const double w=std::min(130.0,(Canvas->SizeX-40.0)/4),y=Canvas->SizeY-46.0;
    if(button({8,y,w,34},TEXT("Esquerda")) || PlayerOwner->WasInputKeyJustPressed(EKeys::Left)){HubArea=rpg::presentation::HubArea::Left;HubView=rpg::presentation::HubView::Focused;}
    if(button({16+w,y,w,34},TEXT("Centro"))){HubArea=rpg::presentation::HubArea::Center;HubView=rpg::presentation::HubView::Focused;}
    if(button({24+2*w,y,w,34},TEXT("Direita")) || PlayerOwner->WasInputKeyJustPressed(EKeys::Right)){HubArea=rpg::presentation::HubArea::Right;HubView=rpg::presentation::HubView::Focused;}
    if(button({32+3*w,y,w,34},plan.view==rpg::presentation::HubView::Panoramic?TEXT("Focar área"):TEXT("Panorama"))){
        HubView=plan.view==rpg::presentation::HubView::Panoramic?rpg::presentation::HubView::Focused:rpg::presentation::HubView::Panoramic;
    }
}
