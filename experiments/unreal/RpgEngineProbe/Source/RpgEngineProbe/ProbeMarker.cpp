#include "ProbeArena.h"
#include "rpg/arena_layout.hpp"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"
void AProbeArenaHUD::HideTargetMarker(){if(TargetMarkerActor){TargetMarkerActor->SetActorHiddenInGame(true);}}
void AProbeArenaHUD::DestroyTargetMarker(){if(TargetMarkerActor){TargetMarkerActor->Destroy();}TargetMarkerActor=nullptr;TargetMarkerSegments=0;TargetMarkerRadius=0;}
void AProbeArenaHUD::UpdateTargetMarker(FVector base,float radius,int32 segments){
    if(!TargetMarkerMesh || !GetWorld()){return;}
    if(base.ContainsNaN() || !FMath::IsFinite(radius) || radius<=0 || radius>10000 || segments<3 || segments>64){HideTargetMarker();return;}
    if(!TargetMarkerActor || TargetMarkerSegments!=segments || !FMath::IsNearlyEqual(TargetMarkerRadius,radius)){
        DestroyTargetMarker();TargetMarkerActor=GetWorld()->SpawnActor<AActor>();if(!TargetMarkerActor){return;}
        TargetMarkerActor->SetOwner(this);
        auto* root=NewObject<USceneComponent>(TargetMarkerActor);root->SetMobility(EComponentMobility::Movable);TargetMarkerActor->SetRootComponent(root);root->RegisterComponent();
        auto* material=TargetMarkerMaterial?UMaterialInstanceDynamic::Create(TargetMarkerMaterial,TargetMarkerActor):nullptr;
        if(material){material->SetVectorParameterValue(TEXT("Color"),FLinearColor(2.0f,1.0f,0.05f));}
        for(const auto& edge:rpg::presentation::marker_edges(radius,static_cast<uint32>(segments))){
            auto* mesh=NewObject<UStaticMeshComponent>(TargetMarkerActor);mesh->SetupAttachment(root);mesh->SetMobility(EComponentMobility::Movable);mesh->SetStaticMesh(TargetMarkerMesh);if(material){mesh->SetMaterial(0,material);}
            mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);mesh->SetCastShadow(false);
            mesh->SetRelativeLocation(FVector(edge.x,edge.y,0));mesh->SetRelativeRotation(FRotator(0,edge.yaw,0));mesh->SetRelativeScale3D(FVector(edge.length/100.f,0.08f,0.025f));mesh->RegisterComponent();
        }
        TargetMarkerSegments=segments;TargetMarkerRadius=radius;
    }
    TargetMarkerActor->SetActorLocation(base);TargetMarkerActor->SetActorHiddenInGame(false);
}
