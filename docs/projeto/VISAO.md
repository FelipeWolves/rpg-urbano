# Visão — RPG Urbano

## Estado da visão versus implementação — 18/11/2026

A identidade urbana/musical permanece original e provisória. Hoje há protótipo textual, habilidades/ciclo local e ponte Windows validada na UE5.8.4; ainda não há arena visual. As regras da primeira experiência registradas abaixo são históricas: referências atuais estão no [painel](../batalhas/PAINEL-BLOCOS.md), nos contratos e no [estado vigente](ESTADO.md). Defesa genérica atual arredonda para baixo; tempo e causalidade evoluíram nos blocos TIME/RES. Não usar o primeiro modelo como especificação atual completa.

Nome provisório, sem marca ou identidade final aprovada. Fundação criada em 18/11/2026.

## Proposta

RPG original de equipes e combate por turnos, com personagens estilizados, estética urbana e música como elemento de identidade. Construir primeiro uma batalha clara, responsiva e interessante. O primeiro produto é um protótipo pequeno offline para PC, não um serviço de escala comercial.

Offline/PC é uma hipótese inicial reversível para reduzir dependências; plataforma comercial, mobile, direção artística, história e monetização continuam em aberto. Não usar personagens, nomes, narrativa, interface, música ou arquivos de jogos existentes.

## Primeira experiência jogável

Uma arena, duas equipes de três posições e recursos visuais provisórios. Selecionar um alvo e uma ação; observar o turno, a mudança de vida e a condição de vitória. Poder reiniciar a batalha. A apresentação final e seis personagens únicos vêm depois da validação das regras.

Três posições são o cenário inicial, não um limite estrutural. COM-01A verificou formações de 1–5 de cada lado e sondagens com seis. O usuário acrescentou requisitos de confrontos consecutivos, limites, três times sucessivos com chefe por etapa e frentes paralelas independentes ou compartilhando chefe. Contratos e perfis implementados em WAVE-01/SEQ-01/PAR-01/PAR-02; cobertura vigente em docs/batalhas/PAINEL-BLOCOS.md. COM-02 adiciona interação textual 3x3, ainda sem menus desses coordenadores. Famílias futuras em MODOS-PVE.md não são funcionalidades prontas; não confundir referências históricas com regras finais deste jogo.

## Regras provisórias do primeiro combate

Estas regras servem a um experimento, não representam balanceamento definitivo.

- Seis combatentes: A1–A3 e B1–B3. Uma ação por combatente vivo em cada rodada.
- Ordem: velocidade decrescente, com desempate pelo ID estável em ordem ordinal crescente. Nenhum sorteio nesta primeira versão.
- Vida, ataque e velocidade inteiros positivos na definição. Vida atual entre zero e o máximo.
- Ataque básico: subtrair o ataque do agressor da vida do alvo inimigo vivo, limitando o resultado a zero. Sem defesa, crítico ou afinidades por enquanto.
- Guarda: usar a ação para receber metade do dano no próximo ataque que acertar esse combatente, arredondando para cima. Consome-se nesse acerto ou expira no início do próximo turno do defensor. Não acumula.
- Ao chegar a zero, o combatente deixa de agir e não pode ser escolhido como alvo de ataque. Checar vitória imediatamente após uma ação válida.
- Ação inválida não modifica estado nem consome o turno. Não há ataques a aliados, ações fora do turno ou efeitos após o fim da batalha.
- Vitória ao eliminar a equipe adversária. Empate simultâneo não ocorre nesse conjunto de regras; definir política se futuras habilidades o permitirem.
- Adversário provisório: escolher o inimigo vivo de menor vida atual, desempatar por ID e usar ataque básico. Não utilizar chamadas de IA externa.

Dados de referência: A1/B1 (vida 100, ataque 20, velocidade 12); A2/B2 (140, 15, 8); A3/B3 (80, 25, 16). Simetria facilita verificar o motor; vantagem por desempate é conhecida e não é design competitivo final.

## Aceite do primeiro ciclo

1. Iniciar batalha com os seis IDs e estatísticas de referência.
2. Ver ordem previsível e rejeição de ações inválidas.
3. Atacar e guardar com efeitos coerentes e combatentes derrotados sem agir.
4. Chegar a vitória, impedir ações posteriores e reiniciar em estado limpo.
5. Funcionar sem rede, contas, loja ou integração com instalação externa.

## Fora do primeiro ciclo

Mundo aberto, multiplayer, contas, gacha, loja, servidor, dezenas de personagens, músicas finais, cinematics, produção artística final, minijogo rítmico e migração para engine futura. Não exigidos para avaliar uma batalha. Ideias permanecem em docs/projeto/TAREFAS.md, sem iniciar implementação antecipada.

## Validação humana

Nas versões visuais, o usuário avalia legibilidade, ritmo e decisões de combate. Antes disso, o primeiro motor pode ser validado por simulações textuais locais; isso não será apresentado como protótipo 3D jogável.
