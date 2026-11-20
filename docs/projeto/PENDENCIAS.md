# Pendências e pontos a esclarecer

## Síntese vigente — revisão20/11/2026

**As12 entregas foram estudadas.** [Matriz por grupo e pontos ENT-01–15](REVISAO-ENTREGAS-2026-11-20.md). Recebimento e revisão encerrados para as versões registradas; implementação não iniciada. Informações de aparelho, imagem única, primeiro roteiro e diretrizes de produto já chegaram: não repetir perguntas genéricas. As decisões principais foram posteriormente respondidas em RESPOSTAS-ENT-2026-11-20.md; faltam implementação e detalhes técnicos delimitados, não o questionário original. Android Studio instalado em D:\Android; SDK/NDK/AVD ainda não confirmados, ver [dependências](ANDROID-DEPENDENCIAS.md).

Para reunir respostas e recursos por área, use a [central de entregas](../../entregas/README.md). Os guias são listas de preparação, não novas pendências que reabrem ESC/TIME/RES/SEQ/PAR. Este documento continua sendo a fonte das dúvidas/contratos; arquivos recebidos só mudam seu estado depois de revisados.

Registro central de perguntas e decisões. **Documento de pesquisa recebido; não estamos mais aguardando seu envio.** A revisão está em [REVISAO-PESQUISA-HABILIDADES-ITENS.md](REVISAO-PESQUISA-HABILIDADES-ITENS.md). Os itens abaixo são a lista vigente para a próxima devolutiva. As respostas ESC abaixo substituem as recomendações iniciais; VIS/REG/SIS mantêm seus estados próprios.

✅ Núcleo, R04, HAB-01, CIC-01 e perfis SEQ/PAR preservados no escopo de seus aceites. Ponte Unreal executada, pacote básico e fluxo visual3×3 já observados; interação revisada compilada, mas ainda sem aceite visual. Evidências em [ESTADO.md](ESTADO.md) e no [painel](../batalhas/PAINEL-BLOCOS.md).

### Situação após implementação

ESC-01 a ESC-07 estão fechados no caso comum; o código e os testes estão em [ESCUDO-PADRAO.md](ESCUDO-PADRAO.md). As propostas antigas preservadas no histórico não são tarefas abertas. A comparação com Clara foi documentada e o usuário aprovou mínimo de1 ponto para a aplicação válida do gerador direto de1%/3 turnos; essa correção já está implementada. Outros geradores, inclusive cura excedente, continuam extensões próprias. VIS-01–03 também aguardam pacote atualizado e aceite visual autorizado. Silêncio/supressão, bloqueio automático, atributos/equipamentos clássicos, categorias específicas e bypass continuam extensões. Cura e máximo efetivo mutável ainda não existem; testar não recálculo nessas operações quando forem introduzidas.

### Como preencher e priorizar

- **✅ P1 — contrato comum fechado:** ESC-01 a ESC-07 respondidos e implementados para o escudo padrão. Contrato/evidências em [ESCUDO-PADRAO.md](ESCUDO-PADRAO.md); não pedir novamente essas decisões. O aceite visual é separado.
- **🟠 P2 — necessário antes da família correspondente:** VIS-01 a VIS-03 e REG-01 a REG-03. Pode pesquisar tudo agora, mas cada extensão só entra no código em seu bloco.
- **🟡 P3 — planejamento e auditoria:** SIS-01 a SIS-04. Registrar respostas sem ampliar automaticamente o escopo.
- **✅ Preservado:** contratos já validados. Uma hipótese externa diferente não os invalida. Sem repetição de testes por simples atualização documental.

Para cada ID, responder: **decisão desejada para nosso jogo; exemplo numérico/cenário; fonte e versão (se referente ao Dislyte); exceções; dúvida restante**. Fonte externa pode ficar “não localizada”; preferência própria não precisa ser apresentada como fato do outro jogo. Respostas parciais são aceitas. O usuário pode devolver todos os itens juntos.

## ✅ P1 — contrato do escudo padrão fechado

### ESC-01 — base do cálculo (explicação1)

**Resposta do usuário — base definida:** o escudo padrão usa **1% do HP/PV máximo do aplicador**, concedido como proteção sobre a vida atual de quem recebe. O HP atual do protegido não é uma segunda multiplicação na fórmula. Exemplo: aplicador com1000PV máximos concede10 pontos; sobre protegido com50PV atuais, esses10 equivalem visualmente a20% da vida preenchida. Com aplicador de2000PV máximos, seriam20 pontos antes de outras regras ainda abertas.

Esta decisão substitui a proposta inicial de10% do HP atual do protegido para o escudo padrão. Escudos fixos históricos foram preservados; a terceira ativa do cenário foi migrada explicitamente para o novo tipo StandardShield. Ver ESCUDO-PADRAO.md.

**Categorias propostas pelo usuário:** padrão (sem borda diferenciada), prateada e dourada. O escudo descrito pertence à categoria padrão. A classificação é uma proposta para nosso projeto inspirada no Dislyte; não comprova regras universais do jogo de referência. Borda não implica automaticamente potência maior, imunidade, prioridade, acúmulo ou permissão de dissipação. Esses comportamentos permanecem em SIS-01.

**Extensibilidade solicitada:** efeitos futuros poderão usar HP atual, máximo, perdido ou outros atributos, com bases e comportamento explicitados por definição. Não aplicar a regra de1% a todos os efeitos ou categorias.

**Estado:** ESC-01 definido; demais regras comuns também respondidas nas seções seguintes e implementadas em StandardShield. A faixa limitada à vida preenchida não define teto mecânico.

### ESC-02 — instante e recálculo (explicação2)

**Resposta do usuário — cálculo definido:** escudos padrão, sem borda diferenciada, calculam seus pontos uma única vez na aplicação. Usar a base definida em ESC-01 (1% do HP/PV máximo do aplicador nesse instante). Depois de aplicado, o escudo pertence ao estado do protegido; alterações posteriores de HP/PV atual ou máximo do aplicador ou protegido não recalculam nem recompõem seus pontos. O aplicador não continua participando do cálculo numérico após a aplicação. Morte/remoção do aplicador e duração são questões específicas de ESC-06/07, não decididas implicitamente aqui.

**Independência do percentual de vida do protegido:** a mesma regra de aplicação vale com vida cheia ou parcial, desde que tenha pelo menos1PV. Ter pouca vida não reduz por si só os pontos calculados pela base de ESC-01. Isso não autoriza revive ou aplicação funcional em morto. O limite visual da faixa não limita por si só o valor mecânico do escudo.

**Cronologia ilustrativa:** aplicador com1000PV máximos concede10 pontos a um protegido com50PV atuais. Cura ou mudança posterior do HP máximo não altera esses pontos. Depois de consumo de proteção por dano conforme a regra de absorção a fechar em ESC-04, cura não restaura pontos consumidos.

**Esclarecimento respondido pelo usuário — opção1:** o escudo padrão absorve somente até os pontos restantes; dano excedente atinge a vida e pode matar. Com5PV e10 pontos de escudo,20 de dano esgotam o escudo e derrotam o protegido. Não há garantia de1PV nem prevenção de morte inerente ao escudo. Eventuais efeitos separados de prevenção continuam seguindo R04.

**Estado:** ESC-02 definido e implementado; demais regras comuns também respondidas e implementadas em ESC-03 a ESC-07. A condição de pelo menos1PV permite receber proteção; não representa um piso de sobrevivência.

### ESC-03 — arredondamento e limites (explicação3)

**Decisão vigente revisada pelo usuário:** arredondar para baixo, mas garantir mínimo de1 ponto na concessão válida do escudo padrão direto. Força = max(1, floor(PV máximo do aplicador/100)). Exemplos de máximo do aplicador:1/9/10/55/99→1;100→1;550→5;1000→10. Isso substitui o descarte anteriormente aprovado quando esse percentual arredondava para0.

**Distinções preservadas:** zero de vida significa morto e não pode ser alvo; o mínimo de proteção não concede piso de1PV. Zero de proteção restante significa ausência de escudo/ícone: consumo ou expiração removem o efeito, sem repor1 ponto. Sem condição de aplicação satisfeita, não criar escudo por inferência; o mínimo não é regra universal para cura excedente ou outros efeitos futuros.

**Limites:** sem soma entre aplicações padrão, sem cap adicional de gameplay inventado; int positivo válido e divisão por100 evitam overflow de multiplicação. INT_MAX e valores pequenos constam dos testes. Não derivar cap mecânico do tamanho da faixa de vida.

**Estado:** mínimo e arredondamento aprovados para este gerador, implementação/testes atualizados. Outras fórmulas, escalas e valores negativos conservam seus contratos próprios. Relatório em ESCUDO-PADRAO.md.

### ESC-04 — absorção e ordem do dano (explicação4)

**Resposta do usuário — absorção definida:** escudo absorve antes do HP/PV. Cada hit consome separadamente os pontos restantes. Ao atingir0, o escudo desaparece, sem ícone/proteção ativa; o dano excedente daquele hit atravessa para a vida. Escudo não garante sobrevivência com1PV. Aplicar o restante da cadeia segundo R04; não retomar hits cancelados nem alterar Victory Lock.

**Exemplos derivados das decisões:**5 de escudo contra8 de dano disponível para absorção consome5 e retira3PV;10 de escudo contra dois hits de6 resulta, no primeiro,4 pontos de escudo restantes sem perda de PV e, no segundo, escudo esgotado e perda de2PV. Com5PV +10 de escudo contra20 de dano, o alvo morre se não houver outro efeito de prevenção. Dano zero não consome pontos de escudo pela absorção numérica; isso não define gatilhos de “ao ser atingido” de futuras habilidades.

**Exceções separadas solicitadas pelo usuário:** quais ataques ignoram escudo e quais proteções resistem a esse bypass serão especificados por habilidade/equipamento/categoria, com base na pesquisa recebida e nas decisões do projeto. O escudo padrão não ganha bypass ou resistência a bypass implicitamente. São interações com o escudo que exigem precedência explícita, mas não devem sobrecarregar sua regra comum. Registrar futuros contratos em SIS-01/02; a lista recebida não é catálogo executável nem evidência externa já auditada.

**Ordem confirmada pelo usuário:** dano → reduções aplicáveis → absorção pelo escudo → dano excedente no HP/PV. Exemplo aprovado:20 de dano com redução aplicável de50% e5 de escudo resulta em10 após redução,5 absorvidos e5 de perda de PV. Sem redução ativa,20 de dano contra5 de escudo retira15PV. A defesa clássica já existente é um exemplo de redução aplicável; nenhuma redução nova foi criada. O perfil Equipes continua sem comando de defesa genérica. Futuras reduções por habilidade/equipamento exigem contratos próprios.

**Estado:** regra comum de ESC-04 definida: ordem das reduções/escudo/vida, consumo por hit, remoção ao zerar e excesso letal. Exceções de bypass adiadas para seus contratos próprios; não bloqueiam o caso comum. Caso comum implementado; exceções futuras não reabrem este aceite. Implementação comum concluída; evidências em ESCUDO-PADRAO.md.

### ESC-05 — reaplicação e coexistência (explicação5)

**Decisão revisada pelo usuário — maior valor por propriedade:** uma segunda aplicação do escudo padrão nunca reduz a força restante nem a duração restante do primeiro. Comparar as duas propriedades independentemente: pontos resultantes = max(pontos restantes, novos pontos); duração resultante = max(duração restante, duração da nova aplicação). Não somar pontos nem durações. A propriedade menor pode ser substituída pela maior da nova aplicação, mesmo quando a outra propriedade permanece do escudo anterior.

**Exemplos de contrato:**10 pontos/3 turnos restantes + novo20/1 resulta20/3;20/1 + novo10/3 resulta20/3;20/3 + novo10/1 conserva20/3;10/1 + novo20/3 resulta20/3. Valores de duração são exemplos, não duração padrão definida. Empate não altera a propriedade. Comparar duração restante, não restaurar por inferência a duração original já transcorrida.

**Substitui decisões anteriores:** a proposta anterior de sempre usar a força do novo escudo, mesmo menor, foi revogada por esta decisão. O reinício de duração anteriormente aceito passa a respeitar o maior entre duração restante e duração oferecida; não diminuir duração nem somar tempos. O modo de medir essa duração permanece em ESC-06.

**Cálculo da nova força:**1% do HP/PV máximo do novo aplicador no instante da aplicação, arredondado para baixo (ESC-01/02/03). O escudo existente não é recalculado. **Revisão aprovada:** cálculo percentual abaixo de1 recebe mínimo de1 na aplicação válida. Essa oferta pode renovar duração, preservando a maior força/duração; não é mais ignorada. Não cria proteção para alvo morto ou condição inválida.

**Alvos e categorias:** protegido com0PV não pode ser escolhido. Esta regra vale apenas para escudo padrão sem borda diferenciada; escudos prateados, dourados e outras categorias terão contratos próprios futuros. Não generalizar para StructuralShield. Não reaplicar efeitos de abertura nem disparar gatilhos de expiração por inferência.

**Escopo esclarecido pelo usuário:** as regras ESC do escudo padrão descrevem uma habilidade que concede escudo ao próprio aplicador ou a outro personagem. Não constituem, por si só, regras de atributo defensivo/equipamento do modo clássico. Na autoaplicação, aplicador e protegido são o mesmo personagem, mantendo a mesma base de cálculo.

**Clássico — conceito solicitado para desenvolvimento futuro:** uma proteção/atributo próprio poderá ter bases e modificadores por equipamento, com regras separadas. Também poderá existir habilidade clássica que conceda o escudo convencional sem borda, por mecânica de aquisição/uso a definir nesse perfil. Não cadastrar habilidade ou equipamento implicitamente nem confundir esse atributo com o comando Defesa.

**Correspondência com o código atual:** `StructuralShield` é uma camada técnica de proteção consumível representada como status de categoria Structural no núcleo compartilhado. Não é um atributo de equipamento clássico já implementado e não é exclusivo do perfil Classic. Hoje o executor consome Shield antes de StructuralShield. Preservar esse comportamento validado até contrato de evolução/migração próprio; o nome existente não confirma a suposição de equivalência. Atributos/equipamentos, recuperação dessa proteção, suas bases e coexistência com escudo de habilidade permanecem futuros. Não interpretar borda do ícone como StatusCategory do núcleo.
**Limites e metadados:** não há acúmulo de escudos padrão em paralelo. Não foi escolhido cap adicional por aplicação; força deriva de1% do máximo válido do aplicador. Cap específico de gameplay permanece opcional futuro. Propriedades podem resultar de fontes diferentes; eventual atribuição de origem por propriedade e condições específicas de escudos futuros precisam de contrato SIS-01; para o padrão, morte do aplicador não remove a proteção (ESC-07), sem dependência numérica posterior do aplicador. Caso comum implementado; ver ESCUDO-PADRAO.md.

**Estado:** ESC-05 comum definido e implementado; ESC-06/07 também respondidos abaixo. Origem por propriedade e categorias futuras só serão necessárias para efeitos que usem esses metadados.

### ESC-06 — duração e relógio (explicação6)

**Resposta do usuário — relógio do protegido:** o escudo padrão passa a proteger assim que aplicado e sua duração acompanha o turno de ação do protegido, não o contador global nem turnos do aplicador. Quando ocorre o turno natural do protegido depois de receber o escudo, descontar1 ao concluir essa oportunidade, mesmo que esteja impossibilitado de agir; manter a proteção durante o turno. Quando já havia agido antes da aplicação, não descontar retroativamente. O escudo expira quando a duração chega a0, podendo acabar antes por consumo dos pontos (ESC-04).

**Autoaplicação:** a ação que aplica escudo no próprio personagem não reduz imediatamente sua duração. O novo efeito entra ao final dessa ação; não cobrar um turno já realizado. Aplicação em outro personagem segue o turno natural futuro do protegido. A fronteira precisa identificar nova aplicação/reaplicação durante a própria oportunidade para evitar desconto imediato indevido; manter a comparação de maior duração de ESC-05.

**Ações fora do turno natural:** ação concedida por outro efeito, assistência, perseguição, contra-ataque ou turno extra não deve reduzir a duração deste escudo padrão. O usuário confirmou que turno extra não altera sua duração. Isso vale como contrato do escudo, sem mudar relógios de outros efeitos/habilidades. Morte do aplicador não altera a duração do escudo já concedido. Outros tipos de escudo poderão declarar mecânicas diferentes futuramente.

**Significado técnico:** NaturalOnly distingue oportunidades naturais do dono de oportunidades extras. EveryTurn inclui naturais e extras reconhecidas pelo escalonador (conforme a política da fonte de extra); não significa receber dano nem ser alvo de qualquer efeito. Reações não se tornam turnos do portador só por executarem ações. A flag atual pode não bastar se uma fonte de extra solicitar tick de efeitos: a implementação deverá garantir a regra própria do escudo, sem assumir equivalência completa pelo nome NaturalOnly.

**Cronologia ilustrativa (2 turnos de duração, apenas exemplo):** escudo aplicado emB → próximo turno natural em queB age: duração2→1 ao fim → reação/extra deB: continua1 → próximo turno natural em queB age:1→0 e remoção. SeB já agiu antes da aplicação, seu turno anterior não conta. Autoaplicação com2 mantém2 ao fechar a ação aplicadora. Cura e mudanças de HP não alteram esse relógio.

**Esclarecimento confirmado — impossibilitado de agir:** se chega a oportunidade natural do protegido e ele não pode agir, o escudo continua válido durante esse turno e perde1 de duração no encerramento. A contagem acompanha a oportunidade natural, não a execução bem-sucedida de uma ação. Manter o fluxo de Skip existente quando aplicável; não descontar em comandos inválidos nem em ações extras/reações. Isso substitui a ambiguidade anterior sobre oportunidade sem ação.

**Terminologia decidida pelo usuário:** chamar essa condição de “impossibilitado de agir”, não “bloqueio”. O identificador técnico atual EffectKind::Block representa impedimento de ação, não chance de anular dano. A revisão de rótulos/textos e eventual renomeação técnica deverá preservar esse comportamento e suas evidências, sem converter enum em mecânica nova por troca de nome.

**Bloqueio — conceito futuro separado:** chance de defesa automática contra ataque; quando ocorre, o personagem não recebe dano desse ataque. Diferente do comando Defesa clássico, que reduz o dano pela metade e arredonda para baixo. Ainda especificar chance/origem, elegibilidade, resolução por ataque ou hit, interação com escudo e efeitos ao acertar, reações, dano inevitável e regras por perfil antes de implementar. Não atribuir probabilidade, imunidade a debuffs ou gatilhos por inferência. Trata-se de decisão de conceito do nosso projeto, não equivalência comprovada com Dislyte.

**Duração padrão confirmada pelo usuário:**3 turnos naturais do protegido. Uma aplicação válida oferece3 de duração; a reaplicação conserva max(duração restante,3), independentemente de max(proteção restante,nova proteção), sem somar. Aplicação válida com resultado percentual abaixo de1 oferece1 ponto e3 turnos, preservando os maiores valores. Autoaplicação mantém3 ao encerrar a ação aplicadora. Depois, cada oportunidade natural concluída desconta1, inclusive quando impossibilitado de agir:3→2→1→0 e remoção; extras/reações não descontam. Se a proteção for consumida antes, o escudo termina antes.

**Estado:** ESC-06 definido documentalmente para o escudo padrão:3 turnos, relógio do protegido, efeito imediato, decremento ao fim de oportunidade natural inclusive sem ação por controle, sem desconto imediato na aplicação/autoaplicação, sem tick em extras/reações e sem dependência da vida do aplicador. Morte do protegido e transições definidas em ESC-07. Implementação e testes do caso comum concluídos no incremento ESCUDO-PADRAO.md.

### ESC-07 — morte, revive e transições (explicação7)

**Resposta do usuário — comportamento comum definido:** remover o escudo padrão na morte do protegido. Morte do aplicador não encerra o escudo já aplicado; revive não restaura o escudo removido. Por padrão, transição de onda, confronto ou troca de equipe não conserva os pontos nem a duração desse escudo. Expiração não dispara efeito adicional. Escudos específicos ou encontros com políticas próprias poderão definir exceções futuramente, explicitamente e com testes próprios.

| Evento | Regra comum do escudo padrão |
|---|---|
| Morte do protegido | Remover pontos e duração; sem ícone ativo |
| Morte do aplicador | Não remover nem alterar escudo já aplicado |
| Revive do protegido | Não restaurar o escudo anterior; nova aplicação válida é independente |
| Mudança de onda | Não conservar escudo: remover pontos e duração |
| Novo confronto | Não conservar escudo: remover pontos e duração |
| Troca de equipe | Não transferir nem conservar escudo da equipe anterior |
| Duração chega a0 | Remover sem consequência adicional própria de expiração |
| Proteção chega a0 | Remover; excesso do hit atinge HP/PV conforme ESC-04 |
| Nova aplicação válida com percentual arredondado para0 | Conceder mínimo de1 ponto e oferecer3 turnos; reaplicar por maiores propriedades |

**Explicação de Preserve/Reset:** são nomes de políticas técnicas. Preserve conserva estados entre etapas quando o contrato do encontro permite; Reset reinicia/remove os estados abrangidos por aquela política. Não são comandos novos do jogador. Para o novo escudo padrão, a regra comum solicitada é não transportar proteção/duração entre essas transições. Uma eventual exceção deve indicar quais eventos conservam o escudo e não recuperar pontos/tempo já gastos.

**SEQ/PAR:** SEQ-01 é o coordenador de equipes sucessivas; PAR são perfis de frentes coordenadas, incluindo chefe compartilhado. Seus aceites já testam políticas explícitas para os estados existentes. A nova regra do escudo deve ser integrada/testada como extensão declarada, sem substituir silenciosamente políticas genéricas Preserve/Reset ou mudar outros estados (HP, recargas etc.). Se um modo explicitamente conservar esse novo escudo, será exceção documentada, não uma mudança universal dos contratos encerrados.

**Integração implementada:** Stage remove StandardShield na transição e SEQ remove o estado temporário; testes cobrem ambos. PAR mantém o escudo ao alternar ações entre frentes vivas, sem confundir agendamento com troca de equipe. Nova aplicação após revive é permitida apenas com alvo vivo e demais condições válidas; nenhuma autoaplicação/recompensa/gatilho de remoção implícito. A ausência de efeito próprio de expiração não inventa imunidade a futuros observadores de eventos definidos separadamente.

**Estado:** ESC-07 definido documentalmente para o comportamento comum. Exceções futuras precisam de contrato específico. Implementação e testes do caso comum concluídos em ESCUDO-PADRAO.md.

## 🟠 P2 — apresentação e contratos relacionados

As respostas ESC também encerram partes dos itens abaixo. **Definido**, **implementado/testado nativamente** e **aceito visualmente** são estados diferentes. Não pedir de novo decisões já registradas; pesquisar/testar apenas o restante.

### VIS-01 — faixa de escudo e painel (explicação8)

**✅ Definido:** proteção azul-clara sobre a vida preenchida; pontos reais e duração no painel; zero significa ausência de escudo/ícone. O valor vem de1% do máximo do aplicador, não de uma segunda porcentagem do HP atual do protegido. Pontos e duração seguem as regras de reaplicação fechadas em ESC-05/06. Excesso visual não limita proteção mecânica.

**✅ Implementado/testado nativamente:** StandardShield é incluído na faixa/painel e os dados de proteção são testados. Aplicação válida com percentual abaixo de1 cria proteção mínima de1; consumo/expiração removem o estado; dados expostos vêm do núcleo. Build Editor aprovado em [ESCUDO-PADRAO.md](ESCUDO-PADRAO.md). As passivas fixas históricas permanecem distintas.

**⚠️ Falta somente no aceite desta apresentação:** observar faixa/ícone/painel durante aplicação, consumo, reaplicação e expiração; confirmar valores completos e legibilidade. Cenários: aplicador1000PV→10 pontos sobre protegido50PV; aplicador550PV→5; aplicador com máximo abaixo de100PV→1 ponto; proteção maior que a vida preenchida. Conferir que o ícone some quando não há escudo e que uma aplicação mínima preserva maior proteção e pode renovar duração.

**Estado revisado por solicitação do usuário:** contrato anterior implementado/testado, sem alteração automática. Antes de avançar no aceite, comparar a geração do escudo associado à básica de Clara em [ANALISE-CLARA-ESCUDO-VIS-01.md](ANALISE-CLARA-ESCUDO-VIS-01.md). A descrição comunitária associa cura da básica à conversão de excedente pela passiva, não à regra universal de1%. O usuário manteve o gerador de1%/3 turnos e aprovou mínimo de1 ponto. Generalizar outros geradores continua extensão futura, não bloqueia essa correção. Essa revisão é de fórmula/escopo do gerador; a UI continua recebendo pontos reais. Fonte primária atualizada e arredondamento de Dislyte ainda não confirmados; código próprio atualizado apenas para o mínimo aprovado. Baixo HP atual do protegido não zera a fórmula vigente, que usa máximo do aplicador.

### VIS-02 — habilidades, alvos e interação

**✅ Definido anteriormente e reforçado pelas respostas:** ativa de escudo pode atingir o próprio personagem ou um aliado vivo; personagem com0PV não pode ser escolhido para recebê-la. A possibilidade do efeito é distinta do alvo cadastrado: a terceira ativa do cenário atual continua Self, sem ganhar seleção de aliado implicitamente. Aplicação válida tem mínimo de1 ponto; cria escudo/ícone, sem tornar gratuita a habilidade executada.

**✅ Implementado/testado nativamente:** validação de alvo vivo, recusa sem alteração do combate, informação da habilidade padrão, candidatos e gesto curto/prolongado. Passivas são consultáveis e não selecionáveis; nível só quando cadastrado. Textos do impedimento de ação foram alterados para “impossibilitado de agir”/“unable to act”, sem implementar bloqueio automático.

**⚠️ Falta verificar visualmente:** abrir/fechar/paginar detalhes; soltar após gesto sem agir; consultar passiva/recarga/condição; selecionar/trocar alvo e confirmar conforme apresentação; automação suspensa durante consulta; motivos claros de recusa, incluindo alvo morto. Conferir a nova terminologia sem confundir indisponibilidade da habilidade com bloqueio automático de dano. Ícone final de recarga/cadeado/indisponibilidade e imagem personalizada de alvo ainda exigem acabamento e aceite, sem presumir entrega só pelos textos.

**Estado:** parcialmente concluído; não repetir perguntas sobre alvo morto, categoria de ação ou básica. Arte de ícones/marcador, toque e validação visual permanecem abertos; abrir aplicativo exige autorização específica.

### VIS-03 — formações, tela e acessibilidade

**✅ Já definido/testado no escopo próprio:** formações de1–5, apresentações Standard em profundidade e Classic lateral separadas das regras; geometria com6 grupos nativos anteriores. Essas decisões não dependem de escudo.

**⚠️ Continua pendente:** inspeção visual1/3/5 e assimetrias; rótulos ao fundo sem caixas agregadoras; resoluções/redimensionamento; mobile/toque e acessibilidade. Sexta posição é extensão não aprovada como ampliação global. Retrato2D original e UI de chefe/auxiliares ainda pendentes; não inferir HP compartilhado pelo layout.

**Estado:** as respostas ESC não resolvem esses testes/recursos. Não pedir nova definição de formação para reconhecer que ainda falta observá-la.

### REG-01 — defesa clássica e bloqueio automático (explicação9)

**Atualização20/11 — informação recebida:** Grupo03 trouxe chance3%/bases por classe/tetos e mitigação30–50%, que conflita com bloqueio que anulava dano. ENT-01/07/14 precisam reconciliar isso antes de implementação; Defesa atual preservada. [Revisão consolidada](REVISAO-ENTREGAS-2026-11-20.md).

**✅ Definido e implementado:** Defesa clássica continua reduzindo o dano pela metade, para baixo, consumida no fecho de ação que a utilizou ou expirada na próxima oportunidade do dono. Sua redução vem antes da absorção do escudo. Escudo por habilidade, proteção/atributo futuro do clássico e comando Defesa são conceitos distintos. Câmera lateral não habilita defesa no modo Equipes.

**✅ Conceitos futuros já escolhidos pelo usuário:** bloqueio será chance automática de anular dano de ataque, diferente de Defesa; o clássico poderá ter atributo/proteção próprio modificado por equipamentos e também habilidade que conceda o escudo convencional.

**⚠️ Falta apenas antes dessas extensões:** probabilidade/origem do bloqueio, checagem por ataque ou hit, ataques elegíveis/inevitáveis e relação com efeitos ao acertar/reações; bases/recuperação/modificadores do atributo clássico e forma de aquisição/uso da habilidade nesse perfil. Uma variante de Defesa de maior duração é possibilidade futura, não pergunta obrigatória nem mudança aprovada.

**Estado:** interação comum Defesa→escudo encerrada/testada; extensões futuras não implementadas. Não reabrir a defesa atual.

### REG-02 — causalidade e vitória (explicação10)

**✅ Encerrado no contrato vigente:** R04/RES-01 preservados: reflexão/explosão FIFO inline, interrupção de hits quando aplicável, auto-revive e Victory Lock antes de reações; morte dupla favorece o lado do autor causal da ação atual. Escudo absorve após reduções e antes da vida, não oferece piso de1PV e não altera essa causalidade.

**✅ Evidência:** regressão da resolução aprovada com o novo escudo integrado; testes próprios verificam absorção por hit, excesso e morte/revive. Não são todas as combinações futuras de habilidades.

**Estado:** nenhuma resposta adicional necessária agora. Pesquisa externa é opcional para referências futuras; só mudança relevante justifica reabrir implementação/validação.

### REG-03 — silêncio, impossibilidade de agir e supressão (explicação11)

**Atualização20/11 — informação recebida:** Matriz concreta recebida no grupo03: silêncio restringeA2/A3, supressão desliga passivas, Freeze/Petrification impedem ação. Não é mais ausência de matriz; falta fechar clocks/extras, pendentes/auras e precedênciaR04 (ENT-06). Diferenciações15/25% são propostas, não código. [Revisão consolidada](REVISAO-ENTREGAS-2026-11-20.md).

**✅ Respondido:** impedimento de ação deve ser chamado “impossibilitado de agir”; Block técnico atual não é o futuro bloqueio probabilístico. A oportunidade natural impossibilitada ainda reduz duração do escudo padrão. Extras/reações não reduzem esse escudo. Sua aplicação não altera relógios de outros efeitos. Básica na posição1 não é silenciada por decisão anterior do projeto, mas pode não ser executada se o personagem estiver impossibilitado de agir.

**✅ Implementado/testado no escopo do escudo:** contagem por oportunidade natural, inclusive Skip; nenhuma perda em extra mesmo com fonte que solicita tick. Terminologia pública atualizada; nome técnico legado preservado.

**⚠️ Perguntas restantes:** quais condições concretas causam impossibilidade de agir? Silêncio bloqueia quais ativas? Supressão desliga quais passivas de abertura/combate? Congelamento e petrificação fazem quais operações? Como contam recargas e outros efeitos, e o que ocorre com reações/extras já enfileirados? Quais exceções por unidade/efeito? FreezeCooldowns não é personagem congelado.

**Estado:** parcialmente respondido; falta matriz efeito×comando/gatilho/clock para essas famílias. Não pedir novamente a regra do relógio do escudo ou a distinção de termos.

## 🟡 P3 — pesquisa ampla e evolução (explicação12)

### SIS-01 — origem, permissões, âncoras e quotas

**Atualização20/11 — informação recebida:** Grupo03 trouxe molduras, âncoras, scopes e quotas declarativas. Revisar caso a caso e limitações do núcleo; não generalizar esses defaults sobreESC. Esquema anuncia8campos mas enumera7: revisar completude ao formalizar, sem inventar campo. [Revisão consolidada](REVISAO-ENTREGAS-2026-11-20.md).

**✅ Respondido para o escudo padrão:** origem é habilidade, aplicada em si ou em aliado vivo; base é máximo do aplicador no instante de aplicação; estado pertence ao protegido. Âncora é fim de oportunidade natural do protegido, inclusive impossibilitado de agir; extras/reações não contam.3 turnos, mínimo de1 na aplicação válida, maiores força/duração independentemente sem soma, remoção na morte do protegido/transições e nenhuma dependência posterior do aplicador. Expiração não dispara consequência adicional. Categorias prateada/dourada terão regras próprias, não herdadas pela cor.

**✅ Implementado/testado:** esse caso comum e sua apresentação de dados estão em ESCUDO-PADRAO.md. Não é um framework completo de atributos/equipamentos.

**⚠️ Resta para outros efeitos/sistemas:** origem por equipamento/divinate/modo; âncoras do aplicador/globais e outras persistências; quotas por fonte/alvo/time/cadeia/batalha; dissipação, roubo, transferência, imunidade e supressão; coexistência/prioridade de outras categorias. Silver/Golden têm divergências na pesquisa recebida. Não usar o nome técnico StructuralShield como prova de categoria visual ou atributo clássico.

**Estado:** parcialmente concluído, com contrato padrão fechado. Não reenviar perguntas genéricas de reaplicação/âncora do escudo já respondidas; especificar a nova categoria/efeito ao perguntar.

### SIS-02 — fórmulas, atributos e auditoria de fontes

**Atualização20/11 — informação recebida:** Fórmula e exemplos recebidos; fontes externas não auditadas. ENT-07 separa piso1dano/TrueDamage/limites/aritmética da regra vigente; nenhum mínimo universal ou nova mitigação foi aplicado. [Revisão consolidada](REVISAO-ENTREGAS-2026-11-20.md).

**✅ Respondido/implementado:** força padrão = max(1,HP máximo inteiro positivo do aplicador/100); divisão inteira arredonda o percentual para baixo sem overflow por multiplicação, e aplicação válida garante1 ponto. Sem acúmulo ou cap adicional de gameplay inventado. Reduções aplicáveis→escudo→HP; cada hit consome separado e excesso pode matar. Defesa clássica usa divisão por2 para baixo. Testes incluem limites até INT_MAX. Cobertura da faixa visual não é cap mecânico.

**⚠️ Resta:** afinidade, ACC/RESIST, Miss/Crit/Boon, mitigação por DEF, dano verdadeiro, bypass, overkill e teto de HP; arredondamento de fórmulas compostas/negativas; atributos base/efetivos e HP máximo mutável. Cura e máximo mutável não existem no núcleo atual: acrescentar testes de não recálculo do escudo quando forem implementados. Cap específico de gameplay é extensão opcional, não bloqueia o escudo entregue.

**⚠️ Auditoria externa separada:** fórmulas embutidas em imagens precisam ser transcritas/conferidas antes de uso. Ocean não comprova sozinho tick normal de recarga; condição de empate de Toland não comprova desempate universal. Datas conflitantes/catálogo parcial não são baseline confirmado. A pesquisa não foi auditada externamente por termos aprovado regras próprias.

**Estado:** aritmética comum do escudo fechada/testada; fórmulas gerais e auditoria continuam abertas. Auditar apenas o necessário ao próximo contrato, podendo o usuário pesquisar tudo junto.

### SIS-03 — ondas, times, chefes e persistência

**Atualização20/11 — informação recebida:** Grupo04 trouxe matriz de famílias, score25%overkill, limiares70/35 e entidades. ENT-08 precisa preservar transições/escudo/R04 e fechar pontuação; não repetir pergunta genérica sobre as famílias já descritas. [Revisão consolidada](REVISAO-ENTREGAS-2026-11-20.md).

**✅ Respondido/implementado para o escudo padrão:** remover na morte do protegido; revive não restaura; morte do aplicador não remove. Não conservar proteção/duração ao mudar onda/confronto/equipe. Stage aplica a regra mesmo com Preserve genérico, sem alterar outros estados; SEQ remove o temporário nas três políticas testadas. Agendamento entre frentes de PAR-02 não é troca de equipe e não remove escudo; teste específico aprovado.

**✅ Preservado:** políticas explícitas dos modos existentes, SEQ-01 e PAR-02 (HP único/barra global no perfil escolhido); outros estados seguem seus contratos. Novas regras do escudo não justificam reabrir esses aceites.

**⚠️ Resta para novos modos:** distinguir gatilhos de início de combate/onda/rodada/oportunidade em outros efeitos; retorno cíclico de equipes; kit por HP/turno e score/sobrevivência; regras de auxiliares/invocações/alcance entre frentes. Uma exceção que conserve o novo escudo precisa de configuração e teste específicos, ainda não implementados. Não há sistema de equipamentos para exigir exclusividade de peças.

**Estado:** persistência comum do escudo respondida/testada; catálogo de modos/chefes e políticas excepcionais continuam abertos. IDs PEN/PVE históricos seguem rastreáveis, sem invalidar perfis encerrados.

### SIS-04 — inventário, ATB, retomada e plataformas

**Atualização20/11 — informação recebida:** Grupos04/09/11 trouxeram5comandos,ATBWait/Active,6slots,moedas,save e produção conectada. São expansões propostas; ENT-12/14/15 cobrem fragmento incompleto, relógios/custos e autoridade de servidor. Studio confirmado; ambienteAndroid não validado. [Revisão consolidada](REVISAO-ENTREGAS-2026-11-20.md).

**✅ Respondido conceitualmente:** proteção/atributo futuro do clássico terá bases e modificadores próprios por equipamento; não é o comando Defesa nem uma habilidade. Uma habilidade clássica também poderá conceder o escudo convencional, por mecânica de uso própria futura. StructuralShield atual não comprova que esse atributo/sistema de equipamento já exista.

**✅ Limites atuais conhecidos:** save v1 de progresso não restaura combate; geometria/layout não comprovam mobile; execução cooperativa não comprova threads ou servidor. Essas limitações continuam válidas.

**⚠️ Resta:** inventário/estamina como consumível, custos e consumo de oportunidade; definição efetiva dos atributos/equipamentos e acesso à habilidade clássica; ATB Active/Wait, cast delay e recursos; snapshot/retomada/migração de combate; servidor, paralelismo, carga, desempenho e plataformas. Tudo exige contratos/medições próprios. Não inferir essas respostas a partir da duração do escudo.

**Estado:** separação de conceitos parcialmente respondida; sistemas ainda não implementados. Não bloqueiam o caso comum do escudo nem autorizam instalações, rede ou abertura de programas.

## HUB-01A — imagem, vídeo e inicialização

**✅ Definido pelo usuário:** panorama compartilhado por áreas; enquadramento por resolução; vídeo horizontal solicita paisagem no celular; vídeo vertical central no desktop com arte lateral; variantes por dispositivo; janela/resolução ou tela cheia escolhidas antes da abertura.

**✅ Base implementada:** política de recorte/proporção/orientação solicitada e metadados,10 testes próprios. Catálogo atualizado com4 mídias recebidas/cadastradas; autoria própria declarada, metadados conferidos em HUB-01A-RECURSOS.md. [Contrato completo](HUB-01A-APRESENTACAO.md).

**⚠️ Restante:** confirmar recortes/função da imagem (terraço único versus três alas) e compatibilidade real de vídeo; estratégia de distribuição/versionamento de mídias grandes; parser/renderização/roteamento do hub já escritos e compilados, faltando aceite real de hitboxes/fluxo; player/codec; configurações antes de criar janela; rotação/restauração reais e áreas seguras; desempenho e testes mobile. Não reabrir os requisitos já dados só porque a integração ainda falta.

Dois acessos da imagem atual definidos: Evento Especial no outdoor e Batalha no dispositivo; terceiro adiado pelo usuário. Alvos confirmados pela marcação do usuário; parser/renderização/rotas escritos e compilados, mas não executados. [Implementação e aceite restante](HUB-01B-IMPLEMENTACAO.md).

O contrato de câmera do hub não é o perfil de combate clássico. Perguntas de avatar/destinos permanecem em HUB-01-REFERENCIAS.md. Essa frente não altera ESC-01–07 nem resolve mobile pela compilação Windows.

## Briefing visual do hub — respostas recebidas

**✅ Definido:** Evento Especial neon magenta/roxo/ciano, Batalha âmbar com ícone pulsante/legenda, áreas ampliadas e feedback; fundo escuro80%; nome provisório e textos do painel recebidos; nenhuma ilustração adicional exigida. As posições já confirmadas não precisam ser respondidas novamente.

**✅ Respondido/implementado:** Cormorant Garamond principal/Lora complementar, Cinzel excluída; LOBO é codinome; logo WebP recebida; pulso, alvo mínimo e movimento reduzido definidos. Tema/emblema implementados,5 novos grupos nativos e compilação Unreal aprovados. **⚠️ Restante técnico:** fontes e PNG da logo já conectados ao runtime; ampliar aceite de contraste/recortes/textos para outras resoluções, toque/foco, preferência de acessibilidade do sistema e persistência. [Integração vigente](HUB-IDENTIDADE.md). Não repetir escolhas encerradas. [Detalhes](HUB-01C-VISUAL.md).

Não altera o core ou os destinos já implementados. Fonte atual do usuário: BRIEFING-VISUAL-HUB.md.

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


### Fechamento e próxima entrega

✅ Usuário confirmou texto sem corte, seleção funcional e batalha conforme combinado. Fontes/logo conectadas; aceite integral ainda parcial. ✅ Seletor no chão corrigido e aceito visualmente pelo usuário. Investigar apresentação mobile separadamente; fluidez aguarda critérios. Prévia por Abrir-Preview.cmd; editor por Abrir-Editor.cmd, ambos usando build aprovado. Nesta entrega final só se validaram os lançadores sem abrir programas. [Evidências](HUB-IDENTIDADE.md).

Seletor: correção por linhas3D com profundidade junto à base do modelo implementada e posteriormente aceita visualmente pelo usuário; o assistente não abriu a prévia nessa entrega. [Escopo e roteiro](VIS-SELETOR-CHAO.md). Não altera regras nem resolve mobile/fluidez; não repetir esse aceite sem mudança ou defeito novo.

Mobile: [proposta revisada e sequência mínima](MOBILE-PLANO-REVISAO.md). HUD atual é Canvas, não UMG; preview/renderizador não troca automaticamente DeviceProfile Windows. Próxima implementação a planejar: apresentação Desktop/Mobile/Auto separada de plataforma, antes de presumir migração UMG ou instalar SDK. Sem implementação nova por esta revisão.

[Fila de implementação independente](FILA-DESENVOLVIMENTO-INDEPENDENTE.md): distingue trabalho de engenharia já especificado de decisões ainda abertas. Não transformar REG/SIS sem contrato em tarefa autorizada só porque há adaptador possível.

## Respostas ENT recebidas —20/11/2026

**ENT-01–15 respondidos pelo usuário; implementação ainda pendente.** [Decisões vigentes e detalhes por bloco](RESPOSTAS-ENT-2026-11-20.md). PreservarESC/R04/Defesa atual até mudança explícita por implementação. Demo:1 Felipe jogável/1 arena;A2passiva. Pausa/abandono,panmobile,vídeo e futuraeconomia agora têm direção consolidada. Não repetir perguntas principais da revisão anterior;formalizar apenas detalhes técnicos no bloco afetado. Nenhuma execução autorizada por esta atualização.

Ponto1/FelipeA2: regra de cargas,limite,consumo,A1eStandardShield definida;destino confirmado no aliado vivo com menor percentualHP,incluindoFelipe. [Contrato](FELIPE-A2-POSTURA-VIGILIA.md). Não repetir essas perguntas;casos de borda e implementação ficam no bloco futuro. Formação6×6 não implementada por essa decisão.

Ponto2 respondido: Block3%/Michel10% total, teto50% geral e100% especial; True Damage/DoT inelegíveis. Mitigação somente por fonte, aditiva até50%. True Damage ignoraDEF/mitigação/Defender/Block e respeita invencibilidade/escudo. [Contrato e limites](BLOQUEIO-MITIGACAO-DANO-PONTO2.md). Especificado, não implementado; demais perguntas técnicas só no contrato afetado.

Ponto3/SIS-02 definido:trilho de dano separado do trilhoImmunity→chance→ACC/RESIST,piso20%resistência não irresistível;crítico base150%total. Novos sistemas comHP/escudo/dano/scoreint64,teto10^12,intermediáriosdouble/floorporhit,preservandoDefesa1→0 ecenáriospequenos. [Contrato e preparação](FUNIL-ESCALA-NUMERICA-PONTO3.md). Migração/limites/precisão/replay ainda exigem implementação e testes;nenhum código alterado.

Adendo técnico recebido20/11: [matriz vigente](ADENDO-TECNICO-CONTRATOS-2026-11-20.md). Debuff exigehitnãoMiss/nãoBlock;supressão/prevenção e reaçõespendentes detalhadas;SilverShieldcoexiste econsomeapósStandard;ondaslimpamcomunsantesdecurar;scorearredondado uma vez. Temas respondidos não são perguntas abertas de produto;bordas técnicas estão delimitadas no adendo. Implementação ainda pendente.

ConferênciaAndroid20/11 concluída em leitura:Studio D:\Android presente;SdkPath vazio,SDK/NDK/AVD não localizados nas configurações/caminhos examinados. Java25doStudio fora da matriz suportada doGradle8.13/AGP8.13.0daengine. [Resultado e próximos passos](ANDROID-CHECK-2026-11-20.md). Não instalar/configurar/executar automaticamente;buscar SDK personalizado ou preparar componentes/JDK compatível sob autorização. AUTWindows não bloqueado.

**Complemento final recebido20/11:** [A2, ACC/RESIST e ponto fixo](COMPLEMENTO-FINAL-A2-ACC-PONTO-FIXO.md). Bordas de A2 e equação/irresistível respondidos; teto10^12 por saturação; double substituído por ponto fixo de escala10000 no núcleo futuro. Não repetir as perguntas resolvidas. Determinismo bit a bit é meta a verificar, não aceite existente. Nenhum código/teste executado nesta recepção.
