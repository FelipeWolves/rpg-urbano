# Tests layout contracts only; no Unreal, network, game or installer execution.
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$build=Join-Path $root 'build/arena-layout'
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
cl.exe %FLAGS% /I"$root/include" /c "$root/src/arena_layout.cpp" /Fo"$build/arena_layout.obj"
if errorlevel 1 exit /b 11
cl.exe %FLAGS% /I"$root/include" "$root/tests/arena_layout_tests.cpp" "$build/arena_layout.obj" /Fo"$build/tests.obj" /Fe"$build/tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 12
"$build/tests.exe"
exit /b %ERRORLEVEL%
"@
$batchPath=Join-Path $build 'testar.cmd';$batch | Set-Content $batchPath -Encoding ascii
$output=@(& $env:ComSpec /d /c $batchPath 2>&1 | ForEach-Object {$_.ToString()});$code=$LASTEXITCODE
$output | Set-Content (Join-Path $build 'resultado.log') -Encoding utf8
$passed=$code -eq 0 -and $output -contains 'ARENA_LAYOUT_TESTS_RESULT passed=6 total=6'
[ordered]@{CheckedUtc=[DateTime]::UtcNow.ToString('o');Passed=$passed;ExitCode=$code;Groups=6;FormationSizes=@(1,2,3,4,5);Layouts=@('Standard','Classic');Default='Standard';Scope='Native placement contracts; not visual/UI tests';EngineExecuted=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/VIS-01-LAYOUT.json') -Encoding utf8
$output | ForEach-Object {Write-Output $_}
if(!$passed){throw 'Arena layout tests failed'}
