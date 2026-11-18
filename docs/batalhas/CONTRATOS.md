# Contratos de combate e fases — análise antes da COM-02

Estado: contratos por família, **não implementados** salvo onde indicado. TIME-01 implementou orçamento local de ações descrito abaixo; WAVE-01 implementou o coordenador consecutivo descrito em WAVE-01.md; demais coordenadores permanecem propostos. Motor com duas equipes e uma ação por vivo por rodada; ataque e guarda. Regras do nosso projeto, sem alegar reproduzir internamente outro jogo. Dúvidas e decisões em aberto estão centralizadas em [PENDENCIAS.md](../projeto/PENDENCIAS.md).

## 1. Formação não é lado de combate

Separar `SideId` (aliança/inimizade), `SquadId` (time de formação), `BattleId` (confronto) e `FighterInstanceId` (instância). Identidade de personagem de catálogo é outro ID. Um lado pode reunir mais de um time, mas não se deve perder o time de origem para auras, cura, alvo e pontuação.

Configurar limites por modo: tamanho mínimo/máximo do time do jogador, tamanho do grupo inimigo, reservas, invocações e número de times. Cinco é perfil inicial, não constante global do núcleo. Os testes adicionais com seis não ativam modo de seis como padrão. Impor limites de dados e capacidade explicitamente na camada de composição antes de alocar; não presumir capacidade ilimitada do kernel.

Casos futuros: 0 integrantes rejeitado; 1 e 5 aceitos no perfil básico; 6 rejeitado nesse perfil e aceito em perfil próprio; IDs repetidos na mesma batalha rejeitados; mesmo personagem em outros times depende da política do modo. Invocação não deve contornar limite por erro de contagem.

## 2. Vocabulário de tempo

- **Comando:** tentativa de escolher uma ação. Inválido não consome orçamento.
- **Turno ativo:** oportunidade de um combatente realizar uma ação principal. Ataque/guarda aceitos consomem uma unidade neste primeiro perfil.
- **Ação derivada:** contra-ataque, assistência ou gatilho futuro. Orçamento de eventos próprio, sem consumir turno principal por padrão; essa política precisa constar do modo.
- **Rodada completa:** um ciclo de oportunidades dos vivos no perfil de ordem fixa. `round()` hoje numera a rodada em andamento, começando em 1; não significa rodadas já concluídas.
- **Tempo lógico:** ordem da simulação, independente da duração de animação e de FPS.
- **Tempo de parede:** duração real de execução; serve a watchdog técnico/telemetria e não deve alterar replay por padrão.

Implementado em TIME-01: limite local opcional em ações principais aceitas, contando ambos os lados, incluindo guarda; comandos inválidos e saltos de mortos não contam. Resultado explícito: empate, derrota de A ou derrota de B. Vitória por eliminação na última ação prevalece. Sem configurar orçamento, o perfil continua sem limite. API e evidências em TIME-01.md. Outras unidades, budgets de fase e resultado por score permanecem em PENDENCIAS.md.

Cada orçamento tem unidade, escopo (`battle`, `lane`, `phase`, `stage`), limite positivo e resultado por esgotamento (`defeat`, `draw`, `score`). Orçamentos locais e globais são distintos. Resolver integralmente a ação permitida e sua vitória antes de aplicar o timeout: vitória na última ação válida vence; não iniciar nova ação após esgotar. Uma fase não pode conceder a cada frente o orçamento global inteiro por engano.

Aceite local de TIME-01 verificado: limites 1/2/N, guarda, invalidez sem consumo, mortos, vitória na última ação, timeout e tentativa posterior, configuração inválida e replay. Ações extras/assistências e orçamento de reações continuam sem implementação; decisão registrada em PEN-09, não coberta por estes testes.

O contador de segurança 4096 dos cenários atuais não é timeout de jogo. Um teste que depende dele para terminar deve falhar como inconclusivo, não conceder vitória/empate.

## 3. Cinco confrontos consecutivos com um time

Introduzir `Stage` com até cinco `EncounterDefinition` no perfil inicialmente solicitado. Vitória de um confronto intermediário não é vitória da fase. A fase controla transição, orçamento global e resultado final.

Políticas de passagem devem ser obrigatórias, não defaults silenciosos:

| Estado | Opções a definir por modo |
|---|---|
| Vida atual | Conservar; restaurar; restaurar percentual definido |
| Derrotados | Permanecer derrotados; reviver conforme regra explícita |
| Escudos/efeitos | Conservar os permitidos, expirar ou limpar |
| Energia/recargas | Conservar ou resetar de forma definida |
| Ordem/AP | Recalcular, conservar progresso ou resetar |
| Contador | Por confronto ou acumulado da fase |
| RNG e score | Estado/semente e placar da fase separados |

Antes de WAVE-01, o construtor criava todos com vida máxima. Agora existe entrada validada de vida separada da definição; Stage implementa as políticas descritas em WAVE-01.md. Recriar Battle entre ondas hoje restaura vida: **não é implementação válida de persistência**. Não usar a vida restante como novo max_health: isso adulteraria atributos e efeitos percentuais futuros. Será necessária entrada de estado validada, mantendo definição e estado distintos, ou orquestração que preserve as instâncias.

Aceite de WAVE-01: 1, 2 e 5 confrontos; encerrar no meio por derrota; não saltar confronto; preservar vida máxima e atual corretamente; não ressuscitar por acidente; não restaurar recarga ao trocar cena; resetar tudo ao começar uma nova fase; crédito de recompensa uma única vez. Snapshot com StageId/EncounterId/configVersion, sem reaproveitar eventos antigos.

## 4. Três times sucessivos e chefe por etapa

Separar identidade do chefe de perfil de habilidades. Cada `BossPhaseDefinition` tem ID, conjunto de habilidades, condição de entrada/saída, política de vida/efeitos, orçamento e equipe ativa. Isso permite manter a identidade narrativa sem fixar um comportamento único no Fighter.

Decisões antes dessa implementação: PEN-04, PEN-06 e PEN-10 em PENDENCIAS.md. Esse é o registro central de vida/recargas do chefe, condição de troca, fronteira de habilidades, dano e score; não adotar defaults silenciosos.

Aceite de SEQ-01: times 1→2→3 sem ordem incorreta; habilidades certas em cada fase; nenhum efeito/recarga do time anterior vaza indevidamente; dano de transição aplicado uma vez; nenhum crédito duplicado; derrota/timeout/vitória priorizados; reentrada no encontro não restaura chefe por engano. O relato do usuário define esse cenário, não suas respostas de persistência.

## 5. Três chefes independentes em paralelo

Três BattleContexts com estado e IDs de instância separados, coordenados por uma fase. A apresentação pode mostrá-los ao mesmo tempo mesmo se o cálculo usar uma fila única determinística. Não é necessário criar três threads para três animações simultâneas.

**Verificado agora:** três batalhas 5x1 intercaladas numa thread, com IDs repetidos entre contextos, mantêm estado/eventos iguais às execuções individuais; alternar a ordem das frentes não muda os resultados independentes. Cada ação de uma frente deixa as outras intactas.

**Não verificado:** relógio unificado, efeitos entre frentes, chefes compartilhados, threads, GPU, multiplayer, derrota/vitória global e sincronização de UI. Reutilizar o mesmo objeto Battle em threads não é seguro por contrato. Se precisar de concorrência física, particionar estado e passar mensagens, com sincronização testada.

Coordenador futuro define sucesso `all`, `any`, `score` ou outra política explícita. Frente encerrada para de receber ações; cancelamento das restantes em vitória global deve ser determinístico. Budgets por frente não são automaticamente budgets do jogador completo.

Aceite de PAR-01 completo: três inícios, falha numa frente, fim antecipado noutra, duas ordens de escalonamento, ausência de starvation, IDs locais repetidos, escopo de buffs, placares agregados e reinício conjunto. Sementes por contexto e chave de desempate estável, sem depender da ordem de threads.

## 6. Três frentes contra um chefe

O relato “mesmo chefe” admite modelos diferentes; decisão em PEN-05 de PENDENCIAS.md. Alternativas de contrato:

1. **HP único autoritativo:** três frentes produzem comandos/dano para um BossState central. Não manter três HPs independentes e tentar somá-los depois.
2. **Partes distintas:** cada parte tem vida/efeitos e relações com o núcleo; destruição pode desbloquear fase ou transferir dano.
3. **Avatares/instâncias com placar conjunto:** três combates separados somam resultado, sem compartilhar HP em tempo de batalha.

Para HP compartilhado, o coordenador resolve ações em ordem total determinística, por exemplo `(tempo_logico, frente_id, sequencia_local)`. Precisa decidir qual frente recebe ataques do chefe e com que cadência, quantas vezes um debuff global é aplicado e quando uma mudança de habilidades é publicada para todas as frentes. “Uma vez por turno do chefe” não pode virar três vezes por acidente.

Aceite de PAR-02: dois golpes letais no mesmo instante; HP nunca negativo; morte e vitória uma única vez; dano excedente não duplica score; efeito global não triplica; etapa de habilidade troca uma vez; nenhuma frente continua batendo após fim global; pausas e speeds de animação não mudam a sequência lógica. Esses contratos exigem coordenação nova: não são fornecidos por três objetos Battle atuais.

## 7. Ação por rodada não equivale a barra de ação

Atualização TIME-02: ActionBar foi implementado como perfil independente da referência fixa, com extras/contra-ataques e relógios mínimos. Contrato e verificação em TIME-02.md. O texto a seguir registra a motivação original; não indica ausência atual de AP.

O kernel atual dá uma ação por vivo em cada rodada, com speed definindo apenas a ordem. Isso é uma simplificação nossa. Antes de habilidades complexas, avaliar um perfil de **barra de ação/AP** separado, se esse for o design escolhido. Uma entidade mais rápida pode precisar agir mais de uma vez enquanto outra não agiu; mudanças de speed/AP, turnos extras e ações fora de turno exigem scheduler, não só reordenar um vetor.

Não refazer esse sistema por imitação: definir qual ritmo queremos e preservar o perfil simples como referência de regressão. Duração de debuffs por turno do portador, turno do aplicador e rodada global não são equivalentes.

## 8. Crescimento e desempenho

Score, dano acumulado, contador de eventos e tempo lógico precisarão de tipos/limites explícitos e aritmética verificada antes de modos de dano prolongado. O dano atual usa int positivo e foi testado em INT_MAX, mas multiplicadores, porcentagens e score não existem ainda. Trocar para 64 bits não elimina a necessidade de checar overflow. Evitar depender de float de apresentação nas decisões determinísticas; estudar escala inteira/fixed-point quando houver porcentagens.

Eventos devem carregar BattleId/StageId e sequência quando saírem do contexto local; gravação de replay com versão da configuração. Limitar cadeia de gatilhos sem confundir falha técnica com derrota do jogador. Não manter histórico completo ilimitado em produção; os traces integrais dos testes são pequenos e deliberados.

Camadas a medir separadamente: núcleo headless, orquestrador, animação/UI/render, rede/persistência de backend. Um ensaio funcional de 1000 batalhas numa thread sem engine não estima jogadores simultâneos, FPS mobile ou capacidade de servidor. Não existe backend neste projeto.

Depois: ensaio Release com aquecimento, várias amostras, percentis de latência, memória máxima/alocações, builds diferentes, limite de duração e mesmo conjunto de dados; em paralelo, medir uma e três frentes visuais no hardware-alvo. Testes de servidor somente quando ele existir, com sessões, fila, mensagens duplicadas, reconexão e persistência idempotente. Não dimensionar infraestrutura a partir do lote atual.

## Sequência de entregas

COM-01A, TIME-01 e WAVE-01 concluídas. Depois SEQ-01 (times/etapas do chefe), PAR-01 (coordenador independente) e PAR-02 (chefe compartilhado). COM-02 segue em espera. Reavaliar escopo e consumo a cada bloco; consultar somente as pendências necessárias à entrega atual. Integração gráfica e rede permanecem posteriores.


