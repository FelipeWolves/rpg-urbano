# Validates local presentation metadata only; never opens a player/editor or executes media.
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$catalogPath=Join-Path $root 'data/presentation/startup.json'
$c=Get-Content -LiteralPath $catalogPath -Raw | ConvertFrom-Json
function Assert-Dimension($value) {
    if($null -eq $value -or $value -isnot [ValueType] -or $value -is [bool]){throw 'Numeric pixel dimension required'}
    $number=[double]$value
    if(![double]::IsFinite($number) -or $number -le 0 -or $number -gt [uint32]::MaxValue -or $number -ne [Math]::Floor($number)){throw 'Positive integer pixel dimension required'}
}
if($c.schemaVersion -ne 1){throw 'Unknown presentation schema'}
if($c.desktopBeforeWindowCreation.mode -notin @('Windowed','Fullscreen')){throw 'Unknown window mode'}
Assert-Dimension $c.desktopBeforeWindowCreation.width
Assert-Dimension $c.desktopBeforeWindowCreation.height
if($c.hub.initialArea -notin @('Left','Center','Right') -or $c.hub.view -notin @('Auto','Focused','Panoramic')){throw 'Unknown hub selection'}
if(![double]::IsFinite([double]$c.hub.panoramicMinAspect) -or $c.hub.panoramicMinAspect -le 0){throw 'Positive aspect threshold required'}
foreach($name in @('Left','Center','Right')){
    $region=$c.hub.regions.$name
    foreach($key in @('x','y','width','height')){
        if($null -eq $region.$key -or ![double]::IsFinite([double]$region.$key)){throw 'Finite hub region required'}
    }
    if($region.x -lt 0 -or $region.y -lt 0 -or $region.width -le 0 -or $region.height -le 0 -or
       ($region.x+$region.width) -gt 1 -or ($region.y+$region.height) -gt 1){throw 'Hub region outside image'}
}
$hotspotIds=@()
foreach($spot in $c.hub.hotspots){
    if([string]::IsNullOrWhiteSpace($spot.id) -or $spot.id -in $hotspotIds -or [string]::IsNullOrWhiteSpace($spot.label)){throw 'Unique hotspot IDs and labels required'}
    if($spot.route -notin @('SpecialEventPlaceholder','ReferenceBattle','Terminal','Unavailable') -or $spot.visible -isnot [bool]){throw 'Unknown provisional hub route or visibility'}
    foreach($key in @('x','y')){
        if($null -eq $spot.anchor.$key -or ![double]::IsFinite([double]$spot.anchor.$key) -or $spot.anchor.$key -lt 0 -or $spot.anchor.$key -gt 1){throw 'Hotspot anchor outside source image'}
    }
    $area=$spot.hitArea
    foreach($key in @('x','y','width','height')){
        if($null -eq $area.$key -or ![double]::IsFinite([double]$area.$key)){throw 'Finite hotspot area required'}
    }
    if($area.x -lt 0 -or $area.y -lt 0 -or $area.width -le 0 -or $area.height -le 0 -or
       ($area.x+$area.width) -gt 1 -or ($area.y+$area.height) -gt 1){throw 'Hotspot hit area outside source image'}
    if($spot.anchor.x -lt $area.x -or $spot.anchor.y -lt $area.y -or
       $spot.anchor.x -gt ($area.x+$area.width) -or $spot.anchor.y -gt ($area.y+$area.height)){throw 'Hotspot anchor outside its hit area'}
    $hotspotIds+=$spot.id
}
if($null -ne $c.hub.visual){
    $visual=$c.hub.visual
    if([string]::IsNullOrWhiteSpace($visual.title) -or [string]::IsNullOrWhiteSpace($visual.codename)){throw 'Hub title and codename required'}
    foreach($name in @('magenta','violet','cyan','cyanBright','amber','gold','crimson','panel')){
        if($visual.palette.$name -notmatch '^#[0-9a-fA-F]{6}$'){throw 'RGB hex palette required'}
    }
    if($visual.reducedMotion -isnot [bool] -or $visual.palette.panelOpacity -lt 0 -or $visual.palette.panelOpacity -gt 1){throw 'Invalid motion/opacity preference'}
    $pulse=$visual.battlePulse
    foreach($name in @('period','minimumScale','maximumScale','ringMaximumScale','ringOpacity','ringStroke','minimumTarget')){
        if($null -eq $pulse.$name -or ![double]::IsFinite([double]$pulse.$name)){throw 'Finite pulse settings required'}
    }
    if($pulse.period -le 0 -or $pulse.minimumScale -le 0 -or $pulse.maximumScale -lt $pulse.minimumScale -or
       $pulse.ringMaximumScale -lt 1 -or $pulse.ringOpacity -lt 0 -or $pulse.ringOpacity -gt 1 -or
       $pulse.ringStroke -le 0 -or $pulse.minimumTarget -lt 48){throw 'Invalid pulse/minimum target policy'}
    if([string]::IsNullOrWhiteSpace($visual.eventPanel.title) -or [string]::IsNullOrWhiteSpace($visual.eventPanel.body)){throw 'Event panel text required'}
}
if($c.video.fit -ne 'Contain' -or $c.video.fallback -ne 'ContinueToLocalEntry' -or $c.video.variantPreference -ne 'DeviceThenOther'){throw 'Unsupported video policy'}
foreach($key in @('mobileRequestMatchingOrientation','restoreHubOrientationAfterPlayback','desktopPortraitSideComposition','allowSkip')){
    if($c.video.$key -isnot [bool]){throw 'Boolean video policy required'}
}
$ids=@();$roles=@();$assets=@()
$boundary=[IO.Path]::GetFullPath((Join-Path $root 'assets'))+[IO.Path]::DirectorySeparatorChar
foreach($asset in $c.assets){
    if([string]::IsNullOrWhiteSpace($asset.id) -or $asset.id -in $ids){throw 'Unique asset ID required'}
    if($asset.role -notin @('HubPanorama','IntroDesktop','IntroMobile','SideArt') -or $asset.role -in $roles){throw 'Unique supported role required'}
    if($asset.status -notin @('Planned','Ready')){throw 'Unknown asset status'}
    $path=[string]$asset.path
    if(!$path.StartsWith('assets/') -or $path -match '[\\:"?*<>|#\x00-\x1F]' -or $path -match '(^|/)(\.{1,2}|)(/|$)' -or $path -match '[ .](/|$)'){throw 'Project-relative asset path required'}
    $absolute=[IO.Path]::GetFullPath((Join-Path $root $path))
    if(!$absolute.StartsWith($boundary,[StringComparison]::OrdinalIgnoreCase)){throw 'Asset outside project assets'}
    Assert-Dimension $asset.width
    Assert-Dimension $asset.height
    $exists=$false
    if($asset.status -eq 'Ready'){
        if([string]::IsNullOrWhiteSpace($asset.origin) -or [string]::IsNullOrWhiteSpace($asset.rights)){throw 'Ready asset needs origin and rights metadata'}
        $current=$absolute
        while($current.Length -gt $root.Length){
            if(Test-Path -LiteralPath $current){
                if((Get-Item -LiteralPath $current -Force).Attributes -band [IO.FileAttributes]::ReparsePoint){throw 'Asset links are not imported by this validator'}
            }
            $current=Split-Path $current -Parent
        }
        if(!(Test-Path -LiteralPath $absolute -PathType Leaf)){throw 'Ready asset file missing'}
        $exists=$true
    }
    $ids+=$asset.id;$roles+=$asset.role
    $assets += [ordered]@{Id=$asset.id;Role=$asset.role;Status=$asset.status;FilePresenceChecked=($asset.status -eq 'Ready');FileExists=$exists;DimensionsDeclared=@($asset.width,$asset.height)}
}
foreach($role in @('HubPanorama','IntroDesktop','IntroMobile','SideArt')){if($role -notin $roles){throw 'Catalog role missing'}}
$report=[ordered]@{CheckedUtc=[DateTime]::UtcNow.ToString('o');Passed=$true;SchemaVersion=1;Catalog='data/presentation/startup.json';Assets=$assets;HotspotIds=$hotspotIds;HotspotRuntimeConnected=$false;MediaDecoded=$false;DimensionsReadFromFile=$false;EngineExecuted=$false;NetworkAccessed=$false;Scope='Catalog metadata and presence of declared Ready local files only; no runtime import/playback or license verification'}
$report | ConvertTo-Json -Depth 6 | Set-Content (Join-Path $root 'docs/relatorios/atuais/HUB-01A-CATALOGO.json') -Encoding utf8
'PRESENTATION_CATALOG_OK assets='+$assets.Count+' ready='+@($assets | Where-Object Status -eq 'Ready').Count
