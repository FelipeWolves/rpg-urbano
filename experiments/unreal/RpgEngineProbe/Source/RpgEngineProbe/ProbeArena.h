#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameFramework/GameModeBase.h"
#include "rpg/battle_presentation.hpp"
#include "rpg/startup_presentation.hpp"
#include "rpg/hub_style.hpp"
#include "ProbeArena.generated.h"
UCLASS()
class RPGENGINEPROBE_API AProbeArenaHUD : public AHUD
{
    GENERATED_BODY()
public:
    AProbeArenaHUD();
    virtual void DrawHUD() override;
    virtual void BeginPlay() override;
private:
    rpg::presentation::VisualSession Session;
    UPROPERTY(Transient)
    TObjectPtr<UTexture2D> HubTexture=nullptr;
    UPROPERTY(Transient) TObjectPtr<UTexture2D> HubLogo=nullptr;
    UPROPERTY(Transient) TObjectPtr<UFont> HubTitleFont=nullptr;
    UPROPERTY(Transient) TObjectPtr<UFont> HubBodyFont=nullptr;
    bool HubEnabled=false;
    bool InHub=false;
    bool HubEventOpen=false;
    rpg::presentation::HubRoute HubPanelRoute=rpg::presentation::HubRoute::SpecialEvent;
    FString HubPanelTitle;
    FString HubPanelBody;
    int32 HubPanelPage=0;
    rpg::presentation::HubArea HubArea=rpg::presentation::HubArea::Center;
    rpg::presentation::HubView HubView=rpg::presentation::HubView::Auto;
    rpg::presentation::HubRegions HubRegions;
    double HubAspectThreshold=1.5;
    rpg::presentation::SafeInsets HubInsets;
    std::vector<rpg::presentation::HubHotspot> HubLinks;
    FString HubError;
    FString HubTitle=TEXT("Mundus Lupis Disco");
    FString HubCodename=TEXT("LOBO");
    FString HubEventTitle=TEXT("EVENTO ESPECIAL — EM PREPARAÇÃO");
    FString HubEventBody=TEXT("Conteúdo em preparação.");
    FLinearColor HubMagenta=FLinearColor::FromSRGBColor(FColor(217,70,239));
    FLinearColor HubCyan=FLinearColor::FromSRGBColor(FColor(34,211,238));
    FLinearColor HubAmber=FLinearColor::FromSRGBColor(FColor(245,158,11));
    FLinearColor HubCrimson=FLinearColor::FromSRGBColor(FColor(220,38,38));
    FLinearColor HubPanel=FLinearColor(0.005f,0.009f,0.025f,0.8f);
    rpg::presentation::PulseSettings HubPulse;
    bool HubReducedMotion=false;
    bool HubReducedMotionForced=false;
    bool PreferencesOpen=false;
    rpg::presentation::PresentationPreferences Preferences;
    FLinearColor HubBasePanel=HubPanel;
    FString PreferencesStatus;
    void LoadPresentationPreferences();
    void SavePresentationPreferences();
    void ApplyPresentationPreferences();
    void DrawPreferences();
    void LoadHubCatalog();
    void DrawHub();
    int32 DrawInfoPanel(const FString& title,const TArray<FString>& paragraphs,int32& page,bool action=false);
    void ReturnToHub();
    int32 SelectedSlot=0;
    uint64 SelectionRevision=0;
    double NextAutomatic=0;
    rpg::presentation::AbilityGesture Gesture;
    rpg::presentation::PointerInput Pointer;
    rpg::presentation::PointerFrame Input;
    float LastTouchX=0,LastTouchY=0;
    rpg::presentation::CanvasMapping CanvasCoordinates;
    bool CanvasCoordinatesKnown=false;
    void ReadPointer();
    bool InspectionMode=false;
    int32 InspectionIndex=0;
    std::vector<rpg::presentation::InspectionFrame> InspectionFrames;
    void RefreshArenaActors();
    void DrawInspectionControls();
    FString SelectedTarget;
    FString PopupTitle;
    TArray<FString> PopupLines;
    int32 PopupPage=0;
    void OpenInfo(const FString& title,const std::vector<std::string>& lines);
    FString Message=TEXT("Choose an ability, then a target.");
};
UCLASS()
class RPGENGINEPROBE_API AProbeArenaMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    AProbeArenaMode();
    virtual void BeginPlay() override;
};

