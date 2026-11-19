# Tests startup presentation contracts only; no Unreal, network, game or installer execution.
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$build=Join-Path $root 'build/startup-presentation'
$vswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$vs=@(& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -format json -utf8 | ConvertFrom-Json)
if($vs.Count -ne 1){throw 'Installed C++ tools required; no installation is performed'}
$devcmd=Join-Path $vs[0].installationPath 'Common7/Tools/VsDevCmd.bat'
foreach($p in @($root,$devcmd)){if($p -match '[%"\r\n&|<>^!]'){throw 'Unsupported batch path'}}
New-Item -ItemType Directory -Path $build -Force | Out-Null
$batch=@"
@echo off
setlocal
set "VSCMD_SKIP_SENDTELEMETRY=1"
call "$devcmd" -no_logo -arch=x64 -host_arch=x64
if errorlevel 1 exit /b 10
set "FLAGS=/nologo /std:c++20 /permissive- /Zc:__cplusplus /EHsc /W4 /WX /utf-8 /Od /MT"
cl.exe %FLAGS% /I"$root/include" /c "$root/src/startup_presentation.cpp" /Fo"$build/startup_presentation.obj"
if errorlevel 1 exit /b 11
cl.exe %FLAGS% /I"$root/include" "$root/tests/startup_presentation_tests.cpp" "$build/startup_presentation.obj" /Fo"$build/tests.obj" /Fe"$build/tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 12
"$build/tests.exe"
exit /b %ERRORLEVEL%
"@
$batchPath=Join-Path $build 'testar.cmd';$batch | Set-Content $batchPath -Encoding ascii
$output=@(& $env:ComSpec /d /c $batchPath 2>&1 | ForEach-Object {$_.ToString()});$code=$LASTEXITCODE
$output | Set-Content (Join-Path $build 'resultado.log') -Encoding utf8
$passed=$code -eq 0 -and $output -contains 'STARTUP_PRESENTATION_TESTS_RESULT passed=10 total=10'
[ordered]@{CheckedUtc=[DateTime]::UtcNow.ToString('o');Passed=$passed;ExitCode=$code;Groups=10;Scope='Native hub image crop, video contain/orientation request and local metadata; no media playback/window/device execution';EngineExecuted=$false;DeviceRotationExecuted=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/HUB-01A.json') -Encoding utf8
$output | ForEach-Object {Write-Output $_}
if(!$passed){throw 'Startup presentation tests failed'}
