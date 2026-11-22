param([ValidateSet('Arithmetic','Wide')][string]$Suite='Arithmetic')
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$build=Join-Path $root ('build/numeric-'+$Suite.ToLowerInvariant())
New-Item -ItemType Directory -Force -Path $build | Out-Null
$vswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$vs=@(& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -format json -utf8 | ConvertFrom-Json)
if($vs.Count -ne 1){throw 'Installed MSVC required; nothing is installed'}
$devcmd=Join-Path $vs[0].installationPath 'Common7/Tools/VsDevCmd.bat'
foreach($p in @($root,$devcmd)){if($p -match '[%"\r\n&|<>^!]'){throw 'Unsupported batch path'}}
$vectorPath=Join-Path $build 'oracle.txt'
$random=[Random]::new(22026);$cap=[long]1000000000000
$lines=for($i=0;$i -lt 256;++$i){$a=$random.NextInt64(0,[long]::MaxValue);$b=$random.NextInt64(0,$cap);$d=$random.NextInt64(1,[long]::MaxValue);$limit=if($i%3 -eq 0){$random.NextInt64(0,$cap)}else{$cap};$value=([System.Numerics.BigInteger]$a*[System.Numerics.BigInteger]$b)/[System.Numerics.BigInteger]$d;if($value -gt $limit){$value=[System.Numerics.BigInteger]$limit};$ratio=([System.Numerics.BigInteger]$a*[System.Numerics.BigInteger]$cap).CompareTo([System.Numerics.BigInteger]$b*[System.Numerics.BigInteger]$d);"$a $b $d $limit $value $ratio"}
$lines | Set-Content $vectorPath -Encoding ascii
$sources=if($Suite -eq 'Wide'){@('battle','ability_profile','local_session','team_selection','save_game','reference_encounter','visual_session','battle_presentation','stage','sequential','shared_boss')}else{@()}
$test=if($Suite -eq 'Wide'){'wide_numeric_tests'}else{'numeric_tests'}
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
$marker=@($output | Where-Object {$_ -match '^(WIDE_)?NUMERIC_TESTS_RESULT passed=(\d+) total=(\d+)'})
$passed=$code -eq 0 -and $marker.Count -eq 1
if($passed){$m=[regex]::Match($marker[0],'passed=(\d+) total=(\d+)');$passed=$m.Groups[1].Value -eq $m.Groups[2].Value}
[ordered]@{Date='2026-11-22';Suite=$Suite;Passed=$passed;ExitCode=$code;Result=$marker;BigIntegerVectors=256;EngineExecuted=$false;AndroidExecuted=$false} | ConvertTo-Json | Set-Content (Join-Path $root ('docs/relatorios/atuais/SIS-02-'+$Suite+'-TESTES.json')) -Encoding utf8
$output | Write-Output
if(!$passed){throw 'Numeric verification failed'}
