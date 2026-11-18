# Roteiro de pesquisa — batalha, tempo e modos

Preparado em 18/11/2026, após TIME-02. Documento para você preencher e devolver com os resultados da sua pesquisa. Relaciona as perguntas aos IDs de PENDENCIAS.md, que permanece como registro central do projeto.

## Como registrar as respostas

Você não precisa responder tudo de uma vez. Priorize P1, depois P2. P3 trata de extensões futuras. Para cada resposta, informe o ID Q, modo/evento, personagem/habilidade/equipamento, versão ou data aproximada e a evidência. Caso a regra varie entre modos, registre respostas separadas.

Uma descrição de habilidade confirma o que ela declara, mas pode não explicar ordem de eventos ou exceções. Vídeo com sequência completa e contadores visíveis ajuda a distinguir ação de turno. Relato sem evidência ainda é útil: identifique como lembrança ou hipótese. Uma gravação isolada não comprova regra universal.

Separe dois campos: **“o que encontrei no Dislyte”** e **“o que desejo para o nosso jogo”**. Uma preferência nossa pode ser adotada mesmo sem confirmação da referência. Não precisamos descobrir detalhes internos de implementação para definir comportamento observável.

## O que já está validado no nosso projeto

49 testes aprovados; 168 cenários anteriores e 25 formações AP com replay. Implementamos barra por velocidade, extras, contra-ataques mínimos, bloqueio, durações/recargas do dono, guarda opcional e fases com políticas explícitas.

Esses resultados confirmam nossas regras atuais. **Não confirmam que as mesmas regras, fórmulas e exceções sejam utilizadas no Dislyte.** Não é necessário repetir os testes locais na sua pesquisa.

## P1 — Tempo, efeitos e recargas

### Q01 — O que incrementa o contador geral? (PEN-12)

**No nosso projeto:** contamos comandos principais aceitos, incluindo resolução de oportunidade bloqueada; contra-ataques têm contador separado. Não implementamos o contador geral específico de cada modo.

**Pesquisar:**

- O contador aumenta quando alguém começa ou termina seu turno, quando usa uma habilidade ou em outra ocasião?
- Conta jogador, inimigos, chefe e invocações da mesma forma?
- Turno extra, assistência, perseguição, contra-ataque e golpe adicional contam separadamente?
- Personagem atordoado/congelado/petrificado incrementa o contador ao perder sua oportunidade?
- O contador reinicia entre ondas, times ou etapas? Há modos com regras diferentes?

**Evidência útil:** valor antes/depois de uma sequência curta, com cada ação identificada. Não presumir que o número exibido seja uma rodada completa.

### Q02 — Como AP, SPD e desempate realmente interagem? (PEN-07)

**No nosso projeto:** barra inteira; selecionamos maior AP, depois velocidade efetiva e ID; zeramos a barra do turno natural escolhido. São escolhas próprias.

**Pesquisar:**

- Como se desempata AP/velocidade iguais? Posição na formação, lado atacante ou outro fator interfere?
- AP pode passar de 100%? O excedente interfere na prioridade ou é descartado?
- Aumento/redução é percentual da barra inteira ou do AP atual? A fórmula varia por habilidade?
- Alteração de SPD conserva o progresso da barra? Uma mudança de AP pode interromper a ação em andamento?
- Há exceções de chefe, imunidade ou troca de AP que não se comportem como aumento/redução?

Não é necessário fornecer fórmula exata se as fontes só sustentarem uma descrição qualitativa.

### Q03 — Quais tipos de turno extra existem? (PEN-07/PEN-09)

**No nosso projeto:** extras entram numa fila antes da próxima seleção natural e preservam o AP dos demais. O relógio de cada habilidade/status define se o extra reduz sua duração/recarga.

**Pesquisar:**

- Extra por equipamento e extra por habilidade seguem regras diferentes?
- O extra consome, conserva ou zera a barra do beneficiário?
- Quem vem primeiro quando duas fontes concedem extras? Eles acumulam, substituem ou são descartados?
- Um extra pode gerar outro? O limite é por personagem, turno original, fonte ou batalha?
- Morte, controle ou mudança de fase cancelam o extra pendente?

### Q04 — Quando uma recarga começa e diminui? (PEN-07/PEN-09)

**No nosso projeto:** habilidade usada recebe a recarga configurada; ela não diminui nesse mesmo encerramento. Diminui ao terminar oportunidades futuras do dono conforme NaturalOnly/EveryTurn.

**Pesquisar:**

- O número mostrado na habilidade já inclui o turno em que ela foi usada?
- A redução natural acontece no início ou no fim do turno?
- Quais extras reduzem recarga? Assistências/contra-ataques podem reduzi-la por efeito específico?
- Atordoamento, congelamento e petrificação têm comportamentos distintos?
- Como funcionam aumentar, reduzir e resetar recarga? Há habilidades imunes e valores máximos?

**Exemplo de resposta:** “habilidade marcada com recarga 3 foi usada; os valores exibidos nas próximas oportunidades foram X, Y, Z; ficou reutilizável em ...”.

### Q05 — Qual relógio controla cada efeito? (PEN-01/PEN-07/PEN-09)

**No nosso projeto:** duração do portador por turno natural ou por todo turno; nova aplicação não perde duração no mesmo turno. Ainda não há relógio do aplicador/global.

**Pesquisar:**

- Buff/debuff expira no início ou fim do turno de quem o carrega, de quem o aplicou ou de outro evento?
- Efeito aplicado ao próprio personagem perde duração imediatamente? E aplicação durante turno alheio?
- Extras e ataques fora de turno gastam duração?
- Veneno, sangramento, cura periódica e escudo usam a mesma fronteira?
- Morte do aplicador remove efeitos? Reaplicar renova, soma duração, empilha ou substitui?

Registre exemplos por efeito; não é necessário catalogar todos os personagens.

### Q06 — O que significa “uma vez por turno”? (PEN-09)

**No nosso projeto:** há proteção técnica contra cadeias excessivas. Não implementamos uma política geral por habilidade para essa expressão.

**Pesquisar:**

- O limite reinicia no turno do dono, no turno de qualquer personagem ou ao acabar a ação que disparou a cadeia?
- Um ataque de vários acertos pode disparar o efeito em cada acerto?
- Reações/assistências podem disparar outras reações? Quais exceções interrompem a cadeia?
- O limite pertence a cada fonte, alvo ou à equipe?

**Apontamento:** proteção técnica do nosso simulador não equivale à regra de jogo de um equipamento/personagem.

### Q07 — Em que ordem se resolvem dano, passivas e reações? (PEN-06/PEN-09/PEN-10)

**No nosso projeto:** efeitos principais seguem sua lista; contra-ataques enfileirados são resolvidos depois, antes do encerramento da oportunidade. Não há sistema completo de interrupção entre acertos.

**Pesquisar:**

- Um contra-ataque acontece entre dois acertos de uma habilidade ou somente depois da habilidade inteira?
- Escudo, redução, reflexão, cura, prevenção de morte, revive e perseguição têm qual prioridade observável?
- Se o atacante morre durante a cadeia, os acertos/efeitos restantes continuam?
- Morte do alvo cancela golpes restantes, redireciona ou permite overkill?
- A vitória encerra a cadeia imediatamente ou permite uma reação/morte simultânea?

### Q08 — Como a guarda deve coexistir com essas regras? (Decisão nossa)

Não procurar equivalência obrigatória no Dislyte: guarda é uma opção extra solicitada para nosso projeto.

**Hoje:** reduz pela metade o próximo dano e expira ao começar a próxima oportunidade do dono, inclusive extra.

**Escolher para nosso jogo:** manter essa regra? Proteger de todos os acertos da próxima habilidade ou somente de um? Extra deve fazê-la expirar? Reações consomem guarda? Há modos em que ela fica indisponível?

### Q09 — O que ocorre exatamente na última oportunidade permitida? (PEN-02/PEN-10/PEN-12)

**Hoje:** o comando e suas reações resolvem antes do timeout; eliminação final pode vencer na última ação. Vitória intermediária sem saldo global não inicia outra onda.

**Pesquisar:** a referência concede vitória na última contagem? A cadeia inteira é permitida? Ataque extra pode acontecer após o contador chegar ao limite? Há empate, derrota ou placar? A regra muda por modo?

## P2 — Confrontos, equipes e chefes

### Q10 — O que persiste entre ondas? (PEN-01/PEN-02/PEN-14)

**Hoje:** políticas explícitas preservam/restauram vida e conservam/reiniciam status/recargas/velocidade. AP, guarda e fila de extras recomeçam na fronteira.

**Pesquisar por modo:** vida atual/máxima, mortos, escudos, pilhas, energia, recargas, AP e extras pendentes. Há cura automática? Os inimigos começam agindo ou a velocidade volta a decidir? O limite é acumulado ou renovado por onda? Efeitos do último golpe entram na onda seguinte?

### Q11 — Como funcionam os três times sucessivos? (PEN-03/PEN-04/PEN-10)

**Pesquisar:**

- A troca acontece após morte do time, limite, dano acumulado, condição de chefe ou comando do jogador?
- O chefe conserva vida, escudo, debuffs, pilhas, recarga e AP?
- A mudança de habilidades ocorre antes ou depois de concluir a ação que atingiu a condição?
- Uma ação que cruza o limiar aplica dano excedente à etapa seguinte?
- É possível repetir personagem/equipamento entre times? A ordem pode ser escolhida?
- Falha de um time encerra a fase ou passa ao próximo? Qual regra decide o resultado final?

Prioridade alta antes de SEQ-01. Nome do modo/chefe e descrição de cada etapa são especialmente úteis.

### Q12 — O que era compartilhado nas três frentes? (PEN-05/PEN-06/PEN-08)

Referência relatada: Celestial Anomaly Season 0.

**Pesquisar separadamente:** três batalhas contra três chefes e três batalhas contra o mesmo chefe.

- Havia uma barra única de vida ou barras independentes? Eram partes distintas do chefe?
- O dano de uma frente alterava imediatamente a vida exibida em outra?
- Um debuff, morte ou mudança de etapa afetava todas as frentes?
- Cada frente tinha contador próprio? Existia contador central?
- O chefe atacava cada frente com ações próprias? Uma frente concluída parava, auxiliava ou aguardava?

Vídeos podem confirmar o comportamento apresentado. Não demonstram, sozinhos, uso de threads, arquitetura do servidor ou motivo de retirada do modo.

### Q13 — Quais habilidades atravessam fronteiras de time? (PEN-08)

**Pesquisar:** quando a descrição diz aliado, equipe ou todos, inclui somente a formação ativa ou outros times/frentes? Aura, cura, revive, AP, assistência e debuff global compartilham o mesmo alcance? Alvos derrotados, reservas e invocações contam como integrantes para essas regras?

### Q14 — Quais são os limites de composição? (PEN-03/PEN-11)

**Pesquisar por modo:** mínimo/máximo de personagens, reservas, invocações, substituições, repetição entre times e identidade de cópias. Uma invocação ocupa vaga? Sai ao morrer o invocador? Times menores recebem alguma compensação? Limite mostrado na interface coincide com os alvos possíveis das habilidades?

### Q15 — Como são calculados placar, empate e recompensa? (PEN-10/PEN-13)

**Pesquisar:** dano real versus excedente, dano em escudo/parte do chefe, penalidade por morte, turnos gastos, conclusão parcial e agregação entre times. Golpes após vida zero contam? Se dois lados morrem na mesma cadeia, quem vence? A recompensa depende da primeira vitória, da melhor pontuação ou de cada tentativa?

**Hoje:** não há score, economia nem crédito persistente de recompensa. O teste de conclusão única garante somente a sinalização da fase em memória.

### Q16 — Como pausa, saída e retomada se comportam? (PEN-14)

**Pesquisar:** pausar congela simulação ou apenas apresentação? Sair perde a tentativa, retoma do início ou restaura o ponto anterior? Personagens/recargas são restaurados? Uma tentativa concluída pode reaparecer como incompleta? Distinguir comportamento desejado offline de necessidade de conta/servidor.

Não precisamos investigar credenciais, arquivos internos ou protocolos para decidir nossas regras de retomada.

## P3 — Expansões e limitações técnicas

### Q17 — Quais diferenças de ATB você deseja? (PEN-07)

Decisão de projeto, inspirada em FFVIII/FFIX: a barra continua avançando enquanto o jogador escolhe? Pausa em menus? A ação tem tempo de preparação? Personagens prontos aguardam numa fila? Energia/MP é recurso separado da prontidão? Esse perfil será um modo opcional ou alternativa ao perfil AP?

Não precisamos implementar duas regras de tempo ao mesmo tempo para manter essa possibilidade arquitetural.

### Q18 — Que mecânicas numéricas precisam de cobertura? (PEN-13)

**Pesquisar exemplos:** dano acumulativo muito alto, muitas pilhas, dano percentual, redução de vida máxima, cura excedente e limites de pontuação. Registrar o comportamento/arredondamento observável quando possível. Valor exibido abreviado não revela o tipo numérico usado internamente.

### Q19 — Quais fatos históricos têm evidência? (VAL-07)

Se encontrar explicação para alteração/retirada de um modo, guarde anúncio oficial, data e texto relevante. Separar declaração da equipe de opinião da comunidade. A ausência posterior de uma mecânica não comprova falha de engine, problema de servidor ou inviabilidade de desenvolvimento.

### Q20 — O que fica para medição no nosso projeto? (VAL-04/VAL-05/VAL-06)

Threads, desempenho visual/mobile e capacidade de servidor exigirão medições próprias quando esses componentes existirem. Não é uma tarefa para você descobrir por vídeos do Dislyte. O laboratório atual não valida essas capacidades; números de batalhas locais não estimam usuários simultâneos ou FPS.

Você pode registrar metas desejadas de plataforma, fluidez e quantidade de frentes visíveis; isso é requisito, não comprovação de desempenho.

## Modelo para cada resposta

Copie e repita o bloco abaixo. Pode devolver apenas os itens já pesquisados, em texto ou arquivo.

### Resposta Q__ — título

- Modo/evento e fase:
- Personagem, habilidade, equipamento e evolução/resonância relevantes:
- Versão/data aproximada:
- Fonte/link ou vídeo e minuto:
- Tipo de evidência: descrição oficial / observação direta / guia / relato / hipótese.
- Situação inicial e contadores visíveis:
- Sequência observada:
- Resultado e valores finais:
- Conclusão sustentada pela evidência:
- Exceções, divergências ou o que ainda não foi possível confirmar:
- Comportamento que desejo para o nosso jogo:

## Checklist para a primeira devolutiva

- [ ] Q01: contador geral, incluindo extras, reações e controle.
- [ ] Q04: fronteira da recarga com um exemplo completo.
- [ ] Q05: duração de efeitos com um exemplo aplicado ao próprio dono e outro a um alvo.
- [ ] Q06/Q07: significado de uma vez por turno e ordem de uma cadeia.
- [ ] Q10/Q11: passagem entre ondas e entre os três times.

Com essas respostas já podemos revisar os contratos mais importantes. Os demais itens podem chegar depois, sem exigir uma pesquisa exaustiva do catálogo.
