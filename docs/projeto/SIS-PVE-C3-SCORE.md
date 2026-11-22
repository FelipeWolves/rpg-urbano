# SIS/PvE C3 — pontuação e recibos

22/11/2026. ScoreAccumulator observa somente comandos aceitos e relatórios de dano, com atribuição pelo emissor e pelo lado configurado. Efetivo inclui escudo absorvido+HP perdido; excesso separado. Soma em unidades de quartos e aplica floor no fechamento; não arredonda25%por hit. Intermediários inteiros amplos evitam truncar excesso antes do cálculo; resultado limitado a1e12. Reflection/explosões do FunnelV1 agora têm relatórios próprios e respeitam invencibilidade como consequência fixa; Legacy mantém seu contrato.

Epoch separa ondas/equipes cujos IDs de ação reiniciam. Repetição idêntica não conta outra vez; repetição conflitante é recusada. ScoreLedger liquida uma única vez porAttemptID e pode selecionar uma faixa explicitamente configurada. Não concede moedas, itens ou recompensas universais. Limite técnico4096lotes/recibos não é limite de dano do modo. Bônus de sobrevivência é opcional0por padrão; dificuldade1xpor padrão, ordem do bônus é configuração explícita, não regra de economia aprovada.

Dez grupos focados aprovados: quatro quartos deoverkill, duplicação/conflito, absorção,epoch,teto sem saturação prematura, bônus/dificuldade,AttemptID/faixa, comando recusado, atribuição de reflexão e invencibilidade. Persistência de recibos em disco/servidor e ranking não foram implantados. Sem telas/Android/internet.
