# Pendências e pontos a esclarecer

Registro central, atualizado em 18/11/2026. Consultar somente os itens relevantes ao bloco atual. Uma pendência de etapa futura não impede entregar o que já tem regras definidas. Não decidir silenciosamente comportamentos de jogo nem apresentar hipóteses como capacidades implementadas.

## Decisões em aberto

| ID | Ponto a esclarecer | Quando resolver | Critério de encerramento |
|---|---|---|---|
| PEN-01 | O que persiste entre confrontos: vida, derrotados, efeitos, energia, recargas e ordem/AP? | WAVE-01 | Política explícita de passagem e testes de 1/2/5 confrontos; máximo de vida não adulterado e nenhuma cura/reanimação implícita |
| PEN-02 | Como contabilizar um orçamento global da fase junto dos limites de cada confronto/frente? | WAVE-01 e PAR-01 | Um contador global autoritativo; unidades compatíveis, precedência e testes sem reset/triplicação acidental |
| PEN-03 | Quais IDs representam personagem, instância, time, lado, confronto, etapa e fase? Pode repetir personagem entre times? | WAVE-01/SEQ-01 | Escopos e regras de composição definidos; IDs locais repetidos não causam vazamento |
| PEN-04 | Na troca dos três times, chefe mantém HP/efeitos/recargas? Qual condição troca seu conjunto de habilidades? | SEQ-01 | Perfil por etapa, fronteira da transição e política de persistência com testes; não confundir identidade narrativa e estado |
| PEN-05 | Três frentes contra o mesmo chefe usam HP único, partes ou avatares com score agregado? | Antes de PAR-02 | Escolher o modelo do nosso jogo; eventual referência histórica identificada sem presumir sua arquitetura |
| PEN-06 | Como resolver ações com mesmo tempo lógico, golpes letais e limiares de fase concorrentes? | PAR-02 | Ordem total estável, uma conclusão de chefe e efeito global aplicado uma única vez |
| PEN-07 | Manter ordem fixa por rodada ou introduzir barra dinâmica de ação/AP? Como speed, controle e turnos extras interagem? | Antes de scheduler/habilidades complexas | Perfil escolhido e especificado, sem confundir round() com duração por portador ou frequência de ações |
| PEN-08 | Quais efeitos e habilidades afetam time, frente ou todos do mesmo lado? | Antes de habilidades entre frentes | Seletores com escopo explícito e casos de isolamento/compartilhamento |
| PEN-09 | Quais ações derivadas contam para o limite? Como parar loops de reação sem alterar a regra de derrota? | Antes de HAB-01 | Políticas e orçamento técnico de eventos separados; contra-ataques/assistências testados quando existirem |
| PEN-10 | Qual regra de empate/score adotar para cada modo e como contar overkill, morte simultânea e recompensa? | Antes dos modos correspondentes | Resultado e aritmética explícitos; conclusão e crédito idempotentes; score não existe no núcleo atual |
| PEN-11 | Quais limites por perfil para formação, reservas, invocações, número de confrontos e times? | Camada de composição/WAVE-01 | Dados validados com extremos; não impor cinco como constante global do kernel |
| PEN-12 | Um perfil precisa de ações apenas do jogador, turnos do chefe, rodadas completas ou tempo real? | Quando um modo exigir outra unidade | Unidade e escopo explícitos com testes próprios; TIME-01 só implementa ações principais aceitas de ambos os lados |
| PEN-13 | Tipos, escala numérica e limites de acumuladores de dano/score/tempo, com porcentagens e multiplicadores? | Antes de introduzir esses valores | Aritmética verificada e determinismo entre plataformas; 64 bits isoladamente não elimina overflow |
| PEN-14 | Como reiniciar/retomar fase sem reaplicar eventos ou conservar estado indevido? | WAVE-01 e futura persistência | Configuração/IDs versionados; reset e reentrada testados; snapshot atual não é formato de save |

## Capacidades ainda não validadas

| ID | Capacidade | Evidência que já existe | O que ainda falta |
|---|---|---|---|
| VAL-01 | Fases consecutivas e persistência | Batalhas isoladas e orçamento local | Orquestrador de fases, estado de entrada e política PEN-01/PEN-02 |
| VAL-02 | Chefe compartilhado e transições de habilidades | Relato do cenário e contratos documentados | Estado autoritativo/partes/avatares escolhido e implementação; resolver PEN-04/PEN-05/PEN-06 |
| VAL-03 | Três frentes como modo completo | Isolamento de três contextos intercalados em uma thread | Coordenador, resultado global, cancelamento, budgets e apresentação |
| VAL-04 | Paralelismo real entre threads | Nenhum ensaio concorrente | Necessidade medida, particionamento/sincronização e testes; Battle não é thread-safe por contrato |
| VAL-05 | Servidor, multiplayer e capacidade de usuários | Apenas ensaio funcional local sem rede | Backend, protocolo/autoridade/persistência e teste de carga específico; não dimensionar pelo lote local |
| VAL-06 | Desempenho visual e mobile | GPU enumerada e núcleo textual | Engine/hardware-alvo, cenas, UI/VFX e medições de FPS/memória; cálculo local não mede renderização |
| VAL-07 | História técnica e causa de alteração de modos de referência | Pesquisa seletiva, relatos e fontes classificadas | Fonte primária verificada para cada causalidade; não assumir teste de servidor ou limite de engine |

## Definido em TIME-01 — não está pendente

- Orçamento local opcional por batalha, expresso em ações principais aceitas de ambos os lados; ataque e guarda consomem uma unidade.
- Comando inválido, salto de combatente morto, troca de rodada e emissão de eventos não gastam ação adicional.
- Limite configurado deve ser positivo. O resultado por esgotamento é escolhido explicitamente: empate, derrota de A ou derrota de B. Não há resultado por score implementado.
- Ação permitida resolve integralmente: vitória por eliminação na última ação tem precedência sobre esgotamento do limite.
- Após esgotar ou vencer, não há novo turno/rodada. Tentativas seguintes são rejeitadas sem alterar estado.
- Sem orçamento configurado, mantém-se o comportamento sem limite de COM-01. Segurança do executor de testes não é regra de jogo.

Detalhes de implementação e aceites: docs/batalhas/TIME-01.md. Os contratos de outras famílias permanecem em docs/batalhas/CONTRATOS.md; as perguntas ativas devem ser mantidas neste documento, evitando listas divergentes espalhadas pelo projeto.

## Atualização WAVE-01 — escopo resolvido

- PEN-01: resolvida para vida/derrotados, guarda e ordem fixa no perfil atual. PreserveHealth ou RestoreTeam obrigatórios, guarda limpa, ordem recalculada. Energia/efeitos/recargas/AP futuros continuam em aberto quando existirem.
- PEN-02: resolvida para fase consecutiva: contador global único, menor limite efetivo, precedência global em igualdade e vitória final por eliminação prioritária. Orçamento entre frentes segue para PAR-01.
- PEN-03/PEN-11: StageId/EncounterId/configVersion e IDs de combatente locais definidos; 1–5 confrontos neste perfil. Identidades de catálogo, múltiplos times e invocações permanecem futuras.
- PEN-14: reset e conclusão única em memória testados. Retomada por save, migração de versão e recompensa persistente não implementadas.
- VAL-01: validada para 1/2/5 confrontos com ataque/guarda, passagem de vida e orçamento global. Outros estados só serão validados quando implementados.

Evidência: docs/batalhas/WAVE-01.md e docs/relatorios/atuais/COMBATE.json (9/9 testes de fase). Este registro atualiza o estado das linhas acima sem descartar seu histórico de questões.

## Pesquisa de tempo — atualização de prioridades

TIME-02 agora precede SEQ-01. Evidências e experimento (11/11) em docs/batalhas/TEMPO-PESQUISA.md; o núcleo de combate permanece inalterado.

- PEN-07: usuário priorizou barra por velocidade/AP, com arquitetura capaz de outros perfis. Desempate, excedente, escala, alteração durante turno, fila de extras e pausas precisam ser explícitos; fórmula interna do Dislyte não comprovada. ATB não é simples troca de atributo.
- PEN-09: separar reação/assistência/perseguição de turno extra. Definir limites de cadeia, ID de origem e significado de uma vez por turno; não pressupor que qualquer ataque reduz duração/recarga.
- PEN-12: contador geral do modo segue sem regra exata comprovada. Resolver se conta oportunidades naturais/extras, bloqueios e quais lados; não equiparar automaticamente a rodada fixa ou ao orçamento atual de comandos aceitos.
- PEN-01/PEN-14: quando efeitos/recargas/AP entrarem no núcleo, ampliar passagem de estado e replay de fases; 9 testes de WAVE-01 cobrem somente os estados já existentes.
- Fronteiras a especificar no TIME-02: quando começa a recarga de habilidade recém-usada; duração aplicada ao próprio dono; oportunidade bloqueada por controle; extras com ou sem tick; morte durante gatilho; persistência da guarda até oportunidade extra/natural. São decisões de perfil, não uma regra universal inferida das fontes.

## TIME-02 — fundação implementada e lacunas restantes

- PEN-07: perfil ActionBar integrado em C++, com regra própria inteira de AP/SPD, desempate e fila FIFO de extras; FixedRounds preservado. ATB, pausa/menu e equivalência numérica ao Dislyte seguem abertos.
- PEN-09: contra-ataques derivados têm contador separado e origem; não reduzem seu relógio de dono. Proteções para fila/cadeia encerram com TechnicalLimit sem vencedor. Assistências/perseguições, prioridade entre passivas e limites por turno específicos ainda não existem.
- PEN-01/PEN-14: fases estendidas agora exigem RuntimeCarry Preserve/Reset. Status, recargas e velocidade efetiva são conservados ou reiniciados explicitamente; AP/guarda/filas/contadores locais reiniciam. Sem save ou retomada arbitrária. Contrato de habilidades preservadas precisa ser consistente entre ondas.
- PEN-12: existem contadores distintos de oportunidades naturais/extras, comandos principais, derivados e tempo lógico. O contador geral de cada modo permanece indefinido; Skip bloqueado conta para o orçamento atual, reações não. Isso é regra própria, não regra oficial comprovada.
- Durações/recargas mínimas do dono: NaturalOnly ou EveryTurn. Nova aplicação não perde duração no mesmo turno; habilidade usada não reduz a nova recarga nesse encerramento. Block consome oportunidade via Skip e reduz relógios correspondentes. Relógio do aplicador/global, congelamento por petrificação, modificadores externos de recarga e pilhas/dispels são futuras famílias.
- VAL-01 ampliada: fases AP 1/2/5, políticas de runtime, reset, fronteiras e replay testados. VAL-02/03/04/05/06 permanecem conforme suas fronteiras, sem novas alegações.

Evidências: docs/batalhas/TIME-02.md, tempo-testes.csv e docs/relatorios/atuais/COMBATE.json. 16 novos grupos e 25 formações AP; 33 testes e 168 cenários anteriores preservados. Próximo bloco SEQ-01, sem reabrir todas as pendências futuras.

## Roteiro para pesquisa do usuário

Perguntas consolidadas e modelo de devolutiva em docs/batalhas/ROTEIRO-PESQUISA-USUARIO.md (Q01–Q20), após TIME-02. O roteiro referencia os IDs deste registro e distingue comportamento próprio, evidência sobre Dislyte e preferência de projeto. Prioridades P1/P2/P3; perguntas não se tornam capacidades comprovadas por estarem documentadas. Aguardamos respostas parciais sem exigir pesquisa completa.


## Primeira devolutiva do usuário — revisão R01

Entrada preservada em docs/batalhas/pesquisa-usuario-recebida-R01.md; revisão em docs/batalhas/REVISAO-PESQUISA-USUARIO-R01.md. Vinte respostas recebidas, com afirmações sobre referência e preferências de projeto separadas. Não classificar tudo como comprovado: faltam URLs/trechos e escopo de várias fontes. Nenhuma regra do combate alterada.

Prioridades: PEN-07/PEN-09 (fronteira da recarga, flags independentes por fonte de extra, disparo/decremento/remoção de efeito); PEN-06/PEN-10 (conflito entre vitória antes de pós-morte e pipeline com revive/reação antes da vitória); PEN-12 (contagem no início versus conclusão); PEN-04 (estado completo do chefe ao trocar time). Guarda por habilidade inteira é proposta Q08, ainda não implementada. Propostas de ATB, score, retomada e metas de desempenho preservadas para seus blocos; não são novas capacidades validadas.

## Complemento dos quatro contratos — revisão R02

Preservado em docs/batalhas/complemento-usuario-recebido-R02.md; avaliação em REVISAO-PESQUISA-USUARIO-R02.md no mesmo diretório. Propostas esclarecem recarga 3 disponível no turno 4 e flags independentes por fonte de extra. Matriz do chefe distingue AP, recargas, escudos temporários/estruturais e permanentes. Sem fontes exatas novas e sem alteração do código.

PEN-06/PEN-10: ainda definir janela de reflexão/explosão pós-morte e papel de atacante no desempate de ambos os lados sem vivos; cadeia inclui revive antes de vitória, mas posição daqueles danos ficou indefinida. PEN-04: faltam buffs temporários do chefe, eventos pendentes, gatilhos de entrada/etapa e mapeamento de recarga ao mudar kit. PEN-07/PEN-09: congelamento de recarga frente a reduções externas e fronteiras de cada efeito continuam parciais. Consultar revisão R02 antes de propor novo bloco.

## Causalidade recebida — revisão R03

Registro em docs/batalhas/REVISAO-PESQUISA-USUARIO-R03.md e original preservado. As duas dúvidas principais de R02 foram esclarecidas como regras do nosso projeto: reflexão/explosão antes da vitória; em 0 × 0, lado do autor da ação atual (inclusive contra-ataque) tem prioridade, independentemente do lado ofensivo do modo. Auto-revive antecede checagem final; morte do autor por reflexão interrompe acertos restantes. Não reabrir a pergunta genérica sobre atacante.

PEN-06/PEN-09/PEN-10: futuras implementações precisam distinguir autor da ação, dono da oportunidade e emissor da consequência, com proteção de cadeia e nova checagem em ações derivadas. Microfronteira da explosão entre acertos ainda tem duas leituras no texto; resolver por caso de aceite no bloco correspondente. Não há reflexão/revive implementados nem testes novos nesta revisão. Timeout favorável ao defensor é política de modo, não substitui todos os perfis anteriores.

## R04 — Opção B de causalidade fechada pelo usuário

Contrato vigente em docs/batalhas/CONTRATO-CAUSALIDADE-R04.md. Substitui a microfronteira ainda aberta em R03: reflexão antes da drenagem de explosões; pendingDeaths FIFO drenada entre acertos, antes de auto-revive e Victory Lock. Morte do autor interrompe hits seguintes; bounce exclui mortos; 0 × 0 favorece o lado capturado de CurrentActionActor, inclusive em contra-ataque. Papéis ofensivo/defensivo do modo ficam separados.

PEN-06/PEN-09/PEN-10: decisão de ordem e desempate resolvida como contrato do nosso projeto; implementação e cinco invariantes continuam pendentes. Não solicitar novamente A/B nem a definição geral de atacante. Nenhuma regra de combate foi alterada e nenhum teste novo executado nesta atualização.

## Validação de prontidão R04

Revisão estática concluída em docs/batalhas/VALIDACAO-CONTRATOS-R04.md. Decisões prioritárias permitem iniciar TIME-03 sem nova rodada de esclarecimentos. Separar dúvida de design resolvida de implementação/teste pendentes. Sequência atualizada: TIME-03 → RES-01 → SEQ-01. Nenhuma nova execução ou alteração do combate nesta validação.


## TIME-03 parte 1 — recarga implementada

PEN-07/PEN-09: recarga 0/1/3 no encerramento do uso elegível implementada e testada em perfis fixo/AP. Comandos inválidos não consomem; replay e passagem de fase aprovados. Esta parte substitui o comportamento antigo de recarga recém-usada de TIME-02. Origem/flags dos extras e congelamento de recarga permanecem para partes 2/3; R04 para RES-01. Relatório docs/batalhas/TIME-03.md.


## TIME-03 parte 2 — fontes de extras implementadas

PEN-07/PEN-09: contexto e duas flags independentes por fonte explícita, FIFO, teto por fonte na cadeia natural e replay implementados/testados. Fontes legadas preservam relógios anteriores. Metadados reiniciam na troca/reset de confronto; limite técnico continua separado do teto da fonte. 60 testes totais aprovados; relatório TIME-03.md. Parte 3 ainda trata congelamento de recarga separado de Block; R04 permanece em RES-01.


## TIME-03 concluído — controle/congelamento e integração

PEN-07/PEN-09: Block e FreezeCooldowns independentes; congelamento domina permissão de recarga do extra e é verificado antes da expiração. Duração segue relógio próprio. PEN-01/PEN-14: 1/2/5 confrontos, Preserve/Reset, expiração e retomada de decremento aprovados. 67 testes totais, matrizes anteriores preservadas. Não há redução/reset externo de recarga nem passiva OnTurnStart; semântica sobre essas futuras operações continua fora deste aceite. Próximo RES-01 em entregas internas, sem reabrir decisões de tempo já encerradas.


## RES-01 parte 1 implementada

Identidade/parentesco de ações, multi-hit de alvo fixo e guarda pela instância ofensiva validados (76 testes totais). Q08 atendido no conjunto atual, sem assistências ainda. PEN-06/PEN-09/PEN-10 continuam para reflexão/FIFO e prevenção/revive/vitória; não marcar cinco invariantes R04 como integralmente testados. Dados e limites em docs/batalhas/RES-01.md. Próxima parte 2, sem reabrir identidade/guarda sem alteração pertinente.


## RES-01 parte 2 implementada

Reflexão antes das explosões, FIFO entre acertos, seleção dinâmica excluindo mortos, cancelamento de reações/extras de mortos e limite técnico validados (84 testes totais). Morte dupla pelo autor funciona sem revive; cinco invariantes R04 completos ainda aguardam prevenção/auto-revive/Victory Lock na parte 3. Stage NoSurvivors preserva distinção entre vencer confronto e concluir fase sem time vivo. Fórmula fixa/alcance do perfil em RES-01.md; não é paridade de dano ou bounce aleatório.


## RES-01 concluído — prevenção/revive/Victory Lock

PEN-06/PEN-09/PEN-10: cinco invariantes R04 testados no perfil mínimo original, com cargas finitas de prevenção e auto-revive do portador. Vitória/timeout depois das janelas obrigatórias; morte dupla favorece autor da ação apenas quando não restar sobrevivente após revive. Reações/extras cancelados e acertos interrompidos não voltam. PEN-01/PEN-14: cargas remanescentes conservadas e resetadas corretamente em fase. 95 testes totais aprovados; relatório RES-01.md. Não há revive de área aliado, assistência/perseguição ou RNG; novas famílias pertencem a seus blocos. Próximo SEQ-01, sem reabrir decisões R04 encerradas.


## SEQ-01 parte 1 — coordenador implementado

PEN-03/PEN-11: CharacterId, FighterId e SquadId separados; três times de 1–5, unicidade em toda inscrição validada. PEN-02/PEN-04: troca por derrota/limite local, HP persistente e política base de recarga testados, sem reconstruir max_health; kit ainda estável, status todos limpos no perfil explícito. PEN-06/PEN-10: resultado R04/auto-revive antecede transição, conclusão única em memória; sem score/economia. Próxima parte 2 define kit por etapa e categorias temporárias/permanentes/estruturais. 103 testes totais aprovados, detalhes SEQ-01.md.


## SEQ-01 parte 2 implementada

PEN-04: kit por etapa na troca de time, KeepMatching/ResetAll/MapExplicit e categorias Temporary/Permanent/Structural implementados. Escudo estrutural conserva capacidade; quebrado não reaparece; estado inicial só no começo/reset. 112 testes totais aprovados. Parte 3 valida combinações integradas. Ainda sem fases por HP, pilhas de atributo, IA completa ou score. Git do usuário não causou conflito; pushes continuam com ele.


## Atualização — SEQ-01 encerrado (18/11/2026)

A/A/B e A/A/A, cargas permanentes consumidas antes da troca, limite final, conclusão única e replay/reset validados. 17 grupos sequenciais e 115 testes totais. Coordenação entre frentes pertence a PAR-01/PAR-02; score, IA completa e limiares de HP permanecem fora desta entrega. Registros históricos não reabrem SEQ-01.


## PAR-01 parte 1 (18/11/2026)

Coordenação, resultado agregado e replay de três frentes independentes implementados. PEN-02 continua aberta para orçamento entre frentes: parte 2 tratará contador global, precedência e cancelamento. PAR-01 permanece parcial; chefe compartilhado pertence a PAR-02. Integração de falha técnica e habilidades/R04 reservada à parte 3.


## PAR-01 parte 2 — PEN-02 (18/11/2026)

PEN-02 resolvida no perfil cooperativo de frentes independentes: orçamento global único, políticas explícitas, limites locais independentes, precedência e cancelamento validados sem reset/triplicação. Não se estende a chefe compartilhado ou servidor. Parte 3 mantém integração pendente de R04/habilidades/Skip e falhas técnicas. 125 testes totais aprovados.


## PAR-01 encerrado — integração final (18/11/2026)

Integração pendente das partes 1/2 concluída: habilidades/R04, Skip, reações, extra cancelado e falha técnica coincidente com timeout. 16 grupos PAR-01, 131 testes totais aprovados. PEN-02 encerrada no perfil independente documentado. PEN-05 permanece aberta para escolha do modelo compartilhado antes de PAR-02; esta entrega não altera essa decisão nem valida servidor/threads/score.


## PAR-02 — proposta inicial

PEN-05 permanece aberta. Proposta docs/batalhas/PAR-02.md detalha três modelos e duas cadências. Perguntas enviadas ao usuário; recomendação de HP único/barra global não é decisão. Alcance de habilidades, relógios e retirada de frentes devem constar do contrato escolhido antes da implementação. Matriz de 12 casos planejada, não executada.


## PAR-02 parte 1 — escolha recebida e PEN-05 resolvida

Usuário escolheu HP único e barra global, ataques comuns alternados entre frentes vivas. SharedBoss básico implementado em um único Battle, composição/identidade/rotação/vitória/reset validados. 136 testes totais, 5 grupos novos. PEN-05 encerrada no perfil escolhido; PEN-06/08 e integração R04/overkill/estado compartilhado permanecem para partes 2/3. Registros anteriores de escolha pendente são históricos.


## Famílias PvE — registro sem perda de exemplos

PEN-10/12: relato Treva Fluxo acrescenta sobrevivência/janela e dano influindo em ganhos; unidade/fórmula/regras ainda desconhecidas. Exemplo dos Milagres precisa de chefe/modo exato e gatilho de troca/fase. PEN-04/06: fase por entrada de time não cobre fase autônoma por HP/turno; retorno cíclico não foi afirmado nem implementado. Catálogo MODOS-PVE.md e roteiro ROTEIRO-PVE-CHEFES.md. Nenhum desses relatos revoga PEN-05 resolvida ou torna todos os modelos implementados.


## PAR-02 parte 2 — escopos e consequências

PEN-08 parcialmente atendida: alvo direto único por próprio time, chefe ou frente da rotação; buffs entre times e skills comuns globais não implementados. PEN-06 validada em cenário de explosão global única, morte dupla e revive do chefe; fronteiras técnicas/overkill continuam parte 3. 142 testes/11 grupos compartilhados aprovados. Kit por HP/turno e pontuação seguem separados; não alegar todos os modos PvE cobertos.


## PAR-02 parte 3 — perfil encerrado

148 testes/17 grupos compartilhados aprovados; ordem de cadeias, overkill, reações, revive, falha técnica e timeout/replay validados. PEN-06/08 continuam parcialmente abertas para fases por HP/turno, área e eventos entre times, não abrangidos pelo perfil final. PEN-10/12 mantêm score/sobrevivência e contadores externos desconhecidos. Regra técnica distingue falha já detectada de limite de oportunidade futura: não gera erro após timeout impedir agendamento. Próximo COM-02; catálogo futuro preservado.


## COM-02 — interação textual concluída

Entrada de ações/alvos e reinício de Battle disponíveis e validados. Não é save/checkpoint nem menu de todos os modos; PEN-14 mantém persistência futura. 155 testes totais, sete de apresentação; núcleo inalterado. Próximo HAB-01 mantém famílias PvE por score/fases separadas.


## HAB-01 — dúvidas por etapa

HAB-01.md concentra decisões próximas: posições e básica sem bypass; primeiro gatilho de passiva, dono da janela/limite de proc; condições de ultimate e seleção automática. Recursos universais, prioridade A3>A2>A1 e regras atuais de todas as passivas do Dislyte não estão confirmados. Não perguntar tudo de uma vez nem reabrir R04; pedir amostras atuais de poucos personagens quando a etapa depender delas. Perfil clássico preservado; implementação dos perfis ainda pendente.


## HAB-01 — relatório de habilidades R01

Revisão em REVISAO-HABILIDADES-USUARIO-R01.md. Separar posição passiva de recarga interna, decrementar naturalmente de alterar CD externamente, dono do limite de proc e ficha-base de progressão. Algumas fichas declaradas incompletas/divergentes; fonte pontual necessária antes de paridade. Não confundir AutoRevive próprio com revive de aliados ou Counter genérico com cast de habilidade. Próximo parte 1; sem nova pesquisa ampla.


## Contrato de equipes R01 — controles separados

Básica A1 não silenciável e sempre disponível quando unidade apta/alvo legal; decisão fechada. Recarga numérica, inatividade por silêncio e condição/cadeado são estados distintos e podem coexistir. Matriz supressão de passivas x silêncio/congelamento/petrificação não fechada: não tratar hipótese como fato. Definir também tick de CD interno e efeito sobre buffs existentes. FreezeCooldowns atual não congela o personagem. Clássico com itens/estamina/especial fica para contrato próprio; sem inventário ou quarta posição no padrão.


## Defesa clássica — extensões futuras

Arredondamento padrão para baixo decidido e implementado. Modificadores de dano recebido/defendido, perfuração, itens/atributos e ordem/limites futuros do clássico são possibilidades a especificar antes de codificar. Não se aplicam automaticamente ao padrão de equipes sem defesa genérica. DEFESA-PADRAO.md; regressão atual155 aprovada.

## HAB-01 após parte 1

Perfis/posições concluídos (162/162 testes). Próxima pendência: contrato e implementação do primeiro gatilho passivo; definir janela, dono do limite, vivos/bloqueados/revive e integração R04. Silêncio/supressão e condições especiais seguem futuros; não declarar esses efeitos testados.

## HAB-01 após parte 2 — estado atual

Resolvida a pendência de primeiro gatilho: BattleStart/escudo próprio com limite de uma ativação por combate. Gatilhos por impacto/morte, supressão, recarga interna e transporte entre coordenadores permanecem futuros. Próximo incremento parte 3: contrato de condição de habilidade; não reabrir R04 nem ampliar o catálogo implicitamente.

## HAB-01 após parte 3

Primeira condição adicional resolvida: vida do dono <= percentual, sem consumo. Contrato HAB-01-CONDICAO-VIDA-R01.md, 176/176 testes. Próximo parte 4: escolher política mínima de prioridades/fallback para especiais sob a mesma validação. Recursos/condições compostas e supressão seguem futuros.

## HAB-01 após parte 4

Política mínima de automação resolvida: prioridades configuradas, disponibilidade comum e básica como alternativa, com alvos determinísticos. 183/183 testes. Próximo parte 5: integração/regressão do pequeno conjunto e limites. IA estratégica, toggle/editor do time A e condições/custos adicionais permanecem futuros.

## HAB-01 encerrado — estado atual

Cinco partes do conjunto mínimo concluídas, 187/187 testes. Híbrida funcional integrada. Pendências futuras de silêncio/supressão, gatilhos durante combate, recarga interna, recursos/requisitos compostos e adaptação SEQ/PAR não foram declaradas implementadas. Próximo CIC-01 parte 1: contrato e composição/seleção em memória; recompensa e save em partes posteriores.

## CIC-01 parte 1 concluída

Composição 1–5 validada em memória e integrada por --equipe; 195/195 testes. Próxima pendência: identidade de conclusão e concessão única da recompensa mínima. Formato de save/migração permanece parte 3; seleção não transporta HP/status de batalhas anteriores.

## CIC-01 parte 2 concluída

Identidade de tentativa e apuração única de recompensa em memória resolvidas, 203/203 testes. Próximo contrato: save versionado, escopo persistido, validação/restauração e como manter concessão única após carregar. Não assumir que o snapshot do kernel ou IDs locais já resolvem persistência.

## CIC-01 encerrado no escopo mínimo

Save v1 de equipe/progresso entre batalhas validado, 211/211 testes; sem nova concessão ao carregar. Save de runtime, versões futuras/migrações, backup, concorrência, autenticação e economia real permanecem extensões. Próximo ENG-01: avaliar compatibilidade antes de escolher/instalar engine.
