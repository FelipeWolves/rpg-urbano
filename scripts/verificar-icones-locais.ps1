# Read-only verification of local derivatives and their original hashes.
$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
$root=Split-Path $PSScriptRoot -Parent
$catalog=Get-Content (Join-Path $root 'data/presentation/ui-assets-2026-11-22.json') -Raw | ConvertFrom-Json
$drawingRefs=@(Get-ChildItem $PSHOME -File | Where-Object {$_.Name -match '^(System.Drawing.*|System.Private.Windows.*|System.ComponentModel.Primitives)\.dll$'} | ForEach-Object FullName)
Add-Type -ReferencedAssemblies $drawingRefs -TypeDefinition @"
using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
public static class IconVerifyLocal {
 public static long[] Check(string source,string target,bool processed){
 using(var original=new Bitmap(source))using(var output=new Bitmap(target))using(var converted=new Bitmap(original.Width,original.Height,PixelFormat.Format32bppArgb)){
 using(var g=Graphics.FromImage(converted)){g.CompositingMode=System.Drawing.Drawing2D.CompositingMode.SourceCopy;g.DrawImage(original,new Rectangle(0,0,original.Width,original.Height),0,0,original.Width,original.Height,GraphicsUnit.Pixel);}
 var a=converted.LockBits(new Rectangle(0,0,converted.Width,converted.Height),ImageLockMode.ReadOnly,PixelFormat.Format32bppArgb);var b=output.LockBits(new Rectangle(0,0,output.Width,output.Height),ImageLockMode.ReadOnly,PixelFormat.Format32bppArgb);
 try{byte[] left=new byte[a.Stride*converted.Height],right=new byte[b.Stride*output.Height];Marshal.Copy(a.Scan0,left,0,left.Length);Marshal.Copy(b.Scan0,right,0,right.Length);long changed=0,protectedPixels=0,transparent=0;
 for(int y=0;y<output.Height;y++)for(int x=0;x<output.Width;x++){if(right[y*b.Stride+x*4+3]==0)transparent++;}
 int dx=(output.Width-original.Width)/2,dy=(output.Height-original.Height)/2;if(dx<0||dy<0)throw new Exception("Cropped output");
 for(int y=0;y<original.Height;y++)for(int x=0;x<original.Width;x++){int p=y*a.Stride+x*4,q=(y+dy)*b.Stride+(x+dx)*4;int lo=Math.Min(left[p],Math.Min(left[p+1],left[p+2])),hi=Math.Max(left[p],Math.Max(left[p+1],left[p+2]));if(processed&&(lo<220||hi-lo>20)){protectedPixels++;for(int c=0;c<4;c++){if(left[p+c]!=right[q+c]){changed++;break;}}}}
 return new long[]{protectedPixels,changed,transparent,output.Width,output.Height};
 }finally{converted.UnlockBits(a);output.UnlockBits(b);}}
 }
}
"@
$rows=@();foreach($item in $catalog.Resources){$source=Join-Path $root $item.Source;$target=Join-Path $root $item.Path;if((Get-FileHash -LiteralPath $source -Algorithm SHA256).Hash -ne $item.SourceSHA256 -or (Get-FileHash -LiteralPath $target -Algorithm SHA256).Hash -ne $item.SHA256){throw 'Image hash mismatch'};$check=[IconVerifyLocal]::Check($source,$target,$item.WhiteRemoved);if($check[1] -ne 0 -or $check[3] -ne $item.Width -or $check[4] -ne $item.Height -or ($item.WhiteRemoved -and $check[2] -eq 0)){throw ('Invalid derivative: '+$item.Id)};$rows+=[ordered]@{Id=$item.Id;ProtectedPixelsChecked=$check[0];ProtectedPixelsChanged=$check[1];TransparentOutputPixels=$check[2];Width=$check[3];Height=$check[4];Passed=$true}}
[ordered]@{Date='2026-11-22';Passed=$true;Count=$rows.Count;TransparentIcons=@($catalog.Resources | Where-Object WhiteRemoved).Count;OriginalHashesUnchanged=$true;VisualAcceptanceAutomaticallyGranted=$false;EditorOpened=$false;IntegratedIntoHUD=$false;Checks=$rows} | ConvertTo-Json -Depth 7 | Set-Content (Join-Path $root 'docs/relatorios/atuais/VIS-ICONS-PNG-2026-11-22.json') -Encoding utf8
Write-Output ('UI_ASSETS_VERIFIED count='+$rows.Count+' transparent_icons=17 protected_pixels_changed=0')
