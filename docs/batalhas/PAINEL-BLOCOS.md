# Painel de blocos — estado após contrato R04

Atualizado em 18/11/2026. Base: TAREFAS.md, ESTADO.md, DECISOES.md e resultados anteriores COMBATE.json. Nenhum teste novo executado para gerar este painel.

## ✅ Encerrados no escopo entregue

**Aviso: não precisam de novos testes nem de revisão agora, enquanto não houver mudança relevante no bloco ou em suas dependências.** Blocos de código continuam elegíveis para regressão quando o motor mudar. Conclusão documental não equivale a funcionalidade implementada.

| Bloco | Entrega encerrada | Evidência/limite |
|---|---|---|
| ✅ FUN-01 | Fundação, escopo e organização do projeto | Base documental; estado/backlog recebem atualizações normais |
| ✅ FUN-02 | Diagnóstico inicial e caminho de compilação | Diagnóstico histórico encerrado; instalação posterior coberta por AMB-01 |
| ✅ AMB-01 | Ambiente C++20/MSVC x64 | Compilação e execução do diagnóstico aprovadas |
| ✅ COM-01 | Combate básico de referência | 13 testes; perfil fixo preservado |
| ✅ COM-01A | Formações e investigação inicial de modos | 168 cenários/replays; não implementa todos os modos pesquisados |
| ✅ TIME-01 | Orçamento local do perfil existente | 11 testes; políticas de modo posteriores são outra entrega |
| ✅ WAVE-01 | 1/2/5 confrontos e limite global consecutivo | 9 testes; não equivale a três times contra chefe persistente |
| ✅ TIME-PESQ | Pesquisa exploratória e experimento de relógios | 11 verificações do experimento; não comprova paridade externa |
| ✅ Contrato R04 | Opção B, FIFO inline e desempate causal | Decisão fechada; sem necessidade de perguntar novamente. Testes de software pertencem a RES-01 |
| ✅ Revisões R01–R03 e prontidão R04 | Recebimento, preservação e comparação dos documentos | Trabalho documental concluído; divergências históricas superadas pelo contrato mais recente |

## ⚠️ Implementado e aprovado, mas precisa de evolução

| Bloco | Estado real | O que requer revisão/teste |
|---|---|---|
| ⚠️ TIME-02 | Implementação entregue com 16 grupos aprovados e 25 formações AP/replays | Regras novas de recarga e fonte de extras exigem TIME-03; novas regras de causalidade/guarda exigem RES-01 |

TIME-02 não está reprovado: os testes confirmaram seu contrato anterior. Os requisitos posteriores mudam esse contrato. Não reabrir a entrega inteira nem refazer pesquisa sem propósito; alterar as partes afetadas e executar regressão pertinente.

## ⚠️ Blocos previstos na estrutura inicial e ainda não concluídos

Classificação pelo roteiro e histórico disponíveis, não por commits históricos (o repositório ainda não tem primeiro commit).

| Bloco | Entrega | Falta |
|---|---|---|
| ⚠️ COM-02 | Entrada de ações e reinício interativo | Implementar e testar; em espera |
| ⚠️ ENG-01 | Escolha e compatibilidade da engine | Avaliar/decidir; integração precisa de teste próprio |
| ⚠️ VIS-01 | Arena visual ligada ao núcleo | Implementar e validar interação/apresentação |
| ⚠️ HAB-01 | Sistema mais amplo de habilidades, recurso e efeitos | Ampliar o mínimo já existente, com aceites próprios |
| ⚠️ CIC-01 | Seleção, recompensas e save versionado | Implementar e testar ciclo/persistência |
| ⚠️ ART-01 | Direção artística e personagem original | Criar e revisar arte/direitos; validar integração depois |
| ⚠️ AUD-01 | Áudio e identidade musical | Criar/selecionar, revisar licença e validar reprodução |
| ⚠️ EXP-01 | Campanha e minijogo rítmico | Avaliar escopo antes de implementar/testar |

Para arte, áudio e escolha de engine, validação não significa necessariamente teste unitário: cada entrega tem seu critério adequado.

## ⚠️ Blocos acrescentados durante o desenvolvimento e ainda não concluídos

| Bloco | Motivo da inclusão | Falta |
|---|---|---|
| ⚠️ TIME-03 | Complementos de recarga/extras após TIME-02 | Implementar e testar origem, flags, recarga no uso e congelamento separado de controle |
| ⚠️ RES-01 | Contrato causal R04 e guarda por habilidade | Implementar multi-hit/reflexão/FIFO/prevenção/revive; testar cinco invariantes e desempate |
| ⚠️ SEQ-01 | Cenário de três times sucessivos | Implementar coordenador e validar estado do chefe, troca e resultado |
| ⚠️ PAR-01 | Três frentes como modo coordenado | Completar resultado global, limites/cancelamento e replay; isolamento básico já foi testado |
| ⚠️ PAR-02 | Chefe/estado compartilhado entre frentes | Implementar modelo configurado e validar coordenação; clones independentes não bastam |

Também surgiram durante o percurso, mas já estão concluídos e aparecem na primeira tabela: COM-01A, TIME-01, WAVE-01, TIME-PESQ. AMB-01 foi a entrega concreta de validação após a instalação manual. TIME-02 foi acrescentado e entregue, mas está destacado na tabela de evolução pelos requisitos posteriores.

## Extensões ainda sem bloco executável completo

⚠️ ATB opcional; backend/multiplayer; distribuição em lojas/launcher; benchmarks gráficos/mobile/servidor. São objetivos ou possibilidades futuras, sem entrega implementada/testada no estado atual. Não contar como blocos concluídos nem como tarefas já completamente especificadas.

## Ordem executável

**TIME-03 → RES-01 → SEQ-01 → PAR-01 → PAR-02.** Depois retomar a interação/engine/visual conforme a decisão de escopo. Uma entrega por vez. Não executar todo o painel como tarefa única.

As dúvidas centrais de causalidade estão fechadas; isso não elimina trabalho futuro de implementação, testes e decisões específicas de famílias ainda não construídas.

## Ordem por área atualizada

Usuário priorizou revisar TIME-02 antes de expandir. Plano detalhado em ../../PLANO-PRIORIDADES.md; fila executável em ../../TAREFAS.md. TIME-03 inicia por recarga, depois extras e controle; após RES-01 seguem modos, interação/habilidades/persistência, engine/visual e identidade/conteúdo. Classificações de conclusão acima permanecem válidas.


## Avanço TIME-03

✅ Parte 1 (recarga) concluída: não precisa de nova revisão/teste sem alteração pertinente. ⚠️ TIME-03 permanece aberto para parte 2 (origem/flags dos extras) e parte 3 (controle/integração). Relatório TIME-03.md; total atual 53 testes aprovados e matrizes preservadas.


✅ TIME-03 parte 2 concluída: origem/flags dos extras, 60 testes totais aprovados. Sem nova revisão/teste enquanto não houver mudança pertinente. ⚠️ TIME-03 aguarda somente parte 3 (controle/congelamento e integração), antes de RES-01.


## Estado final TIME-03

✅ TIME-03 concluído nas três partes, com 67 testes e matrizes aprovados. A revisão prioritária de tempo do TIME-02 está encerrada no escopo definido; não precisa de nova revisão/teste sem mudança relevante. ⚠️ RES-01 é a próxima ampliação, ainda não validada: iniciar por identidade/instância de ação, multi-hit e guarda por habilidade. As indicações anteriores de partes pendentes ficam como histórico.


✅ RES-01 parte 1 concluída (identidade, multi-hit e guarda por habilidade), 76 testes totais aprovados. Sem nova revisão/teste sem alteração pertinente. ⚠️ RES-01 continua aberto nas partes 2 (reflexão/FIFO) e 3 (prevenção/revive/vitória).


✅ RES-01 parte 2 concluída e validada (84 testes totais): reflexão/FIFO e fronteiras testadas. Sem nova revisão/teste sem alteração pertinente. ⚠️ RES-01 aguarda somente parte 3 (prevenção/auto-revive/Victory Lock), antes de SEQ-01.


## Estado final RES-01

✅ RES-01 concluído nas três partes e cinco invariantes R04 validados no perfil mínimo: 95 testes totais aprovados, matrizes preservadas. Não precisa de nova revisão/teste sem mudança relevante. A área de revisão prioritária da base está encerrada no escopo definido. ⚠️ Próximo SEQ-01; depois PAR-01/PAR-02. Famílias de efeitos ainda ausentes continuam no backlog, sem alegação de paridade integral ao Dislyte.


✅ SEQ-01 parte 1 concluída: composição/identidade e coordenador dos três times, 103 testes totais aprovados. Sem nova revisão/teste sem mudança pertinente. ⚠️ SEQ-01 continua aberto para parte 2 (perfis/estados do chefe) e parte 3 (validação integrada).

