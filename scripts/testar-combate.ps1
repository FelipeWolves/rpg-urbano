# Build the own combat core once, then run behavior tests and an automated demo.
$ErrorActionPreference = 'Stop'
$root = Split-Path $PSScriptRoot -Parent
$build = Join-Path $root 'build\combate'
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (-not (Test-Path -LiteralPath $vswhere -PathType Leaf)) { throw 'Ferramentas ausentes; nenhuma instalação automática.' }
$raw = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -format json -utf8
if ($LASTEXITCODE -ne 0) { throw 'Falha no vswhere.' }
$instances = @($raw | ConvertFrom-Json)
if ($instances.Count -ne 1 -or -not $instances[0].isComplete) { throw 'Instalação C++ completa não encontrada.' }
$devcmd = Join-Path $instances[0].installationPath 'Common7\Tools\VsDevCmd.bat'
if (-not (Test-Path -LiteralPath $devcmd -PathType Leaf)) { throw 'VsDevCmd ausente.' }
foreach ($path in @($root, $devcmd)) { if ($path -match '[%"\r\n&|<>^!]') { throw 'Caminho não suportado pelo wrapper.' } }
New-Item -ItemType Directory -Path $build -Force | Out-Null
New-Item -ItemType Directory -Path (Join-Path $build 'temp') -Force | Out-Null
$batch = @"
@echo off
setlocal
set "TEMP=$build\temp"
set "TMP=$build\temp"
set "VSCMD_SKIP_SENDTELEMETRY=1"
set "DOTNET_CLI_TELEMETRY_OPTOUT=1"
call "$devcmd" -no_logo -arch=x64 -host_arch=x64
if errorlevel 1 exit /b 10
echo MSVC_TOOLSET=%VCToolsVersion%
echo WINDOWS_SDK=%WindowsSDKVersion%
set "FLAGS=/nologo /std:c++20 /permissive- /Zc:__cplusplus /EHsc /W4 /WX /utf-8 /Od /MT"
cl.exe %FLAGS% /I"$root\include" /c "$root\src\battle.cpp" /Fo"$build\battle.obj"
if errorlevel 1 exit /b 20
cl.exe %FLAGS% /I"$root\include" "$root\tests\battle_tests.cpp" "$build\battle.obj" /Fo"$build\battle_tests.obj" /Fe"$build\battle_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 21
"$build\battle_tests.exe"
if errorlevel 1 exit /b 30
cl.exe %FLAGS% /I"$root\include" "$root\tests\time_limit_tests.cpp" "$build\battle.obj" /Fo"$build\time_limit_tests.obj" /Fe"$build\time_limit_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 24
"$build\time_limit_tests.exe"
if errorlevel 1 exit /b 33
cl.exe %FLAGS% /I"$root\include" /c "$root\src\stage.cpp" /Fo"$build\stage.obj"
if errorlevel 1 exit /b 25
cl.exe %FLAGS% /I"$root\include" "$root\tests\stage_tests.cpp" "$build\battle.obj" "$build\stage.obj" /Fo"$build\stage_tests.obj" /Fe"$build\stage_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 26
"$build\stage_tests.exe"
if errorlevel 1 exit /b 34
cl.exe %FLAGS% /I"$root\include" "$root\tests\timing_tests.cpp" "$build\battle.obj" "$build\stage.obj" /Fo"$build\timing_tests.obj" /Fe"$build\timing_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 27
"$build\timing_tests.exe" "$build\tempo-testes.csv"
if errorlevel 1 exit /b 35
cl.exe %FLAGS% /I"$root\include" "$root\tests\resolution_tests.cpp" "$build\battle.obj" "$build\stage.obj" /Fo"$build\resolution_tests.obj" /Fe"$build\resolution_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 28
"$build\resolution_tests.exe" "$build\resolucao-testes.csv"
if errorlevel 1 exit /b 36
cl.exe %FLAGS% /I"$root\include" /c "$root\src\sequential.cpp" /Fo"$build\sequential.obj"
if errorlevel 1 exit /b 29
cl.exe %FLAGS% /I"$root\include" "$root\tests\sequential_tests.cpp" "$build\battle.obj" "$build\sequential.obj" /Fo"$build\sequential_tests.obj" /Fe"$build\sequential_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 40
"$build\sequential_tests.exe" "$build\sequencia-testes.csv"
if errorlevel 1 exit /b 41
cl.exe %FLAGS% /I"$root\include" "$root\tests\scenario_tests.cpp" "$build\battle.obj" /Fo"$build\scenario_tests.obj" /Fe"$build\scenario_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 23
"$build\scenario_tests.exe" "$build\formacoes.csv"
if errorlevel 1 exit /b 32
cl.exe %FLAGS% /I"$root\include" "$root\app\simular.cpp" "$build\battle.obj" /Fo"$build\simular.obj" /Fe"$build\simular.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 22
"$build\simular.exe"
if errorlevel 1 exit /b 31
exit /b 0
"@
$batchPath = Join-Path $build 'testar.cmd'
[IO.File]::WriteAllText($batchPath, $batch, [Text.Encoding]::Default)
Push-Location $build
try { $messages = @(& $env:ComSpec /d /c $batchPath 2>&1 | ForEach-Object { $_.ToString() }); $code = $LASTEXITCODE }
finally { Pop-Location }
$messages | Set-Content -LiteralPath (Join-Path $build 'resultado.log') -Encoding utf8
$messages | ForEach-Object { Write-Output $_ }
if ($code -ne 0) { throw "Combate falhou (código $code). Consulte build/combate/resultado.log." }
$testResult = @($messages | Where-Object { $_ -match '^TESTS_RESULT passed=(\d+) total=(\d+)$' })
$simulation = @($messages | Where-Object { $_ -match '^SIMULATION_OK ' })
if ($testResult.Count -ne 1 -or $simulation.Count -ne 1) { throw 'Resultado esperado ausente.' }
$match = [regex]::Match($testResult[0], '^TESTS_RESULT passed=(\d+) total=(\d+)$')
if ($match.Groups[1].Value -ne $match.Groups[2].Value) { throw 'Há testes reprovados.' }
$limitResult = @($messages | Where-Object { $_ -match '^LIMIT_TESTS_RESULT passed=(\d+) total=(\d+)$' })
if ($limitResult.Count -ne 1) { throw 'Resultado de limites ausente.' }
$limitMatch = [regex]::Match($limitResult[0], '^LIMIT_TESTS_RESULT passed=(\d+) total=(\d+)$')
if ($limitMatch.Groups[1].Value -ne $limitMatch.Groups[2].Value) { throw 'Há testes de limites reprovados.' }
$stageResult = @($messages | Where-Object { $_ -match '^STAGE_TESTS_RESULT passed=(\d+) total=(\d+)$' })
if ($stageResult.Count -ne 1) { throw 'Resultado de fases ausente.' }
$stageMatch = [regex]::Match($stageResult[0], '^STAGE_TESTS_RESULT passed=(\d+) total=(\d+)$')
if ($stageMatch.Groups[1].Value -ne $stageMatch.Groups[2].Value) { throw 'Testes de fases reprovados.' }
$timingResult = @($messages | Where-Object { $_ -match '^TIMING_TESTS_RESULT passed=(\d+) total=(\d+)$' })
if ($timingResult.Count -ne 1) { throw 'Resultado de tempo ausente.' }
$timingMatch = [regex]::Match($timingResult[0], '^TIMING_TESTS_RESULT passed=(\d+) total=(\d+)$')
if ($timingMatch.Groups[1].Value -ne $timingMatch.Groups[2].Value) { throw 'Testes de tempo reprovados.' }
$resolutionResult = @($messages | Where-Object { $_ -match '^RESOLUTION_TESTS_RESULT passed=(\d+) total=(\d+)$' })
if ($resolutionResult.Count -ne 1) { throw 'Resultado de resolução ausente.' }
$resolutionMatch = [regex]::Match($resolutionResult[0], '^RESOLUTION_TESTS_RESULT passed=(\d+) total=(\d+)$')
if ($resolutionMatch.Groups[1].Value -ne $resolutionMatch.Groups[2].Value) { throw 'Testes de resolução reprovados.' }
$resolutionCsv = @(Import-Csv -LiteralPath (Join-Path $build 'resolucao-testes.csv'))
if ($resolutionCsv.Count -ne [int]$resolutionMatch.Groups[2].Value -or @($resolutionCsv | Where-Object status -ne 'pass').Count) { throw 'Matriz de resolução inconsistente.' }
$sequentialResult = @($messages | Where-Object { $_ -match '^SEQUENTIAL_TESTS_RESULT passed=(\d+) total=(\d+)$' })
if ($sequentialResult.Count -ne 1) { throw 'Resultado de times sucessivos ausente.' }
$sequentialMatch = [regex]::Match($sequentialResult[0], '^SEQUENTIAL_TESTS_RESULT passed=(\d+) total=(\d+)$')
if ($sequentialMatch.Groups[1].Value -ne $sequentialMatch.Groups[2].Value) { throw 'Testes de times sucessivos reprovados.' }
$sequentialCsv = @(Import-Csv -LiteralPath (Join-Path $build 'sequencia-testes.csv'))
if ($sequentialCsv.Count -ne [int]$sequentialMatch.Groups[2].Value -or @($sequentialCsv | Where-Object status -ne 'pass').Count) { throw 'Matriz sequencial inconsistente.' }
$scenario = @($messages | Where-Object { $_ -match '^SCENARIOS_OK cases=168 replays=168 ' })
$isolation = @($messages | Where-Object { $_ -match '^ISOLATION_OK ' })
$batchResult = @($messages | Where-Object { $_ -match '^BATCH_OK battles=1000 ' })
if ($scenario.Count -ne 1 -or $isolation.Count -ne 1 -or $batchResult.Count -ne 1) { throw 'Verificação ampliada incompleta.' }
$csv = @(Import-Csv -LiteralPath (Join-Path $build 'formacoes.csv'))
if ($csv.Count -ne 168 -or @($csv | Where-Object { $_.replay -ne 'pass' -or $_.invariants -ne 'pass' }).Count) { throw 'Matriz de formações inconsistente.' }
$timingCsv = @(Import-Csv -LiteralPath (Join-Path $build 'tempo-testes.csv'))
if ($timingCsv.Count -ne [int]$timingMatch.Groups[2].Value -or @($timingCsv | Where-Object status -ne 'pass').Count) { throw 'Matriz de tempo inconsistente.' }
$reportDirectory = Join-Path $root 'docs\batalhas'
New-Item -ItemType Directory -Path $reportDirectory -Force | Out-Null
Copy-Item -LiteralPath (Join-Path $build 'formacoes.csv') -Destination (Join-Path $reportDirectory 'formacoes-testadas.csv')
Copy-Item -LiteralPath (Join-Path $build 'tempo-testes.csv') -Destination (Join-Path $reportDirectory 'tempo-testes.csv')
Copy-Item -LiteralPath (Join-Path $build 'resolucao-testes.csv') -Destination (Join-Path $reportDirectory 'resolucao-testes.csv')
Copy-Item -LiteralPath (Join-Path $build 'sequencia-testes.csv') -Destination (Join-Path $reportDirectory 'sequencia-testes.csv')
[ordered]@{
    CheckedUtc = [DateTime]::UtcNow.ToString('o')
    Command = '.\scripts\testar-combate.ps1'
    VisualStudio = $instances[0].catalog.productDisplayVersion
    Toolset = @($messages | Where-Object { $_ -match '^MSVC_TOOLSET=' })[0]
    WindowsSDK = @($messages | Where-Object { $_ -match '^WINDOWS_SDK=' })[0]
    Passed = [int]$match.Groups[1].Value
    Total = [int]$match.Groups[2].Value
    LimitTestsPassed = [int]$limitMatch.Groups[1].Value
    LimitTestsTotal = [int]$limitMatch.Groups[2].Value
    StageTestsPassed = [int]$stageMatch.Groups[1].Value
    StageTestsTotal = [int]$stageMatch.Groups[2].Value
    TimingTestsPassed = [int]$timingMatch.Groups[1].Value
    TimingTestsTotal = [int]$timingMatch.Groups[2].Value
    ResolutionTestsPassed = [int]$resolutionMatch.Groups[1].Value
    ResolutionTestsTotal = [int]$resolutionMatch.Groups[2].Value
    SequentialTestsPassed = [int]$sequentialMatch.Groups[1].Value
    SequentialTestsTotal = [int]$sequentialMatch.Groups[2].Value
    APFormationCases = 25
    Simulation = $simulation[0]
    FormationCases = $csv.Count
    FormationResult = $scenario[0]
    IsolationResult = $isolation[0]
    LocalBatchResult = $batchResult[0]
    ExitCode = $code
    Scope = 'Own combat core with action identities/multi-hit/skill guard and fixed/AP timing, skills/status clocks, extras/reactions, optional guard, consecutive stages, three successive squads with stable/phased boss kits and classified states/shields and local/global action budgets; formation/replay matrix; independent interleaved contexts; bounded local functional batch; not shared-boss/multithreading/server validation'
} | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $root 'COMBATE.json') -Encoding utf8
