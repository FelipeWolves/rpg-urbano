# PAR-01 — Três frentes independentes

18/11/2026. **Parte 1 concluída; bloco ainda aberto.**

## Divisão e aceite

1. Coordenador determinístico, identidade, isolamento, resultados locais/agregado, alternância e replay/reset — concluído.
2. Orçamento global autoritativo, precedência com limites locais e cancelamento coordenado — pendente.
3. Integração com habilidades/R04, limites técnicos e término único nas fronteiras — pendente.

## Contrato da parte 1

ParallelFronts recebe exatamente três FrontDefinition e uma permutação de índices. Cada frente contém um Battle independente; o construtor valida todas as definições, inclusive a terceira. Identificador de fase e versão são obrigatórios, e IDs das frentes são únicos. IDs locais de personagens podem se repetir entre frentes: não representam a mesma instância nem HP compartilhado. Os limites genéricos de composição continuam os de Battle, sem regra nova de inscrição de personagens nesta entrega.

O escalonador oferece uma ação principal aceita por frente em cada ciclo, na ordem configurada; ignora frentes encerradas. Reações e consequências de uma ação resolvem integralmente no Battle antes de ceder a vez. Comando rejeitado não avança cursor nem contador. A garantia de alternância vale para comandos aceitos: um chamador que insiste em comando inválido precisa corrigi-lo. Não há relógio físico, execução simultânea, threads ou rede.

Cada resposta identifica StageId, FrontId e StageActionId global monotônico por comando aceito. Eventos internos mantêm suas identidades locais de Battle; para consumo externo usar o escopo fase + frente + ação. Snapshot captura as três batalhas, ordem, frente agendada, contador e conclusão. Views são constantes; não se modifica uma frente por fora do coordenador.

Nesta parte todas as frentes terminam antes da conclusão agregada; não há encerramento antecipado. Todas vencidas por A: vitória de A. Qualquer vitória de B: vitória de B. Sem vitória de B, mas com empate local: empate agregado. Qualquer limite técnico: resultado agregado técnico sem vencedor, com precedência sobre resultados competitivos. Esse último caminho receberá integração dedicada na parte 3. Não existe score implementado.

Limites locais já funcionam por Battle. Orçamento global e cancelamento ainda não existem nesta parte. Reset reconstrói todos os contextos e restaura a ordem/contadores; snapshots não são formato de save.

## Verificação efetiva

Cinco grupos novos aprovados:

- configuração inválida, inclusive roster da terceira frente e permutação incorreta;
- rejeição atômica e isolamento apesar de IDs locais repetidos;
- alternância, retirada de frentes encerradas, IDs globais e conclusão única;
- duas ordens de escalonamento, replay completo e reset sem vazamento;
- vitória agregada, derrota por uma frente e empate por limite local.

MSVC /W4 /WX aprovado. Total: 120 testes (115 anteriores + 5 de frentes), 168 cenários/replays, 25 formações AP/replays, isolamento anterior, 1000 simulações funcionais e demo preservados. Evidências: COMBATE-PAR-01-PARTE-1.json e frentes-testes.csv. Não é prova de paralelismo real, servidor, fairness temporal ou chefe compartilhado.

Próxima entrega: parte 2, orçamento global e cancelamento. PAR-02 permanece separado, pois clones independentes não constituem chefe compartilhado.
