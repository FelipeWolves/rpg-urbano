# Experimento de contrato; nao integra Battle nem reproduz formulas internas de jogos.
$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
$root = Split-Path $PSScriptRoot -Parent
$threshold = [long]1000000
function New-Clock($ids = @('A','B'), $speeds = @(200,100)) {
    if ($ids.Count -eq 0 -or $ids.Count -ne $speeds.Count) { throw 'Invalid roster' }
    $units = @()
    for ($i=0; $i -lt $ids.Count; $i++) {
        if ([string]::IsNullOrWhiteSpace($ids[$i]) -or $speeds[$i] -lt 1 -or $speeds[$i] -gt 10000 -or
            @($units | Where-Object Id -eq $ids[$i]).Count) { throw 'Invalid unit' }
        $units += [pscustomobject]@{ Id=$ids[$i]; Speed=[long]$speeds[$i]; AP=[long]0; Alive=$true; Turns=0; Cooldown=3; Duration=3 }
    }
    return @{ Units=$units; Time=[long]0; Natural=0; Actions=0 }
}
function Next-Natural($clock) {
    $living = @($clock.Units | Where-Object Alive)
    if ($living.Count -eq 0) { throw 'No living units' }
    $ready = @($living | Where-Object { $_.AP -ge $threshold })
    if ($ready.Count -eq 0) {
        $delta = [long]::MaxValue
        foreach ($unit in $living) {
            $wait = [long][Math]::Ceiling(($threshold-$unit.AP)/[double]$unit.Speed)
            $delta = [Math]::Min($delta, $wait)
        }
        foreach ($unit in $living) { $unit.AP += $unit.Speed*$delta }
        $clock.Time += $delta
        $ready = @($living | Where-Object { $_.AP -ge $threshold })
    }
    # Proposed order: AP descending, SPD descending, ordinal ID. Reset discards overflow.
    $selected = $ready[0]
    foreach ($unit in $ready) {
        if ($unit.AP -gt $selected.AP -or ($unit.AP -eq $selected.AP -and
            ($unit.Speed -gt $selected.Speed -or ($unit.Speed -eq $selected.Speed -and
             [string]::CompareOrdinal($unit.Id,$selected.Id) -lt 0)))) { $selected=$unit }
    }
    $selected.AP=0
    return $selected
}
function Complete-Opportunity($clock,$unit,[string]$kind,[bool]$tickCooldown,[bool]$tickDuration) {
    if ($kind -notin @('Natural','Extra','Reaction')) { throw 'Unknown opportunity' }
    $clock.Actions++
    if ($kind -eq 'Natural') { $clock.Natural++ }
    if ($kind -ne 'Reaction') { $unit.Turns++ }
    if ($tickCooldown) { $unit.Cooldown=[Math]::Max(0,$unit.Cooldown-1) }
    if ($tickDuration) { $unit.Duration=[Math]::Max(0,$unit.Duration-1) }
}
function Change-AP($unit,[long]$delta) { $unit.AP=[Math]::Clamp($unit.AP+$delta,[long]0,$threshold) }
function Assert($condition,[string]$message) { if (-not $condition) { throw $message } }
function Trace($clock,[int]$count) {
    $rows=@()
    for($i=0;$i -lt $count;$i++) {
        $unit=Next-Natural $clock
        Complete-Opportunity $clock $unit 'Natural' $true $true
        $rows += [pscustomobject]@{ Index=$i+1; Actor=$unit.Id; LogicalTime=$clock.Time; Natural=$clock.Natural; Actions=$clock.Actions }
    }
    return $rows
}
$results=[Collections.Generic.List[object]]::new()
function Test([string]$id,[scriptblock]$body) {
    try { & $body; $results.Add([pscustomobject]@{Id=$id;Status='pass';Error=$null}) }
    catch { $results.Add([pscustomobject]@{Id=$id;Status='fail';Error=$_.Exception.Message}) }
}
Test 'spd_frequency' {
    $trace=@(Trace (New-Clock) 30)
    Assert ((@($trace|Where-Object Actor -eq 'A').Count -eq 20) -and (@($trace|Where-Object Actor -eq 'B').Count -eq 10)) 'SPD frequency wrong'
}
Test 'ordinal_tie_and_replay' {
    $a=@(Trace (New-Clock @('A','B') @(100,100)) 12)
    $b=@(Trace (New-Clock @('B','A') @(100,100)) 12)
    Assert (($a|ConvertTo-Json -Compress) -ceq ($b|ConvertTo-Json -Compress)) 'Input order changed replay'
    Assert ($a[0].Actor -ceq 'A') 'Ordinal tie wrong'
}
Test 'ap_push' {
    $c=New-Clock; Change-AP $c.Units[1] 1000000
    Assert ((Next-Natural $c).Id -eq 'B' -and $c.Time -eq 0) 'AP boost not immediate'
}
Test 'ap_down_clamp' {
    $c=New-Clock; Change-AP $c.Units[0] 900000; Change-AP $c.Units[0] -2000000
    Assert ($c.Units[0].AP -eq 0) 'AP underflow'
    Change-AP $c.Units[1] 2000000; Assert ($c.Units[1].AP -eq 1000000) 'AP overflow'
}
Test 'speed_change' {
    $c=New-Clock; $null=Next-Natural $c; $c.Units[1].Speed=1000
    Assert ((Next-Natural $c).Id -eq 'B') 'SPD change not used'
}
Test 'extra_turn_separate_clocks' {
    $c=New-Clock; $u=Next-Natural $c; Complete-Opportunity $c $u 'Natural' $true $true
    $ap=$c.Units[1].AP; $time=$c.Time
    Complete-Opportunity $c $u 'Extra' $false $true
    Assert ($c.Natural -eq 1 -and $c.Actions -eq 2 -and $u.Turns -eq 2 -and $u.Cooldown -eq 2 -and $u.Duration -eq 1) 'Extra tick conflated'
    Assert ($c.Time -eq $time -and $c.Units[1].AP -eq $ap) 'Extra advanced passive AP'
}
Test 'reaction_not_turn' {
    $c=New-Clock; $u=$c.Units[1]; Complete-Opportunity $c $u 'Reaction' $false $false
    Assert ($c.Actions -eq 1 -and $c.Natural -eq 0 -and $u.Turns -eq 0 -and $u.Cooldown -eq 3 -and $u.Duration -eq 3) 'Reaction consumed turn clock'
}
Test 'owner_only_ticks' {
    $c=New-Clock; $u=Next-Natural $c; Complete-Opportunity $c $u 'Natural' $true $true
    Assert ($c.Units[1].Cooldown -eq 3 -and $c.Units[1].Duration -eq 3) 'Other owner clock changed'
}
Test 'dead_not_scheduled' {
    $c=New-Clock; $c.Units[0].Alive=$false
    Assert ((Next-Natural $c).Id -eq 'B' -and $c.Units[0].AP -eq 0) 'Dead scheduled'
}
Test 'invalid_speed' {
    foreach($speed in @(0,-1,10001)) {
        $rejected=$false; try { $null=New-Clock @('A') @($speed) } catch { $rejected=$true }
        Assert $rejected 'Invalid SPD accepted'
    }
}
Test 'cooldown_saturation' {
    $c=New-Clock
    for($i=0;$i -lt 8;$i++) { Complete-Opportunity $c $c.Units[0] 'Extra' $true $true }
    Assert ($c.Units[0].Cooldown -eq 0 -and $c.Units[0].Duration -eq 0) 'Negative duration'
}
$destination=Join-Path $root 'docs\batalhas'
$trace=@(Trace (New-Clock) 30)
$trace|Export-Csv -LiteralPath (Join-Path $destination 'tempo-experimento.csv') -NoTypeInformation -Encoding utf8
$failed=@($results|Where-Object Status -eq 'fail')
[ordered]@{
    CheckedUtc=[DateTime]::UtcNow.ToString('o'); Scope='Proposed isolated clock experiment; not Dislyte parity or Battle integration';
    ModelVersion=1; Threshold=$threshold; Passed=$results.Count-$failed.Count; Total=$results.Count; Tests=@($results.ToArray());
    Policies=@{Overflow='discard_on_natural_turn';Tie='AP_desc_SPD_desc_ordinal_ID';GlobalExample='natural_opportunities_only_not_official_rule';Precision='integer_discrete_tick_proposal_not_official_formula'}
}|ConvertTo-Json -Depth 6|Set-Content -LiteralPath (Join-Path $destination 'tempo-experimento.json') -Encoding utf8
Write-Output "CLOCK_EXPERIMENT passed=$($results.Count-$failed.Count) total=$($results.Count)"
if($failed.Count) { $failed|Format-Table; throw 'Clock experiment failed' }
