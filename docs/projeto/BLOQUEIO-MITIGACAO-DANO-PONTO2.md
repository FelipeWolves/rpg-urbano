# Bloqueio, mitigação e True Damage — ponto2

Decisão consolidada do usuário em20/11/2026. Atualiza ENT-01/07 e o ponto2 da revisão técnica nos assuntos abaixo. **Especificado, não implementado ou testado.** Não substituir Block técnico legado (impossibilitado de agir) por esse sistema implicitamente.

## Bloqueio probabilístico

- Chance universal3% por hit; Michel tem10% de base total (não13%). Bônus de habilidades/equipamentos somam-se à base, até50% por fontes gerais; até100% somente por estado especial explícito.
- Sucesso anula completamente o dano do hit elegível.
- True Damage e DoT (Veneno, Sangramento, Queimadura) ignoram bloqueio. DoT não deve ser confundido com os hits diretos de uma habilidade multihit.

## Mitigação percentual

- Não há redução universal de30%. Só existe quando concedida por buff, passiva específica ou equipamento.
- Fontes percentuais somam até50%; exceção acima desse teto precisa ser declarada por habilidade especial.
- Combina-se com mitigação por DEF e com Defender do perfil clássico. Sua posição é a camada anterior ao escudo/HP; a Defesa clássica mantém floor(dano/2) e pode resultar0.
- Valores de cada fonte vêm da habilidade/equipamento; o teto não significa atribuir50% a todos os portadores.

## True Damage

Ignora DEF, mitigação percentual, Defesa Clássica e bloqueio probabilístico. Imunidade a Dano/Invencibilidade pode impedi-lo; escudos ativos o absorvem antes do HP. SilverShield é futuro e não passa a existir pelo fato de estar citado. O ponto atual não muda duração ou fórmula do StandardShield nem o resolvedor R04.

A expressão “só é impedido” descreve barreiras/redutores de dano. Não foi usada para remover Prevenção de Morte/auto-revive já contratados; estes pertencem à resolução de morte. Se uma futura habilidade quiser ignorá-los, exigirá exceção explícita em contrato próprio.

## Delimitação do que ainda precisa de contrato

A inelegibilidade de DoT ao bloqueio está respondida; não implica que DoT herde todas as propriedades de True Damage. DEF, mitigação, escudo e invencibilidade para cada família DoT ainda devem ser declarados. Também permanecem separados: chances/Miss/ACC/RESIST/imunidade a debuff, aplicação de efeitos quando o hit é bloqueado, limites/RNG e arredondamentos intermediários. Não repetir as perguntas de base universal, teto ou elegibilidade já respondidas.

## Verificações futuras — não executadas

- Base3% geral/Michel10% total; soma de fontes e teto50%, exceção100% explícita.
- Hits diretos separados; DoT e True Damage não sorteiam bloqueio.
- Sem fonte de mitigação:0%; múltiplas fontes somam e limitam50%; não aplicar30% universal.
- True Damage atravessa DEF/mitigação/Defender/Block, mas respeita invencibilidade e escudo; excesso atinge HP, com R04 preservado.
- Dano comum1 sob Defender vira0; estado de fonte, remoção e expiração devem refletir-se sem acumulação residual.

Sem código, catálogo executável, build ou teste nesta atualização. Implementar em bloco REG/SIS próprio, preservando baselines históricos.
