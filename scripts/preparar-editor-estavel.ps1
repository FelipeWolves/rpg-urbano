# Populate missing local presentation resources in the stable Unreal source project.
# Does not build, launch, download or change SDK preferences; existing differing files are preserved.
param()
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$destination=Join-Path $root 'experiments/unreal/RpgEngineProbe/Content/Presentation'
$catalog=Get-Content -LiteralPath (Join-Path $root 'data/presentation/startup.json') -Raw | ConvertFrom-Json
$panorama=@($catalog.assets | Where-Object role -eq 'HubPanorama')
if($panorama.Count -ne 1){throw 'Expected one hub panorama'}
$paths=@('data/presentation/startup.json',[string]$panorama[0].path,'assets/runtime/hub/logo-lobo.png','assets/runtime/fonts/CormorantGaramond.ttf','assets/runtime/fonts/CormorantGaramond-OFL.txt','assets/runtime/fonts/Lora.ttf','assets/runtime/fonts/Lora-OFL.txt')
$plan=@()
foreach($relative in $paths){
    if($relative -match '[\\:"?*<>|]' -or $relative.Split('/') -contains '..' -or !($relative.StartsWith('assets/runtime/') -or $relative -eq 'data/presentation/startup.json')){throw 'Unsafe resource path'}
    $source=Join-Path $root $relative
    if(!(Test-Path -LiteralPath $source -PathType Leaf)){throw "Missing local resource: $relative"}
    $target=Join-Path $destination $relative
    $hash=(Get-FileHash -LiteralPath $source).Hash
    if((Test-Path -LiteralPath $target) -and (Get-FileHash -LiteralPath $target).Hash -ne $hash){throw "Existing resource differs; review manually, preserved: $relative"}
    $plan+=@{Source=$source;Target=$target;Hash=$hash}
}
foreach($item in $plan){
    if(!(Test-Path -LiteralPath $item.Target)){
        New-Item -ItemType Directory -Force -Path (Split-Path $item.Target -Parent) | Out-Null
        Copy-Item -LiteralPath $item.Source -Destination $item.Target
    }
    if((Get-FileHash -LiteralPath $item.Target).Hash -ne $item.Hash){throw 'Resource copy hash mismatch'}
}
Write-Output "STABLE_RESOURCES_READY count=$($plan.Count)"
