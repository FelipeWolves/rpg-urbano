# Entrega — Universo, Narrativa, Tutorial e Localização (P2)

**Decisões Já Fechadas (Preservadas):**
- **Identidade:** *Mundus Lupis Disco* / codinome `LOBO`, fundamentado na série literária *Contos de Um Lobo na Cidade*.
- **Idioma Base:** Português do Brasil (`pt-BR`) como idioma nativo do produto (mantendo o ambiente de desenvolvimento da Unreal Engine 5.8.4 em inglês).

---

## 1. Premissa, Tom, Mundo, Facções e Elenco Original (3–5 Personagens)
**DECISÃO DESEJADA (Cânone Literário Autoral — Sem Nomes ou Personagens de Terceiros):**

- **Premissa e Mundo:**
  - Nas noites chuvosas de uma metrópole contemporânea onde o concreto, as luzes neon e os ecos musicais escondem travessias entre planos emocionais e mundos paralelos, um lobo antropomórfico caminha entre humanos e seres despertos. Cada encontro, memória ou conflito não resolvido ecoa como uma frequência na cidade — e cada conto vivido é catalogado nos **4 Livros Fechados** (67 capítulos no total) e nas **3 Linhas Narrativas** (*Luminosa*, *Crepuscular* e *Sombria*).
- **Tom Narrativo:**
  - **Noir Urbano & Realismo Fantástico Melancólico:** Adulto, introspectivo e cinematográfico (estética ultra-realista), equilibrando acolhimento poético (*Linha Luminosa*), mistério cotidiano (*Linha Crepuscular*) e confrontos trágicos de superação (*Linha Sombria*).
- **Facções / Correntes do Universo:**
  1. **Os Errantes da Vigília (Caminhantes da Cidade):** Seres despertos e observadores que protegem o equilíbrio silencioso entre a metrópole humana e os ecos espirituais sem buscar domínio.
  2. **O Círculo do Disco (*Mundus Lupis Disco*):** Curadores e intérpretes sonoros que leem, preservam e purificam as memórias emocionais deixadas pelos contos em forma de frequências musicais.
  3. **Dissonâncias / Sombras do Asfalto (Antagonistas):** Manifestações distorcidas de traumas, obsessões e ruídos emocionais que corrompem lembranças e provocam rupturas na cidade.
- **Elenco Original da Primeira Experiência (5 Personagens Autorais):**
  1. **Felipe, O Lobo da Cidade (`WOLF_FELIPE` — Protagonista / Observador):** Lobo antropomórfico urbano de olhar atento e postura reservada. Na primeira experiência, conduz o jogador pela cidade e alterna suas **Fases de Sintonia Elemental** (*Umbra/Sombra*, *Luz/Shimmer* e *Natureza/Verde*) conforme o avanço nas Linhas Narrativas.
  2. **O Filhote (`WOLF_PUP` — Aliado / Protegido):** Jovem companheiro de espírito curioso e resiliente que acompanha Felipe nas travessias urbanas; possui progressão própria de **Fases Elementais** vinculada aos laços construídos nos capítulos.
  3. **Michel (`CHAR_MICHEL` — Aliado / Elo Urbano):** Figura próxima e estratégica dentro da trama cotidiana da cidade, atuando como suporte tático e ponto de ancoragem humana/urbana nos primeiros contos.
  4. **O Dálmata (`CHAR_DALMATIAN` — Aliado / Companheiro de Vigília):** Aliado leal de temperamento marcante que integra os arcos conectados do *Guia dos Livros*, também dotado de **Fases de Sintonia Elemental** desbloqueáveis pela campanha.
  5. **O Curador das Frequências (`NPC_CURATOR` — Guia do Estúdio Sonoro / Hub):** Personagem residente do *Mundus Lupis Disco* responsável por ler os ecos emocionais dos capítulos concluídos e conduzir a purificação musical no **Modo DJ** (sem substituir o protagonismo de Felipe nos livros).
  
  - **Regra de Escalabilidade do Elenco e Proteção dos Manuscritos:**
  - Os 5 personagens acima compõem o **elenco piloto da primeira experiência**. Os demais personagens já documentados nos **4 Livros** entrarão em lotes incrementais nas expansões seguintes, sem bloquear o MVP.
  - **Os textos integrais dos 4 livros originais não são inseridos neste documento nem no repositório de código:** a obra literária permanece protegida no acervo do autor, sendo adaptada progressivamente para o jogo apenas na forma de sinopses, roteiros curtos de diálogo e chaves de localização (`StringTables`).

---

## 2. Roteiro Curto de Abertura e Primeiro Encontro / Tutorial
**PROPOSTA / DECISÃO DESEJADA (Fluxo Jogável da Primeira Fatia — ~3 a 5 Minutos):**

- **Cena 1 — Abertura Curta (Vídeo / Transição Atmosférica):**
  - *Visual:* Chuva fina refletindo luzes azul-celeste e âmbar no asfalto noturno. Felipe observa a metrópole do alto do *Terraço da Vigília* com o comunicador iluminado na mão.
  - *Legenda (`pt-BR`):* *"Toda cidade guarda histórias que ninguém ousa contar em voz alta. Algumas pedem apenas alguém que escute. Outras exigem que a gente enfrente o que ficou preso na sombra."*
  - *Transição:* Segue para o **Hub Principal**, destacando o pulso suave no dispositivo em sua mão (**Batalha**).
- **Cena 2 — Primeiro Encontro / Tutorial Guiado (Capítulo 1 — Livro 1):**
  - *Cenário:* *Terraço da Vigília / Mirante Noturno* (arena piloto).
  - *Objetivo Pedagógico (3 Passos Sem Textos Longos):*
    1. **Turno 1 — Barra de Ação (`AP`) e Habilidade Básica (`A1`):** O tutorial destaca a barra de velocidade/ação no topo e orienta o toque/clique no inimigo (*Eco de Dissonância*) para executar `A1 — Garra do Asfalto`.
    2. **Turno 2 — Habilidades Táticas (`A2 / A3`), Escudo e Bloqueio:** O inimigo prepara um golpe; Felipe utiliza `A2 — Olhar da Garoa` (aplicando escudo padrão de `1%` do Max HP com mínimo de 1 ponto) e demonstra a mitigação/contra-ataque.
    3. **Turno 3 — Encerramento e Registro no Livro:** O jogador finaliza o encontro com `A3 — Eclipse na Floresta de Pedra`. O sistema exibe a tela de **Vitória**, registra o **Capítulo 1 no Guia do Livro (Vol. 1)** e marca o primeiro ponto na **Linha Crepuscular**.

---

## 3. Catálogo Inicial de Textos Localizáveis (`pt-BR` Primeiro, `en` Futuro)
**DECISÃO DESEJADA (Chaves de `StringTable` Prontas para Uso na UI):**

| Chave de Localização (`ID`) | Texto Pronto em Português (`pt-BR` — Prioritário) | Tradução Planejada (`en` — Futuro) |
| :--- | :--- | :--- |
| `UI_HUB_TITLE` | `MUNDUS LUPIS DISCO` | `MUNDUS LUPIS DISCO` |
| `UI_HUB_BTN_BATTLE` | `BATALHA` | `BATTLE` |
| `UI_HUB_BTN_EVENT` | `EVENTO ESPECIAL` | `SPECIAL EVENT` |
| `UI_HUB_BTN_BOOKS` | `GUIA DOS LIVROS` | `BOOK CODEX` |
| `UI_HUB_BTN_ROSTER` | `ELENCO E SINTONIAS` | `ROSTER & ATTUNEMENT` |
| `UI_HUB_BTN_DJ` | `ESTÚDIO SONORO (MODO DJ)` | `SOUND STUDIO (DJ MODE)` |
| `UI_HUB_BTN_SETTINGS` | `CONFIGURAÇÕES` | `SETTINGS` |
| `UI_HUB_EVENT_MODAL_TITLE` | `EVENTO ESPECIAL — EM PREPARAÇÃO` | `SPECIAL EVENT — IN PREPARATION` |
| `UI_HUB_EVENT_MODAL_BODY` | `Área reservada para a linha de eventos especiais em Modo Clássico e desafios temporários do universo Contos de Um Lobo na Cidade.` | `Area reserved for the Classic Mode special event line and temporary challenges from the Tales of a Wolf in the City universe.` |
| `UI_HUB_LOCKED_SOON` | `EM BREVE — CAPÍTULO EM PREPARAÇÃO` | `COMING SOON — CHAPTER IN PREPARATION` |
| `UI_HUB_ONLINE_ONLY` | `DISPONÍVEL APENAS NA VERSÃO CONECTADA` | `AVAILABLE IN CONNECTED VERSION ONLY` |
| `UI_COMBAT_VICTORY` | `VITÓRIA — CONTO PRESERVADO` | `VICTORY — TALE PRESERVED` |
| `UI_COMBAT_DEFEAT` | `DERROTA — A FREQUÊNCIA SE DISPERSOU` | `DEFEAT — THE FREQUENCY DISPERSED` |
| `UI_CLASSIC_CMD_ATTACK` | `Atacar` | `Attack` |
| `UI_CLASSIC_CMD_SKILLS` | `Habilidades` | `Skills` |
| `UI_CLASSIC_CMD_DEFEND` | `Defender` | `Defend` |
| `UI_CLASSIC_CMD_ITEMS` | `Itens` | `Items` |
| `UI_CLASSIC_CMD_LIMIT` | `Especial` | `Limit` |
| `UI_TUTORIAL_STEP_01` | `Quando a barra de ação encher, toque no adversário para atacar.` | `When the action bar fills, tap the opponent to attack.` |
| `UI_TUTORIAL_STEP_02` | `Use Olhar da Garoa para projetar um escudo e proteger a equipe.` | `Use Drizzle Gaze to project a shield and protect the team.` |

---

## 4. Recorte de Escopo: Primeira Versão (MVP) vs. Expansões Futuras
**DECISÃO DESEJADA (Controle de Escopo Sem Perder a Visão Completa):**

- **O Que Cabe na Primeira Versão (Fatia Jogável / MVP Local):**
  - **Hub Principal:** Panorama responsivo (`16:9` e `9:16`) com os 2 acessos ativos (**Batalha** e painel provisório de **Evento Especial**) e painel de **Configurações**.
  - **Combate:** Núcleo determinístico validado em **Modo Padrão de Equipes** (com suporte técnico preservado ao perfil **Modo Clássico** nos cenários de teste), 1 arena piloto (*Terraço da Vigília*) e o kit piloto de **Felipe** (junto ao elenco mínimo de teste).
  - **Narrativa & Save:** Vídeo de abertura com rotação responsiva e legendas `pt-BR`, tutorial jogável do Capítulo 1 e persistência local offline (`Save v1`).
- **O Que Entra Como Expansão Posterior (Fases Seguintes):**
  - **Campanha Completa:** Todos os **67 capítulos** distribuídos nos **4 Livros**, seletor completo das **4 dificuldades** (com variações narrativas no *Difícil* e *Pesadelo/Eclipse*), **Linhas Narrativas**, **Fases de Sintonia Elemental** e **Círculos Narrativos** (expansão de inventário).
  - **Modos Especiais:** Linha pública de eventos em **Modo Clássico (PS1/PS2)**, **Estúdio Sonoro / Modo DJ (`EXP-01`)** com *Dados Sonoros*, chefes avançados (`SEQ-01`, `PAR-02`, *Treva Fluxo*) e **Arena PvP** com ranking diário.
  - **Ecossistema Online:** Servidor autoritativo, **5 Lojas Separadas** + reciclagem de excedentes, localização completa em inglês (`en`) e campanhas comunitárias.

---

## 5. Separação: Inspiração Externa vs. Decisão Autoral e Direitos de Imagem
**REFERÊNCIA / AUDITORIA DE PROPRIEDADE INTELECTUAL:**

| Elemento | Inspiração Externa (Conceito de Gênero) | Decisão Autoral do Nosso Projeto (100% Original) |
| :--- | :--- | :--- |
| **Estrutura de Combate PvE/PvP** | Combate tático por barra de velocidade (`AP`), 3 habilidades por unidade e relíquias (*Dislyte*). | Cânone literário próprio, fórmulas próprias (`StandardShield` 1% Max HP, causalidade `R04`), e sistema de **Fases de Sintonia Elemental** ligadas aos livros. |
| **Modo Clássico (Eventos)** | Menu de 5 comandos, *ATB Active/Wait* e *Limit Break* dos JRPGs de PS1/PS2 (*Final Fantasy*). | Implementado como linha de evento especial separada (`-CombatProfile=Classic`), com economia própria (*Fichas Clássicas*). |
| **Modo DJ / Purificação** | Dinâmica narrativa de leitura musical (*Discboom*) e mecânicas de *Pump It Up* / *Taiko* / *BeastBeat*. | Protagonizado por curador próprio do *Mundus Lupis Disco*, integrado a **Dados Sonoros**, buffs automáticos e músicas-tema dos personagens dos livros. |
| **Universo, Texto e Personagens** | Nenhuma importação de nomes, lore ou personagens de terceiros. | **100% obra literária autoral** (*Contos de Um Lobo na Cidade*, de Felipe Sena Pereira). |

- **Origem e Licença das Ilustrações e Mídias:**
  - *Textos, Roteiros e Livros:* Autoria exclusiva de **Felipe Sena Pereira** (direitos integrais do autor).
  - *Panorama do Hub e Vídeos de Abertura:* Acervo próprio dirigido/gerado pelo autor em seus fluxos autorais (uso liberado).
  - *Silhueta Provisória do Lobo (`klemzo123_silhouette-wolf-2...png`):* Mantida exclusivamente como placeholder temporário de desenvolvimento local; **substituição obrigatória** por logotipo vetorial 100% autoral antes de qualquer build comercial ou distribuição pública.
  - *Modelos 3D Futuros (`ART-01`):* Serão produzidos a partir das referências do autor via ferramentas 3D com **licença comercial comprovada** arquivada em `assets/sources/`.