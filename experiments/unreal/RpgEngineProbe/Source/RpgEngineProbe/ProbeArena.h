#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameFramework/GameModeBase.h"
#include "rpg/battle_presentation.hpp"
#include "ProbeArena.generated.h"
UCLASS()
class RPGENGINEPROBE_API AProbeArenaHUD : public AHUD
{
    GENERATED_BODY()
public:
    AProbeArenaHUD();
    virtual void DrawHUD() override;
    virtual void BeginPlay() override;
private:
    rpg::presentation::VisualSession Session;
    int32 SelectedSlot=0;
    uint64 SelectionRevision=0;
    double NextAutomatic=0;
    rpg::presentation::AbilityGesture Gesture;
    FString SelectedTarget;
    FString PopupTitle;
    TArray<FString> PopupLines;
    int32 PopupPage=0;
    void OpenInfo(const FString& title,const std::vector<std::string>& lines);
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
