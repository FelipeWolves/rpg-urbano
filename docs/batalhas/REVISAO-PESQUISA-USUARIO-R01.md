# Revisão da primeira devolutiva de pesquisa

Data: 18/11/2026. Entrada: roteiro_de_pesquisa_preenchido.md, fornecido pelo usuário. Cópia preservada em pesquisa-usuario-recebida-R01.md. Esta revisão não altera o motor nem repete testes.

## Como interpretar esta devolutiva

O documento entregue reúne explicações úteis e propostas detalhadas para todos os 20 tópicos. Há três camadas distintas: afirmações sobre Dislyte, preferências sugeridas para nosso jogo e recomendações técnicas. Nenhuma se torna regra implementada apenas por constar do anexo.

Campos intitulados “descrição oficial”, “auditado” ou “validado” são classificações do documento recebido, não validação independente. Wiki comunitária, Reddit e guias não são publicações oficiais da desenvolvedora. Muitas referências são títulos/nome de site, sem URL, versão precisa ou minuto de vídeo. As afirmações amplas de 2022–2026 precisam ser limitadas por modo/versão e verificadas por exemplo.

Foi feita busca seletiva no domínio oficial por Astralis, petrificação/recarga e Chimera/marcos de turnos; não retornou resultados úteis nesta consulta. Isso não demonstra que as regras sejam falsas. Significa somente que esta revisão não obteve nova confirmação primária desses pontos. A pesquisa anterior TEMPO-PESQUISA.md permanece como evidência já registrada, com seus próprios limites.

## Matriz de aproveitamento

| Questão | Contribuição recebida | Relação com o estado atual | Tratamento |
|---|---|---|---|
| Q01 | Contar naturais/extras/bloqueados; excluir reações | Contagem de comandos concluídos aproxima os totais, mas não a fronteira de início nem futuros turnos sem comando | Proposta de contrato; confirmar por modo e separar iniciado/concluído |
| Q02 | Overflow manual de AP e desempate por lado/slot | Hoje boost é limitado à barra e desempate final é ID | Mudança de design proposta; slots e papel atacante ainda precisam de representação |
| Q03 | Extra por fonte, com ticks e trava próprios | Hoje relógio pertence à habilidade/status; extra não carrega essas flags | Alteração necessária se adotada; reduzir duração e recarga devem ser flags independentes |
| Q04 | Descontar recarga no uso; petrificação congela recarga | Diverge da regra atual; Block não congela | Requer cronologia sem ambiguidade e efeitos de controle distintos |
| Q05 | Efeitos no início/fim, empilhamento e exceções | Temos apenas status mínimos do dono, sem DoT/HoT | Base para futuro contrato; falta separar disparo, decremento e remoção |
| Q06 | Limites por turno ativo, alvo e conjunto compartilhado | Existe só limite técnico de cadeia | Proposta de chaves de ativação; exemplos precisam de rastreabilidade |
| Q07 | Pipeline por acerto, assistência, revive e contra-ataque | Nosso conjunto é menor e encerra eliminação imediatamente | Nova família de resolução; conflito com Q15 antes de implementar |
| Q08 | Guarda cobre habilidade inteira | Hoje cobre um dano recebido | Preferência proposta; precisa identificar a instância da habilidade |
| Q09 | Última cadeia completa; não iniciar extras além do teto | Compatível em grande parte com o atual; score/PvP ainda ausentes | Manter invariantes; acrescentar políticas dos modos quando existirem |
| Q10 | Preservar estado e resetar iniciativa entre ondas | Vida/status/recargas/velocidade já têm políticas; escudos e pilhas não existem | Compatível como direção; não declarar escudos/pilhas implementados |
| Q11 | Três times disjuntos, troca por wipe/limite e chefe persistente | Ainda não implementado | Bom ponto de partida para SEQ-01; chefe/fase exigem políticas adicionais |
| Q12 | Frentes locais, habilidades globais e coordenador | Só isolamento intercalado independente foi testado | Requisito futuro; HP único/barras vinculadas não são variantes triviais |
| Q13 | Jogadores locais; chefes podem afetar todas as frentes | Não há seletor entre frentes | Proposta de alcance do modo; “nunca” na referência não comprovado |
| Q14 | Perfil 1–5, unicidade e invocações | Núcleo admite formações maiores; composição ainda não tem catálogo/CharacterId | Implementar limites por perfil; morte do invocador como regra explícita |
| Q15 | Separar danos/score e políticas de recompensa | Ainda não há score/economia | Proposta futura; resolver prioridade de morte e desempate por tempo real |
| Q16 | Pausa, abortar e retomar por replay | Só reset integral em memória existe | Direção útil; seed/comandos não são snapshot e não provam arquitetura externa |
| Q17 | AP prioritário; ATB opcional com pausas e cast | Temos fixo/AP, sem relógio real, MP ou cast | Manter como escopo futuro; nome de interface não é interface implementada |
| Q18 | Tipos largos, tetos, floor e overheal | HP/dano básicos ainda int; multiplicadores/score ausentes | Revisar escolha numérica antes de adotar; precisão e overflow precisam de contrato |
| Q19 | Separar mudança histórica de causa técnica | Compatível com o cuidado já registrado | Retirar inferência de motivação por monetização como fato; faltam fontes exatas |
| Q20 | Metas de FPS/CPU e carga própria | Ainda sem render/backend | Metas propostas; definir hardware, carga e percentis antes de medir |

## Pontos a resolver primeiro

### R01 — Fronteira de recarga (Q04)

O exemplo diz que recarga 3 fica 2 no fim do uso e chega a 0 no terceiro turno, liberando uso nele. Isso só fica inequívoco se dissermos quando a redução de 1 para 0 acontece em relação à escolha da habilidade. Se acontecer ao fim do terceiro turno, a escolha nesse turno já ocorreu.

Precisamos de uma sequência com cinco marcos: início do turno, gatilhos de início, escolha, resolução e fim. Para cada marco, registrar o número mostrado e se a habilidade pode ser usada. Incluir um caso sem equipamento/passiva e depois exceções. Não trocar o algoritmo com base em um exemplo cuja fronteira ainda é ambígua.

### R02 — Extra por fonte (Q03/Q05)

A resposta descreve Astralis sem redução de recarga, mas a preferência agrupa recarga e duração numa flag única false. Isso não prova que a duração também fique congelada. Precisamos de escolhas independentes: reduz recarga? reduz duração? consome AP? conta no limite? pode gerar outro extra da mesma fonte?

Também definir a chave de “uma vez por turno natural”: fonte, beneficiário e ID da oportunidade natural originadora, inclusive quando um aliado concede o extra.

### R03 — Disparo e duração do efeito (Q05)

O texto propõe DoT/HoT em OnTurnStart e ao final diz “sempre após o portador encerrar sua oportunidade”. Esses são momentos diferentes. Separar: quando o efeito produz dano/cura, quando perde duração e quando é removido. Um controle de duração 1 não pode desaparecer antes de impedir a oportunidade que deveria bloquear.

Precisamos de um exemplo de buff aplicado durante o próprio turno e um aplicado durante o turno alheio. O comportamento pode ser definido como regra nossa mesmo se a referência continuar sem confirmação.

### R04 — Vitória, revive e efeitos pós-morte (Q07/Q15)

Q07 coloca revives e contra-ataques antes da vitória; Q15 propõe conceder vitória ao abater o último inimigo antes de efeitos pós-morte. Com prevenção/revive, as duas regras podem produzir vencedores diferentes.

Separar prevenção de morte (a morte ainda não se confirmou), morte confirmada e reação à morte. Precisamos decidir quais janelas precisam acabar antes do resultado final. O TIME-02 atual interrompe na eliminação; não tem prevenção/revive. Não devemos estender essa regra sem teste de ambos os lados potencialmente derrotados.

### R05 — Guarda de habilidade inteira (Q08)

A proposta é clara e difere do comportamento atual. Para adotá-la, precisamos de uma instância da habilidade ofensiva e uma janela de proteção: todos os acertos da mesma instância no portador; consumo ao encerrar essa instância. Assistência/contra-ataque posterior é uma instância distinta, salvo política expressa.

Ainda definir dano periódico, reflexão e dano não causado por habilidade ofensiva. Não é necessário buscar equivalência no Dislyte para essa mecânica autoral.

### R06 — Contador e início de oportunidade (Q01/Q09)

Contar no início e contar ao concluir podem dar o mesmo total em batalhas simples, mas diferem com morte por efeito de início, cancelamento, erro técnico e retomada. Hoje selecionar o ator seguinte pode ocorrer antes de receber outro comando; equiparar diretamente seleção a gasto de orçamento pode consumir uma oportunidade que nunca foi resolvida.

Precisamos manter limite, oportunidades iniciadas/concluídas e ações executadas separados. Exemplo útil: portador morre por DoT no início do último turno permitido. O contador aumenta? A cadeia é resolvida? O resultado é registrado antes da próxima seleção?

### R07 — Fronteira do chefe na troca de time (Q11)

Persistir HP/score/etapa e limpar debuffs é um contrato proposto aproveitável. Ainda faltam AP do chefe, recargas, escudos, buffs, estados permanentes, ações pendentes e duração da etapa. Também falta definir se limite de um time passa ao próximo ou encerra a tentativa.

A pesquisa textual mistura HP finito, progresso de score e chefe de longa duração; vamos escolher um perfil de teste primeiro. Sem HP literalmente infinito: um modo de score pode usar alvo não eliminável com acumulador de dano e condição de encerramento próprias.

## Ajustes técnicos às recomendações

- **C++ e tipos:** usar std::int64_t quando se exigir inteiro assinado de 64 bits. Em MSVC/Windows, long tem 32 bits; escrever “int64 (long)” pode levar a uma implementação incorreta. Isto se refere ao C++ do projeto, não ao alias long de C#.
- **Determinismo:** double seguido de floor não garante sozinho resultados idênticos entre plataformas/compiladores. Preferir contrato de precisão/escala, ordem de operações e testes entre plataformas; avaliar aritmética inteira escalada para porcentagens. Nenhuma troca numérica foi feita nesta revisão.
- **Replay e snapshot:** seed + comandos formam um registro para reconstrução, não um snapshot completo. Também exigem configuração/versão, estado inicial e entradas externas determinantes. Checkpoint completo e log de comandos são alternativas complementares.
- **Tempo real e ranking:** DurationMs precisa dizer se mede simulação, animação ou relógio real. Relógio real em desempate pode favorecer hardware, velocidade de apresentação ou comportamento de pausa; não adotar automaticamente no ranking determinístico.
- **Metas de desempenho:** 60 FPS equivale aproximadamente a 16,67 ms por frame. Menos de 2 ms por turno exige definir equipamento-alvo, tamanho da cadeia, build e percentil. Três frentes visuais não exigem três threads de cálculo.
- **Inferências externas:** fechamento do app não demonstra validação somente ao fim no servidor; pontuação elevada não revela o tipo primitivo interno; ausência de evidência de falha não comprova causa comercial. Esses pontos ficam como hipótese, sem guiar implementação como fatos.

## Próxima devolutiva sugerida

Não refazer as 20 respostas. Priorizar:

1. Q04: cronologia exata de uma recarga 3, sem efeitos adicionais.
2. Q03/Q05: duração e recarga separadas em extra de equipamento versus extra de habilidade.
3. Q07/Q15: sua preferência para a janela de vitória quando há revive/prevenção/efeito pós-morte.
4. Q11: política de AP, recarga, escudo e estado permanente do chefe na troca dos times.

Para cada evidência, bastam URL exata + trecho relevante ou vídeo/minuto, modo e data. As outras respostas ficam preservadas e aproveitáveis como hipóteses ou propostas. Não se exige confirmação de todo o catálogo para prosseguir com um perfil experimental explícito.

## Estado da implementação

Nenhuma mudança de comportamento nem execução de testes nesta revisão documental. Os 49 testes e matrizes citados são resultados anteriores de TIME-02. Preferências escritas no documento recebido são registradas como propostas para ratificação/implementação em bloco próprio, não ordens executadas silenciosamente.
