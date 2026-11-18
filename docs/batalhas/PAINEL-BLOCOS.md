# Painel de blocos — estado consolidado

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

Atualizado em 18/11/2026 após COM-02. Fonte atual de resultados: COMBATE.json e COMBATE-DEFESA-BAIXO.json; COMBATE-COM-02-FINAL.json preserva a entrega anterior. Este painel substitui marcações antigas; relatórios anteriores permanecem preservados.

## Legenda das siglas e marcações

As siglas são identificadores internos das áreas do projeto, não nomes de ferramentas ou engines. O número identifica o bloco; um sufixo, como A em COM-01A, identifica um complemento.

| Sigla | Significado | Área |
|---|---|---|
| FUN | Fundação | Visão, organização e decisões iniciais |
| AMB | Ambiente | Compilador, SDK e ferramentas locais |
| COM | Combate | Motor básico, análises e interação textual |
| TIME | Tempo (inglês) | Relógios, ordem de ação, recargas e extras; não significa equipe |
| PESQ | Pesquisa | Sufixo de investigação, como TIME-PESQ |
| WAVE | Onda (inglês) | Confrontos consecutivos com o mesmo time |
| RES | Resolução | Causalidade de dano, mortes e reações |
| SEQ | Sequência | Times sucessivos contra chefe persistente |
| PAR | Paralelo | Coordenação de frentes; nesta etapa usa intercalação local, sem threads |
| HAB | Habilidades | Ações, recursos e efeitos ampliados |
| CIC | Ciclo | Equipe, recompensa e persistência |
| ENG | Engine | Escolha e integração do motor gráfico |
| VIS | Visual | Arena e apresentação funcional |
| ART | Arte | Direção artística e recursos originais |
| AUD | Áudio | Sons e identidade musical |
| EXP | Expansão | Campanha e experiências adicionais |

✅ indica encerramento no escopo descrito; ⚠️ indica trabalho ou validação ainda pendente. Parte concluída não encerra automaticamente o bloco inteiro. R01–R04 identificam revisões/contratos documentais, e PEN identifica uma pendência em PENDENCIAS.md.
## ✅ Encerrados no escopo entregue

**Não precisam de novos testes nem de revisão agora, salvo mudança relevante no bloco ou nas suas dependências.** Entrega documental não equivale a funcionalidade implementada. Regressão pertinente continua válida quando o núcleo mudar.

| Bloco | Entrega validada | Evidência/limite |
|---|---|---|
| ✅ FUN-01 | Fundação e organização | Visão, arquitetura e contratos |
| ✅ FUN-02 | Diagnóstico inicial de ferramentas | Registro histórico; ambiente posterior em AMB-01 |
| ✅ AMB-01 | C++20/MSVC x64 | Diagnóstico compilado e executado |
| ✅ COM-01 | Combate básico | 13 testes |
| ✅ COM-01A | Formações e investigação de modos | 168 cenários e replays; pesquisa não implementa todos os modos |
| ✅ TIME-01 | Orçamento local de ações | 11 testes |
| ✅ WAVE-01 | 1/2/5 confrontos e orçamento global | 9 testes |
| ✅ TIME-PESQ | Pesquisa e experimento de relógios | Não comprova paridade externa |
| ✅ TIME-02 | Perfis fixo/AP | 25 formações AP/replays; evolução posterior concluída |
| ✅ TIME-03 | Recarga, extras e controle separado | 34 grupos de tempo acumulados |
| ✅ RES-01 | Resolução causal R04 e sobrevivência | 31 grupos acumulados, incluindo escudos/categorias |
| ✅ SEQ-01 | Três times sucessivos e chefe por etapa | 17 grupos; três partes concluídas; A/A/B e A/A/A |
| ✅ PAR-01 | Três frentes independentes coordenadas, orçamento global e cancelamento | 16 grupos; três partes concluídas, integração R04/replay/falha técnica |
| ✅ PAR-02 — perfil HP único | HP/AP globais, skills de alvo único, R04, limites e replay | 17 grupos; sem fases autônomas de kit, área comum ou score |
| ✅ COM-02 | Entrada textual, alvo, estado/resultado e reinício | Base de 7 grupos; 15 textuais após integração HAB-01; sem menu SEQ/PAR |
| ✅ HAB-01 | Perfis/posições, passiva, condição, automação e kits integrados | Cinco partes concluídas; 24 grupos de perfis e 15 textuais acumulados; HAB-01-FINAL.md |
| ✅ CIC-01 | Seleção de equipe, recompensa única e save v1 entre batalhas | Três partes concluídas; seleção 7, recompensa 7, save 7 e interação 18 grupos acumulados; CIC-01-SAVE-R01.md |
| ✅ Contrato R04 e revisões R01–R03 | Decisões e pesquisa recebida | Fechamento documental; implementação mínima em RES-01 |

Total atual: **211 testes aprovados**, além das matrizes, isolamento intercalado, 1000 simulações funcionais e demonstração. Não é benchmark de servidor ou renderização.

## ⚠️ Implementado que ainda exige revisão obrigatória

**Nenhum bloco encerrado com revisão obrigatória identificada.** As revisões antes exigidas de TIME-02 foram atendidas por TIME-03 e RES-01. SEQ-01, PAR-01 e perfil PAR-02 concluíram suas integrações. Famílias novas e modos ainda ausentes seguem abaixo; não representam reprovação dos contratos encerrados.

## ⚠️ Previstos desde a estrutura inicial — ainda por implementar e validar

Classificação pelo roteiro documentado, sem inferir datas pela criação do repositório.

| Ordem posterior aos modos críticos | Bloco | Falta |
|---|---|---|
| 1 | ⚠️ ENG-01 | Escolha e teste de compatibilidade da engine |
| 2 | ⚠️ VIS-01 | Arena visual integrada ao núcleo |
| 3 | ⚠️ ART-01 | Direção artística original e personagem representativo |
| 4 | ⚠️ AUD-01 | Áudio próprio/licenciado e integração |
| 5 | ⚠️ EXP-01 | Delimitar campanha/minijogo rítmico antes de implementar |

## ⚠️ Acrescentados durante o projeto — ainda por implementar e validar

| Família catalogada | Estado | Falta |
|---|---|---|
| ⚠️ PVE-09 | Não implementada | Fases de kit por HP/turno e eventos únicos; fora do perfil encerrado |
| ⚠️ PVE-06 | Relato | Sobrevivência/score/recompensas do Treva Fluxo a detalhar |
| ⚠️ PVE-07/08 | A esclarecer | Milagres e possível retorno cíclico de times |
| ⚠️ Seletores ampliados | Não implementados | Área comum, dinâmica do chefe e efeitos entre times |

Também acrescentados durante o percurso, mas já encerrados: COM-01A, TIME-01, WAVE-01, TIME-PESQ, TIME-02, TIME-03, RES-01, SEQ-01, PAR-01 e perfil PAR-02. AMB-01 formalizou a validação após instalação manual.

## Limitações e extensões

⚠️ Não implementados nesta entrega: skills comuns de área e redirecionamento dinâmico do chefe, paralelismo real, servidor, IA completa, score, fases por limiar de HP, catálogo completo de habilidades, ATB opcional, lojas e produção multiplataforma. Sem aceites completos de implementação não são blocos fechados. Arte, áudio e escolha da engine terão validação apropriada, não necessariamente testes unitários.

Ordem atual: **ENG-01 → VIS-01 → ART-01 → AUD-01 → EXP-01**. Uma entrega por vez. Priorização por risco/dependências e menor esforço útil dentro da área, sem estimativa de prazo ou consumo.


## Comparação complementar — concluída

✅ Matriz de 24 áreas em [comparacao-dislyte-2026-11-18/comparacao.md](comparacao-dislyte-2026-11-18/comparacao.md), com CSV/JSON. Reaproveita os 131 testes; não executa bateria nova nem reabre blocos encerrados. Oito arquivos da cópia local coincidem por tamanho/mtime com inventário anterior. R04 é contrato próprio; cobertura de testes não comprova paridade ou superioridade ao Dislyte. Próximo bloco continua PAR-02.


## Organização complementar PvE — concluída

✅ [MODOS-PVE.md](MODOS-PVE.md): 12 entradas distinguindo exemplos relatados, pistas históricas, decisões autorais e cobertura. Treva Fluxo/sobrevivência/pontuação e chefes dos Milagres separados de PAR-02. Checklist antigo atualizado, histórico preservado. [ROTEIRO-PVE-CHEFES.md](ROTEIRO-PVE-CHEFES.md) orienta pesquisa adicional. Não é catálogo completo atual; nenhum teste novo. PAR-02 continua parcial, próximo parte 2.


✅ Abertura COM-02 corrigida: usar Jogar.cmd ou executável direto. Bloqueio de .ps1 reproduzido no Windows PowerShell; nenhuma política alterada. Verificações pontuais de abertura aprovadas; os 155 testes de motor/interação não foram repetidos.


✅ Contrato padrão de equipes R01 documentado; terminologia defesa aplicada no protótipo. ⚠️ HAB-01 continua aberto: posições/perfil, silêncio e passivas funcionais ainda exigem implementação. Matriz de congelamento/petrificação/supressão não confirmada. Verificação localizada 7/7, sem nova bateria integral; COM-02-DEFESA.json.


✅ Defesa genérica atualizada: metade arredondada para baixo por acerto. Regressão pertinente dos blocos afetados concluída,155/155; executável confirmou25->12. COMBATE-DEFESA-BAIXO.json. ⚠️ Outras regras de dano/defesa do clássico são possibilidades futuras em DEFESA-PADRAO.md, não funcionalidades do padrão de equipes.

## HAB-01 parte 1 — histórico da entrega (18/11/2026)

✅ Perfis equipe/clássico, posições 1–3, básica canônica sem recarga, defesa restrita ao clássico e metadados passivos não selecionáveis implementados. 162/162 testes; dois lançadores conferidos. Este estado substitui indicações históricas de perfil ainda não implementado.

⚠️ HAB-01 continua aberto: passivas funcionais, silêncio e condições especiais não implementados. Próximo: parte 2, contrato do primeiro gatilho passivo autoral e implementação delimitada. Relatório completo e limites: docs/batalhas/HAB-01.md; evidência COMBATE-HAB-01-PARTE-1.json. Jogar.cmd abre equipes; Jogar-Classico.cmd preserva o clássico.
