# CIC-01 parte 2 — resultado e recompensa em memória R01

## Regra provisória do laboratório

Vitória do time A por Elimination concede **10 créditos de teste**. Derrota, empate, ActionLimit (mesmo com vencedor A) e TechnicalLimit concedem zero. Não é economia final, item, experiência ou moeda comercial; não há loja nem consumo nesta entrega. Time A é o jogador nos dois perfis textuais.

## Identidade e autoridade local

LocalSession é dona do ProfileBattle e do LocalProgress. Cada tentativa tem ID sequencial, começando em 1 e avançando ao reiniciar, mesmo quando a tentativa anterior foi abandonada. O ID só é único dentro dessa instância de sessão; sessões novas podem ter o mesmo número e não compartilham saldo.

A camada não recebe resultados ou IDs de concessão fornecidos pelo chamador. Apenas comandos são executados; settle consulta o Battle interno terminado e registra LocalCompletion com tentativa, vencedor, motivo, ações aceitas e créditos. Depois de um comando aceito, a conclusão é apurada automaticamente. A consulta pública de progresso é somente leitura; cópias podem ser inspecionadas sem editar a fonte.

- Antes do fim: settle retorna falso e não registra nada.
- Primeira conclusão: adiciona um único registro e o valor elegível ao saldo.
- Mesma tentativa já registrada: settle retorna falso. Comandos após o fim são recusados e não pagam novamente.
- Resultado zero também é registrado, impedindo repetir sua apuração.
- Reiniciar preserva registros/saldo e abre outra tentativa com runtime novo e mesma configuração/equipe.
- Reiniciar ou sair durante uma batalha não concluída não gera resultado nem crédito. IDs podem ter lacunas nos registros.
- Vitória causal R04 é respeitada como o kernel a resolveu, inclusive morte dos dois lados; não recalcular resultado pelos sobreviventes na camada de recompensa.

Registros são inseridos antes do incremento de saldo, com verificação de overflow de uint64. Falha de alocação ou limite numérico não converte a falha em recompensa. Um erro de apuração deixa o combate concluído disponível para nova tentativa de apuração; não há rollback do golpe final. Isso não é transação de disco nem garantia contra encerramento do processo.

## Integração textual

Estado mostra tentativa e saldo local. Uma nova conclusão imprime o crédito uma vez; estado/ajuda não concedem nem repetem a mensagem. TextReport contém progresso em memória junto do estado final do combate. A salvaguarda de apresentação continua independente do resultado: batalha ainda Running não é concluída por um encerramento da interface.

Composição de equipe, perfis, habilidades, AP/SPD, dano, R04 e modos do kernel são preservados. LocalSession expõe apenas visão const do perfil e os comandos; chamadas externas não obtêm acesso mutável ao Battle interno.

## Limites

Saldo e histórico somem ao encerrar a sessão/programa. Não há importação de progresso, save, conta, autenticação, antifraude, autoridade de servidor, persistência de IDs, recompensa entre processos ou adaptação dos coordenadores SEQ/PAR. Não usar esse saldo provisório como economia de produção. Cópias/instâncias independentes representam estados independentes, não uma carteira compartilhada.

## Aceite

Sete grupos: vitória e reapuração, derrota/empate/limites, interrupção técnica, reinício/abandono/IDs, ação inválida e batalha em andamento, paridade com perfil e vitória causal, replay/isolamento. Integração textual: duas vitórias, consultas e comando pós-fim, reinício preservando saldo, mensagem única por conclusão e EOF sem recompensa.

Próximo: CIC-01 parte 3. Antes de gravar saves, definir esquema/versionamento, escopo do dado persistido, validação/restauração e forma de evitar nova concessão ao carregar uma conclusão já contabilizada. Snapshot de batalha não é automaticamente um save completo.
