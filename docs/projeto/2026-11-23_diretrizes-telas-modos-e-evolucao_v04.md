# Diretrizes de Telas, Modos e Evolução do Projeto (`DESIGN / VISÃO`)
**Projeto:** Contos de um Lobo na Cidade (`LOBO` / *Mundus Lupis Disco*)\
**Data:** 23 de novembro de 2026\
**Versão:** `v04` (Consolidada com PvP 1x1, Obtenção por História/Canônico, Moldura/Customização 2D→3D e Equipamentos Clássico FFIX vs. Padrão Dislyte)\
**Status:** `DIRETRIZ DE DESIGN (NÃO SUBSTITUI A FILA TÉCNICA IMEDIATA DA PRÉ-ALFA)`

---

## 1. Regra de Fases e Versões (`Pré-Alfa` → `Stable`)
O versionamento do projeto é vinculado diretamente ao percentual de conclusão do escopo completo de produção (`100% = v1.0.0-stable`):

- **Pré-Alfa (`0%–39%` — atual `v0.28-pre-alpha`):** Desenvolvimento interno exclusivo, sem login externo, save local técnico sujeito a resets.
- **Alfa Plena (`40%–69%` — `v0.40-alpha` a `v0.69-alpha`):** Entrada das primeiras amostras piloto de 3D/rig/animações e áudio; acesso restrito à equipe e testadores convidados (sem abertura pública no Patreon); save em validação de compatibilidade.
- **Beta (`70%–99%` — `v0.70-beta` a `v0.99-beta`):** Abertura para apoiadores ativos via **Patreon** (OAuth, validação de tier e segurança); save persistente obrigatório entre builds da Beta + marcação permanente na conta do apoiador (`BETA_SUPPORTER`) garantindo **recompensas exclusivas na versão Stable** mesmo que haja reset de balanceamento.
- **Stable (`100%` — `v1.0.0-stable`):** Produção completa e funcional, com save definitivo e entrega automática das recompensas de pioneiros e apoiadores.

---

## 2. Mapa Geral de Telas, Protagonismo e Modos
- **Protagonismo, Escolha Livre e Moldura na Tela Inicial:**
  - Embora **Lobo / Felipe** seja o personagem principal dos contos, o jogador pode escolher livremente com qual personagem jogar as fases.
  - A **tela inicial** deve contar com uma **moldura na parte inferior** exibindo o personagem escolhido atualmente.
  - **Personagem Editável (`[IDEIA FUTURA — 2D → 3D]`):** Registrar formalmente o desejo futuro de tornar o personagem editável. Como personalização ampla exige migrar/expandir do **2D para o 3D**, o recurso fica planejado para o futuro; com a base do Hub 3D já iniciada pelo agente, deve-se **avaliar progressivamente na engine até onde é viável ir no 3D** sem desviar o foco atual da Pré-Alfa.
- **Terminal Multimodal (Tela 2D de Seleção de Modos):**
  - *Monotrilho (superior esquerdo):* Rotas especiais, expedições e modos alternativos/PvE (*Ritual*, *Sobrevivência*, *3 Frentes*).
  - *Trem Regional / Metropolitano (andar intermediário / direita / ponto final):* **Modo História** (trilhas e capítulos).
  - *Ônibus (térreo / fundo central):* **Bosses de Equipamentos** (estilo *Guardiões do Milagre*).
  - *Metrô (térreo / escadas à direita):* **Farm Rápido, Recursos e Torre/Provações**.
  - *Loja de Conveniência (térreo / esquerda):* Em aberto / reserva futura.
- **HUD Fixa:** Inventário/Bolsa acessado via HUD fixa de forma **experimental**; predefinições de equipe reutilizáveis entre PvE e PvP.
- **Praça:** Hub comercial central definido e organizado por abas de moeda e trocas.

---

## 3. Obtenção de Personagens e Sistema Base de Trilhas do Modo História
- **Obtenção de Personagens:**
  - Ao menos os **4 primeiros personagens** (e, preferencialmente, os personagens recorrentes presentes nos contos e capítulos) devem ser obtidos **concluindo histórias específicas**.
  - Quando um personagem passa por um **evento canônico**, isso **desbloqueia a fase respectiva daquele evento**.
  - **Colaborações / Outros Mundos:** A arquitetura não impede a entrada futura de personagens vindos de **outros mundos / colaborações externas**.
- **Sistema Base de Trilhas (*Data-Driven* — Estilo *Dislyte*):**
  - Motor reutilizável orientado a dados (JSON / DataTable / Structs C++) para histórias curtas, médias ou longas (com divisão em Partes):
  - **Hierarquia (4 Níveis):** `BookId` → `StoryId` (`Title`, `MapBackgroundAsset`) → `PartIndex` / `PartTitle` *(opcional)* → `NarrativeLine` (`Luminosa` | `Crepuscular` | `Sombria`).
  - **Nós (`Nodes[]`):** `NodeId`, `NodeType` (`Dialogue` | `Combat` | `Hybrid` | `Boss`), `Position2D` (`X, Y` normalizados `0.0–1.0`), `NextNodeIds[]`, `EncounterId`, `DialogueId`, `StarsEarned` (`0–3`) e `MilestoneRewards[]`.
  - **Visual e Câmera:** Trilhas luminosas dinâmicas por linha narrativa, rolagem/pan suave para trilhas longas, centralização no próximo nó disponível, seletor de linha/dificuldade no topo e barra de estrelas/recompensas no rodapé.

---

## 4. Biblioteca, Atlas e `Lab - Exp`
- **Núcleo Definitivo da Biblioteca (Ala Geral — Estrutura Permanente):**
  - *Atlas Geral:* Catálogo completo de personagens e entidades do universo (obtidos ou não), com lore, retratos/artes, afiliações e status de descoberta.
  - *Árvores de Correlação:* Vínculos entre personagens, facções e eventos que concedem **fichas de personagem, itens específicos e buffs** (permanentes ou em batalhas/eventos).
  - *Mapas, Glossário e Crônicas:* Acervo cartográfico e registros desbloqueados na história.
- **Ala Privada da Biblioteca — `[CARÁTER EXPERIMENTAL]`:**
  - Visualização e gestão dos personagens que o jogador já possui, estruturada como **módulo plugável/desacoplado** para permitir ajustes ou migrações futuras.
- **`Lab - Exp` (Área Externa da Biblioteca) — `[CARÁTER ESPECIAL / POSIÇÃO MÓVEL]`:**
  - Ancorado como ponto de entrada independente para eventos e shows temáticos, preparado para mudar de local no Hub sem reescrever os eventos internos.

---

## 5. Pub, Instrumentos Musicais e Conexão com `Games`
- **Quadro Negro (Diárias, Semanais e Caçadas — Estilo *Final Fantasy XII*):**
  - *Caçadas (`Hunts`):* Contratos de alvos/chefes com contratante, briefing, pista de localização e recompensas.
  - **Ameaça e Requisitos na UI:** Exibir o nível de ameaça **em ressonância com o nível da conta e a média de nível da equipe**, além de destacar explicitamente **quando um personagem específico ou um tipo/sintonia/classe é exigido**.
- **Piano e Instrumentos Musicais (Impacto Real e Substancial):**
  - Bônus com **impacto real e relevante na jogabilidade** (evitando sistemas decorativos ou em desuso), habilitados conforme personagens/fichas e divididos em **Bônus Permanentes** e **Bônus Temporários** integrados aos modificadores de combate.
  - **Integração com `Games` (`[POSIÇÃO MÓVEL NO HUB]`):** Evolução progressiva junto à área de **Games**, prevendo realocação futura de **Games** e **Pub** (inclusive próximos/integrados) sem quebrar referências.
- **Ambiente, Salão e Balcão / Barman (`[EM ABERTO / MODULAR]`):**
  - Interações leves no salão; **Balcão / Barman** em aberto para pedidos de bebidas consumíveis mediante custo/troca.

---

## 6. Equipes, Equipamentos (Clássico vs. Padrão), Arena PvP e Economia
- **Predefinições de Equipe (*Presets*) e Limite de Até 5 Integrantes:**
  - Banco central de *Presets* reutilizáveis entre **PvE** (1 equipe, 3 equipes sequenciais e 3 frentes) e **PvP**.
  - A partir do marco definido do jogo base, **cada equipe deve suportar até 5 integrantes**, respeitando as restrições de cada modo.
- **Slots de Equipamentos — Modo Clássico (*FFIX*) vs. Modo Padrão (*Dislyte*):**
  - **Modo Clássico (Referência: *Final Fantasy IX*):** Slots e progressão de equipamentos no molde clássico de RPG tradicional (ex.: Arma, Cabeça, Braço/Proteção, Corpo, Acessório, com identidade de papel e bônus/habilidades associados às peças).
  - **Modo Padrão (Referência: *Dislyte* + Ênfase em *Resistência* e *Precisão*):** Conjuntos/slots no estilo *Dislyte* (vinculados em grande parte ao **Ônibus / Bosses de Equipamentos**), **com atenção reforçada a *Resistência* e *Precisão*** além de *Velocidade* e demais atributos.
- **Arena PvP (`1x1` Tempo Real vs. Equipes Predefinidas Assíncronas):**
  - **Etapa 1:** Duelo individual (**`1x1` em tempo real**), inspirando-se na leitura tática e estados do *Dislyte* e na clareza/mecânicas clássicas de *FFVIII / FFIX / FFXII* (ritmo/turnos, leitura clara de estados e recursos de sintonia/canalização).
  - **Etapa 2:** Evolução para **equipes predefinidas (não em tempo real, estilo *Dislyte*)**.
  - **Recompensas:** Duelos `1x1` e equipes predefinidas têm recompensas distintas; recompensas exclusivas no modelo *Dislyte* precisam ter **utilidade clara e prática**. *União* permanece adiada.
- **Economia Consolidada, Câmbio e Sinks de Endgame (`[MONITORAMENTO CONTÍNUO]`):**
  - Moedas: **Ouro**, **Gemas** e **Notas Musicais**.
  - **Loja de Ouro:** Atualiza a cada **3 ou 6 horas** (estilo *Dislyte*) com todos os tipos de itens elegíveis e **teto de nível limitado pelo nível da conta e nível atual do jogador**.
  - **Câmbio entre Moedas (Valores a Calibrar):**
    - *Ouro → Gemas:* por um valor **absurdamente alto**;
    - *Gemas → Ouro:* por custo em Gemas calibrado face ao valor em Ouro;
    - *Notas Musicais → Ouro:* a um custo proporcionalmente **mais elevado** que a conversão de Gemas para Ouro;
    - *Gemas → Notas Musicais:* por um **valor alto**, dando acesso a itens exclusivos de faixas superiores.
  - **Endgame:** Manter acompanhamento contínuo para evitar sobra excessiva de Ouro e inflação de recursos no *endgame* (como ocorrido no *Dislyte*).