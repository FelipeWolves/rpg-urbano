# Revisão R02 — Complemento dos quatro contratos

Recebido e revisado em 18/11/2026. Entrada preservada em complemento-usuario-recebido-R02.md. Complementa REVISAO-PESQUISA-USUARIO-R01.md; não substitui o histórico nem altera a implementação.

## Resultado da leitura

O complemento resolve a ambiguidade temporal do exemplo de recarga e a separação das flags dos extras. A matriz de troca de times agora distingue recargas, escudos temporários e estados estruturais do chefe. A vitória tem uma proposta mais detalhada, mas ainda precisa de uma fronteira para danos pós-morte e de definição dos papéis.

O documento não contém novos links exatos, vídeos/minutos ou trechos rastreáveis. Portanto, estas definições podem servir como propostas autorais de contrato; não são tratadas nesta revisão como confirmação independente das regras do Dislyte. Expressões do anexo como “definições fechadas” não autorizam mudança silenciosa do código. Nenhum componente do jogo foi consultado e nenhum teste foi repetido nesta revisão.

## 1. Recarga — ambiguidade principal resolvida (R01/Q04)

Sequência proposta sem modificadores para N = 3:

| Oportunidade do dono | Antes da escolha | Encerramento |
|---|---|---|
| 1 | Habilidade disponível; uso coloca recarga 3 | Recarga passa a 2 |
| 2 | Recarga 2; indisponível | Passa a 1 |
| 3 | Recarga 1; indisponível | Passa a 0 |
| 4 | Recarga 0; disponível | Depende da ação escolhida |

Isso define reutilização em T_uso + N. Difere do TIME-02, que não reduz a recarga recém-atribuída no encerramento do uso. A mudança precisará de atualização do contrato e de testes de fronteira.

Também ficam propostas: reduções explícitas no início ocorrem antes da escolha; controle simples perde ação sem congelar recarga; petrificação tem uma propriedade independente que congela recarga.

Ainda definir ao implementar: congelamento impede somente o decremento natural/final ou também reduções explícitas de início, resets e efeitos externos? O exemplo fecha o primeiro caso, não estabelece todos os demais. Incluir N = 0 e N = 1 nos testes, sem permitir valor negativo.

## 2. Extras — flags separadas (R02/Q03/Q05)

A tabela corrige a união anterior de recarga e duração:

| Origem proposta | Reduz recarga | Avança duração |
|---|---|---|
| Natural | Sim, salvo congelamento | Sim |
| Extra de habilidade | Sim, salvo congelamento | Sim |
| Extra de equipamento | Não | Sim |
| Ação auxiliar fora de turno | Não | Não |

Essas são propriedades da oportunidade/origem; nosso TIME-02 ainda configura relógios principalmente por habilidade/status. Para suportar o contrato, a fila de extras precisará carregar metadados da fonte e a regra de avanço.

A linha “natural congela os demais” deve ser lida como pausa durante a execução da oportunidade. A barra dos demais continua avançando na seleção natural seguinte. Não significa paralisar permanentemente o progresso natural.

Permanecem em aberto a prioridade entre fontes simultâneas, limite por fonte/turno originador e o momento exato de decremento/remoção de cada efeito. Avançar duração não significa que todos os efeitos sejam reduzidos na mesma fase: dano periódico, duração e expiração continuam conceitos separados. As lacunas da R01 sobre disparo/remoção não foram fechadas para todo o catálogo.

## 3. Vitória — proposta melhor delimitada, duas lacunas essenciais (R04/Q07/Q15)

O documento propõe prevenção de morte por acerto, conclusão dos acertos, auto-revive, checagem de vitória e reações dos sobreviventes. Isso resolve parte do conflito anterior: não confirmar vitória enquanto há auto-revive relevante pendente.

Faltam dois detalhes para um contrato executável:

1. **Janela de reflexão/explosão pós-morte.** O complemento dá resultado para ambos os lados mortos por esses danos, mas não coloca a execução desses danos nas cinco etapas. Se acontecem depois de Victory Lock, o combate já terminou; se acontecem antes, precisam constar como eventos resolvidos antes da checagem, com prevenção/revive pertinentes.
2. **Significado de atacante.** Pode ser o dono da ação principal, o autor do golpe atual, o jogador que iniciou o desafio ou o lado ofensivo fixo do PvP. Um contra-ataque troca o autor do dano, mas não necessariamente deve trocar a prioridade do modo. Usar papéis distintos em vez de uma palavra com vários sentidos.

Também registrar ao implementar:

- Reações pendentes da unidade morta são canceladas; um revive não deve reativar automaticamente eventos cancelados sem regra explícita.
- A eliminação causada por assistência/contra-ataque requer nova checagem em fronteira definida, sem duplicar conclusão.
- “Não há defensor vivo” torna inaplicável uma reação de defensor sobrevivente, mas não elimina a necessidade de tratar um gatilho de morte já criado.
- O timeout continua após a cadeia permitida e antes de nova oportunidade. Proteção técnica contra loop continua distinta do resultado de jogo.

**Proposta para discussão, não adotada:** explicitar uma janela de consequências obrigatórias de dano/morte (incluindo reflexão e revive previstos), fechar essa janela, então consultar os vivos e aplicar uma política de empate/morte simultânea do modo. Isso permite testar o desempate sem depender de “quem morreu primeiro” inferido pela ordem acidental do código.

## 4. Troca de times — matriz aproveitável para SEQ-01 (R07/Q11)

| Estado | Time novo | Chefe |
|---|---|---|
| AP | Reinicia | Reinicia |
| Recarga | Pronta | Conserva; mudança de kit pode ter política própria |
| Escudo comum | Não herda | Remove |
| Escudo estrutural de fase | Não herda | Conserva |
| Debuff temporário | Não herda | Limpa |
| Estado permanente/etapa | Novo contexto | Conserva |

HP/score persistente e gatilhos de troca por derrota/limite vieram da primeira devolutiva e continuam como propostas relacionadas, não foram redefinidos neste complemento.

Cuidados restantes:

- Definir efeito por categoria explícita; “temporário”, “estrutural” e “permanente” não podem ser inferidos só de duração ou nome.
- Especificar se buffs temporários benéficos comuns do chefe também são limpos. A tabela esclarece debuffs e escudos, mas não todos os buffs.
- Declarar a fronteira para ações/re ações pendentes e se gatilhos de início pertencem ao início da tentativa, à entrada de time ou à mudança de etapa. Não disparar início de combate do chefe três vezes por acidente.
- Mapear recargas ao trocar kit: quais habilidades permanecem, desaparecem ou começam prontas. “Reinicia se coincidir com kit” precisa de uma política de configuração, não de identificação pelo nome.
- Verificar orçamento local por time e global da tentativa separadamente; o esgotamento global não deve iniciar outro time.

## Estado consolidado das quatro prioridades

| Item | Estado após complemento |
|---|---|
| Recarga 3: quando pode reutilizar | Esclarecido como proposta; disponível na oportunidade 4 |
| Extra: recarga e duração separadas | Esclarecido como proposta; fonte com flags independentes |
| Vitória, reações e revive | Parcial; falta janela de dano pós-morte e papel do atacante |
| Estado do chefe na troca | Parcialmente especificado; matriz útil, ainda faltam categorias/fronteiras acima |

Os dois primeiros não precisam de nova explicação genérica. A prioridade para uma próxima resposta é só: **quando reflexão/explosão entra na cadeia e quem recebe a prioridade quando os dois lados ficam sem vivos**. Se houver evidência da referência, fornecer URL/minuto; se for preferência para o nosso jogo, basta identificá-la assim.

## Implementação

Nenhuma regra modificada. Resultados anteriores de TIME-02 permanecem anteriores, não testes dos contratos deste complemento. Antes de SEQ-01, planejar um bloco de ajuste das regras que forem adotadas; não misturar novas recargas, múltiplas famílias de efeito e coordenadores em uma alteração não revisada.
