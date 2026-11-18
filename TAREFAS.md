# Tarefas

Uma entrega por vez. Ordem por área, criticidade e dependências, conforme PLANO-PRIORIDADES.md. Esforço relativo não é estimativa de prazo ou consumo.

## Entregas encerradas — histórico preservado

- [x] FUN-01 — Criar visão, arquitetura, regras de trabalho, decisões e estado; definir batalha inicial. Aceite: documentos consistentes, escopo independente e próxima tarefa executável.
- [x] FUN-02 — Verificar ferramentas existentes e definir como compilar o núcleo. Resultado: RTX 3060 identificada; compilador/SDK e editor Unreal utilizáveis não encontrados nas buscas seletivas. C++20/MSVC definido como caminho mínimo; proposta em FERRAMENTAS.md. Nenhuma instalação ou compilação realizada.
- [x] AMB-01 — Usuário escolheu e instalou manualmente Visual Studio 2026. Validado VS Community 2026 18.12.1, MSVC 14.52.37025 e Windows SDK 10.0.26100.0: diagnóstico próprio C++20 compilado, vinculado e executado em x64, com /W4 /WX, sem avisos e com saída zero. Comando repetível em AGENTS.md e evidência em AMBIENTE.json. Nenhuma instalação pelo agente; sem gameplay ou engine.
- [x] COM-01 — Núcleo C++20 independente implementado: simulação 3x3, ataque/guarda, rodadas, derrota e vitória; comandos inválidos sem mutação. 13/13 testes aprovados; referência determinística de 35 ações/12 rodadas e demo com guarda de 40 ações/14 rodadas. Build/teste via scripts/testar-combate.ps1; resultado em COMBATE.json, limites em COMBATE.md. Sem UI interativa, engine, artes ou progressão.
- [x] COM-01A — Antes da interface, analisar formações/fases/modos e ampliar testes. 168 cenários + 168 replays aprovados, matriz completa de tamanhos 1–5, sondagens de seis, três contextos 5x1 intercalados e 1000 simulações funcionais locais. Pesquisa pública com procedência e limites; contratos de fases/chefes em docs/batalhas/. Nenhuma implementação desses coordenadores ou mudança do núcleo de produção.
- [x] TIME-01 — Orçamento local opcional de ações principais de ambos os lados implementado; empate ou derrota de lado escolhido, motivo de término e contador no snapshot/eventos. Vitória na última ação prevalece. 11/11 testes novos + 13 anteriores e matriz aprovados. Contrato global documentado, não implementado. Detalhes em docs/batalhas/TIME-01.md; dúvidas futuras centralizadas em PENDENCIAS.md.
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
- [ ] PAR-01 — Coordenador de três frentes independentes; ampliar a prova de isolamento para condições de vitória/derrota da fase, budgets e cancelamento. Não requer threads por padrão. Aceite: sem starvation ou vazamento de estado; duas ordens de escalonamento; replay por frente e resultado global.
  - [x] Parte 1 — Coordenador, alternância determinística, isolamento, resultado agregado e replay/reset; 120 testes totais aprovados. Relatório docs/batalhas/PAR-01.md.
  - [x] Parte 2 — Orçamento global, precedência local/global e cancelamento coordenado; 125 testes totais aprovados.
  - [ ] Parte 3 — Integração R04/habilidades, limites técnicos e fronteiras finais.
- [ ] PAR-02 — Após decidir o modelo, investigar chefe em três frentes: HP autoritativo, partes ou avatares/score. Aceite: mortes simultâneas, overkill, efeitos/skill-phase globais uma vez, orçamento e ordem total. Não substituir essa implementação por clones independentes do chefe.

## 3. Uso, habilidades ampliadas e persistência

- [ ] COM-02 — Após os modos prioritários: entrada de ações e reinício interativo no protótipo textual, reutilizando as regras do motor; sem UI gráfica obrigatória.
- [ ] HAB-01 — Habilidades distintas, recurso e primeiros efeitos de estado; especificar e testar antes de expandir conteúdo.
- [ ] CIC-01 — Seleção de equipe, recompensa mínima e salvamento local versionado.

## 4. Engine e apresentação funcional

- [ ] ENG-01 — Escolher engine estável e validar compatibilidade com o núcleo e hardware. Teste pequeno de integração; instalação/download e escolha definitiva exigem decisão específica.
- [ ] VIS-01 — Integrar núcleo à primeira arena visual com placeholders. Aceite: selecionar ação/alvo, ver turno/vida/resultado e reiniciar; nenhuma regra duplicada na UI. Sem qualidade artística final.

## 5. Identidade e conteúdo

- [ ] ART-01 — Proposta de direção artística original e um personagem representativo; manter direitos e arquivos-fonte.
- [ ] AUD-01 — Áudio provisório licenciado/próprio, feedback e definição da identidade musical.
- [ ] EXP-01 — Somente após avaliação: campanha pequena e minijogo rítmico original.

## Próximo bloco executável

PAR-01 parte 3: integração de habilidades/R04, Skip, falhas técnicas e fronteiras de limite/cancelamento. Partes 1/2 concluídas; ler docs/batalhas/PAR-01.md. Uma entrega por vez.

## Encerramento de cada entrega

Atualizar ESTADO.md com implementação, testes efetivos, limites e próxima ação. Não repetir bateria/pesquisa sem mudança pertinente. Não interpretar esta fila como execução de todos os blocos em uma única tarefa.
