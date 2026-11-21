# Entrega — Core, Habilidades, Efeitos e Fórmulas (P1/P2)

## 1. Matriz de Controle: Silêncio, Supressão, Congelamento e Petrificação
**DECISÃO DESEJADA / PROPOSTA:** Separação rigorosa entre incapacitação total (perda de turno) e restrição parcial (força habilidade básica ou desativa passivas), mantendo a regra já fechada de que a Básica (A1) nunca é silenciada.

| Estado de Controle | Pode Agir no Turno? | Ativas Bloqueadas | Passivas Afetadas | Relógio (`TickAnchor`) | Exceções e Regras Específicas |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Silêncio (*Silence*)** | **Sim** | A2 e A3 bloqueadas (forçado a usar **A1 Básica**). | **Nenhuma** (passivas e reações já engatilhadas continuam ativas). | `CarrierTurnEnd` (decrementa ao fechar o turno ativo do portador). | Não bloqueia passivas nem comandos de *Guarda* (se permitida no modo). Moldura *Red* (dissipável/transferível; respeita `RESIST - ACC` e `Immunity`). |
| **Supressão (*Suppression* / *Seal*)** | **Sim** | **Nenhuma** (pode usar A1, A2 e A3 normalmente). | **Desativa passivas nativas** (gatilhos *On-Hit*, *On-Death*, auras e reações passivas do kit) enquanto ativo. | `CarrierTurnEnd` (decrementa ao fechar o turno ativo do portador). | Não desativa atributos base já somados nem efeitos de equipamentos (estes são desligados por estado equivalente a *Molten Hot*). Chefes (`isBoss`) possuem imunidade configurável por encontro. |
| **Congelamento (*Freeze*)** | **Não** (perde a oportunidade de ação). | Todas (A1, A2 e A3). | **Incapaz de agir / reagir** (bloqueia *Counter* e *Assist* enquanto congelado; gatilhos passivos passivos de mitigação ou morte continuam validados). | `CarrierTurnEnd` (ao chegar sua vez, perde a ação, consome 1 turno do efeito e encerra o turno). | **DECISÃO DESEJADA (Identidade Própria):** Ao contrário de *Sleep* (que quebra ao sofrer dano), *Freeze* **não quebra ao sofrer dano**, mas reduz a velocidade de recuperação de AP/ATB em 15% enquanto ativo (opcional/configurável). Chefes são imunes. |
| **Petrificação (*Petrification*)** | **Não** (perde a oportunidade de ação). | Todas (A1, A2 e A3). | **Incapaz de agir / reagir** (bloqueia *Counter*, *Assist* e *Pursuit*). | `CarrierTurnEnd` (perde a ação, decrementa 1 turno e encerra). | **PROPOSTA (Diferenciação Tática):** Enquanto petrificado, o alvo recebe **+15% de resistência a dano físico/comum**, mas sofre **+25% de dano de True Damage / Quebra** (diferenciando-o mecanicamente de *Stun* e *Freeze*). Chefes são imunes. |

---

## 2. Contrato Declarativo de Efeitos Contínuos
**DECISÃO DESEJADA:** Todo efeito novo registrado no catálogo deve declarar explicitamente os 8 campos abaixo antes de entrar em código:

1. **Alvo (`TargetScope`):** `Self`, `SingleAlly`, `AllAllies`, `SingleEnemy`, `AllEnemies`, `LowestHPAlly`, `HighestAPEnemy` (com flags `excludeSelf`, `includeDead`, `excludeBoss`).
2. **Momento de Aplicação (`ApplyTiming`):** `PreHit` (antes do cálculo de dano, ex.: dissipação pré-golpe), `OnHit` (por golpe válido sem *Miss*), `PostSkill` (após todos os golpes), `TurnStart`, `TurnEnd`, `OnDeath`, `OnRevive`.
3. **Duração (`Duration`):** Número inteiro de turnos (`0` = instantâneo, `1..N` = contínuo, `-1` = permanente até condição de remoção).
4. **Quem Conta (`TickAnchor`):** `CarrierTurnStart`, `CarrierTurnEnd`, `CasterTurnEnd`, `AnyActiveTurnEnd`, `NonCarrierTurnEnd`. Inclui flag `skipTickOnApplicationTurn = true` para buffs aplicados no próprio turno do dono.
5. **Coexistência (`StackPolicy`):**
   - `RefreshDuration`: Renova o tempo mantendo o maior valor (padrão para buffs/debuffs simples como *ATK Up*, *DEF Down*).
   - `StackIntensity`: Acumula pilhas até `maxStacks` (ex.: *Brand*, *Gossamer*, *Cadence*), definindo se o tempo renova a cada pilha ou expira em bloco.
   - `IndependentInstances`: Instâncias separadas com relógios próprios (ex.: *Bleed* e *Poison* de fontes distintas).
   - `IgnoreIfPresent`: Não substitui nem renova se já ativo.
6. **Dissipação, Roubo e Imunidade (`ProtectionTier` — 4 Molduras):**
   - **Green (Buff Padrão):** Respeita `Buff Blocker`; dissipável e roubável; conta como buff ativo.
   - **Red (Debuff Padrão):** Passa por `TriggerChance -> NotMiss -> (RESIST - ACC, piso 20%) -> Immunity`; dissipável e transferível.
   - **Silver (Estado Determinístico):** Ignora `ACC/RESIST` e `Miss`; dissipável (salvo exceção explícita), mas **não transferível nem roubável**.
   - **Golden (Estado Único de Kit):** Indissipável, inroubável, imune a `Buff Blocker` e não conta como buff comum para passivas condicionais; declara explicitamente `removeOnCasterDeath` e `persistThroughCarrierDeath`.
7. **Limite de Ativações (`TriggerQuota`):** `BattleTurnTeamGlobal`, `SourcePerActiveTurn`, `TargetPerActiveTurn`, `PerActionChain`, `OncePerBattle` + `maxCount` + `allowReactionChain` (impedindo loops infinitos).

---

## 3. Fórmulas Numéricas, Ordem de Operações e Exemplo Prático
**REFERÊNCIA EXTERNA (Dislyte — gacha.gg / Battle Guide, Patch 3.4.49) vs. DECISÃO PARA O NOSSO JOGO:**

### 3.1 Ordem de Cálculo de Dano Padrão
1. **Base Bruta:** `RawDamage = SkillRatio * ScaledStat` (`ATK`, `DEF`, `MaxHP` ou `SPD` efetivos após buffs/debuffs e pilhas).
2. **Mitigação de Defesa (Curva Hiperbólica):**
   $$\text{DefMultiplier} = \frac{1800}{570 + \max(0,\ \text{EffectiveDEF} \times (1 - \text{DefIgnore}\%))}$$
3. **Modificadores de Tipo de Acerto (Exclusivos nesta ordem):**
   - Se **Miss** (`+50%` por desvantagem elemental ou *Miss Rate Up*): `HitMultiplier = 0.75` (anula Crítico, Bónus Elemental e debuffs comuns).
   - Senão, se **Crítico** (`C.RATE - CRIT_RESIST`): `HitMultiplier = 1.0 + Effective_C_DMG`.
   - Senão, se **Vantagem Elemental**: `HitMultiplier = 1.30`.
   - Senão (**Acerto Normal**): `HitMultiplier = 1.00`.
4. **Modificadores de Dano e Guarda:**
   - `DamageMod = (1 + BonusDamageDealt% - DamageReduction%) * GuardMultiplier` (onde `GuardMultiplier = 0.50` se o alvo estiver em **Guarda** e o dano não for *True Damage*).
5. **True Damage (Regra Própria):** Ignora completamente `DefMultiplier`, `GuardMultiplier` e não realiza acerto crítico (salvo habilidade com cláusula explícita), respeitando tetos por estatística (`damageCapByStatRatio`) e redutores de chefe (`bossMultiplier`).
6. **Arredondamento e Pisos/Tetos:**
   - Todos os cálculos intermediários operam em ponto flutuante de 64 bits (`double`) ou ponto fixo (`int64`) para evitar *overflow* em pilhas altas (até +999%).
   - Arredondamento final via `floor()` apenas na aplicação do dano, com **piso mínimo de `1` de dano** (exceto sob *Invincibility* ou bloqueio total).
   - `EffectiveMaxHP` possui piso mínimo de `1` (ou `10%` do `BaseMaxHP` em chefes) após reduções de teto de HP.

### 3.2 Exemplo Numérico Passo a Passo
- **Cenário:** Atacante com `ATK = 3.000`, habilidade com `SkillRatio = 150%` (`1.50`), `C.DMG = +100%` (`HitMultiplier = 2.00`), bônus de dano `+10%` (`1.10`). Alvo com `DEF = 1.430` (sem ignorar defesa) e **sem Guarda**.
- **Cálculo:**
  1. `RawDamage = 3000 * 1.50 = 4.500`
  2. `DefMultiplier = 1800 / (570 + 1430) = 1800 / 2000 = 0.90`
  3. `CritDamage = 4.500 * 0.90 * 2.00 = 8.100`
  4. `FinalDamage = floor(8.100 * 1.10) = 8.910`
  - *(Se o alvo estivesse em **Guarda**, `8.910 * 0.50 = 4.455`. Se o alvo tivesse `5.000 HP`, sofre `5.000` de dano efetivo e gera `3.910` de `OverkillDamage`).*

---

## 4. Especificação de Bloqueio Automático Universal (Reflexo Defensivo)
**DECISÃO DESEJADA (Base 3% Geral, Base Elevada por Classe e Teto Escalonável até 100%):**
- **Distinção Conceitual:** Diferente do comando ativo de **Guarda** (ação deliberada que reduz 50% do dano até o próximo turno natural) e do atributo **DEF** (mitigação matemática contínua), o **Bloqueio Automático (*Auto-Block / Parry*)** é um **reflexo defensivo universal** disponível para todos os personagens, escalável por classe, atributos, vantagem de velocidade e vantagem elemental, além de habilidades ativas/passivas.
- **Fórmula da Chance de Bloqueio (`BlockChance`):**
  $$\text{BlockChance} = \text{clamp}\Big(\text{ClassBase} + \text{BonusStatus} + \text{BonusSPD} + \text{BonusElemental},\ 0\%,\ \text{ActiveCap}\Big)$$
  - **Base Mínima por Classe (`ClassBase`):**
    - **Padrão Geral (Atacantes, Suportes, Controladores):** **`3%`** inatos.
    - **Classes Especializadas (Defensores / Vanguardas):** Base elevada configurável por arquétipo (ex.: **`8%` a `12%`** inatos).
    - **Exceção de Incapacitação:** Cai temporariamente para **`0%`** enquanto o personagem estiver sob *Stun*, *Freeze*, *Sleep* ou *Petrification*.
  - **Status / Equipamentos (`BonusStatus`):** Bônus vindos de subatributos de equipamentos, passivas individuais e buffs comuns (*Green*).
  - **Vantagem de Velocidade (`BonusSPD`):** Até **`+15%`** quando `SPD_defensor > SPD_atacante` (`min(15%, max(0, ((SPD_def - SPD_atk) / SPD_atk) * 25%))`), representando maior tempo de reação.
  - **Vantagem Elemental (`BonusElemental`):** **`+10%`** quando o defensor possui vantagem elemental sobre o atacante (`0%` em neutro ou desvantagem).
  - **Teto (`ActiveCap` — 50% Natural até 100% Especial):**
    - **Teto Natural (`50%`):** Limite máximo atingível apenas pela soma passiva de `ClassBase + BonusStatus + BonusSPD + BonusElemental`.
    - **Extensão Especial (até `100%`):** Habilidades dedicadas, posturas defensivas, estados únicos (*Golden*) ou cargas garantidas (*Parry Charges*) podem elevar o teto individual do portador acima de `50%`, chegando até **`100%`** enquanto o efeito ou carga estiver ativo.
- **Granularidade e Elegibilidade:**
  - Avaliado **por golpe (`perHit`)** em ataques diretos (`DirectSkill`, `Pursuit`, `Counter`, `Assist`).
  - **Não se aplica** a dano contínuo de turno (*Poison*, *Bleed*, *Echo Scar*), *True Damage* ou habilidades com a flag `ignoresAutoBlock = true`.
- **Efeitos ao Acertar o Bloqueio:**
  - Reduz o dano do golpe em **`30%`** (mitigação base universal, amplificável até `50%` por passivas/itens) e **impede acerto crítico** naquele golpe (mantendo a verificação normal de debuffs).
  - Dispara gancho opcional `OnBlockSuccess` para kits reativos (respeitando `TriggerQuota` para evitar loops infinitos).

---

## 5. Amostra Inicial: 4 Fichas Originais (Universo *Contos de Um Lobo na Cidade*)
Escopo de Perfil: As 3 posições (A1 / A2 / A3) e recargas em turnos aplicam-se ao Perfil Padrão de Equipes (PvE/PvP); no Perfil Clássico (-CombatProfile=Classic), os personagens utilizam custo de Estamina/SP, comando ativo de Defesa (floor(dano / 2)) e uma 4ª habilidade de Especial/Limite carregada por barra própria (especificada na entrega de Chefes, Encontros e Modo Clássico).
**DECISÃO DESEJADA (Fidelidade ao Universo do Livro + Estrutura Rígida A1 / A2 / A3 + Fases Elementais):**
- **Regra Estrutural das 3 Posições:**
  1. **A1 (Básica):** Sempre ataque ativo; **imune a Silêncio** (se o personagem puder agir no turno, sempre pode usar A1; apenas incapacitação total como *Stun/Freeze/Petrification* impede a ação).
  2. **A2 (Secundária):** Pode ser habilidade ativa com recarga **ou** a **Passiva Pura** do personagem (afetada por *Supressão*).
  3. **A3 (Ultimate):** Sempre uma **habilidade ativa executável** (nunca passiva pura), podendo incluir componente passivo acoplado ou **critérios de ativação** (ex.: acúmulo de cargas/condição narrativa, com opção de disparo automático ou acionamento manual no turno).
- **Sistema de Fases Elementais (`FElementalPhaseData`):** Em vez de duplicar personagens que evoluem ao longo da história em várias entradas separadas, uma única ficha suporta **Fases de Sintonia Elemental** desbloqueadas pela narrativa (aplicado a **Felipe**, **O Filhote** e **O Dálmata**), alterando sua afinidade elemental e modificadores temáticos sobre o mesmo kit base.

### Ficha 01 — Felipe, O Lobo da Cidade (Lutador Híbrido Noturno)
- **Elemento e Fases:** **Umbra (Sombra)** na Fase 1 *(estrutura preparada para desbloqueio narrativo da **Fase 2: Luz/Shimmer** e **Fase 3: Natureza/Verde**)*.
- **A1 — Garra do Asfalto (Básica Ativa | CD: 0 | Imune a Silêncio):**
  - Ataca 1 inimigo (110% ATK). Se o alvo possuir qualquer debuff, aplica *Sangramento* (`Red`, 2 turnos) e ganha +10% de AP. *(Modificador de Fase: na Fase Luz aplica brilho/purificação; na Fase Natureza regenera 5% do HP próprio).*
- **A2 — Olhar da Garoa (Passiva Pura — afetada por Supressão):**
  - Inicia o combate com **2 cargas de *Manto Noturno*** (`Golden`, indissipável, máx. 4). Sempre que um aliado sofrer um ataque de alvo único, consome 1 carga (máx. 1x por turno ativo) para contra-atacar o agressor com *Garra do Asfalto* (A1). Recupera +1 carga ao fim de cada turno ativo próprio.
- **A3 — Eclipse na Floresta de Pedra (Ultimate Ativa | CD: 4 turnos):**
  - Dissipa até 2 buffs (`Green`) de todos os inimigos antes do impacto e desfere 3 golpes em área (65% ATK cada). Causa +25% de dano adicional contra alvos sem buffs ativos.

### Ficha 02 — O Filhote (Lobo-Guará) (Suporte / Guia das Almas)
- **Elemento e Fases:** **Verde (Natureza) + Umbra (Sombra)** *(suporta ao menos 1 mudança de fase narrativa para ajuste de sintonia entre Natureza plena e Guia Noturno)*.
- **A1 — Passo Ágil (Básica Ativa | CD: 0 | Imune a Silêncio):**
  - Ataca 1 inimigo (100% ATK) e concede *Aumento de Velocidade* (`Green`, +30% SPD, 1 turno) ao aliado com menor barra de AP.
- **A2 — Chama Amiga da Noite (Passiva Pura — afetada por Supressão):**
  - No início de cada onda (`OnRoundStart`), concede *Escudo* (`Green`, 15% do Max HP do Filhote, 2 turnos) a toda a equipe. Sempre que um aliado receber cura ou escudo, o Filhote acumula +1 *Luz Guia* (`Golden`, máx. 3 pilhas).
- **A3 — Festa das Gotas Cintilantes (Ultimate Ativa com Critério | CD: 4 turnos ou 3 pilhas de *Luz Guia*):**
  - Purifica até 2 debuffs (`Red`) de todos os aliados, aplica *Recuperação* (`Green`, 15% Max HP por 2 turnos) e impulsiona o AP da equipe em +20%. *(Quando atinge 3 pilhas de Luz Guia antes do fim da recarga, libera ativação antecipada manual ou automática configurável).*

### Ficha 03 — Michel, O Guardião (Defensor / Vanguarda Terrena)
- **Elemento:** **Fogo / Terra (Terreno Bruto) + Fluxo** *(identidade fixa robusta)*.
- **A1 — Impacto de Prata (Básica Ativa | CD: 0 | Imune a Silêncio):**
  - Ataca 1 inimigo (100% DEF) com 60% de chance de aplicar *Provocação* (`Red`, 1 turno).
- **A2 — Égide Inabalável (Passiva Pura — afetada por Supressão):**
  - Base inata de **Bloqueio Automático elevada para 10%**. Reduz em 15% o dano em área sofrido por todos os aliados vivos. Sempre que Michel bloquear um golpe ou sofrer dano em **Guarda**, acumula +1 *Firmeza* (`Golden`, +5% DEF e +3% Chance de Bloqueio, máx. 10 pilhas).
- **A3 — Bastião da Vigília (Ultimate Ativa + Efeito Passivo | CD: 4 turnos):**
  - *Passivo acoplado:* Enquanto esta habilidade estiver disponível (fora de recarga), Michel ganha +15% de Resistência a Controle.
  - *Ativo:* Concede *Aumento de DEF* (`Green`, +60%, 2 turnos) e *Imunidade* (`Green`, 1 turno) a toda a equipe, ativando o estado de **Guarda** em si mesmo e desferindo um golpe em área (120% DEF) que reduz o AP dos inimigos em 15%.

### Ficha 04 — O Dálmata (Controlador / Estrategista Urbano)
- **Elemento e Fases:** **Fluxo + Luz (ou Sombra)** *(suporta ao menos 1 mudança de fase narrativa para alternar o foco entre controle táctico e suporte ofensivo)*.
- **A1 — Investida Marcada (Básica Ativa | CD: 0 | Imune a Silêncio):**
  - Ataca 1 inimigo (100% ATK) com 65% de chance de aplicar *Redução de SPD* (`Red`, -30%, 2 turnos) e convoca o aliado de maior ATK para uma *Assistência* (50% de chance, máx. 1x por turno ativo).
- **A2 — Instinto de Matilha (Passiva Pura — afetada por Supressão):**
  - Aumenta a Precisão (`ACC`) da equipe em +20%. Sempre que um inimigo tentar ativar uma habilidade fora de turno (*Counter* ou *Assist*), O Dálmata reduz o dano desse contra-ataque em 25%. *(Na Fase 2 narrativa, também concede +10% de Chance de Bloqueio aos aliados adjacentes).*
- **A3 — Cerco da Encruzilhada (Ultimate Ativa | CD: 4 turnos):**
  - Ataca todos os inimigos (110% ATK) com 75% de chance de aplicar **Supressão** (`Red`, desativa passivas nativas por 2 turnos) e 65% de chance de aplicar **Silêncio** (`Red`, bloqueia A2/A3 por 1 turno) no alvo principal.

---

## 6. Rastreabilidade de Fontes Externas (*Dislyte*)
- **Fonte Primária Oficial:** Farlight Games — Notas de Atualização *Patch 3.4.49* (publicada em 26/10/2026, implementada em 03/11/2026) e *Patch 3.4.42 a 3.4.48* (junho–outubro/2026).
- **Fontes Técnicas Secundárias:** *Dislyte Fandom Wiki* (molduras *Green/Red/Silver/Golden*, chefes *Yinglong*, histórico de *Toland 3.4.50* e *Tevor*), *PlayDislyte Battle Guide* (funil `Trigger -> Miss -> RESIST-ACC piso 20% -> Immunity`) e *Gacha.gg Damage Calculator* (fórmula hiperbólica `1800 / (570 + DEF)`).
- **Separação Arquitetural:** Todas as mecânicas extraídas do *Dislyte* servem exclusivamente como referência de comportamento sistémico; nomes, personagens, valores de equilíbrio e regras próprias (como *Guarda*, *Supressão*, diferenciação *Freeze/Petrification* e *Bloqueio Automático*) pertencem à especificação original deste projeto.