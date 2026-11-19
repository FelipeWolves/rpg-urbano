using UnrealBuildTool;
public class RpgEngineProbeTarget : TargetRules
{
    public RpgEngineProbeTarget(TargetInfo target) : base(target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
        ExtraModuleNames.Add("RpgEngineProbe");
    }
}
