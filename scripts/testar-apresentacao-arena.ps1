# Runs only the explicitly authorized arena package; does not open the editor.
param([ValidateSet('Standard','Classic')][string]$Layout='Standard')
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$build=Get-Content -LiteralPath (Join-Path $root 'docs/relatorios/atuais/VIS-01-LAYOUT-PACOTE.json') -Raw | ConvertFrom-Json
if(!$build.Passed -or $build.ExitCode -ne 0){throw 'Current arena package required'}
$archive=[IO.Path]::GetFullPath($build.Archive)
$boundary=[IO.Path]::GetFullPath((Join-Path $root 'build/p'))+[IO.Path]::DirectorySeparatorChar
if(!$archive.StartsWith($boundary,[StringComparison]::OrdinalIgnoreCase)){throw 'Package outside evaluation directory'}
$exe=Join-Path $archive 'Windows/RpgEngineProbe.exe'
$mainExe=Join-Path $archive 'Windows/RpgEngineProbe/Binaries/Win64/RpgEngineProbe.exe'
foreach($file in @($exe,$mainExe)){if(!(Test-Path -LiteralPath $file -PathType Leaf)){throw 'Packaged executable missing'}}
$folder=Join-Path $root ('build/arena-runtime/'+$Layout+'-'+[DateTime]::UtcNow.ToString('yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Path $folder | Out-Null
$log=Join-Path $folder 'runtime.log'
$psi=[Diagnostics.ProcessStartInfo]::new();$psi.FileName=$exe;$psi.WorkingDirectory=Split-Path $exe -Parent;$psi.UseShellExecute=$false;$psi.RedirectStandardOutput=$true;$psi.RedirectStandardError=$true
foreach($a in @('-culture=en','-Windowed','-ResX=1280','-ResY=720','-NoSplash','-NoSound','-ExecCmds=t.MaxFPS 30',('-UserDir='+$folder+'/user'),('-abslog='+$log))){$psi.ArgumentList.Add($a)}
# Omit the argument in Standard to verify the actual default. Classic is explicit.
if($Layout -eq 'Classic'){$psi.ArgumentList.Add('-ArenaLayout=Classic')}
$entry=[Diagnostics.Process]::Start($psi);$out=$entry.StandardOutput.ReadToEndAsync();$err=$entry.StandardError.ReadToEndAsync()
$reportPath=Join-Path $root ('docs/relatorios/atuais/VIS-01-LAYOUT-RUNTIME-'+$Layout.ToUpperInvariant()+'.json')
$r=[ordered]@{StartedUtc=[DateTime]::UtcNow.ToString('o');RequestedLayout=$Layout;ExplicitLayoutArgument=($Layout -eq 'Classic');EntryProcessId=$entry.Id;EntryExecutable=$exe;ExecutableSHA256=(Get-FileHash -LiteralPath $mainExe).Hash;Log=$log;Status='running';NetworkChanged=$false;VisualAcceptance=$false}
$r | ConvertTo-Json | Set-Content -LiteralPath $reportPath -Encoding utf8
$main=$null
for($i=0;$i -lt 30;$i++){
    $found=@(Get-CimInstance Win32_Process -Filter "Name='RpgEngineProbe.exe'" | Where-Object ExecutablePath -eq $mainExe)
    if($found.Count -eq 1){$main=[Diagnostics.Process]::GetProcessById($found[0].ProcessId);$null=$main.Handle;break}
    if($entry.HasExited){break}
    Start-Sleep -Milliseconds 300
}
Write-Output "ARENA_RUNNING layout=$Layout"
if($main){$main.WaitForExit();$r.RuntimeProcessId=$main.Id;$r.RuntimeExitCode=$main.ExitCode}
$entry.WaitForExit();$null=$out.GetAwaiter().GetResult();$null=$err.GetAwaiter().GetResult()
$r.EntryExitCode=$entry.ExitCode;$r.FinishedUtc=[DateTime]::UtcNow.ToString('o');$r.Status='closed_pending_visual_report'
$r | ConvertTo-Json | Set-Content -LiteralPath $reportPath -Encoding utf8
Write-Output "ARENA_CLOSED layout=$Layout entry_exit=$($entry.ExitCode)"
