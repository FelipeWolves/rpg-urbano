# Arquitetura inicial

Atualização TIME-02: BattleRules seleciona perfil fixo/AP; eventos distinguem oportunidade e reação, SkillDefinition declara efeitos/recarga e RuntimeCarry rege passagem nas fases estendidas. Contrato atual em docs/batalhas/TIME-02.md; demais seções iniciais são base histórica quando superadas.

Estado: núcleo C++20 e matriz de formações; TIME-01 adicionou orçamento local de ações e resultado explícito de encerramento. MSVC x64 validado; engine não escolhida. WAVE-01 adicionou Stage com passagem explícita de vida, reset e orçamento global consecutivo (docs/batalhas/WAVE-01.md). Chefes por etapa/compartilhados, entrada do jogador e apresentação gráfica seguem pendentes em PENDENCIAS.md.

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


