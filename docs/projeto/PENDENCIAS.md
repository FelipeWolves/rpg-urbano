# Pendências e pontos a esclarecer

## Síntese vigente —19/11/2026

Registro central de perguntas e decisões. **Documento de pesquisa recebido; não estamos mais aguardando seu envio.** A revisão está em [REVISAO-PESQUISA-HABILIDADES-ITENS.md](REVISAO-PESQUISA-HABILIDADES-ITENS.md). Os itens abaixo são a lista vigente para a próxima devolutiva. Recomendações não são regras aprovadas nem implementadas.

✅ Núcleo, R04, HAB-01, CIC-01 e perfis SEQ/PAR preservados no escopo de seus aceites. Ponte Unreal executada, pacote básico e fluxo visual3×3 já observados; interação revisada compilada, mas ainda sem aceite visual. Evidências em [ESTADO.md](ESTADO.md) e no [painel](../batalhas/PAINEL-BLOCOS.md).

### Como preencher e priorizar

- **🔴 P1 — necessário antes do próximo efeito:** ESC-01 a ESC-07. Decidir esses pontos permite implementar o primeiro escudo percentual; não bloqueiam a inspeção visual da interface já compilada.
- **🟠 P2 — necessário antes da família correspondente:** VIS-01 a VIS-03 e REG-01 a REG-03. Pode pesquisar tudo agora, mas cada extensão só entra no código em seu bloco.
- **🟡 P3 — planejamento e auditoria:** SIS-01 a SIS-04. Registrar respostas sem ampliar automaticamente o escopo.
- **✅ Preservado:** contratos já validados. Uma hipótese externa diferente não os invalida. Sem repetição de testes por simples atualização documental.

Para cada ID, responder: **decisão desejada para nosso jogo; exemplo numérico/cenário; fonte e versão (se referente ao Dislyte); exceções; dúvida restante**. Fonte externa pode ficar “não localizada”; preferência própria não precisa ser apresentada como fato do outro jogo. Respostas parciais são aceitas. O usuário pode devolver todos os itens juntos.

## 🔴 P1 — contrato do primeiro escudo percentual

### ESC-01 — base do cálculo (explicação1)

**Pergunta:** usar HP atual do aplicador ou do protegido? Efeitos futuros poderão usar HP máximo, perdido ou atributos diferentes?

**Recomendação ainda não aprovada:** primeiro exemplo usa HP atual do protegido;10% de100=10 e10% de50=5. Manter a base declarada por efeito, sem converter escudos fixos existentes. **Fechamento:** identificar sujeito/base e pelo menos dois exemplos.

**Resposta do usuário:** pendente.

### ESC-02 — instante e recálculo (explicação2)

**Pergunta:** calcular na aplicação ou atualizar com perda/cura/alteração de HP? Recalcular poderia restaurar pontos já consumidos? Qual comportamento se houver atualização do HP máximo?

**Recomendação ainda não aprovada:** converter o percentual em pontos uma única vez na aplicação; dano/cura posterior não recalcula nem recompõe proteção. **Fechamento:** cenário com aplicação, dano e cura, incluindo ordem dos eventos.

**Resposta do usuário:** pendente.

### ESC-03 — arredondamento e limites (explicação3)

**Pergunta:**55×10%=5,5 vira5? Valores abaixo de1 viram0 ou têm mínimo? Há teto por instância/total? Zero pode criar apenas ícone ou deve significar ausência de escudo?

**Recomendação ainda não aprovada:** arredondar para baixo; permitir zero sem mínimo implícito; não criar proteção funcional de valor zero. Limites numéricos devem ser validados, sem truncamento/overflow silencioso. **Fechamento:** exemplos HP0/1/9/10/55, percentual zero e limites admissíveis; não aplicar escudo funcional a morto sem regra específica.

**Resposta do usuário:** pendente.

### ESC-04 — absorção e ordem do dano (explicação4)

**Pergunta:** escudo absorve antes do HP, depois das reduções? Excesso atravessa? Cada hit consome separadamente? Quais ataques ignoram escudo e quais escudos podem impedir esse bypass?

**Recomendação ainda não aprovada:** usar dano após mitigação aplicável;5 de escudo contra8 absorve5 e deixa3 para HP. Reutilizar o executor existente; exceções declaradas por efeito, sem alterar R04. **Fechamento:** exemplo simples, multi-hit, escudo esgotado, dano zero e bypass.

**Resposta do usuário:** pendente.

### ESC-05 — reaplicação e coexistência (explicação5)

**Pergunta:** reaplicar soma, substitui, mantém o maior ou cria instâncias? Duração renova? Fontes diferentes coexistem? Se coexistirem, qual é consumida primeiro? Escudo comum e StructuralShield seguem a mesma regra?

**Recomendação ainda não aprovada:** mesmo efeito conserva o maior entre pontos restantes e nova aplicação, renovando duração. Não habilitar acúmulo entre fontes sem decisão; não unificar categorias só pela aparência. **Fechamento:** aplicações10→3→novo5 e10→8→novo5; fontes diferentes e durações diferentes.

**Resposta do usuário:** pendente.

### ESC-06 — duração e relógio (explicação6)

**Pergunta:** contar turno do aplicador/protegido/global? Início ou fim? NaturalOnly ou EveryTurn? Aplicação no próprio turno desconta imediatamente? Bloqueio, turno extra e morte do aplicador alteram duração?

**Recomendação ainda não aprovada:** oportunidades naturais do protegido; nenhuma perda imediata no turno de aplicação. Especificar a fronteira exata compatível com o núcleo antes de implementar; reação não equivale a turno do dono. **Fechamento:** cronologia de aplicação, oportunidade natural, extra, bloqueio e expiração.

**Resposta do usuário:** pendente.

### ESC-07 — morte, revive e transições (explicação7)

**Pergunta:** remover na morte do protegido? Morte do aplicador encerra? Revive restaura? Onda, confronto e troca de equipe conservam pontos/duração? Expiração dispara algum efeito?

**Recomendação ainda não aprovada:** remover este escudo na morte do portador; revive não o restaura; transições seguem política explícita do encontro. Dependência do aplicador é atributo separado. **Fechamento:** tabela por evento e casos Preserve/Reset, sem mudar políticas SEQ/PAR já aprovadas.

**Resposta do usuário:** pendente.

## 🟠 P2 — apresentação e contratos relacionados

### VIS-01 — faixa de escudo e painel (explicação8)

Já implementado em fonte: pontos reais sobre a parte preenchida da vida, faixa azul-clara limitada à área de HP e valor integral no painel. Confirmar visualmente100HP/10,50HP/5,50HP/10, escudo maior que HP e escudo zero. Diferenciar escudo fixo de percentual no texto; mostrar origem/duração conforme metadados disponíveis. **Prioridade:** aceite da revisão atual; não depende de cadastrar todo o catálogo.

**Resultado/observação do usuário:** pendente.

### VIS-02 — habilidades, alvos e interação

Verificar ícone/painel de efeitos, nomes/descrições completos, nível somente quando cadastrado, passivas consultáveis e não selecionáveis, recarga com contador, bloqueio com motivo. Testar clique curto/prolongado e soltura sem ação, pausa local da automação durante consulta, alvo válido, troca/confirmação e recusa sem consumo. Standard e lateral conservam seus fluxos declarados. Símbolo de alvo deve admitir futura personalização; imagem própria/licenciada ainda não implementada.

**Resultado/observação do usuário:** pendente. Compilação não substitui teste visual; abrir aplicativo exige autorização específica.

### VIS-03 — formações, tela e acessibilidade

Conferir1/3/5 unidades por lado, assimetrias, rótulos ao fundo, ausência de caixas agregadoras e outras resoluções/redimensionamento. Seis posições, mobile/toque e acessibilidade são extensões a especificar/testar separadamente. Retrato2D original e UI específica de chefe/auxiliares permanecem pendentes, sem inferir compartilhamento de HP pelo layout.

**Resultado/observação do usuário:** pendente.

### REG-01 — defesa clássica (explicação9)

✅ Regra vigente: metade do dano por hit, para baixo; consumida no fecho de ação que a utilizou ou expirada na próxima oportunidade do dono. Documento novo sugere duração até próxima oportunidade natural e outras exceções. **Pergunta futura:** manter variante separada? Como interage com extras, dano verdadeiro, perfuração e modificadores? **Recomendação:** preservar atual e nome Defesa; não habilitar no modo Equipes por escolher câmera lateral.

**Decisão sobre variante:** pendente; não bloqueia o escudo nem reabre o aceite da defesa atual.

### REG-02 — causalidade e vitória (explicação10)

✅ R04/RES-01 preservados: reflexão/explosão FIFO inline; interrupção dos hits quando aplicável; auto-revive; Victory Lock antes de reações. Em morte dupla, lado do autor da ação atual, inclusive contra-atacante. A sequência Q07 do documento não substitui esse contrato.

**Pesquisa opcional:** caso haja evidência externa diferente, registrar versão/contexto e exemplo; não mudar automaticamente o projeto. Timeout do modo e papel ofensivo/defensivo devem permanecer separados do autor causal. Sem nova decisão necessária para avançar o escudo.

### REG-03 — silêncio, bloqueio e supressão (explicação11)

**Perguntas:** quais efeitos impedem agir, bloqueiam apenas ativas ou suprimem passivas? Congelamento/petrificação fazem quais operações? Passiva de abertura difere de gatilho em combate? Recargas continuam contando? O que ocorre com reações/extras já enfileirados? Quais exceções por unidade/efeito?

✅ Básica na posição1 não é silenciada por decisão do projeto; impedimento de agir é condição distinta. Block não significa silêncio; FreezeCooldowns não significa personagem congelado. **Fechamento futuro:** matriz efeito×comando/gatilho/clock, incluindo exceções explícitas. Necessário antes de implementar essa família, não antes da inspeção visual.

**Resposta do usuário:** pendente.

## 🟡 P3 — pesquisa ampla e evolução (explicação12)

### SIS-01 — origem, permissões, âncoras e quotas

Distinguir habilidade, equipamento, divinate e regra de modo. Definir quem aplica, quem porta, quais eventos disparam, âncora temporal, persistência, reaplicação e limites por fonte/alvo/time/cadeia/batalha. Definir dissipação, roubo, transferência, imunidade e supressão independentemente da cor do ícone. Silver/Golden têm divergências no documento. Esses sistemas ainda não existem integralmente no projeto.

**Resposta/fontes:** pendentes; prioridade antes dos respectivos sistemas, sem plugins/dependências antecipados.

### SIS-02 — fórmulas, atributos e auditoria de fontes

Verificar contexto/versão de afinidades, ACC/RESIST, Miss/Crit/Boon, mitigação por DEF, dano verdadeiro, limites contra chefes, overkill e teto de HP. Transcrever e conferir as fórmulas embutidas como imagens antes de qualquer uso. Separar atributos base/efetivos, escala/arredondamento e limites seguros. Ocean não comprova sozinho tick normal de recarga; empate em habilidade de Toland não comprova desempate universal. Datas conflitantes de3.4.50 e catálogo incompleto não são baseline confirmado. Auditar apenas o necessário para o próximo contrato, podendo o usuário pesquisar todos de uma vez.

**Resposta/fontes:** pendentes.

### SIS-03 — ondas, times, chefes e persistência

Distinguir início de combate/onda/rodada/oportunidade, efeitos que persistem, retorno cíclico de times, kit fixo ou variável por etapa, condições por HP e score/sobrevivência. Preservar SEQ-01 e PAR-02 (HP único/barra global no perfil escolhido); proposta de instâncias independentes é outro modo. Identificar quais exceções afetam chefe, auxiliares, invocações ou outras frentes. Não impor exclusividade de equipamentos antes de haver sistema de equipamentos. IDs PEN/PVE históricos permanecem rastreáveis abaixo e no painel.

**Resposta/fontes:** pendentes; famílias futuras não invalidam perfis já encerrados.

### SIS-04 — inventário, ATB, retomada e plataformas

Equipamento não equivale a consumível: definir estamina/itens de combate separadamente no clássico, custos, turno consumido e regras de uso. ATB Active/Wait, barra de energia, cast delay e recursos não são mera troca de SPD. Save de progresso existente não restaura combate; snapshots/retomada precisam de contrato. Servidor, threads, carga, desempenho, mobile e distribuição precisam de medição própria, não de inferências sobre remoção de modos do Dislyte.

**Resposta/preferências:** pendentes; não bloqueiam o primeiro escudo nem autorizam instalações, rede ou novos programas.

## Registro histórico e rastreabilidade

As tabelas PEN/VAL e atualizações datadas a seguir registram a evolução. Linhas antigas como “não implementado” não substituem os aceites posteriores. Para estado vigente usar a síntese acima, [ESTADO.md](ESTADO.md) e o [painel](../batalhas/PAINEL-BLOCOS.md). Histórico anterior à consolidação preservado em [PENDENCIAS.md.txt](historico/2026-11-18/PENDENCIAS.md.txt).

## Decisões em aberto

| ID | Ponto a esclarecer | Quando resolver | Critério de encerramento |
|---|---|---|---|
| PEN-01 | O que persiste entre confrontos: vida, derrotados, efeitos, energia, recargas e ordem/AP? | WAVE-01 | Política explícita de passagem e testes de 1/2/5 confrontos; máximo de vida não adulterado e nenhuma cura/reanimação implícita |
| PEN-02 | Como contabilizar um orçamento global da fase junto dos limites de cada confronto/frente? | WAVE-01 e PAR-01 | Um contador global autoritativo; unidades compatíveis, precedência e testes sem reset/triplicação acidental |
| PEN-03 | Quais IDs representam personagem, instância, time, lado, confronto, etapa e fase? Pode repetir personagem entre times? | WAVE-01/SEQ-01 | Escopos e regras de composição definidos; IDs locais repetidos não causam vazamento |
| PEN-04 | Na troca dos três times, chefe mantém HP/efeitos/recargas? Qual condição troca seu conjunto de habilidades? | SEQ-01 | Perfil por etapa, fronteira da transição e política de persistência com testes; não confundir identidade narrativa e estado |
| PEN-05 | Três frentes contra o mesmo chefe usam HP único, partes ou avatares com score agregado? | Antes de PAR-02 | Escolher o modelo do nosso jogo; eventual referência histórica identificada sem presumir sua arquitetura |
| PEN-06 | Como resolver ações com mesmo tempo lógico, golpes letais e limiares de fase concorrentes? | PAR-02 | Ordem total estável, uma conclusão de chefe e efeito global aplicado uma única vez |
| PEN-07 | Manter ordem fixa por rodada ou introduzir barra dinâmica de ação/AP? Como speed, controle e turnos extras interagem? | Antes de scheduler/habilidades complexas | Perfil escolhido e especificado, sem confundir round() com duração por portador ou frequência de ações |
| PEN-08 | Quais efeitos e habilidades afetam time, frente ou todos do mesmo lado? | Antes de habilidades entre frentes | Seletores com escopo explícito e casos de isolamento/compartilhamento |
| PEN-09 | Quais ações derivadas contam para o limite? Como parar loops de reação sem alterar a regra de derrota? | Antes de HAB-01 | Políticas e orçamento técnico de eventos separados; contra-ataques/assistências testados quando existirem |
| PEN-10 | Qual regra de empate/score adotar para cada modo e como contar overkill, morte simultânea e recompensa? | Antes dos modos correspondentes | Resultado e aritmética explícitos; conclusão e crédito idempotentes; score não existe no núcleo atual |
| PEN-11 | Quais limites por perfil para formação, reservas, invocações, número de confrontos e times? | Camada de composição/WAVE-01 | Dados validados com extremos; não impor cinco como constante global do kernel |
| PEN-12 | Um perfil precisa de ações apenas do jogador, turnos do chefe, rodadas completas ou tempo real? | Quando um modo exigir outra unidade | Unidade e escopo explícitos com testes próprios; TIME-01 só implementa ações principais aceitas de ambos os lados |
| PEN-13 | Tipos, escala numérica e limites de acumuladores de dano/score/tempo, com porcentagens e multiplicadores? | Antes de introduzir esses valores | Aritmética verificada e determinismo entre plataformas; 64 bits isoladamente não elimina overflow |
| PEN-14 | Como reiniciar/retomar fase sem reaplicar eventos ou conservar estado indevido? | WAVE-01 e futura persistência | Configuração/IDs versionados; reset e reentrada testados; snapshot atual não é formato de save |

## Capacidades ainda não validadas

| ID | Capacidade | Evidência que já existe | O que ainda falta |
|---|---|---|---|
| VAL-01 | Fases consecutivas e persistência | Batalhas isoladas e orçamento local | Orquestrador de fases, estado de entrada e política PEN-01/PEN-02 |
| VAL-02 | Chefe compartilhado e transições de habilidades | Relato do cenário e contratos documentados | Estado autoritativo/partes/avatares escolhido e implementação; resolver PEN-04/PEN-05/PEN-06 |
| VAL-03 | Três frentes como modo completo | Isolamento de três contextos intercalados em uma thread | Coordenador, resultado global, cancelamento, budgets e apresentação |
| VAL-04 | Paralelismo real entre threads | Nenhum ensaio concorrente | Necessidade medida, particionamento/sincronização e testes; Battle não é thread-safe por contrato |
| VAL-05 | Servidor, multiplayer e capacidade de usuários | Apenas ensaio funcional local sem rede | Backend, protocolo/autoridade/persistência e teste de carga específico; não dimensionar pelo lote local |
| VAL-06 | Desempenho visual e mobile | GPU enumerada e núcleo textual | Engine/hardware-alvo, cenas, UI/VFX e medições de FPS/memória; cálculo local não mede renderização |
| VAL-07 | História técnica e causa de alteração de modos de referência | Pesquisa seletiva, relatos e fontes classificadas | Fonte primária verificada para cada causalidade; não assumir teste de servidor ou limite de engine |

## Definido em TIME-01 — não está pendente

- Orçamento local opcional por batalha, expresso em ações principais aceitas de ambos os lados; ataque e guarda consomem uma unidade.
- Comando inválido, salto de combatente morto, troca de rodada e emissão de eventos não gastam ação adicional.
- Limite configurado deve ser positivo. O resultado por esgotamento é escolhido explicitamente: empate, derrota de A ou derrota de B. Não há resultado por score implementado.
- Ação permitida resolve integralmente: vitória por eliminação na última ação tem precedência sobre esgotamento do limite.
- Após esgotar ou vencer, não há novo turno/rodada. Tentativas seguintes são rejeitadas sem alterar estado.
- Sem orçamento configurado, mantém-se o comportamento sem limite de COM-01. Segurança do executor de testes não é regra de jogo.

Detalhes de implementação e aceites: docs/batalhas/TIME-01.md. Os contratos de outras famílias permanecem em docs/batalhas/CONTRATOS.md; as perguntas ativas devem ser mantidas neste documento, evitando listas divergentes espalhadas pelo projeto.

## Atualização WAVE-01 — escopo resolvido

- PEN-01: resolvida para vida/derrotados, guarda e ordem fixa no perfil atual. PreserveHealth ou RestoreTeam obrigatórios, guarda limpa, ordem recalculada. Energia/efeitos/recargas/AP futuros continuam em aberto quando existirem.
- PEN-02: resolvida para fase consecutiva: contador global único, menor limite efetivo, precedência global em igualdade e vitória final por eliminação prioritária. Orçamento entre frentes segue para PAR-01.
- PEN-03/PEN-11: StageId/EncounterId/configVersion e IDs de combatente locais definidos; 1–5 confrontos neste perfil. Identidades de catálogo, múltiplos times e invocações permanecem futuras.
- PEN-14: reset e conclusão única em memória testados. Retomada por save, migração de versão e recompensa persistente não implementadas.
- VAL-01: validada para 1/2/5 confrontos com ataque/guarda, passagem de vida e orçamento global. Outros estados só serão validados quando implementados.

Evidência: docs/batalhas/WAVE-01.md e docs/relatorios/atuais/COMBATE.json (9/9 testes de fase). Este registro atualiza o estado das linhas acima sem descartar seu histórico de questões.

## Pesquisa de tempo — atualização de prioridades

TIME-02 agora precede SEQ-01. Evidências e experimento (11/11) em docs/batalhas/TEMPO-PESQUISA.md; o núcleo de combate permanece inalterado.

- PEN-07: usuário priorizou barra por velocidade/AP, com arquitetura capaz de outros perfis. Desempate, excedente, escala, alteração durante turno, fila de extras e pausas precisam ser explícitos; fórmula interna do Dislyte não comprovada. ATB não é simples troca de atributo.
- PEN-09: separar reação/assistência/perseguição de turno extra. Definir limites de cadeia, ID de origem e significado de uma vez por turno; não pressupor que qualquer ataque reduz duração/recarga.
- PEN-12: contador geral do modo segue sem regra exata comprovada. Resolver se conta oportunidades naturais/extras, bloqueios e quais lados; não equiparar automaticamente a rodada fixa ou ao orçamento atual de comandos aceitos.
- PEN-01/PEN-14: quando efeitos/recargas/AP entrarem no núcleo, ampliar passagem de estado e replay de fases; 9 testes de WAVE-01 cobrem somente os estados já existentes.
- Fronteiras a especificar no TIME-02: quando começa a recarga de habilidade recém-usada; duração aplicada ao próprio dono; oportunidade bloqueada por controle; extras com ou sem tick; morte durante gatilho; persistência da guarda até oportunidade extra/natural. São decisões de perfil, não uma regra universal inferida das fontes.

## TIME-02 — fundação implementada e lacunas restantes

- PEN-07: perfil ActionBar integrado em C++, com regra própria inteira de AP/SPD, desempate e fila FIFO de extras; FixedRounds preservado. ATB, pausa/menu e equivalência numérica ao Dislyte seguem abertos.
- PEN-09: contra-ataques derivados têm contador separado e origem; não reduzem seu relógio de dono. Proteções para fila/cadeia encerram com TechnicalLimit sem vencedor. Assistências/perseguições, prioridade entre passivas e limites por turno específicos ainda não existem.
- PEN-01/PEN-14: fases estendidas agora exigem RuntimeCarry Preserve/Reset. Status, recargas e velocidade efetiva são conservados ou reiniciados explicitamente; AP/guarda/filas/contadores locais reiniciam. Sem save ou retomada arbitrária. Contrato de habilidades preservadas precisa ser consistente entre ondas.
- PEN-12: existem contadores distintos de oportunidades naturais/extras, comandos principais, derivados e tempo lógico. O contador geral de cada modo permanece indefinido; Skip bloqueado conta para o orçamento atual, reações não. Isso é regra própria, não regra oficial comprovada.
- Durações/recargas mínimas do dono: NaturalOnly ou EveryTurn. Nova aplicação não perde duração no mesmo turno; habilidade usada não reduz a nova recarga nesse encerramento. Block consome oportunidade via Skip e reduz relógios correspondentes. Relógio do aplicador/global, congelamento por petrificação, modificadores externos de recarga e pilhas/dispels são futuras famílias.
- VAL-01 ampliada: fases AP 1/2/5, políticas de runtime, reset, fronteiras e replay testados. VAL-02/03/04/05/06 permanecem conforme suas fronteiras, sem novas alegações.

Evidências: docs/batalhas/TIME-02.md, tempo-testes.csv e docs/relatorios/atuais/COMBATE.json. 16 novos grupos e 25 formações AP; 33 testes e 168 cenários anteriores preservados. Próximo bloco SEQ-01, sem reabrir todas as pendências futuras.

## Roteiro para pesquisa do usuário

Perguntas consolidadas e modelo de devolutiva em docs/batalhas/ROTEIRO-PESQUISA-USUARIO.md (Q01–Q20), após TIME-02. O roteiro referencia os IDs deste registro e distingue comportamento próprio, evidência sobre Dislyte e preferência de projeto. Prioridades P1/P2/P3; perguntas não se tornam capacidades comprovadas por estarem documentadas. Aguardamos respostas parciais sem exigir pesquisa completa.


## Primeira devolutiva do usuário — revisão R01

Entrada preservada em docs/batalhas/pesquisa-usuario-recebida-R01.md; revisão em docs/batalhas/REVISAO-PESQUISA-USUARIO-R01.md. Vinte respostas recebidas, com afirmações sobre referência e preferências de projeto separadas. Não classificar tudo como comprovado: faltam URLs/trechos e escopo de várias fontes. Nenhuma regra do combate alterada.

Prioridades: PEN-07/PEN-09 (fronteira da recarga, flags independentes por fonte de extra, disparo/decremento/remoção de efeito); PEN-06/PEN-10 (conflito entre vitória antes de pós-morte e pipeline com revive/reação antes da vitória); PEN-12 (contagem no início versus conclusão); PEN-04 (estado completo do chefe ao trocar time). Guarda por habilidade inteira é proposta Q08, ainda não implementada. Propostas de ATB, score, retomada e metas de desempenho preservadas para seus blocos; não são novas capacidades validadas.

## Complemento dos quatro contratos — revisão R02

Preservado em docs/batalhas/complemento-usuario-recebido-R02.md; avaliação em REVISAO-PESQUISA-USUARIO-R02.md no mesmo diretório. Propostas esclarecem recarga 3 disponível no turno 4 e flags independentes por fonte de extra. Matriz do chefe distingue AP, recargas, escudos temporários/estruturais e permanentes. Sem fontes exatas novas e sem alteração do código.

PEN-06/PEN-10: ainda definir janela de reflexão/explosão pós-morte e papel de atacante no desempate de ambos os lados sem vivos; cadeia inclui revive antes de vitória, mas posição daqueles danos ficou indefinida. PEN-04: faltam buffs temporários do chefe, eventos pendentes, gatilhos de entrada/etapa e mapeamento de recarga ao mudar kit. PEN-07/PEN-09: congelamento de recarga frente a reduções externas e fronteiras de cada efeito continuam parciais. Consultar revisão R02 antes de propor novo bloco.

## Causalidade recebida — revisão R03

Registro em docs/batalhas/REVISAO-PESQUISA-USUARIO-R03.md e original preservado. As duas dúvidas principais de R02 foram esclarecidas como regras do nosso projeto: reflexão/explosão antes da vitória; em 0 × 0, lado do autor da ação atual (inclusive contra-ataque) tem prioridade, independentemente do lado ofensivo do modo. Auto-revive antecede checagem final; morte do autor por reflexão interrompe acertos restantes. Não reabrir a pergunta genérica sobre atacante.

PEN-06/PEN-09/PEN-10: futuras implementações precisam distinguir autor da ação, dono da oportunidade e emissor da consequência, com proteção de cadeia e nova checagem em ações derivadas. Microfronteira da explosão entre acertos ainda tem duas leituras no texto; resolver por caso de aceite no bloco correspondente. Não há reflexão/revive implementados nem testes novos nesta revisão. Timeout favorável ao defensor é política de modo, não substitui todos os perfis anteriores.

## R04 — Opção B de causalidade fechada pelo usuário

Contrato vigente em docs/batalhas/CONTRATO-CAUSALIDADE-R04.md. Substitui a microfronteira ainda aberta em R03: reflexão antes da drenagem de explosões; pendingDeaths FIFO drenada entre acertos, antes de auto-revive e Victory Lock. Morte do autor interrompe hits seguintes; bounce exclui mortos; 0 × 0 favorece o lado capturado de CurrentActionActor, inclusive em contra-ataque. Papéis ofensivo/defensivo do modo ficam separados.

PEN-06/PEN-09/PEN-10: decisão de ordem e desempate resolvida como contrato do nosso projeto; implementação e cinco invariantes continuam pendentes. Não solicitar novamente A/B nem a definição geral de atacante. Nenhuma regra de combate foi alterada e nenhum teste novo executado nesta atualização.

## Validação de prontidão R04

Revisão estática concluída em docs/batalhas/VALIDACAO-CONTRATOS-R04.md. Decisões prioritárias permitem iniciar TIME-03 sem nova rodada de esclarecimentos. Separar dúvida de design resolvida de implementação/teste pendentes. Sequência atualizada: TIME-03 → RES-01 → SEQ-01. Nenhuma nova execução ou alteração do combate nesta validação.


## TIME-03 parte 1 — recarga implementada

PEN-07/PEN-09: recarga 0/1/3 no encerramento do uso elegível implementada e testada em perfis fixo/AP. Comandos inválidos não consomem; replay e passagem de fase aprovados. Esta parte substitui o comportamento antigo de recarga recém-usada de TIME-02. Origem/flags dos extras e congelamento de recarga permanecem para partes 2/3; R04 para RES-01. Relatório docs/batalhas/TIME-03.md.


## TIME-03 parte 2 — fontes de extras implementadas

PEN-07/PEN-09: contexto e duas flags independentes por fonte explícita, FIFO, teto por fonte na cadeia natural e replay implementados/testados. Fontes legadas preservam relógios anteriores. Metadados reiniciam na troca/reset de confronto; limite técnico continua separado do teto da fonte. 60 testes totais aprovados; relatório TIME-03.md. Parte 3 ainda trata congelamento de recarga separado de Block; R04 permanece em RES-01.


## TIME-03 concluído — controle/congelamento e integração

PEN-07/PEN-09: Block e FreezeCooldowns independentes; congelamento domina permissão de recarga do extra e é verificado antes da expiração. Duração segue relógio próprio. PEN-01/PEN-14: 1/2/5 confrontos, Preserve/Reset, expiração e retomada de decremento aprovados. 67 testes totais, matrizes anteriores preservadas. Não há redução/reset externo de recarga nem passiva OnTurnStart; semântica sobre essas futuras operações continua fora deste aceite. Próximo RES-01 em entregas internas, sem reabrir decisões de tempo já encerradas.


## RES-01 parte 1 implementada

Identidade/parentesco de ações, multi-hit de alvo fixo e guarda pela instância ofensiva validados (76 testes totais). Q08 atendido no conjunto atual, sem assistências ainda. PEN-06/PEN-09/PEN-10 continuam para reflexão/FIFO e prevenção/revive/vitória; não marcar cinco invariantes R04 como integralmente testados. Dados e limites em docs/batalhas/RES-01.md. Próxima parte 2, sem reabrir identidade/guarda sem alteração pertinente.


## RES-01 parte 2 implementada

Reflexão antes das explosões, FIFO entre acertos, seleção dinâmica excluindo mortos, cancelamento de reações/extras de mortos e limite técnico validados (84 testes totais). Morte dupla pelo autor funciona sem revive; cinco invariantes R04 completos ainda aguardam prevenção/auto-revive/Victory Lock na parte 3. Stage NoSurvivors preserva distinção entre vencer confronto e concluir fase sem time vivo. Fórmula fixa/alcance do perfil em RES-01.md; não é paridade de dano ou bounce aleatório.


## RES-01 concluído — prevenção/revive/Victory Lock

PEN-06/PEN-09/PEN-10: cinco invariantes R04 testados no perfil mínimo original, com cargas finitas de prevenção e auto-revive do portador. Vitória/timeout depois das janelas obrigatórias; morte dupla favorece autor da ação apenas quando não restar sobrevivente após revive. Reações/extras cancelados e acertos interrompidos não voltam. PEN-01/PEN-14: cargas remanescentes conservadas e resetadas corretamente em fase. 95 testes totais aprovados; relatório RES-01.md. Não há revive de área aliado, assistência/perseguição ou RNG; novas famílias pertencem a seus blocos. Próximo SEQ-01, sem reabrir decisões R04 encerradas.


## SEQ-01 parte 1 — coordenador implementado

PEN-03/PEN-11: CharacterId, FighterId e SquadId separados; três times de 1–5, unicidade em toda inscrição validada. PEN-02/PEN-04: troca por derrota/limite local, HP persistente e política base de recarga testados, sem reconstruir max_health; kit ainda estável, status todos limpos no perfil explícito. PEN-06/PEN-10: resultado R04/auto-revive antecede transição, conclusão única em memória; sem score/economia. Próxima parte 2 define kit por etapa e categorias temporárias/permanentes/estruturais. 103 testes totais aprovados, detalhes SEQ-01.md.


## SEQ-01 parte 2 implementada

PEN-04: kit por etapa na troca de time, KeepMatching/ResetAll/MapExplicit e categorias Temporary/Permanent/Structural implementados. Escudo estrutural conserva capacidade; quebrado não reaparece; estado inicial só no começo/reset. 112 testes totais aprovados. Parte 3 valida combinações integradas. Ainda sem fases por HP, pilhas de atributo, IA completa ou score. Git do usuário não causou conflito; pushes continuam com ele.


## Atualização — SEQ-01 encerrado (18/11/2026)

A/A/B e A/A/A, cargas permanentes consumidas antes da troca, limite final, conclusão única e replay/reset validados. 17 grupos sequenciais e 115 testes totais. Coordenação entre frentes pertence a PAR-01/PAR-02; score, IA completa e limiares de HP permanecem fora desta entrega. Registros históricos não reabrem SEQ-01.


## PAR-01 parte 1 (18/11/2026)

Coordenação, resultado agregado e replay de três frentes independentes implementados. PEN-02 continua aberta para orçamento entre frentes: parte 2 tratará contador global, precedência e cancelamento. PAR-01 permanece parcial; chefe compartilhado pertence a PAR-02. Integração de falha técnica e habilidades/R04 reservada à parte 3.


## PAR-01 parte 2 — PEN-02 (18/11/2026)

PEN-02 resolvida no perfil cooperativo de frentes independentes: orçamento global único, políticas explícitas, limites locais independentes, precedência e cancelamento validados sem reset/triplicação. Não se estende a chefe compartilhado ou servidor. Parte 3 mantém integração pendente de R04/habilidades/Skip e falhas técnicas. 125 testes totais aprovados.


## PAR-01 encerrado — integração final (18/11/2026)

Integração pendente das partes 1/2 concluída: habilidades/R04, Skip, reações, extra cancelado e falha técnica coincidente com timeout. 16 grupos PAR-01, 131 testes totais aprovados. PEN-02 encerrada no perfil independente documentado. PEN-05 permanece aberta para escolha do modelo compartilhado antes de PAR-02; esta entrega não altera essa decisão nem valida servidor/threads/score.


## PAR-02 — proposta inicial

PEN-05 permanece aberta. Proposta docs/batalhas/PAR-02.md detalha três modelos e duas cadências. Perguntas enviadas ao usuário; recomendação de HP único/barra global não é decisão. Alcance de habilidades, relógios e retirada de frentes devem constar do contrato escolhido antes da implementação. Matriz de 12 casos planejada, não executada.


## PAR-02 parte 1 — escolha recebida e PEN-05 resolvida

Usuário escolheu HP único e barra global, ataques comuns alternados entre frentes vivas. SharedBoss básico implementado em um único Battle, composição/identidade/rotação/vitória/reset validados. 136 testes totais, 5 grupos novos. PEN-05 encerrada no perfil escolhido; PEN-06/08 e integração R04/overkill/estado compartilhado permanecem para partes 2/3. Registros anteriores de escolha pendente são históricos.


## Famílias PvE — registro sem perda de exemplos

PEN-10/12: relato Treva Fluxo acrescenta sobrevivência/janela e dano influindo em ganhos; unidade/fórmula/regras ainda desconhecidas. Exemplo dos Milagres precisa de chefe/modo exato e gatilho de troca/fase. PEN-04/06: fase por entrada de time não cobre fase autônoma por HP/turno; retorno cíclico não foi afirmado nem implementado. Catálogo MODOS-PVE.md e roteiro ROTEIRO-PVE-CHEFES.md. Nenhum desses relatos revoga PEN-05 resolvida ou torna todos os modelos implementados.


## PAR-02 parte 2 — escopos e consequências

PEN-08 parcialmente atendida: alvo direto único por próprio time, chefe ou frente da rotação; buffs entre times e skills comuns globais não implementados. PEN-06 validada em cenário de explosão global única, morte dupla e revive do chefe; fronteiras técnicas/overkill continuam parte 3. 142 testes/11 grupos compartilhados aprovados. Kit por HP/turno e pontuação seguem separados; não alegar todos os modos PvE cobertos.


## PAR-02 parte 3 — perfil encerrado

148 testes/17 grupos compartilhados aprovados; ordem de cadeias, overkill, reações, revive, falha técnica e timeout/replay validados. PEN-06/08 continuam parcialmente abertas para fases por HP/turno, área e eventos entre times, não abrangidos pelo perfil final. PEN-10/12 mantêm score/sobrevivência e contadores externos desconhecidos. Regra técnica distingue falha já detectada de limite de oportunidade futura: não gera erro após timeout impedir agendamento. Próximo COM-02; catálogo futuro preservado.


## COM-02 — interação textual concluída

Entrada de ações/alvos e reinício de Battle disponíveis e validados. Não é save/checkpoint nem menu de todos os modos; PEN-14 mantém persistência futura. 155 testes totais, sete de apresentação; núcleo inalterado. Próximo HAB-01 mantém famílias PvE por score/fases separadas.


## HAB-01 — dúvidas por etapa

HAB-01.md concentra decisões próximas: posições e básica sem bypass; primeiro gatilho de passiva, dono da janela/limite de proc; condições de ultimate e seleção automática. Recursos universais, prioridade A3>A2>A1 e regras atuais de todas as passivas do Dislyte não estão confirmados. Não perguntar tudo de uma vez nem reabrir R04; pedir amostras atuais de poucos personagens quando a etapa depender delas. Perfil clássico preservado; implementação dos perfis ainda pendente.


## HAB-01 — relatório de habilidades R01

Revisão em REVISAO-HABILIDADES-USUARIO-R01.md. Separar posição passiva de recarga interna, decrementar naturalmente de alterar CD externamente, dono do limite de proc e ficha-base de progressão. Algumas fichas declaradas incompletas/divergentes; fonte pontual necessária antes de paridade. Não confundir AutoRevive próprio com revive de aliados ou Counter genérico com cast de habilidade. Próximo parte 1; sem nova pesquisa ampla.


## Contrato de equipes R01 — controles separados

Básica A1 não silenciável e sempre disponível quando unidade apta/alvo legal; decisão fechada. Recarga numérica, inatividade por silêncio e condição/cadeado são estados distintos e podem coexistir. Matriz supressão de passivas x silêncio/congelamento/petrificação não fechada: não tratar hipótese como fato. Definir também tick de CD interno e efeito sobre buffs existentes. FreezeCooldowns atual não congela o personagem. Clássico com itens/estamina/especial fica para contrato próprio; sem inventário ou quarta posição no padrão.


## Defesa clássica — extensões futuras

Arredondamento padrão para baixo decidido e implementado. Modificadores de dano recebido/defendido, perfuração, itens/atributos e ordem/limites futuros do clássico são possibilidades a especificar antes de codificar. Não se aplicam automaticamente ao padrão de equipes sem defesa genérica. DEFESA-PADRAO.md; regressão atual155 aprovada.

## ENG-01 gráfico — 19/11/2026

- ENG-G01: 14 erros genéricos de automação na inicialização, causa/impacto desconhecidos; investigar registros antes de repetir.
- ENG-G02: tentativa de telemetria apesar de NoAnalytics; entrega não confirmada, isolamento não validado. Planejar bloqueio efetivo antes de outra execução.
- Abertura, mapa, menus e encerramento foram observados; aceite gráfico integral permanece parcial. Roteiro e limites em [ENG-01-GRAFICO.md](ENG-01-GRAFICO.md). Futuro uso de programas exige autorização específica.


## ENG-G01/G02 — atualização do diagnóstico e idioma

Três testes internos de mensagens explicam as 14 falhas em pt-BR; passaram em inglês. Priorizar -culture=en nos testes Unreal como contorno, sem declarar correção da causa. Opt-out bSendUsageData=False sem os marcadores de telemetria anteriores; sem auditoria/bloqueio de rede por decisão do usuário. O jogo em português permanece objetivo separado: definir localização de interface/textos, fontes, formatos regionais e validação no pacote, independentemente do idioma do editor. Evidências e limites em [ENG-01-GRAFICO-DIAGNOSTICO.md](ENG-01-GRAFICO-DIAGNOSTICO.md).


## Apresentação padrão — imagem do usuário em19/11/2026

- Formação em leque escalonado, aliados no primeiro plano e inimigos ao fundo; Classic lateral preservado por chamada própria. Inspeção visual pendente.
- Retrato2D próprio/licenciado do ator junto das três habilidades; existe apenas painel de identificação provisório.
- Futura UI PvE: identificar chefe/auxiliares explicitamente, barra e kit do chefe no topo, vidas dos auxiliares independentes. Não derivar HP agregado nem compartilhar vida automaticamente. Layout do chefe não implica nova mecânica de combate. Detalhes em VIS-01-LAYOUTS.md.


## VIS-01 — interação R01

Ícone/painel, escudo em pontos reais, metadados opcionais, consulta prolongada e orientação de alvos implementados e testados nativamente. Aceite visual atualizado, formações e resoluções continuam pendentes. Regra percentual e interações aguardam documento do usuário; sexta posição permanece aberta. Ver VIS-01-INTERACAO-IMPLEMENTACAO.md e VIS-01-CLASSICO-INTERACAO.md.

## Pesquisa de habilidades recebida —19/11/2026

Documento do usuário preservado e comparado aos contratos. [Revisão e matriz](REVISAO-PESQUISA-HABILIDADES-ITENS.md). Q07/Q08 e proposta de frentes não substituem R04, defesa atual ou PAR-02. Fontes externas ainda não auditadas; escudo percentual permanece em especificação (base, instante/recalculo, reaplicação e consumo), não mais aguardando envio do documento. Próximo passo: fechar esse contrato mínimo. Sem código/testes novos nesta recepção; aceite visual atual permanece pendente.
