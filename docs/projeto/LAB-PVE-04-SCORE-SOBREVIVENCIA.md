# LAB-PVE-04 — pontuação, sobrevivência e recibo

22/11/2026. Novos acessos locais no Terminal. Código/testes/compilação, sem executar o jogo ou controlar tela. Aceites de LAB-PVE-02/03 ainda não informados; esta entrega também precisa de teste visual. Estimativa da demo mantida ~28%.

## Acesso

Hub → Terminal → Mais modos >. Página2 tem Pontuação — janela de12ações e Sobrevivência —3turnos do chefe. Primeiros modos retorna à página1; Voltar retorna ao hub. Navegação mantém até quatro entradas e uma linha de controles por página, para não estender a lista além da tela. O Terminal reabre na página1.

## Parâmetros provisórios explícitos

Todos os modos usam o mesmo funil Team e kit Felipe; regras de dano/efeitos/Block/recargas não foram copiadas ou alteradas. Cada modo pode declarar sua janela/objetivo, conforme PveDefinition já implementado.

- Pontuação: ScoreAttack com janela de12ações aceitas e resultado de limite Draw na configuração. Ao terminar, o controlador informa WindowClosed, não vitória de Ritual.
- Sobrevivência: meta de3turnos naturais do chefe ECO-1, relógio NaturalOnly; score_enabled para observar dano em paralelo. Limite de segurança de200ações/empate da base comum preservado.
- Nos dois testes, ECO-1 tem1.000.000PV para a janela/meta poder ser observada; ATK/SPD e kit de Felipe/ecos preservados. ECO-2 mantém PV independentes.
- Sem bônus de sobrevivência ou faixas de recompensa novos configurados nas fixtures. Valores são testes, não balanceamento ou conteúdo final.

Laboratório genérico, piloto separado, Ritual simples, Ritual com fases e ondas continuam com suas próprias sessões. A janela12não altera o teto das outras batalhas.

## HUD e resultado

HUD exibe score parcial/encerrado. Sobrevivência separa turnos naturais do chefe de ações aceitas e apresenta a meta3. PveEncounter é autoridade de encerramento: pode concluir sobrevivência com Battle ainda Running. Nenhum comando posterior é aceito pelo adaptador.

Detalhes/Resultado do encontro consulta score e recibo do ID da tentativa atual. Mostra valor consolidado, flag survived e faixa quando existir. survived significa aliados vivos ao encerrar; não significa automaticamente objetivo cumprido. A derrota antecipada do chefe não cumpre a meta de sobrevivência neste contrato.

Score é o produzido pela política existente, inclusive dano elegível contra auxiliares; não é um placar calculado pelo HUD nem limitado ao chefe por suposição. Fórmula, pesos de dano/overkill e escala continuam no núcleo. Score de uma tentativa não é moeda.

## Recibo e isolamento

PveVisualSession expõe cópia opcional de ScoreReceipt e quantidade de registros para verificação. A consulta nunca chama settle. PveEncounter já consolida uma única vez; nova tentativa usa outro ID e score zerado. Getters não concedem item, moeda ou recompensa novamente. Faixa é metadado: o executor de economia comercial não foi implementado.

Abandono interrompe a sessão sem criar recibo do score parcial. Recomeçar não mostra o recibo da tentativa anterior como se fosse atual. Recibos anteriores permanecem apenas no ledger em memória; não foi criado arquivo de save ou histórico persistido. Sessões do score/sobrevivência e demais variantes têm donos separados, inclusive após move do adaptador.

## Testes

Scored13: parâmetros/kit comum, janela, consultas repetidas/imutabilidade, score parcial/recusa, sobrevivência, derrota, chefe derrotado antes da meta, abandono, novas tentativas, move seguro, faixa apenas informativa, isolamento e identidade de variante inválida. Regressões de Score, Ritual e Transitions registradas em LAB-PVE-04.json.

Sem mudanças de core de dano/resolução; novo material/arte/áudio não é necessário. Android, Shipping/cook, persistência comercial e servidores continuam fora do recorte.

## Roteiro de aceite

1. Terminal → Mais modos >: conferir os dois acessos e retorno à primeira página sem corte/duplo clique inesperado.
2. Pontuação: realizar ações, observar score parcial e janela12; ao encerrar, abrir/fechar resultado repetidamente. O score/recibo não deve aumentar por consulta.
3. Nova tentativa: score começa zerado, ID do recibo final deve mudar. Não alterar créditos/save do laboratório genérico.
4. Sobrevivência: contador avança somente em turnos naturais do chefe; concluir em3, com ações bloqueadas e resultado consultável.
5. Abandonar uma tentativa parcial e entrar de novo; não deve existir recibo concedido pelo abandono. Alternar com Ritual/ondas/piloto.
6. Reportar resolução e sequência se houver sobreposição. Compilação não é aceite visual do menu/HUD.

Próximo: LAB-PVE-05, equipes sequenciais e transições, sem depender de uma nova entrega para fixtures locais. Conteúdo/ícones/3D/áudio finais podem continuar em paralelo.
Resultado: Scored13/13, Score10/10, Ritual12/12 e Transitions13/13 aprovados; compilação55,36s e stage68 verificado, sem cook/execução. Avisos conhecidos do toolchain/header da engine permanecem, sem instalação de ferramentas.
