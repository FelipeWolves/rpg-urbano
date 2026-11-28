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
if errorlevel 1 exit /b 13
cl.exe %FLAGS% /I"$root/include" "$root/tests/hub_hotspot_tests.cpp" "$build/startup_presentation.obj" /Fo"$build/hub_tests.obj" /Fe"$build/hub_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 14
"$build/hub_tests.exe"
if errorlevel 1 exit /b 15
cl.exe %FLAGS% /I"$root/include" /c "$root/src/hub_style.cpp" /Fo"$build/hub_style.obj"
if errorlevel 1 exit /b 16
cl.exe %FLAGS% /I"$root/include" "$root/tests/hub_style_tests.cpp" "$build/hub_style.obj" /Fo"$build/hub_style_tests.obj" /Fe"$build/hub_style_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 17
"$build/hub_style_tests.exe"
if errorlevel 1 exit /b 18
cl.exe %FLAGS% /I"$root/include" "$root/tests/hub_expansion_tests.cpp" "$build/hub_style.obj" "$build/startup_presentation.obj" /Fo"$build/hub_expansion_tests.obj" /Fe"$build/hub_expansion_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 19
"$build/hub_expansion_tests.exe"
if errorlevel 1 exit /b 20
cl.exe %FLAGS% /I"$root/include" "$root/tests/hub_frame_tests.cpp" "$build/startup_presentation.obj" /Fo"$build/hub_frame_tests.obj" /Fe"$build/hub_frame_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 21
"$build/hub_frame_tests.exe"
if errorlevel 1 exit /b 22
cl.exe %FLAGS% /I"$root/include" "$root/tests/pointer_input_tests.cpp" "$build/startup_presentation.obj" /Fo"$build/pointer_input_tests.obj" /Fe"$build/pointer_input_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 23
"$build/pointer_input_tests.exe"
if errorlevel 1 exit /b 24
cl.exe %FLAGS% /I"$root/include" "$root/tests/text_panel_tests.cpp" "$build/startup_presentation.obj" /Fo"$build/text_panel_tests.obj" /Fe"$build/text_panel_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 25
"$build/text_panel_tests.exe"
if errorlevel 1 exit /b 26
cl.exe %FLAGS% /I"$root/include" "$root/tests/presentation_preferences_tests.cpp" "$build/startup_presentation.obj" /Fo"$build/presentation_preferences_tests.obj" /Fe"$build/presentation_preferences_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 27
"$build/presentation_preferences_tests.exe"
if errorlevel 1 exit /b 28
cl.exe %FLAGS% /I"$root/include" "$root/tests/canvas_coordinate_tests.cpp" "$build/startup_presentation.obj" /Fo"$build/canvas_coordinate_tests.obj" /Fe"$build/canvas_coordinate_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 29
"$build/canvas_coordinate_tests.exe"
if errorlevel 1 exit /b 30
cl.exe %FLAGS% /I"$root/include" "$root/tests/screen_flow_tests.cpp" "$build/startup_presentation.obj" /Fo"$build/screen_flow_tests.obj" /Fe"$build/screen_flow_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 31
"$build/screen_flow_tests.exe"
exit /b %ERRORLEVEL%
"@
$batch=$batch.Replace('exit /b %ERRORLEVEL%',@"
if errorlevel 1 exit /b 79
cl.exe %FLAGS% /I"$root/include" "$root/tests/interaction_routing_tests.cpp" "$build/startup_presentation.obj" /Fo"$build/interaction_routing_tests.obj" /Fe"$build/interaction_routing_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 82
"$build/interaction_routing_tests.exe"
if errorlevel 1 exit /b 83
cl.exe %FLAGS% /I"$root/include" "$root/tests/media_flow_tests.cpp" "$build/startup_presentation.obj" /Fo"$build/media_flow_tests.obj" /Fe"$build/media_flow_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 80
"$build/media_flow_tests.exe"
if errorlevel 1 exit /b 81
exit /b 0
"@)
$batchPath=Join-Path $build 'testar.cmd';$batch | Set-Content $batchPath -Encoding ascii
$output=@(& $env:ComSpec /d /c $batchPath 2>&1 | ForEach-Object {$_.ToString()});$code=$LASTEXITCODE
$output | Set-Content (Join-Path $build 'resultado.log') -Encoding utf8
if($code -ne 0){$output | Select-Object -Last 20 | Write-Output;throw "Presentation test executable/build failed: $code"}
$passed=$code -eq 0 -and $output -contains 'STARTUP_PRESENTATION_TESTS_RESULT passed=11 total=11'
[ordered]@{CheckedUtc=[DateTime]::UtcNow.ToString('o');Passed=$passed;ExitCode=$code;Groups=11;Scope='Native hub and media geometry, local metadata, Terminal catalog and return navigation; no media playback/window/device execution';EngineExecuted=$false;DeviceRotationExecuted=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/HUB-01A.json') -Encoding utf8
$hotspotsPassed=$code -eq 0 -and $output -contains 'HUB_HOTSPOT_TESTS_RESULT passed=5 total=5'
[ordered]@{CheckedUtc=[DateTime]::UtcNow.ToString('o');Passed=$hotspotsPassed;Groups=5;ExitCode=$code;Scope='Native hotspot projection/clipping/hit bounds; not Unreal UI acceptance';EngineExecuted=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/HUB-01B.json') -Encoding utf8
$stylePassed=$code -eq 0 -and $output -contains 'HUB_STYLE_TESTS_RESULT passed=5 total=5'
[ordered]@{CheckedUtc=[DateTime]::UtcNow.ToString('o');Passed=$stylePassed;Groups=5;ExitCode=$code;Scope='Native pulse timing, reduced motion and minimum target geometry only';EngineExecuted=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/HUB-01C.json') -Encoding utf8
if(!$stylePassed){throw 'Hub style tests failed'}
$output | ForEach-Object {Write-Output $_}
if(!$hotspotsPassed){throw 'Hub hotspot tests failed'}
if(!$passed){throw 'Startup presentation tests failed'}

$expansionPassed=$code -eq 0 -and $output -contains 'HUB_EXPANSION_TESTS_RESULT passed=6 total=6'
[ordered]@{CheckedUtc=[DateTime]::UtcNow.ToString('o');Passed=$expansionPassed;Groups=6;ExitCode=$code;Scope='Presentation policy override, destination routes and reduced label motion; no visual/mobile acceptance';EngineExecuted=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/HUB-EXPANSAO.json') -Encoding utf8
if(!$expansionPassed){throw 'Hub expansion tests failed'}
$framePassed=$code -eq 0 -and $output -contains 'HUB_FRAME_TESTS_RESULT passed=8 total=8'
[ordered]@{CheckedUtc=[DateTime]::UtcNow.ToString('o');Passed=$framePassed;Groups=8;ExitCode=$code;Scope='Safe margins, nonoverlapping hub regions and minimum 48px targets; pure native geometry, no editor/device';EngineExecuted=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/AUT-02-GEOMETRIA.json') -Encoding utf8
if(!$framePassed){throw 'Safe frame tests failed'}
$pointerPassed=$code -eq 0 -and $output -contains 'POINTER_INPUT_TESTS_RESULT passed=11 total=11'
[ordered]@{CheckedUtc=[DateTime]::UtcNow.ToString('o');Passed=$pointerPassed;Groups=11;ExitCode=$code;Scope='Pointer ownership, touch/mouse deduplication, focus/revision/drag cancellation and tap thresholds; no physical device or visual acceptance';EngineExecuted=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/AUT-03-ENTRADA.json') -Encoding utf8
if(!$pointerPassed){throw 'Pointer input tests failed'}
$textPassed=$code -eq 0 -and $output -contains 'TEXT_PANEL_TESTS_RESULT passed=10 total=10'
[ordered]@{CheckedUtc=[DateTime]::UtcNow.ToString('o');Passed=$textPassed;Groups=10;ExitCode=$code;Scope='UTF-8 scalar wrapping with injected font metrics, pagination and nonoverlapping panel geometry; no visual acceptance';EngineExecuted=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/AUT-04-TEXTO.json') -Encoding utf8
if(!$textPassed){throw 'Text panel tests failed'}
$preferencesPassed=$code -eq 0 -and $output -contains 'PREFERENCES_TESTS_RESULT passed=10 total=10 combinations=324'
[ordered]@{Date='2026-11-21';Passed=$preferencesPassed;ExitCode=$code;Groups=10;RoundTripCombinations=324;Scope='Versioned preferences codec, validation and primary/backup/default recovery policy; UE filesystem adapter not executed';EngineExecuted=$false;UnrealPersistenceExecuted=$false;AndroidExecuted=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/AUT-07-TESTES.json') -Encoding utf8
if(!$preferencesPassed){throw 'Preferences tests failed'}
$flowPassed=$code -eq 0 -and $output -contains 'SCREEN_FLOW_TESTS_RESULT passed=10 total=10'
[ordered]@{Date='2026-11-22';Passed=$flowPassed;ExitCode=$code;Groups=10;Scope='Explicit screen transitions, Back priority, modal gating and idempotent abandonment; no visual acceptance';EngineExecuted=$false;AndroidExecuted=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/AUT-08-FLUXO.json') -Encoding utf8
if(!$flowPassed){throw 'Screen flow tests failed'}
$mediaPassed=$code -eq 0 -and $output -contains 'MEDIA_FLOW_TESTS_RESULT passed=16 total=16'
[ordered]@{Date='2026-11-22';Passed=$mediaPassed;ExitCode=$code;Groups=16;Scope='Pure local media lifecycle, stale callbacks, watch state, pause/skip/failure and existing video geometry; no player or file IO';PlayerExecuted=$false;EditorOpened=$false;AndroidExecuted=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/AUT-10-TESTES.json') -Encoding utf8
if(!$mediaPassed){throw 'Media flow tests failed'}

$routingPassed=$code -eq 0 -and $output -contains 'INTERACTION_ROUTING_TESTS_RESULT passed=8 total=8'
[ordered]@{Date='2026-11-22';Passed=$routingPassed;Groups=8;Scope='Foreground Canvas first gesture, target change/overlap/origin/cancellation; native contract only';EngineExecuted=$false;AndroidExecuted=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/VIS-ENG-ENTRADA-TESTES.json') -Encoding utf8
if(!$routingPassed){throw 'Interaction routing tests failed'}
