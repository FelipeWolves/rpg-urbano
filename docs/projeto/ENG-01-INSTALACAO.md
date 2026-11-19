# ENG-01 — instalação confirmada por metadados

Relatório da fase indicada. O aceite posterior da execução está em [ENG-01-RUNTIME.md](ENG-01-RUNTIME.md); pendências mencionadas aqui podem ter sido resolvidas nessa etapa.

18/11/2026. Verificação somente em leitura da pasta fornecida pelo usuário. Nenhum editor, launcher, UBT, script de build ou componente da engine foi executado; nenhum arquivo da instalação foi alterado.

## Confirmado

- Raiz efetiva: D:\Unreal Engine\UE_5.8.
- Build.version: 5.8.4, changelist58902052, compatible55116800, build promovida da linha Release-5.8.
- UnrealEditor.exe, UBT e Build.bat presentes; marcador de installed build presente. Lista pontual de componentes em ENG-01-INSTALACAO.json.
- UBT declara net10.0 e framework10.0.0. Runtime dotnet empacotado em Engine/Binaries/ThirdParty/DotNet/10.0/win-x64.
- O arquivo local MicrosoftPlatformSDK.Versions.cs delega requisitos a Engine/Config/Windows/Windows_SDK.json; essa configuração instalada é a referência para o próximo teste.

## Compilador e SDK reais dessa instalação

Preferências MSVC: 14.50.35717–14.50.99999 e14.44.35207–14.44.99999. Há subfaixas proibidas dentro dessas famílias: conferir o JSON, não instalar uma versão apenas pelo prefixo. Mínimo14.38.33130.

O MSVC14.52.37025 disponível está acima do mínimo e fora das faixas proibidas listadas, porém não é preferido. Isso não demonstra que irá compilar a engine/projeto. O14.29 também instalado está abaixo do mínimo. Não modificar configurações da engine para ignorar rejeições; a decisão sobre ferramenta adicional depende do resultado real do UBT.

Windows_SDK.json informa MainVersion10.0.22621.0 e intervalo19041.0–10.9.99999.0. O SDK instalado10.0.26100.0 está no intervalo declarado. Esse dado local é mais específico que a referência documental anterior; ainda falta confirmar qual versão será selecionada pelo build.

## O que continua pendente

Presença/versão não equivalem a integridade completa, execução correta ou suporte mobile pronto. Não houve hash completo, verificação do launcher, teste de GPU/editor, inventário de dependências transitivas ou certificação de Android/iOS.

Próximo incremento: preparar um projeto C++ de avaliação separado, revisar seus arquivos e compilar com UBT em ambiente controlado. Primeiro validar compilação; abertura do editor fica em passo explícito posterior. Preservar o protótipo, testes e saves atuais; não mover projeto para dentro da instalação nem gravar nela por esta inspeção.

ENG-01 permanece aberto. Os213 testes standalone anteriores foram reaproveitados, sem nova execução por uma leitura de metadados. Evidência estruturada: ../relatorios/atuais/ENG-01-INSTALACAO.json.
