# HAB-01 — aceite final do conjunto mínimo autoral

18/11/2026. **✅ Cinco partes concluídas no escopo abaixo. 187/187 testes aprovados.**

## Conjunto jogável

| Modelo | Participantes | Posição 1 | Posição 2 | Posição 3 |
|---|---|---|---|---|
| Ativo | A1/B1 | Básica, sem recarga/requisito | Golpe duplo ativo | Proteção própria |
| Passivo | A2/B2 | Básica, sem recarga/requisito | Preparação: escudo de abertura automático, não selecionável | Proteção própria |
| Híbrido | A3/B3 | Básica, sem recarga/requisito | Golpe duplo preparado: ataque ativo com recarga + escudo passivo de abertura | Proteção de emergência, exige vida <= 50% |

Escudos de abertura: 10 pontos, dois turnos naturais próprios, uma vez por combate. A ativa híbrida não reaplica a passiva nem consome o escudo do dono por ser usada; a duração continua avançando pelos relógios existentes. Proteção ativa substitui o escudo comum segundo a regra já testada, sem empilhamento.

Posições, alvos, recarga, requisito e componentes automáticos são visíveis. As condições não atendidas não gastam ação. O time A é manual; o B usa prioridades [3,2] e básica como alternativa. Clássico preservado em lançador separado, com defesa floor(dano/2). Configurações não são conteúdo definitivo de personagens nem reprodução de outro jogo.

## Aceites das cinco partes

| Parte | Resultado | Evidência principal |
|---|---|---|
| 1 | Perfis e até três posições; básica canônica; clássico preservado | perfis-testes.csv, COMBATE-HAB-01-PARTE-1.json |
| 2 | Primeira passiva funcional de abertura, registro e reset | HAB-01-PASSIVA-ABERTURA-R01.md, aceite parte 2 |
| 3 | Requisito de vida exato, sem custo e independente de recarga | HAB-01-CONDICAO-VIDA-R01.md, aceite parte 3 |
| 4 | Política determinística, alvos e validação comum manual/auto | HAB-01-AUTOMACAO-R01.md, aceite parte 4 |
| 5 | Kits ativo/passivo/híbrido integrados; batalhas completas e reinício | COMBATE-HAB-01-FINAL.json, interacao-testes.csv |

Validação atual: 24 grupos de perfis, 15 de interação e regressões anteriores, total 187. Quatro grupos novos nesta parte verificam kits reais, batalha completa via comandos contra o mesmo perfil, reset após resultado e oito combinações curtas dos kits com relógio fixo/AP e prioridades distintas, com replay/orçamento. Permanecem os 168 cenários/replays, 25 formações AP e 16 combinações automáticas da parte 4. São matrizes de naturezas diferentes, não somar como se fossem um único número de testes.

A configuração da demo tem uma fábrica única, team_reference_setup, reutilizada pela sessão e pelos testes. A comparação entre execução direta e textual verifica o adaptador; os testes específicos anteriores continuam sendo a evidência das regras de dano/tempo/causalidade. Não tratar duas execuções do mesmo kernel como validação independente de fórmulas externas.

Abertura dos dois lançadores confirmada; híbrida aparece como componente passivo automático e escudo real. Condição inicial recusada sem custo, reset reaplica aberturas uma vez e clássico confirma dano 12 sob defesa. Nenhum cliente ou recurso externo acessado.

## Como experimentar

Abra Jogar.cmd. A3 começa agindo. `3 A3` inicialmente é recusado pela vida acima de 50%. `2 B3` usa os dois acertos; B3 chega a 40/80 após seu escudo de abertura e a automação pode usar a proteção de emergência. Consulte `estado` e `ajuda`, reinicie com `reiniciar`. Jogar-Classico.cmd mantém a experiência clássica.

## Limites de fechamento

Este aceite fecha somente o conjunto mínimo autoral e seus contratos. Não há silêncio/supressão, passivas por impacto/morte ou recarga interna, energia/custos, requisitos compostos/de alvo, IA estratégica, botão de automação do time A, inventário, save, UI gráfica ou adaptação dos perfis de habilidade aos coordenadores SEQ/PAR. A básica não ter requisito/recarga não comprova imunidade funcional a um silêncio ainda inexistente.

Não é equivalência ao Dislyte, balanceamento final ou desempenho de servidor/mobile/renderização. Blocos fechados não precisam ser reabertos sem mudança/falha pertinente; novas funcionalidades exigem seus próprios aceites e regressão de fronteiras afetadas.

## Próximo bloco

CIC-01, primeira entrega: contrato de composição/seleção de equipe e dados validados em memória. Recompensa e save versionado virão em incrementos separados. Não começar gravação de saves, engine ou infraestrutura junto deste aceite.
