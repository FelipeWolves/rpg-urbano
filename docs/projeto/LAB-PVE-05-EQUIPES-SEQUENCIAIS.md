# LAB-PVE-05 — equipes sequenciais no Terminal

22/11/2026. Integração visual de SequentialSquads, sem alterar suas regras e sem abrir o jogo/editor. Aceites visuais de LAB-PVE-02–05 ainda pendentes. Demo ~28% mantida.

## Acessos

Terminal → Mais modos > → 3equipes — fases do chefe ou 3equipes — kit fixo do chefe. A segunda página mantém pontuação e sobrevivência; não aumenta a quantidade de linhas visíveis. As duas variantes sequenciais têm sessões independentes.

Cada equipe participa uma vez, em ordem. Se o chefe morrer antes, o coordenador encerra sem obrigar os times restantes a entrar. Se os três times terminarem sem derrotá-lo, não há retorno cíclico à primeira equipe.

## Fixture e personagens

Três equipes de uma unidade: Felipe, A1 e B1. B1 é aliado apenas neste teste; sua definição no laboratório genérico continua inimiga. Atributos, kits, condições e recargas dos personagens permanecem os já existentes. IDs de personagem e instância são distintos entre os três times.

Chefe próprio CHEFE-SEQ: 12.500PV, ATK1800 e SPD10, habilidade CHEFE-SEQ-A1 derivada do ataque básico já usado pelo eco. É dado de laboratório para compatibilizar personagens cuja SPD está em escalas diferentes, não atualização do personagem ECO-1 nem balanceamento final.

Janela local de4ações aceitas por confronto/equipe, incluindo ações do chefe; não significa quatro turnos do aliado. Ao esgotar a janela com chefe vivo, o coordenador troca a equipe. Limite global200ações/empate. HP do chefe continua entre equipes, sem restauração inventada na UI.

Na variante fixa, boss_phase_id é stable e a habilidade permanece igual. Na variante com fases, SEQ-1/2/3 usam a mesma identidade de habilidade com power_ratio10000/12500/15000 e KeepMatching para recargas. PreserveSpecialBordersV2, versão2, mantém o contrato classificado do coordenador. Não foram alteradas regras globais de dano ou exceções de personagem.

## HUD, comandos e transições

SequentialVisualSession observa o coordenador e encaminha skills pelos preparadores Team compartilhados. Nenhum Battle.apply direto na interface. HUD separa equipe atual/total, janela local, ações globais, PV do chefe e fase. Detalhes mostra próxima equipe, equipe/fase anterior e estado final real.

Revisão de entrada cresce após toda ação, inclusive na fronteira; ator ou gesto da equipe anterior não pode agir na seguinte. Mudança de squad_index reconstrói as esferas mantendo as correções de mobilidade e marcador. Cues continuam sendo das ações que os geraram; não há animação final da transição.

Somente BossDefeated é objetivo cumprido. SquadsExhausted, GlobalActionLimit, TechnicalLimit e MutualElimination são apresentados como estados distintos, respeitando o resultado do núcleo. Chegar à equipe3não é vitória por si só.

## Isolamento e limites

Pausa/abandono/resultado/nova tentativa/retorno seguem o fluxo existente; recomeçar restaura equipe1e HP inicial do chefe. Nada é persistido nos saves dos outros laboratórios, e não há score/recompensa automática neste corte. O wrapper ScoreSequence existente no core não foi integrado à UI por esta entrega.

O adaptador recusa loadouts de passivas BattleStart até existir a ponte explícita de runtime de abertura no coordenador. A fixture foi escolhida sem remover passivas dos personagens: Felipe usa A2 reativa; A1/B1 mantêm ativas. A2/A3 genéricos com abertura continuam disponíveis no laboratório normal, sem mudança. Essa extensão é trabalho de código posterior, não nova pesquisa obrigatória do usuário.

Os perfis de chefe deste teste preservam IDs de skills. Troca de IDs/slots exige mapeamento explícito; não foi declarada pronta. Não há servidor, paralelismo ou modo clássico novo.

## Testes

SeqVisual13 cobre identidade/afiliação provisória, pureza, troca de equipe/kit, revisão/ator antigo, esgotamento sem ciclo, vitória antecipada, kit fixo, força por fase, equivalência com SequentialSquads, limite global, abandono/reset, conclusão/isolamento e catálogo inválido. Regressões Phases/Scored/Transitions, build e stage em LAB-PVE-05.json.

## Roteiro de aceite

1. Abrir a variante com fases na página2doTerminal. Conferir equipe1/Felipe e CHEFE-SEQ, objetivo/PV e contadores.
2. Para observar mais trocas, usar A1; uma habilidade mais forte pode encerrar antes, o que é válido. Ao atingir a janela local, deve aparecer A1na equipe2e B1na equipe3.
3. HP do chefe deve continuar; ações locais reiniciam e globais continuam. A fase deve seguir SEQ-1→2→3; conferir próximas/equipe anterior nos detalhes.
4. Repetir no kit fixo: fase stable não deve forçar troca de habilidade.
5. Observar derrota/limite/vitória conforme o ocorrido, sem anunciar sucesso só por chegar à última equipe. Consultar resultado não concede recompensa.
6. Testar abandono, nova tentativa e volta aos modos anteriores. Reportar resolução/sequência se houver corte, esferas ausentes ou seleção incorreta.

Próximo: LAB-PVE-06, frentes locais e chefe compartilhado. Arte, áudio e dados finais podem chegar em paralelo. Sem telas/Android/rede/instalações/cook nesta entrega.
Resultado: SeqVisual13/13, Phases11/11, Scored13/13 e Transitions13/13 aprovados. Build inicial54,28s; incremental de clareza da janela22,30s; stage68 verificado, sem cook/execução. Avisos conhecidos do toolchain/header da engine permanecem, sem instalação de ferramentas.
