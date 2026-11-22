# REG-01-A — outcomes, RNG e debuff

22/11/2026. Primeiro corte de REG: contrato puro em hit_resolution.hpp. Não altera ainda os golpes do Battle legado.

RNG v1 usa operações uint64 explícitas e chaves de seed/ação/hit/alvo/efeito/canal, sem relógio, std::hash, std::random ou estado global. Canais Miss/Block/crit/chance-base/resistência são separados e etiquetados; rejeição de módulo evita viés de reduzir diretamente todas as palavras a 10000. Seeds são de laboratório/replay, não mecanismo de segurança/anticheat. Incluir identidade do alvo de forma estável e não usar FMath::Rand do seletor visual como RNG autoritativo.

classify_hit: invencibilidade precede Miss, Block precede crítico. Block normal tem teto 50%, ou 100% só se estado explícito permitir; True e DoT não sorteiam Block nem crit por padrão. DoT não sorteia Miss; isso não decide seu catálogo, mitigação ou escalas. Block/contato contam para passiva de atingido, Miss/invencibilidade não. Outcome não depende de HP perdido: absorção por escudo será conectada no próximo corte.

Debuff: Immunity é distinta da invencibilidade a dano; exige contato não Miss/não Block quando houver ataque, passa na chance-base e só então testa RESIST-ACC (piso20%, teto100%). Irresistível ignora só ACC/RESIST, não Immunity ou chance-base. Skill pura sem ataque não exige contato inexistente. Imunidades estruturais devem ser incluídas pelo executor na flag de imunidade correspondente.

Nove grupos nativos aprovados: vetor fixo do mixer, faixa/repetição por chave, precedências, elegibilidade especial, debuff/imunidade/irresistível/skill pura, amostra determinística do piso de resistência e entradas inválidas. Amostra não é prova de distribuição estatística completa. A implementação inteira é determinística por construção no domínio de operações, mas ARM/iOS ainda não foram executados.

Próximo REG-01-B: funil de dano racional/floor final e ligação ao executor sob perfil explícito, sem mudar baselines antigos por introduzir RNG. Catálogo de DoTs e stats de produto não são inventados neste corte.
