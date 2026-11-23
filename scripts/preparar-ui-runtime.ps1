# Derive bounded-size runtime UI textures locally; keeps full-resolution prepared originals.
$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$root=Split-Path $PSScriptRoot -Parent
$catalog=Get-Content (Join-Path $root 'data/presentation/ui-assets-2026-11-22.json') -Raw | ConvertFrom-Json
$records=@()
foreach($item in $catalog.Resources){
 $source=Join-Path $root $item.Path;if((Get-FileHash $source).Hash -ne $item.SHA256){throw 'Prepared source changed'}
 $relative=$item.Path.Replace('2026-11-22-v02','2026-11-23-runtime-v01');$destination=Join-Path $root $relative
 $size=if($item.Id -like 'ico_status*'){128}elseif($item.Id -like 'ico_skill*'){512}else{256}
 if(Test-Path $destination){throw 'Runtime derivative exists; preserve version'}
 New-Item -ItemType Directory -Force -Path (Split-Path $destination -Parent) | Out-Null
 $original=[Drawing.Image]::FromFile($source);$bitmap=[Drawing.Bitmap]::new($size,$size,[Drawing.Imaging.PixelFormat]::Format32bppArgb);$g=[Drawing.Graphics]::FromImage($bitmap)
 try{$g.Clear([Drawing.Color]::Transparent);$g.CompositingMode=[Drawing.Drawing2D.CompositingMode]::SourceCopy;$g.InterpolationMode=[Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic;$g.PixelOffsetMode=[Drawing.Drawing2D.PixelOffsetMode]::HighQuality;$g.DrawImage($original,[Drawing.Rectangle]::new(0,0,$size,$size),0,0,$original.Width,$original.Height,[Drawing.GraphicsUnit]::Pixel);$bitmap.Save($destination,[Drawing.Imaging.ImageFormat]::Png)}finally{$g.Dispose();$bitmap.Dispose();$original.Dispose()}
 $records+=[ordered]@{Id=$item.Id;Source=$item.Path;SourceSHA256=$item.SHA256;Path=$relative;Width=$size;Height=$size;SHA256=(Get-FileHash $destination).Hash;Cropped=$false;Status='RuntimeReadyNotVisuallyAccepted'}
}
[ordered]@{SchemaVersion=1;Date='2026-11-23';Resources=$records;Note='Runtime variants retain full canvas and aspect; full-size prepared images and originals preserved'} | ConvertTo-Json -Depth 8 | Set-Content (Join-Path $root 'data/presentation/ui-runtime.json') -Encoding utf8
$presentation=Join-Path $root 'experiments/unreal/RpgEngineProbe/Content/Presentation'
foreach($relative in @('data/presentation/ui-runtime.json')+@($records.Path)){$target=Join-Path $presentation $relative;New-Item -ItemType Directory -Force -Path (Split-Path $target -Parent) | Out-Null;Copy-Item -LiteralPath (Join-Path $root $relative) -Destination $target;if((Get-FileHash (Join-Path $root $relative)).Hash -ne (Get-FileHash $target).Hash){throw 'Presentation copy mismatch'}}
Write-Output ('UI_RUNTIME_READY count='+$records.Count)
