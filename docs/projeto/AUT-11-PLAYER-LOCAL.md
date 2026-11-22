# AUT-11 — ensaio de reprodução local na Unreal

22/11/2026. Backend e acesso manual implementados e compilados. **Reprodução efetiva não executada/validada. AUT-11 ainda não está encerrado como aceite de produto.** Não houve editor, prévia, controle de tela, Android, downloads ou acesso à rede.

## Preparado

- Botão **Rever abertura** no hub normal. DrawHUD desvia exclusivamente para mídia enquanto ativa; não despacha ações de combate ou navegação por trás do vídeo.
- UMediaPlayer, UMediaTexture e UMediaSoundComponent usam módulos já instalados MediaAssets/AudioMixer. Nenhum plugin instalado/habilitado nesta entrega. Escolha real de decoder é da engine; compatibilidade de codec não é inferida por extensão MP4.
- Leitura do catálogo local Content/Presentation/data/presentation/startup.json, schema 1, limitada a 1 MiB. Variantes IntroDesktop/IntroMobile Ready, IDs conhecidos, caminhos relativos validados e arquivo local existente. Variante preferida por modo de apresentação/proporção atual; fallback à outra variante cadastrada. Falha de abertura não tenta todas as variantes automaticamente.
- Reprodução sem loop, preparação com limite de 15 segundos e retorno ao hub em erro. Recursos encerrados ao terminar, pular, reiniciar ensaio ou destruir o HUD. Delegate de término removido antes de Close; estado usa ticket do AUT-10. Preparação/erro observados por polling; nenhum callback externo pode autorizar comandos de batalha.
- Pulo na primeira exibição por hold de 800 ms; revisão já assistida aceita clique imediato. ESC/Espaço usam o mesmo hold. Gesto começa na área do botão e é interrompido ao sair/perder foco. Segundo plano pausa o player e o áudio; retorno requer toque/clique para continuar.
- Exibição por contenção, sem cortar vídeo. Barras escuras de fallback. Não solicita rotação física neste ensaio Windows.
- Marcador simples por variante/versão em Saved/Media/intro-desktop-v1.flag ou intro-mobile-v1.flag. Só conteúdo exato INTRO_V1_SEEN é aceito; inválido/ausente significa não assistido. Conclusão/skip grava; falha nova não marca. Preferências e progresso de combate permanecem separados. Não há migração/backup nem garantia contra queda de energia.
- Originais autorais preservados. scripts/preparar-midias-unreal.ps1 copia somente variantes conhecidas para Content/Presentation e recusa sobrescrever destinos existentes. Foram copiados intro-desk.mp4 (184788524 bytes) e intro-mobile.mp4 (33907001 bytes); comprimentos conferidos, sem afirmar integridade criptográfica. Binários/mídias/Saved continuam ignorados no Git.
- Manifesto de preparo de pacote inclui ProbeMedia.cpp e as duas variantes; Presentation já era NonUFS. Preparo/cook/pacote não executados nesta entrega.

## Evidência e problemas corrigidos

Primeira compilação falhou por declaração EndPlay duplicada no GameMode e dependência AudioMixer ausente para SetVolumeMultiplier. Declaração mantida somente no HUD e módulo instalado incluído. Compilação final Editor Win64 Development aprovada, saída 0, sete ações; log build/aut11-build-final.log. Avisos históricos MSVC não preferido/C4864 permanecem.

Reexecutado binário nativo dos 12 grupos de contrato de mídia do AUT-10, aprovado. Núcleo puro não mudou; não são 12 testes novos do player. A bateria de 94 não foi repetida. Nenhum ensaio executou UMediaPlayer, decodificação, textura, áudio, persistência do marcador ou fallback real na engine.

## Aceite manual pendente — prioridade

1. Abrir projeto normalmente, iniciar prévia e clicar Rever abertura. Conferir imagem, áudio, proporção e tempo de preparação; falha deve retornar ao hub com painel informativo.
2. Na primeira exibição, tocar rapidamente em Pular não encerra; segurar 0,8 s encerra. Rever novamente e conferir pulo imediato. Conferir flag após reabrir a prévia.
3. Perder foco/retornar: áudio e imagem devem permanecer pausados até continuar; não permitir turno/ação por trás da mídia.
4. Terminar vídeo naturalmente, pular e fechar prévia durante vídeo; conferir liberação de áudio e retorno. Repetir com apresentação Mobile no Windows (não é Android).
5. Conferir arquivo ausente, flag inválida, decoder indisponível, falha de gravação e janela pequena em ensaios separados. Não alterar originais importantes para simular erros.

## Trabalho que ainda falta para concluir o bloco ampliado

- Validar os itens acima. Inicialização automática na primeira abertura permanece deliberadamente desativada até aceite de playback; o ensaio só inicia por clique manual no hub.
- Integrar legendas SRT/VTT e idioma; nesta entrega não há carregador/renderizador de legendas.
- Integrar volumes mestre/canal Mídia, mute e opções acessíveis. Componente de áudio está separado, porém volume do ensaio é 1.0, sem mixer de configurações do produto.
- Compor arte lateral dedicada; ensaio usa barras escuras. Rever posição/tamanho de botões em janela estreita e texto ampliado.
- Validar flag em disco, recuperação, notificações/lifecycle, codec e desempenho. A flag identifica variante/versão v1; substituir conteúdo exige nova versão explícita, sem hash automático nesta entrega.
- Rotação/restauração física de orientação, Android/iOS, downscaling/decodificação e pacote final exigem testes próprios. Não há transcodificação automática implementada.

Próxima prioridade do AUT-11 é aceite de reprodução; AUT-12 (marcador de runtime) pode avançar como código independente por nova solicitação. Nenhum percentual global aumentado apenas por compilação.

## Correção após teste do usuário — 22/11/2026

O usuário relatou flash ao clicar Rever abertura e erro aparecendo apenas ao abrir Equipe / Save. Log local confirmou `no media player plug-ins are installed and enabled in this project`. Causa: DisableEnginePluginsByDefault=true no .uproject; MediaAssets sozinho não habilita decoder.

Habilitado explicitamente WmfMedia, fornecido pela Epic e já presente na UE5.8, somente para Win64. Nenhuma instalação, download ou alteração global; não habilitar outros plugins indiscriminadamente. A afirmação anterior de nenhum plugin habilitado descreve apenas a entrega inicial.

Corrigida apresentação dos pop-ups no hub: DrawHUD mostra o painel e consome a entrada antes de navegação/equipe. O aviso não deve ficar oculto até abrir outra tela. Falha de reprodução ainda não concede watched.

Reexecutados os binários nativos existentes: 12 grupos MediaFlow e 10 ScreenFlow aprovados; não são testes de decodificação nem novos casos. Compilação registrada em AUT-11-CORRECAO.json. Não houve execução gráfica/Android. Reprodução continua pendente de novo teste do usuário.

Para testar: reabrir o projeto, iniciar Play e clicar Rever abertura. Apenas abrir o .uproject abre o editor; não inicia o jogo nem a intro. Autoabertura permanece desativada conforme o gate de aceite anterior. Se ainda ocorrer falha, o painel agora deve aparecer imediatamente no hub e o novo log permitirá avaliar codec/backend.

## Retorno do usuário e foco — 22/11

Reprodução confirmada pelo usuário, mas exigiu segundo clique. Isso não encerra o aceite completo de áudio, skip, flag e proporções. Corrigida distinção entre foco de viewport e janela em primeiro plano na mídia/entrada da mídia; início de nova solicitação limpa estado oculto anterior. Pausa por desativação real e retomada explícita preservadas. Logs de transição sem caminhos/credenciais permitem confirmar se o clique chegou e quando o player preparou/pausou. Causa provável, não comprovada pelo log anterior; reteste necessário. Compilação em build/aut11-focus.log.

Backends Android/iOS registrados explicitamente em PENDENCIAS.md: WMF não é portátil; habilitação, codec, orientação, áudio e lifecycle exigem validação por plataforma. Nenhum Android ou tela acessados.

## Primeiro clique após iniciar a prévia — 22/11

Reteste do usuário: somente a primeira reprodução requer segundo clique; seguintes funcionam com um. Logs INTRO_PREPARED mostram Playing, sem INTRO_BACKGROUND_PAUSE, portanto não confirmam a hipótese anterior de pausa. Primeira preparação cerca de 206 ms, seguintes cerca de 32 ms; diferença não explica sozinha clique perdido. Ajustado modo de entrada Canvas para GameOnly com ConsumeCaptureMouseDown=false e NoCapture, mantendo cursor visível e validação de foco. Objetivo: não consumir clique para captura inicial. Captura/foco inicial é hipótese forte, ainda depende de reteste. Conferir também seleção na arena e acesso à equipe, pois o modo de entrada é comum. Se futuramente migrar para UMG, revisar esse modo explicitamente.
