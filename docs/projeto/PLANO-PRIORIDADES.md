# Prioridades — estado vigente

19/11/2026. Revisão do implementado primeiro; prioridade por dependência/risco e menor incremento útil. Não são estimativas de prazo ou consumo. [Plano histórico](historico/2026-11-18/PLANO-PRIORIDADES.md.txt).

Não há revisão obrigatória identificada nos blocos encerrados. A ponte Windows já tem compilação e execução equivalentes ao standalone; não reabrir sem mudança/falha pertinente.

| Ordem | Área/bloco | Próxima entrega delimitada | Dependência |
|---|---|---|---|
| 1 | ENG-01 — gráfico parcial | Consolidar uso de inglês e preparar escopo do pacote | Diagnóstico concluído; contorno em inglês, sem isolamento de rede |
| 2 | ENG-01 — pacote | Primeiro executável próprio de avaliação fora do editor | Teste gráfico inicial e cena/fluxo mínimo definidos |
| 3 | VIS-01 | Arena provisória usando um único estado de combate | Aceites iniciais de engine; não duplicar regras na UI |
| 4 | Multiplataforma | Primeiro teste Android, depois fluxo iOS conforme ferramentas/dispositivos | Não adiar decisões de escala/interface até arte final |
| 5 | ART-01 | Direção visual e um personagem original | Arena provisória suficiente para testar |
| 6 | AUD-01 | Pequeno conjunto próprio/licenciado e integração | Fluxo demonstrável; pode anteceder arte quando necessário |
| 7 | EXP-01 | Uma fase ou trecho rítmico | Escopo e aceites definidos antes de expandir |

HAB-01/CIC-01 mínimos estão concluídos. Novas famílias PvE, passivas, condições, inventário, servidor e ATB opcional devem receber contratos próprios; não entrar silenciosamente na tarefa de editor/arena.

## Próxima ação concreta

Planejar o pacote Windows de avaliação com inglês como padrão de trabalho Unreal. Diagnóstico em ENG-01-GRAFICO-DIAGNOSTICO.md. Localização do jogo em português será uma tarefa independente; definir estratégia e validar no produto, sem presumir idioma pela configuração do editor. Futuro uso de programa exige autorização específica. Nenhuma execução nesta entrega documental. Não instalar plugin/SDK, alterar toolset ou abrir serviços por antecipação. Preservar fonte única do núcleo e adaptadores separados de apresentação/persistência.

A engine não garante migração sem retrabalho. Não escolher preview por novidade nem inferir desempenho gráfico/mobile dos testes de console ou da commandlet NullRHI.
