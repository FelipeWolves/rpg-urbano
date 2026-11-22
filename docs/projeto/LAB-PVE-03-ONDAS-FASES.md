# LAB-PVE-03 — apresentação de ondas e fases

22/11/2026. Dois testes locais separados no Terminal. Código, testes e compilação; sem execução visual pelo assistente. Aceites de LAB-PVE-02 e desta nova tela continuam separados dos controles já aprovados. Demo ~28% mantida.

## Acessos

- Terminal → Ondas consecutivas — Felipe: duas ondas, primeiro ECO-1 e depois ECO-2. A equipe participa das duas, com política WaveV2 já existente. Objetivo é concluir todas as ondas.
- Terminal → Ritual — fases do chefe: mesmo kit Felipe do Ritual, ECO-1 como chefe e ECO-2 como auxiliar independente. Em90% PV do chefe entra Vigilia-90 sem trocar habilidades; em60% entra Furia-60, preservando ECO-1-A1 e alterando seu power_ratio para15000 (150% ATK antes das demais camadas). São dados provisórios de laboratório, não conteúdo final.

Laboratório de equipe, Ritual simples e piloto na Arena PvP local continuam disponíveis e isolados. Não há novo modo clássico, servidor, multiplayer ou recompensa comercial.

## Ondas — fonte única de regras

WaveVisualSession adapta Stage, sem recalcular cura/limpeza ou inventar relógios. O HUD separa onda atual/total, ações globais e ações locais. Contador local reinicia na transição; global continua e mantém teto200/empate. Somente StageEndReason::Completed é objetivo concluído; limite/falha/técnico não viram vitória.

WaveV2 continua responsável pela limpeza, cura30% dos PV máximos dos aliados vivos, remoção de escudo padrão na fronteira, preservação classificada, recargas/cargas conforme contrato e reinício de AP/relógio local. Não ressuscita mortos. O adaptador apenas encaminha comandos pelos preparadores Team compartilhados antes de Stage.apply.

Catálogos e slots de cada onda são validados com ProfileBattle; aliado mantém o mesmo kit, inimigos/IDs mudam. A revisão de entrada cresce entre ondas e rejeita comandos ou alvos da onda anterior. Placeholders são reconstruídos quando encounter_index muda, reutilizando as correções de mobilidade/geometria. Ações e eventos pertencem à onda que os gerou; transition_events ficam separados para a apresentação da fronteira.

## Fases — dados e feedback

O HUD mostra a fase efetivamente aplicada. Detalhes do encontro lista o último lote de transições em ordem, inclusive quando uma habilidade cruza os dois limiares. Ler a lista não reaplica efeitos nem avança relógio. Nova tentativa limpa a lista e retorna ao estado inicial.

Neste piloto os IDs dos slots do chefe permanecem estáveis; por isso o loadout continua válido ao mudar força. Troca futura de IDs/quantidade de slots exige mapeamento explícito, não é inferida. Fase sem troca de habilidade permanece suportada e coberta por teste.

## Detalhes, resultado e isolamento

Detalhes do encontro fica disponível durante a luta e o botão vira Resultado do encontro ao terminar. Mostra fase ou onda, contadores e eventos de limpeza/cura reportados pelo controlador. É consulta, sem dano/score/recompensa. O painel bloqueia ações enquanto aberto, pelo fluxo já existente.

Pausa, abandono, nova tentativa e retorno ao hub são reutilizados nos dois acessos. Sessões próprias: abandonar uma não altera PV/recargas/saves da outra. Nenhuma nova persistência de runtime foi criada.

## Limites técnicos explícitos

- WaveVisualSession recusa passivas BattleStart, até existir uma inicialização de runtime de abertura no Stage com semântica aprovada; a fixture usa A2 reativa, implementada no núcleo. Isso não remove passivas de abertura do Ritual/laboratório, já suportadas por ProfileBattle.
- Não há animação final de transição ou reprodução visual dos cues; eventos estão observáveis para integração futura. A troca atual é direta, com esferas provisórias.
- Matriz ampla de resolução, material em Shipping/mobile e save real continuam pendentes. Nenhum Android, cook, rede, instalação ou tela nesta entrega.

## Roteiro do usuário

1. Terminal: conferir os quatro testes de batalha e Voltar. Entrar em Ondas consecutivas e observar Onda1/2.
2. Concluir a primeira: deve aparecer ECO-2, Onda2/2 e ações locais reiniciadas; ações globais continuam. Consultar detalhes da transição e efeitos no personagem.
3. Concluir a segunda: mostrar conclusão das ondas; nova tentativa volta à primeira. Testar pausa/abandono e retorno aos outros laboratórios.
4. Ritual com fases: A1 pode disparar a fase sem troca; A3 pode atravessar dois limiares. Conferir fase atual e ordem no painel. O chefe continua usando seu slot básico com a nova força após Furia-60.
5. Confirmar ausência de alvo antigo após a onda e ausência de crash/esferas sumidas. Não precisa repetir todos os aceites anteriores por rotina.

## Verificação e continuidade

Transitions13 cobre contadores, pureza, fronteira/revisão/alvo antigo, conclusão, equivalência de carry com Stage, limite global, abandono/reset, batalha completa, fase sem troca, mudança do kit/automação, reset de feedback, isolamento e catálogo inválido. Regressões WaveV2/Ritual/Phases, build e stage em LAB-PVE-03.json.

Próximo: LAB-PVE-04, ScoreAttack/Survival e score/recibo, com dados provisórios e sem depender de novos recursos de arte. Conteúdo final pode chegar em paralelo. Não iniciar SEQ/PAR antes dos cortes anteriores estarem utilizáveis.
Resultado: Transitions13/13, WaveV2 7/7, Ritual12/12 e Phases11/11 aprovados; build Unreal52,54s e stage68 verificado, sem cook/execução. Avisos conhecidos do toolchain/header da engine permanecem, sem instalação ou alteração de ferramentas.
