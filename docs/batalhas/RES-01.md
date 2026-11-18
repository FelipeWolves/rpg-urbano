# RES-01 — Resolução causal incremental

18/11/2026. **Bloco concluído nas três partes, no escopo mínimo original de laboratório.** Prevenção, auto-revive e Victory Lock implementados e validados na parte 3. Seções anteriores preservam o histórico incremental, conforme CONTRATO-CAUSALIDADE-R04.md.

## Parte 1 — Identidade, múltiplos acertos e guarda

### Identidade da ação

Cada comando principal aceito recebe ActionContext com ID local crescente, autor e lado capturados, tipo, alvo e habilidade. parent_id é zero e root_id é o próprio ID. ActionResult expõe esse contexto. Cada contra-ataque executado ganha outro ID, preservando root_id e o ID da ação imediatamente causadora em parent_id. Fonte textual legada da reação permanece disponível; a árvore de IDs acrescenta a proveniência precisa.

Comando rejeitado não cria contexto nem consome ID, turno, recarga ou orçamento. Reação pendente cujo autor/alvo está morto é ignorada antes de receber ID. Um ID identifica ação executada, não cada entrada na fila.

Os eventos de ação incluem o contexto correspondente. Eventos de início de turno/rodada e expiração de guarda por nova oportunidade continuam sendo ciclo de tempo, sem se tornarem outra ação ofensiva. TurnEnded pertence ao comando principal; eventos de dano/reação pertencem às suas próprias instâncias. Snapshot inclui next_action_id para diagnóstico/replay.

IDs reiniciam em cada Battle e no reset: para acompanhar uma fase, usar também o StageId/EncounterId do wrapper, não somente o número. Isso não implementa identidade persistente entre saves/tentativas nem um protocolo de rede.

### Múltiplos acertos

EffectDefinition::hit_count declara repetição do efeito Damage. Cada bloco de dano aceita 1–64 acertos e uma habilidade tem teto técnico de 256 acertos planejados somando seus blocos. Efeitos sem dano mantêm hit_count 1; configuração inválida é rejeitada antes da batalha.

Cada acerto aplica o ataque básico do autor separadamente, sem multiplicar previamente um int pelo número de golpes. Eventos associados ao acerto registram hit_index (começa em 1) e planned_hits (total previsto para aquela habilidade). Dois blocos de dano da mesma habilidade compartilham ActionContext e numeração contínua.

O alvo é fixo nesta parte. Ao morrer, não recebe os golpes seguintes nem dano abaixo de zero; acertos executados podem ser menos que planned_hits. O loop também exige autor vivo antes do próximo golpe, preparando a fronteira para consequências futuras. Não existe bounce/aleatoriedade nem reflexão para validar morte do autor entre acertos ainda.

Todos os acertos do comando principal gastam **uma** unidade do orçamento primário. Recarga/turno também encerram uma vez; os golpes não viram turnos extras. Contra-ataques seguem o contador separado de derivados. O Counter mínimo atual pode enfileirar reação por acerto; não implementa uma regra geral de uma vez por turno de cada habilidade comercial.

### Guarda por instância de habilidade

Uma guarda ativa reduz, com arredondamento para cima, todos os acertos recebidos da mesma instância ofensiva. É consumida uma única vez ao terminar essa instância, antes das reações posteriores. Se uma habilidade tem vários blocos Damage, eles compartilham essa proteção.

Contra-ataques são instâncias separadas, não extensão automática da proteção da habilidade principal. A guarda continua pertencendo ao alvo atingido; executar uma ação não consome, por si só, guarda do autor. A regra anterior de expirar na próxima oportunidade própria, inclusive extra, e a opção guard_enabled permanecem.

O encerramento da ação agora concentra consumo de guarda e a checagem de eliminação disponível. BattleFinished permanece no fim da resposta terminal, inclusive quando houver consumo de guarda. Este encerramento ainda não é a pipeline R04 completa: faltam consequências de morte e revive antes de concluir.

## Verificação

Executado scripts/testar-combate.ps1. Compilação final MSVC /W4 /WX sem avisos. 13 testes básicos + 11 de limite + 9 de fase + 34 de tempo + **9 de resolução** = **76 aprovados**. Preservados 168 cenários/replays e 25 formações/replays AP, isolamento intercalado e lote funcional local. Demo: 40 ações/14 rodadas, A vencedor.

Novos grupos: identidade/rejeição sem consumo; guarda em todos os acertos; vários blocos de dano numa ação; morte e descarte de reações de mortos; identidade derivada e guarda; parentesco de reações encadeadas; configuração/tetos e INT_MAX sem multiplicação indevida; última ação permitida e replay; escopo e reset de IDs em 1/2/5 confrontos.

O teste exato do evento básico passou a verificar também ActionContext e índice/total de acertos; dano e sequência do perfil simples foram preservados. Um estado inicial incompleto de recarga numa fixture nova foi corrigido antes da execução final aprovada.

Evidências: ../../COMBATE.json, COMBATE-RES-01-PARTE-1.json e resolucao-testes.csv. Resultados anteriores TIME-03 continuam preservados. Nenhuma nova pesquisa, instalação ou acesso a arquivos de jogos.

## Próxima entrega interna

Parte 2: reflexão e confirmação de mortes com fila FIFO de consequências entre acertos. Incluir política explícita de alvo dinâmico para testar o caso de alvo secundário eliminado, sem alegar bounce antes de implementar. Verificar interrupção por morte do autor, ordem de mortes/efeitos e orçamento técnico de fila, preparando a separação de morte e vitória.

Parte 3: prevenção/auto-revive e Victory Lock R04, com cinco invariantes completos, morte dupla pelo autor da ação e prioridade perante timeout. Não marcar RES-01 concluído até essas partes. Nenhuma equivalência completa ao Dislyte é inferida desses testes.


## Parte 2 — Reflexão e mortes FIFO entre acertos concluída

Esta seção atualiza as indicações anteriores de ausência de reflexão/explosão. Parte 1 e seus resultados permanecem como histórico; ainda não há prevenção ou auto-revive.

### Perfil mínimo de dano das consequências

Reflect e DeathExplosion são novos status com duração e magnitude positiva, aplicáveis pela configuração de habilidades ou pelo estado inicial validado. A magnitude é **dano inteiro fixo** do perfil de laboratório, não porcentagem, fórmula de personagem ou alegação de equivalência ao Dislyte. Cada dano é limitado ao HP restante, inclusive em INT_MAX, sem somar/multiplicar inteiros antes da aplicação.

Reflect devolve a magnitude ao autor do acerto após o dano direto, inclusive quando o portador de Reflect morreu nesse acerto. DeathExplosion aplica sua magnitude aos inimigos vivos do morto, na ordem ordinal canônica de IDs. Neste perfil, reflexão e explosão não são habilidades ofensivas independentes: não usam/consomem guarda, não refletem entre si e não geram Counter. Uma explosão pode gerar novas mortes com suas próprias explosões; estas entram na mesma FIFO.

Essas escolhas numéricas e de alcance são dados mínimos para verificar causalidade. Não implementar um catálogo por cópia de habilidades externas nem confundir categorias futuras de consequência com esta única fórmula de teste.

### Ordem por acerto

1. Dano direto e confirmação de mortes (transição de HP positivo para zero).
2. Dano refletido e eventual morte do autor.
3. Drenagem FIFO de todas as mortes daquele acerto. Cada morto com explosão dispara uma vez; vítimas novas entram no fim da fila. Mesmo com o autor morto, a fila obrigatória continua sendo drenada.
4. Só então verificar se o autor e um alvo válido continuam vivos para iniciar outro acerto.

A fila é iterativa, sem recursão profunda. Aplicação em alvo já morto não repete a morte. Ao confirmar morte, removem-se reações pendentes da unidade e as dirigidas a ela, além dos extras cujo beneficiário morreu. O processador de reações agora retira a cabeça antes de executar, para que cancelamentos durante o dano não pulem outra entrada por deslocamento de índice.

Eventos distinguem ReflectedDamage, DeathEffectTriggered e DeathExplosion. O emissor aparece em actor; ActionContext continua capturando o autor da ação em resolução e seu lado. Consequências não recebem IDs de ações novas nem gastam orçamento primário/contador de contra-ataques. A ordem do vetor registra a sequência causal.

### Seleção dinâmica de laboratório

HitSelection::FixedTarget mantém o comportamento anterior. LowestHealthEnemy seleciona a cada acerto o inimigo vivo com menor HP, empatando por ID ordinal. É determinístico, não aleatório. A seleção ocorre após a FIFO, inclusive quando o alvo original morreu e outro bloco de dano dinâmico continua. Efeitos não ofensivos continuam vinculados ao alvo original; não ganham redirecionamento implícito.

O teste elimina um inimigo secundário numa explosão em cadeia antes do segundo acerto: os golpes seguintes escolhem apenas o sobrevivente. Isso valida a fronteira de seleção necessária ao caso de bounce de R04, **não implementa bounce aleatório/RNG**.

### Limite técnico e resultado intermediário

max_consequence_events é positivo, default 256 e máximo 4096. Conta unidades de trabalho: dano refletido, processamento de cada morto e dano de explosão a cada vítima. Não corresponde simplesmente ao tamanho do vetor de eventos. last_consequence_events no snapshot registra o consumo do último comando principal, incluindo suas reações; reinicia apenas no próximo comando aceito.

Ao exceder, encerra com TechnicalLimit e sem vencedor, em vez de declarar vitória com consequências não resolvidas. O orçamento primário continua separado; as consequências da última ação permitida resolvem antes do timeout.

Depois das consequências da ação, a checagem atual termina por eliminação de qualquer lado. Se ambos estiverem sem vivos, prevalece o lado capturado do autor daquela ação, inclusive em contra-ataque. Este resultado já é verificável **sem revive**, mas não substitui a futura janela de prevenção/auto-revive da parte 3.

### Fase sem sobreviventes

Uma vitória por morte dupla no confronto intermediário pode deixar o time A sem vivos. Com PreserveHealth, Stage não tenta construir uma batalha inválida: encerra com NoSurvivors, sem vencedor global e sem iniciar próxima onda. A vitória do confronto permanece em Battle; a fase não foi concluída. Com RestoreTeam, a restauração explícita permite continuar. Na última onda, vitória por morte dupla conclui a fase conforme o contrato de vitória já definido.

Essa fronteira foi necessária pela introdução de mortes nos dois lados; não altera o perfil sequencial de três times ainda ausente.

### Verificação efetiva

Executado scripts/testar-combate.ps1, compilação MSVC /W4 /WX sem avisos: 13 básicos + 11 limites + 9 fases + 34 tempo + **17 resolução = 84 aprovados**. Mantidos 168 cenários/replays, 25 formações/replays AP, isolamento intercalado, lote funcional e demo 40 ações/14 rodadas.

Oito grupos novos: reflexão inline/interrupção; morte dupla com autor principal ou contra-atacante; ordem FIFO de quatro explosões e reflexão anterior; seleção dinâmica após morte secundária; explosão que interrompe autor na última ação; orçamento/configuração de consequências; efeitos declarativos/replay e INT_MAX; fase sem sobreviventes com políticas de conservar/restaurar.

Evidência: COMBATE-RES-01-PARTE-2.json; resultado atual ../../COMBATE.json; resolucao-testes.csv com 17 grupos. Evidências da parte 1 e TIME-03 preservadas. Não repetir a bateria após mudanças só documentais.

### Próxima entrega interna

Parte 3: prevenção de morte, auto-revive após os acertos e antes da conclusão, Victory Lock e validação integral dos cinco invariantes R04. Garantir que revive não restaure reações canceladas nem retome acertos já interrompidos. Manter proteção de cadeia e distinção entre autor da ação e emissor da consequência. Somente então encerrar RES-01.


## Parte 3 — Prevenção, auto-revive e Victory Lock; bloco encerrado

Esta seção atualiza as indicações anteriores de funcionalidades pendentes. RES-01 conclui a fundação causal do perfil mínimo; não implementa catálogo completo de habilidades ou todas as passivas de outros jogos.

### Prevenção inline

PreventDeath é status do portador com duração e 1–1024 cargas. Ao receber dano letal, consome uma carga e mantém o portador em 1 HP, sem confirmar morte, criar explosão, cancelar ações ou consumir auto-revive. O evento DeathPrevented identifica essa fronteira. Sem cargas, o status é removido. Nova aplicação explícita pode conceder novas cargas.

O evento de dano registra a perda efetiva de HP; em um portador a 1 HP salvo pela prevenção, o dano efetivo é zero. Não há subtração negativa nem multiplicação intermediária. Reflexão e explosão também consultam prevenção. Se o autor permaneceu vivo graças a ela, seus acertos podem continuar; morte efetiva continua interrompendo-os.

É uma regra mínima de proteção com cargas, não a reprodução integral de Standoff/Ollie/Ikki. Não há protetor aliado, cura percentual, imunidade ou prevenção infinita neste escopo.

### Janela de auto-revive

AutoRevive é status do portador, com duração, cargas finitas e HP inteiro positivo de retorno, limitado ao max_health original. A confirmação de morte consome uma carga e enfileira uma ressurreição para o fim da ação. Carga zero remove o status. Não disparar ressurreição por mera presença de status num cadáver importado: é necessário o evento de morte real daquela ação.

A FIFO de explosões é completamente drenada entre acertos. Somente depois de terminar/interromper os golpes é que a janela de auto-revive é processada, na ordem em que as mortes foram confirmadas. FighterRevived é emitido antes da consulta de vitória. Isso vale também na fronteira de cada contra-ataque derivado.

Ao reviver, HP assume o valor limitado, AP fica zero e guarda fica desativada. Os demais estados/recargas são preservados conforme suas regras; a nova vida não recompõe cargas consumidas. Os acertos interrompidos não recomeçam e as reações/extras cancelados na morte não são restaurados. O retorno não concede, por si só, oportunidade extra. Este perfil cobre auto-revive do próprio portador, sem revive em área de passiva aliada.

### Confirmação da vitória

Depois das consequências e de todos os auto-revives aplicáveis:

- Um lado ainda sem vivos perde para o lado sobrevivente.
- Ambos sem vivos: vence o lado capturado do autor da ação em resolução, incluindo o autor de um contra-ataque.
- Ambos com vivos: não declarar eliminação; continuar a fila permitida de reações ou encerrar a oportunidade normalmente.
- Resultado final impede novas reações. Se o último inimigo foi revivido, não declarar vitória prematura.
- O timeout só é aplicado depois dessa cadeia. Um inimigo revivido pode fazer o resultado da última oportunidade ser timeout em vez de vitória por eliminação.

As políticas genéricas de empate/derrota do orçamento continuam disponíveis; este bloco não substituiu todos os perfis por uma política PvP específica. Assistência/perseguição e desempate inicial por lado/slot ainda são famílias futuras, não novas capacidades por se usar o nome Victory Lock.

### Segurança e persistência

Prevenção e execução de revive consomem o mesmo orçamento técnico de trabalho das consequências. Se ele esgotar, TechnicalLimit não concede vencedor e não finge que todas as ressurreições foram resolvidas. Estado parcial técnico é diagnóstico, não partida concluída nem ponto de retomada de save.

Status agora admitem oito tipos mínimos; o importador valida duração, magnitude e cargas pertinentes. RuntimeCarry::Preserve conserva as cargas restantes entre confrontos. reset() reinicia a configuração inicial. Os testes verificam duas mortes e retornos em ondas sucessivas sem recompor cargas indevidamente.

### Cobertura dos cinco invariantes R04

| Invariante | Evidência no perfil mínimo |
|---|---|
| 1 — Morte interrompe multi-hit | Reflexão/explosão interrompem; auto-revive posterior não retoma golpes; prevenção mantém autor vivo e permite continuar |
| 2 — Remover alvo morto da seleção seguinte | Explosão remove alvo secundário; seleção dinâmica não o usa durante golpes, mesmo com revive pendente para depois |
| 3 — FIFO sem recursão | Cadeia B1 → A1 → A2 → B2 é verificada; todos os disparos terminam antes dos revives B1 → A1 |
| 4 — Vitória após consequências/revive | Revive pode impedir eliminação ou mudar o vencedor; 0 × 0 sem sobreviventes favorece o autor, inclusive contra-atacante |
| 5 — Sem reações após vitória | Resultado final interrompe fila; reações canceladas pela morte não voltam com o revive |

A seleção dinâmica usada é LowestHealthEnemy, determinística. Cobertura da fronteira do invariante 2 não significa bounce aleatório implementado. A pipeline de prevenção/revive foi verificada para os efeitos originais declarados, sem alegação de paridade integral ao Dislyte.

### Verificação final

Executado scripts/testar-combate.ps1 com MSVC /W4 /WX sem avisos: 13 básicos + 11 limites + 9 fases + 34 tempo + **28 resolução = 95 aprovados**. Mantidos 168 cenários/replays, 25 formações/replays AP, isolamento intercalado, lote funcional e demo 40 ações/14 rodadas.

Onze grupos novos: prevenção inline e prioridade perante revive; prevenção em reflexão; revive do alvo antes de vitória; revive do autor sem retomar golpes; FIFO inteira antes dos revives; alvo dinâmico morto aguardando janela; cancelamento permanente de trabalho; resultado/timeout após revive; cargas/limites técnicos/HP extremo; conservação de cargas e reset de fase; replay completo com sobrevivência.

Resultado final em COMBATE-RES-01-FINAL.json e ../../COMBATE.json; resolucao-testes.csv com 28 grupos. Resultados das partes 1/2 preservados. Não repetir a bateria depois desta atualização documental.

### Próximo bloco

SEQ-01: três times sucessivos e chefe por etapa. Começar pelo coordenador e troca de equipes com IDs/configuração e políticas explícitas, usando as famílias mínimas já testadas; só depois expandir perfis/estado do chefe. Não reabrir regras R04 encerradas sem mudança relevante.
