# PAR-01 — Três frentes independentes

18/11/2026. **Partes 1/2 concluídas; bloco ainda aberto para integração final.**

## Divisão e aceite

1. Coordenador determinístico, identidade, isolamento, resultados locais/agregado, alternância e replay/reset — concluído.
2. Orçamento global autoritativo, precedência com limites locais e cancelamento coordenado — concluído.
3. Integração com habilidades/R04, limites técnicos e término único nas fronteiras — pendente.

## Contrato da parte 1 — histórico, complementado pela parte 2

ParallelFronts recebe exatamente três FrontDefinition e uma permutação de índices. Cada frente contém um Battle independente; o construtor valida todas as definições, inclusive a terceira. Identificador de fase e versão são obrigatórios, e IDs das frentes são únicos. IDs locais de personagens podem se repetir entre frentes: não representam a mesma instância nem HP compartilhado. Os limites genéricos de composição continuam os de Battle, sem regra nova de inscrição de personagens nesta entrega.

O escalonador oferece uma ação principal aceita por frente em cada ciclo, na ordem configurada; ignora frentes encerradas. Reações e consequências de uma ação resolvem integralmente no Battle antes de ceder a vez. Comando rejeitado não avança cursor nem contador. A garantia de alternância vale para comandos aceitos: um chamador que insiste em comando inválido precisa corrigi-lo. Não há relógio físico, execução simultânea, threads ou rede.

Cada resposta identifica StageId, FrontId e StageActionId global monotônico por comando aceito. Eventos internos mantêm suas identidades locais de Battle; para consumo externo usar o escopo fase + frente + ação. Snapshot captura as três batalhas, ordem, frente agendada, contador e conclusão. Views são constantes; não se modifica uma frente por fora do coordenador.

Nesta parte todas as frentes terminam antes da conclusão agregada; não há encerramento antecipado. Todas vencidas por A: vitória de A. Qualquer vitória de B: vitória de B. Sem vitória de B, mas com empate local: empate agregado. Qualquer limite técnico: resultado agregado técnico sem vencedor, com precedência sobre resultados competitivos. Esse último caminho receberá integração dedicada na parte 3. Não existe score implementado.

Limites locais já funcionam por Battle. Orçamento global e cancelamento ainda não existem nesta parte. Reset reconstrói todos os contextos e restaura a ordem/contadores; snapshots não são formato de save.

## Verificação da parte 1

Cinco grupos novos aprovados:

- configuração inválida, inclusive roster da terceira frente e permutação incorreta;
- rejeição atômica e isolamento apesar de IDs locais repetidos;
- alternância, retirada de frentes encerradas, IDs globais e conclusão única;
- duas ordens de escalonamento, replay completo e reset sem vazamento;
- vitória agregada, derrota por uma frente e empate por limite local.

MSVC /W4 /WX aprovado. Total: 120 testes (115 anteriores + 5 de frentes), 168 cenários/replays, 25 formações AP/replays, isolamento anterior, 1000 simulações funcionais e demo preservados. Evidências: COMBATE-PAR-01-PARTE-1.json e frentes-testes.csv. Não é prova de paralelismo real, servidor, fairness temporal ou chefe compartilhado.

Próxima entrega prevista à época: parte 2, concluída abaixo. PAR-02 permanece separado, pois clones independentes não constituem chefe compartilhado.

## Parte 2 — Orçamento global e cancelamento

18/11/2026. Contador global único por fase; `global_budget` é opcional, com teto positivo e política explícita Draw/DefeatA/DefeatB. `remaining_actions()` fornece saldo; snapshot registra configuração e estados de frente. Sem orçamento o comportamento normal da parte 1 permanece.

Cada comando principal aceito, inclusive Skip, consome exatamente uma unidade global e uma unidade no contador local da frente. Comandos inválidos não consomem nem avançam. Reações, reflexão, explosões e demais consequências da ação resolvem dentro do Battle antes da decisão global; não consomem unidades globais adicionais. O limite global não é inserido nem reiniciado em cada Battle. Ao atingir o teto, nenhum próximo comando é aceito.

Precedência após a cadeia aceita:

1. Falha técnica de qualquer frente: encerra a fase sem vencedor e cancela as demais frentes abertas. A integração dedicada de falhas/habilidades é da parte 3.
2. Todas as frentes encerradas por eliminação: resultado agregado normal, mesmo na última ação global.
3. Teto global esgotado: aplica política global, inclusive quando coincide com encerramento local por limite. Uma vitória local isolada não constitui vitória da fase.
4. Sem teto esgotado, limites locais encerram só a frente; as demais continuam alternando. Quando todas terminam, aplica-se agregado normal da parte 1.

Draw global não concede vencedor; DefeatA concede B; DefeatB concede A. O limite global pode determinar resultado distinto dos resultados locais previamente obtidos: trata-se de regra configurada do modo, não reescrita dos resultados de Battle.

`FrontState` separa Running, Finished e Cancelled no coordenador. Encerrar a fase preserva Finished e marca apenas Running como Cancelled. As batalhas canceladas conservam HP, AP, status, eventos e contadores; podem continuar com Phase::Running internamente como evidência do ponto de interrupção, mas não são mais agendadas nem recebem comandos pelo coordenador. Consumidores devem usar também o estado do coordenador. Não se fabrica vitória/derrota local para uma frente cancelada.

`cancel()` permite interrupção explícita entre comandos, sem vencedor, sem gastar orçamento e sem alterar os snapshots das batalhas. Retorna false após o fim; repetir não muda o resultado. No encerramento automático, a resposta identifica `cancelled_fronts`; no cancelamento explícito, consultar o snapshot. Reset restaura todas as frentes, orçamento e agendamento. O API é cooperativo, não seguro para chamadas concorrentes e não interrompe uma ação no meio.

## Verificação da parte 2

Cinco grupos novos, dez grupos PAR-01 acumulados:

- configuração do orçamento, três políticas, rejeição sem consumo e imutabilidade após timeout;
- limite local sem terminar a fase, saldo compartilhado entre frentes e prioridade global na coincidência com três limites locais;
- eliminação da fase na última ação prevalece; uma eliminação local isolada não prevalece;
- cancelamento explícito idempotente, conservação dos resultados já encerrados e reset;
- replay comando a comando, conclusão única, teto de cinco ações e reset isolado em duas ordens.

**125/125 testes aprovados**, incluindo 10/10 grupos PAR-01, em MSVC /W4 /WX. Preservados 168 cenários/replays, 25 formações AP/replays, 1000 simulações funcionais e demo. Evidências: COMBATE-PAR-01-PARTE-2.json, COMBATE.json e frentes-testes.csv. Arquivos anteriores preservados.

Próxima entrega: parte 3, integração com habilidades/R04, Skip, falha técnica, limites e cancelamento nas fronteiras. Não marcar PAR-01 inteiro concluído ainda; sem threads, chefe compartilhado, servidor ou pontuação.
