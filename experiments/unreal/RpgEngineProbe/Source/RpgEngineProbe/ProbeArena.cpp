#include "ProbeArena.h"
#include <algorithm>
#include "Camera/CameraActor.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Misc/ConfigCacheIni.h"
#include "rpg/arena_layout.hpp"
#include "Components/StaticMeshComponent.h"
#include "Engine/Canvas.h"
#include "CanvasTypes.h"

#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"
#include "UnrealClient.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "UObject/ConstructorHelpers.h"
namespace {
FString Label(const std::string& value){return UTF8_TO_TCHAR(value.c_str());}
rpg::CombatProfile SelectedCombatProfile(){FString name;if(FParse::Value(FCommandLine::Get(),TEXT("CombatProfile="),name) && name==TEXT("Classic")){return rpg::CombatProfile::Classic;}if(!name.IsEmpty() && name!=TEXT("Team")){UE_LOG(LogTemp,Warning,TEXT("Unknown CombatProfile; using Team"));}return rpg::CombatProfile::Team;}
std::optional<rpg::presentation::ArenaLayout> SelectedLayout(){FString name;if(!FParse::Value(FCommandLine::Get(),TEXT("ArenaLayout="),name)){return rpg::presentation::default_arena_layout;}return rpg::presentation::parse_arena_layout(TCHAR_TO_UTF8(*name));}
}
AProbeArenaHUD::AProbeArenaHUD():Session(SelectedCombatProfile()==rpg::CombatProfile::Classic?rpg::classic_reference_setup():rpg::team_reference_setup(),SelectedCombatProfile()){static ConstructorHelpers::FObjectFinder<UStaticMesh> marker(TEXT("/Engine/BasicShapes/Cube.Cube"));TargetMarkerMesh=marker.Object;}
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
        // This prototype uses Canvas controls, not Slate/UMG focusable widgets.
        // Forward the initial capture click instead of consuming it for focus.
        FInputModeGameOnly inputMode;inputMode.SetConsumeCaptureMouseDown(false);
        pc->SetInputMode(inputMode);
        if(auto* viewport=GetWorld()->GetGameViewport()){viewport->SetMouseCaptureMode(EMouseCaptureMode::NoCapture);}
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
    LoadHubCatalog();
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
    for(TActorIterator<AStaticMeshActor> it(GetWorld());it;++it){if(it->Tags.Contains(FName(TEXT("RpgPrototypeActor")))){it->Destroy();}}
    std::vector<rpg::Definition> roster;
    const auto snapshot=InspectionMode?InspectionFrames[InspectionIndex].battle:Session.state().combat.battle;
    for(const auto& fighter:snapshot.fighters){roster.push_back(fighter.definition);}
    auto* mesh=LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    for(const auto& placement:rpg::presentation::arena_placements(roster,*layout)){
        auto* sphere=GetWorld()->SpawnActor<AStaticMeshActor>(FVector(placement.x,placement.y,placement.z),FRotator(0,placement.facing_yaw,0));
        if(sphere && mesh){sphere->GetStaticMeshComponent()->SetStaticMesh(mesh);sphere->Tags.Add(FName(*Label(placement.fighter_id)));sphere->Tags.Add(FName(TEXT("RpgPrototypeActor")));}
    }
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
void AProbeArenaHUD::ReadPointer()
{
    float mx=0,my=0,tx=0,ty=0;bool touched=false;
    const bool mouseValid=PlayerOwner->GetMousePosition(mx,my);
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
    const bool focused=viewport && viewport->Viewport && (IntroActive?viewport->Viewport->IsForegroundWindow():viewport->Viewport->HasFocus());
    const std::uint8_t screen=InspectionMode?(!PopupLines.IsEmpty()?201:200):static_cast<std::uint8_t>(Flow.screen())+(!PopupLines.IsEmpty()?16:0);
    Input=Pointer.update({PlayerOwner->IsInputKeyDown(EKeys::LeftMouseButton),mouseValid && mouse.has_value() && CanvasCoordinatesKnown,mouse?mouse->x:0,mouse?mouse->y:0},
        {touched,touch.has_value() && CanvasCoordinatesKnown,touch?touch->x:0,touch?touch->y:0},focused,GetWorld()->GetTimeSeconds(),{InspectionMode?static_cast<std::uint64_t>(InspectionIndex):Session.state().revision,screen});
    if(Input.cancelled){Gesture.cancel();}
}
void AProbeArenaHUD::DrawHUD()
{
    Super::DrawHUD();if(!Canvas || !PlayerOwner){return;}
    HideTargetMarker();ReadPointer();
    // Optional visual consumers never gate command dispatch or mutate the battle.
    for(int32 i=0;i<64;++i){const auto cue=Session.next_visual_cue();if(!cue){break;}
        UE_LOG(LogTemp,VeryVerbose,TEXT("VIS_CUE revision=%llu phase=%d index=%llu"),cue->revision,static_cast<int32>(cue->phase),static_cast<uint64>(cue->index));
    }
    if(IntroActive){DrawIntro();return;}
    // Hub dialogs must be rendered before any underlying hub/team actions.
    if(Flow.in_hub() && !PopupLines.IsEmpty()){
        DrawRect(HubPanel,0,0,Canvas->SizeX,Canvas->SizeY);
        if(DrawInfoPanel(PopupTitle,PopupLines,PopupPage)==1){PopupLines.Empty();Pointer.cancel_until_release();}
        return;
    }
    if(!InspectionMode && Session.state().combat.battle.phase==rpg::Phase::Finished){Flow.finish();}
    if(!InspectionMode && PlayerOwner->WasInputKeyJustPressed(EKeys::Escape)){HandleBack();return;}
    if(HubEnabled && Flow.in_hub()){DrawHub();return;}
    if(!InspectionMode && (Flow.screen()==rpg::presentation::UiScreen::Pause || Flow.screen()==rpg::presentation::UiScreen::AbandonConfirm)){DrawBattleMenu();return;}
    auto view=Session.state();
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
        const auto result=Session.step_automatic();
        Message=result.accepted()?TEXT("Opponent acted."):TEXT("Presentation stopped; restart to retry.");
        NextAutomatic=now+0.5;SelectedSlot=0;SelectedTarget.Empty();view=Session.state();
    }
    const bool classicRules=view.combat.profile==rpg::CombatProfile::Classic;
    const bool running=view.combat.battle.phase==rpg::Phase::Running;
    const bool manual=running && !view.automatic && !view.stopped && Flow.accepts_actions(!PopupLines.IsEmpty());
    if(SelectedSlot && SelectionRevision!=view.revision){SelectedSlot=0;SelectedTarget.Empty();}
    const auto candidates=rpg::presentation::target_candidates(view,static_cast<std::uint32_t>(SelectedSlot));
    const auto validTarget=[&](const std::string& id){return std::ranges::find(candidates,id)!=candidates.end();};
    const auto submit=[&](const FString& id){
        const auto result=classicRules?Session.submit_action(SelectionRevision,{view.combat.battle.current_actor.value_or(""),rpg::ActionType::Attack,TCHAR_TO_UTF8(*id),{}}):Session.submit(SelectionRevision,static_cast<uint32>(SelectedSlot),TCHAR_TO_UTF8(*id));
        Message=result.accepted()?TEXT("Action accepted."):Label(rpg::presentation::action_error_message(result.action.error));
        UE_LOG(LogTemp, Display, TEXT("VIS_ACTION accepted=%d revision=%llu"),result.accepted()?1:0,static_cast<unsigned long long>(Session.state().revision));
        SelectedSlot=0;SelectedTarget.Empty();
    };
    const auto button=[&](float x,float y,float w,float h,const FString& text,bool enabled){
        DrawRect(enabled?FLinearColor(0.08f,0.20f,0.32f):FLinearColor(0.12f,0.12f,0.12f),x,y,w,h);DrawText(text,FColor::White,x+8,y+8);
        return clicked && enabled && rpg::presentation::pointer_clicked(Input,{x,y,w,h});
    };
    const FString actor=view.combat.battle.current_actor?Label(*view.combat.battle.current_actor):TEXT("-");
    DrawText(classic?TEXT("RPG URBANO / Classic lateral arena"):TEXT("RPG URBANO / Standard depth arena"),FColor::White,20,15);
    DrawText(FString::Printf(TEXT("Actor %s | Round %llu | Opportunity %llu | Actions %llu"),*actor,
        static_cast<unsigned long long>(view.combat.battle.round),static_cast<unsigned long long>(view.combat.battle.turn_id),static_cast<unsigned long long>(view.combat.battle.accepted_actions)),FColor::White,20,38);
    DrawText(classicRules?TEXT("Rules: Classic / Attack + Defense"):TEXT("Rules: Team / Ability slots"),FColor::Cyan,Canvas->SizeX-320.f,15);
    TArray<FBox2D> occupied;
    for(TActorIterator<AStaticMeshActor> it(GetWorld());it;++it){
        for(const auto& fighter:view.combat.battle.fighters){
            const FString id=Label(fighter.definition.id);
            if(!it->Tags.Contains(FName(*id))){continue;}
            it->SetActorHiddenInGame(fighter.health<=0);
            FVector2D screen;
            if(!PlayerOwner->ProjectWorldLocationToScreen(it->GetActorLocation()+FVector(0,0,80),screen)){continue;}
            const auto labelPosition=rpg::presentation::viewport_to_canvas(screen.X,screen.Y,CanvasCoordinates);
            if(!labelPosition){continue;}screen=FVector2D(labelPosition->x,labelPosition->y);
            const float width=150.f;
            const float x=FMath::Clamp(static_cast<float>(screen.X)-width/2,4.f,FMath::Max(4.f,Canvas->SizeX-width-4));
            float y=FMath::Clamp(static_cast<float>(screen.Y)-25,66.f,FMath::Max(66.f,Canvas->SizeY-175));
            for(int32 attempt=0;attempt<10;++attempt){
                const FBox2D box(FVector2D(x,y),FVector2D(x+width,y+44));bool overlap=false;
                for(const auto& other:occupied){if(box.Intersect(other)){overlap=true;break;}}
                if(!overlap || y<=66){break;}y=FMath::Max(66.f,y-48);
            }
            occupied.Add(FBox2D(FVector2D(x,y),FVector2D(x+width,y+44)));
            DrawLine(static_cast<float>(screen.X),static_cast<float>(screen.Y),x+width/2,y+40,FLinearColor(0.6f,0.7f,0.8f),1);
            const bool active=view.combat.battle.current_actor==fighter.definition.id;
            DrawText((active?TEXT("> "):TEXT(""))+id,active?FColor::Yellow:FColor::White,x,y);
            DrawText(FString::Printf(TEXT("%lld / %lld"),static_cast<long long>(fighter.health),static_cast<long long>(fighter.definition.max_health)),FColor::White,x,y+14);
            const auto shield=rpg::presentation::shield_display(fighter);
            DrawRect(FLinearColor(0.04f,0.04f,0.05f),x,y+31,width,7);
            DrawRect(fighter.definition.team==rpg::Team::A?FLinearColor(0.1f,0.85f,0.7f):FLinearColor(0.95f,0.25f,0.4f),x,y+31,width*static_cast<float>(shield.health_fraction),7);
            DrawRect(FLinearColor(0.65f,0.9f,1.f),x,y+31,width*static_cast<float>(shield.overlay_fraction),7);
            DrawText(TEXT("[i]"),FColor::Cyan,x+width-25,y+8);
            if(clicked && PopupLines.IsEmpty() && rpg::presentation::pointer_clicked(Input,{x+width-28,y,28,28})){OpenInfo(TEXT("Effects / ")+id,rpg::presentation::effect_details(fighter));return;}
            if(clicked && manual && validTarget(fighter.definition.id) && rpg::presentation::pointer_clicked(Input,{x,y,width-28,40})){
                if(classic){submit(id);}else{SelectedTarget=id;Message=TEXT("Target selected. Confirm when ready.");}return;
            }
            if(fighter.health>0 && (SelectedTarget==id || (classic && validTarget(fighter.definition.id) && inside(x,y,width-28,40)))){
                FString style;FParse::Value(FCommandLine::Get(),TEXT("TargetMarker="),style);
                const int32 segments=style==TEXT("Diamond")?4:28;
                // World-depth lines sit at the model's feet instead of painting over its silhouette.
                FVector center,extent;it->GetActorBounds(false,center,extent);
                const float radius=FMath::Max(extent.X,extent.Y)*1.5f;
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
        DrawText(TEXT("ACTIVE UNIT"),FColor::Cyan,portraitX+8,bottom-12);DrawText(actor,FColor::White,portraitX+15,bottom+8,nullptr,2.f);DrawText(TEXT("Portrait pending"),FColor::White,portraitX+8,bottom+55);
    }
    if(classicRules){
        const auto actorIt=std::ranges::find(view.combat.battle.fighters,view.combat.battle.current_actor.value_or(""),[](const rpg::Fighter& f){return f.definition.id;});
        const bool blocked=actorIt!=view.combat.battle.fighters.end() && rpg::incapacitated(*actorIt);
        const auto perform=[&](rpg::ActionType type){
            const auto result=Session.submit_action(view.revision,{view.combat.battle.current_actor.value_or(""),type,{},{}});
            Message=Label(rpg::presentation::action_error_message(result.action.error));SelectedSlot=0;SelectedTarget.Empty();
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
                const auto ids=rpg::presentation::target_candidates(view,1);
                if(!classic && !ids.empty()){SelectedTarget=Label(ids[static_cast<std::size_t>(FMath::RandHelper(static_cast<int32>(ids.size())))]);}
                Message=classic?TEXT("Choose a valid enemy."):TEXT("Target suggested. Change it or confirm.");return;
            }
        }
        if(button(130,bottom,100,50,TEXT("Pass"),manual && blocked)){perform(rpg::ActionType::Skip);return;}
    }
    for(const auto& ability:view.abilities){
        const auto card=rpg::presentation::ability_card(view,ability.slot.position);
        const bool ready=manual && card.selectable;
        const float x=Canvas->SizeX-660.f+(ability.slot.position-1)*215.f;
        const FString text=FString::Printf(TEXT("%u / %s"),ability.slot.position,*Label(card.kind));
        button(x,bottom,205,50,text,ready);
        const FColor color=card.badge==rpg::presentation::AbilityBadge::Passive?FColor(180,130,240):card.badge==rpg::presentation::AbilityBadge::Ready?FColor::Cyan:card.badge==rpg::presentation::AbilityBadge::Unavailable?FColor(180,180,180):FColor(245,158,11);
        const bool selected=ready && SelectedSlot==static_cast<int32>(ability.slot.position);
        DrawText(selected?TEXT("Selecionada"):Label(card.status),color,x+8,bottom+30);
        const float cx=x+189,cy=bottom+15;
        if(card.cooldown>0){
            const FString timer=FString::FromInt(card.cooldown);float width=0,height=0;GetTextSize(timer,width,height);
            const float scale=FMath::Min(1.f,32.f/FMath::Max(1.f,width));
            DrawText(timer,FColor(245,158,11),x+165,bottom+30,nullptr,scale);
        }
        if(card.badge==rpg::presentation::AbilityBadge::Locked){
            DrawLine(cx-6,cy-1,cx+6,cy-1,FLinearColor(color),1.5f);DrawLine(cx-6,cy-1,cx-6,cy+7,FLinearColor(color),1.5f);
            DrawLine(cx+6,cy-1,cx+6,cy+7,FLinearColor(color),1.5f);DrawLine(cx-6,cy+7,cx+6,cy+7,FLinearColor(color),1.5f);
            DrawLine(cx-4,cy-1,cx-4,cy-7,FLinearColor(color),1.5f);DrawLine(cx-4,cy-7,cx+4,cy-7,FLinearColor(color),1.5f);DrawLine(cx+4,cy-7,cx+4,cy-1,FLinearColor(color),1.5f);
        }else if(card.badge==rpg::presentation::AbilityBadge::Unavailable){
            for(int i=0;i<20;++i){const float a=2*PI*i/20,b=2*PI*(i+1)/20;DrawLine(cx+7*FMath::Cos(a),cy+7*FMath::Sin(a),cx+7*FMath::Cos(b),cy+7*FMath::Sin(b),FLinearColor(color),1.5f);}
            DrawLine(cx-5,cy+5,cx+5,cy-5,FLinearColor(color),1.5f);
        }else if(card.badge==rpg::presentation::AbilityBadge::Passive){DrawText(TEXT("i"),color,cx-3,cy-9);}
        if(selected){DrawLine(x+2,bottom+2,x+203,bottom+2,FLinearColor::Yellow,2);}
        if(pressed && PopupLines.IsEmpty() && inside(x,bottom,205,50)){Gesture.press(ability.slot.position,now,view.revision);}
        if(shortSlot==ability.slot.position && inside(x,bottom,205,50)){
            if(!ready){SelectedSlot=0;SelectedTarget.Empty();Message=Label(card.reason);return;}
            SelectedSlot=static_cast<int32>(ability.slot.position);SelectionRevision=view.revision;
            const auto ids=rpg::presentation::target_candidates(view,ability.slot.position);SelectedTarget.Empty();
            if(!classic && !ids.empty()){SelectedTarget=Label(ids[static_cast<std::size_t>(FMath::RandHelper(static_cast<int32>(ids.size())))]);}
            Message=classic?TEXT("Choose a valid target."):TEXT("Target suggested. Change it or confirm.");return;
        }
    }
    if(HubEnabled && PopupLines.IsEmpty() && button(20.f,bottom-42,130,30,running?TEXT("Pausar / Voltar"):TEXT("Voltar ao hub"),true)){if(running){HandleBack();}else{Flow.finish();ReturnToHub();}return;}
    DrawText(Message,FColor::White,20,bottom+60);
    if(!running && !view.progress.completions.empty()){
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
    if(!running){DrawText(view.combat.battle.winner==rpg::Team::A?TEXT("Team A wins"):view.combat.battle.winner==rpg::Team::B?TEXT("Team B wins"):TEXT("Draw"),FColor::Yellow,20,62);}
    if(!running && PopupLines.IsEmpty() && button(20.f,bottom,100,50,TEXT("Nova tentativa"),true)){
        Session.reset();Flow.finish();Flow.restart_result();Gesture.cancel();SelectedSlot=0;SelectedTarget.Empty();NextAutomatic=0;Message=TEXT("New attempt.");UE_LOG(LogTemp,Display,TEXT("VIS_RESET"));return;
    }
    if(!classic && button(Canvas->SizeX-205.f,bottom+58,180,30,TEXT("Confirm action"),manual && !SelectedTarget.IsEmpty() && SelectedSlot!=0)){
        submit(SelectedTarget);return;
    }
    if(PopupLines.IsEmpty() && clicked && manual && SelectedSlot && !classic){
        FHitResult hit;const auto point=rpg::presentation::canvas_to_viewport(mx,my,CanvasCoordinates);
        if(point && PlayerOwner->GetHitResultAtScreenPosition(FVector2D(point->x,point->y),ECC_Visibility,false,hit) && hit.GetActor()){
            for(const auto& id:candidates){if(hit.GetActor()->Tags.Contains(FName(*Label(id)))){SelectedTarget=Label(id);Message=TEXT("Target changed. Confirm when ready.");break;}}
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
        if(confirming){if(Flow.confirm_abandon()){ReturnToHub();Message=TEXT("Tentativa abandonada sem recompensa.");}}
        else{BattleMenuPage=0;Flow.request_abandon();}
    }
}
