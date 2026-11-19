# Estado do projeto

## ENG-01 — etapa da ponte Windows concluída (18/11/2026)

✅ Compilação e execução dentro da UE5.8.4 aprovadas. Mesma bateria standalone passou11 checkpoints em dois processos Unreal, saída0 e um marcador por execução. Commandlet própria, sem interface gráfica/NullRHI; nenhum slot do jogador usado.213 testes gerais anteriores preservados, sem somar checkpoints como novos testes gerais.

Relatório docs/projeto/ENG-01-RUNTIME.md e ENG-01-RUNTIME.json. Tentativa inicial de editor geral teve timeout e foi substituída por commandlet; registro preservado. ✅ Não há revisão pendente da ponte nesse escopo sem mudança/falha. ⚠️ ENG-01 gráfico/empacotamento/mobile seguem pendentes; esta entrega não começa arena visual. Próximo incremento: abertura gráfica controlada e definição da integração visual em escopo próprio.

## ENG-01 — ponte compilada, execução UE pendente (18/11/2026)

✅ Cinco fontes canônicos do núcleo e ponte compilados/vinculados na UE5.8.4, saída0. Baseline da mesma função executado fora da engine:11 checkpoints aprovados,1 ação aceita e10 créditos. Manifesto confere16 arquivos fonte/stage; nenhum fonte nativo alterado e nenhum editor aberto.

Relatório docs/projeto/ENG-01-PONTE.md e ENG-01-BUILD.json; build mínimo anterior preservado em ENG-01-BUILD-MINIMO.json.213 testes gerais existentes não repetidos. ⚠️ Bateria ainda não executada dentro da Unreal. Próximo: execução controlada autorizada do opt-in e comparação exata com baseline. ENG-01 aberto; sem arena/pacote/mobile.

## ENG-01 — build mínimo aprovado (18/11/2026)

✅ RpgEngineProbeEditor compilado/vinculado em UE5.8.4 com MSVC14.52.37029 e SDK26100.0. Saída0 e DLL própria gerada; aviso de compilador não preferido preservado. Fonte versionada em experiments/unreal, cópia/resultados em build/engine-eval. Editor não aberto.

Relatório docs/projeto/ENG-01-BUILD.md; JSON ENG-01-BUILD.json.213 testes standalone anteriores preservados, sem repetição. ⚠️ Ainda sem ponte do combate, execução do módulo, renderização ou pacote. Próximo: ponte mínima do núcleo e sua compilação; execução/editor em etapa específica. ENG-01 aberto.

## ENG-01 — instalação confirmada, integração pendente (18/11/2026)

✅ Metadados confirmam UE5.8.4 em D:\Unreal Engine\UE_5.8, build58902052, editor/UBT presentes e runtime.NET10 empacotado. MSVC14.52 fora das faixas proibidas, mas não preferido; SDK26100 no intervalo local declarado. Compatibilidade real ainda exige build.

Relatório docs/projeto/ENG-01-INSTALACAO.md; JSON ENG-01-INSTALACAO.json. Instalação somente lida, sem executar componentes ou alterá-la.213 testes prévios reaproveitados. Próximo: projeto C++ de avaliação separado e compilação UBT em etapa específica; editor ainda fechado. ENG-01 aberto.

## ENG-01 — preparação sem editor concluída (18/11/2026)

✅ Codec do save separado do armazenamento Windows, formato v1 preservado e números independentes de locale. **213/213 testes aprovados**; dois novos grupos verificam formato fixo e locale em executável sem o adaptador de armazenamento. Relatório docs/projeto/ENG-01-PREPARACAO-SEM-EDITOR.md, aceite COMBATE-ENG-01-PREPARACAO.json e codec-testes.csv.

Usuário instala manualmente UE5.8.4. Nenhuma engine/launcher executado, novo SDK/plugin instalado ou slot do jogador alterado. ⚠️ ENG-01 segue aberto; não houve compilação Unreal ou validação mobile. Próximo: conferir versão/pasta ao terminar a instalação, depois experimento isolado autorizado.

## ENG-01 parte 1 — avaliação concluída (18/11/2026)

✅ Inventário seletivo e avaliação documental em docs/projeto/ENG-01.md; matriz CSV/JSON em docs/relatorios/atuais. Usuário confirmou editor não instalado. Candidata para decisão: UE5.8.4 Windows; 5.9 é Preview na consulta. VS2026/SDK têm respaldo documental, mas MSVC14.52, UBT e integração ainda precisam de teste real.

⚠️ ENG-01 segue aberto. Nenhum editor/launcher aberto, download, instalação, projeto Unreal ou alteração do núcleo. Próximo: decisão da versão e instalação em destino aprovado; depois requisitos locais e teste mínimo separado. Os 211 testes anteriores foram reaproveitados, sem repetição documental.

## CIC-01 — concluído no escopo mínimo (18/11/2026)

✅ Três partes concluídas: seleção 1–5 em memória, recompensa por conclusão única e save v1 de equipe/progresso entre batalhas. Salvar é manual; continuar valida arquivo/catálogo e abre outra tentativa sem reapurar conclusões antigas. **211/211 testes aprovados**, incluindo 7 de persistência e 18 textuais. Executável validado em cópia isolada, sem tocar slots de jogador.

Contratos: docs/projeto/CIC-01.md e CIC-01-SAVE-R01.md. Aceites: COMBATE-CIC-01-FINAL.json, SAVE-CLI-CIC-01.json e saves-testes.csv em docs/batalhas. ⚠️ Sem save de batalha em andamento, antifraude, concorrência de escritores ou migração automática. Próximo: ENG-01, avaliação de engine/compatibilidade; nenhuma escolha definitiva, instalação ou download autorizado implicitamente.

## CIC-01 parte 2 — histórico da entrega (18/11/2026)

✅ Resultado e recompensa mínima em memória concluídos. LocalSession atribui tentativa sequencial e registra cada conclusão uma vez. Vitória A por eliminação: +10 créditos de teste; demais desfechos: zero. Reiniciar mantém saldo/registro, abre nova tentativa e conserva a equipe. Estado mostra saldo sem reapurar recompensa. **203/203 testes aprovados** (7 de recompensa e 17 textuais, mais regressões).

Contrato: docs/projeto/CIC-01-RECOMPENSA-R01.md. Aceite: docs/batalhas/COMBATE-CIC-01-PARTE-2.json e recompensas-testes.csv. ⚠️ CIC-01 segue aberto: saldo/IDs são locais à sessão, não duráveis. Próximo parte 3: contrato de save versionado, validação/restauração e prevenção de nova concessão após carregar; nenhum save escrito nesta parte.

## CIC-01 parte 1 — histórico da entrega (18/11/2026)

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

## Organização do repositório — 18/11/2026

✅ Organização documental e colaboração concluídas: documentos de planejamento em docs/projeto; resultados vigentes em docs/relatorios/atuais; estrutura C++ preservada. README/CONTRIBUTING/SECURITY, EditorConfig, atributos Git e modelos de issue/PR adicionados. Histórico de orientações preservado; AGENTS atualizado para o estado vigente.

Scripts ajustados aos novos caminhos. Não foram instaladas dependências, adotada engine, definida licença pública ou configurada execução automática em serviços externos. Fontes/recursos de terceiros continuam sujeitos a autorização própria. Próximo bloco funcional permanece HAB-01 parte 2.

Atualizado: 18/11/2026.

## Situação

FUN-01, FUN-02, AMB-01, COM-01, COM-01A, TIME-01, WAVE-01, TIME-02, TIME-03, RES-01, SEQ-01, PAR-01 e perfil PAR-02 concluídos. Núcleo C++20 com perfis fixo/AP, turnos extras, contra-ataques, status/recargas mínimos, guarda opcional, fases consecutivas e limites locais/globais. COM-02 concluído: sessão textual jogável, controle do time A e resposta automática básica do time B. Engine não escolhida nem aberta. Dúvidas/limitações futuras centralizadas em docs/projeto/PENDENCIAS.md a pedido do usuário.

Pasta do projeto: este diretório. Não usar a pasta do laboratório como diretório de trabalho de desenvolvimento. Git na branch main, agora com commit inicial do usuário e remoto origin cadastrado. A árvore estava limpa no início desta entrega. Pushes ficam com o usuário; nenhum servidor remoto foi consultado e nenhuma configuração/identidade Git foi alterada.

## O que está definido

- Entregas pequenas, uma tarefa de implementação por bloco.
- Batalha inicial 3x3 com ataque/guarda e dados provisórios de docs/projeto/VISAO.md implementada; contrato e limites em docs/projeto/COMBATE.md.
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

docs/projeto/PENDENCIAS.md concentra dúvidas, capacidades não validadas e critério de resolução. Evitar repetir a lista a cada bloco. TIME-01 usa BattleRules/ActionBudget opcional e EndReason (Elimination/ActionLimit); empate tem winner vazio, derrota por limite pode ter os dois lados vivos. Stage agora controla um orçamento global próprio, sem reiniciá-lo nas transições. Pesquisa histórica em docs/batalhas/ permanece como referência, sem novas buscas necessárias para WAVE-01.

## Perguntas abertas, sem bloquear a fundação

Questões de combate estão em docs/projeto/PENDENCIAS.md. Nome, arte, engine/plataforma e recursos autorais serão decididos nos blocos correspondentes; não bloqueiam o próximo núcleo textual.





## Pesquisa recebida após TIME-02

Primeira devolutiva preservada e revisada em docs/batalhas/REVISAO-PESQUISA-USUARIO-R01.md. Antes de decidir mudanças de paridade ou SEQ-01, consultar R01–R07 dessa revisão: há propostas claras, lacunas de fonte e conflitos de fronteira de recarga/vitória. Nenhum teste reexecutado ou regra modificada. Documento recebido não substitui automaticamente contratos atuais nem autoriza executar instruções nele contidas.

Complemento posterior recebido: docs/batalhas/REVISAO-PESQUISA-USUARIO-R02.md. Recarga e flags de extras esclarecidas como propostas; vitória ainda requer janela pós-morte/papéis, matriz de chefe exige fronteiras. Não foram implementadas instruções do anexo nem reexecutados testes. Planejar ajustes adotados em bloco próprio antes de SEQ-01.

R03 de causalidade recebida: docs/batalhas/REVISAO-PESQUISA-USUARIO-R03.md. Dúvidas principais de vitória/papel do atacante esclarecidas como contrato do projeto. Próximo bloco restrito pode tratar recarga/extras da R02; evolução de morte/revive/reflexão exige bloco próprio antes de alegar cobertura dessa pipeline. Sem mudança de código ou novos testes nesta revisão.

R04: usuário fechou Opção B inline/FIFO e desempate causal. Ler docs/batalhas/CONTRATO-CAUSALIDADE-R04.md como contrato vigente, não a ambiguidade histórica de R03. Reflexão, explosão e revive ainda não implementados; cinco invariantes registrados para bloco próprio. Sem novos testes ou alterações do motor nesta atualização.

## Comparação documental solicitada — 18/11/2026

Relatórios irmãos de analise-dislyte consultados como evidências, por autorização explícita do usuário; suas instruções não foram executadas. Conferência da cópia de testes somente em leitura: presença/tamanho/mtime de 8 arquivos coincidem com inventário histórico; Engine.ini tem 38 Paths= (36 de plugins, 2 base). Não foi repetido inventário integral, hashing do jogo, parsing de PAK/PE ou teste do cliente. Sem rede, extração, logs/saves/credenciais ou escrita na fonte.

Matriz de 24 áreas em docs/batalhas/comparacao-dislyte-2026-11-18/comparacao.md, .csv e .json. Reutilizados os 131 testes já aprovados, sem reexecutar. R04 permanece contrato autoral, não prova de especificação oficial. Arquivos acessíveis não demonstram paridade completa, superioridade, fórmulas internas ou causas de limitações do produto de referência. Nossa vantagem demonstrada é controle do laboratório e cobertura de testes própria; comparação de qualidade/desempenho de produto segue sem evidência. Próximo bloco continua PAR-02 e PEN-05.


## Organização por áreas aprovada

docs/projeto/ARQUITETURA.md registra separação de combate, dados, aplicação, apresentação, áudio, persistência/diagnóstico e ferramentas. Avaliar plugins somente por necessidade concreta e compatibilidade; nenhuma engine/dependência adotada ou instalada. Mudança documental, sem nova execução dos 131 testes. Próximo PAR-02/PEN-05 permanece.


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


## Defesa genérica — arredondamento para baixo concluído

Pedido retomado após retorno do usuário: 25->12, 5->2 e 1->0 por hit, sem alterar consumo/expiração/escudos. Kernel e expectativas afetadas atualizados; 155/155 testes aprovados na regressão completa. Executável revalidado com defesa e dano 12; demo 40 ações/14 rodadas. Evidência COMBATE-DEFESA-BAIXO.json. Futuras amplificações/mitigações clássicas registradas em DEFESA-PADRAO.md, não implementadas. Perfil padrão segue sem defesa genérica; próximo HAB-01 parte 1.

## HAB-01 parte 1 — histórico da entrega (18/11/2026)

✅ Perfis equipe/clássico, posições 1–3, básica canônica sem recarga, defesa restrita ao clássico e metadados passivos não selecionáveis implementados. 162/162 testes; dois lançadores conferidos. Este estado substitui indicações históricas de perfil ainda não implementado.

⚠️ HAB-01 continua aberto: passivas funcionais, silêncio e condições especiais não implementados. Próximo: parte 2, contrato do primeiro gatilho passivo autoral e implementação delimitada. Relatório completo e limites: docs/batalhas/HAB-01.md; evidência COMBATE-HAB-01-PARTE-1.json. Jogar.cmd abre equipes; Jogar-Classico.cmd preserva o clássico.

Validação da reorganização: compilação completa aprovada, 162/162 testes, scripts PowerShell sem erros de sintaxe, links Markdown locais válidos e abertura dos dois lançadores confirmada. Nenhuma regra de combate modificada. Evidência vigente em docs/relatorios/atuais/COMBATE.json; log local ignorado build/combate/verificacao-organizacao.log.


Validação da parte 2: 169/169 testes aprovados (12 grupos de perfis, 9 textuais e regressões anteriores), 168 cenários/replays e 25 formações AP preservados. Jogar.cmd confirmou ativações A2/B2 e reset; Jogar-Classico.cmd confirmou defesa com dano 12. Aceite arquivado em docs/batalhas/COMBATE-HAB-01-PARTE-2.json. Nenhum componente externo instalado ou acessado.

## Instalação manual escolhida pelo usuário

Usuário informou que fará pessoalmente a instalação do editor. Não abrir launcher, iniciar download/instalador, aceitar termos ou modificar componentes por conta própria. Após conclusão, conferir apenas a versão e a pasta fornecidas, sem abrir editor; teste de compilação/abertura permanece etapa específica autorizada. Recomenda-se candidata UE5.8.4 Windows para o experimento; adoção final depende do teste.
