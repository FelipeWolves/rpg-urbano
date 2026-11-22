# QA-only filesystem tests; never launches Unreal or reads normal Saved paths.
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$build=Join-Path $root 'build/qa-save-io'
$vswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$vs=@(& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -format json -utf8 | ConvertFrom-Json)
if($vs.Count -ne 1){throw 'Existing C++ tools required; nothing is installed'}
$devcmd=Join-Path $vs[0].installationPath 'Common7/Tools/VsDevCmd.bat'
foreach($p in @($root,$devcmd)){if($p -match '[%"\r\n&|<>^!]'){throw 'Unsupported batch path'}}

$boundary=[IO.Path]::GetFullPath((Join-Path $root 'build'))
foreach($path in @($root,$boundary,$build)){
 if((Test-Path -LiteralPath $path) -and ((Get-Item -LiteralPath $path).Attributes -band [IO.FileAttributes]::ReparsePoint)){throw 'QA paths must not be junctions or symlinks'}
}
New-Item -ItemType Directory -Force -Path $build | Out-Null
$run=Join-Path $build ('run-'+[Guid]::NewGuid().ToString('N'))
if(![IO.Path]::GetFullPath($run).StartsWith([IO.Path]::GetFullPath($build)+[IO.Path]::DirectorySeparatorChar,[StringComparison]::OrdinalIgnoreCase)){throw 'Invalid QA boundary'}
New-Item -ItemType Directory -Path $run | Out-Null
$batch=@"
@echo off
setlocal
set "VSCMD_SKIP_SENDTELEMETRY=1"
call "$devcmd" -no_logo -arch=x64 -host_arch=x64
if errorlevel 1 exit /b 10
set "FLAGS=/nologo /std:c++20 /permissive- /Zc:__cplusplus /EHsc /W4 /WX /utf-8 /Od /MT"
"@
$sources=@('battle','ability_profile','local_session','reference_encounter','visual_session','battle_presentation','arena_layout','startup_presentation','team_selection','save_game')
foreach($name in $sources){$batch+="`ncl.exe %FLAGS% /I`"$root/include`" /c `"$root/src/$name.cpp`" /Fo`"$build/$name.obj`"`nif errorlevel 1 exit /b 11"}
$objects=($sources | ForEach-Object {'"'+$build+'/'+$_+'.obj"'}) -join ' '
$tests=@('save_io_lab_tests')
foreach($name in $tests){$batch+="`ncl.exe %FLAGS% /I`"$root/include`" `"$root/tests/$name.cpp`" $objects /Fo`"$build/$name.obj`" /Fe`"$build/$name.exe`" /link /INCREMENTAL:NO`nif errorlevel 1 exit /b 12`n`"$build/$name.exe`" `"$run`"`nif errorlevel 1 exit /b 13"}
$batch+="`nexit /b 0`n"
$batchPath=Join-Path $build 'testar.cmd';$batch | Set-Content -LiteralPath $batchPath -Encoding ascii
$output=@(& $env:ComSpec /d /c $batchPath 2>&1 | ForEach-Object {$_.ToString()});$code=$LASTEXITCODE
$output | Set-Content (Join-Path $build 'resultado.log') -Encoding utf8
$passed=$code -eq 0 -and $output -contains 'QA_SAVE_IO_RESULT passed=16 total=16'
[ordered]@{Date='2026-11-22';Passed=$passed;ExitCode=$code;Groups=16;Backend='Shared production persistence orchestration and codecs; Windows QA backend, not Unreal IFileManager';Directory=[IO.Path]::GetRelativePath($root,$run);NormalSavesTouched=$false;UnrealAdapterExecuted=$false;EditorOpened=$false;GameExecuted=$false;AndroidExecuted=$false;Cases=@($output | Where-Object {$_ -like 'CASE,*'});Limitations=@('No power-loss durability certification','No actual disk-full or OS ACL changes','No automatic progress backup added','Unreal UI save/reopen remains pending')} | ConvertTo-Json -Depth 6 | Set-Content (Join-Path $root 'docs/relatorios/atuais/LAB-QA-01C.json') -Encoding utf8
$output | Select-Object -Last 14
if(!$passed){throw 'Isolated save IO tests failed'}
