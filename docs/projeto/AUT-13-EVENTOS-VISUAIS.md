# AUT-13 — adaptador de eventos visuais

22/11/2026. Preparação funcional do canal de eventos, sem animação final. VisualSession entrega apenas resultados aceitos para VisualEventQueue: Begin, eventos em ordem causal (Impact ou State), End. Evento original é preservado, incluindo hit_index, planned_hits e contexto da ação/reações. O classificador de impacto cobre Attack, reflexão, explosão e absorção; eventos restantes não são descartados.

Identidade: revisão monotônica da sessão + índice do cue. Revisões repetidas/antigas e ações recusadas não criam cues. Reset/troca/carregamento esvaziam fila sem autorizar replay de revisão passada. Fila limitada a 1024 cues no padrão (2–4096 configuráveis); lote excedente é descartado integralmente e contabilizado, sem reexecutar dano/recompensa. Falta de consumidor ou recurso de animação não bloqueia o combate.

HUD drena até 64 cues por frame e oferece trace VeryVerbose VIS_CUE. Não há ponteiros para o executor, callback que cause dano ou obrigação de aguardar animação. Início/impacto/término são ordem visual, não um novo relógio de gameplay. Ataques continuam usando a resolução imediata anterior.

Sete grupos nativos: ordem/identidade/dados de hit, deduplicação, recusa, limite/backpressure, lote grande, eventos de estado e limites de configuração. Regressão da sessão e build em AUT-13-BUILD.json. Recursos próprios inexistentes não geram espera.

Pendências: ligação a poses, rig, VFX, som e efeitos provisórios visíveis; duração/interpolação, cancelamento de reprodução gráfica, fluidez e aceite visual. Esta entrega não afirma que personagens já animam, nem introduz threads/servidor. Sem execução gráfica, Android, internet ou recurso externo.

Próximo: AUT-14, esquema versionado e diagnóstico dos dados do cenário sem trocar o contrato de batalha por conteúdo incompleto.
