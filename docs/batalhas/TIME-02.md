# TIME-02 — Barra, turnos, efeitos e recargas

Atualização posterior: TIME-03 parte 1 alterou a recarga para descontar no encerramento do próprio uso elegível. Consultar TIME-03.md para regra vigente e resultados (53 testes); a descrição de recarga original abaixo é histórica.

Implementação original, 18/11/2026. Código C++20 em include/rpg/battle.hpp, src/battle.cpp e integração com Stage. Pesquisa anterior em TEMPO-PESQUISA.md; não houve nova pesquisa, download, instalação ou acesso a jogos nesta entrega.

## Resultado

Battle agora oferece dois perfis explícitos de tempo. FixedRounds conserva a referência anterior. ActionBar permite frequência variável de turnos por velocidade, alterações de AP e velocidade efetiva. Não é declaração de equivalência exata ao Dislyte. O perfil recomendado para os próximos experimentos é ActionBar; o default da API e a demo antiga permanecem FixedRounds para compatibilidade de regressão.

### Seleção por barra

- Escala inteira: 1.000.000 equivale a barra cheia. Velocidade válida no perfil AP: 1–10.000.
- A simulação avança pelo menor número inteiro de ticks necessário para algum vivo ficar pronto. O resto inteiro é preservado nas barras dos demais, sem depender de FPS ou animação.
- Havendo prontos, não avança tempo. Escolhe maior AP, depois maior velocidade efetiva, depois ID ordinal. São decisões próprias do perfil.
- Ao selecionar um turno natural, zera a barra do escolhido, descartando seu excedente. Alterações diretas de AP ficam entre zero e 1.000.000; SetSpeed altera velocidade efetiva sem mudar o atributo-base.
- A oportunidade ativa termina antes de escolher outra; uma alteração de AP não interrompe o comando em resolução.
- round() e cursor do snapshot continuam sendo informações legadas do perfil fixo. No AP, round() permanece 1 e não emite RoundStarted. living_order lista os vivos na ordem-base de cadastro/velocidade; não prevê a sequência futura por barra.

### Relógios separados

| Dado | Significado |
|---|---|
| logical_time | Ticks já avançados pela seleção natural |
| turn_id | Identidade local da oportunidade ativa, começa em 1 |
| natural_turns | Oportunidades naturais concluídas, não rodadas |
| extra_turns | Oportunidades extras concluídas |
| accepted_actions | Comandos principais aceitos, unidade do orçamento atual |
| derived_actions | Contra-ataques executados dentro dos comandos principais |

Cada Fighter também registra seus totais naturais/extras. Reação mantém o contexto da oportunidade causadora e não cria turno próprio. Eventos do perfil estendido incluem turn_id, turn_kind e marca de ação derivada. source_actor identifica o autor do comando principal que originou a cadeia; a ordem no vetor de eventos determina a ordem local. O wrapper Stage identifica o confronto de origem. Não é protocolo de rede nem save.

Não existe contador geral fiel aos modos do Dislyte. Os dados separados permitem defini-lo posteriormente, sem confundir contador de ações, número de rodadas e tempo lógico.

## Comandos e efeitos mínimos

SkillDefinition pertence à configuração confiável da batalha, com ID único, dono, alvo permitido, recarga, relógio de recarga e efeitos. O comando Skill apenas referencia esse ID; não recebe poderes arbitrários do jogador.

Há ataque básico, Skill, guarda opcional e Skip para resolver uma oportunidade bloqueada. Habilidade desconhecida, dono incorreto, alvo inválido, habilidade em recarga, guarda desabilitada e ação durante bloqueio são rejeitados sem gastar limite ou alterar estado. A política automática básica escolhe Skip quando bloqueada; não escolhe habilidades automaticamente.

Efeitos do perfil mínimo:

- Damage usa o mesmo ataque básico, sem multiplicadores, e exige alvo inimigo.
- ChangeAP e SetSpeed alteram a barra/velocidade efetiva no perfil AP; SetSpeed é alteração persistente até nova alteração/reset, não buff temporário de SPD.
- ExtraTurn concede oportunidade ao alvo vivo. Fila FIFO, antes da próxima seleção natural, sem avançar AP dos demais e sem consumir a barra guardada do beneficiário. Funciona também no perfil fixo.
- Block impede ataque/habilidade/guarda; Skip encerra a oportunidade e aplica seus ticks. Não há atordoamento automático com seleção invisível de comando.
- Counter permite contra-ataque básico após dano enquanto vivo e sem Block; ações derivadas são resolvidas sequencialmente após os efeitos principais do comando, antes de encerrar seu turno. Não há assistência, perseguição, interrupção entre acertos ou prioridades de passiva completas.
- Marker fornece estado temporário observável para verificar duração; não modifica atributos.

Block, Counter e Marker têm uma instância por tipo por combatente. Reaplicação substitui duração e relógio; não soma pilhas. Há no máximo esses três tipos de status neste perfil. Múltiplos eventos de dano podem enfileirar múltiplas reações; não existe ainda uma política geral de uma vez por turno por habilidade.

### Durações e recargas

O relógio é do **portador/dono**: NaturalOnly ou EveryTurn (naturais e extras). Não implementamos ainda relógio do aplicador, contador global, tempo real ou regra personalizada de cada personagem.

No fim da oportunidade, reduz somente os estados do dono pertinentes ao relógio escolhido. Status aplicado/reaplicado durante a oportunidade atual não perde duração imediatamente. Reações não reduzem seus próprios status/recargas; a oportunidade do autor principal ainda termina normalmente.

Ao usar habilidade, a recarga assume o valor configurado e não diminui nesse mesmo encerramento. Depois diminui no fim das próximas oportunidades pertinentes do dono, até zero. Uma recarga 2 por NaturalOnly exige duas oportunidades naturais futuras concluídas antes de reutilização. Se um extra ocorre depois de zerar, pode reutilizar; se EveryTurn, o extra também reduz uma recarga anterior. São regras explícitas nossas, sujeitas a perfis posteriores.

Block não congela recarga neste perfil: resolver sua oportunidade com Skip aplica os ticks correspondentes. Não implementamos ainda a exceção de petrificação ou modificadores de recarga por outra habilidade.

### Guarda

guard_enabled habilita/desabilita o comando. Habilitada, reduz o próximo dano recebido pela metade, arredondando para cima, ou expira ao começar a próxima oportunidade do dono, natural ou extra. Contra-ataques também podem consumir a guarda. Não transforma guarda num comando documentado do Dislyte.

## Limites e conclusão

Ataque, Skill, guarda e Skip aceitos gastam uma unidade do orçamento primário. Uma oportunidade extra exige outro comando, gastando outra unidade. Contra-ataques derivados não gastam essa unidade; têm contador próprio. Oportunidade bloqueada resolvida por Skip conta no perfil atual, sem alegar que todos os modos de referência fazem o mesmo.

Resolve os efeitos/reação permitidos do comando antes do timeout. Eliminação produz resultado imediato e interrompe ações pendentes. A última reação permitida pode decidir a vitória na última ação do orçamento. BattleFinished é o último evento da resposta terminal; não há novo turno após a conclusão.

max_derived_actions, positivo até 1024 (default 64), limita reações por comando, extras consecutivos e tamanho da fila extra. Excedê-lo encerra com TechnicalLimit e sem vencedor. Não é derrota nem empate normal. Stage mantém a distinção. O limite técnico não substitui a regra de orçamento e não impõe um teto comercial à quantidade de turnos extras.

## Integração com confrontos consecutivos

Fases com AP ou habilidades exigem RuntimeCarry::Preserve ou Reset, além da política de vida existente. Ausência de política é erro de configuração.

- Preserve conserva status, duração, recargas e velocidade efetiva do time A. Habilidades do time e perfil de tempo precisam ser consistentes entre confrontos; configuração divergente é rejeitada antecipadamente.
- Reset restaura esses estados explicitamente. Política de vida é independente: restaurar vida não implica automaticamente zerar recarga.
- Ambos reiniciam AP, guarda, fila de extras, IDs/contadores locais e seleção de ordem ao entrar no próximo confronto. A seleção inicial natural pode avançar a barra até o primeiro pronto; portanto o snapshot inicial já pode ter AP residual e logical_time positivo.
- Marcadores de aplicação de status conservados são normalizados para a nova batalha: não são tratados como efeito recém-aplicado nem colidem com turn_id reiniciado.
- Inimigos são novos; limite global da fase permanece autoritativo. reset() da fase volta integralmente ao estado inicial.

InitialRuntime valida identidade, velocidade, tipos de status, durações e recargas conhecidas do dono. Não carrega vida/AP/guarda; vida tem entrada própria, AP e guarda seguem a política de fronteira acima. Não é carregamento de save nem API de retomada arbitrária.

## Verificação efetiva

Compilação MSVC /W4 /WX sem avisos na versão final. A bateria executou 13 testes básicos, 11 de limite local, 9 de fase e **16 grupos de tempo**, todos aprovados (49 no total). Mantidos 168 cenários/replays anteriores; acrescentadas 25 formações AP (1–5 contra 1–5), cada uma com replay por entrada invertida. Demo antiga permanece 40 ações/14 rodadas.

Grupos novos: validação; frequência/desempate/replay; AP/SPD; extras e durações; bloqueio/Skip; guarda; relógio de reação; reação letal no limite; proteções técnicas; atomicidade de habilidades; passagem de runtime; fases AP 1/2/5 e orçamento; matriz AP; extras no perfil fixo; morte com extra pendente e reação bloqueada; fronteiras de orçamento e erro técnico de fase.

Na consolidação, foi corrigida a passagem do caminho CSV ao executor de testes. Todos os executáveis da bateria já haviam passado; reexecutamos somente os testes de tempo para gerar o CSV e a etapa de consolidação, aproveitando o log aprovado. Uma compilação intermediária detectou retorno de teste não verificado; foi corrigido antes da execução final aprovada.

Resultados: ../../COMBATE.json e tempo-testes.csv. O JSON/CSV de tempo-experimento continuam preservados como experimento anterior, sem serem confundidos com esta integração C++.

## Limites e próximo bloco

Sem ATB em tempo real, RNG, probabilidades, catálogo de habilidades, buff temporário de SPD, dispel, invocação/revive, assistência/perseguição, relógio do aplicador, contador global configurável de modos, UI ou servidor. A seleção é extensível por perfil de regras; um perfil ATB exigirá avanço explícito, pausas e fila de prontidão, não troca de nome de atributo.

Fundação suficiente para retomar SEQ-01 com o conjunto mínimo já implementado. Qualquer nova família de habilidade terá aceite próprio; as pendências restantes estão em ../../PENDENCIAS.md, sem transformar lacunas da referência em regras inventadas.

