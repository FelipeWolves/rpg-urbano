using UnrealBuildTool;
using System.IO;
public class RpgEngineProbe : ModuleRules
{
    public RpgEngineProbe(ReadOnlyTargetRules target) : base(target)
    {
        PCHUsage = PCHUsageMode.NoPCHs;
        bUseUnity = false;
        bEnableExceptions = true;
        CppStandard = CppStandardVersion.Cpp20;
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public", "NativeCore"));
        PrivateDependencyModuleNames.Add("Json");
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
    }
}
