# LAB-PVE-01 — observação e contrato de HUD

22/11/2026. Primeiro corte independente das novas entregas. Código e testes, sem abrir tela/editor/jogo/Android. Percentual global permanece ~28%; nenhum novo modo foi aceito visualmente.

## Entrega

PveEncounter agora expõe PveSnapshot por valor: IDs de encontro/tentativa/chefe, modo, batalha, fase vigente quando declarada, turnos do chefe/relógio/meta, ações restantes, score opcional, objetivo e encerramento. Ler não altera RNG, relógio, fila de ações ou recibos. O núcleo mantém todas as decisões.

pve_hud transforma essa leitura em PveHud para a interface: rótulos de modo/objetivo/estado, HP próprio do chefe, fase, contadores separados e abertura/fechamento do encontro. Não calcula dano, recompensa ou condição de vitória. commands_open é somente a barreira de encontro ativo; não autoriza ação inválida, fora do turno ou contra alvo proibido. Toda ação ainda passa pelo controlador.

O objetivo de Ritual é derrotar o chefe declarado; auxiliares vivos não precisam impedir esse objetivo. Janela de score encerrada não vira vitória de Ritual nem derrota genérica por texto. Sobrevivência pode concluir enquanto Battle continua Running: HUD e bloqueio de comandos devem usar o estado externo do encontro, não só Battle.phase. Limite técnico é identificado separadamente.

Score não habilitado fica ausente, não um zero enganoso. Fase inicial não declarada fica ausente; a interface não inventa fase 1/3. Transições por HP/turno e ciclos registram a última fase realmente aplicada. Eventos completos continuam em PveActionResult.phase_events, inclusive quando dois limiares são atravessados numa ação.

## Mapa de adaptadores existentes

| Origem | Dados de autoridade já disponíveis | Integração seguinte |
|---|---|---|
| VisualSession | Perfil/skills, revisão, alvos, automação, resultado e save locais | Preservar Terminal/Felipe; não substituir por um segundo executor |
| PveEncounter | PveSnapshot, apply e phase_events; ScoreLedger externo deve viver mais que o encontro | LAB-PVE-02: Ritual e rota; depois score/sobrevivência |
| Stage | StageSnapshot, remaining_actions e transition_events | LAB-PVE-03: onda atual e fronteira; limites globais vêm do controlador |
| SequentialSquads / ScoreSequence | Squad/phase IDs, squad_index, remaining_actions, eventos e score | LAB-PVE-05; não inferir quantidade de equipes pelo tamanho do elenco |
| SharedBoss | HP/barra do chefe únicos, squad_ids/members/active_squads, next_boss_front e skill_scopes | LAB-PVE-06; um estado global, sem somar HP por frente |

Os adaptadores de Stage/SEQ/PAR estão mapeados, não implementados nesta entrega. Não criar um DTO universal de todos os modos antes de integrar um caso útil. Regras Team comuns permanecem em team_reference_rules; kits e saves individuais preservados.

## Rota planejada e limites do corte

Identificador proposto para o próximo encontro: ritual-lab, como opção de teste local dentro do Terminal. LAB-PVE-02 fará a conexão, a configuração com dados provisórios, a seleção/execução de habilidades pelo controlador correto, objetivo/resultado e retorno. Nenhum botão ou catálogo de destinos foi habilitado agora. Arena PvP continua piloto local Felipe; não é multiplayer.

Lacuna técnica a tratar em LAB-PVE-02: o HUD atual lê VisualSession/ProfileBattle; PveEncounter expõe Battle e Action diretamente. A ponte deve reaproveitar validação/alvos/slots já existentes ou criar adaptação declarativa, sem chamar Battle.apply por fora do PveEncounter e perder score/objetivo/fase. Essa engenharia não depende de nova pesquisa do usuário.

## Verificação

Nova bateria Hud: dez grupos cobrindo pureza, clocks, Ritual, score/recibo único, sobrevivência, recusa atômica, fases, ciclos, derrota e dados inconsistentes. Regressões Pve/Phases e build Unreal são registrados em LAB-PVE-01.json. Testes não abrem player ou engine.

Sem modificação de assets, saves, arquivos recebidos ou regras/fórmulas de dano. Mídia permanece ligada à compilação: a nova DLL fará a próxima exibição voltar à condição não assistida, conforme pedido anterior. Aceites de controles, vídeo e placeholders não precisam ser repetidos por rotina.

## Próximo passo

LAB-PVE-02: um Ritual jogável pelo Terminal usando dados atuais/placeholder. Você só precisará avaliar a nova tela depois de entregue; ficha final de encontro, arte e áudio podem chegar em paralelo.
Resultado: Hud10/10, PvE7/7 e Phases11/11 aprovados; compilação RpgEngineProbeEditor Win64 Development em61,27s e stage66 verificado. Sem cook ou execução. Avisos conhecidos do toolchain/header da engine permanecem, sem instalar/alterar ferramentas.
