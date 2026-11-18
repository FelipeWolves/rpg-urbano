# Validação de prontidão após R04

18/11/2026. Escopo: revisão estática do contrato R04, das atualizações R02/R03, dos pontos correspondentes do código e do resultado anterior COMBATE.json. Sem execução de testes, alteração do combate ou verificação externa das regras do Dislyte.

## Conclusão

As decisões prioritárias de recarga, flags independentes de extras, posição inline da explosão e autor do desempate permitem avançar para implementação em blocos. Não é necessário repetir essas perguntas. A validação comportamental das regras novas ainda depende de código e testes novos.

O R04 apresenta ordem executável para o escopo fechado: dano/prevenção, reflexão, drenagem FIFO de mortes, interrupção de acertos, auto-revive, resultado e reações. O autor da ação permanece separado do emissor das consequências e do papel ofensivo do modo. A janela de vitória deixa de depender de checagem imediatamente após cada dano.

## Contrato versus implementação

| Regra recebida | Situação atual observada | Trabalho necessário |
|---|---|---|
| Recarga reduz no encerramento do próprio uso | end_turn exclui a habilidade recém-usada | Adaptar regra e testar recargas 0/1/3 |
| Extra carrega flags independentes de recarga/duração | Fila guarda apenas índice do combatente; relógios por habilidade/status | Criar metadados da oportunidade/fonte e verificar passagem de fase |
| Petrificação bloqueia decremento, controle simples não | Existe somente Block simples | Distinguir bloqueio de ação de congelamento do decremento |
| Reflexão e mortes FIFO entre acertos | hit encerra eliminação diretamente; não há reflexão/explosão | Separar dano, morte e conclusão; introduzir fila limitada de consequências |
| Auto-revive antes do resultado | Ausente | Implementar janela de revive e validar que morte não conclui prematuramente |
| 0 × 0 favorece autor da ação atual | Caso não existe no perfil atual | Capturar lado do autor, inclusive em contra-ataque |
| Guarda cobre habilidade ofensiva inteira | Consome no primeiro dano | Proteger por instância de habilidade e verificar multi-hit |
| Troca de três times preserva estado selecionado do chefe | Stage atual troca inimigos e conserva o time A | Novo coordenador, não simples reutilização da passagem entre ondas |

Os 49 testes, 168 cenários fixos e 25 formações AP são evidência anterior de TIME-02. Nenhum desses números constitui teste de reflexão/revive que ainda não existem. Reexecutá-los sem alteração não validaria R04.

## Sequência priorizada

1. **TIME-03 — Ajustar recarga e origem de extras.** Preservar separação entre turno, ação e reação; introduzir flags por oportunidade e congelamento do decremento. Testar 0/1/3, extras de origens diferentes, controle, invalidade sem consumo, limites, replay e passagem entre confrontos. Não expandir para catálogo de equipamentos/passivas.
2. **RES-01 — Resolução causal mínima R04.** Implementar conjunto original mínimo de multi-hit, reflexão, explosão FIFO, prevenção/auto-revive e guarda por habilidade. Testar os cinco invariantes e os três casos de morte dupla, além da prioridade frente ao timeout e proteção de cadeia. Fórmulas de dano desses efeitos devem ser explícitas nos dados de teste; não alegar fidelidade a fórmulas não pesquisadas.
3. **SEQ-01 — Três times e chefe por etapa.** Usar a fundação já validada, com política explícita de persistência/limites e habilidades originais mínimas. Detalhes de categorias de estado do chefe são tratados no bloco, sem reabrir as decisões já fechadas.

Essa divisão atualiza a ordem antiga de TAREFAS.md/ESTADO.md para evitar começar SEQ-01 antes de incorporar os ajustes prioritários recebidos do usuário. ATB, servidor, gráfico, score e outras famílias continuam fora desses blocos.

## Critério de encerramento

Cada bloco deve entregar código, testes dos comportamentos novos, regressão pertinente e relatório incremental. A validação atual está concluída como revisão documental e comparação estática; não como aprovação de funcionalidades ainda ausentes. Nenhuma nova pesquisa ou resposta do usuário é necessária para iniciar TIME-03 no escopo acima.
