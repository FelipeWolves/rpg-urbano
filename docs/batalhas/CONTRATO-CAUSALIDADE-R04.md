# Contrato de causalidade R04 — Opção B confirmada

Decisão explícita do usuário em 18/11/2026. Este é o contrato autoral vigente para a futura implementação de reflexão, morte, explosão e revive. Substitui a ambiguidade entre acertos/fim da habilidade registrada em R02/R03. Não afirma equivalência comprovada às regras do Dislyte.

## 1. Identidades

- CurrentActionActor é o autor da ação ofensiva em resolução. Seu lado é capturado e preservado durante as consequências dessa ação, mesmo se morrer.
- Em um contra-ataque, CurrentActionActor passa a ser o autor daquele contra-ataque. O dono da oportunidade original não é necessariamente esse autor.
- Uma reflexão/explosão tem emissor próprio, mas não substitui o CurrentActionActor da ação que originou essas consequências.
- ModeOffensiveSide e ModeDefensiveSide são papéis do modo, separados do autor da ação.

## 2. Pipeline vigente

### Etapa 1 — Acertos e consequências inline

Para cada acerto permitido:

1. Aplicar mitigação, escudo e dano ao HP do alvo.
2. Se o HP do alvo chegar a zero, verificar prevenção de morte. Se impedir a morte, manter vivo com 1 HP conforme o efeito. Caso contrário, confirmar a transição vivo → morto, cancelar contra-ataques pendentes dessa unidade e colocá-la no fim de pendingDeaths.
3. Resolver a reflexão desse acerto imediatamente, antes de drenar pendingDeaths.
4. Se a reflexão levar o autor a zero HP, verificar sua prevenção. Caso morra, confirmar morte, cancelar seus contra-ataques pendentes e colocá-lo no fim de pendingDeaths.
5. Drenar pendingDeaths em FIFO: retirar um morto e executar seus efeitos de morte. Novas mortes confirmadas após prevenção são inseridas no fim da mesma fila. Não usar recursão profunda para explosões em cadeia. Consequências obrigatórias continuam sendo drenadas mesmo que o autor já tenha morrido.
6. Antes do próximo acerto, interromper se o autor estiver morto ou não houver alvo válido. Em bounce, selecionar somente entre os que continuarem vivos após a drenagem.

A inserção representa morte confirmada (vivo → morto), não cada ocorrência de HP zero em uma unidade já morta. O contrato de filas deve preservar essa distinção para não duplicar efeitos de morte.

### Etapa 2 — Auto-revive

Após terminar ou interromper o loop de acertos, resolver ressurreições automáticas pertinentes às mortes da etapa 1. Não confirmar vitória enquanto houver auto-revive aplicável pendente. Reviver após o loop não retoma os acertos já cancelados pela morte do autor.

### Etapa 3 — Victory Lock

Após as consequências e auto-revives:

- Se somente um lado tiver vivos, ele vence.
- Se ambos estiverem sem vivos, vence o lado capturado de CurrentActionActor.
- Se o resultado for final, encerrar sem abrir a etapa 4.
- Se ambos mantiverem vivos, continuar para a etapa 4.

### Etapa 4 — Reações de sobreviventes

Processar assistências/perseguições do lado ativo e depois contra-ataques dos defensores sobreviventes. Mortos não executam contra-ataques pendentes cancelados. Uma ação derivada ofensiva deve resolver suas consequências e consultar o resultado antes de permitir que a fila continue após uma conclusão final.

## 3. Papéis do modo

No perfil definido pelo usuário:

- Empate exato de AP/SPD na iniciativa inicial favorece ModeOffensiveSide.
- Timeout, após a última cadeia permitida e com ambos os lados vivos, favorece ModeDefensiveSide.
- Esses papéis não determinam o vencedor no caso de morte dupla por consequências da ação.

Trata-se de política desse perfil. As políticas genéricas existentes de orçamento não foram apagadas nem alteradas nesta entrega documental.

## 4. Casos de morte dupla

| Ação em resolução | Consequência | Vencedor |
|---|---|---|
| Jogador elimina último inimigo | Reflexão/explosão elimina último aliado | Jogador |
| Inimigo elimina último aliado | Reflexão/explosão elimina último inimigo | Inimigo |
| B contra-ataca e elimina último de A | Reflexão/explosão elimina último de B | B |

Aplicar somente depois de resolver os auto-revives pertinentes. Estes são resultados definidos para nosso projeto, não resultados oficiais externos verificados.

## 5. Cinco invariantes recebidos para testes

1. **Interrupção:** morte do autor por reflexão ou explosão no acerto k impede os acertos k+1 até M.
2. **Bounce:** alvo secundário morto por explosão no acerto k não pode ser selecionado nos acertos seguintes.
3. **FIFO:** todas as explosões e mortes encadeadas do acerto são drenadas em ordem FIFO antes do próximo acerto ou do auto-revive.
4. **Fronteira e desempate:** Victory Lock acontece depois das filas obrigatórias e auto-revive; em 0 × 0 vence o lado de CurrentActionActor.
5. **Conclusão:** resultado final na etapa 3 impede toda a janela posterior de reações.

Os testes futuros devem registrar a sequência de eventos, e não apenas o vencedor: um resultado final igual pode ocultar uma ordem de execução incorreta. Manter orçamento técnico de eventos separado de vitória/derrota para tratar configurações que gerem ciclos.

## 6. Estado e sequência de implementação

Contrato fechado quanto à posição inline das explosões e ao desempate causal. Não pedir novamente a escolha A/B ou o significado geral de atacante.

O motor TIME-02 ainda não implementa reflexão, explosão, prevenção ou revive; seus resultados anteriores não validam estes invariantes. Não houve alteração de código nem execução de testes nesta entrega. R01–R03 e arquivos recebidos permanecem preservados como histórico.

Implementar as famílias em bloco próprio e verificar os cinco invariantes, preservando regressão do perfil atual. Ajustes de recarga/extras recebidos em R02 permanecem uma entrega distinta; coordenação de três times continua separada dessas mudanças.
