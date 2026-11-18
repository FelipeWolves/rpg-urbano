# Decisões

## DEC-001 — Projeto independente

Estado: confirmado. Novo jogo autoral, em diretório separado do laboratório. Nenhuma leitura ou reutilização de instalações, contas, launchers ou recursos protegidos. Inspiração de gênero não define personagens, regras ou identidade final.

## DEC-002 — Entregas pequenas

Estado: confirmado. Uma entrega verificável por bloco. Histórico e estado concisos no repositório. Processamento repetitivo automatizado localmente quando útil; nenhuma obrigação de produzir longos relatórios a cada ajuste.

## DEC-003 — Consumo de Codex

Estado: preferência confirmada. Usuário acompanha o painel; reserva desejada de 30%. Não há acesso automático ao saldo nem previsão garantida por tarefa. Calibrar o tamanho dos blocos a partir de consumo observado. Não iniciar paralelismo por padrão.

## DEC-004 — Engine e atualização

Estado: em aberto. Unreal moderna é candidata, não instalada ou escolhida neste bloco. Começar em versão estável compatível com os recursos disponíveis. Atualizações testadas em ramificação/cópia; sem garantia de migração automática para futuras gerações. Código-fonte e recursos editáveis devem ser preservados.

## DEC-005 — Primeira batalha

Estado: hipótese de trabalho reversível. Arena 3x3, seis instâncias provisórias, ataque e guarda, adversário determinístico e vitória por eliminação. Estatísticas e regras detalhadas em docs/projeto/VISAO.md. Primeiro avaliar as regras; depois produzir personagens e arte final.

## DEC-006 — Plataforma e conectividade

Estado: hipótese de trabalho. Protótipo offline para PC, sem contas, servidor ou loja. Confirmar plataforma final antes de decisões gráficas caras. Mobile e serviços online não fazem parte do primeiro ciclo.

## DEC-007 — Linguagem do núcleo

Estado: definida em FUN-02 e viabilizada em AMB-01. C++20 independente da Unreal, build direto MSVC x64, biblioteca padrão e sem CMake/framework de testes externo no primeiro motor. A proposta VS2022 foi substituída pela escolha do usuário por Visual Studio 2026, instalado manualmente e validado pelo diagnóstico. Ponte e compatibilidade com Unreal serão avaliadas ao escolher a engine.

## DEC-008 — Limite do bloco atual

Estado: confirmado. Fundação documental e repositório local, sem desenvolvimento de gameplay, instalação de engine, chamadas de API pagas ou publicação. Próximo bloco proposto: FUN-02.

## DEC-009 — Encerramento de FUN-02

Estado: diagnóstico concluído em 18/11/2026. GPU identificada como RTX 3060; aproximadamente 31,8 GiB de RAM. .NET disponível é runtime, sem SDK. Engine/compilador utilizáveis não foram confirmados nas buscas seletivas. Não inferir ausência de instalações em caminhos personalizados, nem VRAM pelo campo AdapterRAM do WMI. Detalhes e fontes em docs/projeto/FERRAMENTAS.md.

Próxima entrega proposta: AMB-01, somente instalação aprovada das ferramentas mínimas e programa diagnóstico próprio. Sem aprovação, não instalar ou trocar linguagem como atalho. COM-01 permanece pendente.

## DEC-010 — Visual Studio 2026 validado

Estado: confirmado em 18/11/2026. O usuário escolheu e instalou manualmente VS Community 2026. vswhere identificou 18.12.1 (instalação 18.12.12412.261), estável, completa e sem reinicialização requerida. VsDevCmd selecionou MSVC 14.52.37025 e Windows SDK 10.0.26100.0.

Diagnóstico próprio C++20/x64 validou concepts, span, ranges, biblioteca padrão e chamada Windows SDK. Compilação /W4 /WX sem avisos, link e execução com saída zero. Comando em AGENTS.md; evidência em docs/relatorios/atuais/AMBIENTE.json. Não instalar VS2022 por causa da proposta superada. Nenhuma engine ou gameplay executado; compatibilidade futura da engine não certificada. Próximo bloco: COM-01.

## DEC-011 — Primeiro motor textual entregue

Estado: confirmado em 18/11/2026. Núcleo C++20 sem dependências de Unreal, Windows ou rede, com definições próprias, consultas constantes, comandos validados e eventos de domínio. As regras de docs/projeto/VISAO.md foram mantidas. Aceita qualquer formação com ambas as equipes e pelo menos um combatente vivo inicial por equipe; o cenário de referência é 3x3.

IDs ordenam canonicamente a entrada e desempates; velocidade é imutável nesta versão. Dano dos eventos é o dano aplicado, limitado à vida restante. Guarda arredonda para cima sem overflow de INT_MAX, consome em um acerto ou expira no início do próximo turno do defensor. Comando inválido não consome turno nem produz eventos.

13 testes de comportamento aprovados com /W4 /WX. Referência sempre-atacar: A vence em 35 ações/12 rodadas, A2 com 50 de vida; replay com entrada invertida tem estado/eventos idênticos. Demo automática usa uma guarda inicial para mostrar a regra e termina em 40 ações/14 rodadas. Não há input interativo ou reinício nesta entrega; COM-02 os adicionará. Detalhes e limites em docs/projeto/COMBATE.md.

## DEC-012 — Investigar formatos antes da interface

Estado: confirmado em 18/11/2026, por solicitação do usuário. COM-02 em espera. COM-01A analisou formações 1–5, confrontos consecutivos, limites, três times sucessivos com habilidades de chefe por etapa e confrontos paralelos. Usuário identificou Celestial Anomaly Season 0 como referência histórica; isso não revela o código interno ou causa de mudança do modo.

168 cenários e 168 replays aprovados; 13 testes anteriores mantidos. Três contextos independentes intercalados foram comparados a execuções individuais. Lote funcional local de 1000 simulações não é benchmark de servidor. Nenhuma mudança de regra do núcleo foi necessária para essa matriz; modos coordenados seguem pendentes.

Não fixar cinco como constante global, confundir AP com rodada fixa, nem clonar chefe em três HPs e chamá-lo de estado compartilhado. Contratos propostos em docs/batalhas/CONTRATOS.md; fontes classificadas em fontes.json. Causas de retirada e desempenho do Dislyte não foram verificadas em fonte técnica primária. Próxima entrega: TIME-01, depois WAVE-01, SEQ-01 e PAR-01/PAR-02 em blocos.

## DEC-013 — Limite local e registro central de pendências

Estado: confirmado em TIME-01, 18/11/2026. Contar ações principais aceitas de ambos os lados, incluindo guarda. Orçamento local opcional, inteiro positivo; política explícita de empate/derrota A/derrota B. Sem configuração, preservar comportamento sem limite. Eliminação na última ação prevalece; não avançar turno após esgotamento.

Adicionar EndReason e contador/regras ao estado/eventos: Finished não significa necessariamente vitória por eliminação. Global de fase, score e ações derivadas não foram implementados. 11 testes novos e regressão anterior aprovados; detalhes em docs/batalhas/TIME-01.md.

Por solicitação do usuário, docs/projeto/PENDENCIAS.md passa a ser o registro central de dúvidas e capacidades não validadas; cada item tem ID e momento de resolução. Referenciar sem repetir todas as ressalvas em cada entrega. Próximo bloco WAVE-01, consultar PEN-01/PEN-02; COM-02 segue em espera.

## DEC-014 — Fase consecutiva com estado e orçamento explícitos

WAVE-01 concluída em 18/11/2026: Stage para 1–5 confrontos, políticas obrigatórias PreserveHealth/RestoreTeam, guarda limpa e ordem reiniciada; vida inicial validada sem alterar atributos. Limites locais/globais contam ações aceitas; empate de limites usa política global. Vitória final por eliminação na última ação prevalece; vitória intermediária sem saldo não inicia nova onda. Eliminação de A não é anulada pela política global. Detalhes e fronteiras em docs/batalhas/WAVE-01.md.

9 testes novos aprovados, 24 anteriores e 168 cenários preservados. Identidade/versão e evento de conclusão únicos em memória; sem economia, save ou habilidades. Próximo bloco SEQ-01. Questões futuras permanecem em docs/projeto/PENDENCIAS.md.

## DEC-015 — Pesquisa e perfil de tempo antes de chefes por etapa

Usuário pediu pesquisa ampla e aproximação das mecânicas gerais do Dislyte, preservando guarda como opção extra e outros sistemas de tempo como possibilidade. TIME-02 passa à frente de SEQ-01. Evidência oficial histórica distingue AP, turno adicional e ações auxiliares; detalhes naturais/contadores têm lacunas ou apenas fontes secundárias. Não declarar paridade com o cliente.

Experimento PowerShell isolado: 11/11 verificações, sem alteração de Battle/Stage. Relatório docs/batalhas/TEMPO-PESQUISA.md; JSON/CSV próprios. Perfil AP será integrado e validado em C++ no próximo bloco. ATB permanece extensão planejada; efeitos e recargas reais não foram implementados por este experimento.

## DEC-016 — Perfis fixo/AP e fronteiras explícitas de turno

TIME-02 integrada em C++ em 18/11/2026. Perfil ActionBar prioritário para experimentos, FixedRounds mantido para regressão. Turno natural/extra e reação não são intercambiáveis. Guarda é opção configurável. Habilidades originais mínimas declarativas fornecem dano básico, AP/SPD, extras, bloqueio, marcador e contra-ataque; sem catálogo proprietário.

Relógios do dono são NaturalOnly ou EveryTurn; nova recarga/status não é reduzida no mesmo uso/aplicação. Skip bloqueado conta como comando principal; reação não. Erro de cadeia tem TechnicalLimit sem vencedor. Fases AP/habilidades exigem política de runtime: Preserve ou Reset, com AP/guarda/filas locais reiniciados. Contrato completo em docs/batalhas/TIME-02.md.

49 testes aprovados, 168 cenários anteriores e 25 formações AP/replays. Não há equivalência comprovada ao Dislyte nem ATB implementado. Próximo bloco SEQ-01; demais lacunas em docs/projeto/PENDENCIAS.md.

## DEC-017 — Opção B: reflexão e explosões entre acertos

Decisão explícita do usuário em 18/11/2026: reflexão de cada hit antecede drenagem FIFO de pendingDeaths; explosões em cadeia resolvem antes do hit seguinte. Auto-revive após loop; Victory Lock depois das consequências obrigatórias. Em 0 × 0, vence o lado de CurrentActionActor da ação em resolução, inclusive contra-ataque, sem trocar esse papel pelo emissor de reflexão/explosão ou pelo lado ofensivo fixo do modo. Referência completa e cinco invariantes em docs/batalhas/CONTRATO-CAUSALIDADE-R04.md. Contrato autoral; não é paridade externa comprovada nem implementação já testada.

## DEC-018 — Primeiro ajuste TIME-03: ciclo de recarga

Implementado e validado: uso elegível já reduz sua nova recarga no encerramento, até zero. Exemplo N=3 reutiliza no turno 4. Status mantêm regra anterior; extras ainda seguem relógios por habilidade até a parte 2. 53 testes e matrizes aprovados; relatório incremental docs/batalhas/TIME-03.md. Bloco TIME-03 ainda aberto.


## DEC-019 — Fonte explícita e contexto do turno extra

TIME-03 parte 2: ExtraTurnSource declara categoria/flags/teto; ExtraTurnContext preserva proveniência e oportunidade natural originadora na fila/eventos/snapshots. Flags explícitas governam recarga/duração nessa oportunidade; ausência preserva relógio legado. Limite por (concedente, beneficiário, fonte) na cadeia natural suprime concessão sem erro técnico. 60 testes aprovados; contrato em docs/batalhas/TIME-03.md. Próxima parte: controle/congelamento.


## DEC-020 — Encerramento TIME-03: bloqueio e congelamento separados

Block impede escolha de ação; FreezeCooldowns impede decremento final. Combinação configura controle com congelamento. Avaliar congelamento antes de expirar status: o último turno congelado não reduz recarga. Flag de extra não contorna congelamento; duração permanece independente. 67 testes aprovados, 168 cenários/replays e 25 formações/replays AP preservados. Contrato em docs/batalhas/TIME-03.md. TIME-03 encerrado; próxima entrega RES-01/identidade e multi-hit/guarda por habilidade.


## DEC-021 — Instância de ação e guarda em múltiplos acertos

RES-01 parte 1 implementada: ActionContext captura autor/lado/ID/raiz/pai; cada contra-ataque executado tem ID próprio, com pai imediato. Acertos de uma habilidade compartilham contexto e numeração. Guarda protege todos os acertos dessa instância e consome uma vez ao fechar ação, antes de reações. Teto 64 por bloco Damage/256 por habilidade, sem multiplicar dano int. 76 testes aprovados; R04 completo permanece para partes 2/3. Relatório docs/batalhas/RES-01.md.


## DEC-022 — Reflexão e FIFO entre acertos no perfil mínimo

RES-01 parte 2 implementa Reflect e DeathExplosion com magnitude fixa positiva, reflexão antes da drenagem e explosões FIFO sem recursão profunda. Consequências não disparam novas reflexões/Counter nem usam guarda; mortes podem gerar novas explosões. Limite técnico por trabalho, distinto de orçamento de turnos. LowestHealthEnemy é seleção dinâmica determinística de teste. Morte dupla usa autor da ação em resolução, sem janela de revive ainda. Fase PreserveHealth sem sobreviventes encerra NoSurvivors, enquanto RestoreTeam permite transição explícita. 84 testes aprovados; próxima parte prevenção/revive/vitória. Detalhes em docs/batalhas/RES-01.md.


## DEC-023 — Encerramento RES-01 e janela de sobrevivência

PreventDeath mantém 1 HP em dano letal e consome carga sem confirmar morte. AutoRevive consome carga na morte, é processado após golpes/FIFO e antes do resultado, limitado ao HP máximo; AP/guarda reiniciam sem retomar trabalho cancelado. Prevenção/revive gastam orçamento técnico. Cinco invariantes R04 validados no conjunto mínimo; sem catálogo ou revive aliado de área. 95 testes aprovados e matrizes preservadas; próximo SEQ-01. Contrato incremental docs/batalhas/RES-01.md.


## DEC-024 — Coordenador de três times sucessivos

SequentialSquads separa personagem, instância e time; valida inscrição inteira (três times de 1–5, sem repetição). Mantém HP e max_health do chefe, troca por eliminação/limite local, aplica orçamento global único e respeita R04 antes da troca. Política base explícita conserva cooldowns ou reinicia runtime, limpando todos os status; categorias refinadas e kits por etapa entram na parte 2. 103 testes aprovados e matrizes preservadas; SEQ-01 ainda aberto. Relatório docs/batalhas/SEQ-01.md.


## DEC-025 — Perfis e estados classificados

SEQ-01 parte 2 vincula três perfis aos três times, mantendo HP/identidade. Perfil escolhe cooldown por ID, reset ou mapa um-para-um. Temporary expira/limpa; Permanent/Structural usam remaining 0 e persistem. Modo classificado conserva cargas e capacidades sem reaplicar estado inicial. Shield e StructuralShield coexistem e absorvem após guarda, antes de HP/prevenção. 112 testes aprovados; próxima validação integrada. Relatório docs/batalhas/SEQ-01.md.

Git agora tem commit inicial do usuário e origin; ausência de commit é informação histórica superada. Pushes ficam com o usuário.


## DEC-026 — Encerramento integrado de SEQ-01

Kits podem repetir entre etapas sem impedir mudança de perfil. Integração valida recargas, revive persistente, limite final, conclusão única e replay/reset. 115 testes aprovados; nenhuma nova família de mecânicas nesta parte. Próximo PAR-01; commit local autorizado e pushes com usuário.


## DEC-027 — Coordenação independente inicial

PAR-01 usa três Battle isolados e ordem cíclica configurável, uma ação principal aceita por frente. Conclusão apenas após todas terminarem; vitória A exige três vitórias, qualquer vitória B derrota A, demais casos empate, falha técnica prevalece sem vencedor. IDs locais se repetem apenas em escopos distintos. Sem threads/HP compartilhado/orçamento global nesta parte; orçamento e cancelamento entram na parte 2. 120 testes aprovados.


## DEC-028 — Orçamento e cancelamento entre frentes

Contador único de comandos principais aceitos; cadeias derivadas resolvem antes da fronteira. Falha técnica prevalece, conclusão de todas por eliminação precede timeout; nos demais casos teto global precede limites locais. Cancelar congela o coordenador sem fabricar resultado local: FrontState é separado de Battle.phase. Cancelamento explícito não tem vencedor e é idempotente. 125 testes aprovados; integração técnica/R04 dedicada na parte 3.


## DEC-029 — Encerramento integrado de PAR-01

Perfil cooperativo independente aprovado com 16 grupos e 131 testes totais. R04 resolve antes da fronteira global; cadeias não atravessam frentes, Skip conta uma vez, reações não gastam orçamento primário, extras cancelados não executam, falha técnica precede timeout. Nenhuma mudança de mecânica nesta parte. PAR-02 exige definir modelo de chefe compartilhado; não inferir HP único nem score da implementação isolada.


## DEC-030 — Comparação por nível de evidência

A pedido do usuário, consulta excepcional somente leitura aos relatórios irmãos e à cópia de testes, sem reutilizar código/ativos ou mudar escopo de execução. Separar relatório histórico, conferência local pontual, pesquisa/relato e contrato autoral. Metadados não validam payload ou regras; 131 testes validam nosso contrato, não superioridade externa. Matriz de 24 áreas em docs/batalhas/comparacao-dislyte-2026-11-18/. PEN-05 não resolvida por arquivos acessíveis.


## DEC-031 — Separação por áreas e plugins conforme necessidade

Usuário aprovou manter combate, dados, aplicação, apresentação, áudio, persistência/diagnóstico e ferramentas separados. Plugin é opção de implementação após pesquisa e pequeno teste de integração, não exigência para cada área. Priorizar adequação, manutenção, licença, compatibilidade e substituição; engine ainda não escolhida, sem downloads/instalações/compras. Diretriz em docs/projeto/ARQUITETURA.md. Sem mudança de código/testes; PAR-02 continua próximo.


## DEC-032 — HP único e barra global

Usuário escolheu HP único compartilhado pelos três times e uma barra global do chefe, ataques alternando entre frentes vivas. Parte 1 usa um único Battle com identidade de time na camada SharedBoss, sem clones de chefe ou mudanças no resolvedor. Composição 3x1–5, AP/SPD global e rotação somente após ataque comum aceito. Habilidades fora do primeiro perfil; integração posterior exige seletores/estado únicos e testes. 136 testes aprovados. PEN-05 resolvida para este modelo.


## DEC-033 — Classificação de famílias PvE por eixos

Separar formação, participação dos times, autoridade de HP, fases, relógios, persistência, resultado, pontuação e recompensas. HP único/barra global/rotação permanecem perfil autoral do PAR-02; não regra universal. Treva Fluxo e exemplo dos Milagres registrados como relatos, sem inventar nome exato ou fórmula. Catálogo de 12 entradas conserva pistas históricas e não afirma exaustividade atual. Atualizados 8 cenários/26 mecânicas com originais preservados. Mudança documental, 136 testes prévios não reexecutados.


## DEC-034 — Habilidades compartilhadas com alvo explícito

SharedBoss exige mapeamento 1:1 skill/escopo compatível: Self, OwnSquad, Boss, RotatingFront. Alvo direto único; efeitos de morte/reflect/reação conservam semântica do resolvedor. Boss dynamic retargeting rejeitado até seletor limitado à frente. Self/guarda/Skip não giram rotação; skill RotatingFront aceita gira uma vez. Único Battle conserva relógios/cargas sem triplicar por time. 142 testes aprovados; parte 3 segue pendente.


## DEC-035 — Encerramento do perfil compartilhado e fronteira técnica

PAR-02 encerra apenas HP/AP único e skills de alvo direto único com escopos/R04, orçamento e replay: 17 grupos, 148 testes totais. Falha ocorrida dentro da cadeia precede timeout; limite de agendamento futuro não substitui timeout que já impede nova oportunidade. Sem alteração de produção nesta parte. Aceite histórico de skill-phase não foi implementado: transições por HP/turno ficam explicitamente nas famílias futuras/PVE-09, sem declaração de aprovação. Área, dinâmica do chefe e score também pendentes. Próximo COM-02.


## DEC-036 — Apresentação textual separada

COM-02 usa streams e Battle para input/estado/eventos, sem regras duplicadas. Usuário controla A, B usa política básica existente. Default 3x3 AP, orçamento de 200/empate; comando exige alvo explícito. Reinício reconstrói dados/regras originais, EOF/sair não simulam vitória. Proteção de 256 ações automáticas encerra apresentação sem mudar resultado. Sete grupos de interação e 155 testes totais aprovados; sem engine/menu de modos ou novas mecânicas. Próximo HAB-01.


## DEC-037 — Roteiro de perfis e conjuntos de habilidades

Observação do usuário orienta padrão de equipe com até três posições e clássico preservado. Organização documental separa menu, AP/SPD e controle. Habilidades podem ser ações de suporte ou conter componente passivo; passivas não viram botões. HAB-01 dividido em perfis/posições, primeira passiva, condições/kits, automação e integração. Detalhes são propostas de contrato, não regras já implementadas ou equivalência oficial ao Dislyte. Pesquisa pública limitada registrada em HAB-01.md; 155 testes não reexecutados.


## DEC-038 — Contrato padrão de equipe e defesa

Usuário definiu até três posições, básica A1 sem silêncio nem indisponibilidade voluntária quando apto, passivas informativas e indicadores separados de CD/inatividade/condição. Sem defesa genérica nesse perfil. Matriz de supressão e congelar/petrificar ainda incerta. Clássico preservado, quatro categorias propostas para contrato posterior. Termo visível defesa aplicado agora, mesmo Guard interno; guardar alias oculto. Perfil novo ainda não implementado. Sete testes textuais e demo revalidados, sem bateria completa.


## DEC-039 — Defesa com arredondamento inferior

Usuário escolheu floor(dano/2) por hit como padrão da defesa genérica; 1 pode virar0. Não muda consumo/expiração nem precedência de escudos. Possibilidades futuras de amplificar/reduzir dano no clássico ficam anotadas, sem implementação ou imposição ao padrão de equipes. 155 testes aprovados e protótipo confirmado25->12; arquivos históricos preservados.

## DEC-040 — Perfis e posições HAB-01 parte 1

Perfil de equipe usa até três posições, básica canônica sem recarga e sem defesa genérica; clássico permanece em lançador próprio. Passivas declaradas não executam efeitos neste incremento. Kernel de batalha reutilizado por composição, com validação de loadouts e comandos no ProfileBattle. Aceite 162/162; próxima parte exige contrato do primeiro gatilho passivo.

## DEC-041 — Estrutura e colaboração

Preservar layout C++ existente, reunir planejamento em docs/projeto e JSON vigente em docs/relatorios/atuais. Histórico de contratos permanece em docs/batalhas. README e guias são entradas para colaboradores; AGENTS concentra regras atuais, com histórico separado. Sem engine, dependências, licença aberta ou CI externa implícitos.

## DEC-042 — Primeira passiva de abertura

Primeiro gatilho autoral é BattleStart, escudo próprio temporário, uma vez por combate. Usa preparação de runtime sem consumir ação e sem reordenar R04. Registro no snapshot identifica dono/posição/definição. Não confundir mecanismo de abertura com event bus durante combate nem reaplicar automaticamente em etapas SEQ/PAR. Contrato HAB-01-PASSIVA-ABERTURA-R01.md.

## DEC-043 — Condição de vida sem custo

Primeiro requisito adicional é vida do dono <= percentual inclusivo exato (64 bits), somente ativa não básica. Não consome HP/recurso. ProfileBattle centraliza checagem por posição/ID, com incapacidade e CD prioritários, requisito visível independentemente. Nova família de custo/condição exige contrato próprio; IA estratégica permanece parte 4.
