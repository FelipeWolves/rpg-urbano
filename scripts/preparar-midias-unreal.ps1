# Copies only the two known authored intro variants into the stable Unreal project.
# Never starts a player/editor, downloads media or edits source videos.
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$destination=Join-Path $root 'experiments/unreal/RpgEngineProbe/Content/Presentation'
$boundary=[IO.Path]::GetFullPath($destination)+[IO.Path]::DirectorySeparatorChar
$results=@()
foreach($name in @('intro-desk.mp4','intro-mobile.mp4')){
    $relative='assets/runtime/video/'+$name
    $source=Join-Path $root $relative
    $target=[IO.Path]::GetFullPath((Join-Path $destination $relative))
    if(!$target.StartsWith($boundary,[StringComparison]::OrdinalIgnoreCase)){throw 'Invalid media destination'}
    if(!(Test-Path -LiteralPath $source -PathType Leaf)){throw "Missing authored intro: $name"}
    $length=(Get-Item -LiteralPath $source).Length
    if($length -lt 1){throw 'Empty intro'}
    if(Test-Path -LiteralPath $target){throw "Destination already exists; preserve and review before replacing: $name"}
    $results+=@{Source=$source;Target=$target;Bytes=$length}
}
foreach($item in $results){New-Item -ItemType Directory -Force -Path (Split-Path $item.Target -Parent) | Out-Null;Copy-Item -LiteralPath $item.Source -Destination $item.Target;if((Get-Item -LiteralPath $item.Target).Length -ne $item.Bytes){throw 'Copy length mismatch'}}
$results | Select-Object @{n='Name';e={Split-Path $_.Source -Leaf}},Bytes | ConvertTo-Json
