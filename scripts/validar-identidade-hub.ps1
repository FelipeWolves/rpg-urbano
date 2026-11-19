# Verifies local resources only. Never downloads, installs or executes media.
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$manifest=Get-Content (Join-Path $root 'data/presentation/hub-resources.json') -Raw|ConvertFrom-Json
foreach($resource in $manifest.Resources){
    if($resource.Path -notmatch '^assets/runtime/(fonts|hub)/[A-Za-z0-9.-]+$'){throw 'Invalid local resource path'}
    $file=Join-Path $root $resource.Path
    if(!(Test-Path -LiteralPath $file -PathType Leaf)){throw ('Missing resource: '+$resource.Path)}
    if((Get-FileHash -LiteralPath $file -Algorithm SHA256).Hash -ne $resource.SHA256){throw ('Resource hash mismatch: '+$resource.Path)}
    if((Get-Item -LiteralPath $file).Length -ne $resource.Bytes){throw 'Resource length mismatch'}
    if($resource.Path.EndsWith('.ttf')){
        $bytes=[IO.File]::ReadAllBytes($file)
        if($bytes.Length -lt 12 -or [BitConverter]::ToString($bytes[0..3]) -ne '00-01-00-00'){throw 'Expected TrueType signature'}
    }
    if($resource.Path.EndsWith('OFL.txt') -and (Get-Content $file -Raw) -notmatch 'SIL OPEN FONT LICENSE'){throw 'Missing font license'}
}
'LOCAL_HUB_RESOURCES_OK count='+$manifest.Resources.Count
