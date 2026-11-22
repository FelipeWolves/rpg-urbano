# AUT-03 — entrada comum de mouse e toque

21/11/2026. Código e compilação concluídos na pasta principal nova. Sem abertura do editor, preview, emulador ou Android. Aceites visual e físico continuam pendentes.

## Comportamento implementado

- `PointerInput` transforma amostras de mouse e toque num fluxo comum com pressionar, manter, soltar, toque curto e cancelamento.
- Um gesto tem uma única origem. Toque tem prioridade no início simultâneo; após terminar, exige-se nova pressão de mouse fora da janela de proteção de0,35s. Mouse mantido durante esse intervalo não vira um novo clique quando o prazo termina.
- Botões só aceitam toque curto iniciado e terminado dentro da sua área. Soltura sem pressão conhecida, movimento acima de12px, coordenada inválida ou tempo inválido não produzem clique.
- Perda de foco, revisão de sessão ou mudança entre hub/painel/arena cancelam o gesto. Depois de cancelamento com botão pressionado é necessário observar todos soltos antes de outro gesto. Ao retornar de uma janela sem foco com o botão já pressionado, ele não ativa a interface.
- Toques de0,5s ou mais não viram cliques. A arena preserva AbilityGesture para consulta prolongada de habilidades; consulta não executa habilidade. Cancelamento de ponteiro também cancela essa consulta pendente.
- Hub e arena usam o mesmo coletor. Seleção3D passa a usar a posição do ponteiro comum, em vez de depender exclusivamente do cursor de mouse.
- Automação da arena não avança enquanto houver gesto ativo. Isso não implementa um sistema geral de pausa/abandono.

Regras de combate, StandardShield, R04, recompensa e dados dos personagens não foram alterados. Não se habilitou mouse como toque nas preferências globais da Unreal.

## Arquivos e verificação

API e implementação neutras em startup_presentation.hpp/.cpp; adaptação no HUD próprio. Polling lê o mouse e o primeiro toque uma vez por DrawHUD, verifica foco do viewport e usa contexto de revisão/tela. Coordenada final do toque é preservada para processar sua soltura. Não é reconhecimento de pinça/multitoque nem uma fila completa de eventos de plataforma.

10 grupos novos: mouse, toque com mouse simultâneo, janela contra mouse sintético, arraste, revisão/tela, foco/retorno, metadados inválidos, duração prolongada, transferência de origem e origem/destino do clique. Resultado:44 grupos de apresentação aprovados (34 anteriores +10 novos). A bateria de combate não foi repetida, pois o resolvedor não mudou.

Compilação inicial da integração:8 ações aprovadas; após incluir o caso de retorno ao foco, recompilação final4 ações, saída0. Logs locais `build/aut03-build-before-focus-edge.log` e `build/aut03-build.log`. Não houve execução gráfica: testes nativos não comprovam foco/coordinates/toque reais dentro do editor ou em aparelho. Aviso conhecido de MSVC não preferido permanece.

## Limites e próximo bloco

Limiares de0,5s,12px e0,35s são provisórios de apresentação, não relógios de batalha. São pixels do viewport; DPI e acessibilidade exigem avaliação posterior. Eventos completos ocorridos entre duas amostras, multitouch, retorno real do aplicativo e variações de emulação mouse/toque ainda precisam de validação apropriada. A deduplicação provada é a do contrato amostrado, não uma garantia universal do sistema operacional.

O usuário poderá testar clique curto, detalhes por pressão longa, arraste para fora e retorno à janela. Em Windows sem simulação de toque, esses testes não validam o touch físico. Android continua fora da fila imediata.

AUT-03: núcleo e adaptador compilados; aceite interativo pendente. Próximo AUT-04: leitura de painéis, quebra de texto, paginação e consulta sem disparar ações. Progresso geral mantido aproximadamente22% até os aceites; não antecipar conclusão mobile ou VIS inteiro.
