# PAR-02 — Proposta de chefe compartilhado

18/11/2026. **Planejamento iniciado; decisões de modelo e cadência aguardam resposta. Não implementado.** PAR-01 permanece encerrado; nenhum teste anterior reexecutado.

## Objetivo e limites

Três times enfrentando o mesmo chefe, com identidade, autoridade de estado e resultado coerentes. Não atribuir esta proposta à arquitetura do Dislyte: relatórios e relato histórico não estabelecem como o modo original funciona internamente. Sem interface, engine, plugins novos, rede ou threads nesta etapa.

## Escolha 1 — O que significa mesmo chefe

| Opção | Estado autoritativo | Consequências |
|---|---|---|
| HP único — recomendada | Um chefe, HP/maxHP e morte únicos | Dano das três frentes afeta a mesma vida; não existem três mortes independentes |
| Partes vinculadas | HP de partes e regra central de vínculo | Exige definir destruição de partes, transferência de dano e condição de vitória |
| Avatares com placar | Instâncias distintas e agregado de resultado | Precisa de fórmula/condição de placar; não equivale a HP compartilhado em combate |

**PEN-05 ainda aberta.** Recomendação não significa decisão do usuário. Não generalizar os três modelos no primeiro incremento; suportar o escolhido e manter os outros como extensões separadas.

## Escolha 2 — Cadência do chefe

| Opção | Consequências |
|---|---|
| Uma barra global — recomendada | Uma sequência de oportunidades, recargas e relógios do chefe; ataques comuns alternam entre frentes vivas |
| Uma oportunidade por frente | Deve ser definido o vínculo dessas oportunidades com AP, extras, recargas e duração de efeitos; não pode multiplicar procs silenciosamente |

O alcance de uma habilidade é diferente da sua cadência: um turno global pode lançar um ataque em um time ou uma habilidade declarada contra todos. Animações de três representações não geram três ações lógicas automaticamente.

## Contrato técnico proposto para HP único + barra global

Esta seção é **proposta**, dependente das escolhas acima. Não muda R04 nem fecha regras silenciosamente.

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

## Estado da entrega

Documento criado; nenhuma mecânica, plugin ou dependência alterada. Permanecem 131 testes anteriormente aprovados, sem nova execução. Próximo passo depende das respostas sobre modelo e cadência; não iniciar o código compartilhado antes delas. Demais itens acima são propostas para fechar o contrato, não fatos implementados.
