# Regras do projeto

## Retomada

Leia README.md, docs/projeto/ESTADO.md e a tarefa ativa em docs/projeto/TAREFAS.md. O painel é docs/batalhas/PAINEL-BLOCOS.md; dúvidas ficam em docs/projeto/PENDENCIAS.md. Consulte visão, arquitetura e decisões apenas quando relevantes. Estado verificado dos arquivos prevalece sobre histórico; não reconstruir decisões relendo chats ou relatórios externos. Histórico anterior de orientações preservado em docs/projeto/HISTORICO-ORIENTACOES.md.

## Escopo e segurança

RPG original, urbano e musical; C++20/MSVC x64, engine ainda não escolhida. Não acessar instalações de jogos, launchers, contas ou conteúdo protegido; não copiar recursos proprietários. Usar recursos próprios ou licenciados, registrando origem. Não contornar proteções. A autorização pontual histórica de investigação externa não amplia o escopo atual.

Dentro da tarefa autorizada, editar arquivos próprios e executar verificações pertinentes. Instalações, downloads, escolha definitiva de engine, serviços, publicação e operações destrutivas exigem decisão específica. Não habilitar integrações de IA/API ou envio de dados implicitamente. Não modificar PATH global, política de scripts ou código de engine.

## Unidade de trabalho e consumo

Uma entrega funcional por bloco com aceite explícito. Não usar subagentes sem solicitação. Não executar todos os blocos numa entrega. Preferir leituras seletivas e automação local; não repetir baterias sem mudança/falha pertinente. Não criar testes para simples documentação. O agente não conhece saldo de uso; a reserva de 30% é acompanhada pelo usuário, sem promessas de estimativa/parada automática.

## Estrutura e engenharia

Siga CONTRIBUTING.md. Interfaces em include/rpg, implementações em src, entradas em app, testes em tests, utilitários próprios em tools e automação em scripts. Separe regras de apresentação, áudio, persistência e serviços; use IDs estáveis e dados/saves versionados. Binários/objetos/logs ficam em build ignorado. Relatórios consolidados: docs/relatorios/atuais; aceites históricos: docs/batalhas.

Poucas dependências/plugins, somente por necessidade demonstrada e procedência/licença verificadas. Não instalar ferramentas antecipadamente. Migração de engine exige avaliação isolada e regressão, sem garantir ausência de retrabalho. Não escolher preview só por ser recente.

## Comandos e Git

Compilar/testar: .\scripts\testar-combate.ps1. Diagnóstico de ambiente: .\scripts\verificar-ambiente.ps1, somente quando necessário. Não usar bypass de política. Jogar.cmd abre equipes, Jogar-Classico.cmd abre clássico; ambos dependem de build/combate/jogar.exe local. Scripts descobrem VS instalado com vswhere e usam VsDevCmd em processo filho; não instalam nada.

Git local com origin cadastrado pelo usuário. Push somente quando solicitado explicitamente; o usuário normalmente faz os pushes; não alterar identidade/configuração/histórico ou fazer push automático. Confira status antes de editar, preserve mudanças dele e avise se conflitarem. Commits locais de entregas concluídas/testadas estão autorizados. Não commitar conteúdo externo, segredos ou binários gerados.

## Estado funcional vigente

HAB-01 e CIC-01 estão concluídos no escopo mínimo. São 213 testes gerais standalone aprovados; a ponte também passou 11 checkpoints em 2 processos Unreal independentes, saída0 e resultado igual ao baseline. Não somar essas medidas nem afirmar que a suíte inteira rodou na engine.

Unreal 5.8.4/CL58902052 em D:\Unreal Engine\UE_5.8. Build validado com MSVC14.52.37029/SDK26100, mantendo aviso de compilador não preferido. Fonte do experimento em experiments/unreal/RpgEngineProbe, stage gerado em build/engine-eval. Script de build confere 18 hashes e compila o baseline antes do UBT. Runtime usa commandlet própria (-run=RpgCoreProbe), sem startup opt-in simultâneo, interface gráfica ou saves do jogador; limite180s por processo. Ferramentas podem criar caches/traces normais fora do projeto, mediante as permissões necessárias. Tentativa inicial de inicialização geral teve timeout e está preservada como não aprovada.

A ponte Windows está concluída. Abertura gráfica básica confirmada em RTX3060/D3D12/SM5. ENG-G01 identificado em três testes internos de mensagens: 14 falhas em pt-BR, zero linhas Error: na comparação em inglês; ambas as saídas0. Priorizar inglês no idioma/região do ambiente Unreal, usando -culture=en nos testes por processo; não mudar Windows ou preferências globais implicitamente. Esta é uma mitigação observada, não correção da engine. Idioma do editor e do jogo são decisões separadas: o produto poderá ser em português via localização a definir/implementar/validar (textos traduzíveis, fontes, formatação e testes no pacote). ENG-G02: opt-out bSendUsageData=False sem marcadores anteriores, sem garantia de ausência de tráfego; rede não isolada por decisão do usuário. Evidências em docs/projeto/ENG-01-GRAFICO-DIAGNOSTICO.md. Pacote Windows Development básico aprovado: build/cook/archive e abertura/renderização/encerramento nesta máquina, saída0 no build e nos dois processos; ENG-01-PACOTE-RUNTIME.md. Stage novo de pacote em build/p (22 entradas com hashes); engine-eval anterior preservado. Scripts preparar-pacote-unreal.ps1 só prepara; empacotar-unreal.ps1 executa build autorizado, sem abrir aplicativo. Falha inicial de caminhos longos preservada. Avisos MSVC/ACL e espera Zen registrados. Gameplay, Windows limpo, desempenho, distribuição, arena/mobile e localização continuam pendentes. VIS-01A concluído com6 casos novos além dos213 anteriores. Cenário em reference_encounter.cpp; console e VisualSession usam os mesmos dados. VisualSession controla revisão de comandos e avanço automático limitado. VIS-01B/C: fluxo3×3 verificado no pacote, vitória/reinício/saídas0; aceite integral parcial, pendências de leitura/alvos e1/5/resoluções em VIS-01C.md. Não repetir suíte por documentação. Preparação do pacote atual inclui28 arquivos; pacote básico anterior preservado. Próximo: fechar essas pendências antes de arte/áudio. Futuro uso de programas exige autorização específica; esta entrega autoriza PowerShell/Git para documentação e commit, sem editor. Não instalar componentes, alterar rede ou iniciar tarefas por inferência.

Núcleo continua em src/include; não editar cópias geradas nem ligar objetos /MT do console à engine. Codec do save é separado do I/O Windows. CIC-01 usa seleção de 1–5 IDs (demo A1/A2/A3), recompensa única provisória e save manual v1 entre batalhas, em build/saves ignorado. Carregar valida equipe/progresso e inicia nova tentativa, sem restaurar batalha nem reapurar histórico. Limites 1 MiB/4096 registros, sem concorrência/autenticação/migração implícita.

Silêncio/supressão, passivas durante combate, custos/condições compostas, inventário, save de runtime, arena e plataformas adicionais não estão validados. Block não é silêncio; FreezeCooldowns não é congelamento da unidade. Estado e próxima ação em docs/projeto/ESTADO.md e TAREFAS.md; histórico documental anterior à consolidação em docs/projeto/historico/2026-11-18/.

Contratos TIME/RES/SEQ/PAR permanecem válidos no escopo de cada relatório. R04 é contrato próprio, não paridade oficial. Não confundir intercalação cooperativa com threads/servidor, nem perfil experimental de chefe compartilhado com todos os modos PvE. Área/dinâmica/score/fases autônomas por HP continuam lacunas explícitas. Defesa atual arredonda dano/2 para baixo por hit.

## Encerramento

Atualize estado, tarefa e painel pertinentes com resultado, verificações efetivamente executadas, limites e próximo passo. Não duplicar o relatório completo entre documentos. Português nos documentos e explicações, inglês técnico consistente no código. Nunca declarar teste, compilação ou recurso funcional que não ocorreu.


## Apresentações da arena

StandardDepth é o padrão visual: aliados próximos da câmera e inimigos ao fundo, com cinco posições possíveis em leque escalonado, conforme referência visual do usuário. ClassicLateral preserva a composição lateral, selecionada por -ArenaLayout=Classic. Definição neutra em arena_layout.hpp/cpp, usada pela Unreal;5 posições possíveis por lado, sem criar personagens novos. Perfil visual é separado de CombatProfile; não habilitar defesa no perfil Equipes por escolher câmera clássica. Jogar.cmd/Jogar-Classico.cmd preservados. Testes nativos e build não equivalem ao aceite visual da nova câmera. Testar posições: scripts/testar-layout-arena.ps1 (6 grupos), sem abrir a Unreal. Retrato ativo tem só um painel provisório; UI específica de chefe/auxiliares ainda pendente. Ver docs/projeto/VIS-01-LAYOUTS.md. Não abrir janela por inferência desta atualização.

Interação gráfica: `-CombatProfile=Classic` seleciona regras clássicas (referência sem loadouts/especiais), independentemente de `-ArenaLayout=Classic`. Não confundir câmera lateral com defesa habilitada. Efeitos/detalhes/escudo em pontos reais compartilhados; defesa também aparece na informação. Mais6 grupos clássicos nativos aprovados, separados dos213+6 adaptador+6 apresentação. Build Editor aprovado, sem abrir janela/pacote desta revisão. Ver VIS-01-CLASSICO-INTERACAO.md. Escudo padrão implementado conforme ESCUDO-PADRAO.md; não converter escudos fixos históricos implicitamente.

Pesquisa de habilidades recebida e revisada. Perguntas novas vivem somente em docs/projeto/PENDENCIAS.md (ESC/VIS/REG/SIS), com estados próprios. ESC-01–07 já definidos/implementados para o caso comum; não pedir respostas novamente. Preservar fonte recebida e contratos encerrados. Não repetir a lista em outros documentos.

StandardShield é novo tipo explícito: max(1,floor(max HP do aplicador/100)),3 turnos naturais, max de pontos/duração independentes; mínimo apenas na aplicação válida. Não avança em extras mesmo se a fonte pede tick; oportunidade natural impossibilitada de agir conta. Remover na morte e transições comuns; não em alternância de frentes. Shield fixo e StructuralShield históricos preservados.14 grupos próprios além dos213+6+6+6; build Editor aprovado, sem pacote/aceite visual. Bloqueio automático probabilístico continua futuro; Block técnico atual significa impossibilitado de agir. Contrato: docs/projeto/ESCUDO-PADRAO.md.

HUB-01A: startup_presentation é política neutra de recortes/proporção e pedidos de orientação;10 testes próprios. data/presentation/startup.json é catálogo previsto validado por script, sem parser/runtime/player ligados. Não afirmar rotação real, imagem carregada única ou configuração de janela aplicada antes de integrar e testar. Preparador atual inclui34 entradas. Novos recursos em assets exigem origem/direitos, sem copiar Dislyte nem instalar tecnologia por antecipação.

HUB-01B escreveu parser do subconjunto hub, textura única e rotas locais em ProbeHub.cpp, ativadas por -Hub. Mesmo VisualSession ao voltar/reentrar; sem reset/recompensa por navegação.5 grupos novos de projeção aprovados além dos10 de apresentação. Preparador agora37 entradas (inclui JSON/JPEG local, não vídeo); Json é dependência da engine. Compilar não valida carga/clique/fluxo; não executar editor, commandlet ou pacote por inferência. Próximo aceite em HUB-01B-IMPLEMENTACAO.md. Janela antes da abertura/player/rotação nativa continuam pendentes.

Hub tema: Cormorant Garamond principal/Lora complementar, Cinzel excluída; LOBO codinome. Decisões não precisam ser perguntadas novamente. Logo WebP recebida em assets/sources, não integrada; fontes reais pendentes. hub_style tem5 grupos de pulso/movimento reduzido/alvo mínimo aprovados; build Editor22 ações aprovado sem runtime, stage c6d1b900. Preparador39 entradas. -ReducedMotion força apresentação estática; não há leitura automática do sistema/persistência. Ver HUB-01C-VISUAL.md; não confundir com aceite VIS-01C da arena.

Atualização vigente da identidade: fontes Cormorant Garamond/Lora e PNG da logo conectados ao HUD; fontes locais com OFL, sem instalação global.44 entradas preparadas; ver [resultado e limites](docs/projeto/HUB-IDENTIDADE.md). [Sequência e entregas independentes](docs/projeto/PROXIMAS-FASES.md); [percentuais revisados](docs/projeto/PROGRESSO-GERAL.md). Esta atualização prevalece sobre menções anteriores de fontes/logo ainda não recebidas.

Fechamento: usuário confirmou texto final sem corte, seleção e batalha; seletor no chão foi corrigido e aceito posteriormente pelo usuário. Mobile não carregou apresentação esperada, causa aberta; fluidez sem contrato novo. Abrir-Preview.cmd e Abrir-Editor.cmd leem o último HUB-IDENTIDADE-BUILD aprovado; não executar por inferência. Relato e limites em HUB-IDENTIDADE.md; resumo HUB-IDENTIDADE-RUNTIME.json. Percentuais revisados, sem novo aumento neste fechamento.

## Central de entregas do usuário

Entrada em entregas/README.md; grupos00–11 têm guias e subpastas recebidos locais ignoradas no Git. Materiais anteriores continuam em assets. Na retomada, inventariar novidades por área, preservar originais, ler somente o necessário, comparar com PENDENCIAS/contratos e distinguir referência, hipótese, proposta e decisão do usuário. Não executar anexos, instalar ferramentas ou promover dados automaticamente. Atualizar entregas/CONTROLE.csv após conferência; o índice inicial não comprova recebimento/revisão. Guias não reabrem regras encerradas nem aumentam percentuais de funcionalidade. Seletor encerrado no protótipo; planejamento mobile é a frente seguinte.

Seletor corrigido em ProbeArena.cpp: DrawDebugLine com profundidade de mundo, base pelos bounds do modelo; círculo/losango preservados. Recurso provisório Development, precisa material/decal/malha antes de Shipping. Aceite visual posteriormente confirmado pelo usuário; não repetir por inferência. Lançadores usam PREVIEW-BUILD.json quando existir, com fallback HUB-IDENTIDADE-BUILD; novo build do marcador em VIS-SELETOR-BUILD.json. Ver VIS-SELETOR-CHAO.md; mobile/fluidez separados.

Mobile: consultar MOBILE-PLANO-REVISAO.md. HUD Canvas atual, não UMG. DeviceProfile usa macros de plataforma; preview gráfico/resize Windows não ativa Mobile do código. Sem instalações/configurações adotadas por revisão de pesquisa. Seletor no chão aceito visualmente pelo usuário; não tratar como defeito aberto. Próximo passo sugerido é modo de apresentação testável, sem decidir migração total ou serviços/lojas implicitamente.

Fila independente em docs/projeto/FILA-DESENVOLVIMENTO-INDEPENDENTE.md: AUT-01–16 apenas planejados. Primeiro candidato AUT-01 Desktop/Mobile/Auto; não executar múltiplos blocos por inferência. Tarefas prontas não aguardam coleta inteira; preparação condicionada não autoriza inventar semânticas REG/SIS, instalar ferramentas ou declarar aceite mobile. ESC/seletor preservados; percentuais não sobem por plano.
