# ENG-01 — primeiro pacote Windows gerado

19/11/2026. **✅ Compilação, cook, stage e archive concluídos; ⚠️ execução do aplicativo ainda não validada.** Pacote de avaliação Development/Win64, sem arena própria ou integração visual de combate. Não é release pronto para distribuição.

## Resultado

AutomationTool BuildCookRun terminou com ExitCode=0 e BUILD SUCCESSFUL em 405,86s (aproximadamente 6m46s). UBT compilou os alvos Editor e Game, 31 ações; UnrealEditor-Cmd concluiu o cook com saída0; geração dos contêineres e archive concluída. Inglês foi solicitado no cook e confirmado nos argumentos; não houve abertura do editor gráfico ou do pacote nesta entrega.

Archive: `build/p/66761013/archive/Windows`. Executável de entrada: `RpgEngineProbe.exe`; binário principal em `RpgEngineProbe/Binaries/Win64/RpgEngineProbe.exe`. São 45 arquivos, 525.955.927 bytes (aproximadamente 501,6 MiB), incluindo 201.805.824 bytes de símbolos PDB de diagnóstico. Tamanho não é estimativa da versão final nem mínimo otimizado. Preservar a pasta completa para futuros testes; não mover apenas o EXE.

Contêineres .pak/.utoc/.ucas gerados pela engine a partir do mapa de avaliação. Não há conteúdo do jogo de referência. A presença desses formatos não equivale a criptografia ou proteção validada. Inventário compacto CSV e hashes SHA-256 dos dois executáveis registrados, sem calcular hashes de todos os arquivos.

## Falha inicial e correção

A primeira tentativa terminou com saída6 antes de concluir a compilação: caminhos temporários longos excederam o limite de260 caracteres imposto pela ferramenta. Preservada em ENG-01-PACOTE-BUILD-INICIAL.json, com logs no stage original. Não mover engine nem mudar registro/política de caminhos do Windows.

A preparação passou a usar `build/p/<id>` dentro do repositório; nova cópia com os mesmos22 arquivos canônicos e hashes conferidos. Foi necessário compilar no novo stage, sem excluir a tentativa anterior. Scripts preparar-pacote-unreal.ps1 e empacotar-unreal.ps1 preservam pastas/logs; o executor recusa reutilizar arquivos de log existentes e não abre o aplicativo. Revalidar plano/manifesto e obter autorização pertinente antes de futuras execuções.

## Avisos e limites

- MSVC14.52.37029/SDK26100 usados; mantido aviso de compilador não preferido. Nenhum componente instalado ou toolset alterado.
- Cook avisou que assets padrão de compressão ACL não estavam disponíveis, compatível com o experimento que desativa plugins por padrão. Não corrigir habilitando plugins indiscriminadamente; animações/compressão ainda precisam de avaliação antes de recursos animados.
- Stage aguardou temporariamente o Zen local; a operação prosseguiu e terminou. Não houve reconfiguração de rede/firewall. Ferramentas usaram seus caches e armazenamento local normais; isso não comprova ausência de tráfego externo.
- DefaultEditorSettings.ini foi excluído do pacote pela ferramenta. O opt-out de telemetria do editor não deve ser apresentado como configuração ou garantia de rede do aplicativo distribuído.
- Não houve teste do aplicativo, FPS/GPU em runtime, dependências em Windows limpo, instalador/pré-requisitos, assinatura, Steam/Store, mobile ou localização do jogo. O redistribuível VC existente na engine não foi instalado nem executado.
- Fonte única do núcleo preservada; sem alterações de regras e sem repetição dos213 testes standalone/11 checkpoints commandlet. Compilar o núcleo no Game não demonstra sua execução durante gameplay.

## Evidências

- [Resultado do empacotamento](../relatorios/atuais/ENG-01-PACOTE-BUILD.json).
- [Inventário de45 arquivos](../relatorios/atuais/ENG-01-PACOTE-ARQUIVOS.csv).
- [Tentativa inicial preservada](../relatorios/atuais/ENG-01-PACOTE-BUILD-INICIAL.json).
- [Preparação e manifesto](../relatorios/atuais/ENG-01-PACOTE-PREPARACAO.json).

Logs completos e artefatos permanecem em build ignorado. Podem conter caminhos, identificadores e parâmetros de armazenamento local; não publicar logs brutos. Nenhum arquivo gerado deve ser commitado como código.

## Próximo teste, mediante autorização específica

Abrir somente o EXE de entrada deste pacote pelo PowerShell, com -culture=en e limite de FPS solicitado, observar carregamento do mapa e resposta/encerramento, registrar log e código de saída. Não supor que argumentos do editor controlam todos os mecanismos do runtime. O aplicativo poderá gravar Saved/logs/preferências normais; identificar os caminhos utilizados e evitar saves do protótipo.

Manter rede inalterada conforme decisão do usuário. Não executar instaladores automaticamente se faltar dependência; registrar erro e interromper. Abertura/inspeção/encerramento requer autorização explícita do aplicativo de avaliação e da interação com sua janela. Depois desse aceite, planejar VIS-01.

Ambiente de trabalho Unreal prioritariamente em inglês. Idioma do produto é separado: português continua objetivo de localização a definir e validar, sem depender do idioma do editor.
