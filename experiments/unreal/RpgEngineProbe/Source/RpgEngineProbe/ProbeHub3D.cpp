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
struct DestinationAnchor {
    std::string_view id;
    FVector point;
};
const DestinationAnchor DestinationAnchors[]={
    {"terminal",{820,0,510}},
    {"library",{995,-920,480}},
    {"lab-exp",{550,-840,320}},
    {"plaza",{55,-320,190}},
    {"games",{430,315,240}},
    {"arena-pvp",{1050,970,460}},
    {"pub",{435,960,215}},
};
}

bool AProbeArenaHUD::UpdateHub3D()
{
    if(!Hub3DEnabled){RestoreHub3DCamera();return false;}
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
        // Library and Lab occupy the left wing. Games is a low pavilion near the square.
        volume(cube,{1120,-930,220},{3.5,5.8,4.4});
        volume(cube,{1010,-930,450},{2.8,6.4,0.38});
        for(double y:{-1110.,-930.,-750.}){
            volume(cylinder,{935,y,213},{0.44,0.44,4.2});
        }
        volume(cube,{680,-860,140},{2.4,4.2,2.8});
        volume(cube,{559,-860,294},{2.8,4.8,0.28});
        volume(cylinder,{510,340,100},{3.2,4.3,2.0});
        volume(cylinder,{510,340,204},{3.7,4.7,0.3});
        // Arena deck and rail above the right wing, with a separate pub at street level.
        volume(cube,{1200,970,190},{4.8,6,3.8});
        volume(cube,{1080,975,390},{5.4,7,0.3});
        volume(cube,{1080,975,430},{4.8,6.4,0.5});
        volume(cube,{570,970,95},{2.8,4.1,1.9});
        volume(cube,{417,980,194},{1.4,4.7,0.22});
        for(int step=0;step<5;++step){
            volume(cube,{590+step*67.0,680,20+step*20.0},{0.9,1.5,0.4+step*0.4});
        }
        // Skyline blocks and planters provide depth references during the camera move.
        for(int i=0;i<7;++i){
            const double height=5.0+(i%3)*1.4;
            volume(cube,{1970.0+(i%2)*140,-1500.0+i*500,height*50},{2.8,3.1,height});
        }
        for(double y:{-660.,580.}){
            volume(cylinder,{145,y,32},{1.2,1.2,0.64});
            volume(cylinder,{145,y,108},{0.21,0.21,1.5});
            volume(cylinder,{145,y,205},{1.3,1.3,0.85});
        }
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
    const FVector eye=FMath::Lerp(FVector(-1050,-150,620),FVector(390,-82,318),blend);
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

const rpg::presentation::HubHotspot* AProbeArenaHUD::DrawHub3DScene(const rpg::presentation::Rect& scene)
{
    using namespace rpg::presentation;
    if(Flow.screen()!=UiScreen::Hub || Hub3DApproach>0.01){return nullptr;}
    for(const auto& link:HubLinks){
        const DestinationAnchor* anchor=nullptr;
        for(const auto& candidate:DestinationAnchors){
            if(candidate.id==link.id){anchor=&candidate;break;}
        }
        if(!anchor){continue;}
        const FVector point=Canvas->Project(PlazaOrigin+anchor->point);
        if(point.Z<=0 || !contains(scene,point.X,point.Y)){continue;}
        const auto upper=hub_uppercase(link.label);
        const FString label=UTF8_TO_TCHAR(upper.c_str());
        float width=0,height=0;
        GetTextSize(label,width,height,HubLabelFont);
        const Rect target{point.X-width/2-12,point.Y-24,width+24,48};
        const bool hover=InputWindowActive && contains(target,Input.x,Input.y);
        const FString haloId=TEXT("halo_label_")+FString(UTF8_TO_TCHAR(link.id.c_str()));
        const auto* geometry=LabelGlowGeometry.Find(haloId);
        if(auto* halo=UiTexture(haloId);halo && geometry && LabelGlowText.FindRef(haloId)==label){
            const float sx=width/geometry->X,sy=height/geometry->Y;
            DrawTexture(halo,point.X-width/2-geometry->Z*sx,point.Y-height/2-geometry->W*sy,
                halo->GetSizeX()*sx,halo->GetSizeY()*sy,0,0,1,1,
                FLinearColor(0.15f,0.75f,1.f,hover?0.85f:0.28f),BLEND_Translucent);
        }
        DrawText(label,hover?FColor::White:FColor(155,225,245),point.X-width/2,point.Y-height/2,HubLabelFont);
        if(pointer_clicked(Input,target)){return &link;}
    }
    return nullptr;
}
