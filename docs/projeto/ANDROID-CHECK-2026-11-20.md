# Android — resultado da conferência de20/11/2026

**Situação: Android Studio instalado; ambiente de build/teste Android ainda não pronto nos caminhos/configurações verificados.** Checagem por arquivos/registro/variáveis e fontes oficiais; nenhuma ferramenta Android executada. Não instalou, configurou, aceitou licenças ou iniciou emulador/dispositivo. Não modifica PATH/JAVA_HOME/configuração da engine.

## Resultado

| Componente | Evidência | Estado |
|---|---|---|
| Android Studio | D:\Android; registro Path confirma local; programa e product-info.json presentes; registro2026.2.2.9 | Instalado |
| Java do Studio | jbr/release:25.0.4;bin/java.exe presente | Presente,mas fora da matriz suportada do Gradle8.13 da engine |
| SDK | HKLM\SOFTWARE\Android Studio:SdkPath vazio;semANDROID_HOME/ANDROID_SDK_ROOT configurados nos escopos consultados;C:\Users\Ergo_\AppData\Local\Android\Sdk não existe | Não localizado;isso não exclui cópia personalizada em outra pasta |
| NDK | SemNDKROOT/NDK_ROOT configurados;SDK não localizado | Não localizado/configurado no escopo examinado |
| AVD | Pasta padrão C:\Users\Ergo_\.android\avd não existe;semANDROID_AVD_HOME/ANDROID_USER_HOME alternativos observados | Nenhum perfil localizado;emulador não iniciado |
| Preferências locais | ConfiguraçãoRoamingGoogleAndroidStudio não localizada;caminhosAndroidSDK/JDK não encontrados nos arquivosUnreal consultados | Nenhum override útil identificado;busca limitada aos locais documentados |
| SuporteAndroidUnreal | Engine/Binaries/Android contém UnrealGame-arm64.so eUnrealGame-x64.so;Engine/Platforms/Android presente | Binários para as arquiteturas presentes;isso não valida linker,SDK,APK ouAVD |
| Gradle da engine | Engine/Build/Android/Java/gradle/gradle/wrapper/gradle-wrapper.properties:8.13 | Versão local confirmada por arquivo,sem executar/download |
| AndroidGradlePlugin | UEDeployAndroid.cs define com.android.tools.build:gradle:8.13.0 ecompilaçãoJava17 | Versão efetiva indicada no código da engine;template3.5.3é fallback e não deve ser tomado como versão do projeto |

## Incompatibilidade identificada

OStudio trazJava25,mas a engine usaGradle8.13. Na [matriz oficialGradle](https://docs.gradle.org/current/userguide/compatibility.html),suporte para executarGradle comJava25 começa em9.1.0. Logo não usar automaticamente D:\Android\jbr comoJDK do buildUE nesta combinação. Não houve tentativa de build para reproduzir falha: conclusão de compatibilidade por versão, não erro runtime observado.

O [AGP8.13 documentaJDK17](https://developer.android.com/build/releases/agp-8-13-0-release-notes?hl=en). Preparar umJDK compatível separado para o build (JDK17é candidato alinhado aoAGP/engine),sem substituir oJBR necessário aoStudio e sem atualizarGradle/AGP/código da engine por conta própria. Conferir toda a cadeia antes de configurar caminhos. Instalação/seleção ainda não realizada.

A fonte da engine tenta localizarStudio/JBR pelo registro. SetupAndroid.bat também pode definirJAVA_HOME do usuário. Por isso **não executar esse instalador automaticamente**: ele pode selecionar oJava25 e escrever preferências globais.

## Referência de componentes da engine instalada

Android_SDK.json daUE5.8.4 especificaAPIandroid-36,BuildTools36.0.0,CMake3.22.1,NDK27.2.12479018(r27c;faixa até r29). Esses componentes não foram encontrados/validados na máquina por esta checagem; são alvos para o próximo plano, não instalação concluída.

## Ordem do trabalho seguinte

1. Verificar se há SDK personalizado já escolhido;senão definir pasta e pacote de componentes necessários. Não reinstalarStudio.
2. Com autorização específica,obter/configurarSDKPlatform36,BuildTools36.0.0,PlatformTools,CommandLineTools,NDK27.2.12479018 e demais ferramentas necessárias ao alvo. Emulator/system-image só para o ensaioAVD;nenhuma licença aceita nesta tarefa.
3. DisponibilizarJDK compatível separado para o buildUE,preferindo configuração restrita ao projeto/processo antes de qualquer mudança global.
4. PlanejarAVD comABI/API/renderizador compatíveis com o binário de teste;existem bináriosUEarm64/x64,mas a arquitetura doAPK deve corresponder aoAVD. Confirmar virtualização/memória/GPU apenas quando necessário;nenhum ajuste dessas opções agora.
5. Só então executar diagnóstico de toolchain,buildAndroid mínimo e ensaio de entrada/retrato/retomada. Não anunciar suporte mobile por Studio ou arquivos.so presentes.

Pode-se continuarAUT-01–04 noWindows enquanto isso. A coleta de decisões de combate é independente dessa preparação. Percentual mobile mantido;nenhum APK ou testeAndroid executado.

## Limites da busca

Foram consultados locais padrão,registroStudio,variáveisAndroid/JAVA_HOME pertinentes e alguns diretóriosAndroid próximos da instalação,sem varrer todas as unidades. Locais padrão foram reconferidos em leitura fora da restrição inicial de acesso. Instalação em caminho personalizado não informado ainda pode existir. Não se conclui ausência global de todos osSDKs/JDKs na máquina.
