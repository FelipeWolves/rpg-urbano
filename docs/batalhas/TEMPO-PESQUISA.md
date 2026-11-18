# Tempo e ações — pesquisa antes do novo núcleo

Consulta: 18/11/2026. Pesquisa pública dirigida em sites oficiais, guias e wiki; sem acesso a cliente, contas ou serviços privados. Resultado: há base para abandonar uma ação por vivo por rodada como único modelo. Não há base para declarar reprodução exata do algoritmo interno ou de todos os modos atuais.

## Evidências e nível de confiança

| Tema | Resultado | Classificação / fonte |
|---|---|---|
| AP, SPD e alterações de barra | Existem absorção, troca de AP e condições por SPD; troca não é necessariamente classificada como aumento/redução | Primária histórica: [ajustes 3.2.6](https://dislyte.farlightgames.com/detail_06_28.html) |
| Turno extra versus ataque auxiliar | Documentos distinguem turno adicional, assistência e perseguição; não devem virar um único evento | Primária histórica: [ajustes 3.1.7](https://dislyte.farlightgames.com/detail639c38bb8964d55c96984498.html) e [Divine Gates](https://dislyte.farlightgames.com/detail_12_21.html) |
| Gatilhos de recarga | Há alterações condicionais e limitações por turno, além da recarga da habilidade | Primária histórica: [ajustes 3.2.6](https://dislyte.farlightgames.com/detail_06_28.html) |
| Progressão natural da barra | Guias descrevem enchimento por SPD, seleção ao atingir 100% e pausa durante a oportunidade ativa | Secundária, hipótese de referência: [guia de AP](https://playdislyte.com/?p=2442) |
| Recarga natural | Guia descreve redução ao terminar o turno do dono | Secundária, requer confirmação de fronteiras e exceções: [guia de combate](https://playdislyte.com/?p=1600) |
| Extra sem reduzir recarga | Wiki relata exceção de equipamento; páginas/épocas divergem em percentuais. Não generalizar para todos os extras | Secundária, pendente: [AP Modification](https://dislyte.fandom.com/wiki/AP_Modification) |
| Limite geral | Reprodução de notas 3.1.7 registra limites por modo/dificuldade. Não especifica integralmente quais eventos incrementam cada contador | Secundária histórica: [notas reproduzidas](https://playdislyte.com/?p=5580) |
| ATB | Square Enix descreve o ATB de FFIX como combate que progride em tempo real | Primária: [sistema de FFIX](https://www.jp.square-enix.com/ff9/system/other.html) |

Limitação de acesso: abertura direta das três páginas históricas oficiais retornou principalmente o invólucro da página. Seus textos de ajustes estavam disponíveis no índice da pesquisa. Assim, a evidência primária é histórica e indexada; não foi possível conferir o artigo integral diretamente nem garantir que descreva o balanceamento atual. Fontes secundárias são pistas, não documentação técnica oficial. Não usamos relatos de fórum como especificação aprovada.

## O que a pesquisa responde

O documento de 3.1.7 descreve concessão de turno extra e assistência vinculada a ataque aliado. O de Divine Gates chega a alterar uma perseguição para concessão de turno: evidência particularmente útil de que são categorias distintas. O de 3.2.6 registra perseguições e modificadores de recarga limitados por turno. Esses exemplos fundamentam eventos separados; não estabelecem toda a ordem de resolução.

Sobre o relato de um personagem agir durante o turno de outro: pode ser ação auxiliar, reação ou turno adicional, dependendo da habilidade. A animação de um ataque não demonstra por si só que ocorreu um ciclo completo de turno do atacante. Precisamos registrar origem e contexto de cada ação.

O contador geral não pode ser presumido como uma rodada na qual todos agem uma vez. Também não está comprovado que seja um relógio universal inalterável comum a todos os modos. A existência de limites não informa se extras, bloqueios e reações são contados. Essa parte permanece aberta, sem invalidar o relato do usuário de contadores distintos.

## Modelo proposto para o nosso RPG

Esta seção define proposta própria, não transcrição do Dislyte.

1. **Seleção da próxima oportunidade**: módulo de tempo substituível. Perfil de referência atual por rodada fixa permanece para regressão; perfil prioritário passa a barra por velocidade. Um perfil ATB poderá receber avanço explícito de tempo lógico e regras de pausa. A mesma habilidade não deve escolher seu próprio scheduler.
2. **Contexto de turno**: ID do turno, dono, origem (natural/extra), turno que o originou e flags de consumo. Uma assistência fica vinculada à ação causadora e não cria automaticamente outro turno do assistente.
3. **Ações**: básica, habilidade ativa, guarda opcional e ações auxiliares. Guarda continua disponível por configuração do nosso modo; não é tratada como comando original do Dislyte. A duração da guarda termina na oportunidade do dono conforme o perfil, não numa rodada presumida.
4. **Eventos de ciclo**: início de oportunidade, antes/depois da ação, por acerto, derrota, fim de oportunidade e conclusão de confronto. Animação e FPS não fazem avançar esses eventos.
5. **Relógios de efeitos**: dono, aplicador, oportunidade natural, oportunidade extra, ação causadora ou evento geral do modo. Cada efeito declara fronteira e política. Efeito criado no fim de turno não deve perder duração imediatamente sem regra expressa.
6. **Recargas**: declarar o evento que reduz, se extra/bloqueio reduz, quando uma habilidade recém-usada começa a contar e imunidade a modificação. Recarga e duração de buff são estados independentes.
7. **Limites**: separar ações executadas, oportunidades naturais/extras, eventos derivados e contador exibido pelo modo. TIME-01/WAVE-01 contam comandos aceitos no perfil atual; não chamá-los de contagem fiel do Dislyte. Manter orçamento técnico contra ciclos infinitos separado da derrota por regra.

Uma barra de prontidão e um recurso de energia/MP são conceitos distintos: gastar energia numa habilidade não precisa zerar a barra de prontidão. ATB também não é apenas trocar o nome de SPD; envolve avanço durante espera, pausas de menu, fila de prontos e eventual tempo de execução. Ainda não foi estudada a fórmula de FFVIII/FFIX nem implementado ATB em tempo real.

## Experimento local executado

scripts/experimentar-tempo.ps1 implementa somente um modelo exploratório de relógios. Não altera Battle, Stage ou os resultados COMBATE.json anteriores.

Proposta numérica explícita: barra inteira com teto 1.000.000, SPD de 1 a 10.000, avanço por ticks até alguém estar pronto; maior AP, maior SPD e ID ordinal desempatam; a barra do selecionado é zerada, descartando excedente. AP manual fica entre zero e o teto. Esses limites, arredondamento e regras de excedente não foram obtidos do jogo e precisarão de decisão/testes antes de produção.

**11/11 verificações aprovadas:** frequência por SPD; replay/desempate independentemente da ordem de entrada; aumento de AP; redução e limites; mudança dinâmica de SPD; extra com relógios separados; reação sem turno; decremento apenas do dono; exclusão de derrotados; SPD inválido; saturação de durações/recargas em zero.

No caso sintético SPD 200 contra 100, 30 oportunidades resultaram em 20 para A e 10 para B. Um extra configurado sem reduzir recarga aumentou a contagem de ações/turnos do dono, preservando o contador natural e a barra passiva dos demais. Uma reação configurada sem ticks não alterou recarga/duração nem turnos do dono. São resultados do modelo proposto, não observações do Dislyte.

O experimento não possui dano, habilidades completas, RNG, efeito funcional ou regra de vitória. Os ticks de recarga/duração são contadores de teste; não substituem um sistema de efeitos. Não comprova equivalência com o cliente, segurança numérica irrestrita, ATB ou integração com fases. Arquivos: tempo-experimento.json e tempo-experimento.csv (30 linhas).

## Bloco seguinte e critérios

TIME-02 precede SEQ-01. Implementar em C++ o contrato de tempo, escolher políticas explícitas e integrar primeiro uma batalha de referência por AP, mantendo o perfil antigo e guarda opcional. Testar composição com limites/fases antes de liberar o novo perfil para Stage.

Aceites: rápidos agem com maior frequência; AP/SPD alteram ordem; empate determinístico; extra não consome barra indevida; reação não decrementa relógio do assistente; bloqueio distingue perder oportunidade de executar ação; efeito recém-aplicado não expira por acidente; recarga de habilidade usada tem fronteira definida; morte/remove/invocações não corrompem fila; guarda funciona habilitada e é rejeitada desabilitada; cadeias encerram com orçamento técnico; replay e budgets locais/globais não se confundem. Não implementar todo o catálogo de personagens para validar esses contratos.

Questões específicas restantes: PEN-07/PEN-09/PEN-12 e atualização ao final de ../../PENDENCIAS.md.
