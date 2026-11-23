# Read-only validation: runtime copies, bounded dimensions, and Portuguese glyphs in the bundled label font.
param([string]$EngineRoot='D:/Unreal Engine/UE_5.8')
$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$root=Split-Path $PSScriptRoot -Parent
$catalog=Get-Content (Join-Path $root 'data/presentation/ui-runtime.json') -Raw | ConvertFrom-Json
if($catalog.SchemaVersion -ne 1 -or @($catalog.Resources).Count -ne 19){throw 'Unexpected UI catalog'}
$rawBytes=0L
foreach($item in $catalog.Resources){$path=Join-Path $root $item.Path;$copy=Join-Path $root ('experiments/unreal/RpgEngineProbe/Content/Presentation/'+$item.Path);foreach($file in @($path,$copy)){if((Get-FileHash -LiteralPath $file).Hash -ne $item.SHA256){throw 'Runtime resource hash mismatch'}};$image=[Drawing.Image]::FromFile($path);try{if($image.Width -ne $item.Width -or $image.Height -ne $item.Height -or $image.Width -gt 512){throw 'Runtime dimensions mismatch'};$rawBytes += [long]$image.Width*$image.Height*4}finally{$image.Dispose()}}
$font=Join-Path $EngineRoot 'Engine/Content/Slate/Fonts/Roboto-Light.ttf'
$bytes=[IO.File]::ReadAllBytes($font)
function ReadU16([int]$at){return [int]$bytes[$at]*256+[int]$bytes[$at+1]}
function ReadU32([int]$at){return [long]$bytes[$at]*16777216+[long]$bytes[$at+1]*65536+[long]$bytes[$at+2]*256+[long]$bytes[$at+3]}
$cmap=0;for($i=0;$i -lt (ReadU16 4);$i++){$at=12+$i*16;if([Text.Encoding]::ASCII.GetString($bytes,$at,4) -eq 'cmap'){$cmap=[int](ReadU32 ($at+8));break}}
if(!$cmap){throw 'No Unicode font map'}
$format4=0;for($i=0;$i -lt (ReadU16 ($cmap+2));$i++){$at=$cmap+4+$i*8;$platform=ReadU16 $at;$sub=$cmap+[int](ReadU32 ($at+4));if((ReadU16 $sub) -eq 4 -and $platform -in @(0,3)){$format4=$sub;break}}
if(!$format4){throw 'Expected Unicode cmap format4'}
$glyphs=@();foreach($code in @(0x00C7,0x00C3,0x00D5,0x00C9)){$segments=(ReadU16 ($format4+6))/2;$glyph=0;for($i=0;$i -lt $segments;$i++){$end=ReadU16 ($format4+14+$i*2);$start=ReadU16 ($format4+16+$segments*2+$i*2);if($code -lt $start -or $code -gt $end){continue};$delta=ReadU16 ($format4+16+$segments*4+$i*2);$rangeAt=$format4+16+$segments*6+$i*2;$range=ReadU16 $rangeAt;if($range -eq 0){$glyph=($code+$delta)%65536}else{$glyph=ReadU16 ($rangeAt+$range+2*($code-$start));if($glyph){$glyph=($glyph+$delta)%65536}};break};if(!$glyph){throw ('Missing uppercase glyph '+$code)};$glyphs+=[ordered]@{Character=[string][char]$code;Glyph=$glyph}}
[ordered]@{Date='2026-11-23';Passed=$true;Textures=19;RuntimeRawRGBABytes=$rawBytes;LabelFont='Engine Roboto Light';FontSHA256=(Get-FileHash $font).Hash;Glyphs=$glyphs;OriginalsPreserved=$true;EditorOpened=$false;VisualAcceptance=$false} | ConvertTo-Json -Depth 6 | Set-Content (Join-Path $root 'docs/relatorios/atuais/UI-INTEGRACAO-2026-11-23-RECURSOS.json') -Encoding utf8
Write-Output ('UI_RUNTIME_VERIFIED textures=19 uppercase_glyphs=4 raw_rgba_bytes='+$rawBytes)
