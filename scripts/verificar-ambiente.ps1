# Compile and run only the project's own C++20 diagnostic. Never install tools.
$ErrorActionPreference = 'Stop'
New-Item -ItemType Directory -Force -Path (Join-Path (Split-Path $PSScriptRoot -Parent) 'docs/relatorios/atuais') | Out-Null
$projectRoot = Split-Path $PSScriptRoot -Parent
$source = Join-Path $projectRoot 'tools\diagnostico.cpp'
$buildRoot = Join-Path $projectRoot 'build\diagnostico'
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (-not (Test-Path -LiteralPath $vswhere -PathType Leaf)) {
    throw 'vswhere não encontrado. Nenhuma ferramenta será instalada automaticamente.'
}
$raw = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -format json -utf8
if ($LASTEXITCODE -ne 0) { throw 'Falha ao consultar as ferramentas C++ instaladas.' }
$instances = @($raw | ConvertFrom-Json)
if ($instances.Count -ne 1 -or -not $instances[0].isComplete) {
    throw 'Nenhuma instalação completa de Visual Studio com ferramentas C++ foi encontrada.'
}
$instance = $instances[0]
$devcmd = Join-Path $instance.installationPath 'Common7\Tools\VsDevCmd.bat'
if (-not (Test-Path -LiteralPath $devcmd -PathType Leaf)) { throw 'VsDevCmd.bat ausente.' }
foreach ($path in @($projectRoot, $devcmd)) {
    if ($path -match '[%"\r\n&|<>^!]') { throw 'Caminho contém caracteres não suportados pelo wrapper de compilação.' }
}
New-Item -ItemType Directory -Path $buildRoot -Force | Out-Null
$tempRoot = Join-Path $buildRoot 'temp'
New-Item -ItemType Directory -Path $tempRoot -Force | Out-Null
$exe = Join-Path $buildRoot 'diagnostico.exe'
$obj = Join-Path $buildRoot 'diagnostico.obj'
$batchPath = Join-Path $buildRoot 'compilar-e-executar.cmd'
$batch = @"
@echo off
setlocal
set "TEMP=$tempRoot"
set "TMP=$tempRoot"
set "VSCMD_SKIP_SENDTELEMETRY=1"
set "DOTNET_CLI_TELEMETRY_OPTOUT=1"
call "$devcmd" -no_logo -arch=x64 -host_arch=x64
if errorlevel 1 exit /b 10
echo MSVC_TOOLSET=%VCToolsVersion%
echo WINDOWS_SDK=%WindowsSDKVersion%
echo SDK_ROOT=%WindowsSdkDir%
echo VC_ROOT=%VCToolsInstallDir%
cl.exe /nologo /std:c++20 /permissive- /Zc:__cplusplus /EHsc /W4 /WX /utf-8 /Od /MT /Fo"$obj" /Fe"$exe" "$source" /link /INCREMENTAL:NO kernel32.lib
if errorlevel 1 exit /b 20
"$exe"
if errorlevel 1 exit /b 30
exit /b 0
"@
[IO.File]::WriteAllText($batchPath, $batch, [Text.Encoding]::Default)
Push-Location $buildRoot
try {
    $messages = @(& $env:ComSpec /d /c $batchPath 2>&1 | ForEach-Object { $_.ToString() })
    $exitCode = $LASTEXITCODE
} finally {
    Pop-Location
}
$messages | Set-Content -LiteralPath (Join-Path $buildRoot 'resultado.log') -Encoding utf8
$messages | ForEach-Object { Write-Output $_ }
if ($exitCode -ne 0) { throw "Diagnóstico falhou (código $exitCode). Consulte build/diagnostico/resultado.log." }
$success = @($messages | Where-Object { $_ -match '^TOOLCHAIN_OK ' })
if ($success.Count -ne 1) { throw 'Programa não confirmou a verificação esperada.' }
function MetadataValue([string]$prefix) {
    $line = @($messages | Where-Object { $_.StartsWith($prefix + '=') })
    if ($line.Count -ne 1) { throw "Metadado de compilação ausente/ambíguo: $prefix" }
    return $line[0].Substring($prefix.Length + 1).Trim().TrimEnd('\')
}
$evidence = [ordered]@{
    CheckedUtc = [DateTime]::UtcNow.ToString('o')
    VisualStudio = $instance.displayName
    VisualStudioDisplayVersion = $instance.catalog.productDisplayVersion
    VisualStudioInstallationVersion = $instance.installationVersion
    MSVCToolset = MetadataValue 'MSVC_TOOLSET'
    WindowsSDK = MetadataValue 'WINDOWS_SDK'
    Architecture = 'x64'
    Standard = 'C++20'
    Flags = '/std:c++20 /permissive- /Zc:__cplusplus /EHsc /W4 /WX /utf-8 /Od /MT'
    Result = $success[0]
    ExitCode = $exitCode
    Checks = @('C++20 concepts/span/ranges', 'x64 pointers', 'standard library sort and sum', 'Windows SDK header/link and GetNativeSystemInfo call')
    Scope = 'Own diagnostic only; no gameplay, engine, downloads or installations'
    Command = '.\scripts\verificar-ambiente.ps1'
}
$evidence | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath (Join-Path $projectRoot 'docs/relatorios/atuais/AMBIENTE.json') -Encoding utf8
Write-Output 'AMBIENTE_OK: compilação, link e execução do diagnóstico confirmados.'
