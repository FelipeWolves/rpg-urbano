# TIME-01 — orçamento local de ações

Concluído em 18/11/2026. Núcleo C++20 próprio, sem engine ou rede. Questões futuras estão centralizadas em ../../PENDENCIAS.md.

## Comportamento entregue

`Battle` aceita `BattleRules` com um `ActionBudget` opcional. A configuração de limite exige quantidade positiva e resultado explícito: `LimitOutcome::Draw`, `DefeatA` ou `DefeatB`. Não configurar orçamento mantém o comportamento anterior sem limite.

```cpp
rpg::Battle battle(rpg::reference_roster(),
    rpg::BattleRules{rpg::ActionBudget{35, rpg::LimitOutcome::Draw}});
```

Unidade: uma ação principal aceita, de qualquer um dos lados, incluindo guarda. O contador aumenta uma vez depois da validação do comando, antes da aplicação. Comando inválido não altera estado/contador, e eventos de guarda consumida, derrota, fim de rodada ou turno não contam como nova ação. Combatente morto saltado não gasta orçamento.

Na última ação permitida, resolve-se a ação e a eliminação antes do limite. Se houve vitória, `EndReason::Elimination` prevalece; caso contrário, encerra-se com `EndReason::ActionLimit` e o resultado configurado. Não avançar rodada/turno nem expirar a guarda do próximo ator após o encerramento. Guarda pode permanecer no snapshot final, pois não houve próximo turno.

`Phase::Finished` significa encerrada, não necessariamente vitória por eliminação. Empate possui winner vazio; uma derrota por limite pode ter vencedor mesmo com ambos os lados vivos. Consumidores devem consultar end_reason() e winner(), não inferir resultado apenas pela presença de sobreviventes. Resultados por score ficam pendentes.

## Consulta e eventos

- `accepted_actions()`: quantidade de ações principais aceitas.
- `remaining_actions()`: saldo do orçamento ou nullopt se não configurado. Um término antecipado por eliminação pode manter saldo, mas não permite novas ações.
- `end_reason()`: nullopt durante execução; eliminação ou limite após terminar.
- `snapshot()`: inclui regras, contador e motivo; comparações de rejeição verificam também esses campos.
- Evento `BattleFinished`: inclui vencedor opcional e motivo de encerramento. Emitido uma única vez.

Limite armazenado como inteiro assinado de 64 bits para rejeitar zero/negativos; contador de aceitas/saldo usa uint64_t. A validação é no construtor de Battle. Não há API para alterar as regras no meio de uma batalha.

## Verificação realizada

Comando: `.\scripts\testar-combate.ps1` na raiz. Compilação /W4 /WX sem avisos/erros. **11/11 testes novos**, além dos 13 originais, 168 cenários/replays e lote funcional existentes aprovados. A suíte ampla foi repetida porque o estado e o encerramento do núcleo mudaram.

Casos novos:

1. Rejeitar zero, negativos e política inválida; preservar limite grande sem estreitamento.
2. Preservar configuração sem limite e contar ações mesmo nesse perfil.
3. Encerrar exatamente em 1, 2, 7 e 10 ações, sem criar novo turno/rodada no fim.
4. Não gastar orçamento em comando inválido, nem contar evento de consumo de guarda como outra ação.
5. Vitória na última ação vence mesmo se o timeout configurado derrotaria esse lado.
6. Vitória antecipada conserva saldo e rejeita novas ações.
7. Limite impede uma ação seguinte que seria letal; tentativa posterior não altera estado.
8. Derrota de A ou B por limite é configurável e não mata personagens artificialmente.
9. Saltar morto e mudar rodada não contam como ação.
10. Orçamentos de instâncias distintas não se misturam; batalha nova começa no zero.
11. Referência completa com 34 ações termina por limite; com 35 termina por eliminação. Replays com entrada invertida mantêm estado/eventos iguais.

Resultado resumido em COMBATE.json; log completo ignorado em build/combate/resultado.log.

## Contrato da futura fase

O orçamento implementado pertence a **uma Battle**, não à campanha/fase/frente. Reutilizar a mesma configuração em duas instâncias cria dois contadores locais independentes. Não compartilhar esse orçamento por referência nem chamar isso de limite global.

Quando Stage existir, ele será responsável por orçamento acumulado: contar somente comandos aceitos, não restaurar saldo ao trocar confronto e interromper a fase uma única vez. A configuração global precisa declarar unidade e política compatíveis com as locais. O coordenador terá de decidir se inicia uma ação em cada frente quando resta apenas uma unidade global; isso não está resolvido por esta API.

WAVE-01 implementará essa coordenação junto da passagem de estado. PEN-01 e PEN-02 do registro de pendências precisam de uma política explícita nesse bloco. Não há persistência, transição de chefe, reação/assistência, AP ou paralelismo novo nesta entrega. Testar evento de guarda consumida não equivale a testar habilidades extras que ainda não existem.
