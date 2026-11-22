# AUT-10 — contrato de mídia local

22/11/2026. Base de estados implementada no módulo de apresentação existente, sem dependência de player. Não há alteração visual na prévia nesta entrega. Reprodução e ligação efetiva ao hub pertencem ao AUT-11.

## Contrato entregue

`MediaFlow` recebe um Asset relativo validado e gera um identificador por solicitação. Estados: Idle, Preparing, Playing, Paused, Completed, Skipped, Failed e Cancelled. Respostas prepared/ended/fail exigem o identificador atual e um estado compatível; callbacks antigos e repetidos são recusados.

- Ausência ou metadados inválidos produzem Failed com motivo explícito e sinal de retorno ao hub. Não abre arquivos/URLs nem consulta a existência do vídeo.
- Preparação, reprodução e pausa bloqueiam a permissão lógica de gameplay. Estados terminais sinalizam retorno ao hub; não navegam a tela por conta própria.
- Primeira exibição: skip exige pelo menos 800 ms. Vídeo já assistido: skip imediato. O adaptador de entrada deverá medir o gesto contínuo e cancelá-lo ao perder foco; o contrato não mede tempo de mouse/toque.
- Conclusão ou skip marcam watched. Falha ou cancelamento não marcam um vídeo novo como assistido nem apagam o estado previamente assistido. O caller deverá associar esse estado ao ID/versão da mídia; persistência da flag ainda pertence ao adaptador AUT-11.
- Segundo plano pausa reprodução; retorno ao primeiro plano mantém a pausa até resume explícito. Preparação que termina em segundo plano também fica pausada. Skip/resume não funcionam enquanto oculto.
- Falhas OpenFailed, Timeout e DecodeFailed são sinais do backend. Este bloco não implementa timer, abertura, decodificação nem fecha handles: esses recursos não existem no contrato puro.
- layout reutiliza plan_video: contenção, composição lateral e pedido de orientação já existentes. Pedido não equivale à rotação física; resolução/modo de janela devem ser fornecidos pelo adaptador. Nenhuma mídia é recortada, convertida ou redimensionada no disco.
- begin substitui a solicitação lógica e invalida callbacks anteriores. O backend futuro deverá liberar a reprodução anterior antes de abrir outra. Limite do identificador é protegido contra overflow.

## Verificação

12 grupos novos em tests/media_flow_tests.cpp: estado inicial, preparação, geometria, limiar de skip, rever, interrupção, preparação oculta/pausa, callbacks antigos, recurso ausente, caminho remoto recusado, falha/timeout/cancelamento e conclusão idempotente. Junto às regressões, 94 grupos de apresentação aprovados. O primeiro roteiro de teste não anexou o caso novo ao batch; a checagem detectou ausência do marcador e falhou. Roteiro corrigido e reexecutado com marcador de sucesso presente.

Relatório: docs/relatorios/atuais/AUT-10-TESTES.json. Resultado de compilação separado em AUT-10-BUILD.json. Não somar esta bateria à suíte geral como casos únicos de produto.

## Limites e sequência AUT-11

1. Resolver apenas as mídias autorais do catálogo local e abrir o backend instalado, sem download ou plugin antecipado.
2. Ligar callbacks com ticket e cancelamento/liberação de recursos; sinalizar timeout e falhas para fallback.
3. Integrar tela de mídia ao fluxo gráfico, entrada hold/click, mensagem de continuar, legendas e canal de áudio conforme decisões recebidas.
4. Persistir IntroWatched por ID/versão, separado do progresso de combate, com recuperação de arquivo inválido.
5. Validar efetivamente codecs, reprodução, áudio e retorno ao hub em ensaio autorizado. Compilação não comprova playback.

Não foram abertos editor, aplicativo, janelas de prévia, emulador ou Android; não houve rede, instalação, controle de tela nem gravação de preferências/saves de jogador. Recursos de imagem/vídeo recebidos permanecem locais e fora do commit. Percentual global mantido: não houve aceite de reprodução nem teste em celular.

Compilação confirmada: Editor Win64 Development, saída 0, 13 ações. Avisos conhecidos de MSVC não preferido e C4864 da engine continuam sem novos erros.
