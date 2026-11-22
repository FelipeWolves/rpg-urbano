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

# Próximas fases e frentes independentes

Plano vigente em19/11/2026. Uma entrega funcional por vez; esta lista não inicia automaticamente os blocos. Fontes e logo locais estão conectadas ao código; evidências de execução ficam em HUB-IDENTIDADE.md. Não reabrir contratos de combate encerrados sem defeito concreto.

## Sequência prioritária

| Ordem | Entrega | Dependência mínima | Critério de encerramento | Esforço relativo |
|---|---|---|---|---|
|1| Fechar aceite visual Windows do hub e da arena revisada | Build atual e execução autorizada | Hub/evento/batalha/volta, animação/estático, acentos,1/3/5 unidades, resoluções e escudo conferidos | Pequeno/médio; corrigir defeitos primeiro |
|2| Interface utilizável em telas pequenas | Resultado da fase1 | Texto integral/rolagem, foco, toque, áreas seguras, opção de acessibilidade persistida | Médio |
|3| Primeiro ensaio mobile sem arte final | Aparelho-alvo, SDK e autorização de instalação/configuração se necessário | Compilar/instalar, abrir arena e hub, medir memória/tempo de quadro, encerrar/retomar | Médio/alto; antecipar antes de produção artística extensa |
|4| Entrada e vídeo local | Catálogo atual e contrato de interrupção/pular/áudio | Player com fallback, proporção, preferências antes da janela; orientação/restauração testadas por plataforma | Médio |
|5| Fatia vertical autoral | Fases1–4 e pequeno conjunto de conteúdo original | Abertura→hub→encontro→resultado→save/reentrada em Windows e celular | Alto, dividir em entregas |
|6| Expansão de sistemas/conteúdo | Fatia vertical estável e contratos REG/SIS/PvE pertinentes | Incrementos testados de habilidades, equipamentos, modos e progressão | Alto, contínuo |
|7| Preparação de distribuição | Desempenho, direitos, localização e escopo fechados | Pacotes e requisitos específicos das lojas; serviços apenas se necessários | Alto; não bloqueia laboratório local |

As fases2/3 podem preparar contratos e diagnóstico em paralelo à fase1; isso não declara experiência mobile aprovada. Windows Store/Steam/backend não são pré-requisitos do protótipo local.

## Entregas que podem ser preparadas agora

| Prioridade | Frente independente | Entrega útil para implementação | Depende de quê depois? |
|---|---|---|---|
|Alta/rápida| Fichas de habilidades e efeitos | Markdown/JSON proposto com ID, alvo, gatilho, base de cálculo, relógio, duração, limites e exemplos | Revisão do contrato antes de virar dados executáveis; não copiar personagens oficiais |
|Alta/rápida| Matriz de interface e textos | Estados, mensagens de recusa, descrição completa, tradução pt-BR/en e ações de cada tela | UI/foco/rolagem; traduções reais ainda precisam localização |
|Alta/rápida| Identidade e acessibilidade | Tamanhos mínimos, contraste, foco, estados sem cor/movimento e teste por resolução | Validação visual/dispositivo, sem depender de combate novo |
|Alta| Definição do aparelho-alvo | Modelo, SO, memória, orientação preferida e disponibilidade para teste | Ferramentas e primeiro build mobile |
|Média| Universo e conteúdo original | Briefing, roteiro, elenco mínimo, encontro/tutorial e condição de vitória | IDs/implementação de conteúdo e balanceamento |
|Média| Hub final e arte2D | Imagem fonte, zonas seguras/recortes, camadas, ícones próprios e licença/autoria | Substituição no catálogo e aceite em resoluções reais |
|Média| Música e efeitos | Arquivos próprios, fontes editáveis, loop, volume e tabela de eventos | Integração de áudio, interrupção/retomada e testes |
|Média| Vídeo de abertura/evento | Arquivos paisagem/retrato existentes ou revisados, storyboard, legendas, duração e regra de pular | Player e validação de codec/plataforma |
|Média/alta| Cenário3D | Uma cena pequena original, unidade/escala, pivôs, materiais e fontes editáveis | Orçamento mobile e integração na arena; não produzir cidade completa antes do ensaio |
|Alta complexidade| Personagem/rig/animação | Um modelo de teste, esqueleto, idle/ação/impacto/morte com pontos de sincronismo | Contrato de animação e integração; sem esperar catálogo inteiro de habilidades |
|Separada| Progressão/economia | Proposta de recompensas/desbloqueios com limites e dados fictícios identificados | Revisão de save/migração e implementação própria |

Não há independência absoluta. Entregar fontes editáveis, arquivo final, versão, dimensões/escala, autoria/licença e critérios de aceite junto de cada recurso. Não se deve começar vários grandes blocos de código simultaneamente na mesma árvore: manter interfaces estáveis, uma entrega por commit e reservar integração/regressão.

## Organização e consumo

- Estado técnico: ESTADO.md; dúvidas por prioridade: PENDENCIAS.md; percentuais: PROGRESSO-GERAL.md; tarefa imediata: TAREFAS.md.
- Regras em src/include; fontes Unreal canônicas em experiments/unreal; dados em data; mídia local em assets/runtime e fontes editáveis em assets/sources.
- Preparação gera cópias em build; nunca editar essas cópias como origem. Manifestos e scripts permitem conferir/reproduzir sem commitar mídia grande.
- Revisar somente o escopo alterado; testes de combate completos apenas se houver mudança/dependência pertinente. Arte recebida não significa integração aprovada.
- O usuário acompanha o saldo de uso; não há acesso do assistente ao limite diário nem garantia de quantos blocos caberão nele.

Seletor corrigido e aceito pelo usuário. Próximo: planejar apresentação mobile e continuar a matriz visual restante, sem repetir o aceite do seletor. Texto sem corte confirmado pelo usuário; seleção e batalha funcionais no relato. Apresentação mobile e fluidez permanecem separadas, sem causa/regra inventada.

## Trabalho independente durante a pausa

Guias detalhados por grupo e pastas de recebimento em [entregas/README.md](../../entregas/README.md). Ali estão separados: informação que falta, recurso que pode ser produzido agora, trabalho que exige código e dependência futura. Receber material não inicia automaticamente um bloco. Ao retomar, conferir somente novidades, reconciliar com pendências e manter a ordem crítica deste plano.

Seletor: correção por linhas3D com profundidade junto à base do modelo implementada e posteriormente aceita visualmente pelo usuário; o assistente não abriu a prévia nessa entrega. [Escopo e roteiro](VIS-SELETOR-CHAO.md). Não altera regras nem resolve mobile/fluidez; não repetir esse aceite sem mudança ou defeito novo.

Mobile: [proposta revisada e sequência mínima](MOBILE-PLANO-REVISAO.md). HUD atual é Canvas, não UMG; preview/renderizador não troca automaticamente DeviceProfile Windows. Próxima implementação a planejar: apresentação Desktop/Mobile/Auto separada de plataforma, antes de presumir migração UMG ou instalar SDK. Sem implementação nova por esta revisão.

## Desenvolvimento que não espera novos materiais

[Plano executável por entregas AUT-01–16](FILA-DESENVOLVIMENTO-INDEPENDENTE.md): ordem, dependências internas, aceite e limites. Complementa esta sequência geral; não agenda nem executa trabalho. Primeiro candidato AUT-01, mantendo ESC/seletor já encerrados.

## Retomada após as12 entregas estudadas —20/11

[Consolidação e divergências](REVISAO-ENTREGAS-2026-11-20.md). PrimeiroAUT-01→02→03→04 e diagnóstico da instalaçãoAndroid existente, quando o usuário informar limite. PrototipagemWindows segue sem exigirbackend; AndroidAVD é etapa dedicada antes doS23 Ultra. Demo pequena antecede67 capítulos,clássicoexpandido,DJ,PvP e5 lojas. Nenhum desses sistemas foi implementado pela revisão. [DependênciasAndroid](ANDROID-DEPENDENCIAS.md).

## Respostas ENT recebidas —20/11/2026

**ENT-01–15 respondidos pelo usuário; implementação ainda pendente.** [Decisões vigentes e detalhes por bloco](RESPOSTAS-ENT-2026-11-20.md). PreservarESC/R04/Defesa atual até mudança explícita por implementação. Demo:1 Felipe jogável/1 arena;A2passiva. Pausa/abandono,panmobile,vídeo e futuraeconomia agora têm direção consolidada. Não repetir perguntas principais da revisão anterior;formalizar apenas detalhes técnicos no bloco afetado. Nenhuma execução autorizada por esta atualização.

Adendo técnico recebido20/11: [matriz vigente](ADENDO-TECNICO-CONTRATOS-2026-11-20.md). Debuff exigehitnãoMiss/nãoBlock;supressão/prevenção e reaçõespendentes detalhadas;SilverShieldcoexiste econsomeapósStandard;ondaslimpamcomunsantesdecurar;scorearredondado uma vez. Temas respondidos não são perguntas abertas de produto;bordas técnicas estão delimitadas no adendo. Implementação ainda pendente.
