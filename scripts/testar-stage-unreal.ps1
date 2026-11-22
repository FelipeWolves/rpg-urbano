# Negative verification fixtures only; does not edit a prepared stage.
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$report=Join-Path $root 'docs/relatorios/atuais/ENG-01-PACOTE-PREPARACAO.json'
$json=Get-Content -LiteralPath $report -Raw
$build=Join-Path $root 'build/aut15-verification'
New-Item -ItemType Directory -Force -Path $build | Out-Null
$results=@()
try{
    foreach($case in @('boundary','hash')){
        $copy=$json | ConvertFrom-Json
        if($case -eq 'boundary'){$copy.SourceManifest[0].StagedRelative='../../outside'}else{$copy.SourceManifest[0].SHA256='0'*64}
        $path=Join-Path $build ($case+'.json');$copy | ConvertTo-Json -Depth 12 | Set-Content $path -Encoding utf8
        $caught=$false
        try{& (Join-Path $PSScriptRoot 'verificar-stage-unreal.ps1') -PreparationReport $path | Out-Null}catch{if(($case -eq 'boundary' -and $_.Exception.Message -like '*escapes its boundary*') -or ($case -eq 'hash' -and $_.Exception.Message -like '*hash mismatch*')){$caught=$true}else{throw}}
        if(!$caught){throw "Negative fixture accepted: $case"}
        $results+=$case
    }
}finally{
    # Restore the evidence for the actual stage even after a negative assertion fails.
    & (Join-Path $PSScriptRoot 'verificar-stage-unreal.ps1') -PreparationReport $report
}
[ordered]@{Date='2026-11-22';Passed=$results.Count -eq 2;Groups=3;Cases=@('valid stage','boundary rejected','hash mismatch rejected');StageModified=$false;BuildExecuted=$false;CookExecuted=$false;EditorOpened=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/AUT-15-TESTES.json') -Encoding utf8
Write-Output 'AUT15_TESTS_RESULT passed=3 total=3'
