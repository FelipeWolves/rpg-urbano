# Arquitetura inicial

Atualização TIME-02: BattleRules seleciona perfil fixo/AP; eventos distinguem oportunidade e reação, SkillDefinition declara efeitos/recarga e RuntimeCarry rege passagem nas fases estendidas. Contrato atual em docs/batalhas/TIME-02.md; demais seções iniciais são base histórica quando superadas.

Estado: núcleo C++20 e matriz de formações; TIME-01 adicionou orçamento local de ações e resultado explícito de encerramento. MSVC x64 validado; engine não escolhida. WAVE-01 adicionou Stage com passagem explícita de vida, reset e orçamento global consecutivo (docs/batalhas/WAVE-01.md). Chefes por etapa/compartilhados, entrada do jogador e apresentação gráfica seguem pendentes em docs/projeto/PENDENCIAS.md.

## Módulos previstos

| Módulo | Responsabilidade | Pode depender de |
|---|---|---|
| Regras de combate | Estado, validação, ordem, ações e vitória | Tipos e dados do próprio núcleo |
| Dados de jogo | Definições de personagens e habilidades, validação de versões | IDs e esquemas documentados |
| Aplicação | Fluxo iniciar/jogar/reiniciar e adaptação de entradas | Núcleo e interfaces de apresentação |
| Apresentação Unreal, se escolhida | Câmera, UI, animação, efeitos, áudio | Estado/eventos do núcleo; não decide dano |
| Persistência | Salvar, carregar e migrar versões | DTOs/IDs; sem serializar ponteiros de cena |

Não implementar todos os módulos agora. O primeiro bloco técnico precisa apenas do núcleo, dados de referência e um verificador textual local.

## Núcleo e integração

Decisão de FUN-02: C++20 para regras sem UObject, Actor ou Blueprint, mantendo a integração com Unreal numa camada separada. Caminho mínimo: MSVC x64 e biblioteca padrão, sem sistema de build adicional para o primeiro motor. A integração com a engine continua proposta a validar. Não antecipar DLLs, microserviços, ECS próprio ou arquitetura de plugins para esse núcleo.

Contrato implementado em include/rpg/battle.hpp e src/battle.cpp: definições entram no construtor Battle; apply(Action) valida ator/ação/alvo, altera estado e retorna ActionResult com motivo de rejeição ou eventos. Consultas são constantes e snapshot() copia o estado para inspeção/testes. A apresentação não altera vida diretamente. Comandos rejeitados não mudam estado nem emitem eventos. Falhas de alocação ou concorrência não fazem parte dessa garantia.

IDs de combatente identificam instâncias da batalha, separados de IDs da definição de personagem. Cada combate tem fase (em andamento/finalizado), rodada, sequência de turnos e resultado. Sem relógio real ou chamadas de rede nas regras. RNG será introduzido apenas quando uma regra o exigir, com semente controlada e testes reproduzíveis.

No código atual, Phase significa estado Running/Finished, não etapa de chefe ou fase de campanha. A futura camada Stage distinguirá isso. Proposta de COM-01A: Stage orquestra confrontos/frentes; SquadId não é SideId; BossState autoritativo separado para chefe compartilhado. Battle atual não importa vida atual de outro confronto: reconstruí-lo restaura vida e não é passagem válida de estado. Não converter vida restante em max_health. Detalhes e aceites em docs/batalhas/CONTRATOS.md.

TIME-01: BattleRules contém ActionBudget opcional, máximo positivo e política explícita Draw/DefeatA/DefeatB. Contar comandos aceitos, não eventos; resolver eliminação antes do timeout. Snapshot inclui regras/contador/motivo; BattleFinished inclui EndReason. Finished não garante vencedor: pode ser empate; derrota por limite pode deixar ambos os lados vivos. Orçamento pertence à instância de Battle, sem compartilhamento/global implícito. Detalhes em docs/batalhas/TIME-01.md; coordenação futura em PEN-02.

Ordem fixa por speed atual não implementa barra de ação/AP. Reações, turnos extras, modificações de AP e recargas exigirão decisão de scheduler/ticks; não presumir que cabem apenas em reordenação. Cinco é limite configurável de perfil, não capacidade global do núcleo.

## Evolução e migração

Regras independentes reduzem retrabalho; não eliminam a dependência da engine para cenas, UI, shaders, física, animação e assets. Evitar criar uma abstração genérica de toda engine. Preservar arquivos originais de modelagem, imagens e áudio e registrar a licença de cada recurso externo usado.

Dados e saves terão schemaVersion e IDs estáveis. Definir formato e migração somente no marco de persistência; não adicionar banco ou servidor para um protótipo offline.

## Repositório e arquivos gerados

Documentos e código-fonte ficam no Git local. Build, cache, resultados temporários e saídas da engine não devem entrar no histórico. Arquivos de projeto/configuração e recursos autorais necessários para reproduzir o jogo devem ser versionados quando existirem. Política de Git LFS será avaliada antes de adicionar grandes binários, sem instalação implícita.

## Verificação

O núcleo terá testes de comportamento: ordem/desempates, limites de vida, guarda, invalidez sem mutação, derrotados sem turno e finalização. Acrescentar uma simulação completa repetível. Verificar preservação do protótipo visual somente após essa camada existir.

AMB-01 validou `.\scripts\verificar-ambiente.ps1` para diagnóstico próprio. COM-01 validou `.\scripts\testar-combate.ps1`: núcleo compilado uma vez, testes e demo separados em build/combate/, com /std:c++20 /permissive- /Zc:__cplusplus /EHsc /W4 /WX e /MT. tests/battle_tests.cpp cobre comportamento e replay determinístico; app/simular.cpp apresenta a simulação automática. A demo não é o núcleo e sua primeira guarda roteirizada não altera a política do adversário.

COM-01A adicionou tests/scenario_tests.cpp: 168 cenários/replays, invariantes por ação, isolamento de três contextos em duas escalas intercaladas e lote funcional limitado. Sem thread concorrente ou boss compartilhado. Esses ensaios não dimensionam CPU/GPU de produção ou servidor. Separar limites de segurança do executor de regras de derrota/empate do jogo.



## Organização aprovada — áreas e dependências (18/11/2026)

Diretriz vigente: separar responsabilidades, sem transformar antecipadamente cada área em plugin. Este quadro complementa as seções históricas; engine permanece sem escolha definitiva. SEQ-01/PAR-01 já concluídos; chefe compartilhado permanece em PAR-02.

| Área | Responsabilidade | Fronteira |
|---|---|---|
| Combate e modos | Tempo, ações, efeitos, resultados e coordenadores | Código C++20 próprio, sem dependência de UI, áudio ou engine |
| Dados | Personagens, habilidades, modos e validação de versões | Definições identificadas; não duplicar regras na interface |
| Aplicação | Comandos, fluxo de partida e adaptação à engine escolhida | Traduz entrada em comandos e eventos em apresentação |
| Apresentação | Interface, personagens, animação e efeitos visuais | Consome estado/eventos; não calcula resultado de combate |
| Áudio | Música, sons e sincronização | Consome eventos; lógica rítmica futura precisa de contrato próprio |
| Persistência e diagnóstico | Save, migrações e registros sem credenciais | Usa IDs e formatos versionados; sem ponteiros de cena no save |
| Ferramentas de produção | Editores de dados, importadores e verificadores | Separar ferramentas de editor do código necessário ao jogo distribuído |

Criar pastas/módulos conforme implementação real, sem reorganização física ampla nesta entrega. Bibliotecas externas e plugins, se necessários, ficam atrás de adaptadores para limitar acoplamento; isso reduz custo de troca, sem garantir migração automática.

## Critério para adoção de plugins

Pesquisar quando uma tarefa concreta precisar: problema atendido, alternativa já disponível na engine, procedência, licença/custos, manutenção, versões compatíveis, suporte PC/mobile, dependências, código-fonte quando disponível e caminho de remoção. Validar com um teste pequeno antes de incorporar ao projeto. Downloads, instalações e compras dependem de aprovação específica; nenhuma dependência adotada nesta decisão.

Distinguir plugins fornecidos pela Epic, plugins de terceiros e plugins próprios do projeto. Referências de uma instalação de outro jogo não identificam automaticamente produtos públicos nem autorizam reutilização. Nem tudo na Unreal é plugin: há recursos nativos, módulos de projeto, Blueprints e conteúdo.

Plugins podem fornecer código, conteúdo e ferramentas de editor. Ajudam em partes reutilizáveis; regras, integração, criação de conteúdo, balanceamento e testes continuam trabalho do projeto. Um recurso de editor não necessariamente acompanha ou funciona durante a execução do jogo.

Referências oficiais consultadas para esta explicação:

- https://dev.epicgames.com/documentation/en-us/unreal-engine/plugins-in-unreal-engine
- https://dev.epicgames.com/documentation/en-us/unreal-engine/unreal-engine-modules

Organização documental aprovada; não muda mecânicas nem exige nova bateria de combate. Próximo bloco permanece PAR-02/PEN-05.


## PAR-02 parte 1 — integração autoritativa

HP único/barra global escolhidos pelo usuário. SharedBoss possui um Battle com todos os integrantes e um chefe; times são metadados de inscrição/alvo. Reutiliza AP/SPD e resolvedor sem alteração. Ataques comuns do chefe respeitam rotação entre times vivos; não há três contextos com HP reconciliado depois. Habilidades ainda rejeitadas até seletores e estados compartilhados serem integrados/testados na parte 2. Contrato atual em docs/batalhas/PAR-02.md; 136 testes aprovados.


PAR-02 parte 2 habilita skills via SharedSkillScope (Self/OwnSquad/Boss/RotatingFront), mantendo um Battle e kernel inalterado. Seletores limitam alvo direto; reação/reflexão/explosão mantêm semântica causal e de lado do resolvedor. Dinâmica do chefe rejeitada para não cruzar frente. Snapshot conserva mapeamento; 142 testes aprovados. Contrato PAR-02.md; próxima parte 3 integração final.


PAR-02 parte 3 concluiu integração do perfil HP/AP único/alvo direto único: 17 grupos, 148 testes totais. Não alterou kernel/coordenador. Fechamento não inclui troca de kit por HP/turno, habilidades comuns de área, score ou thread. Famílias futuras registradas em MODOS-PVE.md/TAREFAS.md. Próximo COM-02 utiliza o núcleo existente para entrada textual e reinício.


COM-02: rpg::presentation::run_text_session adapta streams/comandos a Battle e eventos a texto; app/jogar.cpp é a entrada terminal. Sem mutação direta de HP ou duplicação de dano/vitória. Time B usa choose_basic_attack; execução automática limitada na apresentação, sem fabricar resultado. Configuração default 3x3 AP, teto 200, sem menu de coordenadores. 155 testes totais; próximo HAB-01.


## Direção HAB-01 — perfis e habilidades

Separar perfil de escolhas, relógio AP/SPD e controle manual/automático. Perfil inicial de equipe com até três posições por personagem, que podem conter componentes ativos/passivos/híbridos; básica pertence ao conjunto, sem bypass por ataque alternativo. Clássico preserva opções configuradas como guarda. Passivas exigem gatilhos reais antes de anunciadas funcionais. Planejamento em docs/batalhas/HAB-01.md, ainda não implementado; sem vínculo obrigatório entre mundo aberto e perfil clássico.

## Perfis de habilidades — HAB-01 parte 1

ProfileBattle compõe Battle: configuração validada de slots, tradução de ataque básico/IA e bloqueio de skills não equipadas. Battle permanece responsável por tempo, efeitos e dano. Perfis não alteram AP/SPD. Passive_note é somente metadado e nunca dispara efeito. Apresentação textual consulta disponibilidade sem executar ações.

## Passivas de abertura — HAB-01 parte 2

AbilitySlot pode apontar PassiveDefinition validada. ProfileBattle resolve BattleStart na montagem inicial, antes de comandos, e registra PassiveActivation no snapshot. Efeito escudo reutiliza InitialRuntime e o kernel existente. Apresentação mostra ativações e escudo; não dispara efeitos. Somente esse gatilho foi implementado, sem integração de eventos entre hits.
