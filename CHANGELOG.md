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
