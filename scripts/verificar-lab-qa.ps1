$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$plan=Get-Content (Join-Path $root 'data/qa/lab-acceptance.json') -Raw | ConvertFrom-Json
if($plan.schemaVersion -ne 1 -or @($plan.cases).Count -ne 16){throw 'Invalid QA schema/count'}
if(@($plan.cases.id | Select-Object -Unique).Count -ne 16){throw 'Duplicate QA ID'}
foreach($case in $plan.cases){
 foreach($field in @('id','area','route','steps','expected','status','evidence','dependency')){if(!$case.$field){throw "Missing QA field $field"}}
 if($case.status -notin @('Pending','Deferred','ProvisionalUser','AcceptedUser')){throw 'Invalid manual status'}
}
foreach($id in @('LQA-14','LQA-16')){if(($plan.cases | Where-Object id -eq $id).status -ne 'Deferred'){throw 'Isolated IO and Android remain deferred'}}
$csv=@(Import-Csv (Join-Path $root 'docs/relatorios/atuais/LAB-QA-01-MATRIZ.csv'))
if(($csv.id -join ',') -ne ($plan.cases.id -join ',')){throw 'CSV case mismatch'}
[ordered]@{Date='2026-11-22';Scope='Schema and plan only';Passed=$true;Cases=16;ManualAutomaticallyAccepted=0;EditorOpened=$false;GameExecuted=$false;AndroidExecuted=$false;RealSaveIOTested=$false} | ConvertTo-Json | Set-Content (Join-Path $root 'docs/relatorios/atuais/LAB-QA-01-PLANO.json') -Encoding utf8
Write-Output 'LAB_QA_PLAN_RESULT cases=16 schema=passed manual_acceptance=0'
