param([Parameter(Mandatory=$true)][string]$EngineRoot)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$build=Get-Content -LiteralPath (Join-Path $root 'docs/relatorios/atuais/ENG-01-BUILD.json') -Raw | ConvertFrom-Json
if($build.ExitCode -ne 0 -or !$build.ModuleBinaryPresent -or !$build.Baseline){throw 'Compiled bridge baseline required'}
$stage=[IO.Path]::GetFullPath($build.Stage)
$allowed=[IO.Path]::GetFullPath((Join-Path $root 'build/engine-eval'))+[IO.Path]::DirectorySeparatorChar
if(!$stage.StartsWith($allowed,[StringComparison]::OrdinalIgnoreCase)){throw 'Invalid evaluation stage'}
foreach($item in $build.SourceManifest){
    $hash=(Get-FileHash -LiteralPath (Join-Path $stage $item.StagedRelative) -Algorithm SHA256).Hash
    $original=(Get-FileHash -LiteralPath (Join-Path $root $item.Original) -Algorithm SHA256).Hash
    if($hash -ne $item.SHA256 -or $original -ne $item.SHA256){throw 'Sources changed; rebuild bridge first'}
}
$engine=[IO.Path]::GetFullPath($EngineRoot)
$version=Get-Content -LiteralPath (Join-Path $engine 'Engine/Build/Build.version') -Raw | ConvertFrom-Json
if($version.Changelist -ne $build.EngineChangelist){throw 'Engine build changed'}
$editor=Join-Path $engine 'Engine/Binaries/Win64/UnrealEditor-Cmd.exe'
if(!(Test-Path -LiteralPath $editor -PathType Leaf)){throw 'Command-line editor missing'}
$runRoot=Join-Path $root ('build/engine-eval/runtime/'+[DateTime]::UtcNow.ToString('yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Force -Path $runRoot | Out-Null
$expected=$build.Baseline.Result
$runs=@()
for($index=1;$index -le 2;$index++){
    $log=Join-Path $runRoot "run-$index.log"
    $info=[Diagnostics.ProcessStartInfo]::new();$info.FileName=$editor;$info.UseShellExecute=$false;$info.CreateNoWindow=$true
    $info.RedirectStandardOutput=$true;$info.RedirectStandardError=$true;$info.WorkingDirectory=$stage
    $arguments=@((Join-Path $stage 'RpgEngineProbe.uproject'),'-run=RpgCoreProbe','-NullRHI','-Unattended','-NoSplash','-NoSound',
        '-NoAnalytics','-NoEpicPortal','-NoSourceControl','-NoLiveCoding','-NoCrashDialog','-stdout','-FullStdOutLogOutput','-UTF8Output',('-abslog='+$log))
    foreach($argument in $arguments){$info.ArgumentList.Add($argument)}
    $process=[Diagnostics.Process]::new();$process.StartInfo=$info
    $watch=[Diagnostics.Stopwatch]::StartNew();[void]$process.Start()
    $stdout=$process.StandardOutput.ReadToEndAsync();$stderr=$process.StandardError.ReadToEndAsync()
    $timedOut=$false
    while(!$process.WaitForExit(1000)){
        if($watch.Elapsed.TotalSeconds -gt 180){$timedOut=$true;$process.Kill($true);$process.WaitForExit();break}
    }
    $text=$stdout.GetAwaiter().GetResult()+"`n"+$stderr.GetAwaiter().GetResult()
    [IO.File]::WriteAllText((Join-Path $runRoot "console-$index.log"),$text,[Text.UTF8Encoding]::new($false))
    $lines=@($text -split "`r?`n")
    $markers=@($lines | Where-Object {$_ -match 'RPG_CORE_PROBE passed='})
    $match=$markers.Count -eq 1 -and $markers[0].Contains($expected)
    $passed=!$timedOut -and $process.ExitCode -eq 0 -and $match
    $runs += [ordered]@{Run=$index;ExitCode=$process.ExitCode;TimedOut=$timedOut;ElapsedSeconds=[math]::Round($watch.Elapsed.TotalSeconds,2);MarkerCount=$markers.Count;MatchesStandalone=$match;Passed=$passed;Markers=$markers}
    Write-Output ("ENGINE_RUNTIME run=$index exit="+$process.ExitCode+" match=$match timeout=$timedOut")
    $process.Dispose()
    if(!$passed){break}
}
$passed=$runs.Count -eq 2 -and @($runs | Where-Object {!$_.Passed}).Count -eq 0
[ordered]@{CheckedUtc=[DateTime]::UtcNow.ToString('o');Scope='Own staged UE project, command-line editor, NullRHI; own commandlet runs the shared bridge; no editor interface';EngineVersion='5.8.4';EngineChangelist=$version.Changelist;StandaloneExpected=$expected;Passed=$passed;Runs=$runs;GraphicalEditorOpened=$false;EngineProcessExecuted=$true;PlayerSavesUsed=$false;TimeoutSecondsPerRun=180;SourceManifestVerified=$true;CaptureDirectory=$runRoot} |
    ConvertTo-Json -Depth 7 | Set-Content -LiteralPath (Join-Path $root 'docs/relatorios/atuais/ENG-01-RUNTIME.json') -Encoding utf8
if(!$passed){throw 'Unreal runtime mismatch/failure; inspect build/engine-eval/runtime logs'}
