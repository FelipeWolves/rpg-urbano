# Terminal Multimodal 2D — referência visual recebida

Imagem composta recebida em 27/11/2026. Referência para a primeira interface de bilhetes em retrato. [Imagem recebida](../../entregas/05-hub-identidade-e-arte2d/recebidos/2026-11-27_terminal-multimodal-2d_v01.png).

A composição mostra um cenário ferroviário noturno, cinco bilhetes com picote e ilustrações, barra de recursos compacta e rodapé com Voltar, retrato, Rotas, Trilhas e Expedição. A arte foi recebida em uma camada. Os saldos, botões, sinais e a aba ativa da imagem são elementos da composição visual. Na interface, texto, áreas de toque, estados dos destinos e navegação precisam ser desenhados separadamente. Os números ilustrados não são saldos do jogo.

| Bilhete | Identidade | Primeira integração |
|---|---|---|
| Monotrilho | Roxo | Abrir seleção dos laboratórios disponíveis e preservar o tutorial padrão |
| Trem Regional | Azul | História e Trilhas, com estado Em breve |
| Ônibus | Verde | Chefes de equipamentos, com estado Em breve |
| Metrô | Laranja | Farm, recursos e Torre, com estado Em breve |
| Loja de Conveniência | Vermelho | Entrada reservada, com estado Em breve |

Preservar acesso a todas as sessões e o retorno ao Terminal. Rotas é a aba inicial. Trilhas e Expedição informam a indisponibilidade até o conteúdo entrar na fila. Créditos técnicos continuam separados de Ouro, Gemas e Notas Musicais. Usar a imagem para orientar a recriação dos componentes e retirar as margens brancas apenas das cópias de trabalho. A imagem composta basta para prototipar. Para acabamento, fundo, ilustrações e moldura separados são úteis se estiverem disponíveis.

Aceite futuro: abrir pelo Hub, rolar por mouse e toque, entrar nos modos existentes, voltar ao Terminal e ao Hub, conferir estados indisponíveis e legibilidade em retrato e em janela larga.

## Prévia funcional

O Terminal do Hub abre a lista de cinco bilhetes. A arte composta fornece os recortes dos modais em tempo de execução. O código desenha texto, controles e disponibilidade. Sem o PNG local, aparecem cartões com acento de cor. O empacotamento inclui o recurso quando presente. A barra comercial, o retrato dinâmico e a arte em camadas ficam para o acabamento.

A lista avança um bilhete por roda, deslize vertical ou botões de paginação quando a altura limita os itens visíveis. Monotrilho abre a seleção local existente. Em cada sessão, confira a saída para o menu do Terminal; no menu, Voltar retorna aos bilhetes e Voltar novamente retorna ao Hub. Repita a entrada a partir do Hub 3D. Verifique que os outros quatro bilhetes e as abas futuras não iniciam sessões. Testar em janela larga, estreita e em aparelho Android quando houver novo pacote.

## Arte principal em foco e mobile

Em Focar área e no mobile, a composição recebida é a base do Terminal. O recorte retira a barra superior com saldos ilustrativos e o rodapé com textos fixos. Os nomes, as descrições, os botões e as abas são desenhados a partir dos destinos disponíveis. O retrato da composição é provisório. Em janela larga no modo Panorama, a lista adaptável continua disponível. O jogo usa a lista adaptável como fallback quando a arte local está ausente.

No aceite, verificar que o Monotrilho é o único bilhete que inicia uma ação, nenhum saldo ilustrativo aparece, o botão Voltar regressa ao Hub e a arte se mantém legível com as margens e a proporção do aparelho.

## Tipografia do cabeçalho

Título em Roboto Bold Condensed com contorno lilás fino e subtítulo ciano. Cada linha usa uma imagem nítida da tipografia e um halo difuso independente. A oscilação afeta apenas o halo. Movimento Reduzido mantém o halo estático. O gerador local é scripts/preparar-halos-letreiros.ps1 -Terminal. O pacote prepara os quatro recursos quando presentes, com texto simples como fallback.

Aceite visual: comparar nitidez e alinhamento em Focar área e Panorama, aguardar um ciclo de brilho e repetir com Movimento Reduzido.

## Aceite do usuário

O usuário confirmou que o Terminal abre a tela correta conforme o parâmetro de apresentação. Aceite registrado para a seleção do layout no recorte testado no RpgEngineProbe. A validação desta entrega em um novo APK segue pendente.
