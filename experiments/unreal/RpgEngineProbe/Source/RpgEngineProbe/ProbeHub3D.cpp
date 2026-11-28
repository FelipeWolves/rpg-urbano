#include "ProbeArena.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Canvas.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/PointLightComponent.h"
#include "Engine/PointLight.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"

namespace {
const FVector PlazaOrigin(18000,0,0);
struct AmbientSignal {
    FVector position;
    FLinearColor color;
    double period;
};
const AmbientSignal AmbientSignals[]={
    {{520,-1000,325},{0.59f,0.27f,0.90f,1.f},6.5},
    {{900,-550,450},{0.27f,0.72f,0.94f,1.f},8.0},
    {{120,105,220},{0.22f,0.82f,0.66f,1.f},7.2},
    {{385,1010,392},{0.88f,0.52f,0.21f,1.f},9.0},
};
struct DestinationAnchor {
    std::string_view id;
    FVector point;
};
const DestinationAnchor DestinationAnchors[]={
    {"terminal",{820,-55,555}},
    {"library",{955,-890,540}},
    {"lab-exp",{600,-1130,330}},
    {"plaza",{-425,-345,175}},
    {"games",{165,105,225}},
    {"arena-pvp",{690,825,280}},
    {"pub",{-160,880,52}},
};
}

bool AProbeArenaHUD::UpdateHub3D(bool focused)
{
    if(!Hub3DEnabled){RestoreHub3DCamera();return false;}
    if(!Hub3DConfigured){
        Hub3DConfigured=true;
        auto* cube=LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cube.Cube"));
        auto* cylinder=LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
        if(!cube || !cylinder){return false;}
        Hub3DCamera=GetWorld()->SpawnActor<ACameraActor>();
        if(!Hub3DCamera){return false;}
    auto* camera=Hub3DCamera->GetCameraComponent();
    camera->SetAspectRatio(9.f/16.f);
    camera->SetConstraintAspectRatio(focused);
    camera->SetFieldOfView(focused?40.f:55.f);
        Hub3DCamera->GetCameraComponent()->SetFieldOfView(75);
        const auto finish=[&](FLinearColor color,float roughness){
            auto* parent=LoadObject<UMaterialInterface>(nullptr,TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
            FLinearColor defaultColor;
            if(!parent || !parent->GetVectorParameterValue(FMaterialParameterInfo(TEXT("Color")),defaultColor)){
                // The arena marker uses this same color-parameter material.
                parent=TargetMarkerMaterial.Get();
            }
            if(!parent){return static_cast<UMaterialInstanceDynamic*>(nullptr);}
            auto* material=UMaterialInstanceDynamic::Create(parent,this);
            if(!material){return material;}
            material->SetVectorParameterValue(TEXT("Color"),color);
            material->SetScalarParameterValue(TEXT("Roughness"),roughness);
            return material;
        };
        auto* stone=finish({0.71f,0.57f,0.39f,1},0.85f);
        auto* pavement=finish({0.38f,0.43f,0.46f,1},0.82f);
        auto* terminal=finish({0.14f,0.29f,0.36f,1},0.34f);
        auto* labMaterial=finish({0.26f,0.15f,0.37f,1},0.55f);
        auto* arenaMaterial=finish({0.38f,0.16f,0.29f,1},0.61f);
        auto* pubMaterial=finish({0.39f,0.21f,0.095f,1},0.75f);
        auto* gamesMaterial=finish({0.08f,0.44f,0.39f,1},0.47f);
        auto* rail=finish({0.24f,0.26f,0.29f,1},0.63f);
        auto* foliage=finish({0.11f,0.32f,0.14f,1},0.88f);
        auto* glass=finish({0.12f,0.33f,0.48f,1},0.22f);
        auto* silver=finish({0.39f,0.47f,0.55f,1},0.42f);
        auto* stoneTrim=finish({0.83f,0.72f,0.53f,1},0.80f);
        auto* tile=finish({0.46f,0.51f,0.54f,1},0.80f);
        auto* tealTrim=finish({0.24f,0.67f,0.64f,1},0.40f);
        auto* labTrim=finish({0.52f,0.32f,0.68f,1},0.54f);
        auto* arenaTrim=finish({0.59f,0.33f,0.43f,1},0.62f);
        auto* pubTrim=finish({0.70f,0.47f,0.22f,1},0.66f);
        auto* darkFrame=finish({0.115f,0.14f,0.19f,1},0.69f);
        UMaterialInterface* current=pavement;
        const auto volume=[&](UStaticMesh* mesh,FVector center,FVector scale,FRotator rotation=FRotator::ZeroRotator){
            auto* actor=GetWorld()->SpawnActor<AStaticMeshActor>(PlazaOrigin+center,rotation);
            if(!actor){return;}
            auto* component=actor->GetStaticMeshComponent();
            component->SetMobility(EComponentMobility::Movable);
            component->SetStaticMesh(mesh);
            if(current){component->SetMaterial(0,current);}
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
        current=terminal;
        volume(cube,{1080,0,230},{3,12,4.6});
        volume(cube,{890,0,460},{1.8,13,0.35});
        for(double y:{-430.,-220.,220.,430.}){
            volume(cube,{900,y,225},{0.45,0.45,4.5});
        }
        current=stone;
        volume(cylinder,{-425,-345,35},{2.6,2.6,0.7});
        volume(cylinder,{-425,-345,94},{0.75,0.75,1.18});
        volume(cylinder,{-425,-345,160},{1.5,1.5,0.22});
        volume(cube,{-170,-330,35},{0.6,2,0.65});
        const auto facade=[&](UStaticMesh* mesh,FVector origin,float yaw,FVector local,FVector scale){
            const FRotator rotation(0,yaw,0);
            volume(mesh,origin+rotation.RotateVector(local),scale,rotation);
        };
        const FVector library(1120,-995,0),lab(680,-1230,0),arena(835,935,0);
        facade(cube,library,-35,{0,0,240},{3.5,5.8,4.8});
        facade(cube,library,-35,{-110,0,492},{2.8,6.4,0.38});
        for(double y:{-180.,0.,180.}){
            facade(cylinder,library,-35,{-185,y,213},{0.44,0.44,4.2});
        }
        current=labMaterial;
        facade(cube,lab,-48,{0,0,140},{2.4,4.2,2.8});
        facade(cube,lab,-48,{-121,0,294},{2.8,4.8,0.28});
        current=arenaMaterial;
        facade(cube,arena,42,{0,0,135},{3.5,5.3,2.7});
        facade(cube,arena,42,{-100,0,267},{2.4,5.7,0.18});
        current=gamesMaterial;
        volume(cylinder,{205,105,100},{3.2,4.3,2.0});
        volume(cylinder,{205,105,204},{3.7,4.7,0.3});
        current=pubMaterial;
        volume(cube,{-260,1150,-55},{3.0,3.8,1.9});
        volume(cube,{-290,923,31},{2.6,0.7,0.18});

        // Surface panels give each destination a readable finish from both camera views.
        current=glass;
        for(double y:{-440.,-220.,0.,220.,440.}){
            volume(cube,{924,y,315},{0.10,1.9,2.38});
        }
        current=silver;
        volume(cube,{914,0,194},{0.22,11.0,0.16});
        volume(cube,{914,0,437},{0.22,11.0,0.13});
        for(double y:{-550.,-330.,-110.,110.,330.,550.}){
            volume(cube,{916,y,314},{0.16,0.10,2.39});
        }
        current=darkFrame;
        volume(cube,{923,0,82},{0.16,1.34,1.62});

        current=stoneTrim;
        facade(cube,library,-35,{-176,0,404},{0.18,5.65,0.20});
        facade(cube,library,-35,{-176,0,42},{0.18,5.65,0.18});
        for(double y:{-180.,0.,180.}){
            facade(cylinder,library,-35,{-185,y,412},{0.57,0.57,0.17});
            facade(cylinder,library,-35,{-185,y,16},{0.57,0.57,0.23});
        }
        current=darkFrame;
        facade(cube,library,-35,{-177,0,122},{0.12,1.25,2.02});

        current=labTrim;
        facade(cube,lab,-48,{-125,0,233},{0.14,3.85,0.19});
        facade(cube,lab,-48,{-124,-170,137},{0.12,0.14,1.65});
        facade(cube,lab,-48,{-124,170,137},{0.12,0.14,1.65});
        current=glass;
        facade(cube,lab,-48,{-125,0,134},{0.14,2.65,1.35});

        current=darkFrame;
        facade(cube,arena,42,{-178,0,124},{0.12,4.65,1.8});
        current=arenaTrim;
        for(double y:{-220.,-110.,0.,110.,220.}){
            facade(cube,arena,42,{-186,y,126},{0.10,0.12,1.94});
        }
        facade(cube,arena,42,{-187,0,222},{0.09,4.65,0.13});

        current=tealTrim;
        volume(cylinder,{205,105,181},{3.24,4.34,0.10});
        volume(cylinder,{205,105,37},{3.24,4.34,0.15});
        current=darkFrame;
        volume(cube,{43,105,109},{0.12,1.6,1.22});

        current=pubTrim;
        volume(cube,{-417,1150,-7},{0.16,3.46,0.18});
        volume(cube,{-416,1035,-59},{0.12,0.84,0.66});
        volume(cube,{-416,1265,-59},{0.12,0.84,0.66});
        current=darkFrame;
        volume(cube,{-420,1150,-70},{0.16,0.78,1.22});

        current=tile;
        for(int row=0;row<3;++row){
            for(int col=0;col<5;++col){
                volume(cube,{-575+row*75.0,-495+col*75.0,0},{0.69,0.69,0.08});
            }
        }
        current=stoneTrim;
        volume(cylinder,{-425,-345,68},{2.62,2.62,0.09});
        volume(cylinder,{-425,-345,172},{1.52,1.52,0.08});

        // Track turns out of the Terminal and advances toward the near right edge.
        current=rail;
        const FVector track[]={{930,365,378},{865,610,369},{655,835,359},
            {355,1045,350},{30,1230,342},{-410,1430,334}};
        for(int i=1;i<UE_ARRAY_COUNT(track);++i){
            const FVector start=track[i-1],end=track[i];
            const FVector delta=end-start;
            const FVector midpoint=(start+end)*0.5;
            const FVector lateral=FVector::CrossProduct(FVector::UpVector,delta).GetSafeNormal();
            const FRotator heading=delta.Rotation();
            const double length=(delta.Size()+15.0)/100.0;
            volume(cube,midpoint,{length,1.78,0.56},heading);
            for(double offset:{-41.,41.}){
                current=silver;
                volume(cube,midpoint+lateral*offset+FVector(0,0,33),{length,0.12,0.10},heading);
                current=rail;
            }
            if(i>=2){
                const double base=midpoint.X<100 && midpoint.Y>630?-150.0:0.0;
                volume(cube,{midpoint.X,midpoint.Y,(midpoint.Z+base)*0.5},
                    {0.5,0.48,(midpoint.Z-base)/100.0});
            }
        }
        // Lower Terminal annex feeds a separate monorail beam on the left.
        current=terminal;
        volume(cube,{1090,-700,157},{2.8,3.4,3.14});
        volume(cube,{1080,-714,318},{3.0,3.6,0.2});
        current=rail;
        const FVector monorail[]={{1075,-835,316},{1040,-1190,306},{850,-1510,296},{500,-1890,290}};
        for(int i=1;i<UE_ARRAY_COUNT(monorail);++i){
            const FVector delta=monorail[i]-monorail[i-1];
            const FVector midpoint=(monorail[i]+monorail[i-1])*0.5;
            volume(cube,midpoint,{(delta.Size()+14)/100,0.86,0.48},delta.Rotation());
            if(i==2){
                volume(cube,{midpoint.X,midpoint.Y,midpoint.Z*0.5},{0.42,0.42,midpoint.Z/100});
            }
        }
        // Skyline blocks and planters provide depth references during the camera move.
        current=pavement;
        for(int i=0;i<7;++i){
            const double height=5.0+(i%3)*1.4;
            volume(cube,{1970.0+(i%2)*140,-1500.0+i*500,height*50},{2.8,3.1,height});
        }
        current=foliage;
        for(double y:{-660.,580.}){
            volume(cylinder,{145,y,32},{1.2,1.2,0.64});
            volume(cylinder,{145,y,108},{0.21,0.21,1.5});
            volume(cylinder,{145,y,205},{1.3,1.3,0.85});
        }
        volume(cube,{1600,980,360},{4,5,7.2});
        for(const auto& signal:AmbientSignals){
            auto* material=finish(signal.color,0.58f);
            Hub3DSignals.Add(material);
            if(material){
                current=material;
                volume(cube,signal.position,{0.13,0.22,0.14});
            }
        }
        if(auto* light=GetWorld()->SpawnActor<APointLight>(PlazaOrigin+FVector(100,-450,800),FRotator::ZeroRotator)){
            if(auto* component=light->FindComponentByClass<UPointLightComponent>()){
                component->SetMobility(EComponentMobility::Movable);
                component->SetIntensity(65000.f);
                component->SetAttenuationRadius(3800.f);
            }
        }
    }
    if(!Hub3DCamera){return false;}
    auto* camera=Hub3DCamera->GetCameraComponent();
    camera->SetAspectRatio(9.f/16.f);
    camera->SetConstraintAspectRatio(focused);
    camera->SetFieldOfView(focused?40.f:55.f);
    if(PlayerOwner->GetViewTarget()!=Hub3DCamera){
        Hub3DOriginalCamera=PlayerOwner->GetViewTarget();
        PlayerOwner->SetViewTarget(Hub3DCamera);
    }
    using namespace rpg::presentation;
    const bool animateAmbient=!HubReducedMotion && Flow.screen()==UiScreen::Hub;
    const double ambientTime=GetWorld()->GetTimeSeconds();
    static const FName SignalColor(TEXT("Color"));
    for(int32 index=0;index<Hub3DSignals.Num();++index){
        if(auto* material=Hub3DSignals[index].Get()){
            const auto& signal=AmbientSignals[index];
            const double wave=animateAmbient?FMath::Sin(ambientTime*2.0*UE_PI/signal.period+index*1.4):0.0;
            const float intensity=0.80f+0.20f*static_cast<float>(wave);
            material->SetVectorParameterValue(SignalColor,{
                signal.color.R*intensity,signal.color.G*intensity,signal.color.B*intensity,signal.color.A});
        }
    }
    const bool approaching=Flow.screen()==UiScreen::Location && HubPanelRoute==HubRoute::Terminal;
    const double target=approaching?1.0:0.0;
    Hub3DApproach=HubReducedMotion?target:FMath::FInterpConstantTo(Hub3DApproach,target,static_cast<double>(GetWorld()->GetDeltaSeconds()),1.35);
    const double blend=FMath::SmoothStep(0.0,1.0,Hub3DApproach);
    const auto selectedSide=[&](){return HubArea==HubArea::Left?-1.0:HubArea==HubArea::Right?1.0:0.0;};
    const double now=GetWorld()->GetTimeSeconds();
    constexpr double slideDuration=0.45;
    const double width=FMath::Min(static_cast<double>(Canvas->SizeX),Canvas->SizeY*9.0/16.0);
    const double height=width*16.0/9.0;
    const Rect portrait{(Canvas->SizeX-width)*0.5,(Canvas->SizeY-height)*0.5,width,height};
    if(!focused || HubReducedMotion || Flow.screen()!=UiScreen::Hub || width<=0){
        Hub3DTrackingDrag=false;
        Hub3DDragging=false;
    }else if(Input.pressed && contains(portrait,Input.x,Input.y)){
        Hub3DTrackingDrag=true;
        Hub3DDragging=false;
        Hub3DDragOrigin=Hub3DSide;
        Hub3DDragLastX=Input.x;
        Hub3DDragLastTime=now;
        Hub3DDragVelocity=0;
    }
    if(Hub3DTrackingDrag){
        const double dx=Input.x-Input.origin_x;
        const double dy=Input.y-Input.origin_y;
        if((Input.down || Input.released) && !Input.cancelled){
            if(FMath::Abs(dx)>=12 && FMath::Abs(dx)>1.25*FMath::Abs(dy)){
                Hub3DDragging=true;
            }
            if(now>Hub3DDragLastTime && (!Input.released || Input.x!=Hub3DDragLastX)){
                Hub3DDragVelocity=(Input.x-Hub3DDragLastX)/(now-Hub3DDragLastTime);
                Hub3DDragLastX=Input.x;
                Hub3DDragLastTime=now;
            }
            if(Hub3DDragging){
                Hub3DSide=FMath::Clamp(Hub3DDragOrigin-dx/width,-1.0,1.0);
            }
        }
        if(Input.released || Input.cancelled || !Input.down){
            if(Hub3DDragging){
                const double velocity=now-Hub3DDragLastTime<=0.10?Hub3DDragVelocity:0;
                const bool advance=FMath::Abs(dx)>=width*0.18
                    || (FMath::Abs(dx)>=24 && FMath::Abs(velocity)>=550 && velocity*dx>0);
                if(Input.released && !Input.cancelled && advance){
                    const double next=FMath::Clamp(selectedSide()+(dx<0?1.0:-1.0),-1.0,1.0);
                    HubArea=next<0?HubArea::Left:next>0?HubArea::Right:HubArea::Center;
                    HubView=HubView::Focused;
                    SavePresentationPreferences();
                }
                Hub3DSideFrom=Hub3DSide;
                Hub3DSideTarget=selectedSide();
                Hub3DSlideStart=now;
                Input.tap=false;
                Pointer.cancel_until_release();
            }
            Hub3DTrackingDrag=false;
            Hub3DDragging=false;
        }
    }
    const double sideTarget=focused?selectedSide():0.0;
    if(HubReducedMotion){
        Hub3DSide=Hub3DSideFrom=Hub3DSideTarget=sideTarget;
    }else if(!Hub3DDragging){
        if(sideTarget!=Hub3DSideTarget){
            Hub3DSideFrom=Hub3DSide;
            Hub3DSideTarget=sideTarget;
            Hub3DSlideStart=now;
        }
        const double progress=FMath::SmoothStep(0.0,1.0,(now-Hub3DSlideStart)/slideDuration);
        Hub3DSide=FMath::Lerp(Hub3DSideFrom,Hub3DSideTarget,progress);
    }
    const bool sliding=Hub3DDragging || !FMath::IsNearlyEqual(Hub3DSide,sideTarget,0.001);
    Hub3DLabelOpacity=HubReducedMotion?1.f:FMath::FInterpConstantTo(
        Hub3DLabelOpacity,sliding?0.f:1.f,GetWorld()->GetDeltaSeconds(),8.f);
    const double terminalSide=TerminalSector==HubArea::Left?-1.0:TerminalSector==HubArea::Right?1.0:0.0;
    TerminalCameraSide=HubReducedMotion?terminalSide:FMath::FInterpTo(
        TerminalCameraSide,terminalSide,static_cast<double>(GetWorld()->GetDeltaSeconds()),6.0);
    const FVector eye=FMath::Lerp(FVector(focused?-1160:-1360,Hub3DSide*470,715),FVector(390,-82+TerminalCameraSide*125,318),blend);
    const FVector look=FMath::Lerp(FVector(660,Hub3DSide*850,focused?250:130),FVector(960,TerminalCameraSide*180,275),blend);
    Hub3DCamera->SetActorLocationAndRotation(PlazaOrigin+eye,(look-eye).Rotation());
    return true;
}

void AProbeArenaHUD::RestoreHub3DCamera()
{
    HideTerminalWorldDock();
    if(Hub3DCamera && PlayerOwner->GetViewTarget()==Hub3DCamera && IsValid(Hub3DOriginalCamera)){
        PlayerOwner->SetViewTarget(Hub3DOriginalCamera);
        Hub3DApproach=0;
        Hub3DTrackingDrag=Hub3DDragging=false;
    }
}

const rpg::presentation::HubHotspot* AProbeArenaHUD::DrawHub3DScene(const rpg::presentation::Rect& scene)
{
    using namespace rpg::presentation;
    if(Flow.screen()!=UiScreen::Hub || Hub3DApproach>0.01 || Hub3DLabelOpacity<=0.f){return nullptr;}
    const bool interactive=Hub3DLabelOpacity>=1.f && !Hub3DDragging;
    for(std::size_t index=0;index<HubLinks.size();++index){
        const auto& link=HubLinks[index];
        const DestinationAnchor* anchor=nullptr;
        for(const auto& candidate:DestinationAnchors){
            if(candidate.id==link.id){anchor=&candidate;break;}
        }
        if(!anchor){continue;}
        FVector2D projected;
        if(!PlayerOwner->ProjectWorldLocationToScreen(PlazaOrigin+anchor->point,projected)){continue;}
        const auto point=viewport_to_canvas(projected.X,projected.Y,CanvasCoordinates);
        if(!point || !contains(scene,point->x,point->y)){continue;}
        const auto upper=hub_uppercase(link.label);
        const FString label=UTF8_TO_TCHAR(upper.c_str());
        float width=0,height=0;
        GetTextSize(label,width,height,HubLabelFont?HubLabelFont.Get():HubTitleFont.Get());
        const auto target=minimum_target({point->x-width/2-12,point->y-24,width+24,48},scene);
        if(!target){continue;}
        const bool hover=interactive && InputWindowActive && contains(*target,Input.x,Input.y);
        DrawAnimatedHubLabel(link,index,label,point->x-width/2,point->y-height/2,1.f,width,height,hover,Hub3DLabelOpacity);
        if(interactive && pointer_clicked(Input,*target)){return &link;}
    }
    return nullptr;
}
