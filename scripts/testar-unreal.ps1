param([Parameter(Mandatory=$true)][string]$EngineRoot)
$ErrorActionPreference='Stop'
$projectRoot=Split-Path $PSScriptRoot -Parent
$engine=[IO.Path]::GetFullPath($EngineRoot)
$buildRoot=Join-Path $projectRoot 'build/engine-eval'
$stage=Join-Path $buildRoot 'RpgEngineProbe'
if(![IO.Path]::GetFullPath($stage).StartsWith([IO.Path]::GetFullPath($projectRoot)+[IO.Path]::DirectorySeparatorChar,[StringComparison]::OrdinalIgnoreCase)){throw 'Stage outside project'}
$version=Get-Content -LiteralPath (Join-Path $engine 'Engine/Build/Build.version') -Raw | ConvertFrom-Json
if($version.MajorVersion -ne 5 -or $version.MinorVersion -ne 8 -or $version.PatchVersion -ne 4){throw 'This probe targets the inspected UE5.8.4 build'}
$dotnet=Join-Path $engine 'Engine/Binaries/ThirdParty/DotNet/10.0/win-x64/dotnet.exe'
$ubt=Join-Path $engine 'Engine/Binaries/DotNET/UnrealBuildTool/UnrealBuildTool.dll'
foreach($file in @($dotnet,$ubt)){if(!(Test-Path -LiteralPath $file -PathType Leaf)){throw "Build component missing: $file"}}
$source=Join-Path $projectRoot 'experiments/unreal/RpgEngineProbe'
$files=@('RpgEngineProbe.uproject','Source/RpgEngineProbeEditor.Target.cs','Source/RpgEngineProbe/RpgEngineProbe.Build.cs','Source/RpgEngineProbe/RpgEngineProbe.cpp')
foreach($file in $files){$target=Join-Path $stage $file;New-Item -ItemType Directory -Force -Path (Split-Path $target -Parent) | Out-Null
    Copy-Item -LiteralPath (Join-Path $source $file) -Destination $target}
$envNames=@('DOTNET_CLI_TELEMETRY_OPTOUT','DOTNET_SKIP_FIRST_TIME_EXPERIENCE','DOTNET_NOLOGO','DOTNET_CLI_HOME','NUGET_PACKAGES','TEMP','TMP')
$old=@{};foreach($name in $envNames){$old[$name]=[Environment]::GetEnvironmentVariable($name,'Process')}
$messages=@();$code=-1
try{
    $env:DOTNET_CLI_TELEMETRY_OPTOUT='1';$env:DOTNET_SKIP_FIRST_TIME_EXPERIENCE='1';$env:DOTNET_NOLOGO='1'
    $env:DOTNET_CLI_HOME=Join-Path $buildRoot 'dotnet-home';$env:NUGET_PACKAGES=Join-Path $buildRoot 'nuget'
    $env:TEMP=Join-Path $buildRoot 'temp';$env:TMP=$env:TEMP
    New-Item -ItemType Directory -Force -Path $env:TEMP,$env:DOTNET_CLI_HOME | Out-Null
    $arguments=@($ubt,'RpgEngineProbeEditor','Win64','Development',('-Project='+(Join-Path $stage 'RpgEngineProbe.uproject')),
        '-NoEngineChanges','-NoHotReloadFromIDE','-NoXGE','-NoSNDBS','-NoFASTBuild','-NoUBA','-MaxParallelActions=2',('-Log='+(Join-Path $buildRoot 'ubt.log')))
    $messages=@(& $dotnet @arguments 2>&1 | ForEach-Object { $_.ToString();Write-Host $_ })
    $code=$LASTEXITCODE
}finally{foreach($name in $envNames){[Environment]::SetEnvironmentVariable($name,$old[$name],'Process')}}
$messages | Set-Content -LiteralPath (Join-Path $buildRoot 'console.log') -Encoding utf8
$binary=Join-Path $stage 'Binaries/Win64/UnrealEditor-RpgEngineProbe.dll'
$report=[ordered]@{CheckedUtc=[DateTime]::UtcNow.ToString('o');Scope='Compile/link own minimal Editor target only; no UnrealEditor execution, no own combat bridge yet';EngineVersion="5.8.$($version.PatchVersion)";EngineChangelist=$version.Changelist;ProjectTemplate='experiments/unreal/RpgEngineProbe';Stage=$stage;ExitCode=$code;ModuleBinaryPresent=Test-Path -LiteralPath $binary;ToolchainLines=@($messages | Where-Object {$_ -match 'toolchain|Windows.*SDK|preferred|compiler|Result:'});EditorOpened=$false;EngineChangesAllowed=$false;DistributedExecutorsAllowed=$false;MaxParallelActions=2}
$report | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath (Join-Path $projectRoot 'docs/relatorios/atuais/ENG-01-BUILD.json') -Encoding utf8
if($code -ne 0 -or !$report.ModuleBinaryPresent){throw "Engine probe build failed ($code); inspect build/engine-eval/console.log"}
Write-Output 'ENGINE_BUILD_OK target=RpgEngineProbeEditor platform=Win64 configuration=Development editor_opened=false'
