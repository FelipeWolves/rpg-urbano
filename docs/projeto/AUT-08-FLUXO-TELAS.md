# AUT-08 — estados de tela, pausa e abandono

22/11/2026. Máquina de estados testada e integração compilada. Aceite visual/interativo novo pendente; sem editor, prévia ou Android.

## Estados e navegação

A fonte de estado é ScreenFlow: Hub, Location (painel de um local), Options, Arena, Pause, AbandonConfirm e Result. Os booleanos paralelos de hub/local/opções foram removidos. Detalhes continuam como painel com conteúdo próprio, mas participam do bloqueio de entrada e da prioridade de Voltar. Tela de equipe e resumo expandido de resultados pertencem ao AUT-09; não foram simulados como sistemas completos nesta entrega.

| Ação | Resultado |
|---|---|
| Abrir local / opções no hub | Abre só a tela correspondente; não inicia batalha por trás |
| Entrar pelo Terminal | Inicia/retoma a sessão existente; após abandono prepara nova tentativa |
| Esc/Voltar com detalhes abertos | Fecha apenas os detalhes |
| Esc/Voltar com seleção ativa | Cancela habilidade/alvo selecionados |
| Esc/Voltar na arena sem seleção | Abre Pause |
| Retomar / Esc na pausa | Retorna à mesma batalha e mesmos valores |
| Abandonar na pausa | Abre confirmação separada |
| Cancelar / Esc na confirmação | Volta à pausa, sem abandonar |
| Confirmar abandono | Vai ao hub, sem vitória nem recompensa; próxima entrada cria outra tentativa |
| Combate concluído | Estado Result, usando a apresentação de resultado atual |
| Voltar de Result ao hub | Preserva resultado/ledger; reabrir não concede recompensa novamente |
| Reiniciar em Result | Usa o reset existente para outra tentativa, preservando créditos/histórico |

Reinício direto enquanto o combate está ativo foi removido; para descartar a tentativa é preciso confirmar abandono. O antigo Voltar ao hub durante batalha passa pela prioridade de seleção/pausa, em vez de ocultar silenciosamente uma tentativa ativa. `-Arena` agora escolhe a tela inicial, mantendo o hub disponível para retorno; não desabilita seus recursos. Inspeção somente leitura permanece separada e não ganha comandos de pausa/abandono.

## Preservação e limites

Pausa impede despacho manual/automático da batalha; retomar aplica a espera visual habitual antes da automação. Não chama SetGamePaused: tempo do mundo/renderização continuam, e pausa de áudio/animações futuras exige integração própria. A seleção de coordenadas corrigida e aceita continua em uso.

Abandono é um estado de navegação com nova tentativa pendente. Não fabrica um resultado de batalha, não chama settle, não concede vitória/derrota/recompensa e não escreve save. Snapshot anterior fica inacessível a comandos e é descartado pelo reset existente na próxima entrada. Progresso em memória de tentativas anteriores é preservado; progresso já salvo em disco não é alterado. Não há retomada de batalha/snapshot em disco nesta entrega. Políticas de ranking/produção e registro persistido de abandono continuam fora do laboratório local.

Modais e telas fora de Arena bloqueiam ações e automação. Um frame com tap também adia a automação, evitando que ela avance antes de processar o clique em Pausar. Transições inválidas são recusadas; confirmar abandono duas vezes não cria duas novas tentativas. Fechar painel ou paginar não modifica sessão/progresso. Menus de pausa/confirmação reaproveitam o painel medido e mantêm sua página entre frames.

## Verificação

-10 grupos puros novos: telas/modal, transições permitidas, Voltar hierárquico, pausa/retomada, confirmação/cancelamento, abandono idempotente e resultado/reinício.
-3 grupos novos com VisualSession real: pausa preserva snapshot, abandono adia reset sem recompensa, reentrada em resultado não duplica ledger.
-Bateria de apresentação:82grupos aprovados. Bateria focada de arena/sessão:35grupos aprovados. São medidas separadas, não um percentual do jogo nem uma nova execução dos213testes gerais.
-UBT Editor Win64 Development:8ações, saída0; `build/aut08-build.log`.
-Sem execução gráfica, testes Android, instalação ou mudança das regras de combate.

## Roteiro posterior e próximo bloco

Na prévia: abrir detalhes e usar Esc; selecionar uma habilidade/alvo e usar Esc; usar Esc outra vez para pausar; retomar; pausar e escolher Abandonar, cancelar, depois confirmar; voltar ao Terminal e verificar nova tentativa. Completar uma batalha e conferir Result/Hub/reentrada/reinício. Aceite desses menus ainda não foi feito pelo usuário.

Próximo AUT-09: seleção do catálogo existente, resumo de resultado/recompensa e ligação com savev1 por adaptador de armazenamento. Não inventar elenco, economia ou snapshot de combate. Android continua adiado.
