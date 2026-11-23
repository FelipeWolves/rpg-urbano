#include "ProbeArena.h"
#include "rpg/persistence.hpp"
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
#include "Misc/ConfigCacheIni.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "Misc/Guid.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
namespace {
using namespace rpg::presentation;
class UnrealSaveFiles final:public rpg::storage::Files {
 FString Root;bool Progress;
 FString Path(std::string_view key)const{return Root/UTF8_TO_TCHAR(std::string(key).c_str());}
public:
 explicit UnrealSaveFiles(FString root,bool progress=false):Root(MoveTemp(root)),Progress(progress){}
 bool ensure_directory()override{return IFileManager::Get().MakeDirectory(*Root,true);}
 rpg::storage::ReadResult read(std::string_view key,std::size_t limit)override{
  const auto path=Path(key);if(!IFileManager::Get().FileExists(*path)){return {};}
  TUniquePtr<FArchive> file(IFileManager::Get().CreateFileReader(*path));const int64 size=file?file->TotalSize():-1;
  if(size<0||size>static_cast<int64>(limit)){return {rpg::storage::ReadState::Failed,{}};}
  if(!Progress){file.Reset();FString text;if(!FFileHelper::LoadFileToString(text,*path)){return {rpg::storage::ReadState::Failed,{}};}return {rpg::storage::ReadState::Ready,TCHAR_TO_UTF8(*text)};}
  std::string bytes(static_cast<std::size_t>(size),char{});if(size){file->Serialize(bytes.data(),size);}
  if(file->IsError()){return {rpg::storage::ReadState::Failed,{}};}return {rpg::storage::ReadState::Ready,MoveTemp(bytes)};
 }
 bool copy(std::string_view from,std::string_view to)override{return IFileManager::Get().Copy(*Path(to),*Path(from),true,false)==COPY_OK;}
 bool write(std::string_view key,std::string_view bytes)override{TArray<uint8> data;data.Append(reinterpret_cast<const uint8*>(bytes.data()),static_cast<int32>(bytes.size()));return FFileHelper::SaveArrayToFile(data,*Path(key));}
 bool replace(std::string_view target,std::string_view temporary)override{return IFileManager::Get().Move(*Path(target),*Path(temporary),true,Progress,false,true);}
 void discard(std::string_view key)override{IFileManager::Get().Delete(*Path(key),false,false,true);}
};
rpg::storage::Keys StorageKeys(const FString& filename){
 const std::string name=TCHAR_TO_UTF8(*filename);const std::string id=TCHAR_TO_UTF8(*FGuid::NewGuid().ToString(EGuidFormats::Digits));
 return {name,name+".tmp-"+id,name+".bak",name+".invalid-"+id};
}
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
Device DeviceProfile(PresentationMode preference){
#if PLATFORM_ANDROID || PLATFORM_IOS
    const Device actual=Device::Mobile;
#else
    const Device actual=Device::Desktop;
#endif
    FString name;
    if(!FParse::Value(FCommandLine::Get(),TEXT("Presentation="),name)){return presentation_device(preference,actual);}
    const auto mode=parse_presentation_mode(Utf8(name));
    if(!mode){static bool warned=false;if(!warned){UE_LOG(LogTemp,Warning,TEXT("Unknown Presentation; using Auto"));warned=true;}return actual;}
    return presentation_device(*mode,actual);
}
}
void AProbeArenaHUD::LoadTutorialProgress(){
 try{UnrealSaveFiles files(FPaths::ProjectSavedDir()/TEXT("Tutorials"),true);TutorialCompleted=rpg::presentation::load_tutorial_completion(files,StorageKeys(TEXT("bola-perdida-v1.txt")));TutorialSaveStatus=TEXT("Conclusoes locais carregadas.");}
 catch(const std::exception& e){TutorialSaveStatus=Text(e.what());}
}
void AProbeArenaHUD::RecordTutorialCompletion(bool classic){
 try{UnrealSaveFiles files(FPaths::ProjectSavedDir()/TEXT("Tutorials"),true);TutorialCompleted=rpg::presentation::record_tutorial_completion(files,StorageKeys(TEXT("bola-perdida-v1.txt")),classic);TutorialSaveStatus=TEXT("Conclusao salva em Saved/Tutorials/bola-perdida-v1.txt; sem moedas.");}
 catch(const std::exception& e){TutorialSaveStatus=Text(e.what());}
}
void AProbeArenaHUD::LoadHubCatalog(){
    HubTexture=nullptr;HubLogo=nullptr;HubTitleFont=nullptr;HubBodyFont=nullptr;HubLinks.clear();
    const auto inset=[&](const TCHAR* key){
        float value=0;GConfig->GetFloat(TEXT("RpgPrototype"),key,value,GGameIni);
        if(!FMath::IsFinite(value) || value<0){UE_LOG(LogTemp,Warning,TEXT("Invalid %s; using zero"),key);return 0.0;}
        return static_cast<double>(value);
    };
    HubInsets={inset(TEXT("SafeLeft")),inset(TEXT("SafeTop")),inset(TEXT("SafeRight")),inset(TEXT("SafeBottom"))};
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
        Preferences.reduced_motion=HubReducedMotion;
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
            if(!object->TryGetBoolField(TEXT("visible"),visible) || (route!=TEXT("SpecialEventPlaceholder") && route!=TEXT("ReferenceBattle") && route!=TEXT("Terminal") && route!=TEXT("Unavailable"))){throw std::invalid_argument("Unknown hub link route");}
            HubHotspot hotspot{Utf8(String(object,TEXT("id"))),Utf8(String(object,TEXT("label"))),route==TEXT("ReferenceBattle")?HubRoute::Battle:route==TEXT("Terminal")?HubRoute::Terminal:route==TEXT("Unavailable")?HubRoute::Unavailable:HubRoute::SpecialEvent,
                Number(anchor,TEXT("x")),Number(anchor,TEXT("y")),ReadRect(Object(object,TEXT("hitArea"))),visible};
            validate_hotspot(hotspot);
            if(std::ranges::any_of(parsed,[&](const HubHotspot& known){return known.id==hotspot.id;})){throw std::invalid_argument("Duplicate hub link");}
            parsed.push_back(std::move(hotspot));
        }
        (void)plan_hub(panorama->size,{1280,720},DeviceProfile(Preferences.mode),HubArea,HubView,HubAspectThreshold,HubRegions);
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
void AProbeArenaHUD::ApplyPresentationPreferences()
{
    HubArea=Preferences.area;HubView=Preferences.view;
    HubReducedMotion=Preferences.reduced_motion || HubReducedMotionForced;
    HubPanel=Preferences.high_contrast?FLinearColor(0,0,0,1):HubBasePanel;
}
void AProbeArenaHUD::LoadPresentationPreferences()
{
    HubBasePanel=HubPanel;HubReducedMotionForced=FParse::Param(FCommandLine::Get(),TEXT("ReducedMotion"));
    Preferences.area=HubArea;Preferences.view=HubView;
    FString configured=TEXT("Auto");GConfig->GetString(TEXT("RpgPrototype"),TEXT("PresentationMode"),configured,GGameIni);
    if(const auto mode=parse_presentation_mode(Utf8(configured))){Preferences.mode=*mode;}
    UnrealSaveFiles files(FPaths::ProjectSavedDir()/TEXT("Presentation"));
    const auto result=rpg::storage::load_preferences(files,StorageKeys(TEXT("preferences-v1.txt")),Preferences);
    Preferences=result.values;
    PreferencesStatus=result.source==PreferenceSource::Backup?TEXT("Cópia anterior carregada."):result.primary_invalid?TEXT("Arquivo inválido; usando padrões."):TEXT("Preferências locais.");
    ApplyPresentationPreferences();
}
void AProbeArenaHUD::SavePresentationPreferences()
{
    Preferences.area=HubArea;Preferences.view=HubView;ApplyPresentationPreferences();
    try{
        UnrealSaveFiles files(FPaths::ProjectSavedDir()/TEXT("Presentation"));
        rpg::storage::save_preferences(files,StorageKeys(TEXT("preferences-v1.txt")),Preferences);
    }catch(const std::exception& e){PreferencesStatus=Text(e.what());return;}
    PreferencesStatus=TEXT("Preferências salvas neste dispositivo.");
}
void AProbeArenaHUD::DrawPreferences()
{
    const auto frame=plan_hub_frame({static_cast<uint32>(Canvas->SizeX),static_cast<uint32>(Canvas->SizeY)},0,HubInsets);
    if(!frame){if(PlayerOwner->WasInputKeyJustPressed(EKeys::Escape)){Flow.back(false,false);}return;}
    const double width=std::min(600.0,frame->safe.width-24),height=std::min(400.0,frame->safe.height-24);
    const double x=frame->safe.x+(frame->safe.width-width)/2,y=frame->safe.y+(frame->safe.height-height)/2;
    DrawRect(HubPanel,static_cast<float>(x),static_cast<float>(y),static_cast<float>(width),static_cast<float>(height));
    const auto text=[&](const FString& value,double tx,double ty,FColor color){
        float w=0,h=0;GetTextSize(value,w,h,HubBodyFont);const float scale=static_cast<float>(std::min(1.0,(width-32)/std::max(1.0,static_cast<double>(w))));
        DrawText(value,color,static_cast<float>(tx),static_cast<float>(ty),HubBodyFont,scale);
    };
    text(TEXT("PREFERÊNCIAS LOCAIS"),x+16,y+8,FColor::Cyan);text(PreferencesStatus,x+16,y+28,FColor::White);
    const auto button=[&](int row,const FString& value,bool enabled=true){
        Rect rect{x+16,y+52+row*52,width-32,48};
        DrawRect(enabled?FLinearColor(0.08f,0.20f,0.32f):FLinearColor(0.12f,0.12f,0.12f),static_cast<float>(rect.x),static_cast<float>(rect.y),static_cast<float>(rect.width),48);
        text(value,rect.x+8,rect.y+10,enabled?FColor::White:FColor(160,160,160));return enabled && pointer_clicked(Input,rect);
    };
    if(button(0,HubReducedMotionForced?TEXT("Movimento reduzido forçado nesta execução"):Preferences.reduced_motion?TEXT("Movimento: reduzido"):TEXT("Movimento: normal"),!HubReducedMotionForced)){Preferences.reduced_motion=!Preferences.reduced_motion;SavePresentationPreferences();return;}
    if(button(1,Preferences.high_contrast?TEXT("Contraste: alto"):TEXT("Contraste: padrão"))){Preferences.high_contrast=!Preferences.high_contrast;SavePresentationPreferences();return;}
    if(button(2,FString::Printf(TEXT("Texto dos painéis: %u%%"),Preferences.text_percent))){Preferences.text_percent=Preferences.text_percent==100?125:Preferences.text_percent==125?150:100;SavePresentationPreferences();return;}
    const TCHAR* mode=Preferences.mode==PresentationMode::Auto?TEXT("Auto"):Preferences.mode==PresentationMode::Desktop?TEXT("Desktop"):TEXT("Mobile");
    FString forcedMode;const bool forced=FParse::Value(FCommandLine::Get(),TEXT("Presentation="),forcedMode);
    if(button(3,forced?FString(TEXT("Apresentação desta execução: "))+forcedMode:FString(TEXT("Apresentação: "))+mode,!forced)){Preferences.mode=Preferences.mode==PresentationMode::Auto?PresentationMode::Desktop:Preferences.mode==PresentationMode::Desktop?PresentationMode::Mobile:PresentationMode::Auto;SavePresentationPreferences();return;}
    const Rect close{x+16,y+height-56,width-32,48};
    DrawRect(FLinearColor(0.08f,0.20f,0.32f),static_cast<float>(close.x),static_cast<float>(close.y),static_cast<float>(close.width),48);
    text(TEXT("Voltar"),close.x+8,close.y+10,FColor::White);
    if(pointer_clicked(Input,close) || PlayerOwner->WasInputKeyJustPressed(EKeys::Escape)){Flow.back(false,false);}
}
void AProbeArenaHUD::ReturnToHub(){
    if(Flow.screen()!=UiScreen::Hub && !Flow.return_hub()){return;}HasActionFeedback=false;Gesture.cancel();PopupLines.Empty();SelectedSlot=0;SelectedTarget.Empty();
    if(PveActive || WavesActive || SequenceActive || SharedActive || TutorialActive){Flow.acknowledge_new_attempt();PveActive=false;WavesActive=false;SequenceActive=false;SharedActive=false;TutorialActive=false;RefreshArenaActors();}
    UE_LOG(LogTemp,Display,TEXT("HUB_RETURN attempt=%llu"),static_cast<unsigned long long>(Session.state().progress.attempt));
}
int32 AProbeArenaHUD::DrawInfoPanel(const FString& title,const TArray<FString>& paragraphs,int32& page,bool action,const FString& actionLabel,const FString& closeLabel)
{
    if(Canvas->SizeX<1 || Canvas->SizeY<1){return 0;}
    const auto frame=plan_hub_frame({static_cast<uint32>(Canvas->SizeX),static_cast<uint32>(Canvas->SizeY)},0,HubInsets);
    if(!frame){DrawText(TEXT("Amplie a janela para ler o painel."),FColor::White,8,20);return PlayerOwner->WasInputKeyJustPressed(EKeys::Escape)?1:0;}
    float tw=0,th=0;GetTextSize(TEXT("Ágj"),tw,th,HubBodyFont);
    const float textScale=Preferences.text_percent/100.f;
    const double step=std::max(22.0,static_cast<double>(th)*textScale+4);
    const auto layout=plan_info_panel(frame->safe,step,action);
    if(!layout){return PlayerOwner->WasInputKeyJustPressed(EKeys::Escape)?1:0;}
    const auto& p=*layout;
    DrawRect(HubPanel,static_cast<float>(p.box.x),static_cast<float>(p.box.y),static_cast<float>(p.box.width),static_cast<float>(p.box.height));
    const auto button=[&](Rect r,const FString& text,bool enabled){
        DrawRect(enabled?FLinearColor(0.08f,0.20f,0.32f):FLinearColor(0.10f,0.10f,0.12f),static_cast<float>(r.x),static_cast<float>(r.y),static_cast<float>(r.width),static_cast<float>(r.height));
        float w=0,h=0;GetTextSize(text,w,h,HubBodyFont);const float scale=static_cast<float>(std::min(1.0,(r.width-12)/std::max(1.0,static_cast<double>(w))));
        DrawText(text,enabled?FColor::White:FColor(150,150,150),static_cast<float>(r.x+6),static_cast<float>(r.y+8),HubBodyFont,scale);
        return enabled && pointer_clicked(Input,r);
    };
    std::vector<std::string> lines;
    try{
        std::string text=Utf8(title)+"\n\n";
        for(int32 i=0;i<paragraphs.Num();++i){if(i){text+='\n';}text+=Utf8(paragraphs[i]);}
        lines=wrap_text(text,p.body.width,[&](std::string_view value){
            float width=0,height=0;GetTextSize(Text(std::string(value)),width,height,HubBodyFont);return static_cast<double>(width)*textScale;
        });
    }catch(const std::exception&){lines={"Texto indisponível."};}
    const auto slice=text_page(lines.size(),p.capacity,static_cast<std::size_t>(std::max(0,page)));
    page=static_cast<int32>(slice.page);
    for(std::size_t i=slice.begin;i<slice.end;++i){
        DrawText(Text(lines[i]),i==0?FColor::Cyan:FColor::White,static_cast<float>(p.body.x),static_cast<float>(p.body.y+(i-slice.begin)*step),HubBodyFont,textScale);
    }
    const FString previous=FString::Printf(TEXT("Anterior (%d/%d)"),page+1,static_cast<int32>(slice.pages));
    if(button(p.close,closeLabel,true) || PlayerOwner->WasInputKeyJustPressed(EKeys::Escape)){return 1;}
    if(button(p.previous,previous,slice.page>0)){--page;}
    if(button(p.next,TEXT("Próxima"),slice.page+1<slice.pages)){++page;}
    if(p.action && button(*p.action,actionLabel,true)){return 2;}
    return 0;
}
void AProbeArenaHUD::SelectLabScenario(bool felipe){
    const bool leavingRitual=PveActive || WavesActive || SequenceActive || SharedActive || TutorialActive;PveActive=false;WavesActive=false;SequenceActive=false;SharedActive=false;TutorialActive=false;
    if(felipe==SessionIsFelipe){if(leavingRitual){RefreshArenaActors();}return;}
    // Preserve each scenario in memory; neither progress ledger is imported into the other.
    if(!AlternateLabSession){
        if(felipe){AlternateLabSession.emplace(rpg::felipe_pilot_setup(PilotTutorial),rpg::CombatProfile::Team);}
        else{AlternateLabSession.emplace(ReferenceProfile==rpg::CombatProfile::Classic?rpg::classic_reference_setup():rpg::terminal_lab_setup(),ReferenceProfile);}
    }
    std::swap(Session,*AlternateLabSession);SessionIsFelipe=felipe;
    Flow.acknowledge_new_attempt();Gesture.cancel();Pointer.cancel_until_release();
    SelectedSlot=0;SelectedTarget.Empty();PopupLines.Empty();BattleMenuPage=0;
    DraftTeam=Session.team().selected;RefreshArenaActors();
    Message=felipe?TEXT("Piloto Felipe — teste local, sem PvP online."):TEXT("Laboratorio de referencia.");
    UE_LOG(LogTemp,Display,TEXT("HUB_LAB_SCENARIO felipe=%d"),felipe?1:0);
}
void AProbeArenaHUD::DrawAnimatedHubLabel(const rpg::presentation::HubHotspot& link,std::size_t index,const FString& labelText,float tx,float ty,float ts,float tw,float th,bool hover,float opacity)
{
    using namespace rpg::presentation;
    const bool future=link.route==HubRoute::Unavailable && link.id!="arena-pvp";
    float& hoverAmount=HubLabelHover.FindOrAdd(Text(link.id));
    hoverAmount=HubReducedMotion?(hover?1.f:0.f):FMath::FInterpTo(hoverAmount,hover?1.f:0.f,FMath::Min(GetWorld()->GetDeltaSeconds(),0.1f),8.f);
    const auto motion=label_motion(GetWorld()->GetTimeSeconds(),HubReducedMotion,static_cast<double>(index)*0.65);
    ty+=static_cast<float>(motion.offset_y);
    auto* font=HubLabelFont?HubLabelFont.Get():HubTitleFont.Get();
    const FLinearColor idleInk=((future?FLinearColor(0.32f,0.55f,0.67f):FLinearColor(0.38f,0.72f,0.84f))*1.3f).GetClamped();
    FColor ink=FMath::Lerp(idleInk,FLinearColor(0.82f,0.98f,1.f),hoverAmount).ToFColorSRGB();
    ink.A=static_cast<uint8>(255.f*opacity);
    const FString glowId=TEXT("halo_label_")+Text(link.id);
    const auto* geometry=LabelGlowGeometry.Find(glowId);
    if(auto* texture=UiTexture(glowId);texture && geometry && LabelGlowText.FindRef(glowId)==labelText){
        const float sx=tw*ts/static_cast<float>(geometry->X),sy=th*ts/static_cast<float>(geometry->Y);
        const FLinearColor glowColor(0.18f,0.75f,1.f,FMath::Lerp(0.28f,0.85f,hoverAmount)*opacity);
        DrawTexture(texture,tx-static_cast<float>(geometry->Z)*sx,ty-static_cast<float>(geometry->W)*sy,
            texture->GetSizeX()*sx,texture->GetSizeY()*sy,0,0,1,1,glowColor,BLEND_Translucent);
    }
    DrawText(labelText,ink,tx,ty,font,ts);
}

void AProbeArenaHUD::DrawHub(){
    if(Canvas->SizeX<1 || Canvas->SizeY<1){return;}
    const bool focused3D=HubView==HubView::Focused || (HubView==HubView::Auto &&
        (DeviceProfile(Preferences.mode)==Device::Mobile || Canvas->SizeX/float(Canvas->SizeY)<HubAspectThreshold));
    const bool preview3D=UpdateHub3D(focused3D);
    const auto framePlan=plan_hub_frame({static_cast<uint32>(Canvas->SizeX),static_cast<uint32>(Canvas->SizeY)},static_cast<uint32>(HubLinks.size()),HubInsets);
    if(!framePlan){DrawText(TEXT("Amplie a janela ou reduza as margens simuladas."),FColor::White,8,20);return;}
    const auto& layout=*framePlan;
    const float mx=static_cast<float>(Input.x),my=static_cast<float>(Input.y);
    const bool clicked=Input.tap;
    const auto button=[&](const Rect& rect,const FString& label){
        const bool hover=contains(rect,mx,my);
        DrawRect(hover?FLinearColor(HubCyan.R*0.15f,HubCyan.G*0.15f,HubCyan.B*0.15f,0.92f):HubPanel,
            static_cast<float>(rect.x),static_cast<float>(rect.y),static_cast<float>(rect.width),static_cast<float>(rect.height));
        float width=0,height=0;GetTextSize(label,width,height,HubBodyFont);
        const float scale=static_cast<float>(std::min({1.0,(rect.width-16)/std::max(1.0,static_cast<double>(width)),(rect.height-12)/std::max(1.0,static_cast<double>(height))}));
        DrawText(label,FColor::White,static_cast<float>(rect.x+8),static_cast<float>(rect.y+6),HubBodyFont,scale);
        return clicked && pointer_clicked(Input,rect);
    };
    const auto enterBattle=[&](){
        if(Flow.screen()!=UiScreen::Hub && Flow.screen()!=UiScreen::Location){return;}
        if(Flow.new_attempt_required()){Session.reset();}
        if(!Flow.enter_arena(Session.state().combat.battle.phase==rpg::Phase::Finished)){return;}
        NextAutomatic=GetWorld()->GetTimeSeconds()+0.5;
        Gesture.cancel();SelectedSlot=0;SelectedTarget.Empty();
        UE_LOG(LogTemp,Display,TEXT("HUB_BATTLE attempt=%llu"),static_cast<unsigned long long>(Session.state().progress.attempt));
    };
    if(!preview3D){DrawRect(FLinearColor(0.015f,0.02f,0.03f),0,0,Canvas->SizeX,Canvas->SizeY);}
    if(Flow.screen()==UiScreen::TeamSetup){DrawTeamSetup();return;}
    if(!HubTexture){
        if(button({layout.safe.x+20,layout.safe.y+170,180,48},TEXT("Equipe / Save"))){if(Flow.new_attempt_required()){Session.reset();Flow.acknowledge_new_attempt();}TeamMenuPage=0;HubPanelPage=0;DraftTeam=Session.team().selected;Flow.open_team();return;}
        DrawText(TEXT("Hub indisponível. A batalha continua acessível."),FColor::White,static_cast<float>(layout.safe.x+20),static_cast<float>(layout.safe.y+40));
        DrawText(HubError,FColor::Orange,static_cast<float>(layout.safe.x+20),static_cast<float>(layout.safe.y+65));
        if(button({layout.safe.x+20,layout.safe.y+110,180,48},TEXT("Abrir batalha"))){enterBattle();}return;
    }
    auto plan=plan_hub({static_cast<uint32>(HubTexture->GetSizeX()),static_cast<uint32>(HubTexture->GetSizeY())},
        {static_cast<uint32>(layout.scene.width),static_cast<uint32>(layout.scene.height)},DeviceProfile(Preferences.mode),HubArea,HubView,HubAspectThreshold,HubRegions);
    plan.content.x+=layout.scene.x;plan.content.y+=layout.scene.y;
    bool panoramaMoving=false;
    if(preview3D || HubReducedMotion || Flow.screen()!=UiScreen::Hub){
        PanoramaFrom.reset();
        PanoramaTarget.reset();
        PanoramaDragBase.reset();PanoramaDragPlan.reset();PanoramaDragging=false;
    }else{
        const double now=GetWorld()->GetTimeSeconds();
        constexpr double duration=0.45;
        const auto blendRect=[](const Rect& from,const Rect& to,double t){
            return Rect{FMath::Lerp(from.x,to.x,t),FMath::Lerp(from.y,to.y,t),
                FMath::Lerp(from.width,to.width,t),FMath::Lerp(from.height,to.height,t)};
        };
        const auto sameRect=[](const Rect& a,const Rect& b){
            return a.x==b.x && a.y==b.y && a.width==b.width && a.height==b.height;
        };
        const auto displayed=[&](){
            auto current=*PanoramaTarget;
            if(PanoramaFrom){
                const double t=FMath::SmoothStep(0.0,1.0,(now-PanoramaStarted)/duration);
                current.source_uv=blendRect(PanoramaFrom->source_uv,current.source_uv,t);
                current.content=blendRect(PanoramaFrom->content,current.content,t);
            }
            return current;
        };

        if(!PanoramaTarget || (sameRect(PanoramaTarget->source_uv,plan.source_uv)
            && !sameRect(PanoramaTarget->content,plan.content))){
            PanoramaTarget=plan;
            PanoramaFrom.reset();
        }else if(!sameRect(PanoramaTarget->source_uv,plan.source_uv)){
            PanoramaFrom=displayed();
            PanoramaTarget=plan;
            PanoramaStarted=now;
        }
        if(PanoramaFrom && now-PanoramaStarted>=duration){
            PanoramaFrom.reset();
            // A press that began on a moving image cannot select an arriving label.
            if(!PanoramaDragBase && (Input.down || Input.tap)){Pointer.cancel_until_release();Input.tap=false;}
        }
        panoramaMoving=PanoramaFrom.has_value();
        plan=displayed();
    }
    if(!preview3D && Flow.screen()==UiScreen::Hub && plan.view==HubView::Focused && !HubReducedMotion){
        const double now=GetWorld()->GetTimeSeconds();
        if(Input.pressed && contains(plan.content,Input.x,Input.y)){
            PanoramaDragBase=plan;
            PanoramaDragPlan=plan;
            PanoramaDragLastX=Input.x;
            PanoramaDragLastTime=now;
            PanoramaDragVelocity=0;
            PanoramaDragging=false;
        }
        if(PanoramaDragBase && (Input.down || Input.released) && !Input.cancelled){
            const double dx=Input.x-Input.origin_x;
            const double dy=Input.y-Input.origin_y;
            if(!PanoramaDragging && FMath::Abs(dx)>=12 && FMath::Abs(dx)>1.25*FMath::Abs(dy)){
                PanoramaDragging=true;
            }
            if(now>PanoramaDragLastTime){
                if(!Input.released || Input.x!=PanoramaDragLastX){
                    PanoramaDragVelocity=(Input.x-PanoramaDragLastX)/(now-PanoramaDragLastTime);
                    PanoramaDragLastTime=now;
                    PanoramaDragLastX=Input.x;
                }
            }
            if(PanoramaDragging){
                auto dragged=*PanoramaDragBase;
                const auto& viewport=dragged.content;
                auto& crop=dragged.source_uv;
                crop.x=FMath::Clamp(crop.x-dx*crop.width/viewport.width,0.0,1.0-crop.width);
                PanoramaDragPlan=dragged;
            }
        }
        if(PanoramaDragBase && (Input.released || Input.cancelled || !Input.down)){
            if(PanoramaDragging && PanoramaDragPlan){
                PanoramaFrom=*PanoramaDragPlan;
                PanoramaStarted=now;
                const double dx=Input.x-Input.origin_x;
                const double velocity=now-PanoramaDragLastTime<=0.10?PanoramaDragVelocity:0;
                const bool advance=FMath::Abs(dx)>=PanoramaDragBase->content.width*0.18
                    || (FMath::Abs(dx)>=24 && FMath::Abs(velocity)>=550 && velocity*dx>0);
                if(Input.released && !Input.cancelled && advance){
                    const int area=HubArea==HubArea::Left?0:HubArea==HubArea::Center?1:2;
                    const int next=FMath::Clamp(area+(dx<0?1:-1),0,2);
                    if(next!=area){
                        HubArea=next==0?HubArea::Left:next==1?HubArea::Center:HubArea::Right;
                        HubView=HubView::Focused;
                        SavePresentationPreferences();
                    }
                }
                plan=*PanoramaDragPlan;
                panoramaMoving=true;
                Input.tap=false;
                Pointer.cancel_until_release();
            }
            PanoramaDragBase.reset();
            PanoramaDragPlan.reset();
            PanoramaDragging=false;
        }else if(PanoramaDragging && PanoramaDragPlan){
            plan=*PanoramaDragPlan;
            panoramaMoving=true;
            Input.tap=false;
        }
    }

    if(preview3D || HubReducedMotion || Flow.screen()!=UiScreen::Hub || !PanoramaLabels){
        PanoramaLabels=plan;
        PanoramaLabelOpacity=1.f;
    }else if(panoramaMoving){
        PanoramaLabelOpacity=FMath::Max(0.f,PanoramaLabelOpacity-GetWorld()->GetDeltaSeconds()/0.14f);
    }else{
        PanoramaLabels=plan;
        PanoramaLabelOpacity=FMath::Min(1.f,PanoramaLabelOpacity+GetWorld()->GetDeltaSeconds()/0.18f);
    }
    const auto& r=plan.content;const auto& uv=plan.source_uv;
    if(!preview3D){DrawTexture(HubTexture,static_cast<float>(r.x),static_cast<float>(r.y),static_cast<float>(r.width),static_cast<float>(r.height),
        static_cast<float>(uv.x),static_cast<float>(uv.y),static_cast<float>(uv.width),static_cast<float>(uv.height),FLinearColor::White,BLEND_Opaque);}
    const auto line=[&](double x1,double y1,double x2,double y2,FLinearColor color,double stroke){
        DrawLine(static_cast<float>(x1),static_cast<float>(y1),static_cast<float>(x2),static_cast<float>(y2),color,static_cast<float>(stroke));
    };
    if(HubLogo){DrawTexture(HubLogo,static_cast<float>(layout.header.x+12),static_cast<float>(layout.header.y+4),48,48,0,0,1,1,FLinearColor::White,BLEND_Translucent);}
    DrawText(HubTitle,HubCyan.ToFColorSRGB(),static_cast<float>(layout.header.x+68),static_cast<float>(layout.header.y+4),HubTitleFont);
    DrawText(TEXT("CODINOME: ")+HubCodename,FColor::White,static_cast<float>(layout.header.x+68),static_cast<float>(layout.header.y+28),HubBodyFont,0.75f);
    if(Flow.screen()==UiScreen::Options){DrawPreferences();return;}
    if(Flow.screen()==UiScreen::Hub && button({layout.safe.x+12,layout.header.y+112,160,48},Hub3DEnabled?TEXT("Voltar ao panorama"):TEXT("Ver hub 3D"))){
        Hub3DEnabled=!Hub3DEnabled;
        if(!Hub3DEnabled){RestoreHub3DCamera();}
        Pointer.cancel_until_release();
        return;
    }
    if(Flow.screen()==UiScreen::Hub && button({layout.safe.x+184,layout.header.y+60,160,48},TEXT("Rever abertura"))){Message=TEXT("Preparando abertura local...");StartIntro();return;}
    if(Flow.screen()==UiScreen::Hub && button({layout.safe.x+12,layout.header.y+60,160,48},TEXT("Equipe / Save"))){
        if(Flow.new_attempt_required()){Session.reset();Flow.acknowledge_new_attempt();}
        TeamMenuPage=0;HubPanelPage=0;DraftTeam=Session.team().selected;TeamStatus=TEXT("Seleção atual. Aplicar inicia nova tentativa; salvar usa a equipe já aplicada.");Flow.open_team();return;
    }
    if(Flow.screen()==UiScreen::Location && HubPanelRoute==HubRoute::SpecialEvent){
        TArray<FString> text={TEXT("A BOLA PERDIDA — EVENTO CLASSICO"),TEXT("Nas nuvens, um garoto chuta e o jovem Lobo espalma a bola para a Terra. Com as maes proibindo a descida, as criancas organizam uma busca secreta."),TEXT("Recupere a bola antes que a tempestade atinja100%. Tres acoes proprias de busca; nao altera o combate padrao."),TEXT("Parametros provisorios do laboratorio: busca+25; concentracao+25na proxima busca; tempestade+15poracao; avancar reduz20antes do avanco. Dialogo em50%. Sem economia ou save de combate; conclusao local em arquivo proprio.")};
        const auto result=DrawInfoPanel(TEXT("LAB - EXP | EVENTOS ESPECIAIS"),text,HubPanelPage,true,TEXT("Iniciar busca classica"),TEXT("Voltar ao hub"));
        if(result==1){Flow.back(false,false);}else if(result==2){Search.reset();SearchActive=true;HubPanelPage=0;Pointer.cancel_until_release();}return;
    }
    if(preview3D && Flow.screen()==UiScreen::Location && HubPanelRoute==HubRoute::Terminal && Hub3DApproach<0.99){return;}
    if(Flow.screen()==UiScreen::Location){
        if(HubPanelRoute==HubRoute::Terminal){
            const double x=layout.safe.x+12,w=std::max(48.0,std::min(400.0,layout.safe.width-24)),y=layout.header.y+104;
            const auto enterPve=[&](rpg::presentation::PveLabKind kind){
                auto& selected=PveLabs[static_cast<std::size_t>(kind)];
                if(!selected){selected=std::make_unique<rpg::presentation::PveVisualSession>(rpg::presentation::pve_lab_definition(kind));}
                else if(selected->finished() || selected->state().stopped){selected->reset();}
                if(Flow.new_attempt_required()){Session.reset();}Flow.acknowledge_new_attempt();
                if(Flow.enter_arena(selected->finished())){TutorialActive=false;PveActive=true;SelectedPveLab=kind;WavesActive=false;SequenceActive=false;SharedActive=false;SelectedSlot=0;SelectedTarget.Empty();Gesture.cancel();Pointer.cancel_until_release();RefreshArenaActors();NextAutomatic=GetWorld()->GetTimeSeconds()+0.5;Message=TEXT("Teste local; consulte objetivo e limites no HUD.");}
            };
            if(TerminalMenuPage!=3 && button({x,y+290,w,48},TEXT("Tutorial A Bola Perdida >"))){TerminalMenuPage=3;Pointer.cancel_until_release();return;}
            DrawText(FString::Printf(TEXT("TERMINAL — TESTES LOCAIS %d/4"),TerminalMenuPage+1),FColor::Cyan,static_cast<float>(x),static_cast<float>(y-35),HubBodyFont,0.8f);
            if(TerminalMenuPage==3){
                if(button({x,y,w,48},TEXT("Tutorial padrao — A Bola Perdida"))){
                    if(!Tutorial){Tutorial=std::make_unique<rpg::presentation::TutorialSession>();}else{Tutorial->reset();}
                    Flow.acknowledge_new_attempt();
                    if(Flow.enter_arena(false)){TutorialActive=true;PveActive=false;WavesActive=false;SequenceActive=false;SharedActive=false;HasActionFeedback=false;SelectedSlot=0;SelectedTarget.Empty();HubPanelPage=0;Gesture.cancel();Pointer.cancel_until_release();RefreshArenaActors();NextAutomatic=GetWorld()->GetTimeSeconds()+0.5;Message=TEXT("Tutorial padrao de teste; conclusao local separada, sem moedas.");}return;
                }
                if(button({x,y+54,w,48},TEXT("< Outros laboratorios"))){TerminalMenuPage=0;Pointer.cancel_until_release();return;}
                if(button({x,y+108,w,48},TEXT("Voltar ao hub"))){Flow.back(false,false);Pointer.cancel_until_release();return;}return;
            }
            if(TerminalMenuPage==0){
                if(button({x,y,w,48},TEXT("Laboratorio de equipe"))){SelectLabScenario(false);enterBattle();return;}
                if(button({x,y+54,w,48},TEXT("Ritual local — Felipe e ecos"))){enterPve(rpg::presentation::PveLabKind::Ritual);return;}
                if(button({x,y+108,w,48},TEXT("Ritual — fases do chefe"))){enterPve(rpg::presentation::PveLabKind::Phased);return;}
                if(button({x,y+162,w,48},TEXT("Ondas consecutivas — Felipe"))){
                    if(!Waves){Waves=std::make_unique<rpg::presentation::WaveVisualSession>();}else if(Waves->finished() || Waves->state().stopped){Waves->reset();}
                    if(Flow.new_attempt_required()){Session.reset();}Flow.acknowledge_new_attempt();
                    if(Flow.enter_arena(Waves->finished())){TutorialActive=false;WavesActive=true;PveActive=false;SequenceActive=false;SharedActive=false;SelectedSlot=0;SelectedTarget.Empty();Gesture.cancel();Pointer.cancel_until_release();RefreshArenaActors();NextAutomatic=GetWorld()->GetTimeSeconds()+0.5;Message=TEXT("Conclua as duas ondas; transicoes seguem WaveV2.");}return;
                }
            }else if(TerminalMenuPage==1){
                if(button({x,y,w,48},TEXT("Pontuacao — janela de 12 acoes"))){enterPve(rpg::presentation::PveLabKind::Score);return;}
                if(button({x,y+54,w,48},TEXT("Sobrevivencia — 3 turnos do chefe"))){enterPve(rpg::presentation::PveLabKind::Survival);return;}
            }
            if(TerminalMenuPage==1){
                const auto enterSequence=[&](bool phases){
                    auto& selected=SequenceLabs[phases?1:0];
                    if(!selected){selected=std::make_unique<rpg::presentation::SequentialVisualSession>(rpg::presentation::sequential_lab_definition(phases));}else if(selected->finished() || selected->state().stopped){selected->reset();}
                    if(Flow.new_attempt_required()){Session.reset();}Flow.acknowledge_new_attempt();
                    if(Flow.enter_arena(selected->finished())){TutorialActive=false;SequenceActive=true;SequencePhased=phases;WavesActive=false;PveActive=false;SharedActive=false;SelectedSlot=0;SelectedTarget.Empty();Gesture.cancel();Pointer.cancel_until_release();RefreshArenaActors();NextAutomatic=GetWorld()->GetTimeSeconds()+0.5;Message=TEXT("Chefe com HP continuo; janela de 4 acoes por equipe.");}
                };
                if(button({x,y+108,w,48},TEXT("3 equipes — fases do chefe"))){enterSequence(true);return;}
                if(button({x,y+162,w,48},TEXT("3 equipes — kit fixo do chefe"))){enterSequence(false);return;}
            }
            if(TerminalMenuPage==2){
                const auto enterShared=[&](bool global){
                    auto& selected=SharedLabs[global?1:0];
                    if(!selected){selected=std::make_unique<rpg::presentation::SharedVisualSession>(rpg::presentation::shared_lab_definition(global));}else if(selected->finished() || selected->state().stopped){selected->reset();}
                    if(Flow.new_attempt_required()){Session.reset();}Flow.acknowledge_new_attempt();
                    if(Flow.enter_arena(selected->finished())){TutorialActive=false;SharedActive=true;SharedGlobalSpecial=global;SequenceActive=false;WavesActive=false;PveActive=false;SelectedSlot=0;SelectedTarget.Empty();Gesture.cancel();Pointer.cancel_until_release();RefreshArenaActors();NextAutomatic=GetWorld()->GetTimeSeconds()+0.5;Message=TEXT("Tres frentes ativas, um chefe. Consulte escopos e proximo ataque local.");}
                };
                if(button({x,y,w,48},TEXT("3 frentes — ataques locais"))){enterShared(false);return;}
                if(button({x,y+54,w,48},TEXT("3 frentes — especial global"))){enterShared(true);return;}
            }
            const double nav=(w-8)/2;
            if(button({x,y+216,nav,48},TerminalMenuPage==0?TEXT("Mais modos >"):TerminalMenuPage==1?TEXT("Frentes locais >"):TEXT("< Primeiros modos"))){TerminalMenuPage=(TerminalMenuPage+1)%3;Pointer.cancel_until_release();return;}
            if(button({x+nav+8,y+216,nav,48},TEXT("Voltar"))){Flow.back(false,false);return;}return;
        }
        TArray<FString> paragraphs;paragraphs.Add(HubPanelBody);
        const int32 result=DrawInfoPanel(HubPanelTitle,paragraphs,HubPanelPage,HubPanelRoute==HubRoute::Terminal || HubPanelRoute==HubRoute::Battle,HubPanelRoute==HubRoute::Battle?TEXT("Testar Felipe (local)"):TEXT("Abrir batalha"));
        if(result==1){Flow.back(false,false);}else if(result==2){SelectLabScenario(HubPanelRoute==HubRoute::Battle);enterBattle();}return;
    }
    const auto openDestination=[&](const HubHotspot& link){
        if(link.route==HubRoute::Battle){enterBattle();return;}
        HubPanelRoute=link.route;HubPanelTitle=Text(link.label);
        if(link.id=="arena-pvp"){HubPanelRoute=HubRoute::Battle;HubPanelTitle=TEXT("Arena PvP — piloto local temporario");HubPanelBody=TEXT("Acesso de teste ao piloto Felipe contra ECO-1 e ECO-2 controlados localmente. A1 ativa, A2 passiva e A3 em area. Nao e PvP, nao ha jogadores remotos ou servico online. Progresso e save separados do laboratorio do Terminal.");}
        else if(link.route==HubRoute::Terminal){
            HubPanelBody=TEXT("Terminal de destinos. Os modais de transporte e seus destinos serão definidos em outro bloco. Você pode acessar abaixo a batalha de laboratório atual; ela não representa História, PvP ou viagem já implementados.");
        }else if(link.route==HubRoute::SpecialEvent){HubPanelTitle=HubEventTitle;HubPanelBody=HubEventBody;}
        else{HubPanelBody=TEXT("Em breve. Este local está mapeado no panorama, mas seu sistema ainda não foi implementado. Nenhuma compra, recompensa, partida online ou alteração de progresso será executada aqui.");}
        HubPanelPage=0;TerminalMenuPage=0;Flow.open_location();
    };
    if(preview3D){if(const auto* destination=DrawHub3DScene(layout.scene)){openDestination(*destination);return;}}
    if(!preview3D && PanoramaLabelOpacity>0.f) for(std::size_t index=0;index<HubLinks.size();++index){
        const auto& link=HubLinks[index];
        const auto projected=project_hotspot(*PanoramaLabels,link);if(!projected){continue;}
        const FString labelText=Text(rpg::presentation::hub_uppercase(link.label));
        float tw=0,th=0;GetTextSize(labelText,tw,th,HubLabelFont?HubLabelFont.Get():HubTitleFont.Get());
        const double scale=std::min(0.9,(r.width-24)/std::max(1.0,static_cast<double>(tw)));
        const double width=std::min(tw*scale+24,r.width),height=std::max(32.0,th*scale+12.0);
        const Rect label{std::clamp(projected->label_x-width/2,r.x,r.x+r.width-width),
            std::clamp(projected->label_y-height/2,r.y,r.y+r.height-height),width,height};
        const auto hit=minimum_target(label,r,48);if(!hit){continue;}
        const bool hover=!panoramaMoving && PanoramaLabelOpacity>=1.f && InputWindowActive && contains(*hit,mx,my);
        if(Preferences.high_contrast){DrawRect(HubPanel,static_cast<float>(label.x),static_cast<float>(label.y-3),static_cast<float>(label.width),static_cast<float>(label.height+6));}
        if(Preferences.high_contrast){line(label.x+5,label.y+label.height,label.x+label.width-5,label.y+label.height,HubCyan,1);}
        DrawAnimatedHubLabel(link,index,labelText,label.x+12,label.y+6,scale,tw,th,hover,PanoramaLabelOpacity);
        if(!panoramaMoving && PanoramaLabelOpacity>=1.f && pointer_clicked(Input,*hit)){openDestination(link);return;}
    }
    // Fixed dock keeps every destination reachable when its source region is cropped.

    for(std::size_t index=0;index<HubLinks.size();++index){
        const auto& link=HubLinks[index];
        const Rect& tile=layout.destinations[index];
        const bool hover=contains(tile,mx,my);
        DrawRect(HubPanel,static_cast<float>(tile.x),static_cast<float>(tile.y),static_cast<float>(tile.width),static_cast<float>(tile.height));
        const FString text=Text(rpg::presentation::hub_uppercase(link.label));
        float tw=0,th=0;GetTextSize(text,tw,th,HubBodyFont);
        const float scale=static_cast<float>(std::min(0.85,(tile.width-12)/std::max(1.0,static_cast<double>(tw))));
        DrawText(text,(hover?HubAmber:FLinearColor::White).ToFColorSRGB(),static_cast<float>(tile.x+(tile.width-tw*scale)/2),static_cast<float>(tile.y+4),HubBodyFont,scale);
        DrawText(link.id=="arena-pvp"?TEXT("Teste local"):link.route==HubRoute::Unavailable?TEXT("Em breve"):TEXT("Abrir"),FColor(180,190,200),static_cast<float>(tile.x+6),static_cast<float>(tile.y+25),HubBodyFont,0.55f);
        if(pointer_clicked(Input,tile)){openDestination(link);return;}
    }
    if(button(layout.navigation[0],TEXT("Esquerda")) || PlayerOwner->WasInputKeyJustPressed(EKeys::Left)){HubArea=rpg::presentation::HubArea::Left;HubView=rpg::presentation::HubView::Focused;SavePresentationPreferences();}
    if(button(layout.navigation[1],TEXT("Centro"))){HubArea=rpg::presentation::HubArea::Center;HubView=rpg::presentation::HubView::Focused;SavePresentationPreferences();}
    if(button(layout.navigation[2],TEXT("Direita")) || PlayerOwner->WasInputKeyJustPressed(EKeys::Right)){HubArea=rpg::presentation::HubArea::Right;HubView=rpg::presentation::HubView::Focused;SavePresentationPreferences();}
    if(button(layout.navigation[3],(preview3D?!focused3D:plan.view==rpg::presentation::HubView::Panoramic)?TEXT("Focar área"):TEXT("Panorama"))){
        HubView=(preview3D?!focused3D:plan.view==rpg::presentation::HubView::Panoramic)?rpg::presentation::HubView::Focused:rpg::presentation::HubView::Panoramic;SavePresentationPreferences();
    }
    if(button(layout.navigation[4],TEXT("Opções"))){Flow.open_options();}
}




void AProbeArenaHUD::DrawTeamSetup(){
    const auto frame=plan_hub_frame({static_cast<uint32>(Canvas->SizeX),static_cast<uint32>(Canvas->SizeY)},0,HubInsets);
    if(!frame){return;}
    const auto& safe=frame->safe;
    if(!PopupLines.IsEmpty()){if(DrawInfoPanel(PopupTitle,PopupLines,PopupPage)==1){PopupLines.Empty();}return;}
    DrawRect(HubPanel,static_cast<float>(safe.x),static_cast<float>(safe.y),static_cast<float>(safe.width),static_cast<float>(safe.height));
    DrawText(TEXT("EQUIPE E PROGRESSO LOCAL"),FColor::Cyan,static_cast<float>(safe.x+12),static_cast<float>(safe.y+8),HubBodyFont);
    const auto team=Session.team();
    // One catalog member per page keeps touch targets usable in small viewports.
    const int32 count=static_cast<int32>(team.available.size());
    HubPanelPage=FMath::Clamp(HubPanelPage,0,FMath::Max(0,count-1));
    const double width=std::max(48.0,safe.width-24);
    const double row=52;
    const int32 capacity=FMath::Max(1,static_cast<int32>((safe.height-140)/row));
    const int32 pages=(6+capacity-1)/capacity;
    TeamMenuPage=FMath::Clamp(TeamMenuPage,0,pages-1);
    const auto footer=[&](int index,const FString& label){
        Rect rect{safe.x+12+index*(width/3),safe.y+safe.height-56,width/3-4,48};
        DrawRect(FLinearColor(0.08f,0.20f,0.32f),static_cast<float>(rect.x),static_cast<float>(rect.y),static_cast<float>(rect.width),48);
        float tw=0,th=0;GetTextSize(label,tw,th,HubBodyFont);const float scale=static_cast<float>(std::min(1.0,(rect.width-8)/std::max(1.0,static_cast<double>(tw))));
        DrawText(label,FColor::White,static_cast<float>(rect.x+4),static_cast<float>(rect.y+12),HubBodyFont,scale);
        return pointer_clicked(Input,rect);
    };
    if(footer(0,TEXT("Voltar"))){Flow.back(false,false);return;}
    if(footer(1,TEXT("Anterior"))){TeamMenuPage=FMath::Max(0,TeamMenuPage-1);return;}
    if(footer(2,TEXT("Próxima"))){TeamMenuPage=FMath::Min(pages-1,TeamMenuPage+1);return;}
    DrawText(FString::Printf(TEXT("Rascunho: %d | Página %d/%d"),static_cast<int32>(DraftTeam.size()),TeamMenuPage+1,pages),FColor::White,static_cast<float>(safe.x+12),static_cast<float>(safe.y+28),HubBodyFont,0.7f);
    const auto button=[&](int index,const FString& label){
        if(index/capacity!=TeamMenuPage){return false;}
        Rect rect{safe.x+12,safe.y+52+(index%capacity)*row,width,48};
        if(rect.y+rect.height>safe.y+safe.height){return false;}
        DrawRect(FLinearColor(0.08f,0.20f,0.32f),static_cast<float>(rect.x),static_cast<float>(rect.y),static_cast<float>(rect.width),48);
        float tw=0,th=0;GetTextSize(label,tw,th,HubBodyFont);
        const float scale=static_cast<float>(std::min(1.0,(width-12)/std::max(1.0,static_cast<double>(tw))));
        DrawText(label,FColor::White,static_cast<float>(rect.x+6),static_cast<float>(rect.y+10),HubBodyFont,scale);
        return pointer_clicked(Input,rect);
    };
    if(count>0){
        const auto& id=team.available[HubPanelPage];const auto found=std::ranges::find(DraftTeam,id);
        const bool selected=found!=DraftTeam.end();
        if(button(0,FString::Printf(TEXT("%s %s (%d/%d)"),selected?TEXT("[X]"):TEXT("[ ]"),*Text(id),HubPanelPage+1,count))){
            if(selected){DraftTeam.erase(found);}else{DraftTeam.push_back(id);}return;
        }
        if(button(1,TEXT("Próximo personagem"))){HubPanelPage=(HubPanelPage+1)%count;return;}
    }
    if(button(2,TEXT("Aplicar equipe (nova tentativa)"))){
        try{const auto error=Session.select_team(DraftTeam);if(error==rpg::SelectionError::None){RefreshArenaActors();SelectedSlot=0;SelectedTarget.Empty();Gesture.cancel();}TeamStatus=error==rpg::SelectionError::None?TEXT("Equipe aplicada; progresso preservado."):TEXT("Equipe inválida: selecione de 1 a 5 IDs disponíveis, sem repetição.");}
        catch(const std::exception& e){TeamStatus=Text(e.what());}OpenInfo(TEXT("Equipe / Save"),{Utf8(TeamStatus)});return;
    }
    const FString directory=FPaths::Combine(FPaths::ProjectSavedDir(),TEXT("LocalProgress"));
    const bool pilot=SessionIsFelipe;
    const FString filename=pilot?(PilotTutorial?TEXT("felipe-tutorial-v1.save"):TEXT("felipe-pilot-v1.save")):TEXT("progress-v1.save");
    UnrealSaveFiles files(directory,true);
    if(button(3,TEXT("Salvar progresso aplicado"))){
        try{
            const auto bytes=Session.export_save();
            rpg::storage::save_progress(files,StorageKeys(filename),bytes);
            TeamStatus=FString::Printf(TEXT("Progresso salvo em Saved/LocalProgress/%s."),*filename);
        }catch(const std::exception& e){TeamStatus=Text(e.what());}OpenInfo(TEXT("Equipe / Save"),{Utf8(TeamStatus)});return;
    }
    if(button(4,TEXT("Carregar progresso (nova tentativa)"))){
        try{
            Session.import_save(rpg::storage::load_progress(files,StorageKeys(filename)));
            RefreshArenaActors();SelectedSlot=0;SelectedTarget.Empty();Gesture.cancel();DraftTeam=Session.team().selected;TeamStatus=TEXT("Progresso carregado. Nova tentativa; batalha anterior não é restaurada.");
        }catch(const std::exception& e){TeamStatus=Text(e.what());}OpenInfo(TEXT("Equipe / Save"),{Utf8(TeamStatus)});return;
    }
    if(button(5,TEXT("Ver mensagem / resultado da operação"))){OpenInfo(TEXT("Equipe / Save"),{Utf8(TeamStatus)});return;}
}
