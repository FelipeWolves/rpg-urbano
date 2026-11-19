# ENG-01 — execução básica do pacote Windows aprovada

19/11/2026. **✅ Pacote Development/Win64 abriu fora do editor, renderizou Template_Default e encerrou normalmente.** Aceite de abertura/renderização/encerramento nesta máquina, sem validar gameplay ou distribuição em outras máquinas.

## Evidência observada

Executável de entrada em `build/p/66761013/archive/Windows/RpgEngineProbe.exe`, com os dois hashes de EXE comparados ao relatório do build antes da abertura. O bootstrap iniciou o processo principal em RpgEngineProbe/Binaries/Win64. Ambos retornaram saída0, capturada separadamente. A janela foi selecionada pelo caminho do aplicativo, observada em primeiro plano e fechada por Alt+F4; ausência da janela confirmada depois. Nenhum editor gráfico foi aberto.

Mapa exibiu piso quadriculado e céu/nuvens. Log confirmou LoadMap e mundo Template_Default, RTX3060, Direct3D12/SM5 e inicialização da engine de2,41s. Não equivale ao tempo completo percebido até a imagem, nem benchmark. Limite solicitado t.MaxFPS30 foi aceito pelo log; FPS não foi medido. Resposta observada: ativação da janela e encerramento; sem teste de controles de gameplay ou menu, que não existem nesta cena técnica.

Idioma e região en confirmados no log via -culture=en. Zero linhas Error:/Fatal/Critical e zero linhas Warning: neste log. Não confundir ausência dessas categorias com ausência de todas as mensagens de diagnóstico: houve eventos de PSO e reconhecimento inicial da região do SO, sem impedir o teste.

## Escritas e limites

Diretório de usuário solicitado por execução em `build/package-runtime/20261119-041456/user`. O log referencia esse diretório para arquivos locais; foram observados GameUserSettings.ini e CrashReportClient.ini na árvore do teste. O arquivo de configuração de crash não é evidência de crash. Logs em runtime.log ao lado da pasta user. Não foi feita auditoria de todas as escritas do processo no sistema; não afirmar isolamento total. Saves do protótipo não foram usados pelo teste.

Rede/firewall mantidos inalterados. Não encontrados marcadores datarouter/outstanding Http Requests no log; não é auditoria de tráfego, prova de modo offline nem garantia de ausência de comunicação. Nenhum instalador, launcher ou cliente de outro jogo executado. Ferramentas de automação interagiram somente com a janela do aplicativo autorizado.

A máquina já tem ambiente de desenvolvimento. Não foram testados Windows limpo, pré-requisitos ausentes, assinatura, instalador, distribuição Steam/Store, Android/iOS, estabilidade prolongada, desempenho, SM6 ou recursos avançados de renderização. Não repetidos213 testes standalone nem11 checkpoints commandlet. O núcleo está ligado ao aplicativo, mas não foi acionado/validado como gameplay visual nesta cena.

Idioma do ambiente Unreal prioritariamente inglês. O jogo poderá ter português por localização independente, a definir/implementar/testar no produto; esta execução não valida localização.

## Relatórios e próxima entrega

- [Execução consolidada](../relatorios/atuais/ENG-01-PACOTE-RUNTIME.json).
- [Saída do processo principal](../relatorios/atuais/ENG-01-PACOTE-PROCESSO.json).
- [Build do pacote](ENG-01-PACOTE-BUILD.md) e inventário/hashes vinculados nele.

Logs e configurações gerados ficam em build ignorado. Capturas de tela não foram adicionadas ao repositório. Próximo incremento: planejar VIS-01, arena provisória conectada ao núcleo com turno/vida, seleção de ação/alvo, resultado e reinício. Não iniciar automaticamente arte, áudio, progressão, servidor ou mobile. O aceite atual não é aceite integral de produção da engine.
