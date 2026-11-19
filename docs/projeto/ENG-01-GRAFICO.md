# ENG-01 — abertura gráfica: validação parcial

19/11/2026. **⚠️ Etapa gráfica parcialmente validada; aceite integral pendente.** A ponte Windows anteriormente aprovada mantém seu aceite.

## Plano e execução realizados

Abrir somente o projeto de avaliação já compilado, conferir os 18 hashes de fonte/stage e a versão da engine, carregar o mapa interno `/Engine/Maps/Templates/Template_Default`, observar renderização e resposta da interface e encerrar normalmente. Não iniciar Play, arena própria, empacotamento ou testes mobile. Não alterar regras, mapas, recursos ou saves do jogador.

O script `scripts/abrir-unreal-avaliacao.ps1` realizou a abertura com limite solicitado de 30 FPS, som desativado e redução de concorrência de shaders. Os argumentos de desativação de analytics, portal, controle de versão e Live Coding expressam a intenção de inicialização; não constituem garantia de isolamento ou desativação efetiva de todos os subsistemas.

## Resultados confirmados

| Verificação | Resultado |
|---|---|
| Engine | Unreal 5.8.4, CL58902052 |
| Mapa | Template_Default renderizado, piso e céu visíveis |
| Interface | Menu Arquivo abriu e fechou em resposta à interação |
| GPU selecionada | NVIDIA GeForce RTX 3060, 12115 MB de memória dedicada informados |
| Renderização efetiva | Direct3D 12, SM5; driver 621.24 |
| Inicialização da engine | 403,54 segundos segundo o log; preparação inicial de shaders/catálogo |
| Compilação de shaders | Quatro workers locais |
| Encerramento | Alt+F4, janela desapareceu e log registrou Editor shut down, Exiting e fechamento às 03:21:22 UTC |

Não foi capturado código numérico de saída. Não foi observado erro fatal. O limite de 30 FPS foi aceito pelo console da engine, mas não houve medição de FPS, benchmark ou validação de SM6/Lumen/Nanite. O tempo inicial não é estimativa de desempenho do jogo.

## Ressalvas que impedem aceite integral

1. **ENG-G01 — erros de automação:** 14 ocorrências `LogAutomationTest: Error: Condition failed` às 03:18:01 UTC, antes da inicialização do editor terminar. Sem identificação suficiente de teste/causa no trecho observado. A origem e o impacto permanecem desconhecidos; não atribuir ao nosso combate nem declarar inofensivos.
2. **ENG-G02 — comunicação externa:** o log registrou início de sessão AnalyticsET e um POST de telemetria da Epic ainda pendente no encerramento, apesar de `-NoAnalytics`. Não foi confirmada entrega de dados, nem demonstrado isolamento de rede. O argumento não foi suficiente para impedir a tentativa. Não repetir abertura antes de definir isolamento efetivo e obter autorização específica.
3. Avisos de PSO e preparação inicial foram observados; a sessão não comprova estabilidade prolongada nem desempenho de gameplay.

O relatório versionado não reproduz identificadores de sessão/usuário ou a URL com parâmetros. O log integral permanece local em build ignorado. Não publicar logs brutos sem revisão de dados.

## Evidências e limites

[JSON consolidado](../relatorios/atuais/ENG-01-GRAFICO.json). Log local: `build/engine-eval/graphical/20261119-031153/editor.log`. Observação visual realizada no próprio editor, sem arquivar capturas com outras aplicações. A ausência da janela foi confirmada após encerramento.

Os 213 testes standalone e os 11 checkpoints em dois processos commandlet são evidências anteriores, não foram repetidos nesta sessão. Não houve alteração do núcleo, instalação de componentes ou criação de arena. O editor produziu caches e configurações normais de avaliação; isso não equivale a ausência de escrita da ferramenta.

## Roteiro restante, por prioridade

1. Leitura direcionada dos registros existentes e, se necessário, do código local pertinente da engine para identificar ENG-G01; sem executar componentes.
2. Definir mecanismo verificável de bloqueio de rede para ENG-G02. Não modificar firewall, configurações da engine ou instalar ferramentas sem autorização correspondente.
3. Somente após autorização específica de programa/finalidade, repetir a menor verificação necessária e revisar log sanitizado. Evitar repetir compilação e regressão do núcleo sem mudança pertinente.
4. Após resolver ou delimitar formalmente as ressalvas, planejar o primeiro pacote Windows de avaliação; depois VIS-01, sem duplicar regras na apresentação.

Nesta entrega documental foram usados somente PowerShell e Git. Futuro uso de qualquer programa exige nova autorização específica conforme solicitado pelo usuário. Nenhuma nova abertura da Unreal faz parte desta atualização.
