# SIS-02-C1 — quantidades no executor e consumidores

22/11/2026. HP/ATK, HP inicial/runtime/revive, magnitudes/amount de efeitos, Event.amount e passivas quantitativas migrados para numeric::Quantity (int64). Quantidades positivas armazenadas são normalizadas ao teto 10^12; inválidos negativos/zeros continuam sujeitos aos domínios anteriores. Valores AP assinados, velocidade, duração, recarga, índices, IDs/revisões e créditos não foram convertidos indiscriminadamente.

BattleRules.numeric_model=2 identifica o novo domínio em memória; versão desconhecida é recusada. Não é versão de save e não é promessa de replay binário entre versões. Save v1 permanece sem estado de combate e passou seus testes de formato/locale/roundtrip/restore/IO e ledger sem reapurar recompensas.

Migrados locals de dano, absorção, reflexão/explosão, revive, StandardShield, passagem de HP no Stage e formato do HUD para argumentos amplos. Defesa continua floor por hit; escudos mantêm ordem e relógios. SEQ/PAR herdam HP amplo pelo Battle. SPD mantém cast explícito após validação 1..10000. Proporções gráficas continuam double/float e não alimentam a regra.

Cada camada de proteção armazena no máximo 10^12; a soma informativa de camadas distintas usa int64 e pode ultrapassar um teto individual, sem achatar/alterar a coexistência do legado. Quantidade agregada de proteção por família futura não é decidida por esta migração. Não há score novo nem uso de credits como score.

## Evidências

- Nove grupos de valores amplos: dano/guard/evento >INT32, gerador/absorção de escudo, reflexão, teto/invalidade, transporte de ondas, passiva/save v1, soma visual, revive, SEQ e chefe compartilhado. Suite Wide em SIS-02-Wide-TESTES.json.
- Regressão geral testar-combate.ps1 aprovada, saída 0: cenários pequenos, relógios/R04, SEQ/PAR, perfis, seleção, recompensa/save e sessão textual. 168 cenários/replays e lote funcional local de 1000 batalhas também passaram; não representam benchmark de servidor.
- Primeira tentativa detectou fallthrough acidental em SetSpeed: comentário ocultava break; corrigido, teste de cooldown entre ondas voltou a passar. Outra compilação detectou colisão entre helper de teste numeric() e namespace novo; renomeado somente o helper, preservando asserções.
- Editor Win64 Development compilado, 18 ações, saída 0; log build/sis02-c1-build.log. Não houve editor/player/janela/Android em execução. Avisos conhecidos de MSVC/C4864 da engine permanecem.

Próximo corte C2/C3: parser/contagens de manifesto, compatibilidade/QA e stage atualizado. O stage preparado antes desta alteração está histórico e não representa as fontes novas; não reutilizar hashes para mascarar drift. Fórmulas, roster pequeno e funcionalidades do funil não foram alterados por este commit.
