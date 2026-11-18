# Prioridades por área

## CIC-01 parte 1 — estado vigente (18/11/2026)

✅ Composição/seleção de equipe em memória concluída: 1–5 IDs disponíveis e distintos, recusa sem mutação, encontros com dependências filtradas e isolamento de batalhas já iniciadas. Demo aceita Jogar.cmd --equipe A1 A3; catálogo atual A1/A2/A3, adversários B1/B2/B3 fixos. Reinício preserva a seleção. **195/195 testes aprovados**, incluindo sete grupos de seleção e uma integração textual adicional.

Contrato/relatório: docs/projeto/CIC-01.md. Aceite: docs/batalhas/COMBATE-CIC-01-PARTE-1.json; matriz equipes-testes.csv. ⚠️ CIC-01 permanece aberto. Próximo: parte 2, fechar identidade do resultado e recompensa mínima em memória com concessão única; save versionado somente na parte 3. Nenhum save ou recurso externo nesta entrega.

## HAB-01 — concluído no escopo mínimo (18/11/2026)

✅ Cinco partes entregues: perfis/posições, passiva de abertura, condição adicional, automação e integração de kits ativo/passivo/híbrido. **187/187 testes aprovados**; lançadores e clássico conferidos. Aceite consolidado: docs/batalhas/HAB-01-FINAL.md e COMBATE-HAB-01-FINAL.json. Não há revisão pendente desse escopo sem mudança/falha pertinente.

⚠️ Silêncio, passivas durante combate, custos/condições compostas e adaptação SEQ/PAR continuam extensões futuras. Próximo: CIC-01, primeira parte, contrato e composição de equipe em memória; recompensa e save em incrementos posteriores.

## HAB-01 parte 4 — histórico da entrega (18/11/2026)

✅ Automação configurável por prioridade de especiais com básica como fallback, alvos determinísticos e execução pela mesma validação manual. Time B de equipes integrado; clássico preservado. 183/183 testes aprovados, incluindo 16 combinações automáticas curtas com replay/orçamento. Contrato docs/batalhas/HAB-01-AUTOMACAO-R01.md; relatório HAB-01.md; aceite COMBATE-HAB-01-PARTE-4.json.

⚠️ Sem IA estratégica, toggle do time A ou editor de prioridade. HAB-01 continua aberto. Próximo: parte 5, integração/regressão do pequeno conjunto autoral e consolidação dos limites; sem novas famílias ou engine nesta etapa.

## HAB-01 parte 3 — histórico da entrega (18/11/2026)

✅ Condição adicional de vida em ativa não básica, validada por posição e ID. Proteção de emergência A3/B3 exige <= 50% de vida; básica preservada. Motivos de condição, recarga e incapacidade separados. 176/176 testes aprovados; contratos/relatório em docs/batalhas/HAB-01-CONDICAO-VIDA-R01.md e HAB-01.md; aceite COMBATE-HAB-01-PARTE-3.json.

⚠️ HAB-01 continua aberto; sem energia, silêncio ou condições compostas. Próximo: parte 4, política mínima de automação com prioridades, especiais válidas e fallback sob a mesma validação manual.

## HAB-01 parte 2 — histórico da entrega (18/11/2026)

✅ Primeira passiva funcional: Preparação aplica escudo na abertura, uma vez por combate, sem consumir ação; A2/B2 da demo já a utilizam. Posição passiva continua não selecionável, e o efeito/registro são verificáveis. Contrato: docs/batalhas/HAB-01-PASSIVA-ABERTURA-R01.md; relatório: docs/batalhas/HAB-01.md.

⚠️ Gatilhos durante combate, supressão e recarga interna não implementados. HAB-01 continua aberto. Próximo: parte 3, fechar a primeira condição adicional de habilidade, com motivo de bloqueio separado de recarga e básica preservada.

Decisão do usuário em 18/11/2026: primeiro revisar o que já foi implementado e recebeu requisitos novos; depois ordenar por criticidade e, respeitadas as dependências, pelo menor esforço para entregar valor verificável.

Esforço é relativo (baixo, médio, alto), não prazo nem previsão de consumo de Codex. Uma entrega por vez. Alterações de requisito ou resultados de testes podem justificar reordenação documentada.

## 1. Revisão da base implementada — prioridade imediata

| Ordem | Bloco | Esforço relativo | Entrega |
|---|---|---|---|
| 1 | ✅ TIME-03 concluído | Médio, divisível | Revisar os pontos afetados de TIME-02: recarga, origem de extras e congelamento de recarga |
| 2 | ✅ RES-01 concluído | Alto, divisível | Aplicar R04 e guarda por habilidade, ampliando a resolução que hoje encerra morte imediatamente |

TIME-03 é a revisão inicial do TIME-02, não um segundo sistema concorrente. Não refazer ambiente, formações ou pesquisa já encerrados. RES-01 traz famílias novas necessárias ao contrato recebido; não é simples correção documental.

### Ordem interna de TIME-03

1. **Recarga:** ajustar o encerramento do uso e validar N = 0/1/3, sem extras novos. É o trecho menor e tem exemplo de aceite fechado.
2. **Origem dos extras:** acrescentar metadados/flags independentes de avanço de recarga e duração; verificar turnos de equipamento/habilidade, filas, limites e replay.
3. **Controle e integração:** separar bloqueio de ação de congelamento do decremento, verificar preservação/reset entre confrontos e regressão pertinente.

Essas são entregas internas do mesmo bloco, não autorização para acrescentar catálogo de equipamentos ou passivas. Cada entrega deve parar em estado compilável/testado e registrar o próximo passo; não marcar TIME-03 concluído antes de cumprir os três aceites.

### Ordem interna de RES-01

1. Preparar identidade de ação/acertos e guarda por instância de habilidade, com comportamento básico preservado.
2. Introduzir confirmação de morte, reflexão e pendingDeaths FIFO entre acertos; testar interrupção, bounce e cadeias.
3. Integrar prevenção/auto-revive, Victory Lock e desempate pelo autor; verificar cinco invariantes R04, reações, timeout e conclusão única.

Não misturar implementação de coordenadores de vários times com essa mudança de resolução.

## 2. Modos e coordenação de combate — criticidade alta

| Ordem | Bloco | Esforço relativo | Por que nesta posição |
|---|---|---|---|
| 3 | ✅ SEQ-01 concluído | Médio/alto | Sequência de três times aproveita a base atual e antecede coordenação entre frentes |
| 4 | ✅ PAR-01 concluído | Médio/alto | Aproveita o isolamento já testado; fecha resultado, orçamento e cancelamento de frentes independentes |
| 5 | ✅ PAR-02 perfil concluído | Alto | Compartilha estado/efeitos e exige a coordenação anterior; maior risco de duplicidade e ordem incorreta |

Usar habilidades originais mínimas já disponíveis para validar cada modo. Não esperar um catálogo completo. Incluir apenas os atributos/contadores de score necessários aos aceites de cada bloco, com tipos/limites explícitos; economia/recompensa persistente pertence a CIC-01.

## 3. Uso do protótipo, habilidades ampliadas e persistência — criticidade intermediária

| Ordem | Bloco | Esforço relativo | Primeira entrega menor |
|---|---|---|---|
| 6 | ✅ COM-02 concluído | Baixo/médio | Escolher ação/alvo e reiniciar no protótipo textual, reutilizando regras do motor |
| 7 | HAB-01 | Médio/alto, variável | Perfis padrão/clássico e posições; depois passivas, condições e automação em partes |
| 8 | CIC-01 | Médio/alto | Separar composição/seleção, conclusão/recompensa e save versionado em entregas incrementais |

COM-02 sai da espera quando a área 2 estiver entregue; não pressupõe interface gráfica. HAB-01 amplia o mínimo existente, sem reimplementar o que TIME/RES já forneceram. CIC-01 deve validar dados em memória antes de gravar saves e não tratar log de comandos como snapshot completo.

## 4. Engine e apresentação funcional — criticidade posterior à base

| Ordem | Bloco | Esforço relativo | Entrega |
|---|---|---|---|
| 9 | ENG-01 | Médio, com incerteza externa | Escolha/compatibilidade da engine e pequeno teste de integração antes de comprometer o projeto |
| 10 | VIS-01 | Alto, variável | Uma arena com formas provisórias, ação/alvo, estados, resultado e reinício |

A base continua independente de engine. Downloads/instalações e escolha definitiva mantêm aprovação específica. Não medir capacidade gráfica/mobile pelo tempo de simulação sem render. Corrigir apenas incompatibilidades demonstradas pelo teste de integração.

## 5. Identidade e conteúdo — prioridade posterior no protótipo atual

| Ordem | Bloco | Esforço relativo | Entrega inicial limitada |
|---|---|---|---|
| 11 | ART-01 | Médio/alto | Direção visual e um personagem representativo original |
| 12 | AUD-01 | Médio/alto | Conjunto pequeno de áudio próprio/licenciado e integração funcional |
| 13 | EXP-01 | Alto, escopo ainda aberto | Uma fase ou um trecho rítmico demonstrável, antes de campanha completa |

Ordem ART/AUD é operacional e pode mudar se um teste visual ou rítmico exigir áudio antes. Recursos provisórios suficientes para testes podem ser usados anteriormente sem antecipar produção final. Música é central à identidade desejada, mas produção completa não bloqueia o núcleo atual.

## Fora da fila executável atual

ATB opcional, backend/multiplayer, publicação em lojas/launcher e produção multiplataforma completa continuam como extensões futuras, a decompor quando necessárias. Não instalar ferramentas, iniciar rede ou criar infraestrutura por antecipação.

## Próxima ação concreta

CIC-01 parte 2: contrato de resultado/recompensa mínima em memória com concessão única. Composição de equipe concluída (parte 1); não iniciar save/engine junto.

## HAB-01 parte 1 — histórico da entrega (18/11/2026)

✅ Perfis equipe/clássico, posições 1–3, básica canônica sem recarga, defesa restrita ao clássico e metadados passivos não selecionáveis implementados. 162/162 testes; dois lançadores conferidos. Este estado substitui indicações históricas de perfil ainda não implementado.

⚠️ HAB-01 continua aberto: passivas funcionais, silêncio e condições especiais não implementados. Próximo: parte 2, contrato do primeiro gatilho passivo autoral e implementação delimitada. Relatório completo e limites: docs/batalhas/HAB-01.md; evidência COMBATE-HAB-01-PARTE-1.json. Jogar.cmd abre equipes; Jogar-Classico.cmd preserva o clássico.
