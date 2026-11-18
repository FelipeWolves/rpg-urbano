# Build the own combat core once, then run behavior tests and an automated demo.
$ErrorActionPreference = 'Stop'
New-Item -ItemType Directory -Force -Path (Join-Path (Split-Path $PSScriptRoot -Parent) 'docs/relatorios/atuais') | Out-Null
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
cl.exe %FLAGS% /I"$root\include" /c "$root\src\parallel.cpp" /Fo"$build\parallel.obj"
if errorlevel 1 exit /b 42
cl.exe %FLAGS% /I"$root\include" "$root\tests\parallel_tests.cpp" "$build\battle.obj" "$build\parallel.obj" /Fo"$build\parallel_tests.obj" /Fe"$build\parallel_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 43
"$build\parallel_tests.exe" "$build\frentes-testes.csv"
if errorlevel 1 exit /b 44
cl.exe %FLAGS% /I"$root\include" /c "$root\src\shared_boss.cpp" /Fo"$build\shared_boss.obj"
if errorlevel 1 exit /b 45
cl.exe %FLAGS% /I"$root\include" "$root\tests\shared_boss_tests.cpp" "$build\battle.obj" "$build\shared_boss.obj" /Fo"$build\shared_boss_tests.obj" /Fe"$build\shared_boss_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 46
"$build\shared_boss_tests.exe" "$build\chefe-compartilhado-testes.csv"
if errorlevel 1 exit /b 47
cl.exe %FLAGS% /I"$root\include" /c "$root\src\ability_profile.cpp" /Fo"$build\ability_profile.obj"
if errorlevel 1 exit /b 53
cl.exe %FLAGS% /I"$root\include" "$root\tests\ability_profile_tests.cpp" "$build\battle.obj" "$build\ability_profile.obj" /Fo"$build\ability_profile_tests.obj" /Fe"$build\ability_profile_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 54
"$build\ability_profile_tests.exe" "$build\perfis-testes.csv"
if errorlevel 1 exit /b 55
cl.exe %FLAGS% /I"$root\include" /c "$root\src\text_session.cpp" /Fo"$build\text_session.obj"
if errorlevel 1 exit /b 48
cl.exe %FLAGS% /I"$root\include" "$root\tests\text_session_tests.cpp" "$build\battle.obj" "$build\ability_profile.obj" "$build\text_session.obj" /Fo"$build\text_session_tests.obj" /Fe"$build\text_session_tests.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 49
"$build\text_session_tests.exe" "$build\interacao-testes.csv"
if errorlevel 1 exit /b 50
cl.exe %FLAGS% /I"$root\include" "$root\app\jogar.cpp" "$build\battle.obj" "$build\ability_profile.obj" "$build\text_session.obj" /Fo"$build\jogar.obj" /Fe"$build\jogar.exe" /link /INCREMENTAL:NO
if errorlevel 1 exit /b 51
"$build\jogar.exe" < "$build\jogar-smoke.txt" > "$build\jogar-smoke.log"
if errorlevel 1 exit /b 52
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
[IO.File]::WriteAllText((Join-Path $build 'jogar-smoke.txt'),"sair`r`n",[Text.Encoding]::ASCII)
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
$parallelResult = @($messages | Where-Object { $_ -match '^PARALLEL_TESTS_RESULT passed=(\d+) total=(\d+)$' })
if ($parallelResult.Count -ne 1) { throw 'Resultado de frentes ausente.' }
$parallelMatch = [regex]::Match($parallelResult[0], '^PARALLEL_TESTS_RESULT passed=(\d+) total=(\d+)$')
if ($parallelMatch.Groups[1].Value -ne $parallelMatch.Groups[2].Value) { throw 'Testes de frentes reprovados.' }
$parallelCsv = @(Import-Csv -LiteralPath (Join-Path $build 'frentes-testes.csv'))
if ($parallelCsv.Count -ne [int]$parallelMatch.Groups[2].Value -or @($parallelCsv | Where-Object status -ne 'pass').Count) { throw 'Matriz de frentes inconsistente.' }
Copy-Item -LiteralPath (Join-Path $build 'frentes-testes.csv') -Destination (Join-Path $root 'docs\batalhas\frentes-testes.csv')
$sharedResult = @($messages | Where-Object { $_ -match '^SHARED_BOSS_TESTS_RESULT passed=(\d+) total=(\d+)$' })
if ($sharedResult.Count -ne 1) { throw 'Resultado de chefe compartilhado ausente.' }
$sharedMatch = [regex]::Match($sharedResult[0], '^SHARED_BOSS_TESTS_RESULT passed=(\d+) total=(\d+)$')
if ($sharedMatch.Groups[1].Value -ne $sharedMatch.Groups[2].Value) { throw 'Testes de chefe compartilhado reprovados.' }
$sharedCsv = @(Import-Csv -LiteralPath (Join-Path $build 'chefe-compartilhado-testes.csv'))
if ($sharedCsv.Count -ne [int]$sharedMatch.Groups[2].Value -or @($sharedCsv | Where-Object status -ne 'pass').Count) { throw 'Matriz de chefe compartilhado inconsistente.' }
Copy-Item -LiteralPath (Join-Path $build 'chefe-compartilhado-testes.csv') -Destination (Join-Path $root 'docs\batalhas\chefe-compartilhado-testes.csv')
$textResult = @($messages | Where-Object { $_ -match '^TEXT_TESTS_RESULT passed=(\d+) total=(\d+)$' })
if ($textResult.Count -ne 1) { throw 'Resultado de interação textual ausente.' }
$textMatch = [regex]::Match($textResult[0], '^TEXT_TESTS_RESULT passed=(\d+) total=(\d+)$')
if ($textMatch.Groups[1].Value -ne $textMatch.Groups[2].Value) { throw 'Testes de interação textual reprovados.' }
$textCsv = @(Import-Csv -LiteralPath (Join-Path $build 'interacao-testes.csv'))
if ($textCsv.Count -ne [int]$textMatch.Groups[2].Value -or @($textCsv | Where-Object status -ne 'pass').Count) { throw 'Matriz textual inconsistente.' }
if ((Get-Content -LiteralPath (Join-Path $build 'jogar-smoke.log') -Raw) -notmatch 'Sessao encerrada\.') { throw 'Verificação do aplicativo interativo falhou.' }
Copy-Item -LiteralPath (Join-Path $build 'interacao-testes.csv') -Destination (Join-Path $root 'docs\batalhas\interacao-testes.csv')
$profileResult = @($messages | Where-Object { $_ -match '^PROFILE_TESTS_RESULT passed=(\d+) total=(\d+)$' })
if ($profileResult.Count -ne 1) { throw 'Resultado de perfis ausente.' }
$profileMatch = [regex]::Match($profileResult[0], '^PROFILE_TESTS_RESULT passed=(\d+) total=(\d+)$')
if ($profileMatch.Groups[1].Value -ne $profileMatch.Groups[2].Value) { throw 'Testes de perfis reprovados.' }
$profileCsv = @(Import-Csv -LiteralPath (Join-Path $build 'perfis-testes.csv'))
if ($profileCsv.Count -ne [int]$profileMatch.Groups[2].Value -or @($profileCsv | Where-Object status -ne 'pass').Count) { throw 'Matriz de perfis inconsistente.' }
Copy-Item -LiteralPath (Join-Path $build 'perfis-testes.csv') -Destination (Join-Path $root 'docs\batalhas\perfis-testes.csv')
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
    ParallelTestsPassed = [int]$parallelMatch.Groups[1].Value
    ParallelTestsTotal = [int]$parallelMatch.Groups[2].Value
    SharedBossTestsPassed = [int]$sharedMatch.Groups[1].Value
    SharedBossTestsTotal = [int]$sharedMatch.Groups[2].Value
    TextTestsPassed = [int]$textMatch.Groups[1].Value
    TextTestsTotal = [int]$textMatch.Groups[2].Value
    TextExecutableSmoke = 'quit_ok'
    ProfileTestsPassed = [int]$profileMatch.Groups[1].Value
    ProfileTestsTotal = [int]$profileMatch.Groups[2].Value
    APFormationCases = 25
    Simulation = $simulation[0]
    FormationCases = $csv.Count
    FormationResult = $scenario[0]
    IsolationResult = $isolation[0]
    LocalBatchResult = $batchResult[0]
    ExitCode = $code
    Scope = 'Own combat core with action identities/multi-hit/skill guard and fixed/AP timing, skills/status clocks, extras/reactions, optional guard, consecutive stages, three successive squads with stable/phased boss kits and classified states/shields and local/global action budgets; formation/replay matrix; three coordinated independent cooperative fronts with global action budget and cancellation; independent interleaved contexts; bounded local functional batch; single-HP shared boss with scoped single-target skills, global clocks and R04; team/classic profiles, validated ability slots with canonical basic, informational declarations and one-shot self-shield BattleStart passives with activation records; text session with player A commands, basic AI B, restart and scripted integration tests; no broad shared-boss catalog/multithreading/server validation'
} | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $root 'docs/relatorios/atuais/COMBATE.json') -Encoding utf8
