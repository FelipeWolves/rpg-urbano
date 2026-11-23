# .NET only; prepares derivatives, never overwrites received originals.
param([switch]$InspectOnly)
$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$root=Split-Path $PSScriptRoot -Parent
$received=Join-Path $root 'entregas/05-hub-identidade-e-arte2d/recebidos'
$output=Join-Path $root 'assets/runtime/ui/2026-11-22-v02'
$qa=Join-Path $root 'build/ui-assets-2026-11-22'
New-Item -ItemType Directory -Force -Path $qa | Out-Null
$items=@()
foreach($name in @('ico_skill_felipe_a1','ico_skill_felipe_a2','ico_skill_felipe_a2_active','ico_skill_felipe_a3')){$items+=@{Id=$name;Source=$name+'.jpg';Target='icones-habilidades-felipe-adulto/'+$name+'.png';RemoveWhite=$true;Square=0}}
foreach($name in @('ico_skill_jovem_lobo_a1','ico_skill_jovem_lobo_a2','ico_skill_jovem_lobo_a2_active','ico_skill_jovem_lobo_a3')){$items+=@{Id=$name;Source='icones-habilidades-jovem-lobo/'+$name+'.jpg';Target='icones-habilidades-jovem-lobo/'+$name+'.png';RemoveWhite=$true;Square=0}}
foreach($file in Get-ChildItem -LiteralPath (Join-Path $received 'icones-status') -Recurse -File -Filter *.png){$relative=[IO.Path]::GetRelativePath($received,$file.FullName).Replace('\','/');$items+=@{Id=$file.BaseName;Source=$relative;Target=$relative;RemoveWhite=$true;Square=384}}
$items+=@{Id='logo-lobo-v01';Source='logo-lobo-v01.png';Target='logo-lobo-v01.png';RemoveWhite=$false;Square=0}
$items+=@{Id='avatar-felipe-v01';Source='portrait-felipe-v01.png';Target='avatar-felipe-v01.png';RemoveWhite=$false;Square=0}
function ContactSheet($paths,$destination){
 $size=220;$cols=5;$rows=[int][Math]::Ceiling($paths.Count/$cols);$sheet=[Drawing.Bitmap]::new($cols*$size,$rows*($size+24));$g=[Drawing.Graphics]::FromImage($sheet);$font=[Drawing.Font]::new('Arial',9);try{
 $g.Clear([Drawing.Color]::FromArgb(65,65,75));$g.InterpolationMode=[Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
 for($i=0;$i -lt $paths.Count;$i++){$image=[Drawing.Image]::FromFile($paths[$i]);try{$x=($i%$cols)*$size;$y=[int][Math]::Floor($i/$cols)*($size+24);$scale=[Math]::Min(($size-12)/$image.Width,($size-12)/$image.Height);$w=[int]($image.Width*$scale);$h=[int]($image.Height*$scale);$g.DrawImage($image,($x+[int](($size-$w)/2)),($y+[int](($size-$h)/2)),$w,$h);$g.DrawString([IO.Path]::GetFileNameWithoutExtension($paths[$i]),$font,[Drawing.Brushes]::White,[single]($x+2),[single]($y+$size))}finally{$image.Dispose()}}
 $sheet.Save($destination,[Drawing.Imaging.ImageFormat]::Png)
 }finally{$font.Dispose();$g.Dispose();$sheet.Dispose()}
}
if($InspectOnly){ContactSheet @($items | ForEach-Object {Join-Path $received $_.Source}) (Join-Path $qa 'antes.png');Write-Output (Join-Path $qa 'antes.png');return}
$drawingRefs=@(Get-ChildItem $PSHOME -File | Where-Object {$_.Name -match '^(System.Drawing.*|System.Private.Windows.*|System.ComponentModel.Primitives)\.dll$'} | ForEach-Object FullName)
Add-Type -ReferencedAssemblies $drawingRefs -TypeDefinition @"
using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
public static class IconAlphaLocal {
 public static long[] Prepare(string source,string destination,int square,bool remove){
  using(var original=new Bitmap(source))using(var bitmap=new Bitmap(original.Width,original.Height,PixelFormat.Format32bppArgb)){
   using(var g=Graphics.FromImage(bitmap)){g.CompositingMode=System.Drawing.Drawing2D.CompositingMode.SourceCopy;g.DrawImage(original,new Rectangle(0,0,original.Width,original.Height),0,0,original.Width,original.Height,GraphicsUnit.Pixel);}
   int w=bitmap.Width,h=bitmap.Height,n=w*h;var data=bitmap.LockBits(new Rectangle(0,0,w,h),ImageLockMode.ReadWrite,PixelFormat.Format32bppArgb);
   byte[] pixels=new byte[data.Stride*h];Marshal.Copy(data.Scan0,pixels,0,pixels.Length);bool[] exterior=new bool[n];int[] queue=new int[n];int head=0,tail=0;
   Action<int> visit=(p)=>{if(exterior[p])return;int offset=(p/w)*data.Stride+(p%w)*4;int lo=Math.Min(pixels[offset],Math.Min(pixels[offset+1],pixels[offset+2]));int hi=Math.Max(pixels[offset],Math.Max(pixels[offset+1],pixels[offset+2]));if(lo>=245&&hi-lo<=12){exterior[p]=true;queue[tail++]=p;}};
   if(remove){for(int x=0;x<w;x++){visit(x);visit((h-1)*w+x);}for(int y=0;y<h;y++){visit(y*w);visit(y*w+w-1);}
    while(head<tail){int p=queue[head++],x=p%w,y=p/w;if(x>0)visit(p-1);if(x+1<w)visit(p+1);if(y>0)visit(p-w);if(y+1<h)visit(p+w);}
   }
   long transparent=0,partial=0,opaque=0;
   for(int p=0;p<n;p++){int x=p%w,y=p/w,o=y*data.Stride+x*4;if(remove&&exterior[p]){pixels[o]=pixels[o+1]=pixels[o+2]=pixels[o+3]=0;}
    else if(remove){bool edge=(x>0&&exterior[p-1])||(x+1<w&&exterior[p+1])||(y>0&&exterior[p-w])||(y+1<h&&exterior[p+w]);int lo=Math.Min(pixels[o],Math.Min(pixels[o+1],pixels[o+2])),hi=Math.Max(pixels[o],Math.Max(pixels[o+1],pixels[o+2]));if(edge&&lo>=220&&hi-lo<=20){int alpha=Math.Min(255,Math.Max(1,(245-lo)*255/25));for(int c=0;c<3;c++)pixels[o+c]=(byte)Math.Clamp((pixels[o+c]*255-255*(255-alpha))/alpha,0,255);pixels[o+3]=(byte)alpha;}}
    if(pixels[o+3]==0)transparent++;else if(pixels[o+3]<255)partial++;else opaque++;
   }
   Marshal.Copy(pixels,0,data.Scan0,pixels.Length);bitmap.UnlockBits(data);
   int side=square>0?Math.Max(square,Math.Max(w,h)):0;int ow=side>0?side:w,oh=side>0?side:h;
   using(var result=new Bitmap(ow,oh,PixelFormat.Format32bppArgb)){using(var g=Graphics.FromImage(result)){g.Clear(Color.Transparent);g.CompositingMode=System.Drawing.Drawing2D.CompositingMode.SourceCopy;g.DrawImage(bitmap,new Rectangle((ow-w)/2,(oh-h)/2,w,h),0,0,w,h,GraphicsUnit.Pixel);}result.Save(destination,ImageFormat.Png);}
   return new long[]{w,h,ow,oh,transparent,partial,opaque,tail};
  }
 }
}
"@
$records=@();foreach($item in $items){$source=Join-Path $received $item.Source;$target=Join-Path $output $item.Target;if(Test-Path -LiteralPath $target){throw ('Derivative already exists; preserve version: '+$target)};New-Item -ItemType Directory -Force -Path (Split-Path $target -Parent) | Out-Null;$before=(Get-FileHash -LiteralPath $source -Algorithm SHA256).Hash;$metrics=[IconAlphaLocal]::Prepare($source,$target,$item.Square,$item.RemoveWhite);if((Get-FileHash -LiteralPath $source -Algorithm SHA256).Hash -ne $before){throw 'Original unexpectedly changed'};if($item.RemoveWhite -and ($metrics[4] -lt 1 -or $metrics[6] -lt 1)){throw 'Invalid alpha removal result'};$records+=[ordered]@{Id=$item.Id;Source=('entregas/05-hub-identidade-e-arte2d/recebidos/'+$item.Source);SourceSHA256=$before;Path=('assets/runtime/ui/2026-11-22-v02/'+$item.Target);SHA256=(Get-FileHash -LiteralPath $target -Algorithm SHA256).Hash;Width=$metrics[2];Height=$metrics[3];SourceWidth=$metrics[0];SourceHeight=$metrics[1];TransparentSourcePixels=$metrics[4];PartialSourcePixels=$metrics[5];OpaqueSourcePixels=$metrics[6];ExteriorWhitePixels=$metrics[7];WhiteRemoved=$item.RemoveWhite;Cropped=$false;Scaled=$false;Status='PreparedNotIntegrated'}}
[ordered]@{SchemaVersion=1;Date='2026-11-22';Method='Local .NET; border-connected near-white only; one-pixel neutral fringe; no crop/scale';OriginalsPreserved=$true;RuntimeIntegrated=$false;Resources=$records} | ConvertTo-Json -Depth 8 | Set-Content (Join-Path $root 'data/presentation/ui-assets-2026-11-22.json') -Encoding utf8
ContactSheet @($items | ForEach-Object {Join-Path $output $_.Target}) (Join-Path $qa 'depois.png')
Write-Output ('UI_ASSETS_PREPARED count='+$records.Count)
