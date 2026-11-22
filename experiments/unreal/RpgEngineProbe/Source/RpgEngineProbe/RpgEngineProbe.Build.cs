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
        // The stable editor project compiles canonical core sources in this module.
        // Isolated package stages instead carry a checked snapshot under NativeCore.
        string repository = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../../.."));
        if (Directory.Exists(Path.Combine(repository, "include", "rpg")) &&
            File.Exists(Path.Combine(repository, "src", "battle.cpp")))
        {
            PublicIncludePaths.Add(Path.Combine(repository, "include"));
            PrivateIncludePaths.Add(Path.Combine(repository, "src"));
            foreach (string file in Directory.GetFiles(Path.Combine(repository, "src"), "*.cpp"))
                ExternalDependencies.Add(file);
            foreach (string file in Directory.GetFiles(Path.Combine(repository, "include", "rpg"), "*.hpp"))
                ExternalDependencies.Add(file);
        }
        else
        {
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public", "NativeCore"));
        }
        PrivateDependencyModuleNames.AddRange(new string[] { "Json", "SlateCore", "MediaAssets", "AudioMixer" });
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
    }
}
