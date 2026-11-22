# Own native test tools only. Compiles core once; never runs UE/game/Android or installs anything.
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$build=Join-Path $root 'build/esc-sis-pve'
New-Item -ItemType Directory -Force -Path $build | Out-Null
$vswhere=Join-Path ([Environment]::GetEnvironmentVariable('ProgramFiles(x86)')) 'Microsoft Visual Studio/Installer/vswhere.exe'
$vs=@(& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -format json -utf8 | ConvertFrom-Json)
if($vs.Count -ne 1){throw 'Installed MSVC required; nothing is installed'}
$devcmd=Join-Path $vs[0].installationPath 'Common7/Tools/VsDevCmd.bat'
foreach($p in @($root,$devcmd)){if($p -match '[%"\r\n&|<>^!]'){throw 'Unsupported batch path'}}
$sources=@('battle','ability_profile','local_session','team_selection','save_game','reference_encounter','visual_session','battle_presentation','stage','sequential','shared_boss','pve')
$suites=[ordered]@{Outcomes='hit_outcome_tests';Pipeline='damage_pipeline_tests';Controls='control_rules_tests';Periodic='periodic_effect_tests';Felipe='felipe_passive_tests';Pilot='felipe_pilot_tests';SpecialShields='special_shield_tests';Waves='wave_v2_tests';Score='score_tests';Pve='pve_mode_tests';Phases='pve_phase_tests';Fronts='front_area_tests'}
$batch=[System.Collections.Generic.List[string]]::new()
$batch.Add('@echo off');$batch.Add('setlocal');$batch.Add('set "VSCMD_SKIP_SENDTELEMETRY=1"')
$batch.Add(('call "{0}" -no_logo -arch=x64 -host_arch=x64' -f $devcmd));$batch.Add('if errorlevel 1 exit /b 10')
$batch.Add('set "FLAGS=/nologo /std:c++20 /permissive- /Zc:__cplusplus /EHsc /W4 /WX /utf-8 /Od /MT"')
foreach($name in $sources){
 $batch.Add(('cl.exe %FLAGS% /I"{0}/include" /c "{0}/src/{1}.cpp" /Fo"{2}/{1}.obj"' -f $root,$name,$build))
 $batch.Add('if errorlevel 1 exit /b 11')
}
$objects=($sources | ForEach-Object {'"'+$build+'/'+$_+'.obj"'}) -join ' '
foreach($name in $suites.Keys){
 $test=$suites[$name]
 $batch.Add(('cl.exe %FLAGS% /I"{0}/include" "{0}/tests/{1}.cpp" {2} /Fo"{3}/test-{4}.obj" /Fe"{3}/{4}.exe" /link /INCREMENTAL:NO' -f $root,$test,$objects,$build,$name))
 $batch.Add('if errorlevel 1 exit /b 12')
 $batch.Add(('"{0}/{1}.exe" > "{0}/{1}.log" 2>&1' -f $build,$name))
 $batch.Add(('echo %ERRORLEVEL% > "{0}/{1}.exit"' -f $build,$name))
}
$batch.Add('exit /b 0')
$path=Join-Path $build 'testar.cmd';$batch | Set-Content $path -Encoding ascii
$output=@(& $env:ComSpec /d /c $path 2>&1 | ForEach-Object {$_.ToString()});$exit=$LASTEXITCODE
$output | Set-Content (Join-Path $build 'compile.log') -Encoding utf8
if($exit -ne 0){$output | Select-Object -Last 20 | Write-Output;throw 'Native test build failed'}
$results=@()
foreach($name in $suites.Keys){
 $code=[int](Get-Content (Join-Path $build ($name+'.exit')) -Raw)
 $lines=@(Get-Content (Join-Path $build ($name+'.log')))
 $markers=@($lines | Where-Object {$_ -match '^REG_[A-Z_]+_TESTS_RESULT passed=(\d+) total=(\d+)'})
 $ok=$code -eq 0 -and $markers.Count -eq 1
 if($ok){$match=[regex]::Match($markers[0],'passed=(\d+) total=(\d+)');$ok=$match.Groups[1].Value -eq $match.Groups[2].Value}
 $result=[ordered]@{Date='2026-11-22';Suite=$name;Passed=$ok;ExitCode=$code;Result=$markers;EngineExecuted=$false;AndroidExecuted=$false}
 $result | ConvertTo-Json | Set-Content (Join-Path $root ('docs/relatorios/atuais/REG-'+$name+'-TESTES.json')) -Encoding utf8
 $results+=$result
 if($ok){Write-Output $markers[0]}else{$lines | Select-Object -Last 5 | Write-Output}
}
$all=@($results | Where-Object {!$_.Passed}).Count -eq 0
[ordered]@{Date='2026-11-22';Passed=$all;CoreCompiledOnce=$true;Suites=$results;EngineExecuted=$false;AndroidExecuted=$false;InternetUsed=$false} | ConvertTo-Json -Depth 6 | Set-Content (Join-Path $root 'docs/relatorios/atuais/ESC-SIS-PVE-TESTES.json') -Encoding utf8
if(!$all){throw 'Native family regression failed'}
