$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$build=Join-Path $root 'build/pointer-focus'
New-Item -ItemType Directory -Force -Path $build | Out-Null
$vswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$vs=@(& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -format json -utf8 | ConvertFrom-Json)
if($vs.Count -ne 1){throw 'Installed MSVC required; nothing is installed'}
$devcmd=Join-Path $vs[0].installationPath 'Common7/Tools/VsDevCmd.bat'
foreach($p in @($root,$devcmd)){if($p -match '[%"\r\n&|<>^!]'){throw 'Unsupported batch path'}}
$vectorPath=Join-Path $build 'oracle.txt'
$sources=@('startup_presentation')
$test='pointer_input_tests'
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
$passed=$code -eq 0 -and $output -contains 'POINTER_INPUT_TESTS_RESULT passed=10 total=10'
[ordered]@{Date='2026-11-23';Passed=$passed;Groups=10;ExitCode=$code;Scope='Existing pointer model only; not runtime mouse confinement or engine event delivery';EditorOpened=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/HUB-POINTER-2026-11-23.json') -Encoding utf8
$output | Select-Object -Last 4
if(!$passed){throw 'Pointer model failed'}
