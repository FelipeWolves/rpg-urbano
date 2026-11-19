# Historical graphical opening recipe; not network-isolated. NoAnalytics did not prevent telemetry.
# Do not rerun until ENG-G01/ENG-G02 are addressed and specific user authorization is obtained.
param([Parameter(Mandatory=$true)][string]$EngineRoot)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$build=Get-Content -LiteralPath (Join-Path $root 'docs/relatorios/atuais/ENG-01-BUILD.json') -Raw | ConvertFrom-Json
if($build.ExitCode -ne 0 -or !$build.ModuleBinaryPresent){throw 'Successful probe build required'}
$stage=[IO.Path]::GetFullPath($build.Stage)
$allowed=[IO.Path]::GetFullPath((Join-Path $root 'build/engine-eval'))+[IO.Path]::DirectorySeparatorChar
if(!$stage.StartsWith($allowed,[StringComparison]::OrdinalIgnoreCase)){throw 'Invalid evaluation stage'}
foreach($item in $build.SourceManifest){
    foreach($file in @((Join-Path $stage $item.StagedRelative),(Join-Path $root $item.Original))){
        if((Get-FileHash -LiteralPath $file -Algorithm SHA256).Hash -ne $item.SHA256){throw 'Rebuild probe: source changed'}
    }
}
$engine=[IO.Path]::GetFullPath($EngineRoot)
$version=Get-Content -LiteralPath (Join-Path $engine 'Engine/Build/Build.version') -Raw | ConvertFrom-Json
if($version.Changelist -ne $build.EngineChangelist){throw 'Engine version changed'}
$editor=Join-Path $engine 'Engine/Binaries/Win64/UnrealEditor.exe'
if(!(Test-Path -LiteralPath $editor -PathType Leaf)){throw 'Graphical editor absent'}
$folder=Join-Path $root ('build/engine-eval/graphical/'+[DateTime]::UtcNow.ToString('yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Force -Path $folder | Out-Null
$log=Join-Path $folder 'editor.log'
$info=[Diagnostics.ProcessStartInfo]::new();$info.FileName=$editor;$info.UseShellExecute=$false;$info.CreateNoWindow=$false;$info.WorkingDirectory=$stage
$arguments=@((Join-Path $stage 'RpgEngineProbe.uproject'),'/Engine/Maps/Templates/Template_Default',
    '-NoSplash','-NoSound','-NoAnalytics','-NoEpicPortal','-NoSourceControl','-NoLiveCoding','-Windowed','-ResX=1280','-ResY=720',
    '-ini:Engine:[DevOptions.Shaders]:PercentageUnusedShaderCompilingThreads=75','-ExecCmds=t.MaxFPS 30',('-abslog='+$log))
foreach($argument in $arguments){$info.ArgumentList.Add($argument)}
$process=[Diagnostics.Process]::Start($info)
[ordered]@{StartedUtc=[DateTime]::UtcNow.ToString('o');Scope='Graphical opening/inspection/closing only; built-in engine template map, no own arena';EngineRoot=$engine;EngineVersion='5.8.4';EngineChangelist=$version.Changelist;Stage=$stage;ProcessId=$process.Id;Log=$log;Passed=$false;Status='started_waiting_observation';GraphicalWindowObserved=$false;PlayerSavesUsed=$false;ShaderUnusedThreadsPercent=75;RequestedMaxFPS=30;SourceManifestVerified=$true} |
    ConvertTo-Json -Depth 6 | Set-Content -LiteralPath (Join-Path $root 'docs/relatorios/atuais/ENG-01-GRAFICO.json') -Encoding utf8
Write-Output "GRAPHICAL_STARTED pid=$($process.Id) log=$log"
