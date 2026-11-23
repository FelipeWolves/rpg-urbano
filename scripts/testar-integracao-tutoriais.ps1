# Shared-core native regression: compile once, run tutorial and related battle/session suites; no Unreal execution.
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$build=Join-Path $root 'build/tutorial-integration'
$vswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$vs=@(& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -format json -utf8 | ConvertFrom-Json)
if($vs.Count -ne 1){throw 'Existing C++ tools required; nothing is installed'}
$devcmd=Join-Path $vs[0].installationPath 'Common7/Tools/VsDevCmd.bat'
foreach($p in @($root,$devcmd)){if($p -match '[%"\r\n&|<>^!]'){throw 'Unsupported batch path'}}
New-Item -ItemType Directory -Force -Path $build | Out-Null
$batch=@"
@echo off
setlocal
set "VSCMD_SKIP_SENDTELEMETRY=1"
call "$devcmd" -no_logo -arch=x64 -host_arch=x64
if errorlevel 1 exit /b 10
set "FLAGS=/nologo /std:c++20 /permissive- /Zc:__cplusplus /EHsc /W4 /WX /utf-8 /Od /MT"
"@
$sources=@('battle','ability_profile','local_session','reference_encounter','visual_session','battle_presentation','arena_layout','startup_presentation','team_selection','save_game','stage','sequential','shared_boss','pve','pve_visual','tutorial')
foreach($name in $sources){$batch+="`ncl.exe %FLAGS% /I`"$root/include`" /c `"$root/src/$name.cpp`" /Fo`"$build/$name.obj`"`nif errorlevel 1 exit /b 11"}
$objects=($sources | ForEach-Object {'"'+$build+'/'+$_+'.obj"'}) -join ' '
$tests=@('damage_pipeline_tests','control_rules_tests','periodic_effect_tests','felipe_passive_tests','felipe_pilot_tests','wave_v2_tests','pve_phase_tests','shared_visual_tests','sequential_visual_tests','action_feedback_tests','classic_visual_tests','visual_save_tests','tutorial_tests')
foreach($name in $tests){$batch+="`ncl.exe %FLAGS% /I`"$root/include`" `"$root/tests/$name.cpp`" $objects /Fo`"$build/$name.obj`" /Fe`"$build/$name.exe`" /link /INCREMENTAL:NO`nif errorlevel 1 exit /b 12`n`"$build/$name.exe`"`nif errorlevel 1 exit /b 13"}
$batch+="`nexit /b 0`n"
$batchPath=Join-Path $build 'testar.cmd';$batch | Set-Content -LiteralPath $batchPath -Encoding ascii
$output=@(& $env:ComSpec /d /c $batchPath 2>&1 | ForEach-Object {$_.ToString()});$code=$LASTEXITCODE
$output | Set-Content (Join-Path $build 'resultado.log') -Encoding utf8
$markers=@($output | Where-Object {$_ -match '^\w+_TESTS_RESULT passed=\d+ total=\d+'})
$passed=$code -eq 0 -and $markers.Count -ge $tests.Count
foreach($marker in $markers){$m=[regex]::Match($marker,'passed=(\d+) total=(\d+)');if($m.Groups[1].Value -ne $m.Groups[2].Value){$passed=$false}}
$total=0;foreach($marker in $markers){$total += [int]([regex]::Match($marker,'passed=(\d+)').Groups[1].Value)}
[ordered]@{Date='2026-11-23';Passed=$passed;ExitCode=$code;Executables=$tests.Count;GroupsPassed=$total;Markers=$markers;EditorOpened=$false;AndroidExecuted=$false;Scope='Shared-core regression and two tutorial models; no graphical acceptance'} | ConvertTo-Json -Depth 6 | Set-Content (Join-Path $root 'docs/relatorios/atuais/TUTORIAL-INTEGRACAO-2026-11-23-TESTES.json') -Encoding utf8
$markers
if(!$passed){throw 'Tutorial integration regression failed'}
