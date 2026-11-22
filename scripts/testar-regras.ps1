param([ValidateSet('Outcomes','Pipeline','Controls','Periodic','Felipe','Pilot','SpecialShields','Waves','Score','Pve','Phases','Fronts','Hud','Ritual')][string]$Suite='Outcomes')
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$build=Join-Path $root ('build/rules-'+$Suite.ToLowerInvariant())
New-Item -ItemType Directory -Force -Path $build | Out-Null
$vswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$vs=@(& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -format json -utf8 | ConvertFrom-Json)
if($vs.Count -ne 1){throw 'Installed MSVC required; nothing is installed'}
$devcmd=Join-Path $vs[0].installationPath 'Common7/Tools/VsDevCmd.bat'
foreach($p in @($root,$devcmd)){if($p -match '[%"\r\n&|<>^!]'){throw 'Unsupported batch path'}}
$vectorPath=Join-Path $build 'oracle.txt'
$sources=if($Suite -in @('Pipeline','Controls','Periodic','Felipe','Pilot','SpecialShields','Waves','Score','Pve','Phases','Fronts','Hud','Ritual')){@('battle','ability_profile','local_session','team_selection','save_game','reference_encounter','visual_session','battle_presentation','stage','sequential','shared_boss')}else{@()}
if($Suite -in @('Pve','Phases','Fronts','Hud','Ritual')){$sources+=@('pve')}
if($Suite -eq 'Ritual'){$sources+=@('pve_visual')}
$test=@{Outcomes='hit_outcome_tests';Pipeline='damage_pipeline_tests';Controls='control_rules_tests';Periodic='periodic_effect_tests';Felipe='felipe_passive_tests';Pilot='felipe_pilot_tests';SpecialShields='special_shield_tests';Waves='wave_v2_tests';Score='score_tests';Pve='pve_mode_tests';Phases='pve_phase_tests';Fronts='front_area_tests';Hud='pve_presentation_tests';Ritual='pve_visual_tests'}[$Suite]
$batch=@"
@echo off
setlocal
set "VSCMD_SKIP_SENDTELEMETRY=1"
call "$devcmd" -no_logo -arch=x64 -host_arch=x64
if errorlevel 1 exit /b 10
set "FLAGS=/nologo /std:c++20 /permissive- /Zc:__cplusplus /EHsc /W4 /WX /utf-8 /Od /MT"
"@
foreach($name in $sources){$batch+="`ncl.exe %FLAGS% /I`"$root/include`" /c `"$root/src/$name.cpp`" /Fo`"$build/$name.obj`"`nif errorlevel 1 exit /b 11"}
$objects=($sources | ForEach-Object {'"'+$build+'/'+$_+'.obj"'}) -join ' '
$batch+="`ncl.exe %FLAGS% /I`"$root/include`" `"$root/tests/$test.cpp`" $objects /Fo`"$build/tests.obj`" /Fe`"$build/tests.exe`" /link /INCREMENTAL:NO`nif errorlevel 1 exit /b 12`n`"$build/tests.exe`" `"$vectorPath`"`nexit /b %ERRORLEVEL%`n"
$path=Join-Path $build 'testar.cmd';$batch | Set-Content $path -Encoding ascii
$output=@(& $env:ComSpec /d /c $path 2>&1 | ForEach-Object {$_.ToString()});$code=$LASTEXITCODE
$output | Set-Content (Join-Path $build 'tests.log') -Encoding utf8
$marker=@($output | Where-Object {$_ -match '^REG_[A-Z_]+_TESTS_RESULT passed=(\d+) total=(\d+)'})
$passed=$code -eq 0 -and $marker.Count -eq 1
if($passed){$m=[regex]::Match($marker[0],'passed=(\d+) total=(\d+)');$passed=$m.Groups[1].Value -eq $m.Groups[2].Value}
[ordered]@{Date='2026-11-22';Suite=$Suite;Passed=$passed;ExitCode=$code;Result=$marker;BigIntegerVectors=0;EngineExecuted=$false;AndroidExecuted=$false} | ConvertTo-Json | Set-Content (Join-Path $root ('docs/relatorios/atuais/REG-'+$Suite+'-TESTES.json')) -Encoding utf8
$output | Write-Output
if(!$passed){throw 'Rules verification failed'}
