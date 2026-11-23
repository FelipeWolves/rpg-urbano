# Halo desfocado e cursor livre — 23/11/2026

## Ajuste visual

Sete máscaras PNG dos glifos reais em Roboto Light, produzidas localmente via .NET. Blur Gaussiano separável sigma6/raio18 no alpha, margem24px e borda alpha0 verificada: a luz é difusa e não uma caixa. Máscara branca com alpha é tingida azul/ciano e desenhada aditivamente atrás do texto; o texto continua nítido numa única passagem. Foram removidas as cópias deslocadas de texto usadas no halo anterior.

Ganho nominal1,3 de brilho em repouso, fonte/tamanho/maiúsculas e hover progressivo preservados. Movimento reduzido não anima o fade. Alto contraste mantém sua apresentação explícita. Os sete arquivos estão em assets/runtime/ui/2026-11-23-label-halos-v01/, ignorados pelo Git; metadados e hashes em data/presentation/ui-runtime.json. Total26texturas, em vez de19. Conferência também compara rótulo/fonte/geometria para evitar halo antigo com palavra nova. Em runtime, máscara com texto divergente não é desenhada; o texto principal continua disponível.

Scripts: preparar-halos-letreiros.ps1 e verificar-ui-integracao.ps1. Não há instalação de fontes ou serviço externo. A prova local build/halo-plaza-proof.png inspeciona a máscara isolada sobre fundo escuro, não é captura da Unreal. Aparência/alinhamento no panorama aguardam aceite.

## Mouse

A inspeção da engine instalada identificou que FInputModeGameOnly::ApplyInputMode solicita LockMouseToWidget e movimento de alta precisão diretamente ao Slate. Alterar depois somente o enum da viewport para DoNotLock não desfaz aquele pedido de trava.

Substituído por GameAndUI com DoNotLock, cursor visível e foco explícito no SViewport; esse caminho libera a captura e a trava Slate. Mantida CaptureDuringMouseDown para receber soltura/gestos enquanto um botão está pressionado, sem confinar a posição do cursor. O código local de SceneViewport encaminha o primeiro mouse-down nessa captura temporária; não se depende de captura permanente para o primeiro clique.

DefaultInput.ini do projeto desativa captura ao iniciar e define captura temporária/DoNotLock. Nenhuma configuração global de Windows, rede ou engine alterada. A dependência Slate foi declarada no módulo para usar a API pública do viewport; nenhum plugin foi instalado. Não foi executado código para recortar/confinar cursor pelo sistema operacional.

## Evidências e limites

- Compilação Editor aprovada, sem executá-lo; stage101entradas/hash/include aprovado.
- 10/10 testes existentes de ponteiro: foco, ownership mouse/toque, deduplicação, revisão, arrasto e hold. Isso é teste do modelo nativo, não de confinamento físico ou entrega de eventos pela janela Unreal.
- 26texturas conferidas por hash/dimensões, sete máscaras/textos e quatro glifos portugueses validados.
- Nenhum combate/save/arquivo de mídia original modificado; nenhuma tela ou Android aberto.

## Reteste dirigido

1. Iniciar em janela e mover o ponteiro para fora sem Shift+F1. Repetir depois de clicar num botão e após perder/recuperar foco da janela.
2. Confirmar clique único nos menus/na seleção da batalha e o gesto de segurar. Arrastar para fora e soltar não deve disparar ação na volta.
3. Conferir halo atrás das letras, sem contorno duro/caixa e sem engrossar o texto. Passar/retirar o mouse deve manter a mudança suave.

Esse relato reabre o aceite de liberdade do cursor; aceites anteriores de logo/PRAÇA/família tipográfica permanecem. Não afirmar correção física validada até o usuário testar. Demo~28% mantida.
