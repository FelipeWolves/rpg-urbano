# Painel de blocos — estado consolidado

Atualizado em 19/11/2026. Este painel é vigente; [histórico integral](../projeto/historico/2026-11-18/PAINEL-BLOCOS.md.txt). Aceites detalhados permanecem nos relatórios de cada bloco.

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
| ✅ COM-02 | Entrada textual, alvo, estado/resultado e reinício | Base de 7 grupos; 18 textuais acumulados após CIC-01/ENG-01; sem menu SEQ/PAR |
| ✅ HAB-01 | Perfis/posições, passiva, condição, automação e kits integrados | Cinco partes concluídas; 24 grupos de perfis e 15 textuais acumulados; HAB-01-FINAL.md |
| ✅ CIC-01 | Seleção de equipe, recompensa única e save v1 entre batalhas | Três partes concluídas; seleção 7, recompensa 7, save 7 e interação 18 grupos acumulados; CIC-01-SAVE-R01.md |
| ✅ ENG-01 — ponte Windows | Fontes ligados e executados na UE5.8.4 | 11 checkpoints equivalentes ao standalone em 2 processos, código0; não inclui gráfico/pacote/mobile |
| ✅ Contrato R04 e revisões R01–R03 | Decisões e pesquisa recebida | Fechamento documental; implementação mínima em RES-01 |

Total atual: **213 testes gerais standalone aprovados**. Separadamente, a ponte passou **11 checkpoints em duas execuções Unreal**, com saída zero e comparação ao baseline. Também foram preservados os resultados das matrizes, do isolamento intercalado, das 1.000 simulações funcionais e da demonstração. Não é benchmark de servidor ou renderização.

## ⚠️ Implementado que ainda exige revisão obrigatória

**Nenhum bloco encerrado com revisão obrigatória identificada.** As revisões antes exigidas de TIME-02 foram atendidas por TIME-03 e RES-01. SEQ-01, PAR-01 e perfil PAR-02 concluíram suas integrações. Famílias novas e modos ainda ausentes seguem abaixo; não representam reprovação dos contratos encerrados.

## ⚠️ Previstos desde a estrutura inicial — ainda por implementar e validar

Classificação pelo roteiro documentado, sem inferir datas pela criação do repositório.

| Ordem posterior aos modos críticos | Bloco | Falta |
|---|---|---|
| 1 | ⚠️ ENG-01 — gráfico/pacote | Abertura básica e diagnóstico regional registrados; inglês priorizado (zero erros na comparação). Pacote/mobile pendentes. Português no jogo terá localização própria a definir; ENG-01-GRAFICO-DIAGNOSTICO.md |
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
