# SEQ-01 — Três times sucessivos e chefe por etapa

18/11/2026. **Três partes concluídas e validadas: coordenador, perfis, estados classificados e integração final.** As seções anteriores registram o histórico da primeira entrega; a atualização ao final apresenta o contrato vigente.

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

## Próxima entrega prevista à época — histórico

Perfis de habilidades do chefe por etapa e políticas de passagem de estado por categoria, mantendo HP/identidade da tentativa. Especificar a fronteira de troca, habilidades que conservam/resetam recarga e estados temporários versus permanentes/estruturais antes de expandir. Usar dados originais e o conjunto de efeitos já validado. Depois validar o fluxo integrado de etapas, limites, conclusão única e replay. SEQ-01 continua aberto; PAR-01/02, interface, engine e rede não foram iniciados.

## Parte 2 — Perfis e estados classificados concluídos

Esta atualização substitui a restrição anterior de kit estável/limpeza total. O modo legado permanece disponível e testado. O modo novo exige PreserveClassifiedStates e três BossPhaseDefinition, alinhados aos três times.

### Perfil e fronteira

Cada perfil possui ID único e habilidades exclusivas do chefe. No modo por perfis, as regras de cada time declaram somente habilidades de jogadores, evitando duas fontes para o kit do chefe. Toda a configuração é validada antes do início.

A etapa muda somente na troca de time, depois de todas as consequências/revives do comando. Não existe ainda mudança por limiar de HP dentro do confronto. Habilidade retirada torna-se indisponível; nova habilidade entra conforme sua recarga. SequentialActionResult registra boss_phase_id de origem, boss_phase_changed e next_boss_phase_id; snapshot informa a etapa ativa. HP e identidade do chefe permanecem.

A política automática básica ainda escolhe ataque/Skip. Os testes invocam habilidades explicitamente; disponibilizar kits não implementa uma IA completa de chefe.

### Recargas

- KeepMatching conserva saldo de habilidades que mantêm o mesmo ID; novas começam prontas e retiradas desaparecem.
- ResetAll inicia todas prontas.
- MapExplicit transfere saldo por pares from_skill → to_skill, um para um, com referências reais dos perfis anterior/novo. Habilidades sem mapeamento começam prontas. A primeira entrada não admite mapa de um perfil anterior inexistente.

A política é obrigatória por perfil; mapeamentos fora da política explícita são rejeitados. O saldo transferido é exato, mesmo se a base da habilidade nova mudar. Não há clamp proporcional implícito.

### Categorias e persistência

StatusCategory é explícita, sem dedução pelo nome:

| Categoria | Duração | Na troca classificada |
|---|---|---|
| Temporary | 1–1.000.000, conforme relógio/fonte | Limpa |
| Permanent | remaining = 0; não expira por turno | Conserva |
| Structural | remaining = 0; não expira por turno | Conserva |

Conservar inclui cargas e magnitudes restantes. boss_initial_statuses aplica somente no começo da tentativa/reset; não é reaplicado na mudança de time. Estados consumidos e escudos quebrados não reaparecem.

AP, guarda, filas e velocidade efetiva reiniciam conforme a regra anterior. SetSpeed é alteração direta, não um buff permanente de atributo. Categorias não implementam, por si só, pilhas acumuláveis nem modificadores de atributos.

Modos legados KeepCooldownsClearStatuses/ResetRuntime continuam limpando tudo como declaravam; não houve mudança silenciosa das configurações anteriores.

### Escudos funcionais

Shield é camada temporária; StructuralShield é estrutural. São independentes e podem coexistir, uma instância de cada tipo, com capacidade positiva. Reaplicação explícita substitui a mesma camada; não há pilhas ilimitadas.

Ordem: guarda quando aplicável → Shield → StructuralShield → HP → prevenção/morte. Escudos também absorvem reflexão/explosão, que continuam sem usar guarda ou gerar novas reflexões/Counter entre si. O perfil de Reflect continua disparando pelo acerto mesmo com dano direto totalmente absorvido.

ShieldAbsorbed/ShieldBroken identificam a camada em status_kind. Evento normal de dano registra somente a perda efetiva de HP. Camada zerada é removida; a soma das capacidades não é calculada previamente, evitando overflow inclusive com INT_MAX em cada camada. Escudo temporário pode expirar; estrutural permanece até consumo/remoção explícita.

O valor estrutural restante passa às etapas seguintes. Isso é uma regra original, não reprodução de fórmula de Life Shield de outro jogo.

### Validação

Executado scripts/testar-combate.ps1, MSVC /W4 /WX sem avisos: **112 aprovados** (13 básicos, 11 limites, 9 fases, 34 tempo, 31 resolução, 14 sequenciais). Mantidos 168 cenários/replays e 25 formações/replays AP, isolamento, lote funcional e demo 40 ações/14 rodadas.

Nove grupos novos: camadas/guarda e extremos inteiros; escudo antes de prevenção/revive; categorias/expiração; kit na fronteira; mapa de recargas; permanentes/escudo parcial/quebrado; configuração inválida; replay/reset; timeout/vitória sem troca indevida. A regressão foi pertinente pela mudança do núcleo de dano/estados.

Evidências: COMBATE-SEQ-01-PARTE-2.json, ../../COMBATE.json, resolucao-testes.csv (31 grupos) e sequencia-testes.csv (14). Parte 1 preservada. Não repetir após alterações somente documentais.

### Próxima entrega prevista à época — histórico prevista à época — histórico

Parte 3: validar combinações de três perfis, R04, orçamento local/global, cargas/escudos, conclusão única e replay, acrescentando somente lacunas relevantes. Não expandir para score/economia, fases por HP, RNG ou IA completa nessa validação. Encerrar SEQ-01 depois desse aceite.

## Parte 3 — Integração final (18/11/2026)

Três grupos novos, sem novas mecânicas no núcleo:

- `mixed_stable_kits`: A → A → B e A → A → A. A etapa muda mesmo com kit igual; recargas compartilhadas conservam saldo 4/3. Habilidade aposentada é rejeitada sem mutação; não existe quarta etapa; reset reproduz instância nova.
- `permanent_revive_phases`: duas cargas permanentes são consumidas antes das duas trocas, sem reposição. Terceiro golpe encerra por derrota do chefe; reset restaura o estado inicial.
- `integrated_final_budget_replay`: duas instâncias reproduzem quatro comandos e duas trocas; limite global encerra na terceira etapa uma única vez. Comando posterior não altera estado; reset não vaza entre instâncias.

17/17 grupos sequenciais e 115/115 testes totais aprovados em MSVC /W4 /WX. Preservados 168 cenários/replays, 25 formações AP/replays e 1000 batalhas funcionais. Evidência consolidada: COMBATE-SEQ-01-FINAL.json; arquivos anteriores preservados. Regressão de R04, escudos parciais, mapas de recarga e vitória na última ação aprovada.

SEQ-01 encerrado no contrato documentado. Não exige novos testes/revisão agora, salvo mudança relevante. Não valida paralelismo real, chefe compartilhado, servidor, IA completa, score ou catálogo externo de habilidades. Conclusão única não equivale a pontuação implementada. Próximo: PAR-01.
