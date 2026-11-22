# LAB-PVE-06 — três frentes e chefe compartilhado

22/11/2026. Duas variantes locais integradas ao Terminal sobre SharedBoss v2. Sem executar editor/jogo ou controlar tela. Aceites visuais02–06 pendentes; demo ~28% mantida. Este corte não encerra toda a família PAR.

## Acesso e fixture

Terminal → Mais modos > → Frentes locais > (página3): 3frentes — ataques locais ou 3frentes — especial global. Primeiros modos retorna à página1. As páginas anteriores e seus laboratórios continuam disponíveis.

Frentes1/2/3: Felipe, A1 e B1aliado de laboratório, respectivamente, com atributos/kits próprios já usados no sequencial. Chefe próprio CHEFE-PAR:150.000PV, ATK700, SPD10. Dados provisórios para observar a rotação entre personagens de escalas distintas; não altera ECO-1 ou personagens das outras sessões. Limite global200ações/empate.

Há uma única instância de SharedBoss/Battle: HP, AP, oportunidade e recargas do chefe são únicos. As três equipes permanecem ativas no mesmo encontro; ações seguem o scheduler determinístico. Não é paralelismo de CPU/rede/servidor, nem três cópias do chefe.

## Escopos e rotação

Ataque básico CHEFE-PAR-A1: RotatingFront. O coordenador escolhe a próxima frente viva; o adaptador usa choose_basic_action do próprio SharedBoss para obter alvo legal antes de executar o slot. Frentes sem sobreviventes são puladas. A interface não mantém outra rotação.

Variante especial: CHEFE-PAR-A2 em slot2, EnemyTeam/AllFronts e recarga3. Pode atingir todas as frentes conforme regras normais de defesa/acerto. É uma ação global; não cria três ações e não avança o cursor dos ataques locais. A próxima frente local indicada não promete que o próximo ato do chefe será local, pois o especial pode estar disponível.

Habilidades ofensivas dos aliados usam escopo Boss; self/OwnSquad respeitam sua frente. Seleção visual filtra candidatos, mas toda tentativa ainda passa por SharedBoss.apply. Alvo de outra frente ou target_scope injetado é recusado sem consumir revisão/ação. A3 em área de Felipe usa o escopo do coordenador, não replica dano ao chefe por equipe.

## HUD e geometria

HUD mostra HP único, ações globais/do chefe, frente do ator, próximo ataque local e resultado. Detalhes lista F1/F2/F3, membros/atividade e ações aliadas por frente. Esses números são contadores de ações aceitas, não novos limites locais. Reações derivadas não são contabilizadas como ações extras pelo HUD.

Formação própria em três colunas de profundidade, com rótulo F1/F2/F3 nos personagens e um único placeholder de chefe. Geometria pura suporta1–5membros por frente (até15aliados +1chefe); a fixture visível inicial usa1por frente. Caso15foi validado por dados/posições, não por leitura visual. Material/câmera/resoluções amplas ainda pedem avaliação específica. Rótulos não devem invadir o cabeçalho reservado; busca de posição tenta espaço acima/abaixo em faixa limitada.

## Estado e isolamento

SharedVisualSession encaminha ações, leitura e cues; o coordenador decide alvos/escopos/derrota/rotação. Somente chefe derrotado com resultado de eliminação favorável é objetivo concluído. Limite/técnico/derrota não são convertidos em vitória pelo HP observado isoladamente.

Pausa, abandono, resultado, reinício e retorno usam o fluxo existente. Nova tentativa restaura HP, frentes, rotação e contadores. Não concede créditos, score comercial ou save dos outros modos. Cada variante compartilhada possui sua própria sessão, sem mexer nas sequenciais/PvE normais.

## Limites explícitos

- Três chefes independentes em paralelo continuam sendo outro modelo; não foram integrados à UI neste corte. Não confundir com este chefe de HP único.
- O core SharedBoss v2 suporta auxiliares por frente, mas o adaptador visual atual recusa front_enemies até haver disposição/kit próprios para eles. Os auxiliares do Ritual continuam funcionando no seu modo.
- Passivas BattleStart de abertura exigem ponte de runtime explícita neste adaptador, assim como em ondas/sequenciais; a fixture mantém Felipe reativo e A1/B1ativos sem remover passivas existentes.
- Formação15membros, acessibilidade/resoluções, animações finais, servidores e desempenho não estão aceitos por testes puros.

## Testes

SharedVisual14: configuração/HP único, pureza, contadores, rotação, especial global, frente eliminada, filtro OwnSquad/recusa, escopo injetado/revisão, vitória/limite, abandono/reset, batalha completa e contagens, layout inválido, geometria15+1 e isolamento. Regressões Fronts/SeqVisual/Scored/layout registradas em LAB-PVE-06.json. Sem mudança das regras do core SharedBoss.

## Roteiro de aceite

1. Navegar às três páginas e abrir ataques locais. Ver três frentes/etiquetas e um único chefe; conferir seleção legal e marcador.
2. Observar próximos/últimos ataques locais e atividade das frentes. O turno de cada aliado continua dependente da SPD; não há um botão para agir fora da vez.
3. Abrir especial global em outra tentativa: conferir alcance global, recarga única e cursor local preservado após o especial.
4. Consultar detalhes, concluir/abandonar, reiniciar e voltar aos outros modos; nenhum ganho ou HP deve se misturar.
5. Reportar resolução/sequência se houver corte, sobreposição ou erro. O aceite de controles antigos não comprova a leitura desta nova formação.

## Depois desta fila

LAB-PVE-01–06 têm recortes implementados, com aceites02–06 ainda abertos. Próxima frente recomendada: LAB-QA-01 (feedback/recusas, matriz de save/tutorial/resoluções) e correções concretas dos testes do usuário. Não reabrir bases por rotina nem ampliar para Android/clássico/servidor automaticamente. Extensões técnicas acima ficam registradas para priorização, sem exigir pesquisa nova do usuário.
Resultado: SharedVisual14/14, Fronts8/8, SeqVisual13/13, Scored13/13 e layout6/6 aprovados. Erro de sintaxe da condição de estado ativo no HUD corrigido antes da entrega; compilação final23,11s aprovada e stage68 verificado. Sem cook/execução. Avisos conhecidos do toolchain/header da engine permanecem, sem instalação de ferramentas.
