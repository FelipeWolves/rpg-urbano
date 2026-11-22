# HAB — fechamento do ponto 1: Felipe e efeitos contínuos

22/11/2026. Escopo: Sangramento, Veneno e integração do Felipe. Não inicia escudos especializados, nova política de ondas, score ou novos modos PvE.

## Entregas verificadas

| Parte | Implementação |
|---|---|
| Sangramento | 50% ATK registrado na aplicação, mínimo1; A1 usa2ticks |
| Veneno | 4% MaxHP do alvo, mínimo1; Boss limitado ao ATK registrado na aplicação. Duração fornecida pela habilidade |
| Relógio / instâncias | Início de oportunidade natural/extra, antes da ação, inclusive com incapacidade; reações não disparam. Piloto3instâncias por tipo, inclusive da mesma fonte |
| Defesas / morte | TrueDot ignora DEF, mitigação, Defesa, Miss/Block/crítico; respeita escudo/invencibilidade. FIFO, prevenção, auto-revive e Victory Lock reutilizados |
| A1 Garra do Asfalto | 110% ATK; se alvo já tinha debuff, Bleed2 e +10% AP próprio. Básica não silenciável |
| A2 Postura de Vigília | Passiva não selecionável. Carga por aliado/habilidade, capacidade5; reação depois dos hits/R04, executa A1 completa e StandardShield no aliado vivo de menorHP% |
| A3 Eclipse na Floresta de Pedra | Remove até2buffs comuns antes de3hits em todos os inimigos,65%ATK/hit, +25% contra sem-buff, CD4 |
| Piloto | Felipe/Umbra contra dois Ecos de Dissonância, integrado à seleção, automação inimiga, resultado e save |
| Tutorial de laboratório | Variação explícita4/5cargas; normal começa0. Não é tutorial guiado final |

A1 verifica debuff anterior à habilidade. Bleed usa chance/ACC/RESIST; o AP próprio não exige vencer a resistência do Bleed. AP usa100.000 na barra de1.000.000, sem sintetizar turno. Alvo morto não recebe Bleed; efeitos próprios posteriores podem continuar se Felipe sobreviver.

A3 usa ordem de buffs armazenada para dissipação; dourados/prateados/estruturais e passivas nativas não são dissipados como buffs comuns. Bônus consulta buffs no momento do hit. Reflexão, explosão, interrupção do autor morto e reações ao fim da habilidade são do executor comum.

A2 deduplica por ação/alvo: Block, escudo integral e golpe letal contam; Miss/invencibilidade não. Capacidade não depende da equipe escolhida/viva. Consome limite e descarta excedente; uma ativação por habilidade. Agressor morto cancela: douradas preservadas, comuns/prateadas zeradas. Felipe morto pela reflexão do contra-ataque não aplica escudo. Supressão desliga a passiva sem apagar cargas douradas. Desempate do escudo: Felipe, depois menor slot; comparação deHP% é racional exata. StandardShield mantém1%MaxHP/min1/3naturais e regras anteriores de reaplicação.

## Teste manual posterior — não executado pelo agente

- Abrir-Piloto-Felipe.cmd: normal0/5cargas.
- Abrir-Tutorial-Felipe.cmd: pré-carga4/5 para observar a A2 mais cedo.
- Atalhos anteriores e o cenário padrão foram preservados.
- Saves separados: Saved/LocalProgress/felipe-pilot-v1.save e felipe-tutorial-v1.save. progress-v1.save anterior não foi renomeado nem sobrescrito.
- Conferir A2 não selecionável, leitura dos detalhes/cargas, A3 em ambos os inimigos, contra-ataque e escudo. Aceite visual fica com o usuário.
- Savev1 continua entre tentativas; não serializa combate/DoTs/cargas.

## Evidências

13grupos Periodic,13Felipe e10Pilot aprovados; Pipeline10, Controls8 e Outcomes9 também aprovados na verificação final. Não somar como medida única de cobertura. Regressão geral aprovada; UBT final Succeeded e stage56entradas verificado, sem build/cook do pacote. Relatórios: HAB-FECHAMENTO.json e REG-*-TESTES.json. Compilar o adaptador não é executar ou aprovar visualmente o jogo.

## Limites preservados

Atributos provisórios: Felipe20.000HP/2.500ATK/SPD110; Ecos12.000/8.000HP,1.800/1.200ATK,SPD100/90. Não são balanceamento aprovado. Poison não foi artificialmente adicionado a personagem sem ficha aplicadora.

Purificação/transferência genéricas, política nova de ondas, score, outros escudos e modos continuam em ESC/SIS/PvE. Os DoTs já são comuns/temporários, mas essa classificação não implementa todos esses operadores. Fonte do tick exige identidade presente no encontro, mesmo morta; novas ondas devem tratar fontes ausentes explicitamente. Persistência entre encontros não altera SEQ/PAR automaticamente.

Tutorial guiado, fases Luz/Natureza, arte/animação e determinismo medido em Android/iOS permanecem pendentes. Nenhuma tela, execução de jogo/editor/commandlet, Android, internet, instalação ou push. Somente testes nativos, compilação e preparação local sem cook.
