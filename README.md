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
