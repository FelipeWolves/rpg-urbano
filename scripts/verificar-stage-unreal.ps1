# Verifies an existing prepared stage; never executes its build plan or binaries.
param([string]$PreparationReport)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
if(!$PreparationReport){$PreparationReport=Join-Path $root 'docs/relatorios/atuais/ENG-01-PACOTE-PREPARACAO.json'}
$report=Get-Content -LiteralPath $PreparationReport -Raw | ConvertFrom-Json
$prefix=[IO.Path]::GetFullPath((Join-Path $root 'build/p'))+[IO.Path]::DirectorySeparatorChar
$stage=[IO.Path]::GetFullPath([string]$report.Stage)
if(!$stage.StartsWith($prefix,[StringComparison]::OrdinalIgnoreCase) -or (Split-Path $stage -Leaf) -ne 'RpgEngineProbe'){throw 'Stage outside expected build/p boundary'}
$stagePrefix=$stage+[IO.Path]::DirectorySeparatorChar
$rootPrefix=[IO.Path]::GetFullPath($root)+[IO.Path]::DirectorySeparatorChar
if($report.Status -ne 'prepared_not_built' -or $report.BuildExecuted -or $report.CookExecuted){throw 'Expected preparation-only report'}
$failures=[Collections.Generic.List[string]]::new()
$verified=0
foreach($item in $report.SourceManifest){
    if([IO.Path]::IsPathRooted($item.Original) -or [IO.Path]::IsPathRooted($item.StagedRelative)){throw 'Rooted manifest entry rejected'}
    $source=[IO.Path]::GetFullPath((Join-Path $root $item.Original))
    $target=[IO.Path]::GetFullPath((Join-Path $stage $item.StagedRelative))
    if(!$source.StartsWith($rootPrefix,[StringComparison]::OrdinalIgnoreCase) -or !$target.StartsWith($stagePrefix,[StringComparison]::OrdinalIgnoreCase)){throw 'Manifest entry escapes its boundary'}
    if(!(Test-Path -LiteralPath $source -PathType Leaf) -or !(Test-Path -LiteralPath $target -PathType Leaf)){$failures.Add('missing: '+$item.StagedRelative);continue}
    if($item.SHA256 -notmatch '^[A-Fa-f0-9]{64}$' -or (Get-FileHash -LiteralPath $source -Algorithm SHA256).Hash -ne $item.SHA256 -or (Get-FileHash -LiteralPath $target -Algorithm SHA256).Hash -ne $item.SHA256){$failures.Add('hash mismatch: '+$item.StagedRelative);continue}
    ++$verified
}
$include=Join-Path $stage 'Source/RpgEngineProbe/Public/NativeCore'
foreach($file in Get-ChildItem -LiteralPath (Join-Path $stage 'Source') -Recurse -File | Where-Object Extension -in @('.cpp','.h','.hpp')){
    foreach($match in [regex]::Matches([IO.File]::ReadAllText($file.FullName),'#include\s+"(rpg/[A-Za-z0-9_]+\.hpp)"')){if(!(Test-Path -LiteralPath (Join-Path $include $match.Groups[1].Value))){$failures.Add('missing include: '+$match.Groups[1].Value)}}
}
$planPath=[IO.Path]::GetFullPath([string]$report.Plan)
$planBoundary=[IO.Path]::GetFullPath((Split-Path $stage -Parent))+[IO.Path]::DirectorySeparatorChar
if(!$planPath.StartsWith($planBoundary,[StringComparison]::OrdinalIgnoreCase)){throw 'Plan outside staging run'}
$plan=Get-Content -LiteralPath $planPath -Raw | ConvertFrom-Json
if($plan.ExecutionAuthorized){$failures.Add('Build plan unexpectedly authorized')}
$project=Get-Content -LiteralPath (Join-Path $stage 'RpgEngineProbe.uproject') -Raw | ConvertFrom-Json
$wmf=@($project.Plugins | Where-Object {$_.Name -eq 'WmfMedia' -and $_.Enabled -and 'Win64' -in $_.PlatformAllowList})
if($wmf.Count -ne 1){$failures.Add('Expected Win64 decoder not enabled')}
[ordered]@{Date='2026-11-23';Passed=$failures.Count -eq 0;StageRelative=[IO.Path]::GetRelativePath($root,$stage);Inputs=$report.SourceManifest.Count;VerifiedHashes=$verified;Failures=@($failures);IncludeClosureChecked=$true;BuildExecuted=$false;CookExecuted=$false;EditorOpened=$false;PackageValidated=$false;AndroidExecuted=$false} | ConvertTo-Json -Depth 5 | Set-Content (Join-Path $root 'docs/relatorios/atuais/AUT-15-STAGE.json') -Encoding utf8
if($failures.Count){throw ($failures -join '; ')}
Write-Output "AUT15_STAGE_VERIFIED inputs=$verified build=False cook=False"
