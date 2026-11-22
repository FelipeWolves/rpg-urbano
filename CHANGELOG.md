# Histórico

## 2026-11-18 — organização do repositório

- Mantida a estrutura C++20 `include/rpg`, `src`, `app`, `tests` e `tools`.
- Planejamento reunido em `docs/projeto`; resultados consolidados em `docs/relatorios/atuais`; scripts ajustados.
- Adicionados README, índice documental, guia de contribuição, orientações de segurança, EditorConfig, atributos Git e modelos de issue/PR.
- Preservados lançadores, relatórios históricos e regras de combate. Nenhuma engine/dependência/licença pública adotada.

## 2026-11-18 — HAB-01 parte 1

Perfis equipe/clássico, posições de habilidade e básica canônica; 162/162 testes. Passivas funcionais seguem pendentes. Histórico detalhado em docs/projeto/ESTADO.md e docs/batalhas/HAB-01.md.

Validação da reorganização: compilação completa aprovada, 162/162 testes, scripts PowerShell sem erros de sintaxe, links Markdown locais válidos e abertura dos dois lançadores confirmada. Nenhuma regra de combate modificada. Evidência vigente em docs/relatorios/atuais/COMBATE.json; log local ignorado build/combate/verificacao-organizacao.log.


## 2026-11-18 — HAB-01 parte 2

Primeira passiva funcional: Preparação concede escudo de abertura em A2/B2, sem ação e com ativação registrada. Reset, rejeição de seleção, absorção, expiração e replay cobertos; híbridas informativas preservadas.

## 2026-11-18 — HAB-01 parte 3

Proteção de emergência exige até 50% de vida em A3/B3. Condição exata, básica incondicional, recusa sem custo e indicadores separados de CD/controle. 176/176 testes; relatório e contrato específicos preservados.

## 2026-11-18 — HAB-01 parte 4

Time B de equipes usa especiais por prioridade configurada e básica como fallback. Seleção pura, alvos determinísticos e validação comum; clássico preservado. 183/183 testes; 16 combinações automáticas com replay e orçamento.

## 2026-11-18 — HAB-01 encerrado

Demo integra kits ativo/passivo/híbrido funcionais, com configuração única, indicação de componentes automáticos e erros de comando específicos. Quatro grupos de integração adicionais: batalha completa, reset e matriz de oito configurações com replay/limite. 187/187 testes aprovados; próximo CIC-01.

## 2026-11-18 — CIC-01 parte 1

Seleção de 1–5 membros por ID, com validação atômica e composição filtrada independente. Jogar.cmd --equipe A1 A3 usa o catálogo autoral atual de três opções. Reinício preserva seleção; sem recompensas ou save. 195/195 testes aprovados.

## 2026-11-18 — CIC-01 parte 2

Resultado com ID por tentativa e recompensa única em memória: +10 por vitória A/eliminação, zero nos demais finais. Reset mantém saldo, consultas não concedem novamente. Estado mostra progresso local; sem save. 203/203 testes aprovados.

## 2026-11-18 — CIC-01 encerrado

Save manual v1 de seleção/progresso em build/saves; continuar valida e abre nova tentativa, sem restaurar batalha nem pagar conclusões antigas. Escrita temporária/flush/substituição e recusa de arquivos inválidos/futuros. 211/211 testes e verificação isolada do executável. Próximo ENG-01.

## 2026-11-18 — ENG-01 avaliação documental

Inventário seletivo, fontes oficiais, candidata UE5.8.4 e matriz de compatibilidade/roteiro registrados em Markdown/CSV/JSON. Editor ainda não instalado, nenhuma execução/instalação de engine ou mudança de gameplay. 211 testes anteriores reaproveitados.

## 2026-11-18 — preparação independente do editor

Codec de save separado do adaptador Windows. Formato v1 preservado, códigos explícitos e locale estável. Dois grupos novos e regressão completa:213/213 aprovados. Instalação manual UE5.8.4 em andamento; sem executar engine.

## 2026-11-18 — engine instalada, leitura confirmada

Metadados UE5.8.4/CL58902052, ferramentas e requisitos locais registrados. Zero execução/escrita na engine; nenhum teste de gameplay repetido. ENG-01 continua aberto até compilação/integração reais.

## 2026-11-18 — primeiro build Unreal

Projeto de avaliação C++ mínimo compilado em UE5.8.4, MSVC14.52.37029/SDK26100.0. Editor fechado; fontes de combate e213 testes standalone preservados. Experimento e script reproduzíveis, logs/binários ignorados.

## 2026-11-18 — ponte do núcleo compilada na Unreal

Cinco fontes nativos e bridge ligados ao módulo UE5.8.4. Baseline da mesma função passou 11 checkpoints fora da engine; manifesto de16 arquivos. Editor/runtime ainda não executados. Fonte canônica e213 testes gerais preservados.

## 2026-11-18 — ponte executada dentro da Unreal

Commandlet própria aprovada em 2 processos UE5.8.4,11 checkpoints idênticos ao baseline, saída0 e sem interface gráfica. Inicialização geral anterior teve timeout; evidência preservada e método corrigido. Ponte Windows concluída;213 testes gerais preservados, sem novo escopo visual.

## 2026-11-18 — consolidação documental após aceite da ponte

Estado, tarefas, prioridades, painel e instruções de retomada consolidados. Sínteses antigas preservadas integralmente em docs/projeto/historico/2026-11-18/. README/índice/arquitetura/ferramentas passam a distinguir o estado atual dos relatos históricos. Documentação apenas: sem código, JSON/CSV de evidência, novos testes ou execução da engine.

## 2026-11-18 — revisão final da consolidação documental

Consolidação já registrada no commit 00fd035, preservada. Links Markdown locais conferidos sem referências quebradas; seis snapshots históricos conferidos com o conteúdo do commit 25516e6. Ajuste de redação no painel. Sem alteração de código/evidências JSON/CSV, nova execução de testes ou engine.

## 2026-11-19 — ENG-01 gráfico parcial

Registrados abertura, mapa, interação e encerramento do editor em RTX 3060/D3D12/SM5. Aceite integral pendente de erros genéricos de automação e tentativa de telemetria. Relatório sanitizado, roteiro e painel atualizados; sem novos testes, execução de engine ou alteração do núcleo nesta atualização documental.


## 2026-11-19 — diagnóstico regional e padrão de trabalho Unreal

Identificados três testes internos com 14 falhas em pt-BR; comparação em inglês sem linhas de erro, ambas as execuções com saída0. Inglês priorizado no ambiente Unreal. Português do produto será tratado por localização separada, ainda a definir e validar. Opt-out observado sem os marcadores anteriores; sem mudanças de rede. Documentação e evidências consolidadas, sem novos testes nesta entrega.


## 2026-11-19 — pacote Windows básico aprovado

Adicionados alvo Game, configurações do mapa de avaliação e scripts de preparação/empacotamento. Build/cook/stage/archive com saída0; 45 arquivos, 525.955.927 bytes incluindo símbolos PDB. Abertura fora do editor, mapa renderizado em RTX3060/D3D12/SM5 e encerramento normal; bootstrap e processo principal com saída0, log sem linhas Error:/Warning:. Falha inicial de caminhos preservada, resolvida com stage curto em build/p. Avisos MSVC/ACL e espera Zen documentados. Não valida gameplay, desempenho, Windows limpo, localização ou rede isolada. Próximo VIS-01; sem repetir testes nesta consolidação.


## 2026-11-19 — adaptador e arena funcional3×3

Cenário compartilhado extraído da apresentação textual; adaptador nativo com revisão de comandos e automação limitada. 213 testes anteriores e6 novos casos aprovados. Cena Canvas com formas provisórias, câmera, vida/escudo e três posições de habilidade ligada ao núcleo. Pacote revisado observado até vitória A em20 ações e reinício, com saída0 e log sem erros/avisos. Aceite integral permanece parcial para leitura, descrição/seleção de alvo e formações1/5/resoluções. Nenhuma arte proprietária, instalação ou mudança de rede.

## 2026-11-19 — arena, interação e organização da pesquisa

- Profundidade padrão e câmera lateral preservada; perfil de regras Classic explícito e independente da câmera.
- Efeitos/detalhes consultáveis, escudo em pontos reais, seleção de alvos e defesa clássica pela sessão compartilhada; evidências e limites em VIS-01-CLASSICO-INTERACAO.md.
- Pesquisa recebida preservada com hash e revisão crítica; perguntas ESC/VIS/REG/SIS centralizadas em PENDENCIAS.md, com prioridades e campos de resposta.
- Índice, estado, tarefas e guia do experimento atualizados. Núcleo/experimento permanecem nas pastas existentes; sem novas dependências, arte proprietária ou mudança de regras por documentação.
- Testes/compilação anteriores preservados; esta organização não executa novamente gameplay nem abre aplicativos.

Verificação desta organização: links Markdown locais dos arquivos alterados válidos, JSONs alterados legíveis, scripts PowerShell sem erros de sintaxe e revisão de espaços (quebras Markdown do documento recebido preservadas por atributo específico). Documento recebido idêntico à origem por SHA-256. Testes funcionais anteriores não repetidos por mudanças somente documentais.

## 2026-11-19 — escudo padrão por habilidade

StandardShield implementa decisões ESC-01–07:1% do máximo do aplicador calculado uma vez,3 turnos naturais do protegido, absorção por hit após mitigação, max de pontos/duração sem soma, zero ignorado, remoção na morte/transições e manutenção após morte do aplicador. Terceira ativa do cenário atualizada; passivas fixas preservadas. Extras com tick não avançam o escudo; impossibilidade de agir em oportunidade natural avança. Terminologia corrigida sem implementar bloqueio probabilístico.14 grupos próprios e regressão pertinente; build Editor sem abertura/pacote. Contrato e limites em docs/projeto/ESCUDO-PADRAO.md.

## 2026-11-19 — propagação das decisões ESC em P2/P3

Corrigida a consolidação de PENDENCIAS.md: VIS/REG/SIS agora distinguem decisões já respondidas, implementação/testes existentes e somente as dúvidas/aceites restantes. Relógios, persistência, reaplicação, alvos, aritmética e distinção Defesa/bloqueio/proteção clássica não devem ser perguntados novamente para o escudo padrão. Alteração documental, sem código ou novos testes.

## 2026-11-19 — mínimo de proteção na aplicação válida

Por decisão do usuário, o escudo padrão direto passa a max(1,floor(PV máximo do aplicador/100)). Isso substitui ignorar percentuais arredondados para0. Reaplicação mínima oferece3 turnos, preservando maior força/duração. Escudo consumido/expirado continua removido; nenhum mínimo de HP, aplicação em morto ou concessão sem condição satisfeita. Cenário B3 e testes de limites/reaplicação atualizados. Análise documental da Clara preservada com ressalvas de fonte e da revisão histórica; VIS-01/P2/P3 e orientações atualizados.

## 2026-11-19 — painel geral e referências do hub

Adicionado PROGRESSO-GERAL.md em destaque no README: estimativas por área, lacunas e frentes separáveis, incluindo cinema/vídeo local. Referências do hub associadas visualmente aos arquivos reais; texto recebido preservado sem copiar capturas/dados de conta. Propostas do outro assistente separadas de decisões e capacidades existentes. Entrega documental: sem mudança de código, novas ferramentas ou testes de gameplay.

## 2026-11-19 — HUB-01A: base de imagem/vídeo por dispositivo

Política C++ para panorama/regiões configuráveis, proporção de vídeo, intenção de orientação mobile e laterais no desktop. Preferências antes da janela e variantes de mídia registradas em catálogo versionado, com validador local;4 Planned/0 Ready.10 grupos nativos aprovados; nenhuma regra de combate alterada. Runtime JSON/renderer/player, orientação nativa e teste em aparelho continuam pendentes. Recursos/arte não gerados nem baixados; sem novas dependências.

## 2026-11-19 — recebimento das mídias do hub/entrada

Panorama JPEG, arte lateral PNG e dois MP4 recebidos; metadados locais conferidos sem player e catálogo atualizado para nomes/dimensões reais. Autoria/geração própria declarada pelo usuário. Acrescentado script de inspeção e relatório; mídias/fontes preservadas e ignoradas no Git até estratégia de grandes arquivos. Não houve mudança no combate, conversão ou execução de vídeo/engine.

## 2026-11-19 — dois acessos no panorama provisório

Cadastro separado da arte para Evento Especial no outdoor e Batalha no dispositivo. Coordenadas normalizadas estimadas e rotas internas identificadas; terceiro adiado pelo usuário. Metadados validados; renderer/roteador ainda não ligados. Nenhuma edição de imagem, abertura de aplicativo ou mudança de combate.

## 2026-11-19 — alvos do hub confirmados por marcação

Imagem marcada do usuário resolve a localização do outdoor e do dispositivo. Âncoras/áreas normalizadas atualizadas no catálogo; marcação preservada localmente fora do Git. Não altera arte original, rotas ou quantidade de acessos e não implica botões já renderizados.

## 2026-11-19 — HUB-01B por código sem abrir editor

Parser JSON do hub, importação do panorama, projeção/clipping dos dois alvos e painel de evento/arena/volta. Entrada opt-in -Hub preserva arena direta. Sessão mantida na navegação, sem ações automáticas no hub pelo fluxo do HUD.10 grupos de apresentação e5 de alvos aprovados; compilação Editor registrada, sem execução gráfica/cook/pacote. Preparação37 entradas inclui catálogo/JPEG e regra de staging, não vídeos. Player/mobile continuam pendentes.

## 2026-11-21 — Consolidação e projeto Unreal estável
- Revisão dos15Markdown, contratos, fila e progresso ponderado da demo (aproximadamente22%).
- Projeto-fonte compilável diretamente, mapa próprio e hub padrão; abertura/reabertura/Play observados.
- Android: caminhos persistidos, descoberta INVALID ainda pendente; nenhum APK.
- Mídias e caches fora doGit; estágios antigos preservados. Controle de tela encerrado a pedido.

## 2026-11-21 — Hub da cidade e política de apresentação
- Panorama diurno, sete locais separados, painéis informativos e movimento reduzido.
- AUT-01 Desktop/Mobile/Auto, dock responsivo parcial e prévias Windows atualizadas.
-26grupos nativos e compilação8ações aprovados; aceite visual pendente do usuário. Android adiado.

## 2026-11-21 — AUT-02 e migração da pasta principal
- Geometria centralizada do hub, margens simuladas e botões mínimos48px.
-34grupos de apresentação e compilação limpa22ações aprovados na pasta nova.
- Cache absoluto copiado identificado, preservado e regenerado; resultado antigo não aceito como validação.
- Sem execução gráfica; próximo AUT-03, Android adiado.

## 2026-11-21 — AUT-03
- Entrada amostrada comum de mouse/toque, deduplicação, origem do clique e cancelamentos.
- Hub/arena integrados; consulta prolongada preservada, regras de combate intactas.
-44grupos nativos e compilação aprovados; aceite interativo pendente, sem Android ou controle de tela.

## 2026-11-21 — AUT-04
- Quebra UTF-8 medida, paginação segura e áreas separadas de texto/botões.
- Painel compartilhado entre habilidades, efeitos e locais do hub.
-54grupos de apresentação e build8ações aprovados; sem validação visual ou Android.

## 2026-11-21 — AUT-05
-13snapshots reais de formações/escudo em modo de inspeção somente leitura.
-Atalhos para profundidade e lateral clássica; nenhum comando/recompensa na inspeção.
-22grupos focados e compilação9ações aprovados; aceite visual pendente, Android adiado.

## 2026-11-21 — AUT-06 e aceite do AUT-05
- Estados de habilidade, contador, cadeado, indisponibilidade e motivo de recusa.
-32grupos focados e compilação7ações aprovados; aceite visual novo pendente.
-Registrado aceite do usuário nos dois layouts/detalhes da inspeção; progresso ponderado22,09%.
-Android adiado; sem execução gráfica pelo assistente.

## 2026-11-21 — AUT-07
- Preferências versionadas de apresentação, movimento, contraste, texto e enquadramento.
- Menu Opções e adaptador local separado do save; backup e falhas visíveis.
-64grupos nativos (324combinações válidas no codec) e build8ações aprovados.
- Persistência real/aceite visual pendentes; nenhum Android ou controle de tela.

## 2026-11-22 — Correção do ponteiro e AUT-08
- Conversão viewport/Canvas corrigida e aceita pelo usuário; commit1400d4e.
- Estados explícitos, pausa/retomada e abandono confirmado sem recompensa.
-82grupos de apresentação,35focados e compilação8ações aprovados.
- Aceite interativo dos menus pendente; sem editor ou Android.

## 2026-11-22 — AUT-09

- Equipe / Save no hub, rascunho validado de 1–5 IDs, detalhes de resultado e adaptador Unreal para codec v1.
- Restauração inicia nova tentativa e preserva histórico; validação por perfil/catálogo, recusa em combate e invalidação de comandos antigos.
- 64 grupos nativos focados aprovados; Editor compilado. Interface e I/O real pendentes, Android adiado. Estimativa global mantida.

## 2026-11-22 — AUT-10

- Contrato puro de mídia local: preparação, reprodução lógica, pausa, conclusão, skip, falha e cancelamento; callbacks identificados e fallback ao hub.
- Regras recebidas de hold 800 ms, rever imediato e retomada pausada; reaproveitamento da geometria de vídeo. Sem player/interface nova; integração no AUT-11.
- 94 grupos de apresentação aprovados, incluindo 12 novos; nenhuma tela ou Android acessados.

## 2026-11-22 — AUT-11 parcial

- Ensaio manual Rever abertura no hub; backend local instalado de mídia/áudio, pausa/skip/fallback, flag por variante e limpeza de recursos.
- Compilação aprovada após correção de linker. Vídeos copiados somente ao runtime ignorado e manifesto atualizado; originais preservados.
- Sem execução de player/telas/Android. Aceite de reprodução, legendas, mixer, arte lateral e autoabertura ainda pendentes.

## 2026-11-22 — correção do ensaio AUT-11

- Habilitado WmfMedia já instalado para Win64 após log confirmar ausência de decoder no projeto.
- Painéis informativos/erros agora interceptam entrada e aparecem no hub, sem depender de abrir equipe.
- Reteste de reprodução pendente; nenhum editor ou Android executado.

## 2026-11-22 — foco do ensaio de mídia

- Corrigida distinção de foco da janela/viewport na mídia; logs de ciclo para investigar segundo clique.
- Backend Android/iOS registrado como validação obrigatória futura; reteste do clique pendente.

## 2026-11-22 — AUT-12

- Marcador de seleção em malhas de runtime, sem DrawDebugLine; ciclo próprio e geometria testada.
- Aceite visual/Shipping/terreno pendentes. Primeiro clique da intro marcado REVER pelo usuário.
