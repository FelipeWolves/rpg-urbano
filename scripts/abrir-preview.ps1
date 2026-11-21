# Opens the already built stable project; never builds, installs or downloads.
param([switch]$Editor,[switch]$Mobile,[switch]$ValidateOnly)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$project=Join-Path $root 'experiments/unreal/RpgEngineProbe/RpgEngineProbe.uproject'
$module=Join-Path (Split-Path $project -Parent) 'Binaries/Win64/UnrealEditor-RpgEngineProbe.dll'
$executable='D:/Unreal Engine/UE_5.8/Engine/Binaries/Win64/UnrealEditor.exe'
foreach($file in @($project,$module,$executable)){if(!(Test-Path -LiteralPath $file -PathType Leaf)){throw "Missing local prerequisite: $file"}}
$arguments=@($project,'-culture=en','-NoSplash')
if(!$Editor){$arguments+=@('-game','-windowed')}
if($Mobile){$arguments+=@('-Presentation=Mobile','-ResX=450','-ResY=800')}
elseif(!$Editor){$arguments+=@('-ResX=1280','-ResY=720')}
if($ValidateOnly){Write-Output ('STABLE_PREVIEW_READY project='+$project+' mobile='+$Mobile);return}
& $executable @arguments
