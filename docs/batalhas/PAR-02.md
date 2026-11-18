# PAR-02 — Proposta de chefe compartilhado

18/11/2026. **HP único e barra global escolhidos pelo usuário. Partes 1/2 implementadas e validadas; PAR-02 ainda aberto.** PAR-01 permanece encerrado. As alternativas abaixo preservam o planejamento; o contrato vigente da parte 1 está ao final.

## Objetivo e limites

Três times enfrentando o mesmo chefe, com identidade, autoridade de estado e resultado coerentes. Não atribuir esta proposta à arquitetura do Dislyte: relatórios e relato histórico não estabelecem como o modo original funciona internamente. Sem interface, engine, plugins novos, rede ou threads nesta etapa.

## Escolha 1 — O que significa mesmo chefe

| Opção | Estado autoritativo | Consequências |
|---|---|---|
| HP único — recomendada | Um chefe, HP/maxHP e morte únicos | Dano das três frentes afeta a mesma vida; não existem três mortes independentes |
| Partes vinculadas | HP de partes e regra central de vínculo | Exige definir destruição de partes, transferência de dano e condição de vitória |
| Avatares com placar | Instâncias distintas e agregado de resultado | Precisa de fórmula/condição de placar; não equivale a HP compartilhado em combate |

**PEN-05 resolvida para o perfil inicial: usuário escolheu HP único compartilhado.** As alternativas não foram adotadas. Não generalizar os três modelos no primeiro incremento; suportar o escolhido e manter os outros como extensões separadas.

## Escolha 2 — Cadência do chefe

| Opção | Consequências |
|---|---|
| Uma barra global — recomendada | Uma sequência de oportunidades, recargas e relógios do chefe; ataques comuns alternam entre frentes vivas |
| Uma oportunidade por frente | Deve ser definido o vínculo dessas oportunidades com AP, extras, recargas e duração de efeitos; não pode multiplicar procs silenciosamente |

O alcance de uma habilidade é diferente da sua cadência: um turno global pode lançar um ataque em um time ou uma habilidade declarada contra todos. Animações de três representações não geram três ações lógicas automaticamente.

## Contrato técnico proposto para HP único + barra global

Modelo e cadência foram escolhidos. Os demais itens desta seção são propostas de evolução; consultar o contrato implementado ao final para distinguir o que já existe. R04 permanece vigente.

- Um estado autoritativo do chefe para HP/maxHP, estados, recargas, cargas de sobrevivência e identidade. Representações por frente são referências, não cópias mutáveis reconciliadas depois.
- Times mantêm identidade e instâncias distintas. Formação inicial proposta: exatamente três times de 1–5; inscrição sem repetição de personagem, como SEQ-01. Conferir se essa regra atende ao modo ao fechar o contrato.
- Ordem de resolução total e determinística. Usar oportunidades AP/SPD e desempates explícitos; a arquitetura deverá preservar velocidade dos integrantes sem conceder três barras ao chefe. Não copiar automaticamente o round-robin independente de PAR-01 como relógio compartilhado.
- Cada comando aceito resolve integralmente sua cadeia R04 antes da próxima oportunidade. Não existem dois golpes simultâneos do ponto de vista do estado autoritativo. A ordem exibida pode parecer simultânea, sem modificar o replay.
- Ataques comuns do chefe escolhem a próxima frente com sobreviventes em ordem cíclica configurada. Frente derrotada é ignorada. Cura e buffs comuns ficam no time de origem; dano das frentes ao chefe usa o mesmo alvo global. Habilidades entre times ou globais exigem seletor explícito futuro.
- Efeito aplicado ao chefe não é multiplicado pelo número de representações. Um evento lógico tem um ID e escopo; exibir em três telas não reaplica dano, status ou cargas. Reaplicações de comandos distintos seguem a regra normal do efeito.
- O turno natural do chefe avança suas recargas e durações uma vez, conforme contrato de relógios. Extras e reações mantêm seus contextos/flags; não se convertem em turnos globais gratuitos.
- Orçamento global conta comandos principais aceitos, não hits nem efeitos. Limites locais só serão incorporados após definir se retiram um time, causam derrota do modo ou têm outro efeito; não herdar automaticamente timeout de PAR-01.
- R04 resolve prevenção, consequências e auto-revive antes de consultar vitória global. Sem HP negativo; morte, transição e conclusão não duplicam. Em morte dupla, aplicar o lado do autor conforme R04, com todos os sobreviventes do lado contabilizados.
- Proposta de vitória: chefe eliminado após R04 encerra para A; ausência de aliados em todos os times encerra para B; derrota de um time isolado não encerra a fase. Falha técnica encerra sem vencedor; timeout usa política explícita do modo.
- Overkill não é dano efetivo nem recompensa duplicada. Sistema de score/recompensas não está incluído no incremento inicial; não inventar fórmula nesta etapa.
- Troca de kit por HP é futura regra configurada, não consequência automática de perder um time. Se necessária, definir momento, limiares múltiplos e transição única antes de implementar.

## Integração sem reabrir entregas concluídas

Battle atual resolve dois lados em contexto próprio; ParallelFronts coordena três contextos independentes. Nenhum deles já oferece autoridade compartilhada. Não criar três chefes em Battle e somar dano posteriormente.

Após fechar modelo/cadência, realizar avaliação pequena da integração: comparar uma batalha autoritativa com identidade de time/seletores versus extração do resolvedor reutilizável. Escolher a menor mudança que preserve R04 e testes existentes; não criar arquitetura genérica de plugin por antecipação. Alterações necessárias no núcleo terão regressão pertinente, sem marcar as entregas anteriores retroativamente como reprovadas.

## Roteiro priorizado após decisões

1. Fechar modelo, cadência e limites do primeiro perfil; registrar decisão do usuário e escopos. Este documento é a entrega de planejamento, não prova funcional.
2. Implementar composição/identidade e estado autoritativo; primeiro fluxo de dano e ação do chefe. Snapshot versionado e reset determinístico.
3. Integrar seletores, relógios, efeitos e vitória R04 ao modelo escolhido; tratar orçamento e interrupção sem duplicação.
4. Validar cenários integrados, consolidar relatórios e encerrar somente o escopo efetivamente aprovado.

As divisões de implementação serão ajustadas após a avaliação do passo 2, sem prometer esforço ou prazo antes de observar o acoplamento real.

## Matriz de aceite planejada — não executada

| Caso | Invariante proposta |
|---|---|
| Três times atingem o chefe | Uma única vida autoritativa diminui pelo dano efetivo |
| Dois comandos seriam letais | Primeiro resolve e encerra; segundo não aplica dano nem emite outra conclusão |
| Escudo/prevenção/revive | Capacidade/cargas globais consumidas uma vez; R04 antes da vitória |
| Morte dupla por reflexão/explosão | Autor e sobreviventes globais determinam resultado segundo R04 |
| SPD/extra do chefe | Uma barra/relógio no perfil recomendado; flags explícitas |
| Efeito global desenhado três vezes | Uma aplicação lógica, sem triplicar cargas ou dano |
| Um time derrotado | Não recebe ataques comuns; demais continuam |
| Todas as frentes sem aliados | Uma derrota global, sem nova ação |
| Última ação de orçamento | Cadeia inteira resolve antes de timeout; regra documentada |
| Comando inválido ou pós-fim | Nenhuma mutação, ID ou orçamento consumido |
| Reset/replay e entradas em ordens equivalentes | Estado inicial íntegro; ordem lógica explícita e reproduzível |
| Limite técnico e cancelamento | Interrupção única, sem vencedor artificial nem reaplicação |

## Estado ao preparar a proposta — histórico

Documento criado; nenhuma mecânica, plugin ou dependência alterada. Permanecem 131 testes anteriormente aprovados, sem nova execução. Próximo passo depende das respostas sobre modelo e cadência; não iniciar o código compartilhado antes delas. Demais itens acima são propostas para fechar o contrato, não fatos implementados.

## Decisão recebida e parte 1 — contrato implementado

18/11/2026. Usuário escolheu **HP único compartilhado e uma barra global**, ataques comuns alternando entre frentes vivas. PEN-05 resolvida para este perfil; partes e avatares/placar ficam fora do primeiro modo.

### Arquitetura efetivamente adotada

`SharedBoss` possui um único `Battle` autoritativo, com os três times do lado A e exatamente um chefe do lado B. Não utiliza três Battle nem reconcilia cópias de HP. Reutiliza o resolvedor e AP existentes sem alterar o kernel. Identidade de fase/versão/time/personagem/instância fica explícita no novo coordenador.

Inscrição exige exatamente três times de 1–5, IDs de time e instância únicos e personagens não repetidos. Nenhuma instância colide com o chefe; A para jogadores, B para chefe. A inscrição inteira é validada, incluindo terceiro time. Perfil ActionBar obrigatório; habilidades são rejeitadas na configuração nesta parte para não aparentar suporte a seletores compartilhados ainda não validados.

Um único scheduler AP/SPD governa todos os integrantes e o chefe. Não há round-robin por frente para oportunidades dos jogadores: velocidade individual continua relevante. O chefe tem uma barra e um turno lógico por oportunidade; não uma cópia de turno por time.

Ataque comum aceito do chefe avança seleção cíclica para o próximo time; mortos são ignorados. Guarda não avança a rotação de ataques. Tentativa contra outra frente é rejeitada sem alterar HP/AP/contadores/cursor. Dentro do time autorizado, alvo vivo pode ser escolhido pelo comando; a escolha automática usa menor HP e desempate por ID. Aliados atacam o único chefe conforme validação de Battle. Guarda opcional permanece disponível. Skip usa regras do kernel; sem habilidades/Block iniciais, não é gerado pelo fluxo básico desta parte.

Snapshot inclui membros, times com sobreviventes, próximo time-alvo e o único snapshot de Battle. Resultado identifica fase e time do ator ou time atacado pelo chefe; término é emitido apenas na ação aceita que encerrou. Battle é exposto somente em leitura.

Time eliminado deixa de ser alvo, mas a partida só termina por eliminação de todo o lado A ou do chefe. HP nunca é transferido entre três chefes; existe apenas um. O orçamento opcional de Battle é global para esta batalha conjunta, contando ambos os lados; vitória na última ação prevalece. Não existem limites locais por time nesta parte. Reset reconstrói a composição e restaura HP, AP e cursor de alvo. Snapshot ainda não é save.

### Verificação efetiva

Cinco grupos novos:

- `composition`: formações 1–5 e único chefe; rejeições de inscrição incompleta, personagens/IDs repetidos, equipe vazia, dados inválidos na terceira equipe e habilidades ainda fora do perfil.
- `one_hp_cadence`: três times reduzem o mesmo HP, sem alterar maxHP; três ataques comuns do chefe atingem times diferentes sem triplicar dano.
- `target_atomics_guard`: alvo de frente incorreta e comandos inválidos sem mutação; guarda não muda rotação.
- `dead_fronts_defeat`: derrota de um time não encerra; rotação ignora o time morto e derrota global ocorre uma vez após perder todos.
- `victory_budget_replay_reset`: vitória por eliminação na última ação global, timeout por empate, replay, imutabilidade após fim e reset sem vazamento entre instâncias.

**136/136 testes aprovados**, incluindo 5/5 grupos de chefe compartilhado, MSVC /W4 /WX. Preservados 168 cenários/replays, 25 formações AP/replays, 1000 simulações funcionais e demo 40 ações/14 rodadas. Evidências: COMBATE-PAR-02-PARTE-1.json, COMBATE.json e chefe-compartilhado-testes.csv. Os 131 anteriores passaram como regressão após adição do coordenador.

### O que ainda falta

Parte 2: concluída no perfil de habilidades de alvo único e consequências globais descrito abaixo. Habilidades amplas de área e redirecionamento dinâmico do chefe permanecem fora do perfil.

Parte 3: integração de mortes/overkill/limites técnicos, término único e replay nas fronteiras; avaliar mudanças de kit por fase conforme contrato definido, sem alegar feature pronta antecipadamente. Limites locais, score, recompensas, partes do chefe, UI, threads e servidor não foram implementados. O bloco inteiro permanece aberto.

## Separação de famílias PvE — atualização documental

MODOS-PVE.md e modos-pve.csv/json preservam os exemplos em 12 entradas, com evidência, participação dos times, HP, término e score separados. Treva Fluxo e chefes dos Milagres são relatos ainda a detalhar, não implementações comprovadas. O perfil HP único/barra global/rotação do PAR-02 é experimental e autoral; não é regra universal dos chefes atuais. “Ignorar times” significa apenas times sem sobreviventes. Contrato básico vigente mantido; próximo parte 2, sem incluir score/recompensas por antecipação.

## Parte 2 — Habilidades com escopo e estado único

18/11/2026. Implementado sem alteração no kernel Battle: SharedBoss habilita skills somente quando todas declaram um único SharedSkillScope válido. Scope também é registrado no snapshot. A restrição total da parte 1 foi substituída por validação de escopo, não removida indiscriminadamente.

| SharedSkillTarget | Dono e alvo direto permitidos |
|---|---|
| Self | Próprio dono; TargetRule::Self |
| OwnSquad | Personagem aliado, um integrante vivo do seu próprio time; TargetRule::Ally |
| Boss | Personagem aliado, chefe único como inimigo; TargetRule::Enemy |
| RotatingFront | Chefe, um integrante vivo da próxima frente da rotação; TargetRule::Enemy |

“OwnSquad” restringe a seleção a um integrante, não aplica efeito em todos os membros automaticamente. IDs de habilidade são únicos segundo Battle. Scope ausente, desconhecido, duplicado ou incompatível é rejeitado antes de iniciar. Atributos/effects/durações continuam validados pelo kernel.

Ataque comum ou skill RotatingFront aceita do chefe avança a rotação uma vez. Self, guarda e Skip não avançam. Tentativas de buff fora do time ou skill inimiga fora da frente são rejeitadas sem consumo de AP, recarga, identidade, orçamento ou cursor. Habilidade desconhecida, dono errado e recarga indisponível continuam sujeitos à rejeição do kernel.

Multi-hit com alvo fixo fica no alvo original, interrompendo conforme R04. LowestHealthEnemy de personagens aliados só pode escolher o chefe, pois existe um único inimigo. Qualquer redirecionamento dinâmico de skill do chefe é rejeitado na configuração: o seletor global do kernel poderia sair da frente permitida. Nenhuma filtragem silenciosa depois de aplicar o dano é usada.

### Alvo direto não é alcance de consequência

Os seletores acima limitam o alvo direto da habilidade. Counter reage ao autor do impacto conforme o kernel; não cria uma nova oportunidade de rotação. Reflect afeta o autor do hit. DeathExplosion segue o alcance de lado existente no kernel: todos os adversários vivos do portador morto. Assim, explosão do chefe pode atingir os três times; é uma consequência global de um único portador, não três cópias da mesma explosão. Este comportamento foi explicitamente testado e documentado; não afirmar que uma habilidade comum RotatingFront passou a ser de área.

A sequência R04 continua mitigação/escudos/prevenção, reflexão e FIFO entre hits, auto-revive, vitória e reações de sobreviventes. Contagem de vivos usa o lado inteiro, não apenas o time atingido. Um status do chefe tem uma magnitude/carga e uma recarga por habilidade; cada turno próprio avança seus relógios uma vez. Turnos dos três times não avançam a recarga/status do chefe.

Skip de chefe bloqueado é selecionado pela política básica e não altera o cursor de ataques. A política automática continua ataque/Skip: skills são invocadas explicitamente nos testes; não há IA completa de habilidades.

### Verificação efetiva

Seis grupos novos, 11 acumulados:

- scope_validation: escopos ausentes/desconhecidos/duplicados/incompatíveis e enum inválido; redirecionamento dinâmico do chefe rejeitado e equivalente fixo aceito.
- squad_rotating_skills: buff único em integrante do mesmo time; tentativa fora do time sem mutação; skills do chefe respeitam três frentes e não duplicam hits.
- boss_clock_skip: uma recarga/status global, recarga impede reutilização, ações dos três times não avançam relógios do chefe, bloqueio gera Skip sem avançar rotação.
- shared_survival: escudo estrutural, prevenção e auto-revive consumidos uma única vez; alvo retorna com 2 HP e a próxima morte não reaplica cargas.
- global_death_consequences: três hits, três reflexões, um gatilho de morte com três vítimas; morte dupla global respeita autor A, sem contra-ataque do morto ou conclusão duplicada.
- scoped_replay_reset: replay com skills e alvo em rotação, orçamento global de 12 comandos, conclusão única e reset de escopos/status/recargas sem vazamento.

A primeira montagem de shared_survival permitia guarda adicional antes do multi-hit, alterando o dano esperado. Ajustado o cenário para transferência direta ao atacante (SPD 301/300), sem mudar a regra de guarda ou o kernel. Não foi tratado como defeito de produção.

**142/142 testes aprovados**, incluindo 11/11 grupos de chefe compartilhado, MSVC /W4 /WX. Preservados 168 cenários/replays, 25 formações AP/replays, 1000 simulações funcionais e demo 40 ações/14 rodadas. Evidências: COMBATE-PAR-02-PARTE-2.json, COMBATE.json e chefe-compartilhado-testes.csv; arquivo da parte 1 preservado.

### Limites e próxima entrega

PAR-02 permanece aberto. Parte 3: integração nas fronteiras de orçamento/falha técnica, reações, overkill, morte do chefe com outros times vivos, extras e término único/replay. Fases de kit por HP/turno exigem contrato separado; não estão prontas nesta entrega. Sem habilidades comuns de área, ataques entre times, seleção dinâmica do chefe, score/recompensas ou modos Treva Fluxo/Milagres completos. Nenhuma engine/plugin, rede ou thread introduzida.
