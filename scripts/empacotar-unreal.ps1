# Executes an explicitly authorized package plan; never launches the packaged application.
param([ValidateSet('ENG-01-PACOTE-BUILD.json','VIS-01C-BUILD.json')][string]$ReportName='ENG-01-PACOTE-BUILD.json')
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$r=Get-Content -LiteralPath (Join-Path $root 'docs/relatorios/atuais/ENG-01-PACOTE-PREPARACAO.json') -Raw | ConvertFrom-Json
$boundary=[IO.Path]::GetFullPath((Join-Path $root 'build/p'))+[IO.Path]::DirectorySeparatorChar
if(![IO.Path]::GetFullPath($r.Stage).StartsWith($boundary,[StringComparison]::OrdinalIgnoreCase)){throw 'Unexpected package stage'}
foreach($i in $r.SourceManifest){foreach($p in @((Join-Path $root $i.Original),(Join-Path $r.Stage $i.StagedRelative))){if((Get-FileHash -LiteralPath $p).Hash -ne $i.SHA256){throw 'Manifest changed'}}}
$plan=Get-Content -LiteralPath $r.Plan -Raw | ConvertFrom-Json
$engine=Split-Path $plan.WorkingDirectory -Parent
$v=Get-Content -LiteralPath (Join-Path $engine 'Engine/Build/Build.version') -Raw | ConvertFrom-Json
if($v.Changelist -ne $r.EngineChangelist){throw 'Engine changed'}
$expected=Join-Path $engine 'Engine/Binaries/ThirdParty/DotNet/10.0/win-x64/dotnet.exe'
if([IO.Path]::GetFullPath($plan.FileName) -ne [IO.Path]::GetFullPath($expected)){throw 'Unexpected execution tool'}
$folder=Split-Path $r.Plan -Parent
$log=Join-Path $folder 'uat-stdout.log';$err=Join-Path $folder 'uat-stderr.log'
if((Test-Path -LiteralPath $log) -or (Test-Path -LiteralPath $err)){throw 'Preserve previous logs: prepare a fresh package stage'}
$psi=[Diagnostics.ProcessStartInfo]::new();$psi.FileName=$plan.FileName;$psi.WorkingDirectory=$plan.WorkingDirectory;$psi.UseShellExecute=$false;$psi.CreateNoWindow=$true
$psi.RedirectStandardOutput=$true;$psi.RedirectStandardError=$true
foreach($a in $plan.ArgumentList){$psi.ArgumentList.Add($a)}
foreach($p in $plan.Environment.PSObject.Properties){$psi.Environment[$p.Name]=[string]$p.Value}
$psi.Environment['DOTNET_CLI_HOME']=Join-Path $folder 'dotnet-home';$psi.Environment['NUGET_PACKAGES']=Join-Path $folder 'nuget'
$stdout=[IO.File]::Open($log,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::Read)
$stderr=[IO.File]::Open($err,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::Read)
try {
    $process=[Diagnostics.Process]::Start($psi)
    $outTask=$process.StandardOutput.BaseStream.CopyToAsync($stdout);$errTask=$process.StandardError.BaseStream.CopyToAsync($stderr)
    $report=[ordered]@{StartedUtc=[DateTime]::UtcNow.ToString('o');Status='running';ProcessId=$process.Id;Stage=$r.Stage;Archive=$r.Archive;Stdout=$log;Stderr=$err;Plan=$r.Plan;SourceManifestVerified=$true;NetworkChanged=$false;PackageLaunched=$false;Passed=$false}
    $reportPath=Join-Path (Join-Path $root 'docs/relatorios/atuais') $ReportName
    $report | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath $reportPath -Encoding utf8
    Write-Output "PACKAGE_BUILD_STARTED pid=$($process.Id)"
    $process.WaitForExit();$null=$outTask.GetAwaiter().GetResult();$null=$errTask.GetAwaiter().GetResult()
    $report.Status='completed';$report.ExitCode=$process.ExitCode;$report.FinishedUtc=[DateTime]::UtcNow.ToString('o');$report.Passed=($process.ExitCode -eq 0)
    $report | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath $reportPath -Encoding utf8
    Write-Output "PACKAGE_BUILD_FINISHED code=$($process.ExitCode)"
} finally {$stdout.Dispose();$stderr.Dispose()}
