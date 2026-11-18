# Estado do projeto

Atualizado: 18/11/2026.

## Situação

FUN-01, FUN-02, AMB-01, COM-01, COM-01A, TIME-01, WAVE-01, TIME-02, TIME-03, RES-01, SEQ-01, PAR-01 e perfil PAR-02 concluídos. Núcleo C++20 com perfis fixo/AP, turnos extras, contra-ataques, status/recargas mínimos, guarda opcional, fases consecutivas e limites locais/globais. COM-02 concluído: sessão textual jogável, controle do time A e resposta automática básica do time B. Engine não escolhida nem aberta. Dúvidas/limitações futuras centralizadas em PENDENCIAS.md a pedido do usuário.

Pasta do projeto: este diretório. Não usar a pasta do laboratório como diretório de trabalho de desenvolvimento. Git na branch main, agora com commit inicial do usuário e remoto origin cadastrado. A árvore estava limpa no início desta entrega. Pushes ficam com o usuário; nenhum servidor remoto foi consultado e nenhuma configuração/identidade Git foi alterada.

## O que está definido

- Entregas pequenas, uma tarefa de implementação por bloco.
- Batalha inicial 3x3 com ataque/guarda e dados provisórios de VISAO.md implementada; contrato e limites em COMBATE.md.
- Núcleo C++20 separado da apresentação; build direto MSVC x64 e biblioteca padrão, sem CMake para o primeiro motor.
- PC offline é hipótese inicial; sem contas, servidores ou loja no primeiro ciclo.
- Engine permanece pendente; integração/compatibilidade com o toolset será validada ao escolhê-la. Não instalar VS2022: proposta superada pelo VS2026 escolhido pelo usuário.
- Reserva de uso acompanhada pelo usuário; não existe medição automática nesta conversa.

## Verificação desta entrega

COM-02 concluído: apresentação textual com comandos de ação/alvo, guarda, skill disponível, Skip, ajuda/estado, reinício/saída. Time A do jogador, B automático; usa Battle sem alterar kernel. Cenário padrão 3x3 AP e orçamento de 200 ações/empate, sem habilidades especiais equipadas. Comando de skills testado em configuração própria, sem catálogo/editor. Proteção de apresentação não inventa resultado.

155 testes totais aprovados em MSVC /W4 /WX, sete novos de interação. Matrizes e demo preservadas. Jogar.exe compilado, verificação de saída e roteiro ataque/guarda/reinício/saída aprovados. Evidências: docs/batalhas/COM-02.md, COMBATE-COM-02-FINAL.json e interacao-testes.csv. Entrada recomendada Jogar.cmd na raiz, ou build/combate/jogar.exe diretamente; se binário ausente, preparar compilação conforme COM-02.md. Windows PowerShell comum bloqueou .ps1, sem mudança de política.

Sem menus dos coordenadores SEQ/PAR, UI gráfica, salvamento, engine, rede ou recursos externos. Catálogo PvE futuro preservado; não declarar todos os modos jogáveis pela interface textual.

## Próximo passo executável

HAB-01 parte 1: implementar posições e perfil de equipes conforme contrato fechado docs/batalhas/HAB-01-PERFIL-EQUIPE-R01.md; preservar clássico. Plano atualizado com dúvidas por etapa; pesquisa oficial pública não expôs especificação atual completa. Nenhuma implementação nova nesta entrega. Reutilizar recargas/efeitos; sem engine/UI gráfica/score junto. Pushes com o usuário.

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


## Correção de entrada COM-02

Reproduzido bloqueio de scripts no Windows PowerShell. Adicionado Jogar.cmd, independente de .ps1 e do diretório atual; binário direto e lançador CMD validados com saída zero. Segurança do Windows preservada. Motor inalterado, 155 testes anteriores não repetidos. Próximo HAB-01 permanece.


## Planejamento HAB-01 após observações do usuário

Direção recebida: perfil padrão de equipe com até três posições, componentes ativos/passivos/híbridos e clássico preservado. AP/SPD, menu e manual/automático são eixos separados. Roteiro em HAB-01.md, cinco partes pequenas com preparação documental concluída. Consulta oficial pública limitada, sem certificar catálogo atual. Nenhum código/launcher alterado ou nova execução dos 155 testes.


## Habilidades recebidas — R01

Relatório externo recebido, sem nova pesquisa ampla: revisão em docs/batalhas/REVISAO-HABILIDADES-USUARIO-R01.md. Modelo ativo/passivo/híbrido, recarga interna/protegida, dono de gatilho e progressão precisam de dimensões separadas. Patches/fichas são alegações recebidas não verificadas independentemente. Sem código ou testes novos; 155 prévios preservados. Próximo HAB-01 parte 1, sem exigir extração integral do outro chat.


## Contrato de equipes e defesa

Usuário fechou perfil até 3 posições, básica A1 não silenciável/disponível se apto, passivas informativas e recarga/inatividade/condição distintas. HAB-01-PERFIL-EQUIPE-R01.md; perfil não implementado ainda. Congelamento/petrificação/supressão exigem matriz futura; motor só tem Block e FreezeCooldowns de recarga. Clássico segue disponível. Terminologia alterada para defesa nos executáveis, guardar alias oculto. Recompilação localizada: 7/7 testes textuais e demo aprovados; 155 anteriores não repetidos integralmente. Próximo HAB-01 parte 1.
