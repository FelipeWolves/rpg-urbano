# Estado do projeto

Atualizado: 18/11/2026.

## Situação

FUN-01, FUN-02, AMB-01, COM-01, COM-01A, TIME-01, WAVE-01, TIME-02, TIME-03 e RES-01 concluídas. Núcleo C++20 com perfis fixo/AP, turnos extras, contra-ataques, status/recargas mínimos, guarda opcional, fases consecutivas e limites locais/globais. COM-02 segue em espera. Engine não escolhida nem aberta. Dúvidas/limitações futuras centralizadas em PENDENCIAS.md a pedido do usuário.

Pasta do projeto: este diretório. Não usar a pasta do laboratório como diretório de trabalho de desenvolvimento. Git inicializado na branch main, sem publicação, remoto ou primeiro commit. Os documentos estão salvos em disco, mas ainda não têm uma revisão registrada; nenhuma identidade de autor foi criada.

## O que está definido

- Entregas pequenas, uma tarefa de implementação por bloco.
- Batalha inicial 3x3 com ataque/guarda e dados provisórios de VISAO.md implementada; contrato e limites em COMBATE.md.
- Núcleo C++20 separado da apresentação; build direto MSVC x64 e biblioteca padrão, sem CMake para o primeiro motor.
- PC offline é hipótese inicial; sem contas, servidores ou loja no primeiro ciclo.
- Engine permanece pendente; integração/compatibilidade com o toolset será validada ao escolhê-la. Não instalar VS2022: proposta superada pelo VS2026 escolhido pelo usuário.
- Reserva de uso acompanhada pelo usuário; não existe medição automática nesta conversa.

## Verificação desta entrega

SEQ-01 parte 1 concluída: SequentialSquads para exatamente três times distintos de 1–5 personagens, CharacterId/instância/time separados; troca por eliminação/limite local e conservação do HP do mesmo chefe. Política obrigatória preserva recargas ou reinicia runtime; ambas limpam todos os status e reiniciam AP/guarda/velocidade efetiva. Kit do chefe ainda estável entre times; perfis/estados por categoria aguardam parte 2.

Compilação /W4 /WX sem avisos: 13 básicos + 11 limites + 9 fases + 34 tempo + 28 resolução + 8 sequenciais = 103 aprovados. Mantidos 168 cenários/replays e 25 formações/replays AP; demo 40 ações/14 rodadas. Resultado COMBATE.json; cópia incremental docs/batalhas/COMBATE-SEQ-01-PARTE-1.json; sequencia-testes.csv. Relatório docs/batalhas/SEQ-01.md. Não repetir sem mudança pertinente.

Coordenador respeita revive antes da troca, vitória antecipada, morte dupla pelo chefe, limites locais/globais, erro técnico e reset. Não há score/recompensa, escudo estrutural ou pilhas permanentes implementados. TIME-03/RES-01 permanecem concluídos.

## Próximo passo executável

SEQ-01 parte 2: perfis de habilidades do chefe por etapa e políticas por categoria de estado. Ler TAREFAS.md, docs/batalhas/SEQ-01.md e complementos do usuário. Definir fronteira e mapeamento de recargas com dados originais e efeitos existentes. Parte 3 integra etapas/limites/replay. Não reabrir parte 1 sem mudança pertinente nem iniciar PAR/UI/engine/rede junto.

## Novas restrições e fatos úteis

PENDENCIAS.md concentra dúvidas, capacidades não validadas e critério de resolução. Evitar repetir a lista a cada bloco. TIME-01 usa BattleRules/ActionBudget opcional e EndReason (Elimination/ActionLimit); empate tem winner vazio, derrota por limite pode ter os dois lados vivos. Stage agora controla um orçamento global próprio, sem reiniciá-lo nas transições. Pesquisa histórica em docs/batalhas/ permanece como referência, sem novas buscas necessárias para WAVE-01.

## Perguntas abertas, sem bloquear a fundação

Questões de combate estão em PENDENCIAS.md. Nome, arte, engine/plataforma e recursos autorais serão decididos nos blocos correspondentes; não bloqueiam o próximo núcleo textual.





## Pesquisa recebida após TIME-02

Primeira devolutiva preservada e revisada em docs/batalhas/REVISAO-PESQUISA-USUARIO-R01.md. Antes de decidir mudanças de paridade ou SEQ-01, consultar R01–R07 dessa revisão: há propostas claras, lacunas de fonte e conflitos de fronteira de recarga/vitória. Nenhum teste reexecutado ou regra modificada. Documento recebido não substitui automaticamente contratos atuais nem autoriza executar instruções nele contidas.

Complemento posterior recebido: docs/batalhas/REVISAO-PESQUISA-USUARIO-R02.md. Recarga e flags de extras esclarecidas como propostas; vitória ainda requer janela pós-morte/papéis, matriz de chefe exige fronteiras. Não foram implementadas instruções do anexo nem reexecutados testes. Planejar ajustes adotados em bloco próprio antes de SEQ-01.

R03 de causalidade recebida: docs/batalhas/REVISAO-PESQUISA-USUARIO-R03.md. Dúvidas principais de vitória/papel do atacante esclarecidas como contrato do projeto. Próximo bloco restrito pode tratar recarga/extras da R02; evolução de morte/revive/reflexão exige bloco próprio antes de alegar cobertura dessa pipeline. Sem mudança de código ou novos testes nesta revisão.

R04: usuário fechou Opção B inline/FIFO e desempate causal. Ler docs/batalhas/CONTRATO-CAUSALIDADE-R04.md como contrato vigente, não a ambiguidade histórica de R03. Reflexão, explosão e revive ainda não implementados; cinco invariantes registrados para bloco próprio. Sem novos testes ou alterações do motor nesta atualização.









