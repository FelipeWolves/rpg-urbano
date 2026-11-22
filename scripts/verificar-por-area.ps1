# Selects native/file checks only. Never launches Unreal, a package or an Android tool.
param(
    [ValidateSet('Marker','Events','Data','Media','Session','Stage','Arithmetic','Wide','Feedback','LabQaPlan')][string[]]$Areas=@('Marker','Events','Data'),
    [switch]$Execute
)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$map=@{
    Arithmetic=@{Script='testar-numerico.ps1';Arguments=@('-Suite','Arithmetic');Evidence='Exact integer arithmetic and BigInteger oracle'}
    Wide=@{Script='testar-numerico.ps1';Arguments=@('-Suite','Wide');Evidence='Wide battle quantities and compatibility; no graphical acceptance'}
    Marker=@{Script='testar-autonomo.ps1';Arguments=@('-Block','12');Evidence='Native marker geometry'}
    Events=@{Script='testar-autonomo.ps1';Arguments=@('-Block','13');Evidence='Native cue contract'}
    Data=@{Script='testar-autonomo.ps1';Arguments=@('-Block','14');Evidence='Native manifest codec and parity'}
    Media=@{Script='testar-apresentacao-inicial.ps1';Arguments=@();Evidence='Native lifecycle/geometry/input; no playback'}
    Session=@{Script='testar-inspecao-arena.ps1';Arguments=@();Evidence='Native team/session/save contracts; no Unreal IO'}
    Feedback=@{Script='testar-regras.ps1';Arguments=@('-Suite','Feedback');Evidence='Native action feedback; no graphical acceptance'}
    LabQaPlan=@{Script='verificar-lab-qa.ps1';Arguments=@();Evidence='QA plan schema only; no manual acceptance'}
    Stage=@{Script='verificar-stage-unreal.ps1';Arguments=@();Evidence='Existing stage hashes/includes; no cook/build'}
}
$smoke=Get-Content -LiteralPath (Join-Path $root 'data/qa/smoke.json') -Raw | ConvertFrom-Json
if($smoke.schemaVersion -ne 1 -or @($smoke.cases).Count -ne @($smoke.cases.id | Select-Object -Unique).Count){throw 'Invalid smoke schema/duplicate ID'}
foreach($case in $smoke.cases){if($case.status -notin @('Pending','Deferred','ProvisionalUser','AcceptedUser') -or !$case.evidence){throw 'Invalid smoke status/evidence'}}
$selected=@($Areas | Select-Object -Unique)
$checks=@();foreach($area in $selected){$item=$map[$area];$path=Join-Path $PSScriptRoot $item.Script;if(!(Test-Path -LiteralPath $path -PathType Leaf)){throw 'Verification script missing'};$checks+=@{Area=$area;Script=('scripts/'+$item.Script);Arguments=$item.Arguments;Evidence=$item.Evidence;Status='Planned';ExitCode=$null}}
$mode=if($Execute){'Execute'}else{'PlanOnly'}
$logRoot=Join-Path $root ('build/qa/'+[Guid]::NewGuid().ToString('N').Substring(0,8))
if($Execute){
    $shell=Join-Path $PSHOME 'pwsh.exe'
    if(!(Test-Path -LiteralPath $shell -PathType Leaf)){throw 'PowerShell 7 runtime required; nothing is installed'}
    New-Item -ItemType Directory -Force -Path $logRoot | Out-Null
    foreach($check in $checks){
        $watch=[Diagnostics.Stopwatch]::StartNew();$path=Join-Path $root $check.Script
        # Separate process supplies a real script exit code, independent of stale native LASTEXITCODE.
        $arguments=@($check.Arguments)
        $output=@(& $shell -NoLogo -NoProfile -NonInteractive -File $path @arguments 2>&1 | ForEach-Object {$_.ToString()})
        $code=$LASTEXITCODE;$watch.Stop()
        $log=Join-Path $logRoot ($check.Area+'.log');$output | Set-Content $log -Encoding utf8
        $check.ExitCode=$code;$check.Status=if($code -eq 0){'Passed'}else{'Failed'};$check.Seconds=[Math]::Round($watch.Elapsed.TotalSeconds,2);$check.Log=[IO.Path]::GetRelativePath($root,$log)
        Write-Output ("QA_AREA {0} status={1} exit={2}" -f $check.Area,$check.Status,$code)
    }
}
$failed=@($checks | Where-Object Status -eq 'Failed')
$result=[ordered]@{Date='2026-11-22';Mode=$mode;Passed=if($Execute){$failed.Count -eq 0}else{$null};Checks=$checks;SmokeCases=$smoke.cases;SmokeAutomaticallyAccepted=$false;EditorOpened=$false;CookExecuted=$false;AndroidExecuted=$false;InternetUsed=$false;Note='Native/file evidence only; not a graphical, platform, performance or product acceptance'}
$name=if($Execute){'AUT-16-EXECUCAO.json'}else{'AUT-16-PLANO.json'}
$result | ConvertTo-Json -Depth 8 | Set-Content (Join-Path $root ('docs/relatorios/atuais/'+$name)) -Encoding utf8
if($failed.Count){throw 'One or more selected verifications failed; see per-area logs'}
if(!$Execute){Write-Output ('QA_PLAN_ONLY areas='+($selected -join ','))}
