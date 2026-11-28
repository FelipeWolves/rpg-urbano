> **23/11/2026.** Enquadramento e deslize aprovados no teste Windows. O molde com cores provisórias e a rotação do panorama foram confirmados no Pixel 7 virtual. A cena editável e os materiais finais seguem na [fila de desenvolvimento](FILA-DESENVOLVIMENTO-INDEPENDENTE.md).

# Prévia 3D da Praça e do Terminal

Abra Abrir-Preview-Hub3D.cmd para testar a cena em uma janela. O atalho normal continua abrindo o panorama. A prévia usa uma área separada do mesmo mapa, construída em tempo de execução com formas básicas da Unreal. A imagem atual serviu de referência de composição. O panorama e as fachadas detalhadas ficam para outra etapa.

Clique em TERMINAL para aproximar a câmera e abrir os bilhetes. Os controles de setor e o atalho 2D estão no [roteiro do Terminal 3D](2026-11-28_terminal-3d-setores-atalho_v01.md). A seta Voltar externa retorna ao Hub. A seta interna do atalho retorna ao setor do Terminal. Observe quanto a fonte e o banco se deslocam em relação à fachada durante a aproximação. A opção de movimento reduzido faz o corte de câmera imediato.

Confira também Voltar ao hub após uma batalha do Terminal, o clique único e a saída livre do cursor pela borda da janela. Esses passos compõem o roteiro de reteste da prévia.

Compilação Win64 Development aprovada e 102 arquivos de preparação verificados. O halo do panorama anterior foi aceito pelo usuário.

## Alternância e molde completo

Na tela inicial, use **Ver hub 3D** e **Voltar ao panorama**. A escolha vale durante a sessão. O atalho dedicado continua iniciando diretamente em 3D.

O molde cobre Praça, Terminal, Biblioteca, Lab - Exp, Games, Arena PvP e Pub. A fonte e os canteiros ficam em primeiro plano, o Terminal ocupa o centro e a ala direita tem uma plataforma elevada e escada. O panorama do projeto orienta a posição relativa dos volumes. Os letreiros e os atalhos fixos abrem as mesmas rotas do hub normal. A aproximação de câmera continua no Terminal, e destinos ainda em desenvolvimento mantêm os painéis informativos existentes.

No reteste, alterne duas vezes, confira os sete destinos, volte da batalha do Terminal e confira a composição em janela estreita.

## Composição panorâmica

Praça no primeiro plano à esquerda, Biblioteca acima dela, Terminal no centro superior, trilho elevado avançando pela direita e Arena PvP abaixo dele. Pub no canto inferior direito, com acesso por degraus a um piso rebaixado. A câmera de abertura foi recuada e ampliada. Compilação aprovada, 102 arquivos verificados. Enquadramento em jogo aguarda aceite.

## Movimento da imagem panorâmica

Na tela 2D, Esquerda, Centro e Direita deslizam o recorte em 450 ms. Os letreiros usam o recorte exibido em cada quadro e seus cliques ficam suspensos durante a passagem. Com Movimento reduzido, a troca é imediata. Para o reteste em janela vertical, alterne as áreas, clique rapidamente em outra direção e confira que os letreiros acompanham a imagem.

### Panorama experimental no Android

A prévia 3D compartilha a regra do Hub 2D no Android. Panorama solicita paisagem e Focar área volta ao retrato. A experiência horizontal mobile é experimental. No PC/Windows, a visão ampla continua disponível na janela do jogo. [Contrato Android](ANDROID-DEPENDENCIAS.md#orientação-do-hub-e-da-abertura-no-android)

### Acabamentos provisórios dos sete destinos

O molde tem painéis azulados e perfis metálicos no Terminal, frisos e bases de colunas na Biblioteca, molduras violetas no Lab - Exp, piso em placas e bordas de pedra na Praça, anéis turquesa em Games, fachada gradeada na Arena e caixilhos âmbar no Pub. Os trilhos distinguem a estrutura escura das guias metálicas. São materiais de cor com detalhes geométricos provisórios.

Build do Editor Win64 Development aprovado por linha de comando. Aceite visual confirmado pelo usuário em 27/11/2026 para os acabamentos provisórios dos sete destinos, no recorte do panorama, das três áreas focadas e da aproximação do Terminal.

### Movimento ambiental do molde

Quatro sinais pequenos variam suavemente de intensidade ao longo de ciclos de 6,5 a 9 segundos, em fases diferentes. Estão junto ao Lab - Exp, ao Terminal, a Games e ao trilho à direita. Com Movimento Reduzido ou em menus, a intensidade fica estática. A presença 3D do Felipe segue adiada.

Para o aceite, abrir Ver Hub 3D no RpgEngineProbe, observar por dez segundos no panorama e repetir nas áreas focadas. Ativar Movimento Reduzido e conferir que os sinais param de pulsar. Confirmar que o contraste dos letreiros e a seleção dos destinos continuam claros.

**Aceite visual confirmado pelo usuário em 28/11/2026.** A sinalização ambiental do Hub 3D foi aprovada no recorte testado.
