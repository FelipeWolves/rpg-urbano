#include "ProbeArena.h"
#include <algorithm>
#include "Camera/CameraActor.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "rpg/arena_layout.hpp"
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
rpg::CombatProfile SelectedCombatProfile(){FString name;if(FParse::Value(FCommandLine::Get(),TEXT("CombatProfile="),name) && name==TEXT("Classic")){return rpg::CombatProfile::Classic;}if(!name.IsEmpty() && name!=TEXT("Team")){UE_LOG(LogTemp,Warning,TEXT("Unknown CombatProfile; using Team"));}return rpg::CombatProfile::Team;}
std::optional<rpg::presentation::ArenaLayout> SelectedLayout(){FString name;if(!FParse::Value(FCommandLine::Get(),TEXT("ArenaLayout="),name)){return rpg::presentation::default_arena_layout;}return rpg::presentation::parse_arena_layout(TCHAR_TO_UTF8(*name));}
}
AProbeArenaHUD::AProbeArenaHUD():Session(SelectedCombatProfile()==rpg::CombatProfile::Classic?rpg::classic_reference_setup():rpg::team_reference_setup(),SelectedCombatProfile()){}
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
        pc->SetInputMode(FInputModeGameAndUI());
    }
    UE_LOG(LogTemp, Display, TEXT("ARENA_LAYOUT %s"),classic?TEXT("Classic"):TEXT("Standard"));
}
void AProbeArenaHUD::BeginPlay()
{
    Super::BeginPlay();
    HubEnabled=FParse::Param(FCommandLine::Get(),TEXT("Hub"));InHub=HubEnabled;
    if(HubEnabled){LoadHubCatalog();}
    const auto layout=SelectedLayout();
    if(!layout){return;}
    std::vector<rpg::Definition> roster;
    for(const auto& fighter:Session.state().combat.battle.fighters){roster.push_back(fighter.definition);}
    auto* mesh=LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    for(const auto& placement:rpg::presentation::arena_placements(roster,*layout)){
        auto* sphere=GetWorld()->SpawnActor<AStaticMeshActor>(FVector(placement.x,placement.y,placement.z),FRotator(0,placement.facing_yaw,0));
        if(sphere && mesh){sphere->GetStaticMeshComponent()->SetStaticMesh(mesh);sphere->Tags.Add(FName(*Label(placement.fighter_id)));}
    }
}
void AProbeArenaHUD::OpenInfo(const FString& title,const std::vector<std::string>& lines)
{
    PopupTitle=title;PopupLines.Empty();PopupPage=0;
    for(const auto& value:lines){
        FString text=Label(value);
        while(text.Len()>78){int32 split=78;int32 space=0;if(text.Left(78).FindLastChar(TEXT(' '),space) && space>0){split=space;}PopupLines.Add(text.Left(split));text=text.Mid(split).TrimStart();}
        PopupLines.Add(text);
    }
    if(PopupLines.IsEmpty()){PopupLines.Add(TEXT("No information available."));}
}
void AProbeArenaHUD::DrawHUD()
{
    Super::DrawHUD();if(!Canvas || !PlayerOwner){return;}
    if(HubEnabled && InHub){DrawHub();return;}
    if(HubEnabled && PlayerOwner->WasInputKeyJustPressed(EKeys::Escape)){
        if(!PopupLines.IsEmpty()){PopupLines.Empty();Gesture.cancel();}else{ReturnToHub();}return;
    }
    auto view=Session.state();
    const double now=GetWorld()->GetTimeSeconds();
    const auto layout=SelectedLayout();
    const bool classic=layout==rpg::presentation::ArenaLayout::ClassicLateral;
    float mx=0,my=0;PlayerOwner->GetMousePosition(mx,my);
    const bool pressed=PlayerOwner->WasInputKeyJustPressed(EKeys::LeftMouseButton);
    const bool released=PlayerOwner->WasInputKeyJustReleased(EKeys::LeftMouseButton);
    if(auto held=Gesture.hold(now,view.revision)){OpenInfo(TEXT("Ability details"),rpg::presentation::ability_details(view,*held));}
    const bool abilityRelease=Gesture.active() && released;
    const auto shortSlot=released?Gesture.release(now,view.revision):std::optional<std::uint32_t>{};
    const bool clicked=released && !abilityRelease;
    const auto inside=[&](float x,float y,float w,float h){return mx>=x && mx<x+w && my>=y && my<y+h;};
    if(PopupLines.IsEmpty() && !Gesture.active() && !abilityRelease && view.automatic && !view.stopped && now>=NextAutomatic){
        const auto result=Session.step_automatic();
        Message=result.accepted()?TEXT("Opponent acted."):TEXT("Presentation stopped; restart to retry.");
        NextAutomatic=now+0.5;SelectedSlot=0;SelectedTarget.Empty();view=Session.state();
    }
    const bool classicRules=view.combat.profile==rpg::CombatProfile::Classic;
    const bool running=view.combat.battle.phase==rpg::Phase::Running;
    const bool manual=running && !view.automatic && !view.stopped && PopupLines.IsEmpty();
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
        return clicked && enabled && inside(x,y,w,h);
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
            DrawText(FString::Printf(TEXT("%d / %d"),fighter.health,fighter.definition.max_health),FColor::White,x,y+14);
            const auto shield=rpg::presentation::shield_display(fighter);
            DrawRect(FLinearColor(0.04f,0.04f,0.05f),x,y+31,width,7);
            DrawRect(fighter.definition.team==rpg::Team::A?FLinearColor(0.1f,0.85f,0.7f):FLinearColor(0.95f,0.25f,0.4f),x,y+31,width*static_cast<float>(shield.health_fraction),7);
            DrawRect(FLinearColor(0.65f,0.9f,1.f),x,y+31,width*static_cast<float>(shield.overlay_fraction),7);
            DrawText(TEXT("[i]"),FColor::Cyan,x+width-25,y+8);
            if(clicked && PopupLines.IsEmpty() && inside(x+width-28,y,28,28)){OpenInfo(TEXT("Effects / ")+id,rpg::presentation::effect_details(fighter));return;}
            if(clicked && manual && validTarget(fighter.definition.id) && inside(x,y,width-28,40)){
                if(classic){submit(id);}else{SelectedTarget=id;Message=TEXT("Target selected. Confirm when ready.");}return;
            }
            if(fighter.health>0 && (SelectedTarget==id || (classic && validTarget(fighter.definition.id) && inside(x,y,width-28,40)))){
                FString style;FParse::Value(FCommandLine::Get(),TEXT("TargetMarker="),style);
                const int32 segments=style==TEXT("Diamond")?4:28;
                FVector2D previous=FVector2D::ZeroVector;bool previousValid=false;
                for(int32 step=0;step<=segments;++step){
                    const float angle=2*PI*step/segments;FVector2D point=FVector2D::ZeroVector;
                    const bool valid=PlayerOwner->ProjectWorldLocationToScreen(it->GetActorLocation()+FVector(75*FMath::Cos(angle),75*FMath::Sin(angle),-52),point);
                    if(valid && previousValid){DrawLine(static_cast<float>(previous.X),static_cast<float>(previous.Y),static_cast<float>(point.X),static_cast<float>(point.Y),FLinearColor(0.3f,0.95f,1.f),3);}
                    previous=point;previousValid=valid;
                }
            }
        }
    }
    const float bottom=FMath::Max(280.f,Canvas->SizeY-135.f);
    DrawRect(FLinearColor(0.025f,0.035f,0.06f,0.9f),0,bottom-12,Canvas->SizeX,135);
    if(!classic){
        const float portraitX=Canvas->SizeX-795.f;DrawRect(FLinearColor(0.06f,0.12f,0.2f),portraitX,bottom-20,120,95);
        DrawText(TEXT("ACTIVE UNIT"),FColor::Cyan,portraitX+8,bottom-12);DrawText(actor,FColor::White,portraitX+15,bottom+8,nullptr,2.f);DrawText(TEXT("Portrait pending"),FColor::White,portraitX+8,bottom+55);
    }
    if(classicRules){
        const auto actorIt=std::ranges::find(view.combat.battle.fighters,view.combat.battle.current_actor.value_or(""),[](const rpg::Fighter& f){return f.definition.id;});
        const bool blocked=actorIt!=view.combat.battle.fighters.end() && std::ranges::any_of(actorIt->statuses,[](const rpg::Status& status){return status.kind==rpg::EffectKind::Block;});
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
        const bool ready=manual && ability.availability==rpg::AbilityAvailability::Ready;
        const float x=Canvas->SizeX-660.f+(ability.slot.position-1)*215.f;
        const FString type=ability.slot.active_skill?(ability.slot.position==1?TEXT("BASIC"):TEXT("SKILL")):TEXT("PASSIVE");
        const FString text=FString::Printf(TEXT("%u / %s"),ability.slot.position,*type);
        FString status;
        if(ability.availability==rpg::AbilityAvailability::Cooldown){status=FString::Printf(TEXT("Cooldown: %d"),ability.cooldown);}
        else if(ability.availability==rpg::AbilityAvailability::PassiveOnly){status=TEXT("Passive / hold for details");}
        else if(ability.availability==rpg::AbilityAvailability::ConditionBlocked){status=TEXT("Locked: hold for condition");}
        else if(!ready){status=TEXT("Unavailable / hold for info");}
        else{status=SelectedSlot==static_cast<int32>(ability.slot.position)?TEXT("Selected"):TEXT("Ready / hold for details");}
        button(x,bottom,205,50,text,ready);DrawText(status,FColor::White,x+8,bottom+30);
        if(pressed && PopupLines.IsEmpty() && inside(x,bottom,205,50)){Gesture.press(ability.slot.position,now,view.revision);}
        if(shortSlot==ability.slot.position && inside(x,bottom,205,50) && ready){
            SelectedSlot=static_cast<int32>(ability.slot.position);SelectionRevision=view.revision;
            const auto ids=rpg::presentation::target_candidates(view,ability.slot.position);SelectedTarget.Empty();
            if(!classic && !ids.empty()){SelectedTarget=Label(ids[static_cast<std::size_t>(FMath::RandHelper(static_cast<int32>(ids.size())))]);}
            Message=classic?TEXT("Choose a valid target."):TEXT("Target suggested. Change it or confirm.");return;
        }
    }
    if(HubEnabled && PopupLines.IsEmpty() && button(20.f,bottom-42,130,30,TEXT("Voltar ao hub"),true)){ReturnToHub();return;}
    DrawText(Message,FColor::White,20,bottom+60);
    if(!running){DrawText(view.combat.battle.winner==rpg::Team::A?TEXT("Team A wins"):view.combat.battle.winner==rpg::Team::B?TEXT("Team B wins"):TEXT("Draw"),FColor::Yellow,20,62);}
    if(PopupLines.IsEmpty() && button(20.f,bottom,100,50,TEXT("Restart"),true)){
        Session.reset();Gesture.cancel();SelectedSlot=0;SelectedTarget.Empty();NextAutomatic=0;Message=TEXT("New attempt.");UE_LOG(LogTemp,Display,TEXT("VIS_RESET"));return;
    }
    if(!classic && button(Canvas->SizeX-205.f,bottom+58,180,30,TEXT("Confirm action"),manual && !SelectedTarget.IsEmpty() && SelectedSlot!=0)){
        submit(SelectedTarget);return;
    }
    if(PopupLines.IsEmpty() && clicked && manual && SelectedSlot && !classic){
        FHitResult hit;
        if(PlayerOwner->GetHitResultUnderCursor(ECC_Visibility,false,hit) && hit.GetActor()){
            for(const auto& id:candidates){if(hit.GetActor()->Tags.Contains(FName(*Label(id)))){SelectedTarget=Label(id);Message=TEXT("Target changed. Confirm when ready.");break;}}
        }
    }
    if(!PopupLines.IsEmpty()){
        Gesture.cancel();const float w=FMath::Min(760.f,Canvas->SizeX-30.f),h=FMath::Min(490.f,Canvas->SizeY-60.f);
        const float x=(Canvas->SizeX-w)/2,y=(Canvas->SizeY-h)/2;
        DrawRect(FLinearColor(0.02f,0.035f,0.06f),x,y,w,h);DrawText(PopupTitle,FColor::Cyan,x+16,y+16);
        const int32 visible=FMath::Max(1,static_cast<int32>((h-110)/22));const int32 pages=FMath::Max(1,(PopupLines.Num()+visible-1)/visible);PopupPage=FMath::Clamp(PopupPage,0,pages-1);
        for(int32 line=0;line<visible && PopupPage*visible+line<PopupLines.Num();++line){DrawText(PopupLines[PopupPage*visible+line],FColor::White,x+16,y+55+line*22);}
        if(button(x+w-105,y+10,90,32,TEXT("Close"),true)){PopupLines.Empty();return;}
        if(button(x+16,y+h-42,90,30,TEXT("Previous"),PopupPage>0)){--PopupPage;}
        if(button(x+120,y+h-42,90,30,TEXT("Next"),PopupPage+1<pages)){++PopupPage;}
        DrawText(FString::Printf(TEXT("%d / %d"),PopupPage+1,pages),FColor::White,x+230,y+h-34);
    }
}
