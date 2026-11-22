# Terminal, marcador e mídia — correções após crash

22/11/2026. Solicitação: vídeo não assistido a cada compilação nova, regras gerais compartilhadas com o piloto e correção do crash ao entrar na Arena PvP local.

## Crash e placeholders

O stack fornecido pelo usuário aponta std::invalid_argument em marker_edges: os parâmetros estavam fora do domínio finito/positivo/limites aceitos. Sem capturar a execução, não é possível afirmar o valor exato recebido. O marcador usava os bounds do placeholder sem tratamento para bounds vazios/transitórios.

- marker_radius usa fallback de 75 unidades para bounds vazios/negativos/não finitos e limita o tamanho ao domínio válido.
- O adaptador UpdateTargetMarker rejeita posição não finita ou geometria inválida antes de criar atores ou chamar a função estrita do núcleo. Um marcador opcional não deve encerrar a batalha por parâmetros inválidos.
- Recriação remove tags, oculta e desliga colisão dos placeholders antigos antes de Destroy (que pode ser diferido pela engine). A renderização ignora atores inválidos e não pertencentes ao protótipo.
- Esferas novas usam AlwaysSpawn, evitando rejeição por colisão durante a transição, e atualizam os bounds após receber o mesh.

Estas mudanças tratam caminhos compatíveis com o relato. Ausência de esferas e crash precisam de confirmação visual posterior. Não declarar reprodução/causa completa comprovadas.

## Regras gerais do Terminal

team_reference_rules centraliza FunnelV1 e seed local para team_reference_setup e felipe_pilot_setup. Compartilham núcleo de dano, Block/Miss/crítico, mitigação, escudos, estados, recargas, efeitos contínuos e reações conforme contratos já implementados. Não há cópia separada de executores.

Preservadas diferenças de conteúdo: Terminal mantém habilidades genéricas e elenco pequeno; A3 continua proteção própria. Felipe mantém A1/Bleed condicional, A2/Vigília e A3 em área. Não se adicionam passivas, veneno ou sangramento a habilidades genéricas sem definição. Limite de batalha do Terminal continua empate; piloto mantém sua regra de limite. Modo clássico e fixtures fechadas preservam o modelo explícito anterior.

## Vídeo por compilação

A marca de assistido é comparada com a data de gravação do binário do módulo RpgEngineProbe (executável como fallback em ambiente monolítico). Uma nova compilação que produz novo binário invalida a marca antiga; na primeira reprodução volta a exigir segurar 800 ms, com barra. Na mesma compilação, concluir ou pular registra a marca e permite skip simples depois. Abrir novamente o mesmo binário não reinicia a marca. Um build sem recompilação/link não é uma versão nova.

Nenhum save ou flag é apagado. A flag é substituída pelo fluxo normal de conclusão/skip quando o jogo for executado pelo usuário. Saves de combate, preferências e vídeos permanecem separados.

## Reteste

1. Abrir versão recém-compilada: Rever abertura → segurar Pular; observar preenchimento e skip após 0,8 s. Repetir na mesma versão: skip simples permitido.
2. Arena PvP → Testar Felipe (local): conferir três esferas (Felipe e dois ECOs), seleção do alvo e marcador, ações e volta ao hub.
3. Terminal → Abrir batalha: conferir seis esferas e habilidades genéricas sob o funil geral; A3 escudo próprio de 1 ponto na escala atual.
4. Alternar entre os dois cenários, sem misturar saves/progresso. Observar se ocorre novo crash.

Nenhum editor, jogo, Android ou tela foi executado na verificação automatizada. Dados pessoais do stack não são reproduzidos no relatório. Evidências consolidadas em TERMINAL-MARCADOR-MIDIA.json. Percentuais globais mantidos até aceite visual.
Resultado automatizado: regressão geral 245/245, marcador 5/5, piloto 10/10 e alternância 2/2 aprovados. Compilação Win64 Development aprovada (52,14 s); stage66 conferido, sem cook/execução. Avisos conhecidos do toolchain/header da engine permanecem, sem mudança de instalação.
