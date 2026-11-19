#pragma once
#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "RpgCoreProbeCommandlet.generated.h"
UCLASS()
class RPGENGINEPROBE_API URpgCoreProbeCommandlet : public UCommandlet
{
    GENERATED_BODY()
public:
    URpgCoreProbeCommandlet();
    virtual int32 Main(const FString& Params) override;
};
