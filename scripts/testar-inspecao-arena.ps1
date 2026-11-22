# Compiles only inspection dependencies and related native presentation tests.
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$build=Join-Path $root 'build/arena-inspection'
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
$sources=@('battle','ability_profile','local_session','reference_encounter','visual_session','battle_presentation','arena_layout')
foreach($name in $sources){$batch+="`ncl.exe %FLAGS% /I`"$root/include`" /c `"$root/src/$name.cpp`" /Fo`"$build/$name.obj`"`nif errorlevel 1 exit /b 11"}
$objects=($sources | ForEach-Object {'"'+$build+'/'+$_+'.obj"'}) -join ' '
$tests=@('arena_inspection_tests','arena_layout_tests','battle_presentation_tests','ability_card_tests')
foreach($name in $tests){$batch+="`ncl.exe %FLAGS% /I`"$root/include`" `"$root/tests/$name.cpp`" $objects /Fo`"$build/$name.obj`" /Fe`"$build/$name.exe`" /link /INCREMENTAL:NO`nif errorlevel 1 exit /b 12`n`"$build/$name.exe`"`nif errorlevel 1 exit /b 13"}
$batch+="`nexit /b 0`n"
$batchPath=Join-Path $build 'testar.cmd';$batch | Set-Content -LiteralPath $batchPath -Encoding ascii
$output=@(& $env:ComSpec /d /c $batchPath 2>&1 | ForEach-Object {$_.ToString()});$code=$LASTEXITCODE
$output | Set-Content (Join-Path $build 'resultado.log') -Encoding utf8
$passed=$code -eq 0 -and $output -contains 'ABILITY_CARD_TESTS_RESULT passed=10 total=10' -and $output -contains 'ARENA_INSPECTION_TESTS_RESULT passed=10 total=10 cases=13' -and $output -contains 'ARENA_LAYOUT_TESTS_RESULT passed=6 total=6' -and $output -contains 'PRESENTATION_TESTS_RESULT passed=6 total=6'
$cases=@($output | Where-Object {$_ -like 'CASE,*'} | ForEach-Object {$_ | ConvertFrom-Csv -Header Kind,Id,HP,Shield,Duration})
$cases | Select-Object Id,HP,Shield,Duration | Export-Csv (Join-Path $root 'docs/relatorios/atuais/AUT-05-CENARIOS.csv') -NoTypeInformation -Encoding utf8
[ordered]@{Date='2026-11-21';Passed=$passed;ExitCode=$code;NewInspectionGroups=10;RelatedLayoutGroups=6;RelatedPresentationGroups=6;Cases=$cases.Count;EngineExecuted=$false;VisualAcceptance=$false;AndroidExecuted=$false;Scope='Real executor snapshots, existing arena placement and shield display; no graphical execution'} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/AUT-05-TESTES.json') -Encoding utf8
$output | ForEach-Object {Write-Output $_}
if(!$passed){throw 'Inspection tests failed'}

[ordered]@{Date='2026-11-21';Passed=$passed;ExitCode=$code;NewAbilityGroups=10;TotalFocusedGroups=32;EngineExecuted=$false;AndroidExecuted=$false;Scope='Ability-card state mapping and reasons, existing arena inspection/layout/details regressions; not visual acceptance'} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/AUT-06-TESTES.json') -Encoding utf8