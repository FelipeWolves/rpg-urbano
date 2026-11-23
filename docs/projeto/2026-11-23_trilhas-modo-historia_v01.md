# Modo História — trilhas orientadas a dados

Diretriz de design de 23/11/2026. Piloto futuro, na rota do Trem Regional / Metropolitano. As [diretrizes LOBO](2026-11-23_diretrizes-desenvolvimento-lobo_v01.md) concentram o mapa de telas e modos, e a [política oficial](POLITICA-VERSIONAMENTO.md) define as fases, o acesso e os saves.

## Contrato proposto

O motor deve renderizar histórias curtas, médias e longas a partir de dados. JSON, DataTable e structs C++ são opções para definir no contrato de Campanha. Capítulos novos devem ser configuráveis sem lógica específica por capítulo.

| Nível | Dados |
|---|---|
| Livro | BookId para livro ou arco |
| História | StoryId, Title e MapBackgroundAsset para o fundo do mapa 2D |
| Parte | PartIndex e PartTitle opcionais para histórias longas |
| Linha narrativa | NarrativeLine com Luminosa, Crepuscular ou Sombria |

Cada linha declara Nodes[]. Cada nó tem NodeId, NodeType (Dialogue, Combat, Hybrid ou Boss), Position2D (X e Y normalizados de 0 a 1), NextNodeIds[], EncounterId, DialogueId, StarsEarned (0 a 3) e MilestoneRewards[]. O contrato de Campanha deve explicitar os campos aplicáveis a cada tipo e separar estrelas obtidas pelo jogador da definição do nó.

## Interface e navegação

- Traçar conexões luminosas a partir de NextNodeIds[] e trocar a cor e os nós visíveis conforme a linha ativa.
- Permitir deslocamento suave em mapas extensos e centralizar no próximo nó desbloqueado, respeitando Movimento Reduzido.
- Exibir seletores de linha e dificuldade no topo. Mostrar nós bloqueados, disponíveis e concluídos com estrelas. Exibir progresso por estrelas e marcos resgatáveis no rodapé.

## Obtenção de personagens e fases canônicas

Ao menos os quatro primeiros personagens devem ser obtidos pela conclusão de histórias específicas. O mesmo fluxo é preferencial para personagens recorrentes. Eventos canônicos desbloqueiam a fase correspondente do personagem. Os dados do elenco devem comportar personagens de outros mundos e futuras colaborações.

## Piloto futuro e aceite

Um capítulo de demonstração com 6 a 8 nós, cobrindo diálogo, combate, híbrido e chefe, com suporte a partes e variação de linha. Reusar o combate atual e o save isolado de QA.

Ao executar, verificar a troca entre linhas, o deslocamento e a centralização, os estados dos nós, a persistência de estrelas e o resgate de marcos. Confirmar que uma alteração nos dados modifica a trilha sem exigir código novo e que o save de QA fica isolado.
