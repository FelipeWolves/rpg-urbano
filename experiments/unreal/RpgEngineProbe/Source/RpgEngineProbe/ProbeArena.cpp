#include "ProbeArena.h"
#include "Camera/CameraActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Canvas.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "UObject/ConstructorHelpers.h"
namespace {
FString Label(const std::string& value){return UTF8_TO_TCHAR(value.c_str());}
}
AProbeArenaMode::AProbeArenaMode(){HUDClass=AProbeArenaHUD::StaticClass();DefaultPawnClass=nullptr;}
void AProbeArenaMode::BeginPlay()
{
    Super::BeginPlay();
    if(auto* pc=GetWorld()->GetFirstPlayerController()){
        auto* camera=GetWorld()->SpawnActor<ACameraActor>(FVector(0,-850,560),FRotator(-33,90,0));
        if(camera){pc->SetViewTarget(camera);}
        pc->bShowMouseCursor=true;
        pc->SetInputMode(FInputModeGameAndUI());
    }
    // Engine-owned primitive, no downloaded character assets.
    auto* mesh=LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    for(int32 i=0;i<6;++i){
        auto* sphere=GetWorld()->SpawnActor<AStaticMeshActor>(FVector(i<3?230:-230,(i%3-1)*170,60),FRotator::ZeroRotator);
        if(sphere && mesh){sphere->GetStaticMeshComponent()->SetStaticMesh(mesh);sphere->Tags.Add(FName(*FString::Printf(TEXT("%c%d"),i<3?TEXT('A'):TEXT('B'),i%3+1)));}
    }
}
void AProbeArenaHUD::DrawHUD()
{
    Super::DrawHUD();
    if(!Canvas || !PlayerOwner){return;}
    auto view=Session.state();
    if(view.automatic && !view.stopped && GetWorld()->GetTimeSeconds()>=NextAutomatic){
        const auto result=Session.step_automatic();
        Message=result.accepted()?TEXT("Opponent acted."):TEXT("Presentation stopped; restart to retry.");
        NextAutomatic=GetWorld()->GetTimeSeconds()+0.5;
        SelectedSlot=0;view=Session.state();
    }
    float mx=0,my=0;PlayerOwner->GetMousePosition(mx,my);
    const bool clicked=PlayerOwner->WasInputKeyJustPressed(EKeys::LeftMouseButton);
    const auto button=[&](float x,float y,float w,float h,const FString& text,bool enabled){
        DrawRect(enabled?FLinearColor(0.08f,0.20f,0.32f):FLinearColor(0.12f,0.12f,0.12f),x,y,w,h);
        DrawText(text,FColor::White,x+8,y+8);
        return clicked && enabled && mx>=x && mx<x+w && my>=y && my<y+h;
    };
    const bool running=view.combat.battle.phase==rpg::Phase::Running;
    const bool manual=running && !view.automatic && !view.stopped;
    if(SelectedSlot && SelectionRevision!=view.revision){SelectedSlot=0;}
    const FString actor=view.combat.battle.current_actor?Label(*view.combat.battle.current_actor):TEXT("-");
    DrawText(TEXT("RPG URBANO / battle laboratory"),FColor::White,20,15);
    DrawText(FString::Printf(TEXT("Actor %s | Round %llu | Opportunity %llu | Actions %llu"),*actor,
        static_cast<unsigned long long>(view.combat.battle.round),static_cast<unsigned long long>(view.combat.battle.turn_id),
        static_cast<unsigned long long>(view.combat.battle.accepted_actions)),FColor::White,20,38);
    // Labels follow the world rather than forming a second, unrelated roster.
    for(TActorIterator<AStaticMeshActor> it(GetWorld());it;++it){
        for(const auto& fighter:view.combat.battle.fighters){
            if(!it->Tags.Contains(FName(*Label(fighter.definition.id)))){continue;}
            it->SetActorHiddenInGame(fighter.health<=0);
            FVector2D screen;
            if(!PlayerOwner->ProjectWorldLocationToScreen(it->GetActorLocation()+FVector(0,0,80),screen)){continue;}
            const float width=170.f;
            const float x=FMath::Clamp(static_cast<float>(screen.X)-width/2,4.f,Canvas->SizeX-width-4);
            const float y=FMath::Clamp(static_cast<float>(screen.Y)-25,66.f,Canvas->SizeY-175);
            const bool active=view.combat.battle.current_actor==fighter.definition.id;
            const bool ally=fighter.definition.team==rpg::Team::A;
            DrawRect(active?FLinearColor(1.f,0.75f,0.2f):FLinearColor(0.15f,0.2f,0.26f),x-3,y-3,width+6,42);
            const FString caption=FString::Printf(TEXT("%s%s  %d / %d"),active?TEXT("> "):TEXT(""),*Label(fighter.definition.id),fighter.health,fighter.definition.max_health);
            if(button(x,y,width,36,caption,manual && SelectedSlot!=0 && fighter.health>0)){
                const auto result=Session.submit(SelectionRevision,static_cast<uint32>(SelectedSlot),fighter.definition.id);
                Message=result.accepted()?TEXT("Action accepted."):TEXT("Target or ability unavailable; choose again.");
                UE_LOG(LogTemp, Display, TEXT("VIS_ACTION accepted=%d revision=%llu"),result.accepted()?1:0,static_cast<unsigned long long>(Session.state().revision));SelectedSlot=0;return;
            }
            DrawRect(FLinearColor(0.05f,0.05f,0.06f),x+5,y+26,width-10,6);
            DrawRect(ally?FLinearColor(0.1f,0.85f,0.7f):FLinearColor(0.95f,0.25f,0.4f),x+5,y+26,(width-10)*fighter.health/fighter.definition.max_health,6);
            int shield=0;for(const auto& status:fighter.statuses){if(status.kind==rpg::EffectKind::Shield || status.kind==rpg::EffectKind::StructuralShield){shield+=status.magnitude;}}
            if(shield){DrawText(FString::Printf(TEXT("Shield %d"),shield),FColor::Cyan,x+90,y+8);}
        }
    }
    const float bottom=FMath::Max(280.f,Canvas->SizeY-135.f);
    DrawRect(FLinearColor(0.025f,0.035f,0.06f,0.9f),0,bottom-12,Canvas->SizeX,135);
    for(const auto& ability:view.abilities){
        const bool ready=manual && ability.availability==rpg::AbilityAvailability::Ready;
        FString text=FString::Printf(TEXT("%u: %s"),ability.slot.position,*Label(ability.slot.name).Left(20));
        FString status;
        if(ability.availability==rpg::AbilityAvailability::Cooldown){status=FString::Printf(TEXT("Cooldown: %d"),ability.cooldown);}
        else if(ability.availability==rpg::AbilityAvailability::PassiveOnly){status=TEXT("Passive / not selectable");}
        else if(ability.availability==rpg::AbilityAvailability::ConditionBlocked){status=TEXT("Locked: owner HP condition");}
        else if(!ready){status=TEXT("Unavailable");}
        else{status=SelectedSlot==static_cast<int32>(ability.slot.position)?TEXT("Selected / choose target"):TEXT("Ready");}
        if(button(Canvas->SizeX-660.f+(ability.slot.position-1)*215.f,bottom,205,50,text,ready)){
            SelectedSlot=static_cast<int32>(ability.slot.position);SelectionRevision=view.revision;
            Message=TEXT("Now choose a target.");return;
        }
        DrawText(status,FColor::White,Canvas->SizeX-652.f+(ability.slot.position-1)*215.f,bottom+30);
    }
    DrawText(Message,FColor::White,20,bottom+60);
    if(!running){DrawText(view.combat.battle.winner==rpg::Team::A?TEXT("Team A wins"):view.combat.battle.winner==rpg::Team::B?TEXT("Team B wins"):TEXT("Draw"),FColor::Yellow,20,62);}
    if(button(20.f,bottom,100,50,TEXT("Restart"),true)){
        Session.reset();UE_LOG(LogTemp, Display, TEXT("VIS_RESET"));SelectedSlot=0;NextAutomatic=0;Message=TEXT("New attempt.");
    }
}
