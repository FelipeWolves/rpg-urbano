# R03 — Causalidade de vitória e consequências de morte

Recebido em 18/11/2026 como texto colado. Original preservado em complemento-causalidade-recebido-R03.txt. Complementa R01/R02; registra regras fornecidas para nosso projeto, sem declarar que sejam resultados oficiais comprovados do Dislyte.

## As duas definições principais foram esclarecidas

1. Reflexão e efeitos de morte são consequências obrigatórias anteriores à confirmação de vitória. Não são tratados como contra-ataque de sobrevivente.
2. Quando os dois lados ficam sem vivos após essas consequências e auto-revives, vence o lado do autor da ação ofensiva em resolução. Em um contra-ataque, esse autor é quem contra-ataca; não o autor do turno original nem o lado ofensivo fixo do modo.

Essas respostas fecham as duas perguntas principais da revisão R02 como contrato recebido do usuário. Não será necessário repetir a pergunta genérica sobre quem é o atacante.

## Papéis que precisam ficar separados no código

| Papel | Uso |
|---|---|
| Dono da oportunidade | Relógios de turno, limite e contexto original |
| Autor da ação em resolução | Fonte da habilidade/ataque/contra-ataque e lado prioritário no caso 0 × 0 |
| Emissor de consequência | Origem de reflexão ou explosão; não substitui automaticamente o autor da ação que causou a cadeia |
| Lado ofensivo do modo | Prioridade inicial quando AP/SPD empatam, conforme perfil |
| Lado defensivo do modo | Vitória por timeout quando o perfil assim determinar |

O autor de ação deve ser capturado como identidade/lado, não consultado em um ponteiro de personagem que pode morrer durante a resolução. Uma ação auxiliar pode ter autor distinto do dono da oportunidade. O TIME-02 atual ainda não representa todos esses papéis.

## Pipeline recebida

1. **Por acerto:** aplicar dano e mitigação; verificar prevenção de morte; confirmar morte quando cabível; cancelar contra-ataques pendentes da unidade morta. Resolver reflexão, com prevenção de morte do autor. Se o autor morrer, interromper seus acertos restantes.
2. **Consequências de morte:** resolver explosões/efeitos correspondentes antes da vitória; novas mortes recebem tratamento explícito. Não criar contra-ataques recursivos por esses danos.
3. **Auto-revive:** resolver as ressurreições automáticas relevantes antes de consultar a eliminação final.
4. **Vitória:** um lado sem vivos perde; se ambos estiverem sem vivos, vence o lado do autor da ação em resolução. Se o resultado for final, não abrir nova janela de reações.
5. **Sobreviventes:** se os dois lados ainda tiverem vivos, processar assistências/perseguições e depois contra-ataques. Cada ação derivada que causar dano deve passar pelas fronteiras pertinentes antes de continuar a fila.

Modo e timeout continuam fora do desempate por morte dupla. A preferência do defensor em timeout deve ser política do perfil correspondente, não substituição global das opções existentes de empate/derrota A/derrota B em todos os modos.

## Casos de aceite extraídos

| Caso | Resultado esperado do contrato |
|---|---|
| Jogador mata último inimigo e morre na reflexão/explosão, sem revive | Vitória do jogador |
| Inimigo mata último aliado e morre na reflexão/explosão, sem revive | Vitória do inimigo |
| B contra-ataca A; ambos ficam sem vivos pelas consequências | Vitória de B |
| Autor morre por reflexão no acerto k de M | Não executar acertos k+1 a M |
| Prevenção mantém autor vivo após reflexão | Não aplicar interrupção por morte enquanto continuar vivo |
| Último defensor tem auto-revive aplicável | Resolver revive antes de concluir eliminação |
| Unidade morreu com contra-ataque pendente | Cancelar a ação pendente; revive não restaura evento cancelado automaticamente |
| Resultado final confirmado | Não executar assistências/contra-ataques posteriores nem concluir duas vezes |
| Última oportunidade do limite | Resolver consequências permitidas antes de timeout; não iniciar novo turno extra além do teto |

São testes a implementar, não testes executados nesta revisão.

## Detalhes para especificar junto da implementação

O texto contém tanto “Etapa 2 após o loop” quanto “imediatamente após a confirmação de morte”. Em multi-hit com morte intermediária, essas frases não determinam a mesma fronteira. Ao implementar, registrar explicitamente se a fila de consequências de morte é drenada antes do próximo acerto ou somente ao terminar/interromper o loop; criar um caso que diferencie os dois resultados. Isso não reabre quem vence o caso 0 × 0.

Também serão necessários contratos para valor/base da reflexão, se consequências de morte podem gerar novas consequências de morte, e a proteção técnica para cadeias. “Sem contra-ataque recursivo” não define sozinho todos os efeitos encadeados. DoT letal no início de turno, sem uma ação ofensiva ativa, exigirá política própria quando DoTs forem implementados.

Esses detalhes devem ser tratados nos blocos correspondentes; não justificam exigir nova pesquisa extensa antes de aproveitar as definições já recebidas.

## Impacto e sequência

O TIME-02 encerra eliminação diretamente ao causar dano e não possui reflexão, prevenção ou revive. Logo, seus 49 testes anteriores não validam esta pipeline nova. O contrato exige desacoplar “HP chegou a zero” de “batalha terminada” antes de introduzir essas famílias de efeito.

Nenhuma alteração de código nem execução de testes nesta revisão. Recarga e fontes de extras da R02 continuam como mudanças propostas para um próximo bloco restrito. A pipeline recebida será referência para a evolução de morte/revive/reflexão, sem juntar essa expansão inteira ao coordenador de três times.
