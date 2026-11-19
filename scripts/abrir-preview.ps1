# Opens an already built local prototype. Never builds, downloads or modifies source files.
param([switch]$Editor,[switch]$ValidateOnly)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$report=Get-Content (Join-Path $root 'docs/relatorios/atuais/HUB-IDENTIDADE-BUILD.json') -Raw|ConvertFrom-Json
if(!$report.Passed){throw 'The current prototype build is not approved.'}
$stage=[IO.Path]::GetFullPath($report.Stage)
$boundary=[IO.Path]::GetFullPath((Join-Path $root 'build/p'))+[IO.Path]::DirectorySeparatorChar
if(!$stage.StartsWith($boundary,[StringComparison]::OrdinalIgnoreCase)){throw 'Build is outside this project.'}
$project=Join-Path $stage 'RpgEngineProbe.uproject'
$executable='D:\Unreal Engine\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe'
foreach($path in @($project,$executable)){if(!(Test-Path -LiteralPath $path -PathType Leaf)){throw ('Missing local file: '+$path)}}
$arguments=@($project,'/Engine/Maps/Templates/Template_Default','-Hub','-culture=en','-NoSplash','-windowed','-ResX=1280','-ResY=720','-ini:EditorSettings:[/Script/UnrealEd.AnalyticsPrivacySettings]:bSendUsageData=False')
if(!$Editor){$arguments+='-game'}
if($ValidateOnly){Write-Output ('PREVIEW_READY mode='+$(if($Editor){'Editor'}else{'Game'})+' project='+$project);return}
& $executable @arguments
