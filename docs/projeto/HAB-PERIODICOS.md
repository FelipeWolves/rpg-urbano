# HAB — efeitos contínuos Bleed e Poison

22/11/2026. Executor implementado no núcleo próprio. Tick no início da oportunidade natural/extra, inclusive com incapacidade; duração diminui após o tick e não no fim do turno. Não há tick em contra-ataque/assistência. Eventos de início da primeira oportunidade ficam disponíveis em Battle::initial_events().

Bleed registra floor(50% ATK), mínimo1, na aplicação. Poison calcula floor(4% MaxHP), mínimo1; contra Boss usa o menor valor entre essa base e ATK registrado na aplicação. MaxHP é imutável neste núcleo. Duração vem da habilidade. Instâncias têm fonte, identidade, dano/teto e duração próprios; reaplicar pela mesma fonte não renova instância antiga.

Piloto: máximo3 por tipo; tentativa excedente não substitui e emite StackLimitReached. Regra configurável com limite0 para retirar teto de gameplay, mantendo limite técnico4096 de estados por unidade. Não é aprovação de um limite definitivo de produto.

TrueDot é uma classificação distinta: não sorteia Miss/Block/crítico nem recebe DEF, mitigação, Defesa ou bônus genérico de dano adicional. Respeita escudos e invencibilidade. Reutiliza FIFO de mortes, prevenção, auto-revive e Victory Lock; sem reflexão ou contra-ataque diretamente provocado pelo tick. Fonte morta não invalida a referência nem recalcula o snapshot. Ticks letais não sintetizam comandos do jogador.

Dez grupos focados aprovados em scripts/testar-regras.ps1 -Suite Periodic. Regressão geral e compilação Unreal serão refeitas no fechamento da integração Felipe. Sem execução de editor/jogo, Android ou internet.

Bleed/Poison são classificados como debuffs comuns; os comandos genéricos de purificação/transferência e a política nova WAVE continuam na família SIS/PvE, fora desta entrega. Não substituir as políticas de transição SEQ/PAR já aprovadas. UI especializada, aceites visuais e plataformas móveis não são comprovados por testes nativos.
Fechamento posterior: bateria ampliada para13grupos, incluindo fonte morta, TrueDot fixo e terminal no limite técnico. Regressão/UBT/stage concluídos em HAB-FELIPE-PILOTO.md.
