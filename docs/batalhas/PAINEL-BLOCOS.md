# Painel de blocos — estado consolidado

Atualizado em 18/11/2026 após PAR-01 parte 3. Fonte de resultados: COMBATE.json e COMBATE-PAR-01-FINAL.json. Este painel substitui marcações antigas; relatórios anteriores permanecem preservados.

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
| ✅ Contrato R04 e revisões R01–R03 | Decisões e pesquisa recebida | Fechamento documental; implementação mínima em RES-01 |

Total atual: **131 testes aprovados**, além das matrizes, isolamento intercalado, 1000 simulações funcionais e demonstração. Não é benchmark de servidor ou renderização.

## ⚠️ Implementado que ainda exige revisão obrigatória

**Nenhum bloco encerrado com revisão obrigatória identificada.** As revisões antes exigidas de TIME-02 foram atendidas por TIME-03 e RES-01. SEQ-01 e PAR-01 concluíram suas integrações. Famílias novas e modos ainda ausentes seguem abaixo; não representam reprovação dos contratos encerrados.

## ⚠️ Previstos desde a estrutura inicial — ainda por implementar e validar

Classificação pelo roteiro documentado, sem inferir datas pela criação do repositório.

| Ordem posterior aos modos críticos | Bloco | Falta |
|---|---|---|
| 1 | ⚠️ COM-02 | Entrada interativa de ações e reinício textual |
| 2 | ⚠️ HAB-01 | Famílias ampliadas de habilidades, recursos e efeitos |
| 3 | ⚠️ CIC-01 | Equipe, recompensa e save local versionado |
| 4 | ⚠️ ENG-01 | Escolha e teste de compatibilidade da engine |
| 5 | ⚠️ VIS-01 | Arena visual integrada ao núcleo |
| 6 | ⚠️ ART-01 | Direção artística original e personagem representativo |
| 7 | ⚠️ AUD-01 | Áudio próprio/licenciado e integração |
| 8 | ⚠️ EXP-01 | Delimitar campanha/minijogo rítmico antes de implementar |

## ⚠️ Acrescentados durante o projeto — ainda por implementar e validar

| Prioridade | Bloco | Falta |
|---|---|---|
| 1 — próximo | ⚠️ PAR-02 | Definir e implementar chefe compartilhado; HP autoritativo, ordem causal e efeitos globais únicos |

Também acrescentados durante o percurso, mas já encerrados: COM-01A, TIME-01, WAVE-01, TIME-PESQ, TIME-02, TIME-03, RES-01, SEQ-01 e PAR-01. AMB-01 formalizou a validação após instalação manual.

## Limitações e extensões

⚠️ Não implementados nesta entrega: chefe compartilhado, paralelismo real, servidor, IA completa, score, fases por limiar de HP, catálogo completo de habilidades, ATB opcional, lojas e produção multiplataforma. Sem aceites completos de implementação não são blocos fechados. Arte, áudio e escolha da engine terão validação apropriada, não necessariamente testes unitários.

Ordem atual: **PAR-02 → COM-02 → HAB-01 → CIC-01 → ENG-01 → VIS-01 → ART-01 → AUD-01 → EXP-01**. Uma entrega por vez. Priorização por risco/dependências e menor esforço útil dentro da área, sem estimativa de prazo ou consumo.


## Comparação complementar — concluída

✅ Matriz de 24 áreas em [comparacao-dislyte-2026-11-18/comparacao.md](comparacao-dislyte-2026-11-18/comparacao.md), com CSV/JSON. Reaproveita os 131 testes; não executa bateria nova nem reabre blocos encerrados. Oito arquivos da cópia local coincidem por tamanho/mtime com inventário anterior. R04 é contrato próprio; cobertura de testes não comprova paridade ou superioridade ao Dislyte. Próximo bloco continua PAR-02.
