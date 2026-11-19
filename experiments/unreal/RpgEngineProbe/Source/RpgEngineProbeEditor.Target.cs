using UnrealBuildTool;
public class RpgEngineProbeEditorTarget : TargetRules
{
    public RpgEngineProbeEditorTarget(TargetInfo target) : base(target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
        ExtraModuleNames.Add("RpgEngineProbe");
    }
}
