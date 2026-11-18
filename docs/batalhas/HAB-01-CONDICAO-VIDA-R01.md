# HAB-01 parte 3 — condição de vida R01

## Contrato

Primeira condição adicional autoral: uma ativa não básica pode exigir **vida atual do dono menor ou igual a X% da vida máxima**, com X inteiro de 1 a 100. A demo equipa **Proteção de emergência** na posição 3 de A3/B3: vida <= 50%, escudo comum de 10, duração 2, recarga 3. Números são provisórios, sem intenção de reproduzir kit externo.

- A condição é consultada no estado atual, antes de executar a ação; não é desbloqueio permanente. O limiar é inclusivo. Comparação exata: HP * 100 <= MaxHP * X, com intermediários de 64 bits; não arredondar percentuais. Para 101 HP máximos, 51 não atende 50%, 50 atende.
- Não há custo adicional: não remove HP, não gasta energia, não consome cargas e não cria um recurso novo. O estado não ganha contador de condição.
- Ao atender, alvo, efeito e recarga continuam sob o kernel existente. Efeito de escudo ativo mantém a regra de substituição de escudo comum; não implica empilhamento.
- Básica/posição 1 não aceita requisito. Posição exclusivamente passiva também não aceita requisito de seleção. Uma habilidade híbrida pode condicionar a ativa, sem condicionar automaticamente seu componente passivo.
- A mesma checagem vale para comando por posição e por ID. Ataque legado/IA básica continuam mapeados à básica sem requisito. Seleção avançada de especiais pela IA pertence à parte 4.
- Falha por condição retorna SkillConditionNotMet, sem evento, alteração de HP/AP, turno, orçamento, recarga, status ou registro de passivas. Ajuda/estado também não alteram o combate.

## Disponibilidade e precedência

AbilityView guarda requisito atendido independentemente da recarga numérica. A apresentação mostra ambos quando coexistem. Não confundir condição, recarga e impedimento de agir.

Prioridade no caminho do perfil: fim de batalha; identificação de dono/habilidade equipada; validação do ator/vez e incapacidade; recarga; condição; validação de alvo e resolução. Ator fora da vez/morto e ação bloqueada seguem os erros existentes do kernel. Se a condição falhar, o alvo ainda não é avaliado; isso é uma recusa sem custo, não um ataque.

Na apresentação, BATALHA ENCERRADA e SEM AÇÃO prevalecem; depois RECARGA; depois BLOQUEADA por condição; por fim DISPONÍVEL. Requisito e situação atendido/não atendido continuam visíveis em todos esses casos. Sem ícones gráficos nesta interface textual.

## Aceite e limites

Testar rejeição atômica pelos dois comandos, básica/IA disponíveis, limiar exato e fracionário, configuração inválida, recarga e condição coexistentes após auto-revive, incapacidade/vez/alvo, replay/reset/fim e integração textual. Auto-revive não é uma regra nova deste incremento: é usado no teste para elevar novamente a vida e comprovar que o requisito é dinâmico.

Não inclui condições de alvo, energia, consumo, combinação AND/OR de requisitos, supressão/silêncio, IA estratégica, restauração em disco ou adaptação SEQ/PAR. ProfileBattle é o ponto de entrada desse contrato: Battle isolado continua um resolvedor sem loadout/requisitos. Não chamar diretamente o kernel para contornar regras de um perfil.

Próximo incremento: parte 4, política mínima de automação que escolha especiais válidas e use a mesma validação do controle manual. Não alterar AP/SPD para implementar escolha de comandos.
