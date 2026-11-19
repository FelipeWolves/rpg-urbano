#include "Modules/ModuleManager.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "CoreProbe.h"
class FRpgEngineProbeModule : public FDefaultGameModuleImpl
{
public:
    virtual void StartupModule() override
    {
        FDefaultGameModuleImpl::StartupModule();
        if (FParse::Param(FCommandLine::Get(), TEXT("RpgCoreProbe")))
        {
            const auto result=rpg_probe::Run();
            UE_LOG(LogTemp, Display, TEXT("RPG_CORE_PROBE passed=%d checkpoints=%d failures=%d accepted=%d credits=%d"),
                result.passed?1:0,result.checkpoints,result.failures,result.accepted,result.credits);
        }
    }
};
IMPLEMENT_PRIMARY_GAME_MODULE(FRpgEngineProbeModule, RpgEngineProbe, "RpgEngineProbe");
