# Nota Preparatória — Estrutura de Campanha, Livros e Linhas Narrativas

**Status:** PROPOSTA / DECISÃO DESEJADA (Registro antecipado de arquitetura narrativa; não autoriza implementação automática antes do contrato de Campanha).
**Relação com outras áreas:** Conecta-se diretamente ao sistema de **Fases de Sintonia Elemental** dos personagens (especialmente Felipe, O Lobo da Cidade) definido em `Core, habilidades, efeitos e fórmulas`.

## 1. Estrutura Base do Catálogo Literário (67 Capítulos)
- **4 Livros Fechados como Volumes de Catalogação:**
  - **Livro 1 - Contos de Um Lobo na Cidade - Vol. 1:** 15 capítulos/contos.
  - **Livro 2 - Contos de Um Lobo na Cidade - Vol. 2 - Histórias não contadas:** 22 capítulos/contos.
  - **Livro 3 - Contos de Um Lobo na Cidade - Vol. 3 - Histórias de vidas paralelas:** 15 capítulos/contos.
  - **Livro 4 - Contos de Um Lobo na Cidade - Vol. 4 - Os mundos e seus seres "vivos":** 15 capítulos/contos.

## 2. Fluxo de Execução e Registro Duplo (Guia do Livro + Linhas Narrativas)
- **PROPOSTA (Cenário Próprio + Arquivo Vivo):**
  - **Execução da Fase:** Cada capítulo acontece diretamente no cenário/ambiente próprio daquela história (sem forçar todos os contos em um único mapa urbano fixo).
  - **Registro no Guia do Livro (`BookCodex`):** Conforme cada capítulo é concluído, ele é desbloqueado e arquivado automaticamente dentro do guia/índice do seu livro específico (Vol. 1, Vol. 2, Vol. 3 ou Vol. 4), permitindo consulta, releitura e repetição em dificuldades maiores.
  - **Preenchimento das Linhas Narrativas (`NarrativeLines`):** Simultaneamente ao registro no livro, cada capítulo concluído preenche um marco na sua **Linha de Tom Narrativo** correspondente:
    1. **Linha Luminosa (Serena / Esperançosa):** Histórias leves, oníricas ou de acolhimento.
    2. **Linha Crepuscular (Urbana / Reflexiva):** Histórias de observação cotidiana, mistério e encontros na cidade.
    3. **Linha Sombria (Trágica / Intensa):** Histórias melancólicas, desesperançosas ou de confronto dramático.
- **Vínculo com a Evolução do Protagonista:** O avanço acumulado nessas linhas narrativas justifica e desbloqueia as **Fases de Sintonia Elemental** de Felipe (Umbra/Sombra, Luz/Shimmer e Natureza/Verde) e dos demais personagens com fases (O Filhote e O Dálmata) sem duplicar fichas no elenco.
- **Círculos Narrativos e Expansão de Inventário:** Capítulos e personagens interligados formam **Círculos Narrativos** no *Guia dos Livros*; completar 100% de um círculo desbloqueia **expansão permanente de inventário** (`+100` a `+200` slots) e libera suas faixas temáticas no catálogo musical.

## 3. Escalabilidade de Dificuldade e Variações Narrativas
- **DECISÃO DESEJADA (Seletor por Tier com Sobreposição Narrativa Opcional):**
  - Cada capítulo catalogado no guia suportará tiers globais de dificuldade (ex.: *Normal*, *Difícil*, *Pesadelo*, *Eclipse/Infernal*), escalando estatísticas e recompensas sem duplicar manualmente a árvore de capítulos.
  - **Variações de História por Dificuldade (Opcional por Capítulo):**
    - **Normal:** Adaptação fiel da história base do livro.
    - **Difícil:** Pode incluir diálogos adicionais / notas de bastidores (perspectiva interna do Lobo ou interações extras do grupo) sem alterar o desfecho canônico.
    - **Pesadelo / Eclipse:** Em capítulos-chave selecionados, pode apresentar encontros alternativos, confrontos interiores ou desdobramentos sombrios ("E se...?" / camadas ocultas), desbloqueando registros secretos no Guia do Livro.