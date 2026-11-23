# Fila vigente — 23/11/2026

Esta ordem substitui as prioridades de execução do histórico abaixo. LAB-PVE-01 a 06 e LAB-QA-01, 01B e 01C já foram entregues em código. A transição por botões no panorama foi confirmada pelo usuário. O acabamento e o gesto abaixo são novos trabalhos. Esta revisão é de planejamento.

| Ordem | Bloco | Próxima entrega e critério |
|---|---|---|
| 1 | HUB / VIS — Fade do panorama | Fazer os letreiros da área atual desaparecerem na saída. Exibir por fade apenas os destinos visíveis depois do encaixe. Reutilizar o comportamento para botões e arraste. Retomar cliques dos letreiros quando a vista estiver estável e legível. Movimento reduzido usa troca imediata. |
| 2 | HUB / VIS — Arraste e encaixe | Permitir arraste horizontal com mouse e toque, usando o recorte atual. A imagem acompanha o gesto. Ao soltar, distância e velocidade horizontal determinam encaixe na área vizinha ou retorno à área atual. Preservar os três botões, o clique único e a distinção entre toque curto e arraste. Tratar cancelamento, perda de foco, bordas e redimensionamento. Pressão do toque fica fora do gesto. |
| 3 | VIS / QA — Validação em janela | Verificar arraste curto, rápido, cancelado e nos extremos, sequência de cliques rápidos, fade após encaixe e movimento reduzido. Compilar a prévia para aceite em janela horizontal e vertical, com teste de mouse. Calibrar toque físico na etapa mobile. |
| 4 | VIS / AUD / ENG — Apresentação de combate | Rever os eventos já expostos por AUT-13 e ligar som e animação provisórios ao início, impacto e fim da ação, sem reaplicar efeitos de combate. Arte, rig e áudio finais podem chegar em paralelo. |
| 5 | HUB — Tela do Terminal | Integrar a composição visual quando o material chegar. Reusar as rotas existentes para laboratório, tutorial padrão e demais encontros. O trabalho de apresentação pode avançar enquanto a arte do Terminal está pendente. |

## Aceites em paralelo

Conferir os dois tutoriais, os novos modos PvE do Terminal, a navegação, a HUD em outras proporções e o registro local das conclusões pela interface. Revisitar clique, vídeo e cursor conforme regressões relatadas. O teste de falhas do backend Unreal continua planejado em ambiente isolado, separado dos saves normais.

## Hub 3D a decidir

- Validar o enquadramento com a Praça e Games mais ao centro, fachadas voltadas para o meio, trilho direito baixo e grosso, monotrilho à esquerda e Pub rebaixado à direita.
- Quando houver decisão sobre o formato editável, preparar uma cena separada. O mapa passa a guardar as posições, rotações e escalas das peças, os pontos dos letreiros e as referências da câmera. O código usa identificadores estáveis para associar cada destino à rota existente. Os pontos de cada fachada acompanham a movimentação do respectivo grupo. O carregamento passa a usar os elementos salvos para evitar peças duplicadas.
- Depois de definir o mapa, avaliar a câmera 3D entre esquerda, centro e direita, com percurso e enquadramento por ponto de vista, aproximação do Terminal e movimento reduzido. Coordenar edições simultâneas no mesmo mapa binário.
- A edição do mapa e a câmera lateral 3D dependem da avaliação do usuário. As etapas 1 a 4 podem avançar em paralelo a essa decisão.

## Mobile e distribuição

Android permanece adiado. Quando retomado, verificar gesto de toque e sua sensibilidade, proporções e áreas seguras, vídeo e áudio real no dispositivo e empacotamento. Shipping, lojas, serviços e MCP seguem frentes próprias. Progresso geral mantido em aproximadamente 28%.

---

## Histórico da fila
> **Integração23/11:** recursos ligados à HUD; tutorial padrão no Terminal; busca clássica no Lab - Exp; conclusão local própria, PRAÇA/letreiros corrigidos. Código/testes/build e stage92 aprovados; aceites gráficos/I-O real/mobile separados. Demo~28% mantida. Fonte: docs/projeto/INTEGRACAO-VIS-TUTORIAIS-2026-11-23.md. Próximo: aceite da versão integrada, sem depender da arte futura do Terminal.

> **LAB-QA-01C — 22/11:** persistência compartilhada entre HUD Unreal e QA; 16/16 testes isolados, compilação e stage69 aprovados. Formatos/políticas preservados; saves normais intocados. Aceites LQA-09/10 e runtime Unreal isolado de LQA-14 continuam separados. Demo ~28% mantida. Fonte: docs/projeto/LAB-QA-01C-PERSISTENCIA-COMPARTILHADA.md. Próxima frente independente proposta: revisar contratos de áudio/animação sobre eventos existentes, sem repetir AUT-13.

> **LAB-QA-01B — 22/11:** testes de disco em pasta nova de QA, com codecs reais e backend Windows próprio; saves normais intocados. Não equivale a executar IFileManager/interface Unreal. LQA-14 de integração continua adiado. Próximo independente LAB-QA-01C: contrato compartilhado de persistência; aceites visuais em paralelo. Demo ~28% mantida. Detalhes: docs/projeto/LAB-QA-01B-IO-ISOLADO.md.

> **LAB-QA-01 — 22/11:** feedback de ações e recusas integrado; detalhes consultáveis e matriz de 16 aceites preparada. Código/testes não equivalem a aceite visual ou I/O real. Próximo independente: LAB-QA-01B, I/O em diretório isolado; usuário pode preencher a matriz em paralelo. Demo ~28% mantida. Ver docs/projeto/LAB-QA-01-FEEDBACK-E-ACEITES.md.

> **LAB-PVE-06 — 22/11:** [três frentes com chefe único](LAB-PVE-06-FRENTES-COMPARTILHADAS.md) no Terminal, página3, ataques locais e especial global. Contadores por frente e limite global, sem limite local novo. LAB01–06 em código; aceites02–06 pendentes. Próximo recomendado LAB-QA-01/aceites. Demo ~28% mantida.

> **LAB-PVE-05 — 22/11:** [três equipes sequenciais no Terminal](LAB-PVE-05-EQUIPES-SEQUENCIAIS.md), variantes de chefe fixo/faseado, HP contínuo e HUD. Janela4ações totais/equipe é fixture explícita. Aceites02–05 pendentes; próximo06frentes compartilhadas. Demo ~28% mantida.

> **LAB-PVE-04 — 22/11:** [score/sobrevivência e recibo no Terminal](LAB-PVE-04-SCORE-SOBREVIVENCIA.md). Acessos na página2; janela12ações e meta3turnos naturais são parâmetros de laboratório. Aceites visuais02–04 pendentes; próximo05equipes sequenciais. Demo ~28% mantida.

> **LAB-PVE-03 — 22/11:** [ondas e fases no Terminal](LAB-PVE-03-ONDAS-FASES.md), com HUD/contadores e consulta de transições. Nova integração aguardando aceite visual; LAB-PVE-02 também não teve aceite informado ainda. Próximo LAB-PVE-04, score/sobrevivência. Demo ~28% mantida.

> **LAB-PVE-02 — 22/11:** [Ritual local integrado ao Terminal](LAB-PVE-02-RITUAL-TERMINAL.md), com objetivo, HUD, resultado e tentativas isoladas. Código/testes/build; aceite visual da nova rota pendente. Próximo LAB-PVE-03, ondas/fases. Demo ~28% mantida.

> **LAB-PVE-01 entregue — 22/11:** [observação e contrato de HUD](LAB-PVE-01-CONTRATO-HUD.md), sem nova tela/rota habilitada. Próximo: **LAB-PVE-02, Ritual no Terminal**; não depende de nova entrega. Demais itens da fila continuam planejados. Progresso global ~28% mantido.

# Fila de desenvolvimento independente das entregas

22/11/2026. Planejamento vigente, sem executar os itens nesta revisão. Não espera arte, áudio ou pesquisa adicionais para os cortes abaixo. Cada entrega segue código → teste pertinente → compile se aplicável → documentação/commit → aceite visual posterior. Sem processo autônomo em segundo plano.

| Ordem / ID de planejamento | Família | Entrega concreta | Pode fazer sem material novo? | Dependência real / aceite |
|---|---|---|---|---|
| 1 LAB-PVE-01 | ENG / PvE / VIS | Mapear adaptadores existentes, snapshots e eventos; contrato de HUD e rota do Terminal | Sim, usando casos de código existentes | Inspeção local; sem decisão nova de combate |
| 2 LAB-PVE-02 | HUB / ENG / PvE | Um Ritual simples acessível pelo Terminal, iniciar/retornar e resultado | Sim, fixtures identificadas como teste | Depende do contrato 01; você avalia a tela depois |
| 3 LAB-PVE-03 | WAVE / VIS | Indicadores de onda/fase e resumo das transições já aprovadas | Sim | Encontro integrado em 02; conferir visualmente depois |
| 4 LAB-PVE-04 | PvE / CIC | ScoreAttack e Survival com objetivo, limite e score/recibo | Sim | Reutiliza 01–03; balanceamento final pode vir depois |
| 5 LAB-PVE-05 | SEQ / VIS | Equipe ativa, próxima equipe, transição e estado persistido entre confrontos | Sim | Snapshot/transição; aceite após implementação |
| 6 LAB-PVE-06 | PAR / VIS | Três frentes, HP compartilhado e relógio global do chefe com identificação clara | Sim, no modelo local aprovado | Depois de SEQ/HUD; não promete simultaneidade de servidor |
| Paralelo LAB-QA-01 | VIS / QA | Feedback de ações/efeitos/recusas e roteiro de validação de save/tutorial/resolução | Sim | Usa eventos e contratos existentes; aceites reais ficam com você |

IDs LAB-PVE são cortes de planejamento novos, não blocos já implementados nem nova família. Registrar a evidência individual ao entregar. Não implementar todos os menus antes de validar o primeiro corte utilizável. Não duplicar regras no HUD.

## Fora da dependência das suas entregas

Preparar validadores de catálogo já definido, contratos de recursos, eventos de áudio/animação com placeholders, cenários de teste e documentação. Produção final de assets não é necessária para esses contratos. Priorização continua na tabela, para evitar dispersão.

## Exige sua entrega/decisão para fechar conteúdo final

Amostra de personagem/rig/animação/arena final; sons e vozes finais; ícones/retratos e procedência; roteiro curto final; kits/encontros inéditos não definidos. Não inventar regras faltantes, licenças ou recursos finais. Entrega parcial por ID/versão é suficiente.

## Exige aceite posterior, não material novo

Telas novas; persistência real após reabrir; backup/recuperação; resoluções/formaçōes; tutorial específico e acessibilidade. Você pode testar pelos programas; o assistente não deve abrir telas automaticamente. Cliques/vídeo/placeholder/seletor já aceitos não voltam à fila sem regressão concreta.

## Adiado por prioridade/permissão

Android/APK e backend mobile, cook/Shipping limpo, lojas/servidor, MCP opcional e clássico ampliado. Não instalar ou mudar rede. Build sem tela é possível com editor fechado; controle visual requer nova autorização.

[Progresso ~28%](PROGRESSO-GERAL.md) · [Sua preparação](../../entregas/NECESSIDADES-ATUAIS.md) · [Famílias](MAPA-FAMILIAS-E-PRIORIDADES.md).
