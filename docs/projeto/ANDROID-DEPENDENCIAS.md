> **Entrega concluída:** [projeto Unreal estável e guia de abertura](UNREAL-PROJETO-ESTAVEL.md). Controle de tela encerrado por solicitação; nova interação com o editor exige nova liberação. Atualização documental/Git apenas após essa solicitação.

> **Estado vigente — 21/11/2026:** [consolidação das 15 entregas e decisões posteriores](CONSOLIDACAO-2026-11-21.md). ENG-ESTAVEL concluído no Windows local: projeto fixo, mapa próprio, abertura/reabertura e Play observados. Próximo: AUT-01–04. ESC comum e seletor já aceitos não são reabertos. Android SDK/NDK/JDK localizados e boot do AVD confirmado pelo usuário; caminhos já persistem, mas ValidatePlatforms retorna Android INVALID r27c; diagnóstico e APK pendentes. Percentuais recalculados no painel de progresso. As notas datadas abaixo preservam histórico e não reabrem decisões substituídas.

# Android — instalação observada e dependências de teste

Conferência somente leitura em20/11/2026. Não foi aberto Android Studio, Java, adb, emulador, sdkmanager ou Unreal; nenhuma instalação, licença aceita, configuração, virtualização ou rede modificada.

## Evidência local

| Componente | Estado confirmado | O que isso não confirma |
|---|---|---|
| Android Studio | Registro do Windows:2026.2.2.9; DisplayIcon aponta D:\Android\bin\studio.exe. product-info.json e studio64.exe presentes em D:\Android; build262.10968.63.2622.16840690 | Compatibilidade do ambiente de build com Unreal 5.8.4 ou SDK completo |
| Java incluído | D:\Android\jbr\release informa25.0.4, JetBrains | Não executar nem selecionar esse Java para Gradle antes de verificar compatibilidade |
| SDK/NDK/AVD | Caminhos padrão consultados não encontrados; caminhos personalizados/configuração não identificados nessa checagem limitada | Não concluir que não existem em outro local; nem marcar AVD pronto |
| Unreal | Android_SDK.json da instalação5.8.4 foi lido: platformsandroid-36, build-tools36.0.0, cmake3.22.1, ndk27.2.12479018; faixaMain/Minr27c, Maxr29 | Valores locais são referência de conferência, não prova de SDK instalado/funcional; faixa permitida não é preferência por versão máxima |
| Dispositivo | Usuário informa S23 Ultra para etapa posterior; AVD8 GB como ambiente inicial desejado | Nenhum aparelho/emulador foi iniciado ou reconhecido poradb nesta entrega |

RegistroAndroid: instalação do programa confirmada. ToolchainAndroid pronto: **não confirmado**. AVD instalado/configurado: **não confirmado**. APK compilado/instalado: **não**. Desempenho mobile: **não medido**.

## Matriz de trabalho e critérios — execução futura

| Ordem | Trabalho nosso | Dependência | Evidência para encerrar |
|---|---|---|---|
|1| Localizar SDK/NDK/JDK e configuraçãoUnreal usando metadados locais, listar versões sem executar ferramentas quando possível | Instalação existente; nenhum novo download necessário para inventário | Caminhos/versões e divergências registrados, sem segredos |
|2| Comparar toolchain/Java/Gradle e suporteAndroidARM64 da engine instalada com requisitos da5.8.4 | Ordem1; conferir documentação/localBuild antes de mudar configuração | Matriz compatível, faltante ou não verificada; plano preciso antes de instalação |
|3| Preparar apresentação mobile testável no Windows | AUT-01–03; não exige AVD ou celular | Layout/entrada separados da plataforma, sem chamar simulação de APK |
|4| Conferir/criar perfil AVD quando autorizado | SDK/emulator/system-image; ABI compatível com binárioUE, aceleração virtualização/renderizador e memória do host | AVD inicia e é reconhecido; versãoAPI/ABI/GPU/RAM explícitas.8 GB alocados não equivalem a8 GB físicos do celular |
|5| BuildAndroid mínimo com mídia/arena provisória | Toolchain compatível, suporteAndroidUE e plano de assinatura de desenvolvimento | Saída de build, manifestoABI/API e tamanho; corrigir erro isoladamente |
|6| Instalar/abrir/encerrar no AVD compatível | Ordem4/5 e autorização de execução | Sem tela preta, controles/retrato/retomada/save; log resumido e limites. Não instalar apenas arrastando APK incompatível |
|7| S23 Ultra e aparelho mínimo representativo quando disponíveis | DepuraçãoUSB autorizada, ABI/renderizador compatível e consentimento para instalação | Áreas seguras, toque, memória, GPU, temperatura/bateria e30/60FPS medidos. S23 Ultra não certifica mínimo intermediário |

Nenhuma etapa acima foi executada por este plano. Instalação de sistema-imagem, componentesSDK/NDK ou ajustes de virtualização/rede exigem autorização aplicável; não usar bypass de política. Não reinstalarAndroid Studio apenas porque os caminhos padrão não apareceram.

## Pendências específicas

Localização real do SDK/NDK e da pastaAVD; versão da imagem/ABI/renderizador; suporteAndroid da engine; Java compatível; ambiente de assinatura de teste; memória disponível doPC. Não solicitar credenciais ou chaves privadas no repositório. Imagem mobile ausente no Windows já foi esclarecida como janela vertical da versãoCanvas; não é falha de umAPK que não foi produzido.

Metas4K/HDR/ultrarrealismo/física/presets são requisitos desejados a avaliar. LOD/downscale/AVD não garantemFPS, custo de decodificação de vídeo ou compatibilidade de todos os recursos. Não publicar mínimos de hardware antes de medições.

ConferênciaAndroid20/11 concluída em leitura:Studio D:\Android presente;SdkPath vazio,SDK/NDK/AVD não localizados nas configurações/caminhos examinados. Java25doStudio fora da matriz suportada doGradle8.13/AGP8.13.0daengine. [Resultado e próximos passos](ANDROID-CHECK-2026-11-20.md). Não instalar/configurar/executar automaticamente;buscar SDK personalizado ou preparar componentes/JDK compatível sob autorização. AUTWindows não bloqueado.

## Atualização após instalação manual —20/11/2026

SDK confirmado em **D:\AndroidSDK**. NDK27.2.12479018,CMake3.22.1,BuildTools36.0.0,PlatformTools37.0.3,Emulator37.3.10 eCommandLineTools presentes por arquivos/metadados. Também presentesCMake4.1.0 eBuildTools37.0.0. PorémSDKPlatforms contém apenasandroid-37.0;**platforms/android-36/android.jar não existe**. BuildTools36 não substitui SDKPlatform36. Ainda falta instalarAPI36estável para a base escolhida e confirmar caminho doJDK17 separado. Nenhuma pasta system-images noSDK;AVD não confirmado. Somente leitura,nada executado/configurado. O diagnóstico anterior deSDKnão localizado é histórico e foi superado por esta instalação.

API36 confirmada após nova instalação manual: D:\AndroidSDK\platforms\android-36\android.jar presente, ApiLevel36/revisão2. Pendência da Platform36 resolvida; restam confirmar caminho JDK17 e preparar/validar AVD e build. D:\Java\jdk-17 não localizado; isso não exclui instalação em outro caminho. Nenhuma ferramenta executada.

JDK separado confirmado por metadados: D:\AndroidJava\jdk-17.0.21.1+2, Eclipse Adoptium, Java17.0.21.1, x86_64; java.exe e javac.exe presentes. FamíliaJava17 alinhada ao build planejado; não executado. Pendência de localização doJDK resolvida. Próximo: indicar naUnreal os caminhos SDK D:\AndroidSDK, NDK D:\AndroidSDK\ndk\27.2.12479018 e esteJDK, depois validar toolchain/AVD. Nenhuma configuração realizada pelo assistente;buildAndroid ainda não testado.
