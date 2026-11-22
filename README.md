> **Vigente — 22/11, ponto 1 entregue:** [Felipe e efeitos contínuos](docs/projeto/HAB-FELIPE-PILOTO.md): Bleed/Poison, A1/A2/A3 e piloto local implementados e testados. Aceites visuais/mobile separados. SIS-02-B/C, REG e A2 não estão mais por iniciar. Próximo grupo possível: ESC/SIS/PvE, **fora da autorização desta entrega**. Notas anteriores são histórico.

> **SIS-02-A concluído — 22/11:** [mapa numérico e compatibilidade](docs/projeto/SIS-02-A-MAPA-NUMERICO.md): 22 âncoras conferidas, cortes de migração e testes futuros definidos. Código/fórmulas/saves intactos; nenhuma bateria ou engine executada. Próxima entrega: **SIS-02-B, aritmética segura**, ainda não iniciada. Save v1 não contém HP; migração de HP não força nova versão por si só. Esta atualização prevalece sobre a indicação histórica de A ainda por iniciar.

> **Fila vigente pós-AUT — 22/11:** [ordem reconciliada de execução](docs/projeto/ORDEM-EXECUCAO-POS-AUT.md). Próxima tarefa: **SIS-02-A, mapa numérico/compatibilidade**; depois aritmética segura, migração, outcomes/RNG, funil, controles e Felipe A2. Aceites VIS/mídia seguem em trilha própria; ESC/TIME/RES/SEQ/PAR encerrados não são reabertos por rotina. Primeiro clique: provisório, **REVER**. Nesta revisão somente planejamento/documentação, sem execução das próximas tarefas. Esta ordem prevalece sobre prioridades históricas abaixo.

> **Estado vigente — AUT-12 a AUT-16 (22/11):** [entregas, verificações e próximos passos](docs/projeto/CONSOLIDACAO-AUT-12-16.md). Marcador runtime, fila visual, schema de referência, stage verificado e QA por área entregues em escopos próprios. Aceites gráficos/Shipping/mobile permanecem pendentes; AUT-11 ainda parcial. Clique inicial: aparentemente correto, **REVER** pelo usuário. Sem telas/editor/cook/Android/internet nesta rodada. Esta nota prevalece sobre os estados históricos abaixo.

> **Correção AUT-11 (22/11):** teste do usuário revelou ausência de decoder habilitado; WmfMedia da engine agora habilitado somente em Win64. Avisos de mídia passam a abrir no próprio hub, bloqueando ações por trás. Reteste de playback pendente; abertura automática continua desativada. Ver AUT-11-PLAYER-LOCAL.md e AUT-11-CORRECAO.json.

> **AUT-11 vigente (22/11):** [ensaio de player local](docs/projeto/AUT-11-PLAYER-LOCAL.md) implementado e compilado, disponível por Rever abertura no hub. Vídeos autorais copiados para runtime ignorado. **Playback/áudio/flag não executados nem aceitos; AUT-11 parcial.** Autoabertura, legendas, mixer e arte lateral seguem pendentes. Sem telas/Android. Próximo: aceite de reprodução; AUT-12 é código independente possível por nova solicitação. Notas anteriores são históricas.

> **AUT-10 vigente (22/11):** [contrato de mídia local](docs/projeto/AUT-10-MIDIA-LOCAL.md) implementado; 12 grupos novos e 82 regressões aprovados. Estados, skip, pausa, callbacks e fallback preparados; sem player ou alteração visual. Compilação registrada em AUT-10-BUILD.json. Próximo: AUT-11, backend e integração efetiva; Android continua adiado. Aceites e I/O de AUT-08/09 permanecem pendentes. Notas anteriores são históricas.

> **AUT-09 vigente (22/11):** [equipe, resultado e save local](docs/projeto/AUT-09-EQUIPE-RESULTADO-SAVE.md) implementados; 64 grupos nativos focados e compilação Editor aprovados. Menu Equipe / Save, seleção validada, detalhes do resultado e codec v1 ligados ao adaptador Unreal. Aceite interativo e gravação/reabertura pela interface pendentes. Catálogo provisório continua com três aliados. Próximo código: AUT-10 (contrato de mídia); Android adiado. Notas anteriores são históricas.

> **AUT-08 vigente:** [estados de tela, pausa e abandono](docs/projeto/AUT-08-FLUXO-TELAS.md) implementados/testados e compilados;82grupos de apresentação e35focados aprovados. Menus novos pendentes de aceite interativo. Correção do ponteiro aceita pelo usuário e salva no commit1400d4e. Próximo: AUT-09 (equipe/resultado/save). Android adiado; notas anteriores são históricas.

> **AUT-07 vigente:** [preferências locais](docs/projeto/AUT-07-PREFERENCIAS.md) implementadas/testadas no modelo e adaptador compilado;64grupos de apresentação aprovados. Menu Opções, movimento/contraste/texto/enquadramento. Gravação/reabertura real e aparência pendentes; save de combate separado. Próximo código: AUT-08, estados/transições de tela. Android adiado; notas anteriores são históricas.

> **AUT-06 vigente:** [estados das habilidades](docs/projeto/AUT-06-HABILIDADES.md) implementados/testados e compilados;32grupos focados aprovados, aparência nova pendente. AUT-05 recebeu aceite do usuário nos dois layouts e detalhes, sem problemas relatados. Próximo código: AUT-07, preferências/acessibilidade persistidas. Android adiado; notas anteriores são históricas.

> **AUT-05 vigente:** [inspeção de formações/escudo](docs/projeto/AUT-05-INSPECAO.md) preparada, testada e compilada;13cenários,22grupos focados aprovados. Atalhos Abrir-Inspecao.cmd e Abrir-Inspecao-Classico.cmd. Somente leitura; aceite visual pendente. Próximo código: AUT-06, estados das habilidades. Android adiado; notas anteriores de AUT-05 por iniciar são históricas.

> **AUT-04 vigente:** [painéis e paginação](docs/projeto/AUT-04-PAINEIS.md) implementados/testados e compilados;54grupos de apresentação aprovados. Habilidades, efeitos e locais usam a mesma base. Aceite visual pendente, Android adiado. Próximo código: preparação AUT-05 (formações/escudo); notas anteriores de AUT-04 por iniciar são históricas.

> **AUT-03 vigente:** [entrada comum mouse/toque](docs/projeto/AUT-03-ENTRADA.md) implementada/testada e adaptador compilado;44grupos de apresentação aprovados. Sem execução gráfica ou Android; aceite interativo pendente. Próximo bloco de código: AUT-04, painéis/textos. Notas anteriores de AUT-03 ainda não iniciado são históricas.

> **AUT-02 / pasta principal:** [geometria e margens seguras](docs/projeto/AUT-02-GEOMETRIA.md) implementadas/testadas, integração compilada;34grupos nativos aprovados. Trabalhar apenas em F:/Testes de jogos/Contos de Um Lobo - Jogo/rpg-urbano. Caches copiados foram preservados e regenerados; não reutilizar Intermediate ao mover o projeto. Próximo código: AUT-03; aceite visual/físico permanece pendente, Android adiado. Esta nota prevalece sobre a fila anterior.

> **Hub vigente21/11:** [panorama da cidade, sete locais e motion](docs/projeto/HUB-PANORAMA-CIDADE.md), implementado/compilado; aceite visual pendente do usuário. AUT-01 pronto em código; AUT-02 parcial, depois AUT-03/04. Android adiado. Sem controle de tela nesta entrega. Esta nota substitui a prioridade anterior de iniciar AUT-01 e as âncoras antigas do terraço.

# RPG Urbano — Mundus Lupis Disco / codinome LOBO

Protótipo original de RPG urbano e musical. Núcleo C++20 independente, apresentação atual na Unreal Engine 5.8.4 e laboratório textual preservado. Demo recortada: Felipe, uma arena e inimigos provisórios; não é o escopo comercial completo.

**[Progresso por área: aproximadamente 22% da demo](docs/projeto/PROGRESSO-GERAL.md)** · [Estado](docs/projeto/ESTADO.md) · [Próximas tarefas](docs/projeto/TAREFAS.md) · [Entregas](entregas/README.md)

## Abrir na Unreal sem comandos

1. Abra [RpgEngineProbe.uproject](experiments/unreal/RpgEngineProbe/RpgEngineProbe.uproject), pelo Explorador ou pelo navegador de projetos da Unreal.
2. Alternativa nesta máquina: **Abrir-Editor.cmd**, na raiz. Chama a Unreal diretamente, sem PowerShell, priorizando inglês no editor.
3. O mapa próprio `PrototypeHub` abre para edição. Clique **Play** para mostrar o hub; Batalha leva à arena. Pare Play para voltar a editar.

Compilação, mapa salvo, reabertura normal pelo `.uproject` e Play foram observados em21/11. [Guia completo, evidências e limitações](docs/projeto/UNREAL-PROJETO-ESTAVEL.md). A associação normal usa o idioma preferido atual do editor; localização do jogo é independente.

O projeto fixo está em `experiments/unreal/RpgEngineProbe`, apesar do nome histórico experiments. Edite esse projeto; `build/p` contém snapshots antigos preservados. Source, Config e o mapa próprio são versionados; caches, binários e preferências locais não. Imagens/vídeos não acompanham o Git. Em outro checkout, recursos locais precisam ser fornecidos e o C++ compilado; `scripts/preparar-editor-estavel.ps1` prepara somente recursos ausentes, conferindo hashes e preservando arquivos diferentes.

**Abrir-Preview.cmd** abre a prévia do projeto estável atual. **Abrir-Preview-Mobile.cmd** abre sua apresentação retrato no Windows; não é um APK. Mudanças C++ precisam de compilação; salvar um mapa não recompila código.

## Organização

| Caminho | Responsabilidade |
|---|---|
| include/rpg / src | Interfaces e implementações canônicas do núcleo |
| app / tests | Executáveis e testes próprios |
| experiments/unreal/RpgEngineProbe | Projeto Unreal estável, apresentação e mapa |
| data | Catálogos e configurações de dados versionados |
| assets / entregas | Recursos locais e materiais recebidos; mídias fora do Git |
| scripts / tools | Automação e verificações próprias |
| docs/projeto | Estado, decisões, tarefas e estimativas |
| docs/batalhas / docs/relatorios | Contratos, aceites e resultados consolidados |
| build | Compilados, snapshots e logs locais ignorados |

O módulo Unreal compila fontes canônicas por unidades de ligação; o núcleo não depende da engine. A apresentação ainda usa Canvas/C++; não é um conjunto completo de Widgets/Blueprints editáveis visualmente. Não migrar para UMG sem avaliar um bloco concreto.

## Testes e laboratório textual

Em ambiente PowerShell que permita scripts: `scripts/testar-combate.ps1`. Não instala ferramentas nem altera a política do Windows. Jogar.cmd e Jogar-Classico.cmd usam o compilado local; seleção/save e recompensa continuam com os contratos do laboratório, não economia comercial.

Histórico:213 testes gerais e suítes incrementais aprovados; ponte anterior11 checkpoints em2 processos. Nesta entrega,11 checkpoints passaram no novo projeto estável, saída0. Não somar esses números nem afirmar que213 rodaram dentro da engine. [Painel de blocos](docs/batalhas/PAINEL-BLOCOS.md).

## Estado e próximo trabalho

[Consolidação das15 entregas](docs/projeto/CONSOLIDACAO-2026-11-21.md) distingue decisões de implementação. ESC comum e seletor aceitos são preservados; ponto fixo, novo funil, A2 de Felipe, SilverShield e demais extensões ainda exigem seus blocos.

AUT-01 implementado; próximo: completar AUT-02 e seguir AUT-03/04 (áreas seguras, entrada e painéis). Arte, áudio, roteiro e modelos podem ser produzidos separadamente com os contratos definidos. [Fila independente](docs/projeto/FILA-DESENVOLVIMENTO-INDEPENDENTE.md).

Android: componentes instalados e boot de AVD confirmado pelo usuário. Caminhos persistem nas preferências da Unreal, mas a verificação sem variáveis injetadas retornou Android INVALID r27c; diagnóstico e APK pendentes. Não interpretar preview Windows como validação de aparelho. Nenhuma instalação ou mudança de rede nesta entrega.

## Colaboração e direitos

[Contribuição](CONTRIBUTING.md) · [Segurança](SECURITY.md) · [Índice](docs/README.md) · [Histórico](CHANGELOG.md).

Commits locais autorizados; push fica com o usuário. Preservar mudanças dele, não acessar instalações de jogos, contas ou conteúdo protegido. Sem licença pública escolhida; recursos de terceiros requerem direitos próprios. Logo atual continua provisória até licença/substituição. O controle de tela foi encerrado a pedido do usuário.
