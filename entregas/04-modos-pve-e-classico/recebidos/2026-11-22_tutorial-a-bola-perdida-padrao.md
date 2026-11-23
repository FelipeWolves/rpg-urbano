# Ficha de Encontro Piloto — Tutorial de Batalha: *A Bola Perdida* (Modo Padrão)
**Projeto:** Contos de um Lobo na Cidade (`LOBO` / *Mundus Lupis Disco*)  
**Obra de referência:** Volume 1 — *A Bola Perdida* (adaptação de jogabilidade)  
**Modalidade:** Tutorial PvE Padrão (Combate por Turnos estilo *Dislyte*)  
**Classificação:** `DECISÃO DESEJADA` (Encontro Piloto / Tutorial Jogável)  
**Nota Canônica:** Os adversários deste tutorial são **manifestações temporárias da tempestade** criadas exclusivamente como obstáculos de treino/mecânica, sem alterar os acontecimentos oficiais do Volume 1.

---

## 1. Contexto Narrativo e Estrutura Geral
- **Ambientação:** Descida do jovem Lobo (*Felipe*) das nuvens em direção à Terra enquanto uma tempestade começa a se formar.
- **Arena Piloto:** 1 cenário único (*Descida da Tempestade / Terraço*), utilizando a câmera e a HUD padrão de combate por turnos (`LAB-PVE-01` / `LAB-PVE-03`).
- **Estrutura de Ondas (`WaveV2`):**
  - **Onda 1 (Etapas 1 a 4 — Fundamentos):** 1 adversário provisório (**Eco da Tempestade — Centelha**) para ensinar seleção de alvo, ataque básico (`A1`), habilidade especial com recarga (`A3`), leitura de buffs/debuffs, barra de ação (`AP`) e contra-ataque passivo (`A2`).
  - **Onda 2 (Etapa 5 — Consolidação):** 2 adversários provisórios (**Eco da Tempestade — Rajada** e **Eco da Tempestade — Trovão**) exigindo combinar prioridade de alvo, leitura da barra de ação, dissipação de buff comum e uso de habilidades especiais para concluir o tutorial.

---

## 2. Parâmetros Numéricos Fechados (Escala de Laboratório `8.000–14.000 HP`)
*Valores calibrados na base comum dos laboratórios para evitar lutas longas ou instantâneas:*

### Aliado Jogável (Protagonista do Tutorial)
| ID | Nome | HP Máx. | ATK | DEF | SPD | Kit Utilizado na Demo |
|---|---|---|---|---|---|---|
| `CHAR_FELIPE_JOVEM` | **Jovem Lobo (Felipe)** | `11.500` | `1.350` | `950` | `115` | `A1` (*Garra do Asfalto*), `A2` (*Postura de Vigília* — pré-carga `4/5` no modo tutorial), `A3` (*Eclipse na Floresta de Pedra*, `CD 4`) |

### Inimigos Provisórios (Manifestações da Tempestade — Não Canônicos)
| Onda | ID | Nome de Exibição | HP Máx. | ATK | DEF | SPD | Comportamento / Objetivo Didático |
|---|---|---|---|---|---|---|---|
| **Onda 1** | `MOB_TEMPESTADE_01` | **Manifestação da Tempestade (Centelha)** | `8.500` | `850` | `700` | `102` | Aplica debuff leve de redução de velocidade (`-15% SPD`, 2 turnos) para ensinar leitura de status e barra de ação (`AP`), além de ativar a última carga (`4/5 → 5/5`) da `A2` do Felipe. |
| **Onda 2** | `MOB_TEMPESTADE_02` | **Manifestação da Tempestade (Rajada)** | `9.200` | `920` | `740` | `118` | Age rápido na barra de `AP` e aplica `1` buff comum de ataque (`+20% ATK`, 2 turnos) em si mesmo, ensinando o jogador a priorizar alvos e dissipar buffs com a `A3`. |
| **Onda 2** | `MOB_TEMPESTADE_03` | **Manifestação da Tempestade (Trovão)** | `10.800` | `1.050` | `820` | `96` | Alvo mais resistente que valida dano contínuo (*Sangramento* da `A1`) e fechamento da onda. |

---

## 3. Roteiro Guiado das 5 Etapas (Textos Curtos para HUD / `Grupo 10`)
1. **Etapa 1 — Introdução e Ataque Básico (`TUT_BP_01`):**
   - *Foco:* Câmera de batalha, identificação dos lados e seleção de alvo único.
   - *Texto HUD:* `"A tempestade está fechando. Selecione a Manifestação à frente e use Garra do Asfalto (A1) para abrir caminho."`
2. **Etapa 2 — Habilidades Especiais e Recarga (`TUT_BP_02`):**
   - *Foco:* Uso de habilidade especial e visualização do tempo de recarga (`Cooldown`).
   - *Texto HUD:* `"Habilidades especiais têm maior impacto, mas exigem turnos de recarga. Use Eclipse na Floresta de Pedra (A3) para dissipar efeitos e atingir a área."`
3. **Etapa 3 — Efeitos Positivos, Negativos e Postura Reativa (`TUT_BP_03`):**
   - *Foco:* Reconhecer ícones de buff/debuff e entender que não há botão manual de defesa (resposta tática via passiva/habilidades).
   - *Texto HUD:* `"Fique atento aos ícones de estado. Ao ser atingido, Postura de Vigília (A2) acumula cargas e contra-ataca automaticamente ao atingir 5/5."`
4. **Etapa 4 — Ordem dos Turnos e Barra de Ação (`TUT_BP_04`):**
   - *Foco:* Leitura da barra de ação (`AP`) e impacto da Velocidade (`SPD`).
   - *Texto HUD:* `"A barra de ação mostra quem age primeiro. Reduções de velocidade atrasam o próximo turno — planeje seu alvo antes do golpe inimigo."`
5. **Etapa 5 — Conclusão e Retorno à Busca (`TUT_BP_05`):**
   - *Foco:* Vencer a Onda 2 aplicando todas as mecânicas anteriores.
   - *Texto Vitória:* `"O céu começa a abrir. Com o caminho livre, o jovem Lobo segue descendo em busca da bola perdida."`

---

## 4. Condições de Vitória, Derrota e Regras de Borda
- **Vitória:** Derrotar todos os inimigos das Ondas 1 e 2 (`WaveV2`: limpeza de efeitos comuns e cura de `30%` do HP máximo entre a Onda 1 e a Onda 2).
- **Derrota / Empate:** HP do Jovem Lobo chegar a `0` ou atingir o limite técnico de `200` ações.
- **Recompensa de Sessão:** Apenas conclusão local de tutorial (`progress-v1.save`), sem economia real ainda.
## Decisão posterior — Sangramento inicial no teste

Aprovado pelo usuário: o inimigo da demonstração de A1 já inicia com Sangramento, satisfazendo a condição de debuff preexistente. A regra geral da A1 não muda, nem os inimigos fora desse tutorial passam a iniciar com debuff. O efeito usa as regras normais de Sangramento; a sequência guiada deve garantir sua presença antes da A1 e respeitar ticks, duração e dano. Esta é preparação exclusiva do encontro de teste, não uma aplicação garantida de novo Sangramento pela A1 (acerto, resistência e imunidades continuam válidos). Contrato de preparação: data/encounters/tutorial-bola-perdida-preparation-v1.json. Cenário equivalente aprovado em teste nativo com o kit atual; o novo tutorial narrativo ainda não foi integrado.

