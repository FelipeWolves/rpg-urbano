# Tarefas

## HAB-01 — concluído no escopo mínimo (18/11/2026)

✅ Cinco partes entregues: perfis/posições, passiva de abertura, condição adicional, automação e integração de kits ativo/passivo/híbrido. **187/187 testes aprovados**; lançadores e clássico conferidos. Aceite consolidado: docs/batalhas/HAB-01-FINAL.md e COMBATE-HAB-01-FINAL.json. Não há revisão pendente desse escopo sem mudança/falha pertinente.

⚠️ Silêncio, passivas durante combate, custos/condições compostas e adaptação SEQ/PAR continuam extensões futuras. Próximo: CIC-01, primeira parte, contrato e composição de equipe em memória; recompensa e save em incrementos posteriores.

## HAB-01 parte 4 — histórico da entrega (18/11/2026)

✅ Automação configurável por prioridade de especiais com básica como fallback, alvos determinísticos e execução pela mesma validação manual. Time B de equipes integrado; clássico preservado. 183/183 testes aprovados, incluindo 16 combinações automáticas curtas com replay/orçamento. Contrato docs/batalhas/HAB-01-AUTOMACAO-R01.md; relatório HAB-01.md; aceite COMBATE-HAB-01-PARTE-4.json.

⚠️ Sem IA estratégica, toggle do time A ou editor de prioridade. HAB-01 continua aberto. Próximo: parte 5, integração/regressão do pequeno conjunto autoral e consolidação dos limites; sem novas famílias ou engine nesta etapa.

## HAB-01 parte 3 — histórico da entrega (18/11/2026)

✅ Condição adicional de vida em ativa não básica, validada por posição e ID. Proteção de emergência A3/B3 exige <= 50% de vida; básica preservada. Motivos de condição, recarga e incapacidade separados. 176/176 testes aprovados; contratos/relatório em docs/batalhas/HAB-01-CONDICAO-VIDA-R01.md e HAB-01.md; aceite COMBATE-HAB-01-PARTE-3.json.

⚠️ HAB-01 continua aberto; sem energia, silêncio ou condições compostas. Próximo: parte 4, política mínima de automação com prioridades, especiais válidas e fallback sob a mesma validação manual.

## HAB-01 parte 2 — histórico da entrega (18/11/2026)

✅ Primeira passiva funcional: Preparação aplica escudo na abertura, uma vez por combate, sem consumir ação; A2/B2 da demo já a utilizam. Posição passiva continua não selecionável, e o efeito/registro são verificáveis. Contrato: docs/batalhas/HAB-01-PASSIVA-ABERTURA-R01.md; relatório: docs/batalhas/HAB-01.md.

⚠️ Gatilhos durante combate, supressão e recarga interna não implementados. HAB-01 continua aberto. Próximo: parte 3, fechar a primeira condição adicional de habilidade, com motivo de bloqueio separado de recarga e básica preservada.

## Organização do repositório — 18/11/2026

✅ Organização documental e colaboração concluídas: documentos de planejamento em docs/projeto; resultados vigentes em docs/relatorios/atuais; estrutura C++ preservada. README/CONTRIBUTING/SECURITY, EditorConfig, atributos Git e modelos de issue/PR adicionados. Histórico de orientações preservado; AGENTS atualizado para o estado vigente.

Scripts ajustados aos novos caminhos. Não foram instaladas dependências, adotada engine, definida licença pública ou configurada execução automática em serviços externos. Fontes/recursos de terceiros continuam sujeitos a autorização própria. Próximo bloco funcional permanece HAB-01 parte 2.

Uma entrega por vez. Ordem por área, criticidade e dependências, conforme docs/projeto/PLANO-PRIORIDADES.md. Esforço relativo não é estimativa de prazo ou consumo.

## Entregas encerradas — histórico preservado

- [x] FUN-01 — Criar visão, arquitetura, regras de trabalho, decisões e estado; definir batalha inicial. Aceite: documentos consistentes, escopo independente e próxima tarefa executável.
- [x] FUN-02 — Verificar ferramentas existentes e definir como compilar o núcleo. Resultado: RTX 3060 identificada; compilador/SDK e editor Unreal utilizáveis não encontrados nas buscas seletivas. C++20/MSVC definido como caminho mínimo; proposta em docs/projeto/FERRAMENTAS.md. Nenhuma instalação ou compilação realizada.
- [x] AMB-01 — Usuário escolheu e instalou manualmente Visual Studio 2026. Validado VS Community 2026 18.12.1, MSVC 14.52.37025 e Windows SDK 10.0.26100.0: diagnóstico próprio C++20 compilado, vinculado e executado em x64, com /W4 /WX, sem avisos e com saída zero. Comando repetível em AGENTS.md e evidência em docs/relatorios/atuais/AMBIENTE.json. Nenhuma instalação pelo agente; sem gameplay ou engine.
- [x] COM-01 — Núcleo C++20 independente implementado: simulação 3x3, ataque/guarda, rodadas, derrota e vitória; comandos inválidos sem mutação. 13/13 testes aprovados; referência determinística de 35 ações/12 rodadas e demo com guarda de 40 ações/14 rodadas. Build/teste via scripts/testar-combate.ps1; resultado em docs/relatorios/atuais/COMBATE.json, limites em docs/projeto/COMBATE.md. Sem UI interativa, engine, artes ou progressão.
- [x] COM-01A — Antes da interface, analisar formações/fases/modos e ampliar testes. 168 cenários + 168 replays aprovados, matriz completa de tamanhos 1–5, sondagens de seis, três contextos 5x1 intercalados e 1000 simulações funcionais locais. Pesquisa pública com procedência e limites; contratos de fases/chefes em docs/batalhas/. Nenhuma implementação desses coordenadores ou mudança do núcleo de produção.
- [x] TIME-01 — Orçamento local opcional de ações principais de ambos os lados implementado; empate ou derrota de lado escolhido, motivo de término e contador no snapshot/eventos. Vitória na última ação prevalece. 11/11 testes novos + 13 anteriores e matriz aprovados. Contrato global documentado, não implementado. Detalhes em docs/batalhas/TIME-01.md; dúvidas futuras centralizadas em docs/projeto/PENDENCIAS.md.
- [x] WAVE-01 — Orquestrar um time em 1/2/5 confrontos com passagem de estado explicitamente configurada. Aceite: vida máxima não adulterada, sem cura/reanimação implícitas, contadores locais/globais corretos, derrota intermediária, vitória final única e reset limpo.
- [x] TIME-PESQ — Pesquisa oficial/comunitária de AP, extras, recargas e contadores; experimento isolado com 11/11 verificações. Evidências e limitações em docs/batalhas/TEMPO-PESQUISA.md.
- [x] TIME-02 — Perfis fixo/AP, guarda opcional, habilidades/status/recargas mínimos, extras/contra-ataques e preservação explícita em fases. 16 novos grupos aprovados + 33 anteriores; 25 formações/replays AP e 168 cenários anteriores. Detalhes em docs/batalhas/TIME-02.md; ATB/contador geral de modos continuam futuros.

## 1. Revisão da base implementada — prioridade imediata

- [x] TIME-03 — Incorporar R02: recarga no encerramento do uso, extras com flags independentes por origem e controle separado de congelamento do decremento. Testar fronteiras e integração com fases; um bloco restrito.
  - [x] Parte 1 — Recarga no encerramento do uso, casos 0/1/3 e replay. 53 testes totais aprovados; relatório docs/batalhas/TIME-03.md.
  - [x] Parte 2 — Origem/contexto e flags independentes dos extras; teto por fonte, FIFO e replay. 60 testes totais aprovados.
  - [x] Parte 3 — Bloqueio e congelamento independentes, precedência sobre extras, expiração e 1/2/5 confrontos. TIME-03 encerrado com 67 testes totais aprovados.
- [x] RES-01 — Implementar causalidade mínima R04: multi-hit, reflexão, explosões FIFO, prevenção/auto-revive, guarda por habilidade e desempate pelo autor. Cinco invariantes, morte dupla e limite técnico; sem catálogo de habilidades.
  - [x] Parte 1 — Identidade de ação, multi-hit e guarda por habilidade; 76 testes totais aprovados. Relatório docs/batalhas/RES-01.md.
  - [x] Parte 2 — Reflexão e mortes/consequências FIFO entre acertos, seleção dinâmica determinística e fronteira de fase sem sobreviventes; 84 testes totais aprovados.
  - [x] Parte 3 — Prevenção/auto-revive com cargas, Victory Lock e cinco invariantes R04 no perfil mínimo; RES-01 encerrado com 95 testes totais aprovados.

## 2. Modos e coordenação — criticidade alta

- [x] SEQ-01 — Modelar três times em sequência e perfis de habilidades do chefe por etapa. Aceite: transição atômica, política de HP/recargas/efeitos explícita, conclusão única (sem sistema de score) e regra de troca definida. Nenhuma fórmula de habilidades do jogo de referência será copiada.
  - [x] Parte 1 — Composição/identidade e coordenador dos três times com kit estável do chefe; 103 testes totais aprovados. Relatório docs/batalhas/SEQ-01.md.
  - [x] Parte 2 — Perfis, conservação/reset/mapa de recarga, categorias e escudos funcionais; 112 testes totais aprovados.
  - [x] Parte 3 — Integração encerrada: A/A/B e A/A/A, cargas permanentes, limite final e replay; 115 testes totais aprovados.
- [x] PAR-01 — Coordenador de três frentes independentes; ampliar a prova de isolamento para condições de vitória/derrota da fase, budgets e cancelamento. Não requer threads por padrão. Aceite: sem starvation ou vazamento de estado; duas ordens de escalonamento; replay por frente e resultado global.
  - [x] Parte 1 — Coordenador, alternância determinística, isolamento, resultado agregado e replay/reset; 120 testes totais aprovados. Relatório docs/batalhas/PAR-01.md.
  - [x] Parte 2 — Orçamento global, precedência local/global e cancelamento coordenado; 125 testes totais aprovados.
  - [x] Parte 3 — Integração R04/habilidades, Skip, reações, extras, falha técnica e fronteiras finais; 131 testes totais aprovados.
- [x] PAR-02 — Perfil HP único/barra global escolhido: composição, skills de alvo único com escopo, estado/R04, overkill, orçamento e conclusão única testados. Partes/avatares e troca de kit por HP/turno ficam separados nas famílias futuras abaixo; não alegar esse aceite histórico concluído para skill-phase.
  - [x] Preparar proposta de modelo/cadência, impactos e matriz de aceite em docs/batalhas/PAR-02.md (documental).
  - [x] Receber escolhas: HP único + barra global; PEN-05 resolvida no perfil inicial, contrato da parte 1 em PAR-02.md.
  - [x] Parte 1 — Composição, único estado/AP, rotação entre times vivos e ataques básicos; 136 testes totais aprovados.
  - [x] Parte 2 — Skills de alvo único com escopos, estados/recargas e R04 compartilhados; 142 testes totais aprovados.
  - [x] Parte 3 — Integração final, limites técnicos, overkill e conclusão/replay; 148 testes totais aprovados.

## 3. Uso, habilidades ampliadas e persistência

- [x] COM-02 — Interação textual de ações/alvos, resposta básica do time B, estado/resultado e reinício. 155 testes totais, 7 novos; app/jogar.cpp e scripts/jogar.ps1. Perfil Battle 3x3 AP, sem menu dos coordenadores ou UI gráfica; docs/batalhas/COM-02.md.
- [x] HAB-01 — Perfis padrão de equipes/clássico, posições de habilidade, passivas e condições, preservando AP/SPD e regras existentes. Recurso somente se necessário. Plano em docs/batalhas/HAB-01.md.
  - [x] Preparação — Plano, dúvidas e contrato de equipes R01 definidos; terminologia defesa entregue, sem implementação do novo perfil.
  - [x] Parte 1 — Perfis/posições, básica vinculada ao conjunto e distinção ativa/passiva/híbrida, sem comportamento fictício.
  - [x] Parte 2 — Primeira passiva autoral com gatilho, limite e janela de resolução.
  - [x] Parte 3 — Condição adicional e primeiros kits próprios, uma família por entrega.
  - [x] Parte 4 — Manual/automático com validação única e prioridade explícita.
  - [x] Parte 5 — Integrar poucos kits ao protótipo, preservar clássico e validar reset/replay.
- [ ] CIC-01 — Seleção de equipe, recompensa mínima e salvamento local versionado.

## 4. Engine e apresentação funcional

- [ ] ENG-01 — Escolher engine estável e validar compatibilidade com o núcleo e hardware. Teste pequeno de integração; instalação/download e escolha definitiva exigem decisão específica.
- [ ] VIS-01 — Integrar núcleo à primeira arena visual com placeholders. Aceite: selecionar ação/alvo, ver turno/vida/resultado e reiniciar; nenhuma regra duplicada na UI. Sem qualidade artística final.

## 5. Identidade e conteúdo

- [ ] ART-01 — Proposta de direção artística original e um personagem representativo; manter direitos e arquivos-fonte.
- [ ] AUD-01 — Áudio provisório licenciado/próprio, feedback e definição da identidade musical.
- [ ] EXP-01 — Somente após avaliação: campanha pequena e minijogo rítmico original.

## Próximo bloco executável

CIC-01 parte 1: fechar contrato e validar composição/seleção de equipe em memória, antes de recompensa/save. HAB-01 concluído; aceite em docs/batalhas/HAB-01-FINAL.md.

## Encerramento de cada entrega

Atualizar docs/projeto/ESTADO.md com implementação, testes efetivos, limites e próxima ação. Não repetir bateria/pesquisa sem mudança pertinente. Não interpretar esta fila como execução de todos os blocos em uma única tarefa.


## Análise complementar concluída

- [x] Comparação documental Dislyte/RPG Urbano: 24 áreas, fontes históricas identificadas e conferência pontual de 8 arquivos somente em leitura; Markdown/CSV/JSON em docs/batalhas/comparacao-dislyte-2026-11-18/. Sem mudança de regras nem novos testes; próximo bloco PAR-02 permanece.


- [x] Registrar organização por áreas e critérios para plugins, sem reorganização física antecipada; docs/projeto/ARQUITETURA.md e DEC-031. Pesquisa específica de dependências pertence ao bloco que precisar delas.


- [x] Organizar famílias/exemplos PvE em MODOS-PVE.md e CSV/JSON; preservar 8 cenários/26 mecânicas e versões anteriores; roteiro para pesquisa complementar. Sem implementação de score, retorno cíclico ou fases autônomas nesta tarefa. Próximo PAR-02 parte 2 permanece.

## Famílias PvE futuras — acrescidas durante o projeto, sem implementação

- [ ] Fases autônomas do chefe por HP/turno/outro gatilho (PVE-09): fechar contrato, transição e eventos de kit globais únicos. Lacuna explícita do aceite histórico de PAR-02; não coberta pelo perfil encerrado.
- [ ] Sobrevivência/pontuação/recompensas (PVE-06, Treva Fluxo relatado): identificar modo, unidade do contador, dano/overkill e fórmula de resultado antes de implementar.
- [ ] Detalhar chefe dos Milagres (PVE-07); retorno cíclico (PVE-08) só vira requisito confirmado com exemplo. Não presumir que os três cenários usem o mesmo coordenador.
- [ ] Habilidades comuns de área, seletores dinâmicos do chefe e efeitos entre times: especificar alcance e ampliar os seletores em bloco próprio.

Essas famílias não são funcionalidades prontas por SEQ-01/PAR-01/PAR-02 estarem encerrados em seus perfis. Detalhes e procedência em MODOS-PVE.md e docs/projeto/PENDENCIAS.md. A fila executável passa a HAB-01; novas prioridades dependem de escopo explícito.


- [x] Corrigir acesso COM-02 no Windows PowerShell comum: Jogar.cmd chama binário direto sem política de scripts; validado CMD/Windows PowerShell, sem mudar segurança nem motor.


- [x] Fechar contrato padrão de equipes HAB-01-PERFIL-EQUIPE-R01.md e trocar terminologia do protótipo para defesa; sete testes textuais revalidados. Novo perfil/posições e controles específicos ainda pendentes na parte 1/famílias correspondentes.


- [x] Defesa genérica arredondada para baixo por padrão; regressão completa 155/155 e protótipo com 25->12. Extensões futuras clássicas anotadas em DEFESA-PADRAO.md, separadas do perfil padrão. HAB-01 parte 1 continua próxima.

## HAB-01 parte 1 — histórico da entrega (18/11/2026)

✅ Perfis equipe/clássico, posições 1–3, básica canônica sem recarga, defesa restrita ao clássico e metadados passivos não selecionáveis implementados. 162/162 testes; dois lançadores conferidos. Este estado substitui indicações históricas de perfil ainda não implementado.

⚠️ HAB-01 continua aberto: passivas funcionais, silêncio e condições especiais não implementados. Próximo: parte 2, contrato do primeiro gatilho passivo autoral e implementação delimitada. Relatório completo e limites: docs/batalhas/HAB-01.md; evidência COMBATE-HAB-01-PARTE-1.json. Jogar.cmd abre equipes; Jogar-Classico.cmd preserva o clássico.
