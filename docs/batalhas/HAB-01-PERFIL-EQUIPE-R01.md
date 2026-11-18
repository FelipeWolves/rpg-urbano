# HAB-01 — Contrato do perfil padrão de equipes R01

18/11/2026. Regras de projeto recebidas do usuário nesta entrega. **Contrato definido; perfil/posições/silêncio ainda não implementados.** A mudança funcional entregue agora é somente a terminologia “defesa” no protótipo existente.

## Escopo e identidade

Perfil padrão de equipes inspirado na referência Dislyte descrita pelo usuário. “Slight/Slide” na fala é tratado como referência de conversa, não nome definitivo do projeto. Não alegar que este contrato confirma todas as regras atuais do jogo externo.

Perfil, relógio e controle são independentes: AP/SPD determina oportunidades; perfil define as ações; jogador/automação selecionam usando as mesmas regras. Até três posições no perfil inicial, sem impor esse número ao kernel genérico.

## Regras fechadas do perfil padrão

- Posição 1 é sempre a habilidade/ataque básico selecionável. Não pode ser silenciada. Se a unidade pode agir e existe alvo legal numa batalha em andamento, a básica deve continuar disponível.
- Como decorrência de disponibilidade permanente nesse perfil, não configurar recarga/custo/condição voluntária que torne a básica inacessível. Impedir a ação inteira é uma condição diferente de silenciar habilidades. Não forçar automaticamente a básica enquanto houver outra ação válida escolhida pelo jogador.
- A mesma definição de básica é usada pelo comando, posição 1 e automação: sem caminho alternativo para burlar regras. Se nenhuma habilidade especial for permitida, básica é a alternativa; se a unidade não puder agir, usar a resolução de bloqueio do modo.
- Até duas posições adicionais, que podem ser ativas, passivas ou conter componentes híbridos. Passiva pura é exibida como informação, nunca selecionada pelo jogador. Posição 2 passiva não produz botão utilizável.
- Passivas disparam somente por seus critérios e janelas declarados. O modelo deverá permitir supressão de passivas por efeitos explicitamente definidos, sem presumir que toda passiva é sempre imune ou sempre desabilitada pelo silêncio comum.
- Sem defesa genérica no perfil padrão. Só entram ações do conjunto de habilidades; comandos de ajuda, estado, reinício e saída não são posições. Passar por impedimento de agir não é quarta habilidade voluntária.

Disponibilidade da básica não significa que ela sempre cause dano ou ignore esquiva/imunidade de dano. Resolve-se a tentativa legal conforme regras de dano/alvo do perfil. Morte da unidade, batalha encerrada e ausência de alvo legal não constituem silêncio.

## Estados de disponibilidade e apresentação

| Situação | Apresentação futura | Equivalente textual planejado |
|---|---|---|
| Disponível | Selecionável | DISPONIVEL |
| Recarga | Contador numérico de turnos restantes | RECARGA: N |
| Inativa/silenciada | Círculo com traço, motivo informado | INATIVA: motivo |
| Condição não satisfeita | Cadeado, requisito por descrição/toque prolongado | BLOQUEADA: requer ... |
| Unidade impedida de agir | Indicação própria de controle/bloqueio | SEM ACAO: motivo |
| Passiva | Informação, sem seleção; pode indicar supressão | PASSIVA / PASSIVA INATIVA: motivo |

Recarga não é sinônimo de silêncio ou requisito não satisfeito. Mais de um motivo pode coexistir: preservar contador e motivos, não apagar um estado ao mostrar outro. A UI futura poderá priorizar um indicador, mas deverá permitir consultar todos os motivos relevantes. Contadores usam o relógio do contrato vigente, não uma nova contagem de rodadas inventada pela interface.

Ícones, tooltip, toque prolongado e novos rótulos de indisponibilidade são requisitos futuros; não foram desenhados ou implementados nesta entrega. Hoje o protótipo mostra a recarga numérica já existente, bloqueio genérico e mensagens de rejeição.

## O que precisa de esclarecimento antes de programar controles

| Ponto | Estado |
|---|---|
| Silêncio sobre a básica | Fechado: não afeta posição 1 |
| Supressão de passivas | Suporte necessário, com alvo/efeito de supressão explícito; matriz ainda a definir |
| Congelamento/petrificação e gatilhos passivos | Usuário relatou dúvida; não confirmado, não decidir por suposição |
| Efeito suprimido versus buffs já concedidos | Ainda definir por efeito; desativar gatilho não deve remover estados anteriores implicitamente |
| Pausar ou avançar recarga interna durante supressão | Ainda definir antes da implementação da família |
| Ao impedir ação, qual categoria de gatilho ainda opera | Ainda definir; bloqueio de turno não equivale automaticamente a desligar tudo |

Inspeção do código: EffectKind contém Block e FreezeCooldowns, sem Silence/Freeze de personagem/Petrify específicos. FreezeCooldowns congela decremento de recarga; não é o congelamento do personagem citado na dúvida. Não reaproveitar esse nome para afirmar equivalência.

Essas dúvidas não reabrem R04 nem bloqueiam modelagem de perfis/posições. Antes de implementar silêncio e supressão, fechar os casos necessários; fontes externas serão consultadas pontualmente ou fornecidas pelo usuário, sem nova busca ampla nesta entrega.

## Perfil clássico — reserva para etapa posterior

Manter o perfil existente e registrar quatro categorias sugeridas: **ataque, defesa, itens e habilidade especial**. A habilidade especial foi sugerida como quarta opção do menu clássico; isso não aumenta as três posições do perfil padrão. Relação com outras habilidades do clássico será detalhada no seu contrato próprio.

Itens/estamina são possibilidades a especificar, não funcionalidades existentes. Não igualar estamina de exploração/conta a AP de combate, nem definir custo, reposição ou inventário silenciosamente. O usuário priorizou fechar o padrão de equipes agora; mundo aberto/semiaberto e conteúdo clássico ampliado ficam fora desta entrega.

## Alteração entregue no protótipo atual

Ajuda, comando recomendado, marcador e mensagem de ação passaram a usar **defesa**, inclusive na demo automática. `defesa` executa o mesmo ActionType::Guard, com o mesmo cálculo de mitigação. Nomes internos em inglês e regras não foram renomeados para evitar regressão sem benefício.

`guardar` permanece somente como alias de compatibilidade não exibido; a saída usa defesa. Jogar.cmd continua sendo a entrada recomendada. Não houve troca automática do cenário clássico pelo novo perfil padrão.

Recompiladas somente apresentação, seus sete testes, jogar.exe e simular.exe usando o objeto do kernel já validado. Sete testes de interação aprovados; demo manteve A, 40 ações/14 rodadas. Roteiro real ajuda/defesa/estado/reinício/saída aprovado; alias antigo também funciona. Os 155 testes totais anteriores não foram repetidos integralmente. Evidência incremental: COM-02-DEFESA.json.

## Próxima entrega

HAB-01 parte 1: implementar perfis e posições conforme este contrato, com validação de básica obrigatória e componentes selecionáveis/informativos. Não tratar registro de passiva como gatilho funcional. Silêncio/supressão/ícones/itens seguem com seu próprio aceite de implementação, não implícitos nesta aprovação.
