param([Parameter(Mandatory=$true)][ValidateSet(12,13,14)][int]$Block)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$build=Join-Path $root "build/aut$Block"
$vswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$vs=@(& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -format json -utf8 | ConvertFrom-Json)
if($vs.Count -ne 1){throw 'Installed MSVC required; no download'}
$devcmd=Join-Path $vs[0].installationPath 'Common7/Tools/VsDevCmd.bat'
foreach($path in @($root,$devcmd)){if($path -match '[%"\r\n&|<>^!]'){throw 'Unsupported batch path'}}
$names=@{12='runtime_marker';13='visual_events';14='encounter_manifest'}
$test=$names[$Block]+'_tests'
$sources=@{12=@('arena_layout');13=@();14=@('battle','ability_profile','team_selection','reference_encounter')}
New-Item -ItemType Directory -Force -Path $build | Out-Null
$batch=@"
@echo off
setlocal
set "VSCMD_SKIP_SENDTELEMETRY=1"
call "$devcmd" -no_logo -arch=x64 -host_arch=x64
if errorlevel 1 exit /b 10
set "FLAGS=/nologo /std:c++20 /permissive- /Zc:__cplusplus /EHsc /W4 /WX /utf-8 /Od /MT"
"@
foreach($name in $sources[$Block]){$batch+="`ncl.exe %FLAGS% /I`"$root/include`" /c `"$root/src/$name.cpp`" /Fo`"$build/$name.obj`"`nif errorlevel 1 exit /b 11"}
$objects=($sources[$Block] | ForEach-Object {'"'+$build+'/'+$_+'.obj"'}) -join ' '
$batch+="`ncl.exe %FLAGS% /I`"$root/include`" `"$root/tests/$test.cpp`" $objects /Fo`"$build/tests.obj`" /Fe`"$build/tests.exe`" /link /INCREMENTAL:NO`nif errorlevel 1 exit /b 12`n`"$build/tests.exe`" `"$root/data/encounters`"`nexit /b %ERRORLEVEL%`n"
$path=Join-Path $build 'testar.cmd';$batch | Set-Content $path -Encoding ascii
$output=@(& $env:ComSpec /d /c $path 2>&1 | ForEach-Object {$_.ToString()});$code=$LASTEXITCODE
$output | Set-Content (Join-Path $build 'tests.log') -Encoding utf8
$marker=@($output | Where-Object {$_ -match "^AUT${Block}_TESTS_RESULT passed=(\d+) total=(\d+)$"})
$passed=$code -eq 0 -and $marker.Count -eq 1
if($passed){$counts=[regex]::Match($marker[0],'passed=(\d+) total=(\d+)');$passed=$counts.Success -and [int]$counts.Groups[1].Value -gt 0 -and $counts.Groups[1].Value -eq $counts.Groups[2].Value}
[ordered]@{Date='2026-11-22';Passed=$passed;ExitCode=$code;Result=$marker;Scope='Native contract only; no graphical/mobile execution'} | ConvertTo-Json | Set-Content (Join-Path $root "docs/relatorios/atuais/AUT-$Block-TESTES.json") -Encoding utf8
$output | Write-Output
if(!$passed){throw "AUT-$Block tests failed"}
