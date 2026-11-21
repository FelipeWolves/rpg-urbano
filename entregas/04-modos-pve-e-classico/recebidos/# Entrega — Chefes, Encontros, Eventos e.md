# Entrega — Chefes, Encontros, Eventos e Modo Clássico (P2)

**Separação Arquitetural Central (DECISÃO DESEJADA):**
O jogo mantém dois grandes perfis de batalha sobre o mesmo núcleo determinístico C++20 (`CombatProfile`):
1. **Modo Padrão de Equipes (PvE e futuro PvP):** Segue a linha tática do *Dislyte* (barra de AP por velocidade `SPD`, 3 posições de habilidade `A1/A2/A3`, sem comando genérico de Defesa).
2. **Modo Clássico (Linha de Evento Especial):** Segue a linha dos RPGs clássicos de PS1/PS2 (*Final Fantasy* VII/VIII/IX/X), utilizando o perfil dedicado `-CombatProfile=Classic` (com comando de **Defesa** explícito, itens/consumíveis de batalha, 4ª posição de Especial/Limit Break e opção de ritmo ATB *Active / Wait*).

---

## 1. Matriz de Modos PvE e Futuro PvP (Formação, Fluxo e Condição de Término)
**DECISÃO DESEJADA / PROPOSTA (Preservando contratos SEQ-01, WAVE-01 e PAR-02 já validados):**

| Modo / Família | Equipes e Unidades | Fluxo (Simultâneo vs. Sequencial) | Vida (HP) e Persistência | Condição de Término |
| :--- | :--- | :--- | :--- | :--- |
| **Campanha / Ondas (`WAVE-01`)** | 1 equipe de 1 a 5 unidades. | **Sequencial por ondas** (até 5 confrontos seguidos). | Inimigos separados por onda. A equipe conserva HP, recargas e estados configurados entre ondas; AP reinicia a cada onda. | **Vitória:** Eliminar todas as ondas.<br>**Derrota:** Eliminação da equipe ou atingir limite máximo de ações/turnos. |
| **Chefe de Desafio / Ritual (`PVE-01`)** | 1 equipe de até 5 unidades. | **Confronto único** (ou ondas prévias + chefe final). | Chefe com HP próprio e auxiliares opcionais. | **Vitória:** Eliminar o chefe principal.<br>**Derrota:** Eliminação da equipe ou exceder limite de turnos. |
| **Chefe em Sequência (`SEQ-01` — 3 Times)** | Até 3 equipes disjuntas (sem repetir personagens; até 5 por equipe). | **Sequencial (1 equipe ativa por vez):** Equipe 1 entra $\rightarrow$ ao cair ou atingir limite local de turnos, entra Equipe 2 $\rightarrow$ depois Equipe 3. **Sem retorno** de equipe derrotada/substituída. | **Chefe persistente:** mantém HP atual, debuffs/pilhas permitidos e recargas entre as trocas de equipe. | **Vitória:** Eliminar o chefe antes de esgotar as 3 equipes.<br>**Derrota:** Eliminação das 3 equipes ou fim do orçamento global de turnos. |
| **Anomalia / 3 Frentes (`PAR-02` — Perfil Fechado)** | 3 equipes simultâneas (até 5 por frente). | **Simultâneo / Intercalado** com barra de AP/SPD global (perfil já fechado). | **HP único compartilhado do chefe** (barra global), mantendo alvos/frentes próprias para habilidades de área local. | **Vitória:** Zerar o HP global do chefe.<br>**Derrota:** Eliminação de todas as frentes ou atingir teto global de ações. |
| **Modo de Pontuação / Sobrevivência (`Treva Fluxo`)** | Até 3 equipes (configurável por evento). | Sequencial por equipe até derrota da equipe ou fim da janela do modo. | Chefe com barra de HP de dano acumulado (ou múltiplas barras de limiar). | **Encerramento por Janela / Sobrevivência:** O fim da janela de turnos ou queda das equipes **encerra a tentativa e registra a pontuação** (não é derrota punitiva; premia por faixa de dano/sobrevivência). |
| **Futuro PvP (Arena / Point War)** | 1 equipe de ataque (até 5) vs. 1 equipe de defesa (até 5). | **Simultâneo** (barra de AP por SPD; desempate determinístico por posição/ID). | Vidas individuais por combatente. | **Vitória:** Eliminar toda a equipe adversária (com regra de desempate/fadiga progressiva após limite alto de turnos). |

---

## 2. Vitória, Sobrevivência, Pontuação e Recompensas por Modo
**DECISÃO DESEJADA / PROPOSTA (Contrato de Score, Economia Separada e Recompensa Idempotente — PEN-10):**
- **Distinção de Resultados:**
  - *Eliminação (`Elimination`):* Zerar HP de todos os alvos obrigatórios (Campanha, Ritual, PvP).
  - *Sobrevivência (`Survival`):* Manter pelo menos 1 aliado vivo ao final de `N` turnos do chefe.
  - *Pontuação (`ScoreAttack`):* Acumular dano/objetivos dentro de uma janela fixa de turnos.
- **Contagem de Dano vs. Overkill:**
  - Para **morte e combate**, vale apenas o **dano efetivo** (limitado ao HP + Escudo restante do alvo).
  - Para **pontuação (`Score`)** em modos de ranking/evento, registra-se `DanoEfetivo + (Overkill * 0.25)` somado a bônus de sobrevivência e multiplicador de dificuldade, garantindo entrega única e idempotente (`CIC-01`).
- **Separação de Recompensas por Modo (Sem Ouro no Clássico, DJ ou PvP):**
  - **Campanha e Chefes PvE:** Concedem **Ouro** (moeda básica), equipamentos/relíquias e materiais de forja.
  - **Modo Clássico (Linha de Evento Especial):** Concede **Fichas Clássicas** (trocáveis na *Loja de Fichas Clássicas*), consumíveis do modo e marcos exclusivos de evento. Pode ser acionado também dentro de **Eventos Rítmicos de Purificação** (`EXP-01`) intercalando música e batalha.
  - **Futuro PvP (Arena / Campeonato):** Concede **recompensas diárias por patente/rank** (contemplando desde os ranks iniciais até o topo da tabela) e **Pontos de PvP**, gastos na *Loja de PvP* (que reúne tanto itens/equipamentos exclusivos válidos apenas no PvP quanto materiais raros para fortalecer personagens raros).

---

## 3. Fases do Chefe: Gatilhos, Mudanças e Persistência
**PROPOSTA (Expansão `PVE-09` sobre o motor atual):**
- **Tipos de Gatilho (`BossPhaseTrigger`):**
  1. **Por Limiar de HP (`HPThreshold`):** Ex.: ao cruzar `70%` e `35%` de HP máximo. Se um golpe massivo cruzar dois limiares de uma vez, resolve a fase mais avançada sem perder eventos obrigatórios de transição.
  2. **Por Contador de Turnos do Chefe (`BossTurnCount`):** Ex.: a cada 5 turnos próprios do chefe, alterna postura (*Ofensiva* $\leftrightarrow$ *Defensiva*).
  3. **Por Troca de Equipe (`TeamSwap` — `SEQ-01`):** Atualiza o kit/postura conforme a entrada da nova equipe.
- **O Que Muda na Transição de Fase:**
  - Troca o conjunto de habilidades ativas/passivas do chefe, pode invocar novos auxiliares ou ativar uma aura de campo (`Golden`).
- **O Que Persiste Entre Fases:**
  - HP atual (salvo cura explícita declarada na transição), debuffs com moldura `Silver` e `Golden` permanecem; debuffs comuns `Red` podem ser purificados **apenas** se a habilidade de transição declarar `cleanseOnPhaseShift = true`.

---

## 4. Diferenciação: Chefe, Auxiliares e Invocações
**DECISÃO DESEJADA (Hierarquia de Entidades em Campo):**
- **Chefe Principal (`Boss`):** Imune a controle rígido (*Stun*, *Freeze*, *Petrification*, *Sleep*) e morte instantânea; possui resistência parcial configurável a manipulação de AP (`apReductionResistance`). Sua eliminação encerra o encontro imediatamente (salvo fase seguinte).
- **Auxiliares Nativos (`Minions / Adds` — ex.: cristais/pilares laterais):** Entram no início da onda com barra de AP própria; **não são imunes** a controle comum e podem ser destruídos para remover bônus/escudos do chefe. Podem ser revividos apenas por habilidade explícita do chefe.
- **Invocações Temporárias (`Summons`):** Ocupam slots de reserva do encontro; **são dissipadas automaticamente** se o invocador morrer (`removeOnSummonerDeath = true`) e não contam como aliados vivos para impedir derrota por eliminação do time principal.
- **Alvos e Frentes:** Em modos de 1 frente, habilidades em área atingem Chefe + Auxiliares + Invocações vivas. Em modos multi-frente (`PAR-02`), habilidades locais atingem apenas a frente atual, enquanto habilidades declaradas como `GlobalBossSkill` atingem todas as frentes.
- **Retorno de Equipes:** Nas famílias `SEQ-01` e `Treva Fluxo`, cada equipe participa **uma única vez** por tentativa (sem retorno cíclico).

---

## 5. Ficha Separada: Modo Clássico (Linha Especial PS1 / PS2 — `-CombatProfile=Classic`)
**DECISÃO DESEJADA / PROPOSTA (Ficha dedicada para a linha de Evento Especial):**

- **Estrutura de Comandos do Menu Clássico (5 Opções):**
  1. **Atacar (`Attack`):** Golpe físico básico sem custo.
  2. **Habilidades / Magias (`Skills / Arts`):** Lista de técnicas do personagem (consumindo **Estamina / SP** ou **MP**).
  3. **Defender (`Defend` — já implementado no núcleo):** Postura defensiva imediata (sem precisar escolher alvo) que reduz o dano recebido pela metade (`floor(dano / 2)`) até ser consumida ou expirar na próxima oportunidade do dono, além de conceder pequena recuperação de Estamina (`+15% SP`).
  4. **Itens (`Items`):** Acesso ao inventário limitado do encontro (consumíveis de cura de HP, restauração de Estamina/SP, purificação de status e reanimação) com limites de estoque por batalha e sem permitir uso abusivo fora do turno.
  5. **Especial / Limite (`Limit / Overdrive` — 4ª Habilidade Exclusiva do Modo Clássico):**
     - **Gatilho:** Barra própria de **Limite / Fúria** (`0%` a `100%`) que carrega ao sofrer dano, bloquear golpes ou executar ações.
     - **Execução:** Ao atingir `100%`, substitui ou libera um comando dourado de impacto cinematográfico próprio do Modo Clássico (ex.: *Despertar Total do Lobo*), independente do sistema de 3 habilidades do modo Equipes.
- **Atributo de Proteção no Modo Clássico:**
  - Mantém **DEF** (mitigação física) + **RES / Espírito** (mitigação mágica/elemental) + comando ativo de **Defesa (`50%` de redução)** + **Bloqueio Automático** (base 3% geral / maior por classe).
- **Ritmo de Turnos (`ATB` — *Active Time Battle* com opção *Active / Wait*):**
  - **Modo `Wait` (Espera — Padrão recomendado para Mobile e acessibilidade):** Enquanto o jogador abre os submenus de *Habilidades* ou *Itens* para escolher o alvo, o relógio lógico de ATB de todos os combatentes **pausa automaticamente**.
  - **Modo `Active` (Ativo — Clássico PS1/PS2):** O preenchimento contínuo da barra ATB baseada em `SPD` **não para** enquanto o jogador navega nos menus, exigindo agilidade tática na escolha dos comandos.