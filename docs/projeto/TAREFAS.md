> **Correção AUT-11 (22/11):** teste do usuário revelou ausência de decoder habilitado; WmfMedia da engine agora habilitado somente em Win64. Avisos de mídia passam a abrir no próprio hub, bloqueando ações por trás. Reteste de playback pendente; abertura automática continua desativada. Ver AUT-11-PLAYER-LOCAL.md e AUT-11-CORRECAO.json.

> **AUT-11 vigente (22/11):** [ensaio de player local](AUT-11-PLAYER-LOCAL.md) implementado e compilado, disponível por Rever abertura no hub. Vídeos autorais copiados para runtime ignorado. **Playback/áudio/flag não executados nem aceitos; AUT-11 parcial.** Autoabertura, legendas, mixer e arte lateral seguem pendentes. Sem telas/Android. Próximo: aceite de reprodução; AUT-12 é código independente possível por nova solicitação. Notas anteriores são históricas.

> **AUT-10 vigente (22/11):** [contrato de mídia local](AUT-10-MIDIA-LOCAL.md) implementado; 12 grupos novos e 82 regressões aprovados. Estados, skip, pausa, callbacks e fallback preparados; sem player ou alteração visual. Compilação registrada em AUT-10-BUILD.json. Próximo: AUT-11, backend e integração efetiva; Android continua adiado. Aceites e I/O de AUT-08/09 permanecem pendentes. Notas anteriores são históricas.

> **AUT-09 vigente (22/11):** [equipe, resultado e save local](AUT-09-EQUIPE-RESULTADO-SAVE.md) implementados; 64 grupos nativos focados e compilação Editor aprovados. Menu Equipe / Save, seleção validada, detalhes do resultado e codec v1 ligados ao adaptador Unreal. Aceite interativo e gravação/reabertura pela interface pendentes. Catálogo provisório continua com três aliados. Próximo código: AUT-10 (contrato de mídia); Android adiado. Notas anteriores são históricas.

> **AUT-08 vigente:** [estados de tela, pausa e abandono](AUT-08-FLUXO-TELAS.md) implementados/testados e compilados;82grupos de apresentação e35focados aprovados. Menus novos pendentes de aceite interativo. Correção do ponteiro aceita pelo usuário e salva no commit1400d4e. Próximo: AUT-09 (equipe/resultado/save). Android adiado; notas anteriores são históricas.

> **AUT-07 vigente:** [preferências locais](AUT-07-PREFERENCIAS.md) implementadas/testadas no modelo e adaptador compilado;64grupos de apresentação aprovados. Menu Opções, movimento/contraste/texto/enquadramento. Gravação/reabertura real e aparência pendentes; save de combate separado. Próximo código: AUT-08, estados/transições de tela. Android adiado; notas anteriores são históricas.

> **AUT-06 vigente:** [estados das habilidades](AUT-06-HABILIDADES.md) implementados/testados e compilados;32grupos focados aprovados, aparência nova pendente. AUT-05 recebeu aceite do usuário nos dois layouts e detalhes, sem problemas relatados. Próximo código: AUT-07, preferências/acessibilidade persistidas. Android adiado; notas anteriores são históricas.

> **AUT-05 vigente:** [inspeção de formações/escudo](AUT-05-INSPECAO.md) preparada, testada e compilada;13cenários,22grupos focados aprovados. Atalhos Abrir-Inspecao.cmd e Abrir-Inspecao-Classico.cmd. Somente leitura; aceite visual pendente. Próximo código: AUT-06, estados das habilidades. Android adiado; notas anteriores de AUT-05 por iniciar são históricas.

> **AUT-04 vigente:** [painéis e paginação](AUT-04-PAINEIS.md) implementados/testados e compilados;54grupos de apresentação aprovados. Habilidades, efeitos e locais usam a mesma base. Aceite visual pendente, Android adiado. Próximo código: preparação AUT-05 (formações/escudo); notas anteriores de AUT-04 por iniciar são históricas.

> **AUT-03 vigente:** [entrada comum mouse/toque](AUT-03-ENTRADA.md) implementada/testada e adaptador compilado;44grupos de apresentação aprovados. Sem execução gráfica ou Android; aceite interativo pendente. Próximo bloco de código: AUT-04, painéis/textos. Notas anteriores de AUT-03 ainda não iniciado são históricas.

> **AUT-02 / pasta principal:** [geometria e margens seguras](AUT-02-GEOMETRIA.md) implementadas/testadas, integração compilada;34grupos nativos aprovados. Trabalhar apenas em F:/Testes de jogos/Contos de Um Lobo - Jogo/rpg-urbano. Caches copiados foram preservados e regenerados; não reutilizar Intermediate ao mover o projeto. Próximo código: AUT-03; aceite visual/físico permanece pendente, Android adiado. Esta nota prevalece sobre a fila anterior.

> **Hub vigente21/11:** [panorama da cidade, sete locais e motion](HUB-PANORAMA-CIDADE.md), implementado/compilado; aceite visual pendente do usuário. AUT-01 pronto em código; AUT-02 parcial, depois AUT-03/04. Android adiado. Sem controle de tela nesta entrega. Esta nota substitui a prioridade anterior de iniciar AUT-01 e as âncoras antigas do terraço.

> **Entrega concluída:** [projeto Unreal estável e guia de abertura](UNREAL-PROJETO-ESTAVEL.md). Controle de tela encerrado por solicitação; nova interação com o editor exige nova liberação. Atualização documental/Git apenas após essa solicitação.

> **Estado vigente — 21/11/2026:** [consolidação das 15 entregas e decisões posteriores](CONSOLIDACAO-2026-11-21.md). ENG-ESTAVEL concluído no Windows local: projeto fixo, mapa próprio, abertura/reabertura e Play observados. Próximo: AUT-01–04. ESC comum e seletor já aceitos não são reabertos. Android SDK/NDK/JDK localizados e boot do AVD confirmado pelo usuário; caminhos já persistem, mas ValidatePlatforms retorna Android INVALID r27c; diagnóstico e APK pendentes. Percentuais recalculados no painel de progresso. As notas datadas abaixo preservam histórico e não reabrem decisões substituídas.

# Tarefas — fila vigente

Atualizado em 19/11/2026. [Checklist histórico integral](historico/2026-11-18/TAREFAS.md.txt); [blocos e legenda](../batalhas/PAINEL-BLOCOS.md).

## Próxima ação vigente

**Desenvolvimento sem aguardar entregas:** [fila independente AUT-01–16](FILA-DESENVOLVIMENTO-INDEPENDENTE.md). Plano apenas, sem execução. Primeiro bloco proposto: AUT-01, apresentação Desktop/Mobile/Auto; pesquisas do usuário não bloqueiam essa base.

- [x] Organizar [coleta por grupos](../../entregas/README.md), modelo de entrega e pastas recebidos locais.
- [x] Receber e estudar12 documentos dos grupos00–11; propostas e divergências mapeadas, sem código novo.
- [x] Inventariar versões recebidas e comparar com contratos; [revisão e conflitos](REVISAO-ENTREGAS-2026-11-20.md). Originais preservados.
- [ ] Quando o usuário informar limite/disponibilidade e retomar: AUT-01, depoisAUT-02/03/04; estudar só novas versões de material, não repetir as12leituras.
- [ ] Conferir caminhos/compatibilidadeSDK/NDK/JDK/AVD existentes; Android Studio instalado, sem instalação/configuração automática. [Matriz de teste](ANDROID-DEPENDENCIAS.md).
- [x] Receber respostas ENT-01–15; decisões principais consolidadas em RESPOSTAS-ENT-2026-11-20.md.
- [ ] Formalizar os detalhes técnicos residuais no bloco afetado; não repetir o questionárioENT. Nenhuma dessas expansões está implementada.
- [x] Seletor no chão implementado e aceito visualmente pelo usuário; encerrado no protótipo atual.

Ordem detalhada e frentes independentes em [PROXIMAS-FASES.md](PROXIMAS-FASES.md). Primeiro fechar aceite visual atual; depois UI pequena/mobile e player, sem esperar catálogo completo de habilidades. Percentuais revisados em [PROGRESSO-GERAL.md](PROGRESSO-GERAL.md).

- [x] Receber, preservar e revisar a pesquisa de habilidades/itens.
- [x] Registrar explicitamente todos os pontos da devolutiva em [PENDENCIAS.md](PENDENCIAS.md), com prioridades, exemplos e campos de resposta.
- [x] P1: ESC-01 a ESC-07 respondidos; escudo padrão implementado e testado, com mínimo de1 ponto na aplicação válida aprovado posteriormente. [Contrato e limites](ESCUDO-PADRAO.md).
- [ ] Em paralelo documental, pesquisar VIS/REG/SIS se desejado; não precisa fechar todas as extensões para avançar.
- [ ] Aceite visual da revisão atual, após autorização específica de abertura; independe do catálogo completo.
## Concluído

- [x] Fundação, ambiente, regras/tempo/R04 e modos no escopo dos aceites registrados no painel.
- [x] COM-02: protótipo textual, comandos, resultado e reinício.
- [x] HAB-01: cinco partes, kits ativo/passivo/híbrido, condição e automação.
- [x] CIC-01: composição de equipe, recompensa única e save v1 entre batalhas.
- [x] Preparação da persistência: codec separado do armazenamento Windows; 213 testes gerais aprovados.
- [x] ENG-01 — ponte Windows: instalação conferida, módulo/ponte compilados e 11 checkpoints aprovados em dois processos Unreal independentes.

Não repetir verificações concluídas sem mudança, falha ou dependência pertinente.

## Entregas recentes e aceite visual restante

- [x] ENG-01 — observação gráfica inicial: mapa, menus, RTX 3060/D3D12/SM5 e encerramento normal confirmados. Aceite integral permanece parcial; [relatório](ENG-01-GRAFICO.md).
- [x] ENG-G01 — diagnóstico: três testes internos de mensagens identificados (7+4+3 falhas em pt-BR); passaram em inglês. Inglês priorizado no ambiente Unreal; causa detalhada não corrigida.
- [x] ENG-G02 — observação delimitada: bSendUsageData=False solicitado nas duas comparações, sem os marcadores anteriores. Sem auditoria ou isolamento de rede; usuário não autorizou mudanças de rede.
- [x] ENG-01 — geração do pacote: Development/Win64, build/cook/stage/archive com saída0; 45 arquivos (~501,6 MiB com PDB). ENG-01-PACOTE-BUILD.md.
- [x] ENG-01 — execução básica do pacote: mapa renderizado fora do editor e encerramento normal; entrada/processo principal com saída0. ENG-01-PACOTE-RUNTIME.md; não valida Windows limpo ou gameplay.
- [x] VIS-01 — plano: [arquitetura, etapas e matriz de aceite](VIS-01-PLANO.md). Plano inicial preservado; implementação posterior registrada nas linhas seguintes.
- [x] VIS-01A: adaptador de sessão visual e cenário compartilhado com o console; preservar regras e testar contratos.
- [x] VIS-01B/C — entrega funcional3×3: cena compilada, vitória após20 ações, passiva/condição/recarga e reinício observados no pacote; processos com saída0. [Aceite parcial](VIS-01C.md).
- [ ] VIS-01B/C — fechamento visual: inspecionar a revisão já compilada de HP/escudo, descrições/alvos e formas geradas do elenco; validar1/3/5 e outras resoluções. Não iniciar arte/áudio por inferência.

Uma entrega por vez. Não abrir editor, instalar componentes nem iniciar serviços por inferência de uma tarefa documental. Download/instalação e escolhas definitivas continuam com decisão específica.

## Áreas posteriores

- [ ] Localização do jogo em português: definir textos traduzíveis e recursos separados, seleção de idioma, fontes/acentos, formatação regional e testes no pacote. Não depende de usar o editor em português; ainda não implementada nem validada.
- [ ] ART-01: direção artística original, personagem representativo, fontes e direitos.
- [ ] AUD-01: áudio próprio/licenciado e integração funcional.
- [ ] EXP-01: delimitar campanha e experiência rítmica antes da implementação.
- [ ] Multiplataforma: confirmar ferramentas/dispositivos e validar Android/iOS; Linux e Windows ARM64 conforme decisão de produto. Sem presumir validação pela presença de opções no instalador.

## Extensões acrescentadas ao longo do projeto

- [ ] PVE-09: fases de kit por HP/turno e eventos únicos.
- [ ] PVE-06: sobrevivência/score/recompensas; detalhar o exemplo relatado de Treva Fluxo.
- [ ] PVE-07/08: identificar o exemplo dos Milagres e confirmar eventual retorno cíclico de times.
- [ ] Seletores de área/dinâmicos e efeitos entre times, com alcance explícito.
- [ ] Silêncio/supressão, passivas durante combate/recarga interna, custos/condições compostas e automação estratégica.
- [ ] Save de runtime, migrações, backups/concorrência e economia de produção: contratos próprios antes de implementar.

As questões e IDs históricos permanecem em [PENDENCIAS.md](PENDENCIAS.md). Não tratar essas extensões como reprovação dos blocos já aceitos.

## Encerramento de uma entrega

Atualizar estado, tarefa/painel pertinentes e evidências; relatar somente testes executados. Preservar aceites datados e evitar acumular resumos contraditórios na síntese vigente. Não iniciar automaticamente a próxima tarefa.


- [x] Separar apresentação Standard em profundidade e Classic lateral; testes nativos de posições aprovados. Não altera perfis de regras.
- [x] Inspeção inicial das duas apresentações no pacote; legibilidade integral continua pendente. [Contrato e chamadas](VIS-01-LAYOUTS.md). Não marcar como visualmente aprovado por compilar.


Apresentações verificadas no pacote em19/11/2026: Standard sem argumento e Classic por chamada explícita, ação/resposta/reinício observados com mesmos valores. Entrada e processo principal de ambas saíram0. Aceite visual integral parcial por sobreposição de etiquetas ao fundo e demais pendências. Relatório: docs/projeto/VIS-01-LAYOUTS-RUNTIME.md. Aplicativos encerrados; próximo ajuste é legibilidade/áreas de seleção.


Contrato inicial de interação: [VIS-01-INTERACAO-R01.md](VIS-01-INTERACAO-R01.md). Implementação posterior registrada abaixo; sexta posição permanece aberta.


Interação inicial implementada e compilada em19/11/2026: ícone/painéis, faixa de escudo real, gesto prolongado, nível opcional e seleção padrão com marcador/sugestão/confirmação. Regressão213+6 e6 novos grupos aprovados. Ainda sem teste visual desta revisão; cálculo percentual do escudo segue separado dos escudos fixos antigos. Relatório: docs/projeto/VIS-01-INTERACAO-IMPLEMENTACAO.md. Próximo: inspeção autorizada do pacote atualizado.

## Incremento compartilhado com o clássico

- [x] Adaptador explícito Classic, ataque/defesa do núcleo, efeitos e consultas compartilhados;6 novos grupos nativos aprovados e build Editor aprovado.
- [ ] Gerar pacote atualizado e realizar aceite visual específico dos dois perfis, formações e resoluções, após autorização de abertura.
- [x] Fechar ESC-01 a ESC-07 e implementar o caso comum; extensões futuras permanecem separadas.

Roteiro e limites: [VIS-01-CLASSICO-INTERACAO.md](VIS-01-CLASSICO-INTERACAO.md). Sem inventário/especiais cadastrados no encontro clássico e sem equivalência declarada com Dislyte.

## Pesquisa de habilidades recebida —19/11/2026

Documento do usuário preservado e comparado aos contratos. [Revisão e matriz](REVISAO-PESQUISA-HABILIDADES-ITENS.md). Q07/Q08 e proposta de frentes não substituem R04, defesa atual ou PAR-02. Fontes externas ainda não auditadas; escudo percentual permanece em especificação (base, instante/recalculo, reaplicação e consumo), não mais aguardando envio do documento. Próximo passo: fechar esse contrato mínimo. Sem código/testes novos nesta recepção; aceite visual atual permanece pendente.

## Proposta recebida — navegação do hub

- [x] Associar as quatro capturas aos nomes antigos do texto e registrar compatibilidade/limitações.
- [ ] Antes de HUB-01A: confirmar direção do avatar, panorama, destinos funcionais e escopo local; perguntas em [HUB-01-REFERENCIAS.md](HUB-01-REFERENCIAS.md).
- [ ] Planejar contrato do hub separado de CombatProfile/ArenaLayout; não iniciar cena/contas/lojas por inferência do documento recebido.

Não interrompe nem encerra o aceite VIS atual.

## HUB-01A — imagem/vídeo por dispositivo

- [x] Registrar regras de vídeo e enquadramento recebidas diretamente do usuário; implementar política neutra e testes10/10.
- [x] Criar catálogo local versionado e validação de recursos previstos, sem mídia de terceiros incorporada.
- [x] Receber arquivos e conferir formatos/dimensões; autoria própria declarada pelo usuário. [Inventário](HUB-01A-RECURSOS.md).
- [ ] Confirmar recortes/composição e destinos da imagem: material atual mostra um terraço contínuo, não três alas distintas.
- [x] Ligar por código o subconjunto hub do catálogo e renderer/rotas; sem aceite de execução.
- [ ] Player e preferências antes da janela; orientação/restauração reais no celular.
- [ ] Aceite gráfico e em aparelho após autorização própria; sem atribuir execução real a teste de geometria.

[Contrato e limites](HUB-01A-APRESENTACAO.md). Pode avançar como base independente sem modificar combate.

## HUB-01B — acessos provisórios

- [x] Cadastrar Evento Especial no outdoor e Batalha no dispositivo; terceiro adiado pelo usuário.
- [x] Escrever renderer/leitor de catálogo e roteador do hub;5 novos grupos nativos aprovados e compilação Unreal registrada.
- [x] Observar fluxo básico Windows hub/evento/arena/volta; aceite de outras proporções, mobile e posições permanece parcial.
- [ ] Evento permanece painel provisório até definição de conteúdo.

[Definição e limites](HUB-01B-ACESSOS.md). Sem mudança de combate ou abertura de aplicativo.

## Briefing visual do hub recebido

- [x] Ler preferências/paleta, nome e texto do painel sem alterar respostas do usuário; conferir procedência das fontes.
- [x] Aplicar estilo por código e texto definido;5 grupos novos e compilação Editor aprovados. Fontes locais integradas posteriormente, conforme HUB-IDENTIDADE.md.
- [x] Fechar Cormorant Garamond/Lora, codinome LOBO, emblema/pulso e receber logo WebP localmente.
- [x] Conectar fontes locais com licenças e PNG da logo ao HUD; compilação aprovada. [Integração](HUB-IDENTIDADE.md).
- [ ] Integrar acessibilidade do sistema/persistência e ampliar aceite visual/dispositivo.
- [ ] Aceite visual e toque continuam pendentes.

[Revisão](HUB-BRIEFING-REVISAO.md).


### Fechamento e próxima entrega

✅ Usuário confirmou texto sem corte, seleção funcional e batalha conforme combinado. Fontes/logo conectadas; aceite integral ainda parcial. ✅ Seletor no chão corrigido e aceito visualmente pelo usuário. Investigar apresentação mobile separadamente; fluidez aguarda critérios. Prévia por Abrir-Preview.cmd; editor por Abrir-Editor.cmd, ambos usando build aprovado. Nesta entrega final só se validaram os lançadores sem abrir programas. [Evidências](HUB-IDENTIDADE.md).

Seletor: correção por linhas3D com profundidade junto à base do modelo implementada e posteriormente aceita visualmente pelo usuário; o assistente não abriu a prévia nessa entrega. [Escopo e roteiro](VIS-SELETOR-CHAO.md). Não altera regras nem resolve mobile/fluidez; não repetir esse aceite sem mudança ou defeito novo.

Mobile: [proposta revisada e sequência mínima](MOBILE-PLANO-REVISAO.md). HUD atual é Canvas, não UMG; preview/renderizador não troca automaticamente DeviceProfile Windows. Próxima implementação a planejar: apresentação Desktop/Mobile/Auto separada de plataforma, antes de presumir migração UMG ou instalar SDK. Sem implementação nova por esta revisão.

## Respostas ENT recebidas —20/11/2026

**ENT-01–15 respondidos pelo usuário; implementação ainda pendente.** [Decisões vigentes e detalhes por bloco](RESPOSTAS-ENT-2026-11-20.md). PreservarESC/R04/Defesa atual até mudança explícita por implementação. Demo:1 Felipe jogável/1 arena;A2passiva. Pausa/abandono,panmobile,vídeo e futuraeconomia agora têm direção consolidada. Não repetir perguntas principais da revisão anterior;formalizar apenas detalhes técnicos no bloco afetado. Nenhuma execução autorizada por esta atualização.

Ponto1/FelipeA2: regra de cargas,limite,consumo,A1eStandardShield definida;destino confirmado no aliado vivo com menor percentualHP,incluindoFelipe. [Contrato](FELIPE-A2-POSTURA-VIGILIA.md). Não repetir essas perguntas;casos de borda e implementação ficam no bloco futuro. Formação6×6 não implementada por essa decisão.

Ponto2 respondido: Block3%/Michel10% total, teto50% geral e100% especial; True Damage/DoT inelegíveis. Mitigação somente por fonte, aditiva até50%. True Damage ignoraDEF/mitigação/Defender/Block e respeita invencibilidade/escudo. [Contrato e limites](BLOQUEIO-MITIGACAO-DANO-PONTO2.md). Especificado, não implementado; demais perguntas técnicas só no contrato afetado.

Ponto3/SIS-02 definido:trilho de dano separado do trilhoImmunity→chance→ACC/RESIST,piso20%resistência não irresistível;crítico base150%total. Novos sistemas comHP/escudo/dano/scoreint64,teto10^12,intermediáriosdouble/floorporhit,preservandoDefesa1→0 ecenáriospequenos. [Contrato e preparação](FUNIL-ESCALA-NUMERICA-PONTO3.md). Migração/limites/precisão/replay ainda exigem implementação e testes;nenhum código alterado.

Adendo técnico recebido20/11: [matriz vigente](ADENDO-TECNICO-CONTRATOS-2026-11-20.md). Debuff exigehitnãoMiss/nãoBlock;supressão/prevenção e reaçõespendentes detalhadas;SilverShieldcoexiste econsomeapósStandard;ondaslimpamcomunsantesdecurar;scorearredondado uma vez. Temas respondidos não são perguntas abertas de produto;bordas técnicas estão delimitadas no adendo. Implementação ainda pendente.

ConferênciaAndroid20/11 concluída em leitura:Studio D:\Android presente;SdkPath vazio,SDK/NDK/AVD não localizados nas configurações/caminhos examinados. Java25doStudio fora da matriz suportada doGradle8.13/AGP8.13.0daengine. [Resultado e próximos passos](ANDROID-CHECK-2026-11-20.md). Não instalar/configurar/executar automaticamente;buscar SDK personalizado ou preparar componentes/JDK compatível sob autorização. AUTWindows não bloqueado.

**Complemento final recebido20/11:** [A2, ACC/RESIST e ponto fixo](COMPLEMENTO-FINAL-A2-ACC-PONTO-FIXO.md). Bordas de A2 e equação/irresistível respondidos; teto10^12 por saturação; double substituído por ponto fixo de escala10000 no núcleo futuro. Não repetir as perguntas resolvidas. Determinismo bit a bit é meta a verificar, não aceite existente. Nenhum código/teste executado nesta recepção.

Recepção21/11: [novidades e política de versionamento](RECEBIMENTO-NOVIDADES-2026-11-21.md).15Markdown recebidos entram noGit;1novo/1alterado noHub,demais13sem mudança.10PNG locais(8variantes+2indicativos).7destinos do novo panorama são direção futura,não sistemas ativados;AUT-01–04 preservados. Sem código/build/teste ou abertura de programas.

AUT-12 (22/11): marcador de runtime implementado, cinco grupos de geometria aprovados; compilação em AUT-12-BUILD.json. Visual/Shipping/terreno pendentes. Próximo AUT-13. Clique inicial: aceite provisório, REVER. Ver docs/projeto/AUT-12-MARCADOR-RUNTIME.md.
