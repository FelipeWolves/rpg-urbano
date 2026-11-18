# HAB-01 — Plano de habilidades e perfis de batalha

18/11/2026. **Planejamento atualizado; implementação ainda não iniciada.** Proposta baseada no relato do usuário e no código atual; nenhuma regra atual completa do Dislyte foi certificada. Os 155 testes existentes não foram repetidos por esta tarefa documental.

## Direção recebida

Aproximar o combate padrão de equipes da referência descrita: até três posições de habilidade por personagem, com ações selecionáveis, passivas e condições próprias. Preservar experiência clássica com ataque/guarda/opções ampliadas para outros perfis. Não apagar a implementação atual do COM-02.

Distinguir três decisões independentes:

- Perfil de escolhas: padrão de equipe versus clássico, com limites e opções configurados.
- Relógio: AP/SPD lógico já implementado; ordem fixa preservada; ATB temporal continua extensão não implementada.
- Controle: manual ou automatizado, utilizando as mesmas validações e habilidades.

Exploração aberta/semiaberta pode usar um perfil adequado ao evento, mas não deve ser acoplada por obrigação ao menu clássico. Nenhum mundo de exploração será construído neste bloco.

## O que já existe e será reutilizado

SkillDefinition declara dono, alvo, efeitos, recarga e relógio. TIME-03/RES-01 implementam recargas, extras, estados e causalidade R04 mínimos. SharedBoss adiciona alcance de alvo direto único. COM-02 oferece comando de habilidade, mas o cenário padrão ainda não equipa skills e usa ataque/guarda como teste de referência.

Ainda não existem: loadout por personagem com posições A1/A2/A3; distinção completa entre habilidade ativa/passiva/híbrida; motor geral de gatilhos; interface padrão de três escolhas; automação estratégica de habilidades; catálogo autoral jogável. Os status Counter/Reflect existentes são mecanismos específicos, não um sistema geral de passivas já completo.

## Modelo de configuração proposto

Três posições no perfil inicial, sem impor três globalmente ao kernel. Uma posição pode conter componente ativo, componente passivo ou ambos. Habilidade ativa pode curar/proteger/controlar, não é necessariamente um ataque. A terceira posição não deve ser codificada como dano máximo universal.

A1 básica deve apontar para a mesma definição/validação ao ser usada por comando, menu ou automação. Não deixar um comando atacar separado contornar condições da habilidade básica. Perfil clássico pode continuar com ataque genérico, conforme configuração própria.

Passivas não viram botões clicáveis. Mostrar descrição e estado quando disponíveis. Passiva sem gatilho realmente implementado não será anunciada como funcional. Passivas podem ter condições, limites de proc, cargas ou recarga interna: cada mecanismo precisa de contrato, não apenas um rótulo.

No perfil padrão proposto, guarda genérica não aparece; só ações do conjunto do personagem e comandos administrativos. Passar é resolução obrigatória de bloqueio quando aplicável, não uma quarta habilidade voluntária. No clássico, guarda permanece opcional conforme regras atuais. Isso organiza o projeto, sem provar equivalência ao Dislyte.

## Entregas pequenas e critérios de aceite

| Parte | Entrega | Aceite e limite |
|---|---|---|
| Preparação — esta entrega | Perfis, sequência, pesquisa e dúvidas | Documento e backlog coerentes; nenhuma mudança funcional |
| 1 | Perfis de escolhas e posições de habilidade | Identidade/loadout/validação explícitos; básica não duplicada; ativa disponível/indisponível distinta de passiva; clássico preservado. Não ativar passiva fictícia |
| 2 | Primeira passiva autoral com gatilho definido | Escolher um gatilho e janela; conferir dono, alvo, limite de proc, morte/revive e R04. Uma família por entrega |
| 3 | Condições de ativação e primeiros kits próprios | Um requisito adicional por vez, reutilizando recarga. Recurso/energia somente se houver necessidade definida; sem catálogo inteiro de efeitos |
| 4 | Seleção manual/automática sobre o mesmo contrato | Automação só escolhe ações válidas; prioridade configurada e fallback; sem afirmar reproduzir a IA do Dislyte. Não alterar AP/SPD por apresentação |
| 5 | Integração no protótipo e regressão | Poucos personagens originais, exemplos ativo/passivo/híbrido conforme suporte implementado; comandos claros, indisponibilidade explicada, clássico acessível e reset/replay. Limites do perfil publicados |

São partes planejadas, não tarefas a executar todas numa entrega. A divisão poderá ser ajustada por acoplamento demonstrado, preservando o objetivo. Não transformar este plano em autorização para downloads, plugins, engine ou arte final.

## Dúvidas por momento — não perguntar tudo de uma vez

### Antes da parte 1

- Fechar quais elementos entram na configuração do perfil e na apresentação: até três posições inicialmente; preservar clássico; sem guarda genérica no padrão. Direção recebida é a base, contrato específico será apresentado com exemplos.
- Como mostrar habilidade passiva e indisponibilidade: informação separada de ação; comandos não devem sugerir uma opção utilizável que não existe.
- Como identificar básica e evitar bypass: A1 como ação real do conjunto, inclusive em automação.

### Antes da primeira passiva

- Qual gatilho autoral escolher primeiro: início da batalha, início/fim do turno, impacto recebido ou outra janela definida?
- Efeito contínuo de atributo ou reação por evento? Requer mecanismos diferentes.
- Quantas vezes pode disparar; de quem é o turno/ação que limita; pode disparar bloqueado, morto ou após revive?
- Como integrar com filas R04 existentes sem reiniciar efeitos nem duplicar consequências?

### Antes de condições/automação

- A ultimate escolhida exige apenas recarga ou também recurso/condição? Não presumir energia universal no modelo de referência.
- Prioridade automática por personagem/modo ou regra inicial simples? Seleção de alvos e fallback precisam de exemplos.
- Quais decisões ficam no modo, no personagem e no efeito? Evitar configuração de interface alterando regra de combate.

### Depois, conforme modo/conteúdo

Fases de chefe, AoE, dispel/imunidade, modificadores completos, crítico/RNG, equipes que retornam, score/recompensas e variações de chefes continuam no catálogo PvE e nas tarefas futuras. Não reabrir o contrato R04 fechado por falta de informação externa sobre outra versão.

## Pesquisa pública realizada nesta entrega

Objetivo limitado: avaliar disponibilidade de documentação pública para orientar o próximo bloco, sem executar cliente, ler arquivos do jogo ou consultar APIs privadas. Consulta web autorizada pelo pedido de informações atuais. Nenhum material externo foi instalado.

| Fonte | Resultado desta consulta | Uso permitido na conclusão |
|---|---|---|
| https://dislyte.farlightgames.com/character.html | Página oficial aberta, mas leitor não expôs fichas de habilidades; apresentou estrutura de busca/filtros | Não fornece especificação atual completa nesta consulta |
| https://dislyte.farlightgames.com/m/ | Página oficial pública; extração não trouxe corpo completo das notícias | Referência de entrada, sem certificar regra de combate |
| https://dislyte.farlightgames.com/detail_06_28.html | Resultado indexado menciona notas históricas 3.2.6 e passivas/gatilhos/recargas; abertura atual retornou estrutura sem o corpo | Pista histórica e continuidade da pesquisa anterior, não confirmação de balanceamento atual |
| https://dislyte.farlightgames.com/detail_12_21.html | Resultado indexado menciona passivas e efeitos de AP; abertura atual retornou estrutura sem o corpo | Pista histórica; não reclassificar como regras atuais verificadas |

A pesquisa anterior TEMPO-PESQUISA.md/fontes.json e os relatos do usuário são reaproveitados com suas limitações. Resultados comunitários apareceram na busca, mas não foram usados como autoridade técnica ou prova de implementação atual. Não extrair regra universal de um exemplo isolado.

Consigo pesquisar fontes e comparar descrições; não posso garantir um catálogo completo, atualizado e observado só com as páginas acessíveis. Para detalhes atuais, fichas de poucos personagens, descrições copiadas pelo usuário, links ou registros observáveis podem resolver lacunas pontuais. Anotar versão/data, habilidade e condição, sem dados de conta. Não é necessário um relatório de todos os personagens para começar a primeira parte.

## Amostra de pesquisa que ajudará quando necessária

| Tipo de exemplo | Informações úteis |
|---|---|
| Três habilidades ativas | Texto de A1/A2/A3, recargas, alvos e condições |
| Uma posição passiva | Texto do gatilho, limites e quando altera o estado |
| Ativa com efeito passivo | Separar o que ocorre ao escolher daquilo que ocorre por evento |
| Ultimate condicionada | Condição/consumo, indisponibilidade e o que a automação faz |

Nomes do Dislyte ficam somente como referência de pesquisa. Conteúdo jogável será original; números e textos externos não serão importados automaticamente.

## Sequência posterior

Após HAB-01 no escopo aprovado: CIC-01 (equipe, resultado/recompensa mínima e save), ENG-01 (escolha/compatibilidade da engine), VIS-01 (arena visual), ART-01/AUD-01 (conteúdo e identidade), EXP-01 (expansões avaliadas). Pesquisa de famílias PvE pode detalhar requisitos durante esse percurso; prioridade muda somente por decisão explícita, não por cada exceção encontrada.

Próxima entrega executável: HAB-01 parte 1, contrato de perfis/posições e primeira implementação delimitada. Nesta entrega só foram atualizados documentos, sem alterar Jogar.cmd ou o comportamento clássico atual. Os 155 testes prévios permanecem como evidência datada, sem nova execução.

## Material recebido após o planejamento — R01

Relatório de habilidades recebido e analisado em REVISAO-HABILIDADES-USUARIO-R01.md. Reforça componentes ativo/passivo/híbrido e a separação entre gatilho, recarga interna, alterações externas e progressão. Afirmações de patch e fichas são recebidas, sem URLs individualizadas verificadas nesta entrega; não promovê-las a confirmação atual automaticamente. AutoRevive/Counter/Shield genéricos não reproduzem kits completos de personagens. Não precisa extrair todo o chat: material suficiente para orientar parte 1, com fontes pontuais somente quando necessárias. Nenhum código/teste alterado nesta revisão.
