# Defesa genérica — arredondamento e extensões

18/11/2026. Decisão explícita do usuário: dano recebido sob defesa genérica é reduzido à metade e **arredondado para baixo por padrão**. Alteração do Guard já existente, não criação de um sistema completo de DEF/atributos.

| Dano antes da defesa | Dano depois |
|---|---:|
| 25 | 12 |
| 5 | 2 |
| 6 | 3 |
| 1 | 0 |

A divisão inteira de valores não negativos implementa o arredondamento. Não foi acrescentado mínimo artificial de 1. Dano zero continua sendo um impacto resolvido pelas regras existentes; não significa esquiva nem inventa imunidade a gatilhos.

Na resolução multi-hit, a redução/arredondamento ocorre por acerto, mantendo o contrato R04: três hits de 5 causam 2 + 2 + 2 antes de escudos, não floor(15/2). Defesa continua sendo consumida ao final da ação ofensiva protegida, ou expira no próximo turno do defensor sem ser atingida. Mitigação ocorre antes das camadas de escudo. Consequências de reflexão/explosão mantêm sua regra atual, sem nova redução genérica implícita.

## Escopo dos perfis

- Clássico/protótipo atual: comando defesa habilitado conforme configuração; novo arredondamento padrão aplicado.
- Perfil padrão de equipes contratado no HAB-01: sem defesa genérica. Habilidades protetoras têm seus próprios contratos; não herdar automaticamente esta redução para todas elas.
- A implementação do novo perfil/posições permanece pendente; este ajuste não equivale à entrega do HAB-01 parte 1.

## Possibilidades futuras do RPG clássico — somente anotações

O usuário deseja margem para outras regras de dano recebido/defendido. Avaliar em bloco próprio:

- Percentuais variáveis de mitigação e posturas que melhorem ou prejudiquem a defesa.
- Vulnerabilidade, amplificação de dano recebido, perfuração/ignorar defesa e bônus de proteção.
- Relação entre atributos defensivos, itens, habilidades, escudos e condições do modo.
- Ordem explícita de modificadores, teto/piso, escala numérica e momento do arredondamento por hit.
- Distinção entre dano que seria causado, mitigado, absorvido e HP efetivamente perdido; eventos e testes adequados.

Essas extensões não foram implementadas. Não alterar o perfil padrão de equipes por consequência de uma regra clássica. Não assumir que estamina de itens seja AP de combate. Números e fórmulas dependerão de decisão futura, preservando a fronteira entre os perfis.

## Verificação

Cobertura de ímpar/par, mínimo com resultado zero, inteiros grandes, consumo/expiração, multi-hit e camadas de escudo ajustada à nova regra. Regressão MSVC /W4 /WX: 155/155 testes aprovados; matrizes e lote funcional preservados. Protótipo real reaberto por entrada roteirizada defesa/sair: B3 contra A3 causou 12. Demo mantém vitória A em 40 ações/14 rodadas. Evidência COMBATE-DEFESA-BAIXO.json e COMBATE.json. Relatórios anteriores preservados como históricos de suas regras à época.
