#include "ProbeArena.h"
#include <algorithm>
#include "Camera/CameraActor.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Misc/ConfigCacheIni.h"
#include "rpg/arena_layout.hpp"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/Canvas.h"
#include "Widgets/SViewport.h"
#include "Engine/Texture2D.h"
#include "CanvasTypes.h"

#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"
#include "UnrealClient.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "InputCoreTypes.h"
#include "UObject/ConstructorHelpers.h"
namespace {
FString Label(const std::string& value){return UTF8_TO_TCHAR(value.c_str());}
rpg::CombatProfile SelectedCombatProfile(){FString name;if(FParse::Value(FCommandLine::Get(),TEXT("CombatProfile="),name) && name==TEXT("Classic")){return rpg::CombatProfile::Classic;}if(!name.IsEmpty() && name!=TEXT("Team")){UE_LOG(LogTemp,Warning,TEXT("Unknown CombatProfile; using Team"));}return rpg::CombatProfile::Team;}
rpg::EncounterSetup SelectedEncounter(){if(SelectedCombatProfile()==rpg::CombatProfile::Classic){return rpg::classic_reference_setup();}if(FParse::Param(FCommandLine::Get(),TEXT("FelipePilot"))){return rpg::felipe_pilot_setup(FParse::Param(FCommandLine::Get(),TEXT("FelipeTutorial")));}return rpg::terminal_lab_setup();}
std::optional<rpg::presentation::ArenaLayout> SelectedLayout(){FString name;if(!FParse::Value(FCommandLine::Get(),TEXT("ArenaLayout="),name)){return rpg::presentation::default_arena_layout;}return rpg::presentation::parse_arena_layout(TCHAR_TO_UTF8(*name));}
}
AProbeArenaHUD::AProbeArenaHUD():Session(SelectedEncounter(),SelectedCombatProfile()){ReferenceProfile=SelectedCombatProfile();SessionIsFelipe=ReferenceProfile!=rpg::CombatProfile::Classic && FParse::Param(FCommandLine::Get(),TEXT("FelipePilot"));PilotTutorial=SessionIsFelipe && FParse::Param(FCommandLine::Get(),TEXT("FelipeTutorial"));static ConstructorHelpers::FObjectFinder<UStaticMesh> marker(TEXT("/Engine/BasicShapes/Cube.Cube"));TargetMarkerMesh=marker.Object;static ConstructorHelpers::FObjectFinder<UMaterialInterface> markerMaterial(TEXT("/Engine/ArtTools/RenderToTexture/Materials/Debug/M_Emissive_Color.M_Emissive_Color"));TargetMarkerMaterial=markerMaterial.Object;}
AProbeArenaMode::AProbeArenaMode(){HUDClass=AProbeArenaHUD::StaticClass();DefaultPawnClass=nullptr;}
void AProbeArenaMode::BeginPlay()
{
    Super::BeginPlay();
    const auto layout=SelectedLayout();
    if(!layout){UE_LOG(LogTemp, Error, TEXT("Unknown ArenaLayout; use Standard or Classic"));return;}
    const bool classic=*layout==rpg::presentation::ArenaLayout::ClassicLateral;
    if(auto* pc=GetWorld()->GetFirstPlayerController()){
        const FVector location=classic?FVector(0,-850,560):FVector(0,-1200,1050);
        const FRotator rotation=classic?FRotator(-33,90,0):FRotator(-40,90,0);
        auto* camera=GetWorld()->SpawnActor<ACameraActor>(location,rotation);
        if(camera){pc->SetViewTarget(camera);}
        pc->bShowMouseCursor=true;
        // GameOnly queues a Slate lock/high-precision capture even if the viewport enum is changed later.
        // GameAndUI releases that lock, and temporary mouse-down capture forwards the first click.
        FInputModeGameAndUI inputMode;
        inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        inputMode.SetHideCursorDuringCapture(false);
        if(auto* viewport=GetWorld()->GetGameViewport()){inputMode.SetWidgetToFocus(viewport->GetGameViewportWidget());}
        pc->SetInputMode(inputMode);
        if(auto* viewport=GetWorld()->GetGameViewport()){viewport->SetMouseCaptureMode(EMouseCaptureMode::CaptureDuringMouseDown);viewport->SetMouseLockMode(EMouseLockMode::DoNotLock);viewport->SetHideCursorDuringCapture(false);}
    }
    UE_LOG(LogTemp, Display, TEXT("ARENA_LAYOUT %s"),classic?TEXT("Classic"):TEXT("Standard"));
}
void AProbeArenaHUD::BeginPlay()
{
    Super::BeginPlay();
    HubEnabled=true;bool startInHub=true;
    GConfig->GetBool(TEXT("RpgPrototype"),TEXT("StartInHub"),startInHub,GGameIni);
    if(FParse::Param(FCommandLine::Get(),TEXT("Hub"))){startInHub=true;}
    if(FParse::Param(FCommandLine::Get(),TEXT("Arena"))){startInHub=false;}
    Flow=rpg::presentation::ScreenFlow(startInHub?rpg::presentation::UiScreen::Hub:rpg::presentation::UiScreen::Arena);
    LoadHubCatalog();LoadUiResources();LoadTutorialProgress();
    FString inspection;
    const bool namedInspection=FParse::Value(FCommandLine::Get(),TEXT("Inspection="),inspection);
    if(namedInspection || FParse::Param(FCommandLine::Get(),TEXT("Inspection"))){
        InspectionFrames=rpg::presentation::arena_inspection_frames();
        InspectionMode=true;Flow=rpg::presentation::ScreenFlow(rpg::presentation::UiScreen::Arena);
        if(!HubEnabled){LoadHubCatalog();}
        if(!inspection.IsEmpty()){
            const auto found=std::ranges::find(InspectionFrames,TCHAR_TO_UTF8(*inspection),&rpg::presentation::InspectionFrame::id);
            if(found!=InspectionFrames.end()){InspectionIndex=static_cast<int32>(found-InspectionFrames.begin());}
            else{UE_LOG(LogTemp,Warning,TEXT("Unknown inspection case; using first read-only case"));}
        }
    }
    LoadPresentationPreferences();
    RefreshArenaActors();
}
void AProbeArenaHUD::RefreshArenaActors()
{
    DestroyTargetMarker();const auto layout=SelectedLayout();if(!layout){return;}
    for(TActorIterator<AStaticMeshActor> it(GetWorld());it;++it){if(it->Tags.Contains(FName(TEXT("RpgPrototypeActor")))){it->Tags.Empty();it->SetActorHiddenInGame(true);it->SetActorEnableCollision(false);it->Destroy();}}
    std::vector<rpg::Definition> roster;
    const auto snapshot=InspectionMode?InspectionFrames[InspectionIndex].battle:ActiveState().combat.battle;
    for(const auto& fighter:snapshot.fighters){roster.push_back(fighter.definition);}
    auto* mesh=LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    const auto placements=SharedActive?rpg::presentation::shared_arena_placements(ActiveShared()->observation()):rpg::presentation::arena_placements(roster,*layout);
    for(const auto& placement:placements){
        FActorSpawnParameters spawn;spawn.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        auto* sphere=GetWorld()->SpawnActor<AStaticMeshActor>(FVector(placement.x,placement.y,placement.z),FRotator(0,placement.facing_yaw,0),spawn);
        if(sphere && mesh){sphere->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);sphere->GetStaticMeshComponent()->SetStaticMesh(mesh);sphere->GetStaticMeshComponent()->UpdateBounds();sphere->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);sphere->GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);sphere->GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);sphere->Tags.Add(FName(*Label(placement.fighter_id)));sphere->Tags.Add(FName(TEXT("RpgPrototypeActor")));}
    }
    if(TutorialActive){WaveGeometryIndex=static_cast<int32>(Tutorial->wave_index());}else if(WavesActive){WaveGeometryIndex=static_cast<int32>(Waves->wave_index());}
    if(SequenceActive){SequenceGeometryIndex=static_cast<int32>(ActiveSequence()->squad_index());}
}
void AProbeArenaHUD::OpenInfo(const FString& title,const std::vector<std::string>& lines)
{
    PopupTitle=title;PopupLines.Empty();PopupPage=0;
    for(const auto& value:lines){
        FString text=Label(value);
        PopupLines.Add(text);
    }
    if(PopupLines.IsEmpty()){PopupLines.Add(TEXT("No information available."));}
}
void AProbeArenaHUD::ShowActionFeedback(const rpg::presentation::VisualResult& result)
{
    for(const auto& event:result.action.events){if(event.type==rpg::EventType::PassiveTriggered){VigilFlashActor=Label(event.actor);VigilFlashUntil=GetWorld()->GetTimeSeconds()+0.8;}}
    if(result.accepted() && TutorialActive && Tutorial->finished() && Tutorial->hud().objective_met){RecordTutorialCompletion(false);}
    LastActionFeedback=rpg::presentation::action_feedback(result);HasActionFeedback=true;Message=Label(LastActionFeedback.summary);
}
void AProbeArenaHUD::ReadPointer()
{
    float mx=0,my=0,tx=0,ty=0;bool touched=false;
    int32 viewportWidth=0,viewportHeight=0;PlayerOwner->GetViewportSize(viewportWidth,viewportHeight);
    const auto inViewport=[&](float x,float y){return x>=0 && y>=0 && x<viewportWidth && y<viewportHeight;};
    const bool mouseValid=PlayerOwner->GetMousePosition(mx,my) && inViewport(mx,my);
    PlayerOwner->GetInputTouchState(ETouchIndex::Touch1,tx,ty,touched);
    if(touched){LastTouchX=tx;LastTouchY=ty;}
    if(Canvas->Canvas){
        // Canvas drawing has a transform in addition to viewport coordinates:
        // player view origin, safe-zone translation and Canvas DPI scale.
        const FMatrix transform=Canvas->Canvas->GetTransform();
        const auto origin=transform.TransformPosition(FVector::ZeroVector);
        const auto axisX=transform.TransformPosition(FVector(1,0,0));
        const auto axisY=transform.TransformPosition(FVector(0,1,0));
        const double dpi=Canvas->Canvas->GetDPIScale();
        const rpg::presentation::CanvasMapping mapping{origin.X*dpi,origin.Y*dpi,
            (axisX.X-origin.X)*dpi,(axisY.X-origin.X)*dpi,(axisX.Y-origin.Y)*dpi,(axisY.Y-origin.Y)*dpi};
        if(CanvasCoordinatesKnown && !rpg::presentation::same_canvas_mapping(CanvasCoordinates,mapping)){
            Pointer.cancel_until_release();Gesture.cancel();
        }
        CanvasCoordinates=mapping;CanvasCoordinatesKnown=true;
    }
    const auto mouse=rpg::presentation::viewport_to_canvas(mx,my,CanvasCoordinates);
    const auto touch=rpg::presentation::viewport_to_canvas(LastTouchX,LastTouchY,CanvasCoordinates);
    const auto* viewport=GetWorld()->GetGameViewport();
    InputWindowActive=viewport && viewport->Viewport && rpg::presentation::canvas_input_active(viewport->Viewport->IsForegroundWindow(),viewport->Viewport->HasFocus());
    const bool focused=InputWindowActive;
    const std::uint8_t screen=SearchActive?180:TutorialActive && Tutorial->awaiting_lesson()?181:IntroActive?128:InspectionMode?(!PopupLines.IsEmpty()?201:200):static_cast<std::uint8_t>(Flow.screen())+(!PopupLines.IsEmpty()?16:0);
    Input=Pointer.update({PlayerOwner->IsInputKeyDown(EKeys::LeftMouseButton),mouseValid && mouse.has_value() && CanvasCoordinatesKnown,mouse?mouse->x:0,mouse?mouse->y:0},
        {touched,inViewport(LastTouchX,LastTouchY) && touch.has_value() && CanvasCoordinatesKnown,touch?touch->x:0,touch?touch->y:0},focused,GetWorld()->GetTimeSeconds(),{SearchActive?Search.state().revision:InspectionMode?static_cast<std::uint64_t>(InspectionIndex):ActiveState().revision,screen});
    if(Input.cancelled){Gesture.cancel();}
}
void AProbeArenaHUD::DrawHUD()
{
    Super::DrawHUD();if(!Canvas || !PlayerOwner){return;}
    if(SequenceActive && SequenceGeometryIndex!=static_cast<int32>(ActiveSequence()->squad_index())){RefreshArenaActors();}
    if(TutorialActive && WaveGeometryIndex!=static_cast<int32>(Tutorial->wave_index())){RefreshArenaActors();}
    if(WavesActive && WaveGeometryIndex!=static_cast<int32>(Waves->wave_index())){RefreshArenaActors();}
    HideTargetMarker();ReadPointer();
    // Optional visual consumers never gate command dispatch or mutate the battle.
    for(int32 i=0;i<64;++i){const auto cue=TutorialActive?Tutorial->next_visual_cue():SharedActive?ActiveShared()->next_visual_cue():SequenceActive?ActiveSequence()->next_visual_cue():WavesActive?Waves->next_visual_cue():PveActive?ActivePve()->next_visual_cue():Session.next_visual_cue();if(!cue){break;}
        UE_LOG(LogTemp,VeryVerbose,TEXT("VIS_CUE revision=%llu phase=%d index=%llu"),cue->revision,static_cast<int32>(cue->phase),static_cast<uint64>(cue->index));
    }
    if(IntroActive){DrawIntro();return;}
    if(SearchActive){DrawClassicSearch();return;}
    // Hub dialogs must be rendered before any underlying hub/team actions.
    if(Flow.in_hub() && !PopupLines.IsEmpty()){
        DrawRect(HubPanel,0,0,Canvas->SizeX,Canvas->SizeY);
        if(DrawInfoPanel(PopupTitle,PopupLines,PopupPage)==1){PopupLines.Empty();Pointer.cancel_until_release();}
        return;
    }
    if(!InspectionMode && ActiveFinished()){Flow.finish();}
    if(!InspectionMode && PlayerOwner->WasInputKeyJustPressed(EKeys::Escape)){HandleBack();return;}
    if(HubEnabled && Flow.in_hub()){DrawHub();return;}
    RestoreHub3DCamera();
    if(!InspectionMode && (Flow.screen()==rpg::presentation::UiScreen::Pause || Flow.screen()==rpg::presentation::UiScreen::AbandonConfirm)){DrawBattleMenu();return;}
    if(TutorialActive && Tutorial->awaiting_lesson()){DrawTutorialLesson();return;}
    auto view=ActiveState();
    if(InspectionMode){view={static_cast<std::uint64_t>(InspectionIndex),{SelectedCombatProfile(),{},InspectionFrames[InspectionIndex].battle,{}},{},{},false,true};}
    const double now=GetWorld()->GetTimeSeconds();
    const auto layout=SelectedLayout();
    const bool classic=layout==rpg::presentation::ArenaLayout::ClassicLateral;
    const float mx=static_cast<float>(Input.x),my=static_cast<float>(Input.y);
    const bool pressed=Input.pressed;
    const bool released=Input.released;
    if(auto held=Gesture.hold(now,view.revision)){OpenInfo(TEXT("Ability details"),rpg::presentation::ability_details(view,*held));}
    const bool abilityRelease=Gesture.active() && released;
    const auto shortSlot=released?Gesture.release(now,view.revision):std::optional<std::uint32_t>{};
    const bool clicked=Input.tap && !abilityRelease;
    const auto inside=[&](float x,float y,float w,float h){return mx>=x && mx<x+w && my>=y && my<y+h;};
    if(Flow.accepts_actions(!PopupLines.IsEmpty()) && !Gesture.active() && !Input.down && !Input.tap && !abilityRelease && view.automatic && !view.stopped && now>=NextAutomatic){
        const auto result=StepActiveBattle();
        ShowActionFeedback(result);
        NextAutomatic=now+0.5;SelectedSlot=0;SelectedTarget.Empty();view=ActiveState();
    }
    const bool classicRules=view.combat.profile==rpg::CombatProfile::Classic;
    const bool running=InspectionMode?view.combat.battle.phase==rpg::Phase::Running:!ActiveFinished();
    const bool manual=running && !view.automatic && !view.stopped && Flow.accepts_actions(!PopupLines.IsEmpty());
    if(SelectedSlot && SelectionRevision!=view.revision){SelectedSlot=0;SelectedTarget.Empty();}
    const auto candidates=ActiveTargets(view,static_cast<std::uint32_t>(SelectedSlot));
    const auto validTarget=[&](const std::string& id){return std::ranges::find(candidates,id)!=candidates.end();};
    const auto submit=[&](const FString& id){
        const auto result=classicRules?SubmitActiveAction(SelectionRevision,{view.combat.battle.current_actor.value_or(""),rpg::ActionType::Attack,TCHAR_TO_UTF8(*id),{}}):SubmitActiveSlot(SelectionRevision,static_cast<uint32>(SelectedSlot),TCHAR_TO_UTF8(*id));
        ShowActionFeedback(result);
        if(LastActionFeedback.applied){NextAutomatic=now+0.5;}
        UE_LOG(LogTemp, Display, TEXT("VIS_ACTION accepted=%d revision=%llu"),result.accepted()?1:0,static_cast<unsigned long long>(ActiveState().revision));
        if(!LastActionFeedback.retry_target || classicRules){SelectedSlot=0;}SelectedTarget.Empty();
    };
    const auto button=[&](float x,float y,float w,float h,const FString& text,bool enabled){
        DrawRect(enabled?FLinearColor(0.08f,0.20f,0.32f):FLinearColor(0.12f,0.12f,0.12f),x,y,w,h);DrawText(text,FColor::White,x+8,y+8);
        return clicked && enabled && rpg::presentation::pointer_clicked(Input,{x,y,w,h});
    };
    const FString actor=view.combat.battle.current_actor?Label(*view.combat.battle.current_actor):TEXT("-");
    const auto pveHud=PveActive?std::optional<rpg::presentation::PveHud>{rpg::presentation::pve_hud(ActivePve()->observation())}:std::nullopt;
    const auto sharedHud=SharedActive?std::optional<rpg::presentation::SharedHud>{ActiveShared()->hud()}:std::nullopt;
    const FString sceneTitle=SharedActive?FString(TEXT("3 FRENTES LOCAIS / HP unico")):SequenceActive?FString(TEXT("EQUIPES SEQUENCIAIS / 3 times")):WavesActive?FString(TEXT("ONDAS LOCAIS / Felipe")):PveActive?Label(pveHud->mode_label)+TEXT(" / Felipe e ecos"):classic?FString(TEXT("RPG URBANO / Classic lateral arena")):FString(TEXT("RPG URBANO / Standard depth arena"));
    DrawText(sceneTitle,FColor::White,20,15);
    DrawText(FString::Printf(TEXT("Actor %s | Round %llu | Opportunity %llu | Actions %llu"),*actor,
        static_cast<unsigned long long>(view.combat.battle.round),static_cast<unsigned long long>(view.combat.battle.turn_id),static_cast<unsigned long long>(view.combat.battle.accepted_actions)),FColor::White,20,38);
    DrawText(classicRules?TEXT("Rules: Classic / Attack + Defense"):TEXT("Rules: Team / Ability slots"),FColor::Cyan,Canvas->SizeX-320.f,15);
    if(PveActive){
        const auto hud=rpg::presentation::pve_hud(ActivePve()->observation());
        DrawText(Label(hud.objective_label)+TEXT(" — ")+Label(hud.boss_id),FColor::Cyan,20,62,nullptr,0.85f);
        DrawText(FString::Printf(TEXT("Chefe PV %lld/%lld | Acoes %llu/%llu"),static_cast<long long>(hud.boss_health),static_cast<long long>(hud.boss_max_health),static_cast<unsigned long long>(hud.actions.completed),static_cast<unsigned long long>(hud.actions.goal.value_or(0))),FColor::White,20,82,nullptr,0.75f);
        DrawText(Label(hud.status_label),hud.encounter_finished?FColor::Yellow:FColor::White,20,102,nullptr,0.8f);
    }
        if(PveActive && SelectedPveLab==rpg::presentation::PveLabKind::Phased){
        const auto observation=ActivePve()->observation();
        DrawText(TEXT("Fase: ")+Label(observation.phase_id.value_or("Inicial")),FColor::Orange,20,122,nullptr,0.8f);
    }
    if(PveActive && pveHud->score){
        FString text;
        if(pveHud->boss_turns.goal){text=FString::Printf(TEXT("Chefe %llu/%llu turnos naturais | Score %lld"),static_cast<unsigned long long>(pveHud->boss_turns.completed),static_cast<unsigned long long>(*pveHud->boss_turns.goal),static_cast<long long>(*pveHud->score));}
        else{text=FString::Printf(TEXT("Score %lld — %s"),static_cast<long long>(*pveHud->score),pveHud->encounter_finished?TEXT("consolidado"):TEXT("parcial"));}
        DrawText(text,FColor::Orange,20,122,nullptr,0.7f);
    }
    if(WavesActive || TutorialActive){
        const auto hud=TutorialActive?Tutorial->hud():Waves->hud();
        DrawText(FString::Printf(TEXT("Onda %llu/%llu — %s"),static_cast<unsigned long long>(hud.wave_number),static_cast<unsigned long long>(hud.wave_count),*Label(hud.wave_id)),FColor::Cyan,20,62,nullptr,0.85f);
        DrawText(FString::Printf(TEXT("Acoes globais %llu/%llu | Nesta onda %llu"),static_cast<unsigned long long>(hud.global_actions),static_cast<unsigned long long>(hud.global_goal.value_or(0)),static_cast<unsigned long long>(hud.local_actions)),FColor::White,20,82,nullptr,0.75f);
        DrawText(Label(hud.status),hud.finished?FColor::Yellow:FColor::White,20,102,nullptr,0.8f);
        if(hud.previous_wave){DrawText(TEXT("Transicao de ")+Label(*hud.previous_wave)+TEXT(" — detalhes abaixo"),FColor::Orange,20,122,nullptr,0.7f);}
    }
        if(SequenceActive){
        const auto hud=ActiveSequence()->hud();
        DrawText(FString::Printf(TEXT("Equipe %llu/%llu — %s"),static_cast<unsigned long long>(hud.squad_number),static_cast<unsigned long long>(hud.squad_count),*Label(hud.squad_id)),FColor::Cyan,20,62,nullptr,0.85f);
        DrawText(FString::Printf(TEXT("Chefe PV %lld/%lld | Janela %llu/%llu acoes"),static_cast<long long>(hud.boss_health),static_cast<long long>(hud.boss_max_health),static_cast<unsigned long long>(hud.local_actions),static_cast<unsigned long long>(hud.local_goal.value_or(0))),FColor::White,20,82,nullptr,0.7f);
        DrawText(FString::Printf(TEXT("Globais %llu/%llu | Fase %s"),static_cast<unsigned long long>(hud.global_actions),static_cast<unsigned long long>(hud.global_goal.value_or(0)),*Label(hud.boss_phase)),FColor::White,20,102,nullptr,0.7f);
        DrawText(Label(hud.status),hud.finished?FColor::Yellow:FColor::Orange,20,122,nullptr,0.75f);
    }
        if(SharedActive){
        const auto& hud=*sharedHud;
        DrawText(FString::Printf(TEXT("Chefe PV %lld/%lld"),static_cast<long long>(hud.boss_health),static_cast<long long>(hud.boss_max_health)),FColor::Cyan,20,62,nullptr,0.85f);
        DrawText(FString::Printf(TEXT("Acoes globais %llu/%llu | Chefe %llu"),static_cast<unsigned long long>(hud.global_actions),static_cast<unsigned long long>(hud.global_goal.value_or(0)),static_cast<unsigned long long>(hud.boss_actions)),FColor::White,20,82,nullptr,0.7f);
        DrawText(TEXT("Frente atuando: ")+Label(hud.actor_front.value_or("Chefe")),FColor::White,20,102,nullptr,0.7f);
        DrawText(TEXT("Proximo ataque local: ")+Label(hud.next_local_front.value_or("nenhuma")),FColor::Orange,20,122,nullptr,0.7f);
        DrawText(Label(hud.status),hud.finished?FColor::Yellow:FColor::White,20,142,nullptr,0.75f);
    }
    TArray<FBox2D> occupied;
    std::vector<rpg::presentation::TargetHitRegion> targetRegions;
    const FVector cameraPosition=PlayerOwner->PlayerCameraManager?PlayerOwner->PlayerCameraManager->GetCameraLocation():FVector::ZeroVector;
    for(TActorIterator<AStaticMeshActor> it(GetWorld());it;++it){
        if(!IsValid(*it) || !it->Tags.Contains(FName(TEXT("RpgPrototypeActor")))){continue;}
        for(const auto& fighter:view.combat.battle.fighters){
            const FString id=Label(fighter.definition.id);
            if(!it->Tags.Contains(FName(*id))){continue;}
            it->SetActorHiddenInGame(fighter.health<=0);
            it->GetStaticMeshComponent()->SetCollisionEnabled(fighter.health>0?ECollisionEnabled::QueryOnly:ECollisionEnabled::NoCollision);
            FVector2D screen;
            if(!PlayerOwner->ProjectWorldLocationToScreen(it->GetActorLocation()+FVector(0,0,80),screen)){continue;}
            const auto labelPosition=rpg::presentation::viewport_to_canvas(screen.X,screen.Y,CanvasCoordinates);
            if(!labelPosition){continue;}screen=FVector2D(labelPosition->x,labelPosition->y);
            const float width=150.f;
            const float x=FMath::Clamp(static_cast<float>(screen.X)-width/2,4.f,FMath::Max(4.f,Canvas->SizeX-width-4));
            const float labelMin=(SharedActive?174.f:(PveActive || WavesActive || SequenceActive || TutorialActive)?154.f:66.f)+(fighter.statuses.empty()?0.f:24.f);
            const float labelMax=FMath::Max(labelMin,Canvas->SizeY-175);
            const float idealY=FMath::Clamp(static_cast<float>(screen.Y)-25,labelMin,labelMax);
            float y=idealY;
            for(int32 attempt=0;attempt<11;++attempt){
                const float offset=attempt==0?0.f:static_cast<float>((attempt+1)/2)*48.f*(attempt%2?-1.f:1.f);
                y=FMath::Clamp(idealY+offset,labelMin,labelMax);
                const FBox2D box(FVector2D(x,y),FVector2D(x+width,y+44));bool overlap=false;
                for(const auto& other:occupied){if(box.Intersect(other)){overlap=true;break;}}
                if(!overlap){break;}
            }            occupied.Add(FBox2D(FVector2D(x,y),FVector2D(x+width,y+44)));
            DrawLine(static_cast<float>(screen.X),static_cast<float>(screen.Y),x+width/2,y+40,FLinearColor(0.6f,0.7f,0.8f),1);
            const bool active=view.combat.battle.current_actor==fighter.definition.id;
            FString frontTag;
            if(SharedActive && !fighter.definition.front_id.empty()){
                const auto front=std::ranges::find(sharedHud->fronts,fighter.definition.front_id,&rpg::presentation::SharedFrontHud::id);
                if(front!=sharedHud->fronts.end()){frontTag=FString::Printf(TEXT(" [F%d]"),static_cast<int32>(front-sharedHud->fronts.begin())+1);}
            }
            const FString name=(active?TEXT("> "):TEXT(""))+id+frontTag;
            float nameWidth=0,nameHeight=0;GetTextSize(name,nameWidth,nameHeight);
            DrawText(name,active?FColor::Yellow:FColor::White,x,y,nullptr,FMath::Min(1.f,(width-30)/FMath::Max(1.f,nameWidth)));
            DrawText(FString::Printf(TEXT("%lld / %lld"),static_cast<long long>(fighter.health),static_cast<long long>(fighter.definition.max_health)),FColor::White,x,y+14);
            const auto shield=rpg::presentation::shield_display(fighter);
            DrawRect(FLinearColor(0.04f,0.04f,0.05f),x,y+31,width,7);
            DrawRect(fighter.definition.team==rpg::Team::A?FLinearColor(0.1f,0.85f,0.7f):FLinearColor(0.95f,0.25f,0.4f),x,y+31,width*static_cast<float>(shield.health_fraction),7);
            DrawRect(FLinearColor(0.65f,0.9f,1.f),x,y+31,width*static_cast<float>(shield.overlay_fraction),7);
            int32 effectIndex=0;for(const auto& status:fighter.statuses){
                FString effectName;switch(status.kind){case rpg::EffectKind::Bleed:effectName=TEXT("bleed");break;case rpg::EffectKind::Poison:effectName=TEXT("poison");break;case rpg::EffectKind::StandardShield:effectName=TEXT("shield_standard");break;case rpg::EffectKind::SilverShield:effectName=TEXT("shield_silver");break;case rpg::EffectKind::Silence:effectName=TEXT("silence");break;case rpg::EffectKind::Suppression:effectName=TEXT("suppression");break;case rpg::EffectKind::Freeze:effectName=TEXT("freeze");break;case rpg::EffectKind::Petrify:effectName=TEXT("petrification");break;case rpg::EffectKind::Invincible:case rpg::EffectKind::DebuffImmunity:effectName=TEXT("immunity_invincible");break;default:break;}
                if(!effectName.IsEmpty() && effectIndex<5){if(auto* texture=UiTexture(TEXT("ico_status_")+effectName)){const float ex=x+effectIndex*22;DrawTexture(texture,ex,y-24,22,22,0,0,1,1,FLinearColor::White,BLEND_Translucent);if(status.remaining>0){DrawText(FString::FromInt(status.remaining),FColor::White,ex+13,y-16,nullptr,0.6f);}++effectIndex;}}
            }
            if(effectIndex>0 && clicked && PopupLines.IsEmpty() && rpg::presentation::pointer_clicked(Input,{x,y-24,110,24})){OpenInfo(TEXT("Efeitos / ")+id,rpg::presentation::effect_details(fighter));return;}
            DrawText(TEXT("[i]"),FColor::Cyan,x+width-25,y+8);
            if(clicked && PopupLines.IsEmpty() && rpg::presentation::pointer_clicked(Input,{x+width-28,y,28,28})){OpenInfo(TEXT("Effects / ")+id,rpg::presentation::effect_details(fighter));return;}
            if(manual && validTarget(fighter.definition.id)){
                const double depth=FVector::DistSquared(cameraPosition,it->GetActorLocation());
                targetRegions.push_back({fighter.definition.id,{x,y,width-28,40},depth,true});
                FVector center,extent;it->GetActorBounds(false,center,extent);
                double left=1e20,top=1e20,right=-1e20,bottomEdge=-1e20;int projected=0;
                for(int corner=0;corner<8;++corner){
                    FVector2D viewportPoint;
                    const FVector point=center+FVector((corner&1)?extent.X:-extent.X,(corner&2)?extent.Y:-extent.Y,(corner&4)?extent.Z:-extent.Z);
                    if(!PlayerOwner->ProjectWorldLocationToScreen(point,viewportPoint)){continue;}
                    if(const auto mapped=rpg::presentation::viewport_to_canvas(viewportPoint.X,viewportPoint.Y,CanvasCoordinates)){
                        left=std::min(left,mapped->x);right=std::max(right,mapped->x);top=std::min(top,mapped->y);bottomEdge=std::max(bottomEdge,mapped->y);++projected;
                    }
                }
                if(projected==8){const double w=std::max(48.0,right-left),h=std::max(48.0,bottomEdge-top);targetRegions.push_back({fighter.definition.id,{(left+right-w)/2,(top+bottomEdge-h)/2,w,h},depth,false});}
            }
            if(fighter.health>0 && (SelectedTarget==id || (classic && validTarget(fighter.definition.id) && inside(x,y,width-28,40)))){
                FString style;FParse::Value(FCommandLine::Get(),TEXT("TargetMarker="),style);
                const int32 segments=style==TEXT("Diamond")?4:28;
                // World-depth lines sit at the model's feet instead of painting over its silhouette.
                FVector center,extent;it->GetActorBounds(false,center,extent);
                const float radius=rpg::presentation::marker_radius(static_cast<float>(extent.X),static_cast<float>(extent.Y));
                const FVector base(center.X,center.Y,center.Z-extent.Z+1.f);
                UpdateTargetMarker(base,radius,segments);
            }
        }
    }
    if(InspectionMode){DrawInspectionControls();return;}
    const float bottom=FMath::Max(280.f,Canvas->SizeY-135.f);
    DrawRect(FLinearColor(0.025f,0.035f,0.06f,0.9f),0,bottom-12,Canvas->SizeX,135);
    if(!classic){
        const float portraitX=Canvas->SizeX-795.f;DrawRect(FLinearColor(0.06f,0.12f,0.2f),portraitX,bottom-20,120,95);
        DrawText(TEXT("ACTIVE UNIT"),FColor::Cyan,portraitX+8,bottom-12);DrawText(actor,FColor::White,portraitX+15,bottom+8,nullptr,2.f);if((actor.Contains(TEXT("FELIPE")) || actor.Contains(TEXT("JOVEM"))) && UiTexture(TEXT("avatar-felipe-v01"))){DrawTexture(UiTexture(TEXT("avatar-felipe-v01")),portraitX,bottom-20,95,95,0,0,1,1,FLinearColor::White,BLEND_Translucent);}else{DrawText(TEXT("Retrato provisório"),FColor::White,portraitX+8,bottom+55);}
    }
    if(classicRules){
        const auto actorIt=std::ranges::find(view.combat.battle.fighters,view.combat.battle.current_actor.value_or(""),[](const rpg::Fighter& f){return f.definition.id;});
        const bool blocked=actorIt!=view.combat.battle.fighters.end() && rpg::incapacitated(*actorIt);
        const auto perform=[&](rpg::ActionType type){
            const auto result=SubmitActiveAction(view.revision,{view.combat.battle.current_actor.value_or(""),type,{},{}});
            ShowActionFeedback(result);if(LastActionFeedback.applied){NextAutomatic=now+0.5;}SelectedSlot=0;SelectedTarget.Empty();
            UE_LOG(LogTemp,Display,TEXT("VIS_CLASSIC_ACTION accepted=%d"),result.accepted()?1:0);
        };
        for(uint32 position=1;position<=3;++position){
            const float x=Canvas->SizeX-660.f+(position-1)*215.f;
            const FString text=position==1?TEXT("Attack"):position==2?TEXT("Defense"):TEXT("Skills / info");
            const bool ready=manual && !blocked;
            button(x,bottom,205,50,text,position==3?true:ready);
            if(pressed && PopupLines.IsEmpty() && inside(x,bottom,205,50)){Gesture.press(position,now,view.revision);}
            if(shortSlot==position && inside(x,bottom,205,50) && PopupLines.IsEmpty()){
                if(position==3){OpenInfo(TEXT("Classic skills"),rpg::presentation::ability_details(view,3));return;}
                if(!ready){Message=TEXT("Unit unable to act; use Pass.");return;}
                if(position==2){perform(rpg::ActionType::Guard);return;}
                SelectedSlot=1;SelectionRevision=view.revision;SelectedTarget.Empty();
                const auto ids=ActiveTargets(view,1);
                if(!classic && !ids.empty()){SelectedTarget=Label(ids[static_cast<std::size_t>(FMath::RandHelper(static_cast<int32>(ids.size())))]);}
                Message=classic?TEXT("Choose a valid enemy."):TEXT("Target suggested. Change it or confirm.");return;
            }
        }
        if(button(130,bottom,100,50,TEXT("Pass"),manual && blocked)){perform(rpg::ActionType::Skip);return;}
    }
    for(const auto& ability:view.abilities){
        const auto card=rpg::presentation::ability_card(view,ability.slot.position);
        const bool ready=manual && card.selectable && (!TutorialActive || Tutorial->allowed_slot(ability.slot.position));
        const float cardStart=Canvas->SizeX>=840?Canvas->SizeX-660.f:Canvas->SizeX>=640?140.f:20.f;
        const float cardWidth=FMath::Max(48.f,(Canvas->SizeX-cardStart-20.f)/3.f-8.f);
        const float x=cardStart+(ability.slot.position-1)*(cardWidth+8.f);
        const FString text=FString::Printf(TEXT("%u / %s"),ability.slot.position,*Label(card.kind));
        button(x,bottom,cardWidth,50,TEXT(""),ready);
        float textW=0,textH=0;GetTextSize(text,textW,textH);DrawText(text,FColor::White,x+54,bottom+6,nullptr,FMath::Min(0.8f,FMath::Max(0.25f,(cardWidth-58)/FMath::Max(1.f,textW))));
        const bool young=actor.Contains(TEXT("JOVEM"));
        const bool felipe=young || actor.Contains(TEXT("FELIPE"));
        FString icon=young?FString::Printf(TEXT("ico_skill_jovem_lobo_a%u"),ability.slot.position):FString::Printf(TEXT("ico_skill_felipe_a%u"),ability.slot.position);
        if(ability.slot.position==2){const auto unit=std::ranges::find(view.combat.battle.fighters,view.combat.battle.current_actor.value_or(""),[](const rpg::Fighter& f){return f.definition.id;});if((VigilFlashActor==actor && now<VigilFlashUntil) || (unit!=view.combat.battle.fighters.end()&&unit->definition.vigil&&unit->vigil_charges>=unit->definition.vigil->max_party_size)){icon+=TEXT("_active");}}
        if(felipe){if(auto* texture=UiTexture(icon)){DrawTexture(texture,x+6,bottom+3,44,44,0,0,1,1,ready?FLinearColor::White:FLinearColor(0.5f,0.5f,0.6f,1.f),BLEND_Translucent);}}
        const FColor color=card.badge==rpg::presentation::AbilityBadge::Passive?FColor(180,130,240):card.badge==rpg::presentation::AbilityBadge::Ready?FColor::Cyan:card.badge==rpg::presentation::AbilityBadge::Unavailable?FColor(180,180,180):FColor(245,158,11);
        const bool selected=ready && SelectedSlot==static_cast<int32>(ability.slot.position);
        DrawText(selected?TEXT("Selecionada"):Label(card.status),color,x+54,bottom+30,nullptr,Canvas->SizeX<640?0.5f:0.75f);
        const float cx=x+cardWidth-16,cy=bottom+15;
        if(card.cooldown>0){
            const FString timer=FString::FromInt(card.cooldown);float width=0,height=0;GetTextSize(timer,width,height);
            const float scale=FMath::Min(1.f,32.f/FMath::Max(1.f,width));
            DrawText(timer,FColor(245,158,11),x+cardWidth-40,bottom+30,nullptr,scale);
        }
        if(card.badge==rpg::presentation::AbilityBadge::Locked){
            DrawLine(cx-6,cy-1,cx+6,cy-1,FLinearColor(color),1.5f);DrawLine(cx-6,cy-1,cx-6,cy+7,FLinearColor(color),1.5f);
            DrawLine(cx+6,cy-1,cx+6,cy+7,FLinearColor(color),1.5f);DrawLine(cx-6,cy+7,cx+6,cy+7,FLinearColor(color),1.5f);
            DrawLine(cx-4,cy-1,cx-4,cy-7,FLinearColor(color),1.5f);DrawLine(cx-4,cy-7,cx+4,cy-7,FLinearColor(color),1.5f);DrawLine(cx+4,cy-7,cx+4,cy-1,FLinearColor(color),1.5f);
        }else if(card.badge==rpg::presentation::AbilityBadge::Unavailable){
            for(int i=0;i<20;++i){const float a=2*PI*i/20,b=2*PI*(i+1)/20;DrawLine(cx+7*FMath::Cos(a),cy+7*FMath::Sin(a),cx+7*FMath::Cos(b),cy+7*FMath::Sin(b),FLinearColor(color),1.5f);}
            DrawLine(cx-5,cy+5,cx+5,cy-5,FLinearColor(color),1.5f);
        }else if(card.badge==rpg::presentation::AbilityBadge::Passive){DrawText(TEXT("i"),color,cx-3,cy-9);}
        if(selected){DrawLine(x+2,bottom+2,x+cardWidth-2,bottom+2,FLinearColor::Yellow,2);}
        if(pressed && PopupLines.IsEmpty() && inside(x,bottom,cardWidth,50)){Gesture.press(ability.slot.position,now,view.revision);}
        if(shortSlot==ability.slot.position && inside(x,bottom,cardWidth,50)){
            if(!ready){SelectedSlot=0;SelectedTarget.Empty();Message=TutorialActive && !Tutorial->allowed_slot(ability.slot.position)?TEXT("Neste primeiro passo, use A1 — Garra do Asfalto."):Label(card.reason);return;}
            SelectedSlot=static_cast<int32>(ability.slot.position);SelectionRevision=view.revision;
            const auto ids=ActiveTargets(view,ability.slot.position);SelectedTarget.Empty();
            if(!classic && !ids.empty()){SelectedTarget=Label(ids[static_cast<std::size_t>(FMath::RandHelper(static_cast<int32>(ids.size())))]);}
            Message=classic?TEXT("Choose a valid target."):TEXT("Target suggested. Change it or confirm.");return;
        }
    }
    if(HubEnabled && PopupLines.IsEmpty() && button(20.f,bottom-42,130,30,running?TEXT("Pausar / Voltar"):TEXT("Voltar ao hub"),true)){if(running){HandleBack();}else{Flow.finish();ReturnToHub();}return;}
    const rpg::presentation::Rect messageArea{20,bottom+52,std::max(48.0,static_cast<double>(Canvas->SizeX)-245),48};
    const float messageScale=0.75f;float ellipsisWidth=0,ellipsisHeight=0;GetTextSize(TEXT("..."),ellipsisWidth,ellipsisHeight,HubBodyFont);
    const std::string messageText=TCHAR_TO_UTF8(*Message);
    const auto messageLines=rpg::presentation::wrap_text(messageText,std::max(1.0,messageArea.width-26-ellipsisWidth*messageScale),[&](std::string_view text){float width=0,height=0;GetTextSize(Label(std::string(text)),width,height,HubBodyFont);return static_cast<double>(width*messageScale);});
    const FString shortMessage=messageLines.empty()?TEXT(""):Label(messageLines.front())+(messageLines.size()>1?TEXT("..."):TEXT(""));
    FColor messageColor=FColor::White;
    if(HasActionFeedback && messageText==LastActionFeedback.summary){messageColor=LastActionFeedback.tone==rpg::presentation::FeedbackTone::Success?FColor::Cyan:LastActionFeedback.tone==rpg::presentation::FeedbackTone::Error?FColor::Red:FColor::Yellow;}
    DrawText(shortMessage,messageColor,20,bottom+60,HubBodyFont,messageScale);
    DrawText(TEXT("[i]"),FColor::Cyan,static_cast<float>(messageArea.x+messageArea.width-22),bottom+60,HubBodyFont,messageScale);
    if(rpg::presentation::contains(messageArea,Input.x,Input.y)){DrawLine(20,bottom+83,static_cast<float>(messageArea.x+messageArea.width),bottom+83,FLinearColor(0.2f,0.8f,0.9f),1);}
    if(PopupLines.IsEmpty() && rpg::presentation::pointer_clicked(Input,messageArea)){
        std::vector<std::string> lines={"Mensagem atual: "+messageText};
        if(HasActionFeedback){lines.push_back("Ultima tentativa de acao:");lines.insert(lines.end(),LastActionFeedback.details.begin(),LastActionFeedback.details.end());}
        else{lines.push_back("Nenhuma tentativa de acao registrada nesta sessao de tela.");}
        OpenInfo(TEXT("Mensagem / ultima acao"),lines);return;
    }
    if((PveActive || WavesActive || SequenceActive || SharedActive || TutorialActive) && PopupLines.IsEmpty() && button(160,bottom-42,FMath::Max(48.f,FMath::Min(210.f,Canvas->SizeX-180.f)),30,running?TEXT("Detalhes do encontro"):TEXT("Resultado do encontro"),true)){
        std::vector<std::string> lines;
        if(SharedActive){const auto hud=ActiveShared()->hud();lines={hud.status,"Chefe unico: "+hud.boss_id+" | PV "+std::to_string(hud.boss_health)+"/"+std::to_string(hud.boss_max_health),"Acoes globais: "+std::to_string(hud.global_actions),"Acoes do chefe: "+std::to_string(hud.boss_actions),"Proximo ataque local: "+hud.next_local_front.value_or("nenhuma"),"Ultimo ataque local: "+hud.last_local_front.value_or("nenhum")};
            for(std::size_t i=0;i<hud.fronts.size();++i){const auto& front=hud.fronts[i];lines.push_back("F"+std::to_string(i+1)+" = "+front.id+(front.active?" | ativa":" | sem sobreviventes")+" | acoes aliadas "+std::to_string(front.allied_actions));}
            lines.push_back(SharedGlobalSpecial?"Especial global declarado; nao avanca a rotacao dos ataques locais.":"Ataques comuns alternam entre frentes vivas.");
            lines.push_back("Sem limite local novo por frente; HP/AP/recargas do chefe sao globais. Simulacao local, sem servidor.");
        }else if(SequenceActive){const auto hud=ActiveSequence()->hud();lines={hud.status,"Equipe "+std::to_string(hud.squad_number)+"/"+std::to_string(hud.squad_count)+": "+hud.squad_id,"Chefe: "+hud.boss_id+" | PV "+std::to_string(hud.boss_health)+"/"+std::to_string(hud.boss_max_health),"Fase: "+hud.boss_phase,"Acoes globais: "+std::to_string(hud.global_actions),"Acoes da janela local (ambos os lados): "+std::to_string(hud.local_actions),"HP do chefe continuo; politica de estados/recargas V2 do coordenador."};
            if(hud.next_squad){lines.push_back("Proxima equipe: "+*hud.next_squad);}if(hud.previous_squad){lines.push_back("Transicao anterior: "+*hud.previous_squad+" -> "+hud.squad_id);}if(hud.previous_phase){lines.push_back("Fase anterior: "+*hud.previous_phase);}
        }else if(WavesActive || TutorialActive){const auto hud=TutorialActive?Tutorial->hud():Waves->hud();lines={hud.status,"Onda "+std::to_string(hud.wave_number)+"/"+std::to_string(hud.wave_count)+": "+hud.wave_id,"Acoes globais: "+std::to_string(hud.global_actions),"Acoes nesta onda: "+std::to_string(hud.local_actions)};
            if(hud.previous_wave){lines.push_back("Transicao anterior: "+*hud.previous_wave+" -> "+hud.wave_id);}
            for(const auto& event:(TutorialActive?Tutorial->transition_events():Waves->transition_events())){if(event.status_kind){lines.push_back(event.target+" | "+rpg::presentation::effect_name(*event.status_kind)+" | "+std::to_string(event.amount));}}
        }else{const auto hud=rpg::presentation::pve_hud(ActivePve()->observation());lines={hud.status_label,"Objetivo: "+hud.objective_label,"Chefe: "+hud.boss_id,"Acoes: "+std::to_string(hud.actions.completed)};
            for(const auto& phase:ActivePve()->phase_events()){lines.push_back("Transicao aplicada: "+phase.id);}
            if(hud.boss_turns.goal){lines.push_back("Turnos naturais do chefe: "+std::to_string(hud.boss_turns.completed)+"/"+std::to_string(*hud.boss_turns.goal));}
            if(hud.score){lines.push_back("Score observado: "+std::to_string(*hud.score));const auto receipt=ActivePve()->receipt();
                if(receipt){lines.push_back("Recibo: "+receipt->attempt_id);lines.push_back("Score consolidado: "+std::to_string(receipt->score));lines.push_back(receipt->survived?"Equipe sobreviveu: sim":"Equipe sobreviveu: nao");lines.push_back("Faixa: "+receipt->reward_tier.value_or("nao configurada"));lines.push_back("Consulta de recibo; nenhum ganho e concedido novamente.");}
                else{lines.push_back("Tentativa ainda sem recibo consolidado.");}
            }
        }
        lines.push_back("Teste local; sem recompensa ou alteracao nos saves dos outros laboratorios.");OpenInfo(TEXT("Encontro local"),lines);return;
    }    if(!running && !view.progress.completions.empty()){
        const auto& result=view.progress.completions.back();
        if(PopupLines.IsEmpty() && button(20,88,190,32,TEXT("Detalhes do resultado"),true)){
            OpenInfo(TEXT("Resultado do laboratório"),{
                "Tentativa: "+std::to_string(result.attempt),
                "Acoes aceitas: "+std::to_string(result.actions),
                "Recompensa desta tentativa: "+std::to_string(result.credits),
                "Saldo local: "+std::to_string(view.progress.credits),
                result.reason==rpg::EndReason::Elimination?"Encerramento: eliminacao":result.reason==rpg::EndReason::ActionLimit?"Encerramento: limite de acoes":"Encerramento: limite tecnico",
                "Valores provisorios do laboratorio; consultar nao concede recompensa."});return;
        }
    }
    if(!running && !PveActive && !WavesActive && !SequenceActive && !SharedActive){DrawText(view.combat.battle.winner==rpg::Team::A?TEXT("Team A wins"):view.combat.battle.winner==rpg::Team::B?TEXT("Team B wins"):TEXT("Draw"),FColor::Yellow,20,62);}
    if((!running || view.stopped) && PopupLines.IsEmpty() && button(20.f,bottom,100,50,TEXT("Nova tentativa"),true)){
        ResetActiveBattle();Flow.finish();Flow.restart_result();Gesture.cancel();SelectedSlot=0;SelectedTarget.Empty();NextAutomatic=0;Message=TEXT("New attempt.");UE_LOG(LogTemp,Display,TEXT("VIS_RESET"));return;
    }
    if(!classic && button(Canvas->SizeX-205.f,bottom+58,180,30,TEXT("Confirm action"),manual && !SelectedTarget.IsEmpty() && SelectedSlot!=0)){
        submit(SelectedTarget);return;
    }
    if(PopupLines.IsEmpty() && clicked && manual && SelectedSlot){
        if(const auto target=rpg::presentation::pick_target(Input,targetRegions)){
            if(classic){submit(Label(*target));}else{SelectedTarget=Label(*target);Message=TEXT("Alvo alterado. Confirme quando estiver pronto.");}
            UE_LOG(LogTemp,Display,TEXT("VIS_TARGET_SELECTED slot=%d revision=%llu"),SelectedSlot,static_cast<unsigned long long>(view.revision));
        }
    }
    if(!PopupLines.IsEmpty()){
        Gesture.cancel();
        if(DrawInfoPanel(PopupTitle,PopupLines,PopupPage)==1){PopupLines.Empty();}
    }
}
void AProbeArenaHUD::DrawInspectionControls()
{
    const auto& frame=InspectionFrames[InspectionIndex];
    if(!PopupLines.IsEmpty()){
        if(DrawInfoPanel(PopupTitle,PopupLines,PopupPage)==1){PopupLines.Empty();}return;
    }
    const float top=FMath::Max(80.f,Canvas->SizeY-145.f);
    DrawRect(FLinearColor(0.02f,0.03f,0.05f,0.95f),0,top,Canvas->SizeX,145);
    DrawText(FString::Printf(TEXT("INSPECAO SOMENTE LEITURA | %d / %d"),InspectionIndex+1,static_cast<int32>(InspectionFrames.size())),FColor::Yellow,16,top+8);
    DrawText(Label(frame.id),FColor::White,16,top+32);
    DrawText(TEXT("Sem comandos de combate, progresso ou recompensa."),FColor::White,16,top+55,nullptr,0.8f);
    const double width=(Canvas->SizeX-32.0)/3;
    const auto button=[&](int index,const TCHAR* text){
        const rpg::presentation::Rect rect{8+index*(width+8),Canvas->SizeY-56.0,width,48};
        DrawRect(FLinearColor(0.08f,0.20f,0.32f),static_cast<float>(rect.x),static_cast<float>(rect.y),static_cast<float>(rect.width),48);
        DrawText(text,FColor::White,static_cast<float>(rect.x+8),static_cast<float>(rect.y+10));
        return rpg::presentation::pointer_clicked(Input,rect);
    };
    const int32 count=static_cast<int32>(InspectionFrames.size());
    int32 next=InspectionIndex;
    if(button(0,TEXT("Anterior"))){next=(InspectionIndex+count-1)%count;}
    if(button(1,TEXT("Proximo"))){next=(InspectionIndex+1)%count;}
    if(button(2,TEXT("Detalhes"))){
        std::vector<std::string> lines=frame.notes;
        const auto focus=std::ranges::find(frame.battle.fighters,frame.focus,[](const rpg::Fighter& f){return f.definition.id;});
        if(focus!=frame.battle.fighters.end()){
            lines.push_back("Foco: "+frame.focus+" | PV "+std::to_string(focus->health)+"/"+std::to_string(focus->definition.max_health));
            const auto details=rpg::presentation::effect_details(*focus);lines.insert(lines.end(),details.begin(),details.end());
        }
        OpenInfo(Label(frame.id),lines);return;
    }
    if(next!=InspectionIndex){InspectionIndex=next;Gesture.cancel();SelectedSlot=0;SelectedTarget.Empty();RefreshArenaActors();}
}

void AProbeArenaHUD::HandleBack()
{
    BattleMenuPage=0;
    const auto effect=Flow.back(!PopupLines.IsEmpty(),SelectedSlot!=0 || !SelectedTarget.IsEmpty());
    if(effect==rpg::presentation::BackEffect::CloseDetails){PopupLines.Empty();}
    if(effect==rpg::presentation::BackEffect::CancelSelection){SelectedSlot=0;SelectedTarget.Empty();Message=TEXT("Seleção cancelada.");}
    if(Flow.screen()==rpg::presentation::UiScreen::Hub && (PveActive || WavesActive || SequenceActive || SharedActive || TutorialActive)){ReturnToHub();}
    Gesture.cancel();Pointer.cancel_until_release();NextAutomatic=GetWorld()->GetTimeSeconds()+0.5;
}
void AProbeArenaHUD::DrawBattleMenu()
{
    const bool confirming=Flow.screen()==rpg::presentation::UiScreen::AbandonConfirm;
    TArray<FString> paragraphs;
    paragraphs.Add(confirming?TEXT("Abandonar encerra esta tentativa sem vitória ou recompensa. O progresso anterior é preservado. Ao entrar em batalha novamente, será iniciada outra tentativa."):TEXT("Batalha pausada. Nenhum comando ou turno automático será executado enquanto este painel estiver aberto."));
    const int32 result=DrawInfoPanel(confirming?TEXT("CONFIRMAR ABANDONO"):TEXT("PAUSA"),paragraphs,BattleMenuPage,true,
        confirming?TEXT("Confirmar abandono"):TEXT("Abandonar tentativa"),confirming?TEXT("Cancelar"):TEXT("Retomar"));
    if(result==1){BattleMenuPage=0;if(confirming){Flow.back(false,false);}else{Flow.resume();}NextAutomatic=GetWorld()->GetTimeSeconds()+0.5;}
    if(result==2){
        if(confirming){if(Flow.confirm_abandon()){if(TutorialActive){Tutorial->abandon();}else if(SharedActive){ActiveShared()->abandon();}else if(SequenceActive){ActiveSequence()->abandon();}else if(WavesActive){Waves->abandon();}else if(PveActive){ActivePve()->abandon();}ReturnToHub();Message=TEXT("Tentativa abandonada sem recompensa.");}}
        else{BattleMenuPage=0;Flow.request_abandon();}
    }
}
