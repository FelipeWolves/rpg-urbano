# SIS-02-C2/C3 — fronteiras, versões e stage

22/11/2026. Fechamento da migração numérica iniciada em C1. Parser de manifesto usa int64 para quantidades, enquanto contagens continuam limitadas a 64, slots a 1..3 e SPD recebe cast apenas depois do limite int. Versão é comparada antes de converter: valores como 4294967297 não podem dar wrap para schema 1 ou slot 1.

Nove grupos de manifesto aprovados, incluindo compatibilidade dos arquivos antigos, números amplos chegando à validação semântica (referência estrita ainda recusa atributos alterados), versão/slot com wrap e conteúdos malformados. Primeira compilação deste corte apontou narrowing de SPD, corrigido com validação explícita em vez de ampliar esse domínio. Schema 1 foi mantido porque a superfície aceita de referência e os arquivos válidos permanecem compatíveis; não foi habilitado carregamento de ficheiros arbitrários no HUD.

Save v1 continua entre batalhas e passou a regressão completa em C1. Nenhum save existente foi regravado/migrado automaticamente. numeric_model=2 é versão do domínio do Battle em memória, não um novo formato de save.

Seletor AUT-16 ganhou áreas Arithmetic/Wide. Plano dessas áreas e Data/Stage foi produzido sem reexecutar baterias por rotina. Stage novo em build/p/a75afa31 preparado com 54 entradas, incluindo numeric.hpp e fontes amplas; hashes/includes conferidos. O stage antigo foi preservado, sem mascarar drift. Build/cook/pacote não executados. Alterações REG/HAB posteriores exigirão novo stage antes de empacotar.

Estado: B e C concluídos no escopo de aritmética/migração de quantidades e fronteiras atuais. Pendências de produto seguem: fórmulas/funil/RNG e controles no REG, A2/piloto, score futuro, gráficos/mobile/replay entre plataformas e Shipping. A leitura em tela dos novos formatos ainda precisa de aceite separado.

Evidências: SIS-02-Arithmetic-TESTES.json, SIS-02-Wide-TESTES.json, SIS-02-C1-BUILD.json, AUT-14-TESTES.json, AUT-15-STAGE.json e ENG-01-PACOTE-PREPARACAO.json. Testes são separados por propósito; não somar como casos únicos de produto.
