# Complemento de Pesquisa — Contratos de Recarga, Extras, Vitória e Troca de Times

Adendo técnico complementar ao `ROTEIRO-PESQUISA-USUARIO-PREENCHIDO.md` (relacionado aos itens `PEN-03`, `PEN-04`, `PEN-06`, `PEN-07`, `PEN-09` e `PEN-10` de `PENDENCIAS.md`). Contém exclusivamente as definições fechadas para os quatro pontos críticos de resolução do simulador.

---

## 1. Recarga: quando chega a zero e permite reutilização (Adendo Q04 — `PEN-07` / `PEN-09`)

No **Dislyte**, uma habilidade com **Recarga Base** $N$ possui ciclo completo de $N$ turnos do dono (o próprio turno de uso já conta como o 1º turno do ciclo) e o decremento natural de $-1$ ocorre **no encerramento de cada turno elegível do dono**.

| Momento na linha do tempo | Exemplo: Habilidade de Recarga $3$ | Estado da habilidade |
| :--- | :--- | :--- |
| **Turno 1 (Uso)** | Habilidade disparada ($\text{CD} = 3$) $\rightarrow$ ao **encerrar** o Turno 1, reduz $-1$ $\rightarrow$ exibe $2$ | Usada neste turno |
| **Turno 2 do dono** | Inicia exibindo $2$ (bloqueada; usa outra skill) $\rightarrow$ ao **encerrar** o Turno 2, reduz $-1$ $\rightarrow$ exibe $1$ | Bloqueada ($2 > 0$) |
| **Turno 3 do dono** | Inicia exibindo $1$ (bloqueada; usa outra skill) $\rightarrow$ ao **encerrar** o Turno 3, reduz $-1$ $\rightarrow$ chega a $0$ | Bloqueada ($1 > 0$) |
| **Turno 4 do dono** | Inicia com $0$ (ícone limpo) | **Liberada para reutilização** |

### Contrato para o simulador

* **Gatilho de decremento:** Reduzir $-1$ **no encerramento** de cada turno elegível do dono (turno natural ou turno extra com `AdvancesCooldowns = true`), **exceto** quando o dono estiver sob **Petrificação** (`FreezeCooldowns = true`), que congela o decremento daquele turno. Sob **Stun** ou **Freeze**, o decremento ocorre normalmente.
* **Efeitos de início de turno (`OnTurnStart`):** Passivas ou conjuntos de equipamento que reduzem cooldown no início do turno (ex.: set *Ocean* ou passiva do *Tevor*) executam antes da escolha de comando; se levarem o contador a $0$, liberam a habilidade imediatamente naquele mesmo turno.
* **Condição de reutilização:** A habilidade torna-se selecionável sempre que `RemainingCooldown == 0`. Sem reduções externas, uma habilidade de recarga $N$ fica bloqueada por $N - 1$ turnos seguintes e retorna exatamente no turno $T_{\text{uso}} + N$.

---

## 2. Extras: separação entre recarga e duração de efeitos (Adendo Q03 / Q05 — `PEN-07` / `PEN-09`)

Turnos extras de habilidades e turnos extras de equipamentos compartilham prioridade imediata na fila de tempo (mantendo a barra de AP dos demais congelada), mas obedecem a contratos distintos para **recargas** e **durações de status**:

| Tipo de ação | Avança Barra de AP dos demais? | Reduz Recargas (`AdvancesCooldowns`)? | Reduz Duração de Efeitos (`AdvancesEffectDurations`)? |
| :--- | :--- | :--- | :--- |
| **Turno Natural** | Zera a do agente; congela demais | **Sim** ($-1$ no fim do turno, salvo Petrificação) | **Sim** (DoTs/HoTs no início; buffs/debuffs no fim) |
| **Turno Extra por Habilidade** *(ex.: Gaius, Tetsuya R6)* | Não (mantém AP congelado) | **Sim** (conta como turno pleno do dono) | **Sim** (consome $1$ turno dos efeitos do dono) |
| **Turno Extra por Equipamento** *(ex.: Set Astralis)* | Não (mantém AP congelado) | **Não** (recargas ficam congeladas) | **Sim** (efeitos ativos no portador continuam rodando) |
| **Reações fora de turno** *(Assistência, Perseguição, Contra-ataque)* | Não | **Não** | **Não** |

### Contrato para o simulador

* Desacoplar duas flags booleanas na definição de `ExtraTurnSource`:
  1. `AdvancesCooldowns`: `true` para turnos extras concedidos por habilidades; `false` para turnos extras concedidos por equipamentos/relíquias (impedindo que equipamentos acelerem recargas além de concederem uma ação extra).
  2. `AdvancesEffectDurations`: `true` para **ambos** os tipos de turno extra (pois o personagem executa uma janela real de turno e usufrui de seus buffs ou sofre DoTs), e `false` para assistências, perseguições e contra-ataques.

---

## 3. Vitória: resolução entre revives, reações e efeitos pós-morte (Adendo Q07 — `PEN-06` / `PEN-09` / `PEN-10`)

Para eliminar qualquer conflito entre aguardar auto-revives e declarar vitória antes de contra-ataques ou reflexões mortais, cada ação encerra seguindo esta pipeline fixa de 5 etapas:

1. **Resolução inline de cada hit ($\text{HP} \le 0$):**
   * Ao reduzir o HP de um alvo a $0$ durante um golpe, verifica-se primeiro a **Prevenção de Morte** (ex.: *Standoff*, passiva do *Ollie*, *Ikki*). Se ativar, o alvo trava em $1\text{ HP}$ e **não morre**.
   * Caso contrário, o alvo entra imediatamente no estado `Dead` (`IsAlive = false`) e quaisquer contra-ataques enfileirados pertencentes a essa unidade morta são **cancelados na hora**.
2. **Conclusão integral dos hits da habilidade:**
   * Todos os acertos da habilidade (alvo único ou *bounce* entre alvos vivos restantes) encerram antes de abrir qualquer janela de reação.
3. **Janela de Auto-Revive / Sobrevivência Pós-Morte (antes de checar vitória):**
   * Efeitos automáticos de ressurreição imediata (ex.: passiva da *Cecilia*) executam **neste momento**.
   * Enquanto houver auto-revive pendente na fila, a batalha **ainda não declara vitória**, pois o lado defensor ainda pode retornar com unidades vivas.
4. **Travamento da Vitória (`Victory Lock`):**
   * Encerrados os hits e os auto-revives imediatos, verifica-se `CountAlive(Defensores) == 0`.
   * Se for $0$, **a vitória do atacante é travada imediatamente**: contra-ataques de defensores sobreviventes não disparam e a batalha encerra como **Vitória**.
   * **Morte simultânea (reflexão ou explosão pós-morte):** Se o último golpe matar o último defensor e o dano refletido/explosão pós-morte matar o último atacante na mesma cadeia (`CountAlive(Defensores) == 0` e `CountAlive(Atacantes) == 0`), prevalece a **Vitória do Atacante**, pois a eliminação da equipe defensora ocorreu primeiro na resolução do golpe.
5. **Reações de sobreviventes (somente se ambos os lados tiverem `CountAlive > 0`):**
   * Executam-se assistências/perseguições do lado atacante e, por fim, contra-ataques dos defensores sobreviventes.

---

## 4. Troca de times: AP, recargas, escudos e estados permanentes do chefe (Adendo Q11 — `PEN-03` / `PEN-04` / `PEN-10`)

Em modos de múltiplos esquadrões sucessivos contra o mesmo chefe (como *Sentinel Hunt* — *Phobetor* e *Dark Star Lord*), a transição $\text{Time } 1 \rightarrow \text{Time } 2 \rightarrow \text{Time } 3$ segue esta matriz de estado:

| Variável de Estado | Novo Time que entra em campo ($\text{Time } 2$ / $\text{Time } 3$) | Chefe persistente na arena |
| :--- | :--- | :--- |
| **Barra de AP** | Inicia zerada em $0\%$ e disputa a primeira rodada pela SPD efetiva (aplicando Aura de Líder e passivas de início de luta do novo time). | **Zerada ($0\%$)** na transição, impedindo ação no frame $0$ antes da corrida de velocidade do novo esquadrão. |
| **Recargas (`Cooldowns`)** | Todas as habilidades entram prontas ($0$ turnos de recarga). | **Conserva as recargas atuais** de onde pararam no fim do time anterior (só reinicia se a troca coincidir com troca de kit por mudança de fase). |
| **Escudos (`Shields`)** | Entra sem escudos residuais de times anteriores; recebe apenas escudos próprios de início de combate (ex.: set *Aegis*). | **Escudos temporários comuns são removidos** na troca de time; **escudos estruturais de mecânica de fase** (ex.: *Life Shield* do Phobetor) **permanecem** até serem quebrados. |
| **Estados Permanentes e Debuffs** | Entra limpo (sem buffs/debuffs herdados), recebendo apenas sua própria Aura de Líder e efeitos de início de batalha. | **Debuffs temporários** (*DEF Down, Poison, Sear, SPD Down*) são **limpos** na transição; **pilhas permanentes de escalonamento e estágio de fase do chefe** permanecem intactos até o fim da tentativa. |