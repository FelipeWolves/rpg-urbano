# Entrega — Cenários 3D, Personagens e Animações (P2 — Fatia Inicial)

**Diretriz Base (DECISÃO DESEJADA):** Validar o pipeline completo (importação -> material -> esqueleto -> eventos de animação -> câmera) com **1 arena pequena** e **1 personagem original** antes de escalar para os demais personagens ou cenários dos 4 livros.

---

## 1. Primeira Fatia Jogável (1 Arena Pequena + 1 Personagem Original)
**DECISÃO DESEJADA (Escopo Vertical Slice):**
- **Arena Pequena de Referência — "Terraço da Vigília / Mirante Noturno":**
  - *Conceito:* Pátio elevado urbano sob garoa leve à noite (conectado à atmosfera de *Contos de Um Lobo na Cidade*), cercado por parapeito baixo, luminárias neon-noir em Azul Celestial (`#38BDF8`) e detalhes em Âmbar (`#F59E0B`), com fundo panorâmico distante (prédios em silhueta/skybox leve) para manter baixíssimo custo de polígonos e luz.
  - *Reutilização:* A mesma arena suporta tanto o enquadramento padrão em profundidade (**Modo de Equipes**) quanto a câmera lateral preservada (**Modo Clássico**), sem duplicar geometria.
- **Personagem Original Piloto — "Felipe, O Lobo da Cidade" (Fase 1: Umbra / Sombra):**
  - *Conceito:* Antropomórfico canídeo (lobo urbano) de porte atlético, casaco/sobretudo escuro com detalhes em azul/roxo noturno e brilho âmbar nos olhos/garras.
  - *Justificativa:* É o protagonista central da obra e já possui o kit piloto definido (`A1 Garra do Asfalto`, `A2 Olhar da Garoa`, `A3 Eclipse na Floresta de Pedra`), permitindo testar ataque corpo a corpo, contra-ataque, ultimate em área, bloqueio e variação de material por Sintonia Elemental.

---

## 2. Conceitos, Escala, Materiais e Pipeline de Exportação
**PROPOSTA / DECISÃO DESEJADA (Padrão Técnico Unreal Engine 5.8.4):**
- **Folha de Conceito (*Model Sheet*):** Entregar vistas **Frente, Lado (Perfil) e Costas** em pose neutra (`A-Pose` ou `T-Pose`) antes da modelagem final, acompanhadas de referências de expressão facial e acessórios.
- **Escala e Unidades:** Padrão nativo da Unreal Engine (`1 unidade = 1 cm`), altura aproximada de `180 cm` a `190 cm` para personagens adultos (e `120 cm` a `140 cm` para *O Filhote*), garantindo compatibilidade direta com o *UE5 Mannequin / Control Rig*.
- **Materiais e Escalabilidade Gráfica:**
  - *PBR Padrão (`BaseColor`, `Normal`, `ORM` — Occlusion/Roughness/Metallic + `Emissive` para detalhes neon/olhos).*
  - *Instâncias de Material Parametrizadas:* Permitem trocar a cor emissiva/aura do Lobo entre as **Fases Elementais** (Umbra/Sombra, Luz/Shimmer, Natureza/Verde) e desligar reflexos pesados/HDR/física de tecido quando o jogador ativar o modo **Desempenho/Baixo** no PC ou Android Studio AVD.
- **Formato de Exportação e Fonte Editável:**
  - *Exportação para Engine:* **FBX** (com esqueleto e *smoothing groups* preservados) ou **glTF 2.0**.
  - *Fonte Editável:* Manter arquivos originais editáveis (`.blend`, `.fbx` mestre, `.spp` / `.psd`) fora do Git em `assets/sources/`.

---

## 3. Direção Estética Ultra-Realista, Pipeline IA 3D e Licenciamento
**DECISÃO DESEJADA (Fidelidade Visual Cinematográfica + Pipeline Image-to-3D):**
- **Estilo Visual Alvo (Ultra-Realista):** O jogo manterá estética **ultra-realista** alinhada às referências cinematográficas geradas pelo autor no **Google Flow** (sem simplificar personagens ou cenários para estilos cartoon/estilizados).
- **Pipeline de Produção 3D Assistido por IA (*Multi-View Image-to-3D*):**
  - Utilizar folhas de referência geradas no Google Flow (Frente, Lado e Costas em fundo neutro) como entrada para geradores 3D de alta fidelidade com suporte a PBR 4K/8K, retopologia e *auto-rigging* (como **Meshy AI**, **Hyper3D Rodin** ou **Tripo AI**), exportando em `.fbx` para refinamento e montagem na **Unreal Engine 5.8.4**.
- **Escalabilidade Sem Perda de Identidade:** O modelo mestre é produzido em alta definição (com mapas PBR completos, *Normal*, *Roughness* e *Subsurface*) para rodar em **Ultra HD / HDR** no PC e dispositivos topo de linha, utilizando o sistema nativo de **LODs (*Level of Detail*)** e escalabilidade de materiais da Unreal Engine para rodar fluido no Android Studio AVD (8 GB RAM) e celulares intermediários sem comprometer a direção de arte.
- **Autoria e Licença:** Todos os modelos 3D gerados utilizarão planos com licença comercial comprovada (registrada em `assets/sources/` junto às matrizes `.fbx` e texturas originais) antes de integrarem builds públicos.

---

## 4. Ciclo de Animações e Separação entre Visual e Regra de Combate
**DECISÃO DESEJADA (Sincronização por Eventos / `AnimNotifies`, Sem Atrasar a Lógica):**

| Estado / Clipe | Duração Visual Alvo | Marcos Internos (`AnimNotifies`) | Relação com a Regra do Núcleo |
| :--- | :--- | :--- | :--- |
| **`Idle` (Postura Base / Guarda)** | Loop contínuo (`1.5s–2.0s`) | Início/Fim de ciclo | Puramente visual; alterna postura quando em **Guarda** (Modo Clássico) ou sob controle (*Stun/Freeze/Petrification* congela ou desacelera o clipe). |
| **`Attack_A1` / `Skill_A2_A3` / `Limit`** | `0.8s` a `1.4s` (Básica) / `1.5s` a `2.5s` (Ultimate) | 1. `Notify_Start` (deslocamento/preparo)<br>2. `Notify_Impact` (momento exato do golpe)<br>3. `Notify_End` (retorno à posição) | **Regra Arquitetural:** O núcleo determinístico resolve a matemática do turno instantaneamente; a interface dispara os números de dano, efeitos e tremores no `Notify_Impact`. Mudar velocidade visual (`1x` / `2x`) ou pular animação **nunca altera o resultado nem o relógio de AP/ATB**. |
| **`Hit_React` & `Auto_Block`** | `0.35s` a `0.5s` | 1. `Notify_BlockSpark` (impacto aparado)<br>2. `Notify_Recover` | Dispara ao receber dano ou ao ativar o **Bloqueio Automático** (base 3%+), sem criar novo turno. |
| **`Defeat` (Derrota / Nocaute)** | `1.0s` a `1.3s` | 1. `Notify_Collapse`<br>2. `Notify_DissolveOrDown` | Executa após drenagem FIFO de mortes/revives (`R04`); se houver *Auto-Revive*, encadeia imediatamente o clipe `Revive`. |
| **`Victory` (Vitória)** | `1.5s` a `2.0s` | 1. `Notify_PoseLock` | Executa somente após encerramento completo da cadeia final de combate. |

---

## 5. Câmera, Chão de Contato, Origem/Pivô e Itens Modulares
**DECISÃO DESEJADA (Padrão Espacial da Arena):**
- **Origem e Pivô (`Root / Pivot`):**
  - Todo personagem possui pivô centralizado no solo (`Z = 0.0`, exatamente entre os pés na sola do calçado/pata), eixo frontal apontando para `+X` na Unreal Engine.
  - O **Chão de Contato** da arena é plano no nível `Z = 0.0` nas áreas de combate para evitar flutuação ou afundamento dos pés e do seletor geométrico no chão já aprovado.
- **Enquadramento de Câmera (Sem Substituir o Modo Clássico):**
  - *Modo Padrão de Equipes (`Standard`):* Câmera em perspectiva isométrica/diagonal em profundidade, ajustando distância e FOV entre `16:9` (Desktop) e `9:16` (Mobile) para manter as formações (1 a 5 unidades) visíveis dentro da `USafeZone`.
  - *Modo Clássico (`-ArenaLayout=Classic`):* Preserva integralmente a **câmera lateral** já validada no projeto (aliados de um lado, inimigos do outro), exibindo o menu clássico de comandos e barras ATB sem sobrepor os modelos 3D.
- **Itens Modulares (`Sockets`):**
  - Preparar *sockets* no esqueleto (`hand_r`, `hand_l`, `spine_back`, `head_top`) para acoplar acessórios modulares (como o dispositivo/comunicador do Hub, armas/garras energizadas ou adornos de evento) sem precisar reexportar o corpo inteiro do personagem.