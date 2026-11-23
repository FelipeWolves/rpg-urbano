#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameFramework/GameModeBase.h"
#include "rpg/battle_presentation.hpp"
#include "rpg/pve_visual.hpp"
#include "rpg/tutorial.hpp"
#include <array>
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
    virtual void EndPlay(const EEndPlayReason::Type reason) override;
private:
    rpg::presentation::VisualSession Session;
    std::unique_ptr<rpg::presentation::TutorialSession> Tutorial;
    rpg::presentation::ClassicSearchSession Search;
    bool TutorialActive=false,SearchActive=false;
    rpg::presentation::TutorialCompletion TutorialCompleted;FString TutorialSaveStatus;
    void LoadTutorialProgress();
    void RecordTutorialCompletion(bool classic);
    UPROPERTY(Transient) TMap<FString,TObjectPtr<UTexture2D>> UiTextures;
    UPROPERTY(Transient) TObjectPtr<UFont> HubLabelFont=nullptr;
    TMap<FString,float> HubLabelHover;
    TMap<FString,FVector4> LabelGlowGeometry;
    TMap<FString,FString> LabelGlowText;
    void LoadUiResources();
    UTexture2D* UiTexture(const FString& id)const;
    void DrawTutorialLesson();
    void DrawClassicSearch();
    rpg::presentation::ActionFeedback LastActionFeedback;
    bool HasActionFeedback=false;double VigilFlashUntil=0;FString VigilFlashActor;
    void ShowActionFeedback(const rpg::presentation::VisualResult& result);
    std::array<std::unique_ptr<rpg::presentation::SharedVisualSession>,2> SharedLabs;
    bool SharedActive=false,SharedGlobalSpecial=false;
    rpg::presentation::SharedVisualSession* ActiveShared(){return SharedLabs[SharedGlobalSpecial?1:0].get();}
    const rpg::presentation::SharedVisualSession* ActiveShared()const{return SharedLabs[SharedGlobalSpecial?1:0].get();}
    std::vector<std::string> ActiveTargets(const rpg::presentation::VisualState& view,std::uint32_t slot)const{return SharedActive?ActiveShared()->targets(slot):rpg::presentation::target_candidates(view,slot);}
    std::array<std::unique_ptr<rpg::presentation::PveVisualSession>,4> PveLabs;
    std::array<std::unique_ptr<rpg::presentation::SequentialVisualSession>,2> SequenceLabs;
    bool SequenceActive=false,SequencePhased=true;
    int32 SequenceGeometryIndex=-1;
    rpg::presentation::SequentialVisualSession* ActiveSequence(){return SequenceLabs[SequencePhased?1:0].get();}
    const rpg::presentation::SequentialVisualSession* ActiveSequence()const{return SequenceLabs[SequencePhased?1:0].get();}
    std::unique_ptr<rpg::presentation::WaveVisualSession> Waves;
    bool PveActive=false,WavesActive=false;
    rpg::presentation::PveLabKind SelectedPveLab=rpg::presentation::PveLabKind::Ritual;
    int32 WaveGeometryIndex=-1,TerminalMenuPage=0;
    rpg::presentation::PveVisualSession* ActivePve(){return PveLabs[static_cast<std::size_t>(SelectedPveLab)].get();}
    const rpg::presentation::PveVisualSession* ActivePve()const{return PveLabs[static_cast<std::size_t>(SelectedPveLab)].get();}
    rpg::presentation::VisualState ActiveState()const{return TutorialActive?Tutorial->state():SharedActive?ActiveShared()->state():SequenceActive?ActiveSequence()->state():WavesActive?Waves->state():PveActive?ActivePve()->state():Session.state();}
    bool ActiveFinished()const{return TutorialActive?Tutorial->finished():SharedActive?ActiveShared()->finished():SequenceActive?ActiveSequence()->finished():WavesActive?Waves->finished():PveActive?ActivePve()->finished():Session.state().combat.battle.phase==rpg::Phase::Finished;}
    void ResetActiveBattle(){if(TutorialActive){Tutorial->reset();}else if(SharedActive){ActiveShared()->reset();}else if(SequenceActive){ActiveSequence()->reset();}else if(WavesActive){Waves->reset();}else if(PveActive){ActivePve()->reset();}else{Session.reset();}HasActionFeedback=false;}
    rpg::presentation::VisualResult SubmitActiveSlot(uint64 revision,uint32 slot,std::string target){return TutorialActive?Tutorial->submit(revision,slot,std::move(target)):SharedActive?ActiveShared()->submit(revision,slot,std::move(target)):SequenceActive?ActiveSequence()->submit(revision,slot,std::move(target)):WavesActive?Waves->submit(revision,slot,std::move(target)):PveActive?ActivePve()->submit(revision,slot,std::move(target)):Session.submit(revision,slot,std::move(target));}
    rpg::presentation::VisualResult SubmitActiveAction(uint64 revision,const rpg::Action& action){return TutorialActive?Tutorial->submit_action(revision,action):SharedActive?ActiveShared()->submit_action(revision,action):SequenceActive?ActiveSequence()->submit_action(revision,action):WavesActive?Waves->submit_action(revision,action):PveActive?ActivePve()->submit_action(revision,action):Session.submit_action(revision,action);}
    rpg::presentation::VisualResult StepActiveBattle(){return TutorialActive?Tutorial->step_automatic():SharedActive?ActiveShared()->step_automatic():SequenceActive?ActiveSequence()->step_automatic():WavesActive?Waves->step_automatic():PveActive?ActivePve()->step_automatic():Session.step_automatic();}
    std::optional<rpg::presentation::VisualSession> AlternateLabSession;
    rpg::CombatProfile ReferenceProfile=rpg::CombatProfile::Team;
    bool SessionIsFelipe=false,PilotTutorial=false;
    void SelectLabScenario(bool felipe);
    UPROPERTY() TObjectPtr<class UStaticMesh> TargetMarkerMesh=nullptr;
    UPROPERTY(Transient) TObjectPtr<AActor> TargetMarkerActor=nullptr;
    UPROPERTY() TObjectPtr<class UMaterialInterface> TargetMarkerMaterial=nullptr;
    int32 TargetMarkerSegments=0;
    float TargetMarkerRadius=0;
    void UpdateTargetMarker(FVector base,float radius,int32 segments);
    void HideTargetMarker();
    void DestroyTargetMarker();
    UPROPERTY(Transient)
    TObjectPtr<UTexture2D> HubTexture=nullptr;
    UPROPERTY(Transient) TObjectPtr<UTexture2D> HubLogo=nullptr;
    UPROPERTY(Transient) TObjectPtr<UFont> HubTitleFont=nullptr;
    UPROPERTY(Transient) TObjectPtr<UFont> HubBodyFont=nullptr;
    UPROPERTY(Transient) TObjectPtr<class UMediaPlayer> IntroPlayer=nullptr;
    UPROPERTY(Transient) TObjectPtr<class UMediaTexture> IntroTexture=nullptr;
    UPROPERTY(Transient) TObjectPtr<class UMediaSoundComponent> IntroSound=nullptr;
    rpg::presentation::MediaFlow IntroFlow;
    rpg::presentation::MediaStartGate IntroStartGate;
    uint64 IntroTicket=0;
    double IntroStarted=0,IntroHold=-1;
    bool IntroActive=false;
    FString IntroWatchPath;
    FString IntroWatchToken;
    void StartIntro();
    void DrawIntro();
    void StopIntro();
    void IntroDeactivate();
    void IntroReactivate();
    UFUNCTION() void IntroEnded();
    bool HubEnabled=false;
    rpg::presentation::ScreenFlow Flow;

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

    rpg::presentation::PresentationPreferences Preferences;
    FLinearColor HubBasePanel=HubPanel;
    FString PreferencesStatus;
    void LoadPresentationPreferences();
    void SavePresentationPreferences();
    void ApplyPresentationPreferences();
    void DrawPreferences();
    void DrawTeamSetup();
    std::vector<std::string> DraftTeam;
    FString TeamStatus;
    int32 TeamMenuPage=0;
    void LoadHubCatalog();
    void DrawHub();
    double CombatCueTime=-1;
    bool CombatCueImpact=false;
    std::optional<rpg::presentation::HubPlan> PanoramaFrom, PanoramaTarget;
    double PanoramaStarted=0;
    std::optional<rpg::presentation::HubPlan> PanoramaLabels, PanoramaDragBase, PanoramaDragPlan;
    float PanoramaLabelOpacity=1.f;
    bool PanoramaDragging=false;
    double PanoramaDragLastX=0, PanoramaDragLastTime=0, PanoramaDragVelocity=0;
    void DrawAnimatedHubLabel(const rpg::presentation::HubHotspot& link,std::size_t index,const FString& labelText,float tx,float ty,float ts,float tw,float th,bool hover,float opacity=1.f);
    bool UpdateHub3D();
    const rpg::presentation::HubHotspot* DrawHub3DScene(const rpg::presentation::Rect& scene);
    void RestoreHub3DCamera();
    UPROPERTY(Transient) TObjectPtr<class ACameraActor> Hub3DCamera;
    UPROPERTY(Transient) TObjectPtr<AActor> Hub3DOriginalCamera;
    double Hub3DApproach=0;
    bool Hub3DConfigured=false,Hub3DEnabled=false;
    int32 DrawInfoPanel(const FString& title,const TArray<FString>& paragraphs,int32& page,bool action=false,const FString& actionLabel=TEXT("Batalha de laboratório"),const FString& closeLabel=TEXT("Fechar"));
    void ReturnToHub();
    void HandleBack();
    void DrawBattleMenu();
    int32 BattleMenuPage=0;
    int32 SelectedSlot=0;
    uint64 SelectionRevision=0;
    double NextAutomatic=0;
    rpg::presentation::AbilityGesture Gesture;
    rpg::presentation::PointerInput Pointer;
    rpg::presentation::PointerFrame Input;
    float LastTouchX=0,LastTouchY=0;
    rpg::presentation::CanvasMapping CanvasCoordinates;
    bool CanvasCoordinatesKnown=false;
    bool InputWindowActive=false;
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
