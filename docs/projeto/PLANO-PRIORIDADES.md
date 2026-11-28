> **28/11/2026 — Terminal na Pré-Alfa.** O [TerminalCore](2026-11-28_terminalcore-mapa-navegacao_v01.md) compartilha catálogo e retorno de sessões entre 2D e 3D. O usuário aceitou o visual das faixas e a seta Voltar com mouse nos dois moldes. O [piloto 2D de QA](2026-11-28_terminal-2d-piloto-qa_v01.md) e a [câmera por setor com atalho 2D](2026-11-28_terminal-3d-setores-atalho_v01.md) compilaram em Win64. Os retestes em jogo, o toque e a geração de novos pacotes continuam na fila.

> **27/11/2026 — Terminal e arquitetura.** A [revisão técnica](2026-11-27_parecer-arquitetura-lobo_v01.md) separa o código existente das novas frentes. AUT-13, QA em janela e Hub 3D provisório seguem disponíveis com os recursos atuais. A revisão orientou os primeiros cortes do Terminal. Os novos pacotes seguem em etapa futura. A interface final do Terminal depende da arte. Hardware mínimo, regras comerciais, provedor, dados de conta e cupons ainda precisam de validação ou decisão. A estimativa da demo segue em 28,43%.

> **Prioridades vigentes em 23/11/2026.** Apresentação de combate por AUT-13, preparação dos aceites em janela e refinamento do Hub 3D avançam com o material atual. A tela final do Terminal depende da imagem. O mapa editável segue após o aceite dos materiais provisórios e a decisão de formato. Os [afazeres](FILA-DESENVOLVIMENTO-INDEPENDENTE.md) e a [central de entregas](../../entregas/NECESSIDADES-ATUAIS.md) separam execução, materiais e aceites.

# Prioridades — estado vigente

19/11/2026. Revisão do implementado primeiro; prioridade por dependência/risco e menor incremento útil. Não são estimativas de prazo ou consumo. [Plano histórico](historico/2026-11-18/PLANO-PRIORIDADES.md.txt).

Não há revisão obrigatória identificada nos blocos encerrados. A ponte Windows já tem compilação e execução equivalentes ao standalone; não reabrir sem mudança/falha pertinente.

| Ordem | Área/bloco | Próxima entrega delimitada | Dependência |
|---|---|---|---|
| 1 | VIS-01 / escudo | Inspecionar pacote atualizado após autorização; escudo comum já implementado | ESC-01–07 fechados;1/5/resoluções e aceite visual continuam pendentes |
| 2 | ENG-01 — distribuição | Windows limpo, pré-requisitos e comportamento do pacote atualizado | Pacote básico aprovado nesta máquina; sem distribuição final validada |
| 3 | Localização | Definir e validar português no produto, separado do editor | Fluxo visual e textos traduzíveis; inglês mantido no ambiente Unreal |
| 4 | Multiplataforma | Primeiro teste Android, depois fluxo iOS conforme ferramentas/dispositivos | Não adiar decisões de escala/interface até arte final |
| 5 | ART-01 | Direção visual e um personagem original | Arena provisória suficiente para testar |
| 6 | AUD-01 | Pequeno conjunto próprio/licenciado e integração | Fluxo demonstrável; pode anteceder arte quando necessário |
| 7 | EXP-01 | Uma fase ou trecho rítmico | Escopo e aceites definidos antes de expandir |

HAB-01/CIC-01 mínimos estão concluídos. Novas famílias PvE, passivas, condições, inventário, servidor e ATB opcional devem receber contratos próprios; não entrar silenciosamente na tarefa de editor/arena.

## Próxima ação concreta

ESC-01 a ESC-07 fechados e implementados em [ESCUDO-PADRAO.md](ESCUDO-PADRAO.md). Priorizar aceite VIS-01–03 com autorização de abertura. REG/SIS continuam contratos futuros; lista central evita repetir perguntas encerradas. Não reabrir R04/SEQ/PAR nem mudar defesa atual por proposta do documento.


Fechar o aceite visual do VIS-01 conforme VIS-01C.md, preservando o fluxo3×3 já observado; pacote Windows básico já gerado, aberto e encerrado com saída0. Ver ENG-01-PACOTE-RUNTIME.md. Manter inglês como padrão de trabalho Unreal. Diagnóstico em ENG-01-GRAFICO-DIAGNOSTICO.md. Localização do jogo em português será uma tarefa independente; definir estratégia e validar no produto, sem presumir idioma pela configuração do editor. Futuro uso de programa exige autorização específica. Nenhuma execução nesta entrega documental. Não instalar plugin/SDK, alterar toolset ou abrir serviços por antecipação. Preservar fonte única do núcleo e adaptadores separados de apresentação/persistência.

A engine não garante migração sem retrabalho. Não escolher preview por novidade nem inferir desempenho gráfico/mobile dos testes de console ou da commandlet NullRHI.
