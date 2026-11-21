# Adendo Técnico Consolidado — Fechamento dos Detalhes de Contrato (20/11/2026)

Este documento fecha os detalhes técnicos pendentes apontados em `RESPOSTAS-ENT-2026-11-20.md` para os blocos futuros (`HAB`, `REG-01`, `REG-03`, `SIS-01`, `SIS-02`, `WAVE-01` e `Score`), preservando integralmente os contratos e testes históricos já aprovados (`ESC-01–07`, `R04`, `TIME-01–03`, `RES-01`, `SEQ-01`, `PAR-01–02`).

---

## 1. Ficha do Felipe — Passiva `A2` (*Postura de Vigília*) (`HAB / Tutorial`)
- **Identidade e Tipo:** `A2` consolidada como **Passiva** (*Postura de Vigília*), não selecionável manualmente.
- **Gatilho e Acúmulo de Cargas:**
  - Felipe ganha **`+1` carga** por cada membro da equipe (ele próprio ou qualquer aliado vivo) atingido por um ataque inimigo.
  - A contagem é feita **por aliado atingido naquela habilidade inimiga** (um ataque em área que atinge Felipe + 4 aliados gera `+5` cargas de uma vez; um ataque multi-hit em um único alvo gera apenas `+1` carga naquela habilidade).
- **Limite Dinâmico (`MaxCharges = MaxPartySize`):**
  - O limite máximo de cargas acompanha o tamanho máximo da formação ativa do modo (**`5` cargas** no padrão `5x5`; **`6` cargas** caso o perfil `6x6` seja utilizado).
- **Consumo e Efeito de Reação:**
  - Ao atingir o limite (`MaxPartySize`), consome **todas as cargas (`5` ou `6`)** para disparar automaticamente:
    1. **Contra-ataque (`A1` — *Garra do Asfalto*)** contra o autor do ataque que completou o acúmulo.
    2. **Escudo Automático:** Concede **`StandardShield`** (`1%` do HP máximo do Felipe, mínimo `1` ponto, duração de **`3` turnos naturais**) ao **próprio Felipe** (ou ao aliado vivo com menor percentual de HP).
- **Comportamento no Tutorial (Capítulo 1):** No roteiro guiado, Felipe inicia com cargas pré-carregadas (`MaxPartySize - 1`) para que o golpe sofrido no Turno 2 complete o máximo e acione a `A2` antes da `A3`.

---

## 2. Bloqueio (`Block`), Mitigação de Dano e `True Damage` / `DoT` (`REG-01`)
- **Bloqueio (`Block` — Chance Real Universal de Anulação):**
  - **Base universal:** **`3%`** por hit para todos os personagens (`10%` base para **Michel**), somando bônus de passivas/habilidades/equipamentos até o **teto geral de `50%`** (e **`100%`** apenas sob estados especiais explícitos).
  - **Efeito:** Quando ativado, **anula completamente** o dano direto daquele golpe/hit (em continuidade com o `Block` validado nos cenários antigos).
  - **Inelegibilidade:** **`True Damage` (Dano Verdadeiro)** e **efeitos contínuos (`DoT` — Veneno, Sangramento, Queimadura)** **ignoram o Bloqueio** (não são elegíveis para roll de `Block`).
- **Mitigação de Dano (`Damage Mitigation` — Camada Independente):**
  - **Não é passivo universal:** A redução percentual (`30%` a `50%`) **não existe** por padrão nos personagens sem efeito ativo; aplica-se **somente** quando concedida por buff defensivo, passiva específica ou equipamento.
  - **Acúmulo e teto:** Múltiplas fontes de mitigação percentual somam-se até o **teto de `50%`** (salvo exceção explícita em habilidade única) e combinam-se com `DEF` e (no Modo Clássico) com o comando `Defender` (`floor(dano / 2)`).

---

## 3. Funil de Dano, Aplicação de Debuffs (`ACC / RESIST`) e Escala Numérica (`SIS-02`)
- **Separação em Dois Trilhos Independentes por Hit:**
  - **Trilho A — Resolução de Dano:**
    `1. Imunidade a Dano / Invencibilidade` → `2. Esquiva (Miss)` → `3. Bloqueio (Block 3%+, ignorado por True Damage)` → `4. Crítico (base 150% de dano crítico, escalável)` → `5. Mitigação (DEF + Redução Percentual + Defesa Clássica)` → `6. Absorção de Escudo` → `7. Aplicação no HP`.
    - *Piso de Dano:* Golpes comuns que acertam e não são anulados têm **piso mínimo de `1` ponto de dano**; no Modo Clássico, `Defender` aplica `floor(dano / 2)` **após** o cálculo base, permitindo resultado **`0`**.
    - *`True Damage`:* Ignora `DEF`, Mitigação Percentual, Defesa Clássica e Bloqueio (`Block`); é barrado apenas por **Imunidade a Dano / Invencibilidade** ou absorvido por **Escudos** ativos.
  - **Trilho B — Aplicação de Debuffs / Controle (após o hit acertar e não ser esquivado/bloqueado):**
    `1. Imunidade a Debuffs (Immunity)` → `2. Chance Base da Habilidade` → `3. Teste de ACC vs. RESIST` (com piso mínimo de `20%` de resistência natural contra efeitos não-irresistíveis).
- **Escala Numérica e Tipos (`int64`):**
  - Preservar os valores pequenos dos cenários históricos de regressão, mas adotar oficialmente a **escala ampla estilo *Dislyte*** para os novos catálogos e fichas (HP de personagens na casa de `10.000–50.000+`, ATK/DEF na casa de `1.000–6.000+`, HP de chefes na casa de milhões).
  - Utilizar **`int64`** para armazenamento de HP, Escudo, Dano e Score, com teto operacional seguro de **`10^12`** (`1.000.000.000.000`), realizando multiplicações intermediárias em ponto flutuante determinístico (`double`) com `floor` no fechamento do hit.

---

## 4. Supressão, Congelamento e Matriz de Remoção de Efeitos (`REG-03 / SIS-01`)
- **Supressão (`Suppression`):**
  - Bloqueia o disparo de passivas reativas e auto-revive vindos diretamente do kit enquanto ativa.
  - **Exceção de Estados Já Ativos / Borda Protegida:** Se o efeito da passiva **já tiver sido ativado e convertido em um estado/buff ativo** antes da aplicação da Supressão — ou se possuir **borda colorida protegida** (*Prateada*, *Dourada* ou *Estrutural*) — **ele permanece ativo e continua funcionando normalmente** mesmo sob Supressão.
  - **Prevenção de Morte:** Prevenção de morte vinda de passiva interna não-ativada fica bloqueada sob Supressão; prevenção de morte vinda de **buff ativo na barra** (especialmente com borda protegida/dourada) **continua funcionando normalmente**.
- **Congelamento (`Freeze`) e Petrificação (`Petrification`):**
  - Cancelam reações já enfileiradas (`Counter`, `Assist`, `Pursuit`) antes da execução, pois o portador está impossibilitado de agir.
  - **Silêncio (`Silence`)** isolado **não** cancela contra-ataque com básica (`A1`), pois `A1` não é silenciável.
- **Matriz Oficial de Categorias × Remoção (`Dispel / Cleanse / Steal / Transfer`):**

| Categoria / Borda | Dissipar / Purificar (`Dispel / Cleanse`) | Roubar / Transferir (`Steal / Transfer`) | Desligado por Supressão quando já ativo | Transição Entre Ondas (`WAVE-01`) |
|---|---|---|---|---|
| **Comum (Sem Borda)** | **Sim** (passa por `ACC/RESIST` no inimigo; direto no aliado) | **Sim** (passa por `ACC/RESIST`) | Não | **Sempre removido** (mesmo com turnos restantes) |
| **Prateada (`Silver`)** | **Sim** (pode ser dissipado ou purificado) | **Não** (imune a roubo e transferência) | **Não** | **Preservado** (enquanto tiver turnos) |
| **Dourada (`Golden`)** | **Não** (imune a remoção comum) | **Não** | **Não** | **Preservado** (enquanto tiver turnos) |
| **Estrutural (`Structural`)** | **Não** | **Não** | **Não** (imune à Supressão) | **Preservado** conforme encontro |

---

## 5. Escudo Especializado do Filhote (`SilverShield`) (`SIS-01`)
- **Fórmula e Duração:** **`15%` do HP máximo do aplicador** por **`2` turnos naturais** do protegido (categoria **Prateada**).
- **Coexistência com `StandardShield`:** Podem **coexistir simultaneamente** na barra de proteção sem substituir um ao outro. Ao sofrer dano, consome-se **primeiro o `StandardShield`** (mais frágil/temporário) e **depois o `SilverShield`**.
- **Renovação (Reaplicação de outro `SilverShield`):** Segue a mesma regra de `ESC-05` — **não soma** pontos nem turnos; preserva `max(pontos atuais, novos pontos)` e `max(turnos atuais, novos turnos)`.
- **Permissões (`Prateado`):** **Pode ser dissipado (`Dispel`)**, mas **não pode ser roubado nem transferido**, **não é desligado por Supressão** após aplicado e **é preservado entre ondas** enquanto restarem turnos.

---

## 6. Transições de Ondas (`WAVE-01`) e Pontuação (`Score`) (`WAVE / SEQ / PAR`)
- **Ordem Exata na Transição Entre Ondas (`WAVE-01`):**
  1. **Limpeza de Efeitos Comuns:** Remove todos os **buffs/debuffs comuns (sem borda diferenciada)** e o **`StandardShield`** de todos os combatentes remanescentes, mesmo que ainda possuam turnos restantes. Efeitos **Prateados (`Silver`)**, **Dourados (`Golden`)**, **Estruturais**, cargas de passiva e recargas (`cooldowns`) permanecem ativos.
  2. **Recuperação de Vida:** Cura **`30%` do HP máximo** (`floor(MaxHP * 0.30)`, limitado ao `MaxHP` e sem reviver aliados derrotados) em todos os **aliados vivos**.
- **Pontuação (`ScoreAttack` — Modular e Fechada uma Única Vez):**
  - **Pontuação Base (`ScoreBase`):** Calculada e arredondada **uma única vez no fechamento** da tentativa sobre os totais acumulados (`ScoreBase = floor(DanoEfetivoTotal + (OverkillTotal * 0.25))`), usando acumulador `int64` com teto seguro de **`10^12`** e entrega idempotente vinculada ao ID da tentativa.
  - **Bônus de Sobrevivência (`SurvivalBonus` — Opcional por Modo/Evento):** **Não** é aplicado por padrão em todas as fases; funciona como um **modificador opcional parametrizado na configuração do evento/fase** (para eventos específicos com recompensas vinculadas à sobrevivência da equipe).

---

## 7. Escopos de Engenharia e Blocos Posteriores
- **Interface, Vídeo e Android (Itens 13–14 e 17–18):** Confirmado como tarefas internas de engenharia (`AUT-01` a `AUT-11`) e conferência local de ambiente (`SDK/NDK/JDK/AVD` em `D:\Android`), sem pendências de pesquisa adicionais.
- **Modo Clássico e Modo DJ (Itens 15–16):** Custos de SP, carregamento de Limite, relógio ATB Active/Wait e pontuação das notas ficam reservados para seus contratos dedicados posteriores, mantendo já fechada a separação de rankings por layout (`3 / 4 / 5` pistas).