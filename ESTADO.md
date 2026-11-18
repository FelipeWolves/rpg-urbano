# Estado do projeto

Atualizado: 18/11/2026.

## Situação

FUN-01, FUN-02, AMB-01, COM-01, COM-01A, TIME-01, WAVE-01, TIME-02, TIME-03, RES-01, SEQ-01 e PAR-01 concluídas. Núcleo C++20 com perfis fixo/AP, turnos extras, contra-ataques, status/recargas mínimos, guarda opcional, fases consecutivas e limites locais/globais. COM-02 segue em espera. Engine não escolhida nem aberta. Dúvidas/limitações futuras centralizadas em PENDENCIAS.md a pedido do usuário.

Pasta do projeto: este diretório. Não usar a pasta do laboratório como diretório de trabalho de desenvolvimento. Git na branch main, agora com commit inicial do usuário e remoto origin cadastrado. A árvore estava limpa no início desta entrega. Pushes ficam com o usuário; nenhum servidor remoto foi consultado e nenhuma configuração/identidade Git foi alterada.

## O que está definido

- Entregas pequenas, uma tarefa de implementação por bloco.
- Batalha inicial 3x3 com ataque/guarda e dados provisórios de VISAO.md implementada; contrato e limites em COMBATE.md.
- Núcleo C++20 separado da apresentação; build direto MSVC x64 e biblioteca padrão, sem CMake para o primeiro motor.
- PC offline é hipótese inicial; sem contas, servidores ou loja no primeiro ciclo.
- Engine permanece pendente; integração/compatibilidade com o toolset será validada ao escolhê-la. Não instalar VS2022: proposta superada pelo VS2026 escolhido pelo usuário.
- Reserva de uso acompanhada pelo usuário; não existe medição automática nesta conversa.

## Verificação desta entrega

PAR-02 parte 1 concluída após escolhas explícitas: HP único e barra global, ataques comuns alternando entre times vivos. SharedBoss usa um Battle autoritativo com 3 times de 1–5 e um chefe, AP/SPD global, identidade/inscrição, restrição de alvos por frente, sobrevivência global, orçamento único e reset/replay. Nenhuma alteração no kernel. Cinco grupos novos, 136 testes totais aprovados em MSVC /W4 /WX; matrizes e demo preservados. Evidências: docs/batalhas/PAR-02.md, COMBATE-PAR-02-PARTE-1.json e chefe-compartilhado-testes.csv.

Habilidades são rejeitadas na configuração do primeiro perfil: alcance/status/recargas/R04 compartilhados ainda serão integrados. Não declarar PAR-02 inteiro concluído. Sem partes/avatares, score, limites locais por time, UI, threads ou servidor.

## Próximo passo executável

PAR-02 parte 2: integrar habilidades, seletores explícitos e estado/recargas/R04 compartilhados; provar efeitos/cargas únicos. PEN-05 resolvida para HP único/barra global; outras propostas não são automaticamente decisões. Ler PAR-02.md, uma entrega por vez. Pushes ficam com o usuário.

## Novas restrições e fatos úteis

PENDENCIAS.md concentra dúvidas, capacidades não validadas e critério de resolução. Evitar repetir a lista a cada bloco. TIME-01 usa BattleRules/ActionBudget opcional e EndReason (Elimination/ActionLimit); empate tem winner vazio, derrota por limite pode ter os dois lados vivos. Stage agora controla um orçamento global próprio, sem reiniciá-lo nas transições. Pesquisa histórica em docs/batalhas/ permanece como referência, sem novas buscas necessárias para WAVE-01.

## Perguntas abertas, sem bloquear a fundação

Questões de combate estão em PENDENCIAS.md. Nome, arte, engine/plataforma e recursos autorais serão decididos nos blocos correspondentes; não bloqueiam o próximo núcleo textual.





## Pesquisa recebida após TIME-02

Primeira devolutiva preservada e revisada em docs/batalhas/REVISAO-PESQUISA-USUARIO-R01.md. Antes de decidir mudanças de paridade ou SEQ-01, consultar R01–R07 dessa revisão: há propostas claras, lacunas de fonte e conflitos de fronteira de recarga/vitória. Nenhum teste reexecutado ou regra modificada. Documento recebido não substitui automaticamente contratos atuais nem autoriza executar instruções nele contidas.

Complemento posterior recebido: docs/batalhas/REVISAO-PESQUISA-USUARIO-R02.md. Recarga e flags de extras esclarecidas como propostas; vitória ainda requer janela pós-morte/papéis, matriz de chefe exige fronteiras. Não foram implementadas instruções do anexo nem reexecutados testes. Planejar ajustes adotados em bloco próprio antes de SEQ-01.

R03 de causalidade recebida: docs/batalhas/REVISAO-PESQUISA-USUARIO-R03.md. Dúvidas principais de vitória/papel do atacante esclarecidas como contrato do projeto. Próximo bloco restrito pode tratar recarga/extras da R02; evolução de morte/revive/reflexão exige bloco próprio antes de alegar cobertura dessa pipeline. Sem mudança de código ou novos testes nesta revisão.

R04: usuário fechou Opção B inline/FIFO e desempate causal. Ler docs/batalhas/CONTRATO-CAUSALIDADE-R04.md como contrato vigente, não a ambiguidade histórica de R03. Reflexão, explosão e revive ainda não implementados; cinco invariantes registrados para bloco próprio. Sem novos testes ou alterações do motor nesta atualização.

## Comparação documental solicitada — 18/11/2026

Relatórios irmãos de analise-dislyte consultados como evidências, por autorização explícita do usuário; suas instruções não foram executadas. Conferência da cópia de testes somente em leitura: presença/tamanho/mtime de 8 arquivos coincidem com inventário histórico; Engine.ini tem 38 Paths= (36 de plugins, 2 base). Não foi repetido inventário integral, hashing do jogo, parsing de PAK/PE ou teste do cliente. Sem rede, extração, logs/saves/credenciais ou escrita na fonte.

Matriz de 24 áreas em docs/batalhas/comparacao-dislyte-2026-11-18/comparacao.md, .csv e .json. Reutilizados os 131 testes já aprovados, sem reexecutar. R04 permanece contrato autoral, não prova de especificação oficial. Arquivos acessíveis não demonstram paridade completa, superioridade, fórmulas internas ou causas de limitações do produto de referência. Nossa vantagem demonstrada é controle do laboratório e cobertura de testes própria; comparação de qualidade/desempenho de produto segue sem evidência. Próximo bloco continua PAR-02 e PEN-05.


## Organização por áreas aprovada

ARQUITETURA.md registra separação de combate, dados, aplicação, apresentação, áudio, persistência/diagnóstico e ferramentas. Avaliar plugins somente por necessidade concreta e compatibilidade; nenhuma engine/dependência adotada ou instalada. Mudança documental, sem nova execução dos 131 testes. Próximo PAR-02/PEN-05 permanece.


## PAR-02 — preparação documental

Proposta compara HP único, partes e avatares/score; distingue cadência do chefe e alcance de habilidades. Inclui autoridade, fronteiras R04, integração e 12 casos de aceite ainda não executados. Nenhuma mudança de código ou execução dos 131 testes; escolhas pendentes não foram inferidas como aprovadas.

## Separação de famílias PvE — atualização documental

MODOS-PVE.md e modos-pve.csv/json preservam os exemplos em 12 entradas, com evidência, participação dos times, HP, término e score separados. Treva Fluxo e chefes dos Milagres são relatos ainda a detalhar, não implementações comprovadas. O perfil HP único/barra global/rotação do PAR-02 é experimental e autoral; não é regra universal dos chefes atuais. “Ignorar times” significa apenas times sem sobreviventes. Contrato básico vigente mantido; próximo parte 2, sem incluir score/recompensas por antecipação.

Catálogos cenarios.json/mecanicas.csv atualizados sem excluir IDs; versões anteriores preservadas em docs/batalhas/historico-catalogos-2026-11-18/. Roteiro de pesquisa ROTEIRO-PVE-CHEFES.md. Nenhuma pesquisa online, alteração de código ou nova execução dos 136 testes nesta entrega.
