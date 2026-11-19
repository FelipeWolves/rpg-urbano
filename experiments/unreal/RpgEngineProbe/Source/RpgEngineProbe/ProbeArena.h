#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameFramework/GameModeBase.h"
#include "rpg/visual_session.hpp"
#include "ProbeArena.generated.h"
UCLASS()
class RPGENGINEPROBE_API AProbeArenaHUD : public AHUD
{
    GENERATED_BODY()
public:
    virtual void DrawHUD() override;
private:
    rpg::presentation::VisualSession Session;
    int32 SelectedSlot=0;
    uint64 SelectionRevision=0;
    double NextAutomatic=0;
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
