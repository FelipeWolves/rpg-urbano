# Entrega — Áudio, Música e Eventual Modo DJ (P2/P3)

**Estado Atual:** Direção musical definida; runtime de mixagem, eventos sonoros e modo rítmico (`EXP-01`) aguardam especificação fechada antes de implementação.

---

## 1. Formato de Recursos, Licenciamento e Ciclo de Loop
**DECISÃO DESEJADA (Pipeline de Ativos Sonoros):**
- **Formato Matriz vs. Empacotamento:**
  - *Fonte de Trabalho (`assets/sources/` — fora do Git):* Arquivos em **WAV (48 kHz / 24-bit)** com stems/projetos editáveis preservados.
  - *Empacotamento Runtime (Unreal Engine 5.8.4):* Compressão gerenciada pela própria engine (**Ogg Vorbis / Bink Audio** para músicas longas em streaming e **ADPCM / PCM comprimido** para efeitos curtos de baixa latência).
- **Direitos e Autoria:** Todas as faixas musicais e efeitos sonoros (`SFX`) utilizados no projeto serão **composições autorais exclusivas** ou provenientes de **bibliotecas com licença comercial comprovada** (registrada junto aos metadados da faixa), sem extração ou reaproveitamento de áudios de jogos de terceiros.
- **Estrutura de Loop das Trilhas:**
  - Cada trilha de fundo (`BGM`) deve declarar explicitamente: `DuraçãoTotal`, `PontoDeIntro` (trecho inicial tocado uma vez) e `PontosDeLoop` (`LoopStart` -> `LoopEnd` sem corte seco), permitindo transições suaves via *crossfade* de `1.0s` a `1.5s`.

---

## 2. Mixagem, Grupos de Volume, Mute e Segundo Plano
**DECISÃO DESEJADA (Hierarquia de Canais `SoundClass` / `SoundMix`):**
- **5 Grupos Independentes de Volume (0% a 100% + Botão Mute Individual):**
  1. **Master (Geral):** Controla o ganho global do aplicativo.
  2. **Música (`BGM`):** Trilhas do Hub, exploração de livros, combates padrão, batalhas de chefe e eventos em Modo Clássico.
  3. **Efeitos Sonoros (`SFX`):** Impactos de golpes, bloqueios, magias, transições de fase e feedbacks de turno.
  4. **Interface (`UI`):** Cliques, navegação de menus, seleção de alvos, confirmação clássica e abertura de painéis.
  5. **Vozes / Dublagem (`VO`):** Falas em cenas, gritos de ação em combate e narrações dos capítulos.
- **Prioridade e *Ducking* (Atenuação Dinâmica):**
  - **Prioridade Máxima:** `VO` (Vozes) e alertas críticos de combate (`Limit Break`, `Supressão`, `Derrota`) -> aplicam *ducking* automático de `-25%` a `-35%` no volume da `BGM` enquanto ativos, restaurando suavemente ao terminar.
  - **Polifonia Limitada:** Efeitos repetitivos simultâneos (ex.: multi-hit rápido) compartilham limite de vozes concorrentes (`MaxConcurrency = 4`) para evitar saturação/clipping.
- **Comportamento em Segundo Plano e Interrupções (Mobile / PC):**
  - *App em segundo plano (`onPause` / minimizado) ou chamada telefônica:* Silencia/pausa imediatamente todos os canais de áudio (`BGM`, `SFX`, `VO`), retomando automaticamente apenas quando o aplicativo voltar ao primeiro plano ativo.
  - *Chave física de Silencioso (iOS/Android):* Respeita o estado mudo do hardware do dispositivo por padrão.

---

## 3. Tabela de Eventos Disparadores de Som (Equipes e Modo Clássico)
**DECISÃO DESEJADA (Mapeamento Evento -> Som — Puramente Reativo, Sem Alterar Regras):**

| Categoria | Evento de Jogo | Comportamento Sonoro Esperado |
| :--- | :--- | :--- |
| **Hub e Menus** | Entrada no Hub (`Mundus Lupis Disco`) | Inicia trilha noturna urbana ambiente (`BGM_Hub`) em loop contínuo. |
| **Hub e Menus** | Clique / Toque válido (`UI_Confirm`) | Som tátil limpo de confirmação; clique em acesso bloqueado dispara som sutil de trava (`UI_Locked`). |
| **Hub e Menus** | Abertura / Fechamento de Modal | *Whoosh* suave sincronizado com a transição de `150ms–200ms` (reduzido no modo *Sem Movimento*). |
| **Combate (Comum)** | Início de Turno / Barra Cheia (`AP` ou `ATB Ready`) | Pulso sonoro discreto indicando oportunidade pronta (mais agudo no **Modo Clássico** ao encher o ATB). |
| **Combate (Comum)** | Execução de Habilidade (`Notify_Start` / `Impact`) | Som de preparação no início e som de impacto elemental no `Notify_Impact` (varia para Acerto Normal, Crítico ou *Miss* abafado). |
| **Combate (Comum)** | **Bloqueio Automático** (`Auto_Block`) & **Defesa Clássica** | Impacto metálico/denso de aparar (`SFX_BlockDeflect`), distinto do som de dano direto ou escudo quebrando. |
| **Combate (Comum)** | Aplicação de Buff / Debuff / Controle | Timbres distintos por categoria: harmônico para buffs (`Green`), grave/tenso para debuffs/controle (`Red/Silver`), ressonante para estados únicos (`Golden`). |
| **Modo Clássico** | Cursor no Menu Clássico & **Limit Break Ready** | Som clássico de cursor de menu JRPG ao navegar entre *Atacar / Habilidades / Defender / Itens / Especial* + fanfarra curta quando a barra de Limite atinge `100%`. |
| **Encerramento** | Vitória / Derrota / Mudança de Fase de Chefe | Transição imediata da `BGM` de batalha para tema de vitória, derrota ou fase crítica do chefe. |

---

## 4. Modo DJ / Rítmico (`EXP-01`) — Narrativa de Purificação e Mecânica Autoral
**DECISÃO DESEJADA / PROPOSTA (Identidade *Mundus Lupis Disco* + Mecânicas Inspiradas em *Pump It Up*, *Taiko* e *BeastBeat*):**

### 4.1 Papel Narrativo no Universo e Desbloqueio de Faixas
- **Divisão de Função na História:** Enquanto **Felipe (O Lobo)** vivencia os contos diretamente na cidade e nos outros mundos, um **personagem curador dedicado ao estúdio/hub (*Mundus Lupis Disco*)** atua como o intérprete capaz de **ler a frequência emocional** deixada pelos capítulos e **purificar memórias/discos corrompidos** (podendo intercalar desafios musicais com batalhas de purificação em eventos especiais).
- **Regra Explícita de Desbloqueio de Músicas:**
  - **Por Posse do Personagem (Regra Principal):** Cada personagem jogável possui sua faixa-tema oficial; **obter/desbloquear o personagem na conta libera permanentemente sua música** no catálogo do Modo DJ.
  - **Por Progressão e Eventos:** Faixas temáticas dos **4 Livros** são desbloqueadas ao concluir capítulos-chave das 3 Linhas Narrativas (*Luminosa, Crepuscular, Sombria*), enquanto faixas sazonais e vencedoras de concursos comunitários entram em rotação de evento.

### 4.2 Gameplay, Layouts e Tipos de Notas
- **Layouts de Pista Selecionáveis:**
  - *Padrão Urbano (3 pistas no Mobile `9:16` / 4 pistas no PC):* Focado em leitura fluida e ergonômica para polegares ou teclado (`D / F / J / K`).
  - *Modo Diagonal / Passo Cruzado (5 zonas — 4 cantos + centro, inspirado em **Pump It Up**):* Modo opcional/avançado no teclado (`Q / E / S / Z / C`) ou toque multi-zona com padrões cruzados e maior multiplicador de pontuação.
- **Tipos de Notas (Inspirados em *Taiko no Tatsujin* e *BeastBeat*):**
  1. **Batida Simples (`Tap` — Azul Celestial):** Toque simples no pulso da música.
  2. **Batida Pesada / Dupla (`Heavy` — Âmbar Dourado):** Exige toque simultâneo de dois dedos/teclas no contratempo grave, concedendo bônus de purificação.
  3. **Sustentada / Deslize (`Hold / Slide` — Púrpura):** Manter pressionado ou deslizar seguindo a melodia.
  4. **Interferência / Ruído (`Dissonance` — Carmesim):** Nota de corrupção da memória que **deve ser evitada** (não tocar preserva a integridade da purificação).

### 4.3 Recompensas, Loja de Dados Sonoros, Timing e Latência
- **Recompensas do Modo DJ (Escalonadas por Maestria Musical e Sem Ouro Genérico):**
  - *Escalonamento por Progressão Rítmica:* O volume de **Estamina direta**, **Dados Sonoros (*Sound Data*)** e fragmentos recebidos cresce progressivamente de acordo com o **Nível de DJ da conta**, a **quantidade total de músicas concluídas** e o **nível/rank mais alto alcançado** nas faixas. *(Não entrega Ouro comum nem altera a velocidade de regeneração natural da Estamina).*
  - *Por Marco de Pontuação (`Rank S / SS / Full Combo`):* Recompensas únicas por faixa e dificuldade (fragmentos adicionais, moeda premium e ícones).
- **Loja de Dados Sonoros & Amplificadores Automáticos:**
  - **Loja Dedicada:** Os *Dados Sonoros* são trocados em loja própria por **itens cosméticos temáticos** (visuais, efeitos de palco/interface, acessórios) que também concedem **bônus funcionais em batalhas e eventos** e elevam o **nível de Estilo** da conta.
  - **Buffs Automáticos e Renováveis:** Os amplificadores temporários (taxa de drop de equipamentos e ganho de experiência/recursos) e os pequenos bônus permanentes de Círculos Narrativos são **ativados e renovados automaticamente fora do fluxo de resgate de recompensas** (sem obrigar o jogador a microgerenciar ativações manuais a cada partida).
- **Loja de Dados Sonoros & Amplificadores Automáticos:**
  - **Loja Dedicada:** Os *Dados Sonoros* são trocados em loja própria por **itens cosméticos temáticos** (visuais, efeitos de palco/interface, acessórios) que também concedem **bônus funcionais em batalhas e eventos** e elevam o **nível de Estilo** da conta.
  - **Buffs Automáticos e Renováveis:** Os amplificadores temporários (taxa de drop, experiência/recursos, regeneração de estamina) e os pequenos bônus permanentes de Círculos Narrativos são **ativados e renovados automaticamente fora do fluxo de resgate de recompensas** (sem obrigar o jogador a microgerenciar ativações manuais a cada partida).
- **Janelas de Timing, Dificuldades e Latência:**
  - *Tolerâncias:* **Perfeito (`±45 ms` — 100%)**, **Ótimo (`±90 ms` — 70%)**, **Bom (`±140 ms` — 40%)** e **Erro (`> 140 ms` — quebra combo)**.
  - *Dificuldades:* *Fácil*, *Normal*, *Difícil* e *Mestre* (com ajuste livre de `Scroll Speed`).
  - *Sincronismo:* Guiado pelo **Sample Clock / DSP do áudio** com **Calibrador de Latência (`Audio Offset` de `-250 ms` a `+250 ms`)**.


---

## 5. Separação entre Trilha de Vídeo, Voz e Localização
**DECISÃO DESEJADA (Arquitetura Desacoplada para Multilíngue):**
- **Vídeo vs. Áudio/Legendas:**
  - Os arquivos de vídeo (`MP4/H.264`) contêm a imagem limpa e a trilha/efeitos sonoros base (ou faixas de áudio selecionáveis), **nunca texto queimado na imagem**.
  - As **legendas** (`PT-BR` padrão e `EN` futuro) são carregadas dinamicamente a partir das tabelas de localização da Unreal Engine (`String Tables`), permitindo traduzir ou corrigir textos sem re-renderizar nenhum vídeo.
- **Estratégia de Vozes (`VO`):**
  - Na fase atual/MVP, o jogo opera com **textos e legendas localizados** acompanhados de efeitos sonoros expressivos e trilhas atmosféricas; caso dublagem completa seja adicionada em etapa posterior, os arquivos de voz serão organizados em pastas por idioma (`Audio/VO/pt-BR/`, `Audio/VO/en/`) vinculados ao mesmo ID de diálogo.


## 6. Arquitetura Econômica, Lojas Separadas e Ecossistema
**DECISÃO DESEJADA / PROPOSTA (Economia Modular Sem Inflação de Moeda Básica):**
- **Papel do Ouro (Moeda Básica):** Concentrado na Campanha, Desafios PvE e missões diárias como moeda base de aprimoramento. **Não é concedido** no Modo Clássico, Modo DJ/Rítmico nem no PvP, que operam com moedas dedicadas.
- **Lojas Separadas por Especialidade (Backend Futuro):**
  1. **Loja de Ouro:** Atualiza automaticamente a cada **2 horas**, oferecendo materiais básicos de evolução, peças iniciais e consumíveis comuns.
  2. **Loja de Forja e Atributos:** Dedicada a pedras de melhoria, recalibração e materiais de aprimoramento de equipamentos.
  3. **Loja de Fichas Clássicas:** Exclusiva para trocar **Fichas do Modo Clássico** por itens de evento, consumíveis clássicos e recompensas sazonais.
  4. **Loja de Dados Sonoros:** Troca *Dados Sonoros* do Modo DJ por cosméticos com bônus de batalha/evento e pontos de Estilo.
  5. **Loja de PvP (Inspirada na Loja de Campeonato):** Concentra tanto **equipamentos/itens exclusivos válidos apenas nas batalhas PvP** quanto **recursos raros e valiosos para fortalecimento de personagens de alta raridade** (trocados por Pontos de Arena/Ranking).
- **Sistema de Conversão de Excedentes (*Endgame Recycling*):** Para evitar acúmulo inútil de moedas no fim de jogo (como excesso de Ouro), determinadas lojas oferecerão abas de conversão limitada permitindo trocar excedentes de moedas básicas por vales/itens de outras lojas, mantendo a utilidade contínua de todos os recursos.
- **Círculos Narrativos, Comunidade e Lojas Regionais:**
  - Concluir 100% de um **Círculo de Personagens Conectados** no *Guia dos Livros* concede **expansão permanente de inventário** (`+100` a `+200` slots).
  - Campanhas comunitárias de música (faixas jogáveis no Modo DJ) e arte (exibição no **Outdoor do Hub**).
  - Publicação planejada também na **Samsung Galaxy Store** (Android) e **Epic Games Store** (PC) para preços regionais e cupons mais acessíveis no Brasil.








