> **Estado vigente — AUT-12 a AUT-16 (22/11):** [entregas, verificações e próximos passos](CONSOLIDACAO-AUT-12-16.md). Marcador runtime, fila visual, schema de referência, stage verificado e QA por área entregues em escopos próprios. Aceites gráficos/Shipping/mobile permanecem pendentes; AUT-11 ainda parcial. Clique inicial: aparentemente correto, **REVER** pelo usuário. Sem telas/editor/cook/Android/internet nesta rodada. Esta nota prevalece sobre os estados históricos abaixo.

> **Correção AUT-11 (22/11):** teste do usuário revelou ausência de decoder habilitado; WmfMedia da engine agora habilitado somente em Win64. Avisos de mídia passam a abrir no próprio hub, bloqueando ações por trás. Reteste de playback pendente; abertura automática continua desativada. Ver AUT-11-PLAYER-LOCAL.md e AUT-11-CORRECAO.json.

> **AUT-11 vigente (22/11):** [ensaio de player local](AUT-11-PLAYER-LOCAL.md) implementado e compilado, disponível por Rever abertura no hub. Vídeos autorais copiados para runtime ignorado. **Playback/áudio/flag não executados nem aceitos; AUT-11 parcial.** Autoabertura, legendas, mixer e arte lateral seguem pendentes. Sem telas/Android. Próximo: aceite de reprodução; AUT-12 é código independente possível por nova solicitação. Notas anteriores são históricas.

> **AUT-10 vigente (22/11):** [contrato de mídia local](AUT-10-MIDIA-LOCAL.md) implementado; 12 grupos novos e 82 regressões aprovados. Estados, skip, pausa, callbacks e fallback preparados; sem player ou alteração visual. Compilação registrada em AUT-10-BUILD.json. Próximo: AUT-11, backend e integração efetiva; Android continua adiado. Aceites e I/O de AUT-08/09 permanecem pendentes. Notas anteriores são históricas.

> **AUT-09 vigente (22/11):** [equipe, resultado e save local](AUT-09-EQUIPE-RESULTADO-SAVE.md) implementados; 64 grupos nativos focados e compilação Editor aprovados. Menu Equipe / Save, seleção validada, detalhes do resultado e codec v1 ligados ao adaptador Unreal. Aceite interativo e gravação/reabertura pela interface pendentes. Catálogo provisório continua com três aliados. Próximo código: AUT-10 (contrato de mídia); Android adiado. Notas anteriores são históricas.

> **AUT-08 vigente:** [estados de tela, pausa e abandono](AUT-08-FLUXO-TELAS.md) implementados/testados e compilados;82grupos de apresentação e35focados aprovados. Menus novos pendentes de aceite interativo. Correção do ponteiro aceita pelo usuário e salva no commit1400d4e. Próximo: AUT-09 (equipe/resultado/save). Android adiado; notas anteriores são históricas.

> **AUT-07 vigente:** [preferências locais](AUT-07-PREFERENCIAS.md) implementadas/testadas no modelo e adaptador compilado;64grupos de apresentação aprovados. Menu Opções, movimento/contraste/texto/enquadramento. Gravação/reabertura real e aparência pendentes; save de combate separado. Próximo código: AUT-08, estados/transições de tela. Android adiado; notas anteriores são históricas.

> **AUT-06 vigente:** [estados das habilidades](AUT-06-HABILIDADES.md) implementados/testados e compilados;32grupos focados aprovados, aparência nova pendente. AUT-05 recebeu aceite do usuário nos dois layouts e detalhes, sem problemas relatados. Próximo código: AUT-07, preferências/acessibilidade persistidas. Android adiado; notas anteriores são históricas.

> **AUT-05 vigente:** [inspeção de formações/escudo](AUT-05-INSPECAO.md) preparada, testada e compilada;13cenários,22grupos focados aprovados. Atalhos Abrir-Inspecao.cmd e Abrir-Inspecao-Classico.cmd. Somente leitura; aceite visual pendente. Próximo código: AUT-06, estados das habilidades. Android adiado; notas anteriores de AUT-05 por iniciar são históricas.

> **AUT-04 vigente:** [painéis e paginação](AUT-04-PAINEIS.md) implementados/testados e compilados;54grupos de apresentação aprovados. Habilidades, efeitos e locais usam a mesma base. Aceite visual pendente, Android adiado. Próximo código: preparação AUT-05 (formações/escudo); notas anteriores de AUT-04 por iniciar são históricas.

> **AUT-03 vigente:** [entrada comum mouse/toque](AUT-03-ENTRADA.md) implementada/testada e adaptador compilado;44grupos de apresentação aprovados. Sem execução gráfica ou Android; aceite interativo pendente. Próximo bloco de código: AUT-04, painéis/textos. Notas anteriores de AUT-03 ainda não iniciado são históricas.

> **AUT-02 / pasta principal:** [geometria e margens seguras](AUT-02-GEOMETRIA.md) implementadas/testadas, integração compilada;34grupos nativos aprovados. Trabalhar apenas em F:/Testes de jogos/Contos de Um Lobo - Jogo/rpg-urbano. Caches copiados foram preservados e regenerados; não reutilizar Intermediate ao mover o projeto. Próximo código: AUT-03; aceite visual/físico permanece pendente, Android adiado. Esta nota prevalece sobre a fila anterior.

> **Hub vigente21/11:** [panorama da cidade, sete locais e motion](HUB-PANORAMA-CIDADE.md), implementado/compilado; aceite visual pendente do usuário. AUT-01 pronto em código; AUT-02 parcial, depois AUT-03/04. Android adiado. Sem controle de tela nesta entrega. Esta nota substitui a prioridade anterior de iniciar AUT-01 e as âncoras antigas do terraço.

> **Entrega concluída:** [projeto Unreal estável e guia de abertura](UNREAL-PROJETO-ESTAVEL.md). Controle de tela encerrado por solicitação; nova interação com o editor exige nova liberação. Atualização documental/Git apenas após essa solicitação.

> **Estado vigente — 21/11/2026:** [consolidação das 15 entregas e decisões posteriores](CONSOLIDACAO-2026-11-21.md). ENG-ESTAVEL concluído no Windows local: projeto fixo, mapa próprio, abertura/reabertura e Play observados. Próximo: AUT-01–04. ESC comum e seletor já aceitos não são reabertos. Android SDK/NDK/JDK localizados e boot do AVD confirmado pelo usuário; caminhos já persistem, mas ValidatePlatforms retorna Android INVALID r27c; diagnóstico e APK pendentes. Percentuais recalculados no painel de progresso. As notas datadas abaixo preservam histórico e não reabrem decisões substituídas.

# Estado do projeto

Atualizado em 19/11/2026. Esta é a síntese vigente; registros anteriores estão no [histórico preservado](historico/2026-11-18/ESTADO.md.txt).

**Visão geral:** [percentuais por área e frentes independentes](PROGRESSO-GERAL.md). Estimativas de planejamento, separadas das evidências técnicas abaixo.

## Prioridade vigente

**Revisão20/11:**12 entregas estudadas e integradas ao planejamento, não ao código. [Síntese/matriz/ENT-01–15](REVISAO-ENTREGAS-2026-11-20.md). Android Studio confirmado emD:\Android; SDK/NDK/AVD/compatibilidade ainda não confirmados. Nenhum programa executado, componente instalado ou teste de gameplay nesta revisão. RetomarAUT-01 e diagnósticoAndroid apenas quando houver limite/disponibilidade informados pelo usuário. Não reler tudo nem reabrirESC/seletor.

Entrega documental: [central de materiais por grupo](../../entregas/README.md) criada para pesquisa/produção independente do usuário. Guias e índice versionados; recebidos locais ignorados até revisão. Na retomada, ler apenas novidades pertinentes antes de implementar. Seletor posteriormente corrigido e aceito; próxima frente proposta é a apresentação mobile testável. Nenhum programa/teste de gameplay executado nesta entrega.

Escudo padrão por habilidade implementado conforme ESC-01 a ESC-07:1% do PV máximo do aplicador, floor e mínimo de1 ponto na aplicação válida,3 oportunidades naturais do protegido e reaplicação por maiores propriedades. Removido na morte do protegido e transições comuns; morte do aplicador não interfere. [Contrato/evidências](ESCUDO-PADRAO.md). Decisões não precisam ser respondidas novamente.

A bateria nativa contém213 testes gerais,6 do adaptador,6 de apresentação,6 clássicos e14 grupos próprios do escudo, em escopos separados. Geometria tem6 grupos anteriores não repetidos nesta revisão. Build Editor aprovado; não houve pacote novo, janela aberta ou teste visual da revisão. Próximo: aceite visual autorizado (VIS-01–03). Silêncio/supressão, bloqueio automático, outras categorias e equipamentos permanecem futuros em [PENDENCIAS.md](PENDENCIAS.md).

## Resultado atual

O protótipo C++20 permanece jogável no console. HAB-01 e CIC-01 estão concluídos no escopo mínimo: habilidades, perfis, seleção de equipe, recompensa única e save v1 de progresso entre batalhas.

A ponte Windows foi compilada e executada na **Unreal Engine 5.8.4**, instalada em `D:\Unreal Engine\UE_5.8`. Dois processos independentes da commandlet própria passaram pelos mesmos **11 checkpoints**, com saída zero e resultado idêntico ao baseline standalone. Essa etapa está concluída.

| Verificação | Evidência atual | Limite |
|---|---|---|
| Regressão do protótipo | 213/213 testes aprovados | Não são 213 testes executados na Unreal |
| Ponte na Unreal | 11 checkpoints em cada uma de duas execuções | Mesmo cenário repetido, não 22 casos distintos |
| Compilador/SDK usados | MSVC 14.52.37029, SDK 10.0.26100.0 | Compilador não preferido; aviso mantido |
| Editor gráfico/GPU | Abertura, mapa, menus e encerramento confirmados; RTX 3060/D3D12/SM5 | Diagnóstico: três testes internos falham em pt-BR e passam em inglês; ENG-01-GRAFICO-DIAGNOSTICO.md |
| Pacote Windows | Build/abertura básica e arena3×3 anterior observados | Interação mais recente apenas compilada; sem Windows limpo ou benchmark |
| Android/iOS | Ainda não validados | Instalar suporte não equivale a validar o jogo |

Primeira inicialização geral atingiu o limite de tempo antes do teste. A rota final por commandlet passou; ambas as evidências foram preservadas. Não há revisão pendente da ponte entregue sem mudança ou falha pertinente.

## Experiência disponível

- `Jogar.cmd`: equipes; `Jogar-Classico.cmd`: clássico com defesa.
- `Jogar.cmd --equipe A1 A3`: seleção anterior à batalha. Catálogo atual A1/A2/A3; suporte a 1–5 validado com dados de teste.
- `salvar`: grava seleção, saldo e conclusões. `--continuar`: carrega e inicia outra batalha, sem retomar HP/turnos.
- Save v1 manual em `build/saves`, ignorado pelo Git. Recompensa provisória: 10 créditos por vitória A/eliminação; zero nos demais finais.
- A2/B2 têm passiva de abertura; A3/B3, habilidade híbrida e proteção condicionada. Time A manual, B com prioridades e básica como alternativa.

## Próximo incremento

Priorizar idioma e região em inglês no ambiente Unreal (`-culture=en` por processo nos testes). A comparação passou sem linhas Error:; pt-BR produziu 14 falhas em três testes internos de mensagens. [Diagnóstico e limites](ENG-01-GRAFICO-DIAGNOSTICO.md). Pacote Windows gerado e execução básica aprovada nesta máquina; [aceite do pacote](ENG-01-PACOTE-RUNTIME.md). VIS-01A concluído: cenário compartilhado e adaptador nativo, com 6/6 novos casos além dos213 anteriores. Cena VIS-01B compilada e fluxo3×3 VIS-01C verificado fora do editor: vitória A após20 ações, proteção condicional, passiva/recarga e reinício. [Resultado e pendências](VIS-01C.md). Próximo: corrigir HP/escudo e descrições/alvos, criar formas conforme elenco e validar1/3/5 e resoluções; aceite visual integral permanece parcial. O jogo poderá ter interface/textos em português por uma camada de localização própria, a definir e validar separadamente; inglês no editor não determina o idioma do produto. Telemetria com opt-out não apresentou os marcadores anteriores; rede continua sem isolamento por decisão do usuário. Nova execução exige autorização específica. A ponte entregue não precisa ser refeita por rotina.

Não assumir que save de progresso é save de combate, que intercalação equivale a threads ou que o perfil de chefe compartilhado cobre todos os modos PvE. Pendências em [PENDENCIAS.md](PENDENCIAS.md), fila em [TAREFAS.md](TAREFAS.md) e [painel](../batalhas/PAINEL-BLOCOS.md).

## Evidências para retomada

- [Resultado standalone](../relatorios/atuais/COMBATE.json) e [6 casos do adaptador](../relatorios/atuais/VIS-01A.json).
- [VIS-01: plano](VIS-01-PLANO.md), [adaptador](VIS-01A.md), [cena](VIS-01B.md) e [aceite parcial no pacote](VIS-01C.md).
- [Compilação da ponte](../relatorios/atuais/ENG-01-BUILD.json).
- [Pacote Windows: build](ENG-01-PACOTE-BUILD.md) e [execução básica](ENG-01-PACOTE-RUNTIME.md): build/cook/archive e os dois processos com saída0; sem benchmark ou Windows limpo validados.
- [Execução Unreal](../relatorios/atuais/ENG-01-RUNTIME.json) e [relatório do aceite](ENG-01-RUNTIME.md).
- [Habilidades](../batalhas/HAB-01-FINAL.md), [ciclo local](CIC-01.md) e [save v1](CIC-01-SAVE-R01.md).

Git: commits locais de entregas autorizados; pushes ficam com o usuário. Não alterar identidade/configuração/histórico. Conferir status antes de editar e comunicar conflitos reais. Resultados datados abaixo delimitam cada incremento.


Apresentação revisada por decisão do usuário: Standard em profundidade como padrão, Classic lateral preservado por -ArenaLayout=Classic. Geometria compartilhada entre C++ e Unreal, cinco posições possíveis por lado; cenário continua3×3. Seis grupos nativos aprovados; inspeção visual da revisão pendente. Ver [VIS-01-LAYOUTS.md](VIS-01-LAYOUTS.md). O aceite anterior não valida a nova câmera.


Apresentações verificadas no pacote em19/11/2026: Standard sem argumento e Classic por chamada explícita, ação/resposta/reinício observados com mesmos valores. Entrada e processo principal de ambas saíram0. Aceite visual integral parcial por sobreposição de etiquetas ao fundo e demais pendências. Relatório: docs/projeto/VIS-01-LAYOUTS-RUNTIME.md. Aplicativos encerrados; próximo ajuste é legibilidade/áreas de seleção.


Contrato inicial de interação: [VIS-01-INTERACAO-R01.md](VIS-01-INTERACAO-R01.md). Implementação e evidências posteriores abaixo prevalecem.


Interação inicial implementada e compilada em19/11/2026: ícone/painéis, faixa de escudo real, gesto prolongado, nível opcional e seleção padrão com marcador/sugestão/confirmação. Regressão213+6 e6 novos grupos aprovados. Ainda sem teste visual desta revisão; cálculo percentual do escudo segue separado dos escudos fixos antigos. Relatório: docs/projeto/VIS-01-INTERACAO-IMPLEMENTACAO.md. Próximo: inspeção autorizada do pacote atualizado.

## Interação clássica — estado vigente em19/11/2026

Compartilhamento implementado: efeitos/escudo/detalhes e orientação de alvos também atendem ao clássico. `-CombatProfile=Classic` escolhe ataque/defesa; `-ArenaLayout=Classic` continua escolhendo só a câmera lateral. Regressão213, adaptador6, apresentação6 e novos grupos clássicos6 aprovados separadamente. Compilação Editor/Win64 aprovada; sem abertura de janela, pacote novo ou aceite visual desta revisão. Próximo: inspeção autorizada e formações/resoluções; escudo percentual e novos efeitos aguardam documento do usuário. [Relatório e roteiro](VIS-01-CLASSICO-INTERACAO.md).

## Pesquisa de habilidades recebida —19/11/2026

Documento do usuário preservado e comparado aos contratos. [Revisão e matriz](REVISAO-PESQUISA-HABILIDADES-ITENS.md). Q07/Q08 e proposta de frentes não substituem R04, defesa atual ou PAR-02. Fontes externas ainda não auditadas; escudo percentual permanece em especificação (base, instante/recalculo, reaplicação e consumo), não mais aguardando envio do documento. Próximo passo: fechar esse contrato mínimo. Sem código/testes novos nesta recepção; aceite visual atual permanece pendente.

## Referência recebida — hub urbano

Quatro capturas associadas ao texto por conteúdo visual em [HUB-01-REFERENCIAS.md](HUB-01-REFERENCIAS.md). Proposta de três áreas, avatar e enquadramentos responsivos separada da evidência estática e da implementação existente. Nenhum hub, login, serviço ou loja implementado/adotado por esta recepção; prioridade executável VIS permanece. Capturas não foram copiadas; o texto recebido foi preservado como referência, sem adotar perfil/recursos como dados do produto.

## HUB-01A — base de apresentação

Política C++ de panorama/recortes e vídeo com proporção, intenção de orientação mobile e composição lateral desktop implementada;10 grupos próprios aprovados. Catálogo local validado com4 Planned/0 Ready. [Relatório e limites](HUB-01A-APRESENTACAO.md). Ainda sem carregamento real de imagem/player, preferências aplicadas à janela ou execução mobile. Nenhuma regra de combate mudou.

Mídias HUB-01A recebidas: panorama JPEG, arte lateral PNG e dois MP4; nomes/dimensões reais cadastrados, autoria própria declarada. [Conferência](HUB-01A-RECURSOS.md). Vídeos não reproduzidos; arquivos permanecem locais e ignorados no Git até estratégia de versionamento. Recortes do terraço e integração de player ainda pendentes.

HUB-01B: dois acessos definidos/cadastrados no panorama atual (Evento Especial no outdoor, Batalha no dispositivo), sem terceiro botão. [Definição e limites](HUB-01B-ACESSOS.md). Renderer/roteador pendentes; nenhuma interface aberta ou alteração da imagem.

Imagem marcada recebida: outdoor com rosto feminino acima do lobo alado→Evento Especial; tela do dispositivo do homem→Batalha. Alvos confirmados e coordenadas atualizadas; referência preservada localmente sem alterar o panorama. Aceite visual e ligação gráfica continuam pendentes.

## HUB-01B — ligação gráfica por código

Parser do catálogo local, panorama único, dois acessos e navegação evento/arena/volta implementados para opção -Hub.10 grupos anteriores e5 novos de projeção aprovados; build Editor registrado sem abrir editor/aplicativo/commandlet.37 entradas preparadas, incluindo JSON e JPEG; sem vídeos ou cook/pacote. [Relatório](HUB-01B-IMPLEMENTACAO.md). Aceite visual/fluxo e integração do player continuam pendentes.

## Hub — tema e decisões visuais vigentes

Cormorant Garamond/Lora confirmadas, Cinzel excluída; LOBO é codinome. Logo WebP recebida localmente, sem importação de runtime. Tema, texto do evento, emblema original, pulso e modo estático implementados;5 grupos novos nativos e build Editor aprovados, sem editor/aplicativo aberto. Preparação39 entradas. Fontes reais/logo, aceites visuais, toque e preferência do sistema continuam pendentes. [Relatório](HUB-01C-VISUAL.md). Esta entrega de hub não encerra VIS-01C da arena.

Atualização vigente da identidade: fontes Cormorant Garamond/Lora e PNG da logo conectados ao HUD; fontes locais com OFL, sem instalação global.44 entradas preparadas; ver [resultado e limites](HUB-IDENTIDADE.md). [Sequência e entregas independentes](PROXIMAS-FASES.md); [percentuais revisados](PROGRESSO-GERAL.md). Esta atualização prevalece sobre menções anteriores de fontes/logo ainda não recebidas.

### Fechamento e próxima entrega

✅ Usuário confirmou texto sem corte, seleção funcional e batalha conforme combinado. Fontes/logo conectadas; aceite integral ainda parcial. ✅ Seletor no chão corrigido e aceito visualmente pelo usuário. Investigar apresentação mobile separadamente; fluidez aguarda critérios. Prévia por Abrir-Preview.cmd; editor por Abrir-Editor.cmd, ambos usando build aprovado. Nesta entrega final só se validaram os lançadores sem abrir programas. [Evidências](HUB-IDENTIDADE.md).

Seletor: correção por linhas3D com profundidade junto à base do modelo implementada e posteriormente aceita visualmente pelo usuário; o assistente não abriu a prévia nessa entrega. [Escopo e roteiro](VIS-SELETOR-CHAO.md). Não altera regras nem resolve mobile/fluidez; não repetir esse aceite sem mudança ou defeito novo.

Mobile: [proposta revisada e sequência mínima](MOBILE-PLANO-REVISAO.md). HUD atual é Canvas, não UMG; preview/renderizador não troca automaticamente DeviceProfile Windows. Próxima implementação a planejar: apresentação Desktop/Mobile/Auto separada de plataforma, antes de presumir migração UMG ou instalar SDK. Sem implementação nova por esta revisão.

Fila independente documentada em [AUT-01–16](FILA-DESENVOLVIMENTO-INDEPENDENTE.md), sem implementação nesta entrega. Inclui trabalho pronto e preparação condicionada; primeiro bloco proposto AUT-01. Não aguardar todos os materiais para avançar nas tarefas prontas.

Recebimento em20/11/2026:7 documentos locais nos grupos00–06; presença/tamanho/SHA-256 registrados em entregas/RECEBIMENTO.csv, sem análise de conteúdo nem integração. Briefing visual alterado pelo usuário para indicar eventos clássicos no acesso Evento Especial, como direção futura; sem mudança de código. Originais recebidos continuam fora do Git.

Nova conferência em20/11/2026:11 arquivos nos grupos00–10;4 novos (07–10),4 alterados (00,01,04,05) e3 sem alteração (02,03,06). Grupo11 sem arquivo recebido. Presença/tamanho/SHA-256 conferidos; conteúdo não analisado nem aplicado. Inventário atual em entregas/RECEBIMENTO.csv; hashes anteriores preservados em entregas/RECEBIMENTO-HISTORICO.csv. Os originais continuam locais, fora do Git; o histórico de hashes não é backup dos conteúdos.

Recebimento final em20/11/2026: documento de Distribuição, Serviços e Direitos (grupo11) presente e registrado. Agora todos os12 grupos00–11 possuem1 documento recebido. Esta conclusão é somente de recebimento; conteúdo ainda não analisado/aprovado/integrado. Originais permanecem locais, fora do Git.

## Respostas ENT recebidas —20/11/2026

**ENT-01–15 respondidos pelo usuário; implementação ainda pendente.** [Decisões vigentes e detalhes por bloco](RESPOSTAS-ENT-2026-11-20.md). PreservarESC/R04/Defesa atual até mudança explícita por implementação. Demo:1 Felipe jogável/1 arena;A2passiva. Pausa/abandono,panmobile,vídeo e futuraeconomia agora têm direção consolidada. Não repetir perguntas principais da revisão anterior;formalizar apenas detalhes técnicos no bloco afetado. Nenhuma execução autorizada por esta atualização.

Ponto1/FelipeA2: regra de cargas,limite,consumo,A1eStandardShield definida;destino confirmado no aliado vivo com menor percentualHP,incluindoFelipe. [Contrato](FELIPE-A2-POSTURA-VIGILIA.md). Não repetir essas perguntas;casos de borda e implementação ficam no bloco futuro. Formação6×6 não implementada por essa decisão.

Ponto2 respondido: Block3%/Michel10% total, teto50% geral e100% especial; True Damage/DoT inelegíveis. Mitigação somente por fonte, aditiva até50%. True Damage ignoraDEF/mitigação/Defender/Block e respeita invencibilidade/escudo. [Contrato e limites](BLOQUEIO-MITIGACAO-DANO-PONTO2.md). Especificado, não implementado; demais perguntas técnicas só no contrato afetado.

Ponto3/SIS-02 definido:trilho de dano separado do trilhoImmunity→chance→ACC/RESIST,piso20%resistência não irresistível;crítico base150%total. Novos sistemas comHP/escudo/dano/scoreint64,teto10^12,intermediáriosdouble/floorporhit,preservandoDefesa1→0 ecenáriospequenos. [Contrato e preparação](FUNIL-ESCALA-NUMERICA-PONTO3.md). Migração/limites/precisão/replay ainda exigem implementação e testes;nenhum código alterado.

Adendo técnico recebido20/11: [matriz vigente](ADENDO-TECNICO-CONTRATOS-2026-11-20.md). Debuff exigehitnãoMiss/nãoBlock;supressão/prevenção e reaçõespendentes detalhadas;SilverShieldcoexiste econsomeapósStandard;ondaslimpamcomunsantesdecurar;scorearredondado uma vez. Temas respondidos não são perguntas abertas de produto;bordas técnicas estão delimitadas no adendo. Implementação ainda pendente.

ConferênciaAndroid20/11 concluída em leitura:Studio D:\Android presente;SdkPath vazio,SDK/NDK/AVD não localizados nas configurações/caminhos examinados. Java25doStudio fora da matriz suportada doGradle8.13/AGP8.13.0daengine. [Resultado e próximos passos](ANDROID-CHECK-2026-11-20.md). Não instalar/configurar/executar automaticamente;buscar SDK personalizado ou preparar componentes/JDK compatível sob autorização. AUTWindows não bloqueado.

**Complemento final recebido20/11:** [A2, ACC/RESIST e ponto fixo](COMPLEMENTO-FINAL-A2-ACC-PONTO-FIXO.md). Bordas de A2 e equação/irresistível respondidos; teto10^12 por saturação; double substituído por ponto fixo de escala10000 no núcleo futuro. Não repetir as perguntas resolvidas. Determinismo bit a bit é meta a verificar, não aceite existente. Nenhum código/teste executado nesta recepção.

SDK localizado após instalação manual emD:\AndroidSDK:NDK/CMake/BuildTools da base presentes. Falta SDKPlatform36 (somente37.0) e confirmarJDK17/AVD. ANDROID-CHECK-2026-11-20.md eANDROID-CHECK.json atualizados;sem execução.

API36 confirmada após nova instalação manual: D:\AndroidSDK\platforms\android-36\android.jar presente, ApiLevel36/revisão2. Pendência da Platform36 resolvida; restam confirmar caminho JDK17 e preparar/validar AVD e build. D:\Java\jdk-17 não localizado; isso não exclui instalação em outro caminho. Nenhuma ferramenta executada.

JDK separado confirmado por metadados: D:\AndroidJava\jdk-17.0.21.1+2, Eclipse Adoptium, Java17.0.21.1, x86_64; java.exe e javac.exe presentes. FamíliaJava17 alinhada ao build planejado; não executado. Pendência de localização doJDK resolvida. Próximo: indicar naUnreal os caminhos SDK D:\AndroidSDK, NDK D:\AndroidSDK\ndk\27.2.12479018 e esteJDK, depois validar toolchain/AVD. Nenhuma configuração realizada pelo assistente;buildAndroid ainda não testado.

Recepção21/11: [novidades e política de versionamento](RECEBIMENTO-NOVIDADES-2026-11-21.md).15Markdown recebidos entram noGit;1novo/1alterado noHub,demais13sem mudança.10PNG locais(8variantes+2indicativos).7destinos do novo panorama são direção futura,não sistemas ativados;AUT-01–04 preservados. Sem código/build/teste ou abertura de programas.

### AUT-11 — retorno de playback e correção de foco

Usuário confirmou que reproduz, com segundo clique indevido. Ajuste de foco de janela preparado e instrumentado; reteste pendente. Backends mobile explicitamente pendentes em PENDENCIAS.md. Nenhum percentual alterado.

AUT-11: reteste confirmou segundo clique só na primeira reprodução. Sem pausa registrada; ajuste de captura inicial do mouse preparado. Reteste de clique único/equipe/arena pendente; não declarar problema encerrado.

AUT-12 (22/11): marcador de runtime implementado, cinco grupos de geometria aprovados; compilação em AUT-12-BUILD.json. Visual/Shipping/terreno pendentes. Próximo AUT-13. Clique inicial: aceite provisório, REVER. Ver docs/projeto/AUT-12-MARCADOR-RUNTIME.md.

AUT-13 (22/11): fila de cues confirmados integrada à sessão, sete grupos próprios aprovados; regressão/build em AUT-13-BUILD.json. Sem animação final, relógio novo ou reaplicação de dano. Próximo AUT-14. Ver AUT-13-EVENTOS-VISUAIS.md.

AUT-14 (22/11): schema e manifestos de referência preparados; sete grupos nativos aprovados. Roster e referências explícitos; sem migração de regras/carga runtime. Pendências em AUT-14-DADOS-CENARIO.md. Próximo AUT-15, somente preparo sob as restrições.

AUT-15 (22/11): stage de 53 entradas preparado e verificado por hash/includes; três grupos do verificador aprovados. Sem build/cook/execução do pacote; Shipping/Windows limpo pendentes. Próximo AUT-16. Ver AUT-15-STAGE-VERIFICAVEL.md.
