$ErrorActionPreference = 'Stop'
$project = Split-Path $PSScriptRoot -Parent
$executable = Join-Path $project 'build\combate\jogar.exe'
if (-not (Test-Path -LiteralPath $executable -PathType Leaf)) {
    throw 'Compile primeiro com .\scripts\testar-combate.ps1 na pasta do projeto.'
}
& $executable
if ($LASTEXITCODE -ne 0) { throw "Sessão textual encerrada com código $LASTEXITCODE." }
