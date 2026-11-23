# Preparation only: never invokes UAT, UBT, editor, compiler or packaged game.
param([Parameter(Mandatory=$true)][string]$EngineRoot)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$engine=[IO.Path]::GetFullPath($EngineRoot)
$version=Get-Content -LiteralPath (Join-Path $engine 'Engine/Build/Build.version') -Raw | ConvertFrom-Json
if($version.MajorVersion -ne 5 -or $version.MinorVersion -ne 8 -or $version.PatchVersion -ne 4 -or $version.Changelist -ne 58902052){throw 'Preparation targets inspected UE5.8.4 CL58902052 only'}
$dotnet=Join-Path $engine 'Engine/Binaries/ThirdParty/DotNet/10.0/win-x64/dotnet.exe'
$uat=Join-Path $engine 'Engine/Binaries/DotNET/AutomationTool/AutomationTool.dll'
foreach($component in @($dotnet,$uat,(Join-Path $engine 'Engine/Binaries/Win64/UnrealEditor-Cmd.exe'),(Join-Path $engine 'Engine/Content/Maps/Templates/Template_Default.umap'))){if(!(Test-Path -LiteralPath $component -PathType Leaf)){throw "Missing installed component: $component"}}
$packageRoot=Join-Path $root 'build/p'
$run=Join-Path $packageRoot ([Guid]::NewGuid().ToString('N').Substring(0,8))
$stage=Join-Path $run 'RpgEngineProbe'
$boundary=[IO.Path]::GetFullPath($packageRoot)+[IO.Path]::DirectorySeparatorChar
if(![IO.Path]::GetFullPath($stage).StartsWith($boundary,[StringComparison]::OrdinalIgnoreCase)){throw 'Invalid package staging boundary'}
$files=@('RpgEngineProbe.uproject','Source/RpgEngineProbeEditor.Target.cs','Source/RpgEngineProbe.Target.cs','Source/RpgEngineProbe/RpgEngineProbe.Build.cs','Source/RpgEngineProbe/RpgEngineProbe.cpp','Source/RpgEngineProbe/ProbeArena.h','Source/RpgEngineProbe/ProbeArena.cpp','Source/RpgEngineProbe/ProbeHub.cpp','Source/RpgEngineProbe/ProbeTutorial.cpp','Source/RpgEngineProbe/ProbeMedia.cpp','Source/RpgEngineProbe/ProbeMarker.cpp','Source/RpgEngineProbe/CoreProbe.h','Source/RpgEngineProbe/CoreProbe.cpp','Source/RpgEngineProbe/RpgCoreProbeCommandlet.h','Source/RpgEngineProbe/RpgCoreProbeCommandlet.cpp','Config/DefaultEngine.ini','Config/DefaultEditorSettings.ini','Config/DefaultGame.ini','Content/PrototypeHub.umap')
$manifest=@()
foreach($file in $files){$manifest+=@{Original=('experiments/unreal/RpgEngineProbe/'+$file);StagedRelative=$file}}
foreach($name in @('battle','ability_profile','local_session','team_selection','save_game','reference_encounter','visual_session','arena_layout','battle_presentation','startup_presentation','hub_style','stage','sequential','shared_boss','pve','pve_visual','tutorial')){
    $manifest+=@{Original="src/$name.cpp";StagedRelative="Source/RpgEngineProbe/Private/NativeCore/$name.cpp"}
    $manifest+=@{Original="include/rpg/$name.hpp";StagedRelative="Source/RpgEngineProbe/Public/NativeCore/rpg/$name.hpp"}
}
$manifest+=@{Original='include/rpg/persistence.hpp';StagedRelative='Source/RpgEngineProbe/Public/NativeCore/rpg/persistence.hpp'}
$manifest+=@{Original='include/rpg/damage_pipeline.hpp';StagedRelative='Source/RpgEngineProbe/Public/NativeCore/rpg/damage_pipeline.hpp'}
$manifest+=@{Original='include/rpg/hit_resolution.hpp';StagedRelative='Source/RpgEngineProbe/Public/NativeCore/rpg/hit_resolution.hpp'}
$manifest+=@{Original='include/rpg/status_operations.hpp';StagedRelative='Source/RpgEngineProbe/Public/NativeCore/rpg/status_operations.hpp'}
$manifest+=@{Original='include/rpg/score.hpp';StagedRelative='Source/RpgEngineProbe/Public/NativeCore/rpg/score.hpp'}
$manifest+=@{Original='include/rpg/numeric.hpp';StagedRelative='Source/RpgEngineProbe/Public/NativeCore/rpg/numeric.hpp'}
$manifest+=@{Original='include/rpg/visual_events.hpp';StagedRelative='Source/RpgEngineProbe/Public/NativeCore/rpg/visual_events.hpp'}
$manifest+=@{Original='include/rpg/encounter_manifest.hpp';StagedRelative='Source/RpgEngineProbe/Public/NativeCore/rpg/encounter_manifest.hpp'}
foreach($resource in @('data/encounters/team-reference-v1.rpgmanifest','data/encounters/classic-reference-v1.rpgmanifest')){$manifest+=@{Original=$resource;StagedRelative=('Content/Presentation/'+$resource)}}
$ui=Get-Content -LiteralPath (Join-Path $root 'data/presentation/ui-runtime.json') -Raw | ConvertFrom-Json
$manifest+=@{Original='data/presentation/ui-runtime.json';StagedRelative='Content/Presentation/data/presentation/ui-runtime.json'}
foreach($asset in $ui.Resources){if(!$asset.Path.StartsWith('assets/runtime/ui/') -or $asset.Path.Split('/') -contains '..'){throw 'Invalid UI path'};if((Get-FileHash -LiteralPath (Join-Path $root $asset.Path)).Hash -ne $asset.SHA256){throw 'UI hash mismatch'};$manifest+=@{Original=$asset.Path;StagedRelative=('Content/Presentation/'+$asset.Path)}}
# Local hub resources and the two authored intro variants; editable originals remain excluded.
& (Join-Path $PSScriptRoot 'validar-identidade-hub.ps1') | Out-Null
$hubCatalog=Get-Content -LiteralPath (Join-Path $root 'data/presentation/startup.json') -Raw | ConvertFrom-Json
$hubAssets=@($hubCatalog.assets | Where-Object role -eq 'HubPanorama')
if($hubAssets.Count -ne 1 -or $hubAssets[0].status -ne 'Ready'){throw 'One ready local hub panorama required'}
$hubPath=[string]$hubAssets[0].path
if(!$hubPath.StartsWith('assets/runtime/hub/') -or $hubPath -match '[\\:"?*<>|#]' -or $hubPath.Split('/') -contains '..'){throw 'Unsafe hub asset path'}
$assetRoot=[IO.Path]::GetFullPath((Join-Path $root 'assets/runtime/hub'))+[IO.Path]::DirectorySeparatorChar
if(![IO.Path]::GetFullPath((Join-Path $root $hubPath)).StartsWith($assetRoot,[StringComparison]::OrdinalIgnoreCase)){throw 'Hub asset outside local hub folder'}
$manifest+=@{Original='data/presentation/startup.json';StagedRelative='Content/Presentation/data/presentation/startup.json'}
$manifest+=@{Original=$hubPath;StagedRelative=('Content/Presentation/'+$hubPath)}
foreach($resource in @('assets/runtime/hub/logo-lobo.png','assets/runtime/fonts/CormorantGaramond.ttf','assets/runtime/fonts/CormorantGaramond-OFL.txt','assets/runtime/fonts/Lora.ttf','assets/runtime/fonts/Lora-OFL.txt')){
    $manifest+=@{Original=$resource;StagedRelative=('Content/Presentation/'+$resource)}
}
foreach($resource in @('assets/runtime/video/intro-desk.mp4','assets/runtime/video/intro-mobile.mp4')){$manifest+=@{Original=$resource;StagedRelative=('Content/Presentation/'+$resource)}}
# Validate all inputs before staging; no generated source is treated as canonical.
foreach($item in $manifest){if(!(Test-Path -LiteralPath (Join-Path $root $item.Original) -PathType Leaf)){throw 'Missing canonical input'}}
foreach($item in $manifest){
    $from=Join-Path $root $item.Original;$to=Join-Path $stage $item.StagedRelative
    New-Item -ItemType Directory -Force -Path (Split-Path $to -Parent) | Out-Null
    Copy-Item -LiteralPath $from -Destination $to
    $item.SHA256=(Get-FileHash -LiteralPath $from -Algorithm SHA256).Hash
    if($item.SHA256 -ne (Get-FileHash -LiteralPath $to -Algorithm SHA256).Hash){throw 'Staged source hash mismatch'}
}
$project=Join-Path $stage 'RpgEngineProbe.uproject'
$archive=Join-Path $run 'archive'
$arguments=@($uat,('-ScriptsForProject='+$project),'BuildCookRun','-nocompile','-installed','-unattended','-nop4','-utf8output',('-project='+$project),'-target=RpgEngineProbe','-platform=Win64','-clientconfig=Development','-build','-cook','-stage','-pak','-archive',('-archivedirectory='+$archive),'-map=/Game/PrototypeHub','-ubtargs=-NoEngineChanges -NoHotReloadFromIDE -NoXGE -NoSNDBS -NoFASTBuild -NoUBA -MaxParallelActions=2','-AdditionalCookerOptions=-culture=en -ini:EditorSettings:[/Script/UnrealEd.AnalyticsPrivacySettings]:bSendUsageData=False')
$plan=[ordered]@{FileName=$dotnet;ArgumentList=$arguments;WorkingDirectory=(Join-Path $engine 'Engine');Environment=@{DOTNET_CLI_TELEMETRY_OPTOUT='1';DOTNET_SKIP_FIRST_TIME_EXPERIENCE='1'};ExecutionAuthorized=$false;Note='Plan only. Review/authorize toolchain, child processes, cache writes and package launch separately. No network isolation.'}
$planPath=Join-Path $run 'package-plan.json'
$plan | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath $planPath -Encoding utf8
[ordered]@{CheckedUtc=[DateTime]::UtcNow.ToString('o');Status='prepared_not_built';EngineVersion='5.8.4';EngineChangelist=$version.Changelist;Stage=$stage;Archive=$archive;Plan=$planPath;Target='RpgEngineProbe';Platform='Win64';Configuration='Development';Map='/Game/PrototypeHub';SourceManifest=$manifest;BuildExecuted=$false;CookExecuted=$false;EditorOpened=$false;PackageValidated=$false;NetworkChanged=$false;ProductPortugueseLocalizationValidated=$false} | ConvertTo-Json -Depth 7 | Set-Content -LiteralPath (Join-Path $root 'docs/relatorios/atuais/ENG-01-PACOTE-PREPARACAO.json') -Encoding utf8
Write-Output "PACKAGE_PREPARED_ONLY inputs=$($manifest.Count) plan=$planPath"
