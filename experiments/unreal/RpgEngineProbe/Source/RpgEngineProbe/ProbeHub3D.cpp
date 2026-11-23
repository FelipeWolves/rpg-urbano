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
    {"terminal",{820,-55,555}},
    {"library",{945,-950,540}},
    {"lab-exp",{595,-1230,320}},
    {"plaza",{-300,-580,165}},
    {"games",{470,195,225}},
    {"arena-pvp",{655,900,275}},
    {"pub",{-160,880,52}},
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
        Hub3DCamera->GetCameraComponent()->SetFieldOfView(75);
        const auto volume=[&](UStaticMesh* mesh,FVector center,FVector scale,FRotator rotation=FRotator::ZeroRotator){
            auto* actor=GetWorld()->SpawnActor<AStaticMeshActor>(PlazaOrigin+center,rotation);
            if(!actor){return;}
            auto* component=actor->GetStaticMeshComponent();
            component->SetMobility(EComponentMobility::Movable);
            component->SetStaticMesh(mesh);
            component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            actor->SetActorScale3D(scale);
        };
        // The main terrace leaves a lower pocket on the right for the Pub.
        volume(cube,{570,-470,-28},{32,22,0.5});
        volume(cube,{1130,1030,-28},{20.8,8,0.5});
        volume(cube,{-340,1030,-175},{8.6,8,0.5});
        for(int step=0;step<6;++step){
            volume(cube,{70-step*54.0,934,-13-step*24.5},{0.54,3.0,0.2});
        }
        volume(cube,{1080,0,230},{3,12,4.6});
        volume(cube,{890,0,460},{1.8,13,0.35});
        for(double y:{-430.,-220.,220.,430.}){
            volume(cube,{900,y,225},{0.45,0.45,4.5});
        }
        volume(cylinder,{-300,-580,35},{2.6,2.6,0.7});
        volume(cylinder,{-300,-580,94},{0.75,0.75,1.18});
        volume(cylinder,{-300,-580,160},{1.5,1.5,0.22});
        volume(cube,{-170,-330,35},{0.6,2,0.65});
        // Library and Lab occupy the left wing. Games is a low pavilion near the square.
        volume(cube,{1120,-995,240},{3.5,5.8,4.8});
        volume(cube,{1010,-995,492},{2.8,6.4,0.38});
        for(double y:{-1110.,-930.,-750.}){
            volume(cylinder,{935,y,213},{0.44,0.44,4.2});
        }
        volume(cube,{680,-1230,140},{2.4,4.2,2.8});
        volume(cube,{559,-1230,294},{2.8,4.8,0.28});
        volume(cylinder,{510,195,100},{3.2,4.3,2.0});
        volume(cylinder,{510,195,204},{3.7,4.7,0.3});
        volume(cube,{835,935,135},{3.5,5.3,2.7});
        volume(cube,{735,935,267},{2.4,5.7,0.18});
        volume(cube,{-260,1150,-55},{3.0,3.8,1.9});
        volume(cube,{-290,923,31},{2.6,0.7,0.18});

        // Track turns out of the Terminal and advances toward the near right edge.
        const FVector track[]={{930,365,440},{865,610,423},{655,835,405},
            {355,1045,390},{30,1230,375},{-410,1430,362}};
        for(int i=1;i<UE_ARRAY_COUNT(track);++i){
            const FVector start=track[i-1],end=track[i];
            const FVector delta=end-start;
            const FVector midpoint=(start+end)*0.5;
            const FVector lateral=FVector::CrossProduct(FVector::UpVector,delta).GetSafeNormal();
            const FRotator heading=delta.Rotation();
            const double length=(delta.Size()+15.0)/100.0;
            volume(cube,midpoint,{length,1.32,0.22},heading);
            for(double offset:{-41.,41.}){
                volume(cube,midpoint+lateral*offset+FVector(0,0,16),{length,0.075,0.075},heading);
            }
            if(i>=2){
                const double base=midpoint.X<100 && midpoint.Y>630?-150.0:0.0;
                volume(cube,{midpoint.X,midpoint.Y,(midpoint.Z+base)*0.5},
                    {0.5,0.48,(midpoint.Z-base)/100.0});
            }
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
    const FVector eye=FMath::Lerp(FVector(-1520,0,715),FVector(390,-82,318),blend);
    const FVector look=FMath::Lerp(FVector(660,0,250),FVector(960,0,275),blend);
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
    for(std::size_t index=0;index<HubLinks.size();++index){
        const auto& link=HubLinks[index];
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
        GetTextSize(label,width,height,HubLabelFont?HubLabelFont.Get():HubTitleFont.Get());
        const Rect target{point.X-width/2-12,point.Y-24,width+24,48};
        const bool hover=InputWindowActive && contains(target,Input.x,Input.y);
        DrawAnimatedHubLabel(link,index,label,point.X-width/2,point.Y-height/2,1.f,width,height,hover);
        if(pointer_clicked(Input,target)){return &link;}
    }
    return nullptr;
}
