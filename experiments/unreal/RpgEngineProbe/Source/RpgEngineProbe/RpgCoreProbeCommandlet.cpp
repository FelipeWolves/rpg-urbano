#include "RpgCoreProbeCommandlet.h"
#include "CoreProbe.h"
URpgCoreProbeCommandlet::URpgCoreProbeCommandlet()
{
    IsClient=false;
    IsServer=false;
    IsEditor=false;
    LogToConsole=true;
}
int32 URpgCoreProbeCommandlet::Main(const FString& Params)
{
    (void)Params;
    const auto result=rpg_probe::Run();
    UE_LOG(LogTemp, Display, TEXT("RPG_CORE_PROBE passed=%d checkpoints=%d failures=%d accepted=%d credits=%d"),
        result.passed?1:0,result.checkpoints,result.failures,result.accepted,result.credits);
    return result.passed?0:1;
}
