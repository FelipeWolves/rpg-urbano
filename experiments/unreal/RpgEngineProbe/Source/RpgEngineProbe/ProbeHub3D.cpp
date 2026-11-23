#include "ProbeArena.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Canvas.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"
#include "Components/PointLightComponent.h"
#include "Engine/PointLight.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"

namespace {
const FVector PlazaOrigin(18000,0,0);
const FVector TerminalLabel(920,0,490);
}

bool AProbeArenaHUD::UpdateHub3D()
{
    if(!FParse::Param(FCommandLine::Get(),TEXT("Hub3D"))){return false;}
    if(!Hub3DConfigured){
        Hub3DConfigured=true;
        auto* cube=LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cube.Cube"));
        auto* cylinder=LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
        if(!cube || !cylinder){return false;}
        Hub3DCamera=GetWorld()->SpawnActor<ACameraActor>();
        if(!Hub3DCamera){return false;}
        Hub3DCamera->GetCameraComponent()->SetFieldOfView(65);
        const auto volume=[&](UStaticMesh* mesh,FVector center,FVector scale){
            auto* actor=GetWorld()->SpawnActor<AStaticMeshActor>(PlazaOrigin+center,FRotator::ZeroRotator);
            if(!actor){return;}
            auto* component=actor->GetStaticMeshComponent();
            component->SetMobility(EComponentMobility::Movable);
            component->SetStaticMesh(mesh);
            component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            actor->SetActorScale3D(scale);
        };
        volume(cube,{480,0,-28},{28,28,0.5});
        volume(cube,{1080,0,230},{3,12,4.6});
        volume(cube,{890,0,460},{1.8,13,0.35});
        for(double y:{-430.,-220.,220.,430.}){
            volume(cube,{900,y,225},{0.45,0.45,4.5});
        }
        volume(cylinder,{40,-205,35},{2.6,2.6,0.7});
        volume(cylinder,{40,-205,94},{0.75,0.75,1.18});
        volume(cylinder,{40,-205,160},{1.5,1.5,0.22});
        volume(cube,{220,300,35},{0.6,2,0.65});
        volume(cube,{1450,-920,300},{4,5,6});
        volume(cube,{1600,980,360},{4,5,7.2});
        if(auto* light=GetWorld()->SpawnActor<APointLight>(PlazaOrigin+FVector(100,-450,800),FRotator::ZeroRotator)){
            if(auto* component=light->FindComponentByClass<UPointLightComponent>()){
                component->SetMobility(EComponentMobility::Movable);
                component->SetIntensity(65000.f);
                component->SetAttenuationRadius(3800.f);
            }
        }
    }
    if(!Hub3DCamera){return false;}
    if(PlayerOwner->GetViewTarget()!=Hub3DCamera){
        Hub3DOriginalCamera=PlayerOwner->GetViewTarget();
        PlayerOwner->SetViewTarget(Hub3DCamera);
    }
    using namespace rpg::presentation;
    const bool approaching=Flow.screen()==UiScreen::Location && HubPanelRoute==HubRoute::Terminal;
    const double target=approaching?1.0:0.0;
    Hub3DApproach=HubReducedMotion?target:FMath::FInterpConstantTo(Hub3DApproach,target,static_cast<double>(GetWorld()->GetDeltaSeconds()),1.35);
    const double blend=FMath::SmoothStep(0.0,1.0,Hub3DApproach);
    const FVector eye=FMath::Lerp(FVector(-465,-260,455),FVector(390,-82,318),blend);
    const FVector look=FMath::Lerp(FVector(780,0,220),FVector(960,0,275),blend);
    Hub3DCamera->SetActorLocationAndRotation(PlazaOrigin+eye,(look-eye).Rotation());
    return true;
}

void AProbeArenaHUD::RestoreHub3DCamera()
{
    if(Hub3DCamera && PlayerOwner->GetViewTarget()==Hub3DCamera && IsValid(Hub3DOriginalCamera)){
        PlayerOwner->SetViewTarget(Hub3DOriginalCamera);
        Hub3DApproach=0;
    }
}

void AProbeArenaHUD::DrawHub3DScene()
{
    using namespace rpg::presentation;
    if(Flow.screen()!=UiScreen::Hub || Hub3DApproach>0.01){return;}
    const FVector point=Canvas->Project(PlazaOrigin+TerminalLabel);
    if(point.Z<=0){return;}
    const FString label=TEXT("TERMINAL");
    float width=0,height=0;
    GetTextSize(label,width,height,HubLabelFont);
    const Rect target{point.X-width/2-12,point.Y-24,width+24,48};
    const bool hover=InputWindowActive && contains(target,Input.x,Input.y);
    if(auto* halo=UiTexture(TEXT("halo_label_terminal"))){
        DrawTexture(halo,point.X-width*0.7,point.Y-height*0.9,width*1.4,height*1.8,0,0,1,1,
            FLinearColor(0.15f,0.75f,1.f,hover?0.85f:0.28f),BLEND_Translucent);
    }
    DrawText(label,hover?FColor::White:FColor(155,225,245),point.X-width/2,point.Y-height/2,HubLabelFont);
    DrawText(TEXT("Prévia 3D | Praça e Terminal"),FColor::White,18,Canvas->SizeY-36,HubBodyFont);
    if(pointer_clicked(Input,target)){
        HubPanelRoute=HubRoute::Terminal;
        TerminalMenuPage=0;
        HubPanelPage=0;
        Flow.open_location();
        Pointer.cancel_until_release();
    }
}
