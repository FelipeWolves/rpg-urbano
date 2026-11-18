# WAVE-01 — Confrontos consecutivos

Entrega de 18/11/2026. Implementação original em include/rpg/stage.hpp e src/stage.cpp, independente de engine, interface e rede.

## Regras implementadas

- StageDefinition exige ID, versão positiva, time A, lista de 1 a 5 confrontos e política explícita de passagem. Cinco limita este perfil de fase, não o núcleo Battle nem o tamanho dos times.
- Todos os confrontos são validados antes do início: IDs de confronto únicos, inimigos B, atributos válidos, IDs de combatente únicos dentro de cada batalha e limites positivos. O mesmo ID de inimigo pode reaparecer em outro confronto; seu estado é novo. O time A mantém seus IDs entre ondas.
- PreserveHealth conserva vida atual e derrotados. RestoreTeam restaura explicitamente todos à vida máxima, incluindo derrotados. Nenhuma regra altera max_health. Não há restauração percentual neste perfil.
- Guarda é limpa e a ordem fixa por velocidade é recalculada; contagem de rodada e ações locais recomeça. Não existe barra dinâmica/AP. Energia, efeitos persistentes e recargas ainda não existem; sua conservação não foi validada.
- Battle aceita estado inicial completo e validado de vida, separado das definições. Vida deve estar entre zero e o máximo; é necessário ao menos um vivo por lado. Mortos não entram na ordem inicial.
- A transição é automática após vitória de A no confronto. Inimigos seguintes começam com vida máxima. Empate ou derrota local termina a fase. Uma vitória local por política de limite também permite avançar, se o limite global não tiver esgotado.
- O limite global opcional conta ações aceitas de ambos os lados em todos os confrontos, uma vez por ação. Guarda conta; comandos inválidos não contam. O limite efetivo do confronto é o menor entre saldo global e limite local configurado. Em igualdade, a política global prevalece.
- Vitória no último confronto pela última ação permitida conclui a fase. Vitória em confronto intermediário com saldo global zero encerra pelo resultado do limite global e não inicia o próximo. Eliminação real do time A prevalece sobre uma política global favorável a A.
- O resultado da fase distingue Completed, EncounterFailure e GlobalActionLimit. winner pode estar vazio (empate) ou indicar vitória por política de limite; Completed identifica conclusão da sequência.

## Identidade, eventos e reinício

StageSnapshot contém StageId, EncounterId, índice, configVersion, contador global, resultado e snapshot da batalha. É uma visão de diagnóstico em memória, não formato de save nem API de retomada.

StageActionResult identifica a fase e o confronto que processou o comando. Seus eventos continuam pertencendo ao confronto anterior quando a ação inicia o seguinte. encounter_advanced indica passagem; stage_finished é emitido apenas na ação que encerra a fase. Comandos posteriores são rejeitados sem alterar estado nem repetir conclusão. Não há economia/recompensa implementada: o sinal único foi testado, crédito persistente permanece futuro.

reset() reconstrói o estado inicial da mesma configuração: vida, mortos, guarda, contadores, índice e resultado são reiniciados. Não reaplica eventos antigos; não afeta outra Stage. Não implementa carregamento de snapshot intermediário.

## Verificação

Executado scripts/testar-combate.ps1: compilação MSVC /W4 /WX sem avisos; 9/9 testes de fase, 13/13 testes básicos, 11/11 de limite local e 168 cenários/replays aprovados. Demo permanece com 40 ações/14 rodadas. Resultados consolidados em COMBATE.json.

Os nove grupos cobrem configuração e validação antecipada; estado inicial inválido; sequências 1/2/5; preservação de mortos e restauração explícita; limpeza de guarda; derrota intermediária; fronteiras globais 1/2/3/4 e prioridade de eliminação; limites locais/globais e resultados explícitos; invalidez sem consumo, replay, reset durante/após fase e isolamento entre instâncias.

## Próxima entrega

SEQ-01: três times sucessivos e perfis de chefe por etapa. Consultar PEN-04/PEN-06/PEN-10 em ../../PENDENCIAS.md. Este bloco não implementou chefe compartilhado, habilidades, threads, recompensas ou servidor.
