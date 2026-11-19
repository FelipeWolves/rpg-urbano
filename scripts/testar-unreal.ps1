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
$files=@('RpgEngineProbe.uproject','Source/RpgEngineProbeEditor.Target.cs','Source/RpgEngineProbe/RpgEngineProbe.Build.cs','Source/RpgEngineProbe/RpgEngineProbe.cpp','Source/RpgEngineProbe/CoreProbe.h','Source/RpgEngineProbe/CoreProbe.cpp','Source/RpgEngineProbe/RpgCoreProbeCommandlet.h','Source/RpgEngineProbe/RpgCoreProbeCommandlet.cpp')
foreach($file in $files){$target=Join-Path $stage $file;New-Item -ItemType Directory -Force -Path (Split-Path $target -Parent) | Out-Null
    Copy-Item -LiteralPath (Join-Path $source $file) -Destination $target}
$coreNames=@('battle','ability_profile','local_session','team_selection','save_game')
$manifest=@()
foreach($file in $files){$manifest += [ordered]@{Original=('experiments/unreal/RpgEngineProbe/'+$file);StagedRelative=$file}}
foreach($name in $coreNames){
    foreach($pair in @(@{From="src/$name.cpp";To="Source/RpgEngineProbe/Private/NativeCore/$name.cpp"},@{From="include/rpg/$name.hpp";To="Source/RpgEngineProbe/Public/NativeCore/rpg/$name.hpp"})){
        $target=Join-Path $stage $pair.To;New-Item -ItemType Directory -Force -Path (Split-Path $target -Parent) | Out-Null
        Copy-Item -LiteralPath (Join-Path $projectRoot $pair.From) -Destination $target
        $manifest += [ordered]@{Original=$pair.From;StagedRelative=$pair.To}
    }
}
foreach($item in $manifest){
    $hash=(Get-FileHash -LiteralPath (Join-Path $projectRoot $item.Original) -Algorithm SHA256).Hash
    $same=$hash -eq (Get-FileHash -LiteralPath (Join-Path $stage $item.StagedRelative) -Algorithm SHA256).Hash
    if(!$same){throw 'Staged core differs from canonical source'}
    $item.SHA256=$hash;$item.MatchesStage=$same
}
# Execute the exact bridge function outside Unreal first; no platform storage is linked.
$baseline=Join-Path $buildRoot 'baseline';New-Item -ItemType Directory -Force -Path $baseline | Out-Null
$vswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$instance=(& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -format json -utf8 | ConvertFrom-Json)[0]
$devcmd=Join-Path $instance.installationPath 'Common7/Tools/VsDevCmd.bat'
foreach($path in @($projectRoot,$devcmd)){if($path -match '[%"
&|<>^!]'){throw 'Unsupported batch path'}}
$probeModule=Join-Path $source 'Source/RpgEngineProbe'
$batch=@('@echo off','setlocal',('call "'+$devcmd+'" -no_logo -arch=x64 -host_arch=x64'),'if errorlevel 1 exit /b 10',
    'set "FLAGS=/nologo /std:c++20 /permissive- /Zc:__cplusplus /EHsc /W4 /WX /utf-8 /Od /MT"')
$objects=@()
foreach($name in $coreNames){
    $object=Join-Path $baseline ($name+'.obj');$objects+='"'+$object+'"'
    $batch+='cl.exe %FLAGS% /I"'+(Join-Path $projectRoot 'include')+'" /c "'+(Join-Path $projectRoot "src/$name.cpp")+'" /Fo"'+$object+'"'
    $batch+='if errorlevel 1 exit /b 11'
}
$bridgeObject=Join-Path $baseline 'CoreProbe.obj'
$batch+='cl.exe %FLAGS% /I"'+(Join-Path $projectRoot 'include')+'" /c "'+(Join-Path $probeModule 'CoreProbe.cpp')+'" /Fo"'+$bridgeObject+'"'
$batch+='if errorlevel 1 exit /b 12'
$batch+='cl.exe %FLAGS% /I"'+$probeModule+'" "'+(Join-Path $source 'CoreProbeStandalone.cpp')+'" "'+$bridgeObject+'" '+($objects -join ' ')+' /Fo"'+(Join-Path $baseline 'main.obj')+'" /Fe"'+(Join-Path $baseline 'core-probe.exe')+'" /link /INCREMENTAL:NO'
$batch+='if errorlevel 1 exit /b 13'
$batch+='"'+(Join-Path $baseline 'core-probe.exe')+'"'
$batch+='exit /b %ERRORLEVEL%'
$batchPath=Join-Path $baseline 'build.cmd';$batch | Set-Content -LiteralPath $batchPath -Encoding ascii
$baselineOutput=@(& $env:ComSpec /d /c $batchPath 2>&1 | ForEach-Object {$_.ToString()})
$baselineCode=$LASTEXITCODE
$baselineOutput | Set-Content -LiteralPath (Join-Path $baseline 'console.log') -Encoding utf8
$expected='RPG_CORE_PROBE passed=1 checkpoints=11 failures=0 accepted=1 credits=10'
if($baselineCode -ne 0 -or !($baselineOutput -contains $expected)){throw 'Native bridge baseline failed; inspect build/engine-eval/baseline/console.log'}
Write-Output $expected
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
$report=[ordered]@{CheckedUtc=[DateTime]::UtcNow.ToString('o');Scope='Compile/link native core and bridge in Unreal module; bridge executed only in standalone baseline, not inside Unreal';EngineVersion="5.8.$($version.PatchVersion)";EngineChangelist=$version.Changelist;ProjectTemplate='experiments/unreal/RpgEngineProbe';Stage=$stage;ExitCode=$code;ModuleBinaryPresent=Test-Path -LiteralPath $binary;ToolchainLines=@($messages | Where-Object {$_ -match 'toolchain|Windows.*SDK|preferred|compiler|Result:'});EditorOpened=$false;EngineChangesAllowed=$false;DistributedExecutorsAllowed=$false;MaxParallelActions=2;SourceManifest=$manifest;Baseline=@{ExitCode=$baselineCode;Result=$expected};RuntimeProbeExecutedInUnreal=$false}
$report | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath (Join-Path $projectRoot 'docs/relatorios/atuais/ENG-01-BUILD.json') -Encoding utf8
if($code -ne 0 -or !$report.ModuleBinaryPresent){throw "Engine probe build failed ($code); inspect build/engine-eval/console.log"}
Write-Output 'ENGINE_BUILD_OK target=RpgEngineProbeEditor platform=Win64 configuration=Development editor_opened=false'
