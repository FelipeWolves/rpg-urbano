# Respostas Consolidadas aos Pontos de Esclarecimento (ENT-01 a ENT-15)

**Data:** 20/11/2026  
**Objetivo:** Fechar as divergências pontuais apontadas na revisão das 12 entregas para liberar os contratos `REG/SIS`, `VIS/HUB`, `AUT` e `CIC` sem ambiguidade.

---

### ENT-01 — Bloqueio (`Block` Universal) vs. Mitigação de Dano (`REG-01`)
- **Decisão (Duas Camadas Separadas):**
  1. **Bloqueio (`Block` — Chance Real de Anulação Completa):** Todo personagem possui uma **chance base universal de `3%`** de **bloquear completamente** o ataque/hit (anulando o dano direto, em continuidade total com o `Block` já validado nos cenários antigos e inspirado na mecânica da Anna). Sobre esses `3%` base somam-se bônus de habilidades/passivas (ex.: **Michel** inicia com `10%` total) ou equipamentos, respeitando teto de `50%` por fontes gerais e `100%` apenas sob estados especiais explícitos.
  2. **Mitigação de Dano (`Damage Mitigation` — Camada Independente):** Quando o ataque **não** é bloqueado, aplica-se separadamente a **Mitigação de Dano** (redução percentual de `30%`, podendo chegar a `50%` via passivas/buffs/equipamentos específicos), calculada **por hit** individual e combinada com `DEF` e (no Modo Clássico) com o comando `Defender`.

### ENT-02 — Felipe `A2` na Ficha vs. no Tutorial (`HAB / Tutorial`)
- **Decisão:** Manter a **Ficha de Felipe** como fonte autoritativa: `A2` (*Postura de Vigília*) é **passiva** (não selecionável manualmente; acumula cargas ao sofrer golpes e dispara contra-ataque/escudo automático).
- **Ajuste no Tutorial (Capítulo 1):**
  - Turno 1: Ensina `A1` (*Garra do Asfalto* — básica).
  - Turno 2: Felipe recebe um golpe do inimigo, demonstrando a ativação automática da passiva `A2` (gerando carga/escudo), e em seguida o jogador usa `A3` (*Eclipse na Floresta de Pedra* — ativa).

### ENT-03 — Escudo do Filhote (`15% / 2 turnos`) vs. `StandardShield` (`ESC-01–07`)
- **Decisão:** Manter `ESC-01–07` (`1%` do Max HP do aplicador, 3 turnos, piso 1) intacto como regra do **Escudo Padrão (`StandardShield`)**.
- O escudo do **Filhote** (`15%` do Max HP do aplicador, duração de `2` oportunidades naturais do protegido) é um **gerador especializado futuro** (*Escudo Reforçado / SilverShield*), a ser implementado em bloco próprio (`SIS-01`), sem alterar a fórmula do `StandardShield`.

### ENT-04 — Tecla `ESC`, Toque Curto vs. Longo e Fluxo de Pausa/Saída (`VIS / HUB`)
- **Decisão:**
  - **Ordem hierárquica do `ESC` (ou botão Voltar no Android):**
    1. Fechar tooltip/modal aberto -> 2. Cancelar seleção de alvo -> 3. Abrir **Menu de Pausa da Batalha** -> 4. Dentro do Menu de Pausa, escolher **Retomar** ou **Abandonar Batalha (Voltar ao Hub)**.
  - **Comportamento ao Abandonar no Meio:** Encerra a tentativa sem conceder vitória nem recompensa (e sem resetar o progresso já salvo anteriormente), retornando ao Hub.
  - **Toque / Clique:** Manter **2 passos separados** conforme contrato já validado: **toque curto** seleciona/confirma alvo ou ação; **toque longo / segurar (`hold`)** abre apenas o painel de inspeção/detalhes sem executar ação.

### ENT-05 — Recorte Mobile (`412×915`) e Posição dos Acessos na Imagem do Hub
- **Decisão:** Usar **enquadramento inteligente centrado no personagem/dispositivo com pan horizontal suave (ou reposicionamento responsivo das âncoras de UI sobre a imagem)**, **sem deformar** a arte `2752×1536` e sem exigir segunda imagem agora.
  - No retrato mobile (`9:16` / `412×915`), a câmera/viewport foca o sujeito principal com o dispositivo na mão (onde fica o acesso **Batalha**), enquanto o acesso **Evento Especial** (Outdoor) pode ser alcançado deslizando lateralmente o panorama do Hub ou por atalho discreto na barra superior/lateral segura.
  - *Correção de descrição:* Fica registrado que na arte atual do Hub o dispositivo iluminado está na mão do personagem humano em primeiro plano (mantendo a descrição fiel à imagem real).

### ENT-06 — Relógio `CarrierTurnEnd`, Supressão e `Freeze` (`REG-03`)
- **Decisão:**
  - **`CarrierTurnEnd`:** Conta **apenas turnos naturais** do portador (turnos extras concedidos por habilidades não consomem a duração de buffs/escudos).
  - **Supressão (`Suppression`):** Bloqueia ativação de passivas reativas e auto-revive enquanto ativa, **exceto** efeitos de morte já enfileirados antes da aplicação ou auras estruturais do chefe.
  - **Congelamento (`Freeze`):** Impede ações próprias, `Counter`, `Assist` e também **`Pursuit`** (ataques de perseguição extra) enquanto congelado; não congela o relógio natural do turno (o portador perde sua vez e o contador de duração reduz ao final dela).

### ENT-07 — Piso de Dano, `True Damage` e Funil de Acerto/Resistência (`SIS-02`)
- **Decisão:**
  - **Piso de Dano:** Golpes comuns que acertam e **não** foram anulados por `Invulnerability`, **Bloqueio (`Block` — anulação completa)** ou Defesa Clássica têm **piso mínimo de `1` ponto de dano**.
  - **Exceção Explícita da Defesa Clássica (`-CombatProfile=Classic`):** A Defesa Clássica aplica `floor(dano / 2)` **após** o cálculo base — portanto, um golpe de `1` ponto defendido no Modo Clássico resulta em **`0` de dano** (preservando integralmente o contrato clássico já implementado).
  - **`True Damage` (Dano Verdadeiro):** Ignora `DEF`, **Mitigação de Dano percentual** e a redução da Defesa Clássica, mas ainda é absorvido por **Escudos** ativos (`StandardShield` / `SilverShield`) antes de atingir o HP.
  - **Ordem do Funil (Contrato Futuro `SIS-02`):** `1. Imunidade` -> `2. Esquiva/Miss` -> `3. Bloqueio (Block — chance base 3%+ de anular o ataque)` -> `4. Crítico` -> `5. Mitigação de Dano (DEF / Redução Percentual / Defesa Clássica)` -> `6. Absorção de Escudo` -> `7. Aplicação no HP`.

### ENT-08 — Transição Entre Ondas (`WAVE-01`), Morte de Chefe e `Score` (`WAVE / SEQ / PAR`)
- **Decisão:**
  - **Entre Ondas (`WAVE-01`):**
    1. **Recuperação de Vida:** Personagens aliados **vivos** recuperam **`30%` do seu HP máximo** (`floor(MaxHP * 0.30)`, sem ultrapassar o `MaxHP` e sem reviver aliados derrotados na onda anterior).
    2. **Remoção de Buffs Comuns:** Todos os **buffs comuns (sem borda diferenciada)** são **sempre removidos** na transição de uma onda para outra, mesmo que ainda restem turnos de duração — alinhado à remoção já fechada do `StandardShield` (`ESC-06`). Apenas efeitos especiais com borda diferenciada (Prateada/Dourada), auras estruturais e cargas de passiva permanecem ativos, além das recargas (`cooldowns`).
  - **Morte de Chefe:** Passa obrigatoriamente pela fila de eventos `R04` (verificando revive/efeitos de morte antes de acionar o *Victory Lock*).
  - **Fórmula de Pontuação (`ScoreAttack` — Contrato Futuro):** `Score = floor(DanoEfetivo + (Overkill * 0.25)) + BonusSobrevivencia`, usando acumulador inteiro `int64` com teto máximo seguro definido no contrato do modo e entrega idempotente vinculada ao ID da tentativa.

### ENT-09 — Vídeo de Abertura: Pular, Retomar, Canal de Áudio e Estado Assistido
- **Decisão:**
  - **Pular:** Na **primeira exibição** da conta, exigir **segurar (`hold`) por `0,8s`** (evita toque acidental); ao **rever manualmente** pelo Hub, liberar **1 toque/clique** com botão `Pular`.
  - **Retomada após interrupção/segundo plano:** Retorna **pausado** com botão central `Continuar` (evita susto sonoro ou perda de quadro ao voltar para o app).
  - **Canal de Áudio:** Adicionar um **6º canal dedicado (`Mídia / Cutscenes`)** no mixer, ou vinculá-lo provisoriamente ao canal **Música** enquanto o mixer completo não existir.
  - **Marcação de Assistido:** Grava `IntroWatched = true` no save local tanto ao concluir o vídeo quanto ao confirmar o pulo (`skip`), mas **não** marca como assistido em caso de erro/arquivo ausente (para permitir nova tentativa após reparo).

### ENT-10 — Limpeza da Duplicação no Grupo 08 e Regra Oficial da Estamina
- **Decisão:** Fica oficializada a regra do **Grupo 09**: a Estamina possui **taxa fixa base de `+1` por minuto** (teto `100–300`) e **não** tem sua velocidade de regeneração alterada por buffs do Modo DJ. O Modo DJ entrega **pacotes diretos de Estamina** escalonados pelo Nível de DJ e maestria musical. O parágrafo antigo duplicado no Grupo 08 deve ser considerado obsoleto.

### ENT-11 — Recorte Exato da Demo Inicial (`MVP`)
- **Decisão:** Para a **primeira fatia jogável (Demo / Vertical Slice)**, implementar **apenas 1 personagem jogável completo (Felipe, O Lobo da Cidade)** na arena piloto **Terraço da Vigília** contra inimigos de teste (*Eco de Dissonância*). Os outros 4 personagens do elenco piloto (*O Filhote, Michel, O Dálmata e O Curador*) ficam registrados no catálogo narrativo para as entregas seguintes, sem bloquear o primeiro aceite visual.

### ENT-12 — Save: Regra de Conflito Nuvem vs. Local (Completando Frase do Grupo 09)
- **Decisão (Texto Completo):** Em caso de divergência entre save local e nuvem ao trocar de dispositivo:
  1. **Progresso de Conta (Livros, Moedas, Equipamentos, Músicas):** Prevalece sempre o registro **mais recente validado pelo servidor** (exibindo data/hora e nível da conta antes de sincronizar).
  2. **Preferências Locais (Resolução, Qualidade Gráfica, FPS, Volume, `Audio Offset`, Movimento Reduzido):** Permanecem **locais por dispositivo**, não sendo sobrescritas pelo save da nuvem.
  3. **Estado de Combate:** Permanece fora do Save v1 (sem snapshot de batalha em andamento no MVP).

### ENT-13 — Paleta Visual do Hub e Personalização Temática (`VIS / HUB`)
- **Decisão:**
  - **Base Padrão Preservada:** Manter a identidade visual já implementada no runtime em **tons de azul noturno, preto e prata** (`#0F172A` com 80% de opacidade, detalhes prateados e acentos luminosos em azul), preservando a tipografia (`Cormorant Garamond` + `Lora`) e os 2 acessos ativos já conectados, sem exigir reescrita imediata da paleta atual.
  - **Temas Dinâmicos e Personalização (Extensão Futura):** O sistema de interface deverá suportar **variações e temas visuais** que podem mudar ou ser personalizados:
    1. **Automaticamente pela História / Livros:** Adaptando o tom e a atmosfera do Hub conforme o **Livro** ou **Linha Narrativa** em andamento (*Luminosa*, *Crepuscular* ou *Sombria*).
    2. **Por Escolha Opcional do Jogador:** Permitindo selecionar temas desbloqueados (incluindo visuais adquiridos na *Loja de Dados Sonoros*).
    3. **Por Eventos Temporários:** Aplicando ambientações sazonais durante eventos especiais do jogo.

### ENT-14 — Modo Clássico (`SP / Limit / ATB`) e Modo DJ (`3/4/5 Pistas`)
- **Decisão (Registrada Apenas Como Especificação Futura):**
  - **Modo Clássico:** Comando `Defender` reduz o dano recebido pela metade (`floor(dano / 2)`) e restaura **`+15%` de SP** (barra de energia clássica para habilidades/Limit). `ATB Wait` pausa as barras enquanto o jogador navega nos submenus de Habilidades/Itens; `ATB Active` mantém o relógio correndo.
  - **Modo DJ:** Cada layout (`3`, `4` ou `5` pistas) possui **tabela de pontuação e ranking próprios** (sem misturar recordes de 3 pistas com 5 pistas). Tudo isso permanece fora do código atual até o contrato específico de cada modo.

### ENT-15 — Autoridade de Produção, Replay e Anticheat
- **Decisão:** Na futura versão comercial conectada, toda sessão de combate (PvE, Clássico, PvP) e Modo DJ será iniciada com uma **semente (`Seed`) e ID de tentativa emitidos pelo servidor**. Ao finalizar, o cliente envia a sequência de comandos/inputs executados; o servidor reexecuta a simulação determinística C++20 para validar o resultado antes de creditar moedas, equipamentos ou pontos de ranking.