# Read-only source inspection. All outputs stay in this original project's docs.
$ErrorActionPreference = 'Stop'
$project = Split-Path $PSScriptRoot -Parent
$analysis = Join-Path (Split-Path $project -Parent) 'analise-dislyte'
$source = 'F:\Testes de jogos\Dislyte Local - Teste'
$output = Join-Path $project 'docs\batalhas\comparacao-dislyte-2026-11-18'
if (Test-Path -LiteralPath $output) { throw 'Relatório já existe; preservar a entrega anterior antes de planejar novo incremento.' }
$inventory = @(Import-Csv -LiteralPath (Join-Path $analysis 'inventario.csv'))
$historical = Get-Content -LiteralPath (Join-Path $analysis 'resumo.json') -Raw | ConvertFrom-Json
$combat = Get-Content -LiteralPath (Join-Path $project 'COMBATE.json') -Raw | ConvertFrom-Json
$selected = @($inventory | Where-Object { $_.Path -match '(game\\Dislyte\.exe|XGame-Win64-Shipping\.exe|WindowsNoEditor\\Engine\.ini|Saved\\InstallPaks\\chunk_info|Saved\\PatchPaks\\(patch_info|upgrade_info)|game\\ver\.dat|Config\\DefaultXGameCustom\.ini)$' })
if ($selected.Count -ne 8) { throw 'Conjunto pontual inesperado; revisar escopo antes de continuar.' }
$rootPrefix = [IO.Path]::GetFullPath($source).TrimEnd('\') + '\'
$checks = @(foreach ($entry in $selected) {
    $path = [IO.Path]::GetFullPath((Join-Path $source $entry.Path))
    if (-not $path.StartsWith($rootPrefix,[StringComparison]::OrdinalIgnoreCase)) { throw 'Caminho fora da fonte autorizada.' }
    $file = Get-Item -LiteralPath $path -ErrorAction SilentlyContinue
    if ($file -and ($file.Attributes -band [IO.FileAttributes]::ReparsePoint)) { throw 'Link encontrado; não seguir.' }
    [pscustomobject]@{
        Path=$entry.Path; Present=[bool]$file
        SameBytes=[bool]($file -and $file.Length -eq [long]$entry.Bytes)
        SameModifiedUtc=[bool]($file -and $file.LastWriteTimeUtc.Ticks -eq [datetime]::Parse($entry.ModifiedUtc).ToUniversalTime().Ticks)
    }
})
$engine = Join-Path $source 'Dislyte\Dislyte Game\game\XGame\Saved\Config\WindowsNoEditor\Engine.ini'
# Whitelist only content search paths, never service/account/configuration values.
$contentPaths = @(Get-Content -LiteralPath $engine | Where-Object { $_ -match '^Paths=' })
$pluginReferences = @($contentPaths | Where-Object { $_ -match '/Plugins/' })
$rows = [Collections.Generic.List[object]]::new()
function Row($id,$area,$external,$own,$state,$parity,$evidence,$next) {
    $rows.Add([pscustomobject]@{Id=$id;Area=$area;DislyteEvidence=$external;OurEvidence=$own;OurStatus=$state;Parity=$parity;Sources=$evidence;NextEvidence=$next})
}
Row C01 'Engine e arquitetura' 'UE4.24.3 Shipping segundo log histórico; cliente empacotado' 'Núcleo original C++20 sem engine gráfica escolhida' 'Implementado no núcleo' 'Arquiteturas diferentes; superioridade não mensurável' 'A01; A02; P01; ESTADO.md' 'ENG-01: integração e medições comparáveis'
Row C02 'Integridade e distribuição' 'Histórico: 5576 PAKs, índices criptografados; dois chunks e referência XGame.exe ausentes' 'Fontes e evidências de testes versionados; sem distribuição comercial' 'Entrega parcial de produto' 'Não comparável diretamente' 'A01; A02; A03' 'Não usar divergências de manifestos para atribuir bugs ou qualidade do combate'
Row C03 'Formações 1 a 5' 'Relato do usuário e pesquisa histórica de formações; arquivos não revelam contrato' '168 cenários/replays; pares 1x1 até 5x5 e sondagens de seis' 'Validado no perfil próprio' 'Paridade de tamanho parcial; regras não verificadas' 'P01; P02; tests/scenario_tests.cpp' 'Casos observáveis equivalentes, mesmos atributos e ações'
Row C04 'SPD e barra de ação' 'Pesquisa histórica registra mecânicas AP/SPD; fórmula completa não acessível' 'Perfis fixo/AP, alterações de velocidade e 25 formações AP/replays' 'Validado no perfil próprio' 'Sem equivalência de fórmula demonstrada' 'P01; P03; docs/batalhas/TIME-02.md' 'Traços observados de AP, arredondamento e desempates'
Row C05 'Recargas e turnos extras' 'Pesquisa e respostas do usuário; regras específicas variam por habilidade' 'Recarga no uso, flags por origem e congelamento separado de bloqueio' 'Validado no perfil próprio' 'Contrato próprio; paridade completa não verificada' 'P03; P04; docs/batalhas/TIME-03.md' 'Exemplos reproduzíveis por habilidade e versão'
Row C06 'Turno geral e limites' 'Limites relatados por modo; contagem universal não estabelecida' 'Orçamentos opcionais por comando principal aceito; derivados separados' 'Validado no perfil próprio' 'Não chamar orçamento de ações de contador universal do Dislyte' 'P01; P03; docs/batalhas/TIME-01.md; docs/batalhas/PAR-01.md' 'Identificar regra observada de cada modo, extras e Skip'
Row C07 'Multi-hit e alvos dinâmicos' 'Ordem interna e seleção protegidas; contrato vindo da pesquisa do usuário' 'Interrupção entre hits; seleção LowestHealthEnemy determinística' 'Validado no perfil próprio' 'Bounce aleatório não implementado; equivalência não verificada' 'P04; P05; docs/batalhas/RES-01.md' 'Catálogo de seletores e RNG próprio antes de alegar fidelidade'
Row C08 'Reflexão, explosão e FIFO' 'Arquivos acessíveis não demonstram ordem causal; R04 é decisão do projeto' 'Consequências inline e FIFO testadas, inclusive entre frentes independentes' 'Validado no perfil próprio' 'R04 não é especificação oficial comprovada do Dislyte' 'P04; P05; docs/batalhas/RES-01.md; docs/batalhas/PAR-01.md' 'Observação de fronteiras em gravações controladas'
Row C09 'Prevenção, revive e vitória' 'Exemplos recebidos do usuário; implementação interna não acessível' 'Cargas finitas, janela de revive e desempate pelo autor da ação' 'Validado no perfil próprio' 'Decisão autoral fechada; equivalência externa não certificada' 'P04; P05; P01' 'Validar casos observáveis sem reabrir contrato próprio automaticamente'
Row C10 'Escudos, dano e atributos' 'Sem fórmulas ou tabelas de habilidades acessíveis' 'Guarda, duas camadas de escudo e dano mínimo; sem catálogo completo de DEF/CRIT/elementos' 'Implementação parcial' 'Não comparável em balanceamento' 'A02; include/rpg/battle.hpp; docs/batalhas/SEQ-01.md' 'HAB-01: especificar fórmulas, RNG e famílias adicionais'
Row C11 'Guarda voluntária' 'Usuário informa ausência de ação equivalente no contexto de referência' 'Ação opcional controlada por perfil' 'Diferença por decisão' 'Extensão própria; não prova vantagem de experiência' 'COMBATE.md; docs/batalhas/TIME-02.md' 'Avaliar utilidade e balanceamento em protótipo interativo'
Row C12 'Confrontos consecutivos' 'Relato de até cinco confrontos na mesma fase' 'Stage com 1/2/5 confrontos, políticas de conservação e orçamento global' 'Validado no perfil próprio' 'Cobertura estrutural; paridade de conteúdo não verificada' 'P02; docs/batalhas/WAVE-01.md; P01' 'Casos observados de persistência e efeitos entre confrontos'
Row C13 'Três times sucessivos' 'Relato de chefe mudando habilidades entre três times' 'SEQ-01: HP persistente, kits A/A/A e A/A/B, categorias e mapa de recargas' 'Validado no perfil próprio' 'Estrutura testada; habilidades e balanceamento distintos' 'P02; docs/batalhas/SEQ-01.md; P01' 'Mapear regras específicas de um modo sem copiar fórmulas protegidas'
Row C14 'Três frentes independentes' 'Relato Season 0 e referências históricas; arquitetura original desconhecida' 'PAR-01: alternância, isolamento, agregado, limites e cancelamento; 16 grupos' 'Validado no perfil próprio' 'Não demonstra threads nem equivalência à simultaneidade original' 'P02; docs/batalhas/PAR-01.md; P01' 'Apresentação simultânea e medição futura de carga real'
Row C15 'Chefe compartilhado' 'Relato de três frentes no mesmo chefe; HP único/partes/score não estabelecidos' 'PAR-02 pendente; clones independentes não resolvem estado compartilhado' 'Não implementado' 'Sem evidência suficiente para escolher modelo por arquivos' 'P02; PENDENCIAS.md PEN-05' 'Definir autoridade de HP, efeitos globais e ordem causal'
Row C16 'Segurança de cadeias' 'Limites internos do Dislyte não acessíveis; causa de mudanças de eventos desconhecida' 'Limites técnicos de extras, derivados e consequências; interrupção sem vencedor' 'Validado no perfil próprio' 'Proteção demonstrada apenas no nosso motor' 'P01; docs/batalhas/RES-01.md; docs/batalhas/PAR-01.md' 'Calibrar limites com catálogo legítimo; não atribuir problemas ao concorrente'
Row C17 'Reprodutibilidade e desempenho' 'Sem execução ou benchmark comparável do cliente' 'Replays e lote funcional de 1000 sem render, build Od' 'Validação funcional' 'Nenhuma prova de maior desempenho ou estabilidade geral' 'P01; P02' 'Benchmark comparável após apresentação; não extrapolar para servidor/mobile'
Row C18 'IA e catálogo de habilidades' 'Referências SkillConfig/SkillEditor em INI; nenhum catálogo interno acessado' 'Escolha automática básica ataque/Skip; habilidades explícitas nos testes' 'Implementação parcial' 'Cobertura própria limitada; catálogo original não comparado' 'A02; L01; docs/batalhas/SEQ-01.md' 'HAB-01 e política de decisão própria'
Row C19 'Personagens 3D e animações' 'Nenhum modelo/rig/animação editável solto identificado na coleta histórica' 'Sem arena visual, personagens ou animações finais' 'Não implementado' 'Recursos protegidos não avaliados nem reaproveitados' 'A02; docs/batalhas/PAINEL-BLOCOS.md' 'VIS-01 e ART-01 com recursos próprios/licenciados'
Row C20 'Artes 2D e interface' 'Imagens/XML/traduções do launcher acessíveis historicamente; UI interna não catalogada' 'Demo textual automatizada; COM-02/VIS-01 pendentes' 'Não implementado como interface jogável' 'Não há comparação de experiência ou qualidade visual' 'A02; P01; TAREFAS.md' 'Interface própria, acessibilidade e testes de usabilidade'
Row C21 'Músicas e áudio' 'FMODStudio presente; nenhum áudio/bank solto identificado no histórico' 'AUD-01 pendente' 'Não implementado' 'DLL de áudio não demonstra trilha, latência ou qualidade musical' 'A02; A04; TAREFAS.md' 'Áudio próprio/licenciado e medições de reprodução'
Row C22 'DJ Contest' 'Nenhum chart, regra ou temporização acessível; nomes de DLL não comprovam função' 'EXP-01 pendente; nenhum minijogo rítmico' 'Não implementado' 'Sem base de comparação funcional' 'A02; TAREFAS.md' 'Regras, charts e avaliação de sincronismo originais'
Row C23 'Progressão e persistência' 'Saves não examinados; economia e autoridade desconhecidas' 'CIC-01 pendente; snapshot/reset não é save versionado' 'Não implementado' 'Sem comparação de economia/progressão' 'A02; docs/batalhas/PAR-01.md; TAREFAS.md' 'Save próprio, versão e ciclo mínimo de recompensas'
Row C24 'Offline, serviços e plataformas' 'Modo oficial offline não demonstrado; cliente não executado na investigação' 'Testes e demo próprios funcionam localmente sem serviços; sem produto distribuído' 'Laboratório local validado' 'Vantagem de controle do nosso código, não superioridade do jogo completo' 'A05; P01; ESTADO.md' 'ENG-01 e futura distribuição; sem usar cliente original como SDK'
$sourceMap = [ordered]@{
    A01=(Join-Path $analysis 'relatorio.md'); A02=(Join-Path $analysis 'etapa-final-01\relatorio-final.md')
    A03=(Join-Path $analysis 'resumo.json'); A04=(Join-Path $analysis 'etapa-final-01\plugins.csv')
    A05=(Join-Path $analysis 'fase-1\relatorio-preparatorio.md'); P01=(Join-Path $project 'COMBATE.json')
    P02=(Join-Path $project 'docs\batalhas\ANALISE.md'); P03=(Join-Path $project 'docs\batalhas\TEMPO-PESQUISA.md')
    P04=(Join-Path $project 'docs\batalhas\REVISAO-PESQUISA-USUARIO-R03.md')
    P05=(Join-Path $project 'docs\batalhas\CONTRATO-CAUSALIDADE-R04.md')
}
$provenance = @(foreach ($key in $sourceMap.Keys) {
    [pscustomobject]@{Id=$key;Path=$sourceMap[$key];SHA256=(Get-FileHash -LiteralPath $sourceMap[$key] -Algorithm SHA256).Hash}
})
$testTotal = [int]$combat.Total + $combat.LimitTestsTotal + $combat.StageTestsTotal + $combat.TimingTestsTotal + $combat.ResolutionTestsTotal + $combat.SequentialTestsTotal + $combat.ParallelTestsTotal
$audit = [ordered]@{
    GeneratedUtc=[datetime]::UtcNow.ToString('o');Scope='Comparação documental e conferência pontual somente leitura; sem execução de jogo ou rede'
    HistoricalGeneratedUtc=$historical.GeneratedUtc;HistoricalFiles=$historical.Files;HistoricalPakCount=$historical.PakCount
    HistoricalEncryptedIndices=$historical.IndexProtection;OurTestTotal=$testTotal;OurTestTimestamp=$combat.CheckedUtc
    LocalChecks=$checks;ContentPathReferences=$contentPaths.Count;PluginContentReferences=$pluginReferences.Count
    L01='Somente nomes de caminhos Paths= do Engine.ini foram lidos; referências não comprovam plugins ativos nem código disponível'
    Sources=$provenance;Matrix=$rows.ToArray()
    Limits=@('8 arquivos conferidos por presença/tamanho/mtime, não inventário completo novo nem prova bit a bit',
      'Sem hash de arquivos do jogo; hashes somente de relatórios pequenos para proveniência',
      'Sem leitura de PAKs, EXEs, DLLs, logs, dumps, saves, cookies ou credenciais',
      'R04 é contrato do projeto; não especificação oficial comprovada do Dislyte',
      'Sem novas pesquisas online nem reexecução de testes; evidências existentes datadas',
      'Nenhum código, mídia ou recurso do jogo copiado; análise não concede permissão de reutilização')
}
$md = [Collections.Generic.List[string]]::new()
$md.Add('# Comparação de evidências — Dislyte e RPG Urbano')
$md.Add('')
$md.Add('18/11/2026. **Nosso núcleo tem cobertura reproduzível no escopo contratado; não há evidência suficiente para afirmar que o jogo é melhor, mais rápido ou equivalente ao Dislyte.**')
$md.Add('')
$md.Add('## O que foi conferido agora')
$md.Add('')
$same = @($checks | Where-Object { $_.Present -and $_.SameBytes -and $_.SameModifiedUtc }).Count
$md.Add("Relatórios de analise-dislyte reaproveitados. Oito arquivos locais conferidos: $same/8 com presença, tamanho e data de modificação iguais ao inventário anterior. Isso não certifica igualdade bit a bit nem o restante da instalação. Engine.ini contém $($contentPaths.Count) referências Paths=, das quais $($pluginReferences.Count) são caminhos de conteúdo de plugins; as outras são caminhos-base. Confirma a contagem histórica de 36 referências de plugins, não 38 plugins ativos.")
$md.Add('')
$md.Add('As contagens históricas de 6.572 arquivos, 5.576 PAKs e seus índices criptografados não foram recontadas nem revalidadas. Os dois chunks ausentes e a referência XGame.exe são divergências históricas de manifestos; não explicam limitações de combate ou qualidade do produto.')
$md.Add('')
$md.Add("Foram reutilizados $testTotal testes aprovados do projeto, 168 cenários/replays e 25 formações AP/replays. Não houve nova compilação, execução do cliente, acesso à rede, extração ou escrita na origem. Leitura direta de conteúdo limitada às linhas Paths= do Engine.ini. Não foram abertos saves, logs, dumps ou credenciais.")
$md.Add('')
$md.Add('## Matriz comparativa')
$md.Add('')
$md.Add('“Validado” significa testado contra nosso contrato. Não significa paridade externa. “Não implementado” é trabalho futuro, não um defeito dos blocos encerrados. CSV/JSON detalham evidências, fontes e próximos dados úteis por linha.')
$md.Add('')
$md.Add('| ID | Área | Situação própria | Conclusão comparativa |')
$md.Add('|---|---|---|---|')
foreach ($row in $rows) { $md.Add("| $($row.Id) | $($row.Area) | $($row.OurStatus) | $($row.Parity) |") }
$md.Add('')
$md.Add('## O que já podemos afirmar')
$md.Add('')
$md.Add('Temos controle do código, contratos explícitos, testes e liberdade para criar perfis próprios sem depender de serviços. Essa é uma capacidade demonstrada do laboratório, não prova de que a arquitetura interna do outro jogo seja inferior. Guarda opcional e regras configuráveis são diferenças de design, não melhorias comprovadas.')
$md.Add('')
$md.Add('Ainda faltam chefe compartilhado, interação jogável, catálogo amplo de habilidades, balanceamento, arte, animação, áudio, progressão e distribuição. Por isso a comparação com um jogo completo não tem resultado de superioridade. O lote de 1000 simulações sem renderização não mede desempenho gráfico, servidor ou consumo mobile.')
$md.Add('')
$md.Add('## Próximos passos que trazem evidência útil')
$md.Add('')
$md.Add('1. PAR-02: definir HP único, partes ou avatares/score; os arquivos atuais não resolvem PEN-05.')
$md.Add('2. Para fidelidade, obter casos observáveis com modo, versão/data, atributos, sequência de ações e resultado esperado; manter decisões autorais separadas de regras verificadas externamente.')
$md.Add('3. Após interface e habilidades ampliadas, comparar usabilidade, balanceamento e desempenho em condições equivalentes. Não repetir hashes ou inspecionar conteúdo protegido para responder questões que dependem de comportamento.')
$md.Add('')
$md.Add('## Proveniência e preservação')
$md.Add('')
foreach ($key in $sourceMap.Keys) { $md.Add("- ${key}: $($sourceMap[$key])") }
$md.Add('- L01: leitura pontual autorizada do Engine.ini da cópia de testes; somente caminhos de conteúdo.')
$md.Add('')
$md.Add('comparacao.json inclui checks e SHA-256 dos relatórios de referência; comparacao.csv contém 24 áreas. O procedimento está em scripts/comparar-evidencias-dislyte.ps1. Relatórios anteriores preservados. Nenhum ativo ou componente do jogo foi reutilizado no projeto.')
New-Item -ItemType Directory -Path $output | Out-Null
$rows.ToArray() | Export-Csv -LiteralPath (Join-Path $output 'comparacao.csv') -NoTypeInformation -Encoding utf8
$audit | ConvertTo-Json -Depth 10 | Set-Content -LiteralPath (Join-Path $output 'comparacao.json') -Encoding utf8
$md -join "`n" | Set-Content -LiteralPath (Join-Path $output 'comparacao.md') -Encoding utf8
[pscustomobject]@{Output=$output;Areas=$rows.Count;MetadataMatches=$same;CheckedFiles=$checks.Count;ContentPaths=$contentPaths.Count;PluginReferences=$pluginReferences.Count;ExistingTests=$testTotal} | ConvertTo-Json
