# Entrega — Progressão, Economia, Equipamentos e Saves (P2/P3)

**Base Já Validada (Preservada):**
- Save v1 local entre batalhas (`CIC-01`), seleção de equipe (1–5) e entrega idempotente de recompensa de conclusão única existem no núcleo C++20; restauração de batalha em andamento, economia completa e inventário são extensões futuras[cite: 7].

---

## 1. Ciclo Completo do Jogador (Loop Principal e Desbloqueios)
**DECISÃO DESEJADA (Fluxo Integrado Entre Hub, Livros, Modos de Batalha e Estúdio Sonoro):**
1. **Entrada e Preparação (Hub `Mundus Lupis Disco`):** O jogador acessa o Hub, ajusta gráficos/acessibilidade ou ritmo ATB (*Wait/Active*), e escolhe a atividade:
   - **Campanha / Guia dos Livros (`BookCodex`):** Seleciona um dos **4 Livros** (67 capítulos no total) e o tier de dificuldade (*Normal, Difícil, Pesadelo, Eclipse*).
   - **Desafios PvE / Chefes:** Confrontos de ondas (`WAVE-01`), sequência de 3 times (`SEQ-01`), 3 frentes (`PAR-02`) ou pontuação (*Treva Fluxo*).
   - **Evento Especial — Modo Clássico (`-CombatProfile=Classic`):** Linha especial estilo JRPG PS1/PS2 com consumíveis de batalha e *Limit Break*.
   - **Estúdio Sonoro — Modo DJ (`EXP-01`):** Minigame rítmico de purificação com as músicas-tema dos personagens possuídos e das Linhas Narrativas.
   - **Futuro PvP:** Batalhas competitivas de arena com ranking diário.
2. **Consumo de Recurso:**
   - Modos de campanha e farm PvE consomem **Estamina** (apenas na conclusão bem-sucedida ou conforme regra do modo, evitando perda injusta por queda de conexão/erro).
   - Modo DJ, PvP e eventos especiais usam suas próprias cargas/ingressos diários ou entrada livre conforme o evento.
3. **Execução do Combate / Desafio:**
   - Resolvido pelo núcleo determinístico (com animação sincronizada por `AnimNotifies` e validação autoritativa no servidor em produção).
4. **Recompensa e Desbloqueios Encadeados:**
   - **Registro Narrativo:** Concluir capítulos preenche o **Guia do Livro** e avança as **3 Linhas Narrativas** (*Luminosa*, *Crepuscular*, *Sombria*), desbloqueando **Fases de Sintonia Elemental** (*Felipe*, *O Filhote*, *O Dálmata*) sem duplicar fichas no elenco.
   - **Desbloqueio Musical e de Inventário:** Obter um personagem libera sua faixa-tema no **Modo DJ**; fechar 100% de um **Círculo Narrativo** concede **expansão permanente de inventário** (`+100` a `+200` slots).
   - **Economia Especializada:** Cada modo entrega sua moeda específica para suas respectivas lojas.

---

## 2. Moedas, Estamina, Fontes, Sumidouros e Lojas Separadas
**DECISÃO DESEJADA / PROPOSTA (Economia Modular Sem Inflação de Ouro — Valores Iniciais Provisórios):**

| Recurso / Moeda | Limite & Recuperação | Fontes Principais | Sumidouros e Lojas Dedicadas |
| :--- | :--- | :--- | :--- |
| **Estamina (Energia PvE)** | Teto inicial provisório: `100` a `300` (sobe com nível da conta).<br>Regeneração: `+1` a cada `1 min` (taxa fixa base, sem aceleração por buffs do Modo DJ). | Regeneração natural, missões diárias, **entregas diretas do Modo DJ** (escalonadas por Nível de DJ e maestria) e itens de recarga. | Entrada em fases dos **4 Livros** e desafios PvE de equipamentos. |
| **Ouro (Moeda Básica)** | Sem teto rígido (`int64`). | Campanha (4 Livros), Desafios PvE e missões diárias.<br>**Regra:** **Não é entregue** no Modo Clássico, Modo DJ nem PvP. | Aprimoramento de equipamentos, evolução de personagens e compras na **Loja de Ouro** *(atualizada a cada **2 horas**)* e **Loja de Forja/Atributos**. |
| **Gemas / Cristal Lunar** *(Moeda Premium)* | Sem teto rígido. | Primeiras conclusões de capítulos/dificuldades, conquistas, eventos e compras opcionais (via Google Play, **Galaxy Store**, Steam e **Epic Games Store**). | Invocações de personagens, recargas pontuais e passes cosméticos. |
| **Fichas Clássicas** | Sem teto rígido (ou por temporada). | Exclusiva da **Linha de Evento Especial em Modo Clássico** (PS1/PS2). | **Loja de Fichas Clássicas:** Consumíveis clássicos, itens de evento, fragmentos e títulos exclusivos. |
| **Dados Sonoros (*Sound Data*)** | Sem teto rígido. | Exclusiva do **Modo DJ / Rítmico (`EXP-01`)** — quantidade escalonada pelo **Nível de DJ**, total de músicas concluídas e dificuldade/rank alcançados. | **Loja de Dados Sonoros:** Cosméticos temáticos que concedem **bônus funcionais em batalhas/eventos** e elevam o **Estilo** da conta, além de **buffs automáticos e renováveis** (drop e experiência/recursos) fora do resgate manual. |
| **Pontos de PvP** | Sem teto rígido. | **Recompensas diárias por rank/patente** (dos ranks iniciais até o topo) e vitórias na Arena PvP. | **Loja de PvP (Estilo Campeonato):** Concentra tanto **equipamentos/itens exclusivos válidos apenas nas batalhas PvP** quanto **recursos raros para fortalecer personagens raros**. |

- **Reciclagem de Excedentes no Fim de Jogo (*Endgame Conversion*):**
  - Lojas selecionadas terão abas de conversão com limite semanal/mensal permitindo que jogadores veteranos troquem moedas básicas acumuladas em excesso (como excesso de **Ouro**) por vales ou materiais de outras lojas, mantendo a utilidade contínua de todos os recursos.

---

## 3. Sistema de Equipamentos (Slots, Conjuntos, Exclusividade e Proteções)
**DECISÃO DESEJADA / PROPOSTA (Desacoplado das Habilidades Nativas — Inspirado em Relíquias + Itens PvP):**
- **6 Slots de Equipamento por Personagem (Divididos em 2 Grupos):**
  - *Grupo Primário (4 Slots — Sets de 4 peças):* **Arma** (ATK fixo), **Armadura/Casaco** (DEF fixa), **Visor/Amuleto** (HP fixo) e **Núcleo/Anel** (Atributo principal percentual: `ATK%`, `DEF%`, `HP%`, `Taxa Crítica%` ou `Dano Crítico%`).
  - *Grupo Secundário (2 Slots — Sets de 2 peças):* **Botas** (Atributo principal: `SPD` fixo ou `ATK%/DEF%/HP%`) e **Selo/Frequência** (Atributo principal: `ACC%`, `RESIST%` ou `HP%/DEF%`).
- **Sub-atributos e Melhoria:**
  - Até 4 sub-atributos secundários por peça, aprimorados com **Ouro** e materiais da **Loja de Forja** até o nível máximo (`+15`), com opção de recalibração de atributos.
- **Exclusividade e Restrições de Uso:**
  - *Equipamentos PvE/Gerais:* Livres para qualquer personagem (respeitando regra de não duplicar a mesma peça física em dois times simultâneos no mesmo encontro `SEQ-01` / `PAR-02`).
  - *Equipamentos Exclusivos de PvP (Loja de PvP):* Marcados com tag `PvPOnly = true` — ativam seus bônus e efeitos especiais **exclusivamente dentro das batalhas PvP**, permanecendo inativos em fases de Campanha ou Chefes PvE.
  - *Sintonia Elemental:* Os equipamentos permanecem equipados na ficha única do personagem (`WOLF_FELIPE`, etc.) mesmo quando o jogador alterna sua **Fase Elemental** (*Umbra / Luz / Natureza*).
- **Distinção Obrigatória Entre Proteções (Sem Misturar Fórmulas):**
  1. **DEF / RES (Atributo de Equipamento e Base):** Mitigação passiva aplicada diretamente na fórmula de cálculo de dano.
  2. **Defesa Clássica (`-CombatProfile=Classic`):** Comando ativo de turno no Modo Clássico que reduz o dano recebido pela metade (`floor(dano / 2)`) até ser consumido ou expirar na próxima oportunidade do dono.
  3. **Escudo por Habilidade (`StandardShield` — Contrato `ESC-01` a `ESC-07`):** Camada temporária de pontos de absorção sobre a barra de vida (`1%` do Max HP do aplicador na regra padrão, mínimo `1` ponto, 3 turnos), totalmente independente de DEF e da Defesa Clássica.

---

## 4. Persistência, O Que Salvar e Encerramento no Meio da Partida
**DECISÃO DESEJADA (Separação Entre Progresso Permanente e Estado Volátil de Batalha):**
- **O Que Deve Ser Salvo Permanentemente (Conta / Save):**
  - Progresso nos **4 Livros (67 capítulos)**, dificuldades concluídas, preenchimento das **Linhas Narrativas**, **Fases de Sintonia Elemental** desbloqueadas, músicas liberadas no **Modo DJ**, inventário de equipamentos e expansões de slots, saldos de moedas, predefinições de equipes e configurações de usuário (gráficos, FPS, volume, *Audio Offset*, *Movimento Reduzido* e modo ATB *Wait/Active*).
- **Quando Salvar:**
  - Salvamento automático atômico imediatamente após: conclusão de batalha/fase, aprimoramento/troca de equipamento, resgate/troca em loja, conclusão de música no Modo DJ e alteração de configurações.
- **Comportamento ao Fechar / Cair no Meio da Batalha:**
  - *Modo Padrão (Offline/Laboratório Atual):* Não grava vitória nem consome recompensas pela metade; ao reabrir o jogo, retorna ao Hub com o estado íntegro anterior ao início da partida.
  - *Extensão Futura (Snapshot de Retomada em Combates Longos — `PEN-14`):* Para encontros extensos (como chefes de 3 equipes ou ondas longas), prever snapshot local/servidor assinado com ID da tentativa para permitir **retomar de onde parou** uma única vez ou descartar a tentativa sem duplicar recompensas.

---

## 5. Modo Offline (Laboratório) vs. Produção Conectada e Migração de Versões
**DECISÃO DESEJADA (Sem Bloquear o Protótipo Local, Preparado para Produção Segura):**
- **Comportamento Offline (Prototipagem e Laboratório):**
  - O protótipo C++20 e os testes na Unreal Engine 5.8.4 funcionam **100% offline** sem exigir conta, login ou servidor ativo, lendo e gravando o save local em formato determinístico independente de *locale* (`CIC-01`).
  - Lojas e modos online exibem aviso honesto (`DISPONÍVEL APENAS NA VERSÃO CONECTADA`) sem travar a navegação offline.
- **Produção Online (PC / Mobile):**
  - Na versão final publicada, até mesmo o progresso single-player (Campanha, Chefes, Modo Clássico, Modo DJ, Lojas e Gacha) será **validado de forma autoritativa pelo servidor** (exigindo conexão para validar resultados e impedir adulteração de moedas/equipamentos), mantendo cache local apenas para preferências visuais/áudio e leitura do *Guia dos Livros* já desbloqueados.
- **Versionamento, Migração e Conflitos de Progresso:**
  - Todo arquivo de save carrega um cabeçalho explícito `SaveSchemaVersion` (ex.: `v1`, `v2`...) e `CatalogVersion`.
  - **Migração Segura:** Ao carregar um save de versão anterior, o jogo executa migradores sequenciais (`v1 -> v2`) criando backup automático antes da conversão e preenchendo novos campos com valores padrão seguros sem apagar conquistas antigas.
  - **Resolução de Conflitos (Multi-dispositivo PC <-> Android):** Em caso de divergência entre save local e nuvem ao