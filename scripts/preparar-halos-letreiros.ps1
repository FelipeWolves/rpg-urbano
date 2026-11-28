# Generate blurred alpha masks of the actual label glyphs, locally; no source images altered.
param([string]$EngineRoot='D:/Unreal Engine/UE_5.8',[switch]$Terminal)
$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$root=Split-Path $PSScriptRoot -Parent
$font=Join-Path $EngineRoot 'Engine/Content/Slate/Fonts/Roboto-Light.ttf'
$refs=@(Get-ChildItem $PSHOME -File | Where-Object {$_.Name -match '^(System.Drawing.*|System.Private.Windows.*|System.ComponentModel.Primitives)\.dll$'} | ForEach-Object FullName)
Add-Type -ReferencedAssemblies $refs -TypeDefinition @"
using System;
using System.Drawing;
using System.Drawing.Text;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
public static class LabelBlurMask {
 public static int[] Create(string fontPath,string text,string destination){
 return Create(fontPath,text,destination,null,false);
 }
 public static int[] Create(string fontPath,string text,string destination,string face,bool outline){
 using(var collection=new PrivateFontCollection()){
 collection.AddFontFile(fontPath);
 using(var font=new Font(collection.Families[0],48,FontStyle.Regular,GraphicsUnit.Pixel))
 using(var probe=new Bitmap(1,1))using(var measure=Graphics.FromImage(probe))
 using(var format=(StringFormat)StringFormat.GenericTypographic.Clone()){
 format.FormatFlags|=StringFormatFlags.NoWrap;
 var bounds=measure.MeasureString(text,font,4096,format);int lw=(int)Math.Ceiling(bounds.Width),lh=(int)Math.Ceiling(font.GetHeight(measure)),pad=24,w=lw+pad*2,h=lh+pad*2;
 if(w>(face==null?512:1024)||h>512||lw<1||lh<1)throw new Exception("Invalid halo geometry");
 using(var image=new Bitmap(w,h,PixelFormat.Format32bppArgb)){
 using(var g=Graphics.FromImage(image)){
 g.Clear(Color.Transparent);
 if(face==null){g.TextRenderingHint=TextRenderingHint.AntiAliasGridFit;g.DrawString(text,font,Brushes.White,new PointF(pad,pad),format);}
 else{
 g.SmoothingMode=System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
 using(var path=new System.Drawing.Drawing2D.GraphicsPath())
 using(var ink=new SolidBrush(outline?Color.FromArgb(29,25,79):Color.FromArgb(124,248,247)))
 using(var stroke=new Pen(Color.FromArgb(201,180,255),2.2f)){
 path.AddString(text,font.FontFamily,(int)FontStyle.Regular,48,new PointF(pad,pad),format);
 g.FillPath(ink,path);
 if(outline){g.DrawPath(stroke,path);}
 image.Save(face,ImageFormat.Png);
 }
 }
 }
 var locked=image.LockBits(new Rectangle(0,0,w,h),ImageLockMode.ReadWrite,PixelFormat.Format32bppArgb);byte[] pixels=new byte[locked.Stride*h];Marshal.Copy(locked.Scan0,pixels,0,pixels.Length);
 int radius=18;double sigma=6,sum=0;double[] kernel=new double[2*radius+1];for(int i=-radius;i<=radius;i++){double value=Math.Exp(-i*i/(2*sigma*sigma));kernel[i+radius]=value;sum+=value;}for(int i=0;i<kernel.Length;i++)kernel[i]/=sum;
 float[] temp=new float[w*h];for(int y=0;y<h;y++)for(int x=0;x<w;x++){double value=0;for(int i=-radius;i<=radius;i++){int xx=x+i;if(xx>=0&&xx<w)value+=pixels[y*locked.Stride+xx*4+3]*kernel[i+radius];}temp[y*w+x]=(float)value;}
 int alphaMax=0,borderMax=0;for(int y=0;y<h;y++)for(int x=0;x<w;x++){double value=0;for(int i=-radius;i<=radius;i++){int yy=y+i;if(yy>=0&&yy<h)value+=temp[yy*w+x]*kernel[i+radius];}int alpha=(int)Math.Clamp(Math.Round(value*2),0,255),o=y*locked.Stride+x*4;pixels[o]=pixels[o+1]=pixels[o+2]=255;pixels[o+3]=(byte)alpha;alphaMax=Math.Max(alphaMax,alpha);if(x==0||y==0||x==w-1||y==h-1)borderMax=Math.Max(borderMax,alpha);}
 Marshal.Copy(pixels,0,locked.Scan0,pixels.Length);image.UnlockBits(locked);
 if(alphaMax<1||borderMax!=0)throw new Exception("Empty or clipped halo");image.Save(destination,ImageFormat.Png);return new int[]{w,h,lw,lh,pad,alphaMax,borderMax};
 }
 }
 }
 }
}
"@
if($Terminal){
    $font=Join-Path $EngineRoot 'Engine/Content/Slate/Fonts/Roboto-BoldCondensed.ttf'
    $relative='assets/runtime/hub/terminal-neon'
    $local=Join-Path $root $relative
    $presentation=Join-Path $root "experiments/unreal/RpgEngineProbe/Content/Presentation/$relative"
    New-Item -ItemType Directory -Force -Path $local,$presentation | Out-Null
    foreach($sign in @(
        @{Id='title';Text='TERMINAL URBANO';Outline=$true},
        @{Id='subtitle';Text='Destinos e Modos Internos';Outline=$false}
    )){
        $face=$sign.Id+'-face.png'
        $halo=$sign.Id+'-halo.png'
        [LabelBlurMask]::Create($font,$sign.Text,(Join-Path $local $halo),(Join-Path $local $face),$sign.Outline) | Out-Null
        foreach($file in $face,$halo){Copy-Item -LiteralPath (Join-Path $local $file) -Destination (Join-Path $presentation $file)}
    }
    return
}
$startup=Get-Content (Join-Path $root 'data/presentation/startup.json') -Raw | ConvertFrom-Json
$catalog=Get-Content (Join-Path $root 'data/presentation/ui-runtime.json') -Raw | ConvertFrom-Json
if(@($catalog.Resources | Where-Object Id -like 'halo_label_*').Count){throw 'Label halo catalog already exists; preserve version'}
$records=@();foreach($link in $startup.hub.hotspots){if($link.id -notmatch '^[a-z0-9-]+$'){throw 'Invalid label ID'};$relative='assets/runtime/ui/2026-11-23-label-halos-v01/'+$link.id+'.png';$target=Join-Path $root $relative;if(Test-Path $target){throw 'Halo exists; preserve version'};New-Item -ItemType Directory -Force -Path (Split-Path $target -Parent) | Out-Null;$text=$link.label.ToUpperInvariant();$metrics=[LabelBlurMask]::Create($font,$text,$target);$records+=[pscustomobject][ordered]@{Id=('halo_label_'+$link.id);Path=$relative;Width=$metrics[0];Height=$metrics[1];SHA256=(Get-FileHash $target).Hash;Text=$text;LayoutWidth=$metrics[2];LayoutHeight=$metrics[3];Padding=$metrics[4];AlphaMax=$metrics[5];BorderAlphaMax=$metrics[6];GaussianSigma=6;GaussianRadius=18;Font='Engine Roboto Light';FontSHA256=(Get-FileHash $font).Hash;Status='BlurredGlyphMaskNotVisuallyAccepted'}}
$catalog.Resources=@($catalog.Resources)+$records;$catalog | ConvertTo-Json -Depth 9 | Set-Content (Join-Path $root 'data/presentation/ui-runtime.json') -Encoding utf8
$presentation=Join-Path $root 'experiments/unreal/RpgEngineProbe/Content/Presentation'
foreach($relative in @('data/presentation/ui-runtime.json')+@($records.Path)){$target=Join-Path $presentation $relative;New-Item -ItemType Directory -Force -Path (Split-Path $target -Parent) | Out-Null;Copy-Item -LiteralPath (Join-Path $root $relative) -Destination $target;if((Get-FileHash (Join-Path $root $relative)).Hash -ne (Get-FileHash $target).Hash){throw 'Halo copy mismatch'}}
Write-Output ('LABEL_HALOS_PREPARED count='+$records.Count+' gaussian_sigma=6 border_alpha=0')
