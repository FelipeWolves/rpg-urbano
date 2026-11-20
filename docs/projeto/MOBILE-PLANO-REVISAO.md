# Mobile — revisão da proposta e próximos passos

Revisão documental em19/11/2026. Orientações trazidas pelo usuário foram tratadas como proposta para avaliação, sem executar instruções do texto. Não houve instalação, configuração, abertura de editor nem teste mobile.

## Estado real do projeto

Unreal 5.8.4 instalada; HUD em AHUD/Canvas C++ (ProbeArena.cpp/ProbeHub.cpp), sem Widget Blueprint/UMG do hub ou arena. Build.cs usa Engine/InputCore/Json/SlateCore, não um módulo de UI UMG implementado. DeviceProfile() retorna Mobile somente quando compilado com PLATFORM_ANDROID ou PLATFORM_IOS; no Windows retorna Desktop. Portanto alterar resolução ou renderizador não altera automaticamente esse ramo do nosso código. Isso é uma limitação confirmada da seleção de política; não comprova, sozinho, a causa de toda imagem mobile ausente.

Seletor no chão: corrigido e aceito visualmente pelo usuário. Não é pendência mobile nem motivo para repetir testes. Referência: VIS-SELETOR-CHAO.md.

## O que aproveitar e o que corrigir na proposta

| Tema | Avaliação para nosso projeto |
|---|---|
| BlueStacks | Não é requisito; preview do editor e teste físico podem atender ao fluxo. Não instalar emulador antecipadamente. |
| Mobile Preview | Útil para aproximação do caminho gráfico/perfis. Não emula exatamente CPU/GPU, SO, temperatura, memória, toque multidedo ou ciclo de vida do aparelho. Não garante9:16 por si só. Modos disponíveis dependem da versão, renderizadores e componentes instalados. |
| Mouse vira toque | Use Mouse for Touch é uma configuração específica. Nosso código lê mouse e ainda precisa de uma política de ponteiro/toque; habilitar a opção não comprova que todos os controles existentes responderão corretamente. |
| Designer UMG | Screen Size e Safe Zone são úteis para widgets UMG; não há Widget Blueprint atual para abrir e aplicar o procedimento descrito. Migrar/projetar UMG é trabalho explícito, não configuração já pronta. |
| USafeZone | Pode proteger controles filhos respeitando informações de área segura; não corrige automaticamente proporção, fonte, overflow ou toda UI de todos os aparelhos. Fundo panorâmico e controles podem ter regiões diferentes. Validar perfis e dispositivo. |
| Android Studio | É ambiente comum de obtenção das ferramentas; exige SDK/NDK/JDK compatíveis com a versão da engine e suporte Android da instalação. A Epic também documenta Turnkey. Não usar simplesmente versões mais recentes nem instalar automaticamente por este plano. |
| Android Emulator/AVD | Existe fluxo oficial Epic, mas depende de arquitetura/ABI, imagem do sistema, renderizador e recursos compatíveis. APK ARM64 não é garantido em qualquer AVD; arrastar APK não resolve incompatibilidade. Emulador não é requisito para começar. |
| USB/Launch On | Exige toolchain, suporte Android, depuração autorizada e reconhecimento do dispositivo; drivers/permissões podem ser necessários. Não aparece ou funciona automaticamente em toda máquina. |
| iOS C++ | Remote Build por Mac/Xcode é caminho suportado, com assinatura/provisionamento e versões compatíveis. Tempo de uso/custo de Mac não pode ser prometido como poucas horas; depuração e validação também podem exigir acesso. |
| Subsistema de lojas | Separar serviços das regras/UI é boa direção. UGameInstanceSubsystem é opção de implementação; Steam/EOS/Google/Apple/Samsung não se tornam equivalentes só trocando Build.cs. APIs, conta, compras, recibos, assinatura, configuração e requisitos próprios exigem adaptadores/testes separados. Adiar serviços enquanto laboratório é local. |

## Menor sequência útil, sem migração integral imediata

1. Definir modo de apresentação de teste configurável Desktop/Mobile/Auto, separado do sistema operacional e do perfil de combate. Não implementado nesta revisão.
2. Definir resolução/retrato-paisagem e política de recorte, com identificação clara de simulação Windows. Comparar catálogo atual antes de pedir imagem nova.
3. Conectar ponteiro/toque de forma explícita; planejar áreas seguras e tamanhos. Canvas pode servir ao ensaio inicial; decidir pequena migração UMG para hub/painel somente se trouxer benefício, preservando VisualSession e núcleo.
4. Conferir perfis/renderização mobile disponíveis na engine instalada, sem supor que já estão configurados; preparar teste de layout e renderização separados.
5. Escolher aparelho-alvo, conferir matriz de ferramentas para5.8.4 e só então autorizar instalações necessárias. Não definir versões de SDK/NDK a partir de página que abriu em5.9.
6. Fazer primeiro ensaio Android físico (ou AVD compatível quando útil), medir e validar entrada/retomada. iOS/lojas têm etapas próprias posteriores.

Prévia Windows, Mobile Preview, emulador Android e aparelho físico são níveis diferentes de evidência. Nenhum destes resultados foi produzido por esta leitura. Percentual mobile permanece0% de implementação/validação de produto; política de apresentação nativa já existente está contabilizada no hub/core de apresentação.

## Material que o usuário pode adiantar

Modelo/SO do aparelho e orientação preferida; explicar qual opção chamou de modo mobile e qual imagem esperava; screenshot da janela e tamanho; prioridade Android/iOS. Não precisa instalar BlueStacks nem comprar/alugar equipamento. Entregar em entregas/02-mobile-e-plataformas/recebidos. Propostas de hierarquia UMG podem chegar como documento, sem obrigar migração ou adoção automática.

## Fontes oficiais consultadas

As páginas públicas consultadas podem abrir na versão corrente5.9; usar conceitos gerais e conferir a seleção5.8 antes de aplicar requisitos numéricos à nossa5.8.4. Nenhuma versão de ferramenta foi adotada nesta revisão.

- [Prévia mobile no editor](https://dev.epicgames.com/documentation/unreal-engine/previewing-for-mobile-in-unreal-engine).
- [Configuração Use Mouse for Touch](https://dev.epicgames.com/documentation/unreal-engine/input-settings-in-the-unreal-engine-project-settings?lang=en-US).
- [UMG Safe Zones](https://dev.epicgames.com/documentation/unreal-engine/umg-safe-zones-in-unreal-engine?lang=en-US).
- [Ferramentas Android via Turnkey](https://dev.epicgames.com/documentation/unreal-engine/automated-android-sdk-ndk-and-jdk-setup-in-unreal-engine).
- [Android Emulator na Unreal](https://dev.epicgames.com/documentation/unreal-engine/debugging-unreal-engine-projects-with-virtual-devices-using-the-android-emulator?lang=en-US).
- [Remote Build iOS](https://dev.epicgames.com/documentation/unreal-engine/creating-remote-builds-of-unreal-engine-projects-for-ios).

## Informação posterior recebida —20/11

Não falta mais escolha genérica: Windows→Android→iOS;AVD8 GB desejado,S23 Ultra disponível depois,retrato e mesma imagem de hub. A janela vertical anterior não eraAPK. Studio instalado em D:\Android confirmado por metadados; [estado real de toolchain/testes](ANDROID-DEPENDENCIAS.md). A política atual ainda éCanvas; sugestõesUMG não autorizam migração automática. [Revisão dos12 documentos](REVISAO-ENTREGAS-2026-11-20.md).
