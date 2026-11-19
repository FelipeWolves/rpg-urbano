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
| HUB | Central de navegação | Proposta de áreas, avatar, menus e acesso ao combate |
| ART | Arte | Direção artística e recursos originais |
| AUD | Áudio | Sons e identidade musical |
| EXP | Expansão | Campanha e experiências adicionais |

✅ indica encerramento no escopo descrito; ⚠️ indica trabalho ou validação ainda pendente. Parte concluída não encerra automaticamente o bloco inteiro. R01–R04 identificam revisões/contratos documentais, e PEN identifica uma pendência em PENDENCIAS.md.
## ✅ Encerrados no escopo entregue

**Não precisam de novos testes nem de revisão agora, salvo mudança relevante no bloco ou nas suas dependências.** Entrega documental não equivale a funcionalidade implementada. Regressão pertinente continua válida quando o núcleo mudar.

| Bloco | Entrega validada | Evidência/limite |
|---|---|---|
| ✅ Escudo padrão (ESC-01–07) | Contrato comum implementado:1% com mínimo de1 ponto;3 turnos;14 grupos nativos e build Editor | [Relatório](../projeto/ESCUDO-PADRAO.md); aceite visual continua pendente |
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
| ✅ ENG-01 — pacote Windows básico | Build/cook/stage/archive e abertura/mapa/encerramento nesta máquina | Development/Win64; saída0 do build, bootstrap e processo principal; ENG-01-PACOTE-RUNTIME.md; sem gameplay/Windows limpo |
| ✅ Contrato R04 e revisões R01–R03 | Decisões e pesquisa recebida | Fechamento documental; implementação mínima em RES-01 |

Total atual: **213 testes gerais standalone aprovados**. Separadamente, a ponte passou **11 checkpoints em duas execuções Unreal**, com saída zero e comparação ao baseline. Também foram preservados os resultados das matrizes, do isolamento intercalado, das 1.000 simulações funcionais e da demonstração. Não é benchmark de servidor ou renderização.

## ⚠️ Implementado que ainda exige revisão obrigatória

**Nenhum bloco encerrado com revisão obrigatória identificada.** As revisões antes exigidas de TIME-02 foram atendidas por TIME-03 e RES-01. SEQ-01, PAR-01 e perfil PAR-02 concluíram suas integrações. Famílias novas e modos ainda ausentes seguem abaixo; não representam reprovação dos contratos encerrados.

## ⚠️ Previstos desde a estrutura inicial — ainda por implementar e validar

Classificação pelo roteiro documentado, sem inferir datas pela criação do repositório.

| Ordem posterior aos modos críticos | Bloco | Falta |
|---|---|---|
| 1 | ⚠️ ENG-01 — produção/multiplataforma | Pacote Windows básico aprovado. Faltam Windows limpo/pré-requisitos, desempenho, distribuição, mobile e localização; inglês no ambiente e português do produto são decisões separadas |
| 2 | ⚠️ VIS-01 | A concluído (6 testes nativos); B/C funcional3×3 aprovado no pacote, vitória/reinício/saída0. Aceite integral parcial: HP/escudo, descrições/alvos e1/5/resoluções pendentes; VIS-01C.md |
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


Decisão de apresentação: Standard em profundidade passa a ser o padrão; Classic lateral preservado por chamada própria. Testes nativos de posições1–5 aprovados; nova câmera ainda sem aceite visual. Contrato: docs/projeto/VIS-01-LAYOUTS.md. Preservar o aceite funcional3×3 anterior como evidência da versão lateral, sem transferi-lo automaticamente para esta revisão.


Apresentações verificadas no pacote em19/11/2026: Standard sem argumento e Classic por chamada explícita, ação/resposta/reinício observados com mesmos valores. Entrada e processo principal de ambas saíram0. Aceite visual integral parcial por sobreposição de etiquetas ao fundo e demais pendências. Relatório: docs/projeto/VIS-01-LAYOUTS-RUNTIME.md. Aplicativos encerrados; próximo ajuste é legibilidade/áreas de seleção.


Interação inicial implementada e compilada em19/11/2026: ícone/painéis, faixa de escudo real, gesto prolongado, nível opcional e seleção padrão com marcador/sugestão/confirmação. Regressão213+6 e6 novos grupos aprovados. Ainda sem teste visual desta revisão; cálculo percentual do escudo segue separado dos escudos fixos antigos. Relatório: docs/projeto/VIS-01-INTERACAO-IMPLEMENTACAO.md. Próximo: inspeção autorizada do pacote atualizado.

### VIS-01 — extensão clássica da interação (19/11/2026)

✅ Implementação/adaptador nativos: Classic explícito, ataque/defesa preservados e informações compartilhadas;6 novos grupos aprovados, além dos213 gerais,6 do adaptador e6 de apresentação. Build Editor aprovado. ⚠️ Aceite visual deste incremento,1/5 e resoluções ainda pendentes; escudo percentual aguarda documento. Não fechar VIS-01 integral por compilação. [Relatório](../projeto/VIS-01-CLASSICO-INTERACAO.md).

Pesquisa recebida: [matriz de incorporação](../projeto/REVISAO-PESQUISA-HABILIDADES-ITENS.md). Preservados contratos encerrados; escudo percentual em especificação e fontes externas sem auditoria. Sem nova execução de testes nesta revisão.

### Fila de decisões e validações — organização vigente

✅ P1: ESC-01–07 definidos e implementados no caso comum;14 grupos próprios e build aprovados. 🟠 P2: VIS-01–03 (aceite visual) e REG-01–03 (contratos relacionados). 🟡 P3: SIS-01–04 (sistemas/pesquisa futura). Lista completa, exemplos e campos de resposta em [PENDENCIAS.md](../projeto/PENDENCIAS.md). O usuário pode pesquisar todos juntos; P2/P3 continuam separados do caso comum P1 já entregue. Propostas de extensões não são decisões aprovadas. Códigos novos são IDs de perguntas, não novos blocos concluídos.

HUB-01: referência recebida e imagens associadas em [HUB-01-REFERENCIAS.md](../projeto/HUB-01-REFERENCIAS.md). ⚠️ Proposta de escopo/contrato, sem implementação; não confundir com o aceite visual da arena.

HUB-01A: ✅ fundação de dados/enquadramento testada (10 grupos nativos); ⚠️ imagem real, renderer/player, preferências de janela e rotação em aparelho pendentes. [Relatório](../projeto/HUB-01A-APRESENTACAO.md). Não encerra HUB integral.
