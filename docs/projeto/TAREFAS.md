> **Prioridades vigentes em 23/11/2026.** Apresentação de combate por AUT-13, preparação dos aceites em janela e refinamento do Hub 3D avançam com o material atual. A tela final do Terminal depende da imagem. O mapa editável segue após o aceite dos materiais provisórios e a decisão de formato. Os [afazeres](FILA-DESENVOLVIMENTO-INDEPENDENTE.md) e a [central de entregas](../../entregas/NECESSIDADES-ATUAIS.md) separam execução, materiais e aceites.

> **23/11/2026.** Hubs 2D e 3D com deslize validado. APK no Pixel 7 virtual confirmou toque, rotação, vídeo e uma ação de combate. A próxima rodada Android cobre persistência, áudio e desempenho em aparelho físico. Mapa editável e acabamento de combate seguem na [fila de desenvolvimento](FILA-DESENVOLVIMENTO-INDEPENDENTE.md).

> **Integração23/11:** recursos ligados à HUD; tutorial padrão no Terminal; busca clássica no Lab - Exp; conclusão local própria, PRAÇA/letreiros corrigidos. Código/testes/build e stage92 aprovados; aceites gráficos/I-O real/mobile separados. Demo~28% mantida. Fonte: docs/projeto/INTEGRACAO-VIS-TUTORIAIS-2026-11-23.md. Próximo: aceite da versão integrada, sem depender da arte futura do Terminal.

> **LAB-PVE-06 — 22/11:** [três frentes com chefe único](LAB-PVE-06-FRENTES-COMPARTILHADAS.md) no Terminal, página3, ataques locais e especial global. Contadores por frente e limite global, sem limite local novo. LAB01–06 em código; aceites02–06 pendentes. Próximo recomendado LAB-QA-01/aceites. Demo ~28% mantida.

> **LAB-PVE-05 — 22/11:** [três equipes sequenciais no Terminal](LAB-PVE-05-EQUIPES-SEQUENCIAIS.md), variantes de chefe fixo/faseado, HP contínuo e HUD. Janela4ações totais/equipe é fixture explícita. Aceites02–05 pendentes; próximo06frentes compartilhadas. Demo ~28% mantida.

> **LAB-PVE-04 — 22/11:** [score/sobrevivência e recibo no Terminal](LAB-PVE-04-SCORE-SOBREVIVENCIA.md). Acessos na página2; janela12ações e meta3turnos naturais são parâmetros de laboratório. Aceites visuais02–04 pendentes; próximo05equipes sequenciais. Demo ~28% mantida.

> **LAB-PVE-03 — 22/11:** [ondas e fases no Terminal](LAB-PVE-03-ONDAS-FASES.md), com HUD/contadores e consulta de transições. Nova integração aguardando aceite visual; LAB-PVE-02 também não teve aceite informado ainda. Próximo LAB-PVE-04, score/sobrevivência. Demo ~28% mantida.

> **LAB-PVE-02 — 22/11:** [Ritual local integrado ao Terminal](LAB-PVE-02-RITUAL-TERMINAL.md), com objetivo, HUD, resultado e tentativas isoladas. Código/testes/build; aceite visual da nova rota pendente. Próximo LAB-PVE-03, ondas/fases. Demo ~28% mantida.

> **LAB-PVE-01 entregue — 22/11:** [observação e contrato de HUD](LAB-PVE-01-CONTRATO-HUD.md), sem nova tela/rota habilitada. Próximo: **LAB-PVE-02, Ritual no Terminal**; não depende de nova entrega. Demais itens da fila continuam planejados. Progresso global ~28% mantido.

# Tarefas — quadro vigente

22/11/2026. Demo ~28% (26–30%). Percentual não é quantidade de tarefas concluídas.

## Entregue e preservado

- [x] AUT-01–16 em seus recortes: estrutura, entrada, painéis, mídia, preferências/modelos de save, cues/schema e preparação/QA.
- [x] SIS/REG/HAB: ponto fixo, funil, controles, Bleed/Poison e piloto Felipe testados.
- [x] ESC/SIS/PvE C1–C6 em código: escudos especializados, ondas, score, modos, fases/reservas e frentes.
- [x] Cliques únicos, vídeo/skip, placeholders, seletor e conclusão da batalha Felipe aceitos no Windows testado.
- [x] Dois laboratórios pelo hub, mesma base Team e limite de 200 ações/empate; kits/saves separados; clássico preservado.

## Desenvolvimento sem nova entrega sua

- [x] LAB-PVE-01: adaptadores mapeados, snapshot e contrato de HUD implementados/testados.
- [x] LAB-PVE-02: Ritual no Terminal com objetivo, resultado e retorno; aceite visual pendente.
- [x] LAB-PVE-03: ondas/fases e detalhes de transição implementados; aceite visual pendente.
- [x] LAB-PVE-04: score/sobrevivência e recibos implementados; aceite visual pendente.
- [x] LAB-PVE-05: equipes sequenciais e variantes do chefe implementadas; aceite visual pendente.
- [x] LAB-PVE-06: frentes locais/chefe único e escopos integrados; aceite visual pendente.
- [ ] LAB-QA-01 em paralelo: feedback e roteiro de save/tutorial/resoluções.

Essa é a ordem do próximo desenvolvimento, não execução nesta atualização. Detalhes e dependências na FILA-DESENVOLVIMENTO-INDEPENDENTE.md.

## Aguarda aceite específico seu

- [ ] Gravar/carregar/reabrir equipe/progresso/preferências; recuperação de backup.
- [ ] Tutorial explícito com preload; resoluções, formações e acessibilidade amplas.
- [ ] Telas futuras de cada LAB-PVE, depois de implementadas.

## Aguarda material ou decisão sua para versão final

- [ ] Um encontro representativo com seus valores/kit/objetivo final; novos efeitos só quando não definidos.
- [ ] Ícones/retratos/textos por ID e confirmação de procedência.
- [ ] Uma amostra de personagem/rig/clips/arena e áudio por evento.
- [ ] Roteiro final curto do piloto; regras novas de progressão/itens se forem entrar no recorte.

Não é necessário reenviar fórmulas fechadas, kit Felipe, vídeos ou panorama atuais. Recursos finais não bloqueiam os cortes de laboratório acima. Android, MCP, clássico ampliado e publicação ficam adiados. [Recebíveis](../../entregas/NECESSIDADES-ATUAIS.md).
## Extensoes de codigo fora dos cortes LAB-PVE-01–06

- [ ] Ponte de runtime para passivas BattleStart em ondas/sequenciais/frentes.
- [ ] Mapeamento explicito quando fases trocarem IDs/slots de habilidades.
- [ ] Layout/kit de auxiliares por frente; core ja possui escopos.
- [ ] Adaptador de tres chefes independentes, sem confundir com HP compartilhado.
- [ ] Matriz visual ampla (incluindo15aliados), feedback e persistencia real.

Engenharia posterior, nao pesquisa obrigatoria nova do usuario. Nao iniciar Android/MCP/classico/servidor por inferencia.
