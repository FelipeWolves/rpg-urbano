# AUT-07 — preferências locais de apresentação

21/11/2026. Modelo, codec, recuperação e adaptador compilados. **Sem execução do aplicativo: gravação/reabertura real e aparência pendentes.** Android adiado.

## Opções disponíveis

No hub, o quinto botão inferior agora abre **Opções**. Preferências:

- Movimento normal/reduzido: desliga deslocamento e variação de brilho dos textos no modo reduzido.
- Contraste padrão/alto: fundo opaco escuro nos painéis/hub; não altera cores de times, combate ou atributos.
- Texto dos painéis:100%,125%,150%; largura medida, quebra e paginação são recalculadas. Não é escala global de todos os textos da arena.
- Apresentação Auto/Desktop/Mobile: política de enquadramento, sem trocar sistema operacional, renderer ou resolução física.

Área Esquerda/Centro/Direita e vista Focada/Panorama também são restauradas. Não há detecção automática das preferências do SO, leitor de tela ou navegação completa por teclado; o “foco” persistido é o enquadramento do hub. Configuração de vídeo/fullscreen/rotação física continua em seus próprios blocos.

## Separação de dados e precedência

Arquivo próprio: `experiments/unreal/RpgEngineProbe/Saved/Presentation/preferences-v1.txt`, formato versionado RPG_PRESENTATION1. Não contém equipe, créditos, recompensa, credencial, progresso ou snapshot da batalha. Não grava no savev1 nem nas configurações Android/Windows. Saved continua ignorado pelo Git; preferências são locais à instalação/cópia do projeto, sem sincronização de conta.

Precedência: padrões do catálogo/configuração → preferências válidas → argumentos específicos da execução. `-ReducedMotion` força movimento reduzido e impede o toggle; `-Presentation=Mobile`, por exemplo, força o enquadramento e identifica a opção como forçada no menu. Esses argumentos não são gravados por cima da preferência persistida ao alterar outra opção. O atalho Mobile continua sendo Windows em retrato, sem APK.

## Recuperação e gravação

- Codec estrito, limite4KiB; valida versão, enumerações, booleanos e escalas permitidas; rejeita campos extras/duplicados, truncamento e formato de save de combate.
- Leitura não escreve: tenta arquivo principal, depois `.bak`, depois padrões. Preferência inválida não toca no progresso e não provoca sobrescrita automática no carregamento.
- Mudanças explícitas no menu/enquadramento salvam; não há gravação a cada frame. Arquivo principal válido anterior é preservado em `.bak`; um principal inválido é copiado para `.invalid-<GUID>` antes de substituição.
- Arquivo anterior grande demais/ilegível ou falha de backup impede a gravação. Alteração fica somente na sessão, com mensagem visível; arquivo anterior é preservado.
- Gravação prepara temporário próprio e usa substituição pela API da engine, com backup prévio. Falha mostra mensagem e remove só o temporário gerado nesta tentativa. Não há garantia validada de durabilidade em queda de energia nem prova de atomicidade do filesystem.

## Evidências e limites

-10 grupos novos;324 combinações de modo/movimento/contraste/escala/área/vista com round-trip válido. Política principal→backup→padrões e entradas inválidas testadas.
-64 grupos nativos de apresentação aprovados (54 anteriores +10 novos). Não contar324 como novos grupos nem somar a suites com casos sobrepostos.
-UBT Editor Win64 Development:8ações, saída0; `build/aut07-build.log`.
-O adaptador de arquivos/menu foi compilado, **não executado**. Os testes nativos exercitam codec e seleção de recuperação, não a API de arquivos da Unreal, permissões de disco ou reinício real.
-Sem editor, prévia, Android, instalação ou alteração de rede. Regras de combate não mudaram.

## Roteiro de aceite posterior

1. Abrir a prévia normal, Hub → Opções; alternar movimento/contraste/texto e confirmar a mensagem de preferência salva.
2. Abrir detalhes de um local/habilidade e conferir aumento real do texto e paginação.
3. Escolher área/vista do hub, fechar e reabrir; conferir restauração e preservação do comportamento do combate.
4. No atalho Mobile, confirmar que a apresentação está identificada como forçada. O teste não valida Android.
5. Recuperação com arquivo inválido, falta de permissão e backup exige teste controlado com cópia de preferências, sem alterar save ou arquivos oficiais; não foi executado nesta entrega.

AUT-07: código e compilação concluídos, aceite interativo/persistência real pendentes. Próximo AUT-08: estados de tela e transições explícitas, incluindo retorno/pausa/abandono conforme contratos existentes. Progresso geral mantido aproximadamente22% até novos aceites.
