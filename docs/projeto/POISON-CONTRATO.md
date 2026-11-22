# Veneno / Poison — contrato recebido em 22/11/2026

Documento direto do usuário durante SIS/REG/HAB; ainda sem implementação neste registro. O cabeçalho introdutório mencionou Sangramento, mas o corpo definiu Veneno: manter ambos separados.

Tick no início da oportunidade natural ou extra do alvo, antes da ação. Após o tick decrementa 1 e remove em 0. Reações fora de turno não disparam nem consomem duração.

Por tick: floor(MaxHP do alvo ×400/10000), mínimo1. Contra chefes: mínimo entre essa base e ATK do aplicador registrado na aplicação. Instâncias independentes com duração própria; usar limite do piloto de3 por efeito decidido na mesma conversa. Não inventar duração padrão: vem da habilidade aplicadora.

Debuff comum, purificável/transferível/removido entre ondas, True Damage contínuo. Ignora DEF/mitigação/Defesa/Block, respeita escudos e invencibilidade. ATK-teto é snapshot; MaxHP ainda é imutável no núcleo. Se máximo mutável for introduzido depois, confirmar a dinâmica do percentual sem retroagir uma decisão não recebida. Na direção final, Bleed/Poison não têm limite de gameplay por quantidade; limites técnicos e exceções de modo são separados.

Implementação 22/11: executor e dez grupos focados aprovados; ver HAB-PERIODICOS.md. Registros anteriores de ausência de implementação são histórico; duração continua fornecida por habilidade.
