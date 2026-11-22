#include "ProbeArena.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "MediaSoundComponent.h"
#include "Engine/Canvas.h"
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"
#include "UnrealClient.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "Misc/CoreDelegates.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
using namespace rpg::presentation;
namespace {
FString MediaString(const TSharedPtr<FJsonObject>& object,const TCHAR* key){FString value;if(!object || !object->TryGetStringField(key,value)){throw std::runtime_error("Campo textual de midia invalido.");}return value;}
double MediaNumber(const TSharedPtr<FJsonObject>& object,const TCHAR* key){double value=0;if(!object || !object->TryGetNumberField(key,value)){throw std::runtime_error("Campo numerico de midia invalido.");}return value;}
}
void AProbeArenaHUD::StartIntro(){
    StopIntro();
    // Isolated opt-in from the hub. No startup autoplay before playback acceptance.
    try{
        const FString root=FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()/TEXT("Presentation"));
        const FString catalogPath=root/TEXT("data/presentation/startup.json");
        const auto size=IFileManager::Get().FileSize(*catalogPath);
        FString json;TSharedPtr<FJsonObject> catalog;
        if(size<=0 || size>1024*1024 || !FFileHelper::LoadFileToString(json,*catalogPath) || !FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(json),catalog) || MediaNumber(catalog,TEXT("schemaVersion"))!=1){throw std::runtime_error("Catalogo de midia indisponivel.");}
        Variants variants;
        const TArray<TSharedPtr<FJsonValue>>* entries=nullptr;if(!catalog->TryGetArrayField(TEXT("assets"),entries)){throw std::runtime_error("Lista de midia ausente.");}
        for(const auto& entry:*entries){
            auto object=entry->AsObject();if(!object){continue;}
            const FString role=MediaString(object,TEXT("role"));
            if(role!=TEXT("IntroDesktop") && role!=TEXT("IntroMobile")){continue;}
            if(MediaString(object,TEXT("status"))!=TEXT("Ready")){continue;}
            const double w=MediaNumber(object,TEXT("width")),h=MediaNumber(object,TEXT("height"));
            if(!FMath::IsFinite(w) || !FMath::IsFinite(h) || w<1 || h<1 || w>8192 || h>8192 || w!=FMath::FloorToDouble(w) || h!=FMath::FloorToDouble(h)){throw std::runtime_error("Dimensoes de video invalidas.");}
            Asset asset{TCHAR_TO_UTF8(*MediaString(object,TEXT("id"))),TCHAR_TO_UTF8(*MediaString(object,TEXT("path"))),{static_cast<uint32>(w),static_cast<uint32>(h)}};
            validate_asset(asset);
            if(asset.id!=(role==TEXT("IntroMobile")?"intro-mobile":"intro-desktop")){throw std::runtime_error("ID de abertura invalido.");}
            auto& target=role==TEXT("IntroMobile")?variants.mobile:variants.desktop;
            if(target){throw std::runtime_error("Variante de video duplicada.");}target=std::move(asset);
        }
        const bool mobile=Preferences.mode==PresentationMode::Mobile || (Preferences.mode==PresentationMode::Auto && Canvas && Canvas->SizeY>Canvas->SizeX);
        const auto* asset=select_variant(variants,mobile?Device::Mobile:Device::Desktop);
        if(!asset){throw std::runtime_error("Nenhuma abertura local cadastrada.");}
        const FString path=FPaths::ConvertRelativePathToFull(root/UTF8_TO_TCHAR(asset->relative_path.c_str()));
        if(!FPaths::IsUnderDirectory(path,root) || IFileManager::Get().FileSize(*path)<=0){throw std::runtime_error("Arquivo de abertura ausente.");}
        // Versioned per variant; checkpoint is separate from battle and UI preferences.
        IntroWatchPath=FPaths::ProjectSavedDir()/TEXT("Media")/(FString(UTF8_TO_TCHAR(asset->id.c_str()))+TEXT("-v1.flag"));
        FString flag;const bool watched=IFileManager::Get().FileSize(*IntroWatchPath)==13 && FFileHelper::LoadFileToString(flag,*IntroWatchPath) && flag==TEXT("INTRO_V1_SEEN");
        IntroFlow.background(false);IntroTicket=IntroFlow.begin(*asset,watched);
        UE_LOG(LogTemp,Display,TEXT("INTRO_REQUEST ticket=%llu watched=%d"),IntroTicket,watched?1:0);
        IntroPlayer=NewObject<UMediaPlayer>(this);IntroPlayer->PlayOnOpen=false;IntroPlayer->SetLooping(false);
        IntroPlayer->OnEndReached.AddDynamic(this,&AProbeArenaHUD::IntroEnded);
        IntroTexture=NewObject<UMediaTexture>(this);IntroTexture->SetMediaPlayer(IntroPlayer);IntroTexture->UpdateResource();
        IntroSound=NewObject<UMediaSoundComponent>(this);IntroSound->SetMediaPlayer(IntroPlayer);IntroSound->RegisterComponent();IntroSound->SetVolumeMultiplier(1.0f);
        FCoreDelegates::ApplicationWillDeactivateDelegate.AddUObject(this,&AProbeArenaHUD::IntroDeactivate);
        FCoreDelegates::ApplicationHasReactivatedDelegate.AddUObject(this,&AProbeArenaHUD::IntroReactivate);
        IntroActive=true;IntroStarted=FPlatformTime::Seconds();IntroHold=-1;Pointer.cancel_until_release();
        if(!IntroPlayer->OpenFile(path)){IntroFlow.fail(IntroTicket,MediaFailure::OpenFailed);}
    }catch(const std::exception&){StopIntro();Message=TEXT("Abertura indisponível; hub preservado.");OpenInfo(TEXT("Mídia local"),{"Abertura indisponivel. Confira os arquivos locais; o hub continua acessivel."});}
}
void AProbeArenaHUD::IntroDeactivate(){if(IntroFlow.state()==MediaState::Playing){UE_LOG(LogTemp,Display,TEXT("INTRO_BACKGROUND_PAUSE ticket=%llu"),IntroTicket);}IntroFlow.background(true);IntroHold=-1;if(IntroPlayer){IntroPlayer->Pause();}}
void AProbeArenaHUD::IntroReactivate(){IntroFlow.background(false);}
void AProbeArenaHUD::IntroEnded(){IntroFlow.ended(IntroTicket);}
void AProbeArenaHUD::StopIntro(){
    FCoreDelegates::ApplicationWillDeactivateDelegate.RemoveAll(this);FCoreDelegates::ApplicationHasReactivatedDelegate.RemoveAll(this);
    if(IntroPlayer){IntroPlayer->OnEndReached.RemoveAll(this);IntroPlayer->Close();}
    if(IntroSound){IntroSound->SetMediaPlayer(nullptr);IntroSound->DestroyComponent();}
    if(IntroTexture){IntroTexture->SetMediaPlayer(nullptr);}
    IntroSound=nullptr;IntroTexture=nullptr;IntroPlayer=nullptr;IntroActive=false;IntroHold=-1;IntroFlow.cancel();
}
void AProbeArenaHUD::EndPlay(const EEndPlayReason::Type reason){StopIntro();Super::EndPlay(reason);}
void AProbeArenaHUD::DrawIntro(){
    const double now=FPlatformTime::Seconds();
    const bool focused=GetWorld()->GetGameViewport() && GetWorld()->GetGameViewport()->Viewport && GetWorld()->GetGameViewport()->Viewport->IsForegroundWindow();
    if(!focused){IntroDeactivate();}else{IntroReactivate();}
    if(IntroPlayer && IntroPlayer->HasError()){IntroFlow.fail(IntroTicket,MediaFailure::DecodeFailed);}
    if(IntroFlow.state()==MediaState::Preparing){
        if(now-IntroStarted>15){IntroFlow.fail(IntroTicket,MediaFailure::Timeout);}
        else if(IntroPlayer && IntroPlayer->IsReady()){
            IntroFlow.prepared(IntroTicket);
            UE_LOG(LogTemp,Display,TEXT("INTRO_PREPARED ticket=%llu state=%d"),IntroTicket,static_cast<int32>(IntroFlow.state()));
            if(IntroFlow.state()==MediaState::Playing && !IntroPlayer->Play()){IntroFlow.fail(IntroTicket,MediaFailure::OpenFailed);}
        }
    }
    if(IntroFlow.return_to_hub()){
        UE_LOG(LogTemp,Display,TEXT("INTRO_TERMINAL ticket=%llu state=%d failure=%d"),IntroTicket,static_cast<int32>(IntroFlow.state()),static_cast<int32>(IntroFlow.failure()));
        const bool seen=IntroFlow.watched();const bool failed=IntroFlow.state()==MediaState::Failed;
        if(seen){IFileManager::Get().MakeDirectory(*FPaths::GetPath(IntroWatchPath),true);if(!FFileHelper::SaveStringToFile(TEXT("INTRO_V1_SEEN"),*IntroWatchPath,FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM)){Message=TEXT("Vídeo visto; não foi possível salvar essa preferência.");}}
        StopIntro();Pointer.cancel_until_release();
        if(failed){OpenInfo(TEXT("Mídia local"),{"Nao foi possivel reproduzir a abertura. Retorno seguro ao hub; video nao marcado como assistido."});}return;
    }
    DrawRect(FLinearColor(0.004f,0.005f,0.01f),0,0,Canvas->SizeX,Canvas->SizeY);
    if(auto plan=IntroFlow.layout({static_cast<uint32>(Canvas->SizeX),static_cast<uint32>(Canvas->SizeY)},Device::Desktop,false);plan && IntroTexture){const auto& r=plan->content;DrawTexture(IntroTexture,static_cast<float>(r.x),static_cast<float>(r.y),static_cast<float>(r.width),static_cast<float>(r.height),0,0,1,1,FLinearColor::White,BLEND_Opaque);}
    const double margin=16;Rect skip{Canvas->SizeX-196.0-HubInsets.right,margin+HubInsets.top,180,48};
    DrawRect(HubPanel,static_cast<float>(skip.x),static_cast<float>(skip.y),180,48);DrawText(IntroFlow.watched()?TEXT("Pular"):TEXT("Segure para pular"),FColor::White,static_cast<float>(skip.x+8),static_cast<float>(skip.y+10),HubBodyFont,0.8f);
    const bool key=PlayerOwner->IsInputKeyDown(EKeys::Escape) || PlayerOwner->IsInputKeyDown(EKeys::SpaceBar);
    const bool held=focused && (key || (Input.down && contains(skip,Input.x,Input.y) && contains(skip,Input.origin_x,Input.origin_y)));
    if(held){if(IntroHold<0){IntroHold=now;}IntroFlow.skip(static_cast<uint64>((now-IntroHold)*1000));}else{IntroHold=-1;}
    if(IntroFlow.watched() && pointer_clicked(Input,skip)){IntroFlow.skip(0);}
    if(IntroFlow.state()==MediaState::Paused){DrawText(TEXT("Toque para continuar"),FColor::White,20,90,HubBodyFont);if(focused && Input.tap && !contains(skip,Input.x,Input.y) && IntroFlow.resume() && !IntroPlayer->Play()){IntroFlow.fail(IntroTicket,MediaFailure::OpenFailed);}}
    if(IntroFlow.state()==MediaState::Preparing){DrawText(TEXT("Preparando vídeo local..."),FColor::White,20,90,HubBodyFont);}
}
