# SEQ-01 — Três times sucessivos e chefe por etapa

18/11/2026. **Primeira entrega concluída e validada: composição, identidade e coordenador de troca.** O bloco continua aberto para perfis/etapas do chefe e tratamento de seus estados por categoria.

Implementação original em include/rpg/sequential.hpp e src/sequential.cpp; testes em tests/sequential_tests.cpp. Stage continua responsável por um time contra ondas de inimigos. SequentialSquads é o fluxo inverso: três times distintos contra o mesmo chefe persistente. Não confundir com frentes simultâneas.

## Registro e identidade

O perfil exige exatamente três SquadDefinition, cada um com 1–5 RegisteredFighter. Cada registro separa CharacterId (personagem do catálogo) de Definition.id (instância de combate); SquadId e StageId têm escopos próprios. Personagens e instâncias são únicos em toda a inscrição; não repetir entre times nem colidir com o ID do chefe. Os IDs são sensíveis a maiúsculas/minúsculas.

Todos os times, habilidades, orçamento e atributos são validados antes do início, inclusive o terceiro time. Membros pertencem a A e o chefe a B. O perfil não impõe esses limites ao kernel Battle genérico nem cria reservas/invocações.

SequentialActionResult identifica o time que executou o comando, mesmo quando a resposta também inicia o seguinte. IDs locais de ações reiniciam no novo Battle; usar StageId + SquadId/índice como escopo. Snapshot inclui versão de configuração, time ativo, contador global, política de passagem e Battle. Ainda não é formato de save.

## Ordem e condições de passagem

- A ordem de entrada é a ordem declarada dos três times, sem saltos.
- Derrota/eliminação do time ou encerramento por limite local passa ao próximo, enquanto houver chefe vivo, time restante e orçamento global.
- Vitória antecipada ao derrotar o chefe encerra a tentativa sem ativar os demais times.
- Esgotar o terceiro time com chefe ainda vivo encerra SquadsExhausted. Vitória/empate por política de limite local não equivalem a derrotar o chefe.
- Esgotar orçamento global encerra GlobalActionLimit, sem iniciar outro time; resultado segue a política global configurada. Eliminação real do chefe na última ação permitida tem precedência.
- TechnicalLimit encerra sem vencedor e não é convertido em troca de time normal.
- Na morte dupla em que o autor era o chefe, a vitória de B decidida pelo R04 é mantida como MutualElimination. Não conceder vitória de A simplesmente porque o HP do chefe chegou a zero.

stage_finished é sinalizado só na resposta que encerra a tentativa. Comandos posteriores são rejeitados sem repetir conclusão ou alterar contadores. Não há crédito de recompensa/economia implementado.

## Persistência implementada nesta primeira entrega

O HP atual do chefe passa integralmente para o confronto seguinte, sem alterar max_health nem restaurar por reconstrução. A fonte autoritativa é o Fighter do Battle atual, copiado para entrada validada ao trocar; não há três barras independentes somadas depois.

BossRuntimeCarry é obrigatório:

| Política base | HP | Recargas do chefe | Status do chefe |
|---|---|---|---|
| KeepCooldownsClearStatuses | Conserva | Conserva | Limpa todos |
| ResetRuntime | Conserva | Reinicia | Limpa todos |

Ambas reiniciam AP, guarda, velocidade efetiva para o atributo-base, filas, IDs e contadores locais. A seleção natural inicial pode avançar as barras até o primeiro pronto: não interpretar o snapshot inicial como todas as barras ainda literalmente em zero.

Os integrantes do novo time entram com seus próprios atributos máximos, vida cheia, habilidades prontas e sem estados/filas herdados do time anterior. A fase mantém um contador global único de comandos aceitos; cada confronto possui seu contador local.

O contrato desta parte deliberadamente aceita **um kit estável de chefe** e um perfil de tempo consistente entre os três confrontos. Habilidades próprias de cada time podem diferir. Mudanças do kit por etapa serão adicionadas na próxima entrega, com mapeamento explícito de recargas.

Não existem ainda escudos estruturais, pilhas permanentes ou classificação de buffs/debuffs do chefe. “Limpa todos” é o perfil base explícito desta API, não a política final de preservar estados estruturais recebida na pesquisa. Não declarar essa matriz final implementada antes da etapa correspondente.

## Integração com R04 e reinício

O coordenador lê o resultado somente depois que Battle conclui suas consequências e auto-revives. Um chefe que morreu e reviveu não conta como derrotado; seu HP retornado e saldo de recarga são passados corretamente quando o limite local troca o time naquela mesma resposta.

reset() inicia a tentativa do começo: time 1, chefe com HP máximo, estado inicial, contadores e resultado limpos. Não reiniciar a tentativa implicitamente numa troca de time. Instâncias separadas não compartilham estado mutável.

## Verificação efetiva

scripts/testar-combate.ps1 compilou em MSVC /W4 /WX sem avisos na execução final. **103 testes aprovados:** 13 básicos + 11 limites + 9 fases + 34 tempo + 28 resolução + 8 grupos sequenciais. Mantidos 168 cenários/replays, 25 formações/replays AP, isolamento intercalado, lote funcional e demo 40 ações/14 rodadas.

Grupos novos: composição válida 1–5 e rejeições antecipadas; derrotas 1→2→3 com HP conservado; vitória antes do terceiro time; limites local/global e última ação; recargas/limpeza do chefe por política; novo time sem vazamento; revive e morte dupla R04 antes da transição; erro técnico/reset/replay e isolamento entre coordenadores.

Dois roteiros de teste inicialmente pressupunham duas ações consecutivas do chefe com SPD insuficiente; os dados foram ajustados para essa sequência sem alterar o scheduler. A execução final aprovou os oito grupos.

Evidências: ../../COMBATE.json, COMBATE-SEQ-01-PARTE-1.json e sequencia-testes.csv. Evidências finais de TIME-03/RES-01 preservadas. Não repetir testes após atualização exclusivamente documental.

## Próxima entrega

Perfis de habilidades do chefe por etapa e políticas de passagem de estado por categoria, mantendo HP/identidade da tentativa. Especificar a fronteira de troca, habilidades que conservam/resetam recarga e estados temporários versus permanentes/estruturais antes de expandir. Usar dados originais e o conjunto de efeitos já validado. Depois validar o fluxo integrado de etapas, limites, conclusão única e replay. SEQ-01 continua aberto; PAR-01/02, interface, engine e rede não foram iniciados.
