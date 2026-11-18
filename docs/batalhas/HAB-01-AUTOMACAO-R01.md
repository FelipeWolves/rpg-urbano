# HAB-01 parte 4 — automação por prioridade R01

## Contrato

Política inicial autoral, determinística, para escolher uma ação por chamada. Não reproduz nem estima a IA de outro jogo.

- Cada CharacterLoadout armazena automatic_priority, lista ordenada de posições especiais 2 e/ou 3, sem repetição. Padrão: [3,2]. Lista vazia: apenas básica. Posição ausente ou exclusivamente passiva é ignorada. Valores 0, 1, >3 e duplicatas são inválidos; posição 1 é sempre fallback implícito.
- Se o combate terminou, não há ação. Se o ator está impedido por Block, escolher Skip. Isso não faz a passiva ocupar uma ação.
- Para cada posição prioritária, consultar disponibilidade atual: ignorar recarga, incapacidade e requisito não atendido. Híbrida com ativa disponível pode ser escolhida; uma nota passiva não produz efeito.
- Self seleciona o dono. Enemy seleciona inimigo vivo com menor HP absoluto; Ally seleciona aliado vivo com menor HP absoluto, incluindo o dono. Desempate por ID ordinal. Não usa percentual de vida, aleatoriedade ou previsão do futuro.
- Sem especial válida, escolher a básica canônica com a seleção já existente. Não há defesa automática no perfil de equipes.
- Escolher não altera estado, eventos, orçamento, AP/SPD ou turno. Executar é uma chamada separada a ProfileBattle::apply; comando manual e automático passam pela mesma validação. Uma escolha guardada pode ficar obsoleta e será revalidada, sem autorização antecipada.
- choose_basic_action continua disponível como controle básico explícito. choose_automatic_action implementa a política. No clássico, choose_automatic_action mantém a escolha básica histórica, sem passar a usar especiais implicitamente.
- Configuração participa do ProfileSnapshot pelo loadout. Reset mantém a política e reconstitui o combate. Não há serialização em disco nesta parte.

## Integração

O time B da sessão textual de equipes usa a política; o time A continua sob comandos do jogador. O log informa ID e alvo da habilidade automática após aceitação. Automação do time A, botão/toggle de batalha automática e editor de prioridades não estão nesta entrega. Prioridades são configuração C++/API de perfil, não comandos novos.

Limites de batalha e salvaguarda de 256 ações automáticas consecutivas da apresentação continuam existentes. Uma habilidade própria de CD zero pode dominar a prioridade e acabar em timeout; a política não garante vitória, dano ótimo, uso inteligente de buffs, economia de recursos ou fim por eliminação. Não duplicar lógica de aplicação na IA para corrigir balanceamento.

## Aceite

Configuração inválida/lista vazia; prioridade padrão/invertida; escolha sem mutação e paridade manual/auto; fallback de passiva/CD/condição; ativação após requisito atendido; Self/Enemy/Ally e alvo derrotado; Skip, ação obsoleta e clássico; replay/reset/fim/orçamento em perfis fixo/AP; integração textual de especial e básica subsequente.

A matriz automática inclui 16 combinações curtas de política, relógio e especial de dano/escudo, com replay e orçamento. Não é benchmark, servidor, paralelismo, busca estratégica ou validação de catálogo completo. SEQ/PAR permanecem fora do adaptador de habilidades do protótipo.

Próximo: parte 5, integração/regressão do pequeno conjunto autoral e documentação de limites. HAB-01 não fica fechado apenas pela conclusão desta política.
