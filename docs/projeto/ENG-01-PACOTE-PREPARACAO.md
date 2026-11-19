# ENG-01 — preparação do primeiro pacote Windows

19/11/2026. **Preparação concluída; compilação, cook, pacote e execução não realizados.** Não altera o aceite anterior nem comprova distribuição.

## Escopo preparado

- Alvo `RpgEngineProbeTarget`, tipo Game, Win64/Development; mantém V7/ordem de includes UE5.8 e módulo existente. Não cria gameplay novo.
- Configuração explícita de mapa inicial e lista de cook para `/Engine/Maps/Templates/Template_Default`, recurso interno da engine já observado no editor. A inclusão e funcionamento desse mapa no pacote ainda precisam de teste; não copiar recursos para o repositório.
- Configuração do projeto de avaliação solicita opt-out de analytics do editor. Isso não garante ausência de tráfego nem altera a rede. Não é uma política completa de telemetria do produto.
- Script `scripts/preparar-pacote-unreal.ps1`: valida versão instalada, copia 22 entradas canônicas para uma pasta nova em build/engine-package e compara SHA-256 de origem/destino. Não invoca executáveis externos, compilador, editor, UAT ou UBT. Gera somente plano JSON de futura invocação e relatório de preparação.
- Stage anterior em build/engine-eval e evidências anteriores preservados. O script antigo testar-unreal.ps1 continua sendo a rota de build do editor/ponte, não a rota desta preparação de pacote.

## Ferramentas verificadas sem execução

Presentes na instalação autorizada: dotnet distribuído com a engine, AutomationTool.dll (UAT), RunUAT.bat e UnrealEditor-Cmd.exe. O arquivo UEPrereqSetup_x64.exe não foi encontrado no caminho tradicional; vc_redist.x64.exe está presente. Nada foi instalado ou executado. A presença desses arquivos não comprova a configuração do compilador, todos os pré-requisitos de uma máquina limpa ou sucesso de empacotamento.

O plano propõe UAT BuildCookRun com build/cook/stage/pak/archive, Win64/Development e limite solicitado de duas ações UBT, sem executores distribuídos solicitados. A flag nocompile diz respeito aos scripts da automação; não significa ausência de compilação do projeto. O plano precisa ser executado e validado em etapa autorizada; não é comando já aprovado ou testado. Nenhum pacote Shipping, assinatura, instalador ou distribuição pública faz parte desta fase.

## Idioma e dados

Priorizar inglês para a ferramenta/cook (`-culture=en` na opção da etapa de cook) e para a futura execução de diagnóstico do pacote. Não alterar idioma do Windows. O inglês usado nas ferramentas não impede o jogo em português: localização do produto continua separada, a definir e validar com textos traduzíveis, fontes e formatos regionais. Não testar localização apenas trocando o idioma do editor.

O primeiro pacote é uma cena técnica, sem UI de combate, menu, seleção de alvo ou integração visual do núcleo. A ponte compilada não prova que o núcleo será acionado em gameplay; a opção RpgCoreProbe continua opt-in. Não usar saves do jogador nem copiar DLLs do console. Não adicionar rede, contas, serviços ou conteúdo de outro jogo.

## Verificações efetivamente realizadas

Análise sintática PowerShell sem execução de compilação; preparação executada somente em PowerShell; 22 entradas copiadas com hashes iguais; JSONs analisados e conferidos. Alvo C#, configurações da Unreal e comando de empacotamento ainda não foram avaliados pelo UBT/UAT. Não repetidos os 213 testes standalone nem os 11 checkpoints commandlet.

[Resultado e manifesto](../relatorios/atuais/ENG-01-PACOTE-PREPARACAO.json). O caminho do plano completo consta no JSON; arquivos gerados e logs futuros ficam em build ignorado. Plano não contém segredos nem argumentos de autenticação. Logs integrais futuros precisam de revisão antes de publicação.

## Próxima autorização e critérios

Próxima etapa requer autorização específica para PowerShell invocar dotnet/UAT/UBT, compilador MSVC/linker, UnrealEditor-Cmd/UHT e compiladores de shader/ferramentas de pacote da instalação existente, com caches normais da engine. Nenhum download/instalador/reconfiguração de rede está previsto. Se faltar componente, interromper e apresentar o diagnóstico.

Após autorização: revalidar manifesto/versão, executar o plano com saída e código capturados, diretório exclusivo e limite de tempo, sem abrir o pacote automaticamente. Critérios: sucesso build/cook/stage/archive, executável e conteúdo presentes e inventário resumido de arquivos/tamanho. Só uma autorização posterior permitirá abrir o executável próprio, conferir mapa/renderização/encerramento e coletar log. Sucesso nesta máquina não comprova pré-requisitos em outro computador. A arena VIS-01 vem em etapa separada.
