# Regras do projeto

## Escopo

Jogo original de RPG por turnos com identidade urbana e musical. Nome de trabalho: RPG Urbano. Este repositório é independente da investigação anterior. Não acessar instalações de jogos, launchers, atalhos, pacotes protegidos ou dados de contas; não copiar conteúdo proprietário. Usar recursos próprios ou com licença registrada.

## Como retomar

Leia ESTADO.md e a tarefa ativa em TAREFAS.md. Consulte VISAO.md para escopo e ARQUITETURA.md/DECISOES.md somente quando relevantes. Não reabrir histórico de chat ou relatórios externos para reconstruir decisões já registradas. O estado atual dos arquivos e os resultados de verificação prevalecem sobre resumos desatualizados.

## Unidade de trabalho

Uma entrega funcional por bloco, com objetivo e aceite explícitos. Concluir implementação e verificações pertinentes, atualizar ESTADO.md e TAREFAS.md, e parar no limite do bloco autorizado. Não abrir frentes paralelas ou usar subagentes sem solicitação. Não transformar uma tarefa pequena em reformulação geral.

## Consumo

Ler arquivos e emitir resultados seletivamente. Preferir automação em lote para processamento repetitivo. Não repetir testes sem mudança ou falha que o justifique; não executar bateria completa a cada edição. Para regras de combate, verificar comportamentos e invariantes relevantes. Não criar testes para simples mudanças documentais.

O agente não conhece o saldo real do usuário. Reserva de 30% é uma preferência operacional acompanhada pelo usuário, não um limite automático. Não estimar percentuais consumidos ou prometer parada automática. Ao encerrar, registrar próximo passo executável e limitações.

## Autonomia e aprovações

Em tarefa autorizada, editar código/documentos próprios e executar verificações locais pertinentes sem pedir aprovação a cada arquivo. Instalações, escolha definitiva de engine, downloads grandes, serviços externos, publicação e operações destrutivas exigem decisão específica. Não instalar engine ou ferramentas nesta fundação. Não executar software de terceiros para investigar jogos.

## Engenharia

Separar regras de combate de apresentação, áudio, serviços e persistência. Usar IDs estáveis; dados e saves versionados. Não modificar código-fonte da engine sem decisão registrada. Poucos plugins, com finalidade e licença documentadas. Preservar arquivos-fonte editáveis dos recursos criados.

Mudanças de engine/bibliotecas precisam de uma cópia ou ramificação de avaliação e critérios de regressão; migração sem retrabalho não é garantida. Não adotar preview por ser mais recente. Não habilitar integrações de IA/API com cobrança ou envio de dados implicitamente.

## Entrega

Resumo curto: resultado, verificação, risco/limite e próximo passo. Não declarar compilação, execução ou teste que não ocorreu. Usar português nas explicações e documentos. Código e IDs técnicos podem usar inglês consistente.

## Ferramentas e comandos

Git está disponível; repositório somente local, sem remoto. AMB-01 concluída após instalação manual de Visual Studio 2026 pelo usuário. Núcleo previsto: C++20/MSVC x64; engine ainda não escolhida.

Comando validado na raiz: `.\scripts\verificar-ambiente.ps1`. Compila e executa somente tools/diagnostico.cpp; objetos, executável e logs ficam em build/diagnostico/ (ignorado), versões/resultado em AMBIENTE.json. Descobre VS com vswhere e inicializa VsDevCmd somente no processo filho. Não instala ferramentas nem modifica PATH global. Não é teste de gameplay. Reexecutar apenas se houver mudança de ambiente ou falha que o justifique.

Versões validadas: Visual Studio Community 2026 18.12.1, MSVC 14.52.37025, Windows SDK 10.0.26100.0. Compatibilidade com futura engine precisa ser verificada; não presumir suporte apenas por ser recente.

COM-01, COM-01A e TIME-01 concluídas. Comando validado: `.\scripts\testar-combate.ps1` (13 testes originais + 11 de orçamento local + 9 de fases + 16 grupos de tempo, 168 cenários/replays + 25 formações/replays AP, isolamento intercalado, lote funcional local e demo). Resultado em COMBATE.json; matriz em docs/batalhas/formacoes-testadas.csv; log ignorado em build/combate/resultado.log. Não repetir sem mudança/falha pertinente. Demo compilada: `.\build\combate\simular.exe`. Núcleo ainda não é interativo.

O usuário pediu um documento separado para dúvidas e limitações: **PENDENCIAS.md é o registro central**. Atualizar IDs/estado quando uma questão for resolvida, referenciar em vez de duplicar perguntas em vários arquivos e não trazer toda a lista a cada entrega. Consultar só as pendências necessárias ao bloco atual.

COM-02 permanece em espera. WAVE-01 concluída; pesquisa de tempo e experimento 11/11 concluídos. TIME-02 integrada (docs/batalhas/TIME-02.md); próxima entrega SEQ-01, depois PAR-01/PAR-02. Ler ESTADO.md e a seção relevante dos contratos; pesquisa histórica permanece em docs/batalhas/ANALISE.md/fontes.json. Não atribuir causas técnicas sem evidência primária. Phase::Finished pode ser empate ou derrota por limite; consultar winner/end_reason. Stage implementa orçamento global para confrontos consecutivos; orçamento entre frentes ainda pendente. Não confundir intercalação com threads ou chefe compartilhado.



Perfis atuais: FixedRounds preserva referência/default; ActionBar é o perfil prioritário experimental. Não interpretar round() como contador geral no AP. Fases AP/habilidades exigem RuntimeCarry explícito; TechnicalLimit não concede vencedor. Pesquisa/experimento anteriores não são paridade comprovada com Dislyte.


Atualização após contrato R04: sequência executável TIME-03 (recarga/origens de extras), RES-01 (causalidade mínima), SEQ-01 (três times), depois PAR. Referência de prontidão em docs/batalhas/VALIDACAO-CONTRATOS-R04.md. As decisões prioritárias estão fechadas; funcionalidades novas não foram testadas por baterias anteriores.


Prioridade atual do usuário: revisão do implementado primeiro. PLANO-PRIORIDADES.md ordena áreas/dependências e entregas menores por esforço relativo. Próxima entrega: TIME-03/recarga (primeira parte), mantendo TIME-03 aberto até os três aceites. Não iniciar toda a sequência de uma vez.


TIME-03 parte 1 concluída: novo ciclo de recarga reduz no próprio uso elegível, com 53 testes totais aprovados. Regra vigente e fronteira intermediária em docs/batalhas/TIME-03.md. Próxima entrega: parte 2 (origem/flags de extras). Não repetir diagnóstico ou parte 1 sem mudança pertinente.


TIME-03 partes 1/2 concluídas; 60 testes totais aprovados, 168 cenários e 25 formações AP preservados. Próxima entrega parte 3: controle versus congelamento de recarga. Fonte explícita de extra governa flags naquela oportunidade; ausência conserva relógios legados. Contrato em docs/batalhas/TIME-03.md.


TIME-03 concluído nas três partes: 67 testes aprovados e matrizes preservadas. Próxima entrega RES-01/parte 1 (identidade/instância de ação, multi-hit e guarda por habilidade). FreezeCooldowns é independente de Block e impede decremento final antes da própria expiração, prevalecendo sobre flags dos extras. Não reabrir TIME-03 sem mudança pertinente.


RES-01 parte 1 concluída (76 testes): ActionContext, multi-hit e guarda por habilidade. Próxima entrega parte 2: reflexão e FIFO de mortes; parte 3 fará prevenção/revive/Victory Lock. IDs locais ao confronto; eventos de dano identificam ação/acerto. Detalhes em docs/batalhas/RES-01.md. Não repetir testes após mudanças só documentais.


RES-01 partes 1/2 concluídas (84 testes), próximo parte 3: prevenção/auto-revive/Victory Lock. Magnitudes de reflexão/explosão são fixas do laboratório, consequências não refletem/geram Counter/usam guarda entre si. Seleção dinâmica LowestHealthEnemy determinística; sem RNG. Stage NoSurvivors impede iniciar onda com time morto em PreserveHealth. Contrato em docs/batalhas/RES-01.md.


RES-01 concluído nas três partes (95 testes totais, matrizes preservadas); próximo SEQ-01, coordenador de três times/chefe por etapa. Prevenção/revive próprios usam cargas finitas; Victory Lock consulta sobreviventes depois dos revives. Não retoma golpes/ações cancelados. Cobertura R04 no perfil mínimo em docs/batalhas/RES-01.md; não alegar catálogo completo ou bounce aleatório.


SEQ-01 parte 1 concluída (103 testes): SequentialSquads, três times de 1–5, HP do chefe conservado, kit estável, política obrigatória de recargas/limpeza total de status. Próxima parte 2: perfis/estados do chefe por categoria. Não alegar matriz final de estados estruturais/permanentes nem score implementados. Relatório docs/batalhas/SEQ-01.md.


Git passou a ter commit inicial do usuário e origin. Pushes ficam com ele; não alterar identidade/configuração/histórico nem executar push automaticamente. Avisar sobre conflitos que afetem a entrega. SEQ-01 parte 2 concluída (112 testes): perfis, mapa de cooldown, categorias e escudos. Próxima parte 3, validação integrada sem novas famílias. Relatório SEQ-01.md.


SEQ-01 encerrado nas três partes: 115 testes, 17 sequenciais; A/A/B e A/A/A, cargas persistentes, término único e replay. Próximo PAR-01, uma entrega por vez. Usuário autorizou commit local desta entrega; não fazer push. Painel consolidado em docs/batalhas/PAINEL-BLOCOS.md.


PAR-01 parte 1 concluída: 120 testes, cinco grupos de frentes; alternância cooperativa determinística, isolamento e agregado/replay. Bloco aberto. Próximo parte 2 orçamento global/cancelamento; parte 3 integração técnica/R04. Relatório docs/batalhas/PAR-01.md. Painel contém legenda das siglas.


PAR-01 parte 2 concluída: 125 testes, 10 grupos de frentes. Orçamento global independente dos locais, eliminação final antes de timeout, cancelamento idempotente preserva Battle e expõe FrontState. Próximo parte 3: integração R04/habilidades/Skip/falha técnica; não marcar bloco concluído ainda. Relatório docs/batalhas/PAR-01.md.


PAR-01 encerrado nas três partes: 131 testes, 16 grupos de frentes; integração R04, Skip, reações, extras e falha técnica antes de timeout. Próximo PAR-02: fechar modelo compartilhado/PEN-05 antes de implementar; PAR-01 não escolhe HP único/partes/avatares. Não iniciar UI/engine/servidor junto. Relatório docs/batalhas/PAR-01.md.


Comparação de 18/11/2026 concluída por autorização explícita e pontual do usuário para leitura de analise-dislyte e cópia de testes. Nenhuma execução de componentes, extração, rede ou escrita na origem. Matriz docs/batalhas/comparacao-dislyte-2026-11-18/; 8 metadados coincidentes e 36 referências de plugins. Não repetir investigação sem motivo; autorização pontual não muda o escopo independente do projeto. Próximo PAR-02/PEN-05; 131 testes preservados sem reexecução.


PAR-02: proposta documental em docs/batalhas/PAR-02.md, sem código/testes novos. Usuário recebeu perguntas sobre modelo (HP único/partes/avatares) e cadência (global/por frente). HP único + barra global são recomendações, não decisões. Não implementar até respostas/contrato; PEN-05 aberta. Preservar 131 testes aprovados.


Escolhas recebidas: HP único + barra global, ataques comuns alternando frentes vivas. PAR-02 parte 1 implementada SharedBoss (um Battle autoritativo, 3 times de 1–5, AP/SPD, rotação e fluxo básico). 136 testes, 5 grupos novos. Habilidades rejeitadas neste perfil até parte 2: seletores/status/recargas/R04 compartilhados. PEN-05 resolvida; não perguntar de novo. Parte 3 integração final. PAR-02 segue aberto; não iniciar UI/engine/servidor junto.


Famílias PvE organizadas em MODOS-PVE.md e modos-pve.csv/json (12 entradas). Distinguir relato, história, teste e decisão autoral. Treva Fluxo é requisito de sobrevivência/score a detalhar; exemplo dos Milagres não identificado. PAR-02 experimental não representa todos os chefes. Antigos cenarios/mecanicas atualizados com histórico preservado. Roteiro ROTEIRO-PVE-CHEFES.md para futura pesquisa. Próximo PAR-02 parte 2; não ampliar silenciosamente para score/retorno cíclico/fases HP. Nenhum teste novo nesta reorganização.


PAR-02 parte 2 concluída: 142 testes, 11 grupos compartilhados. Self/OwnSquad/Boss/RotatingFront explícitos, apenas alvo direto único; dinâmica do chefe rejeitada. DeathExplosion mantém escopo de lado do kernel e pode atingir três times a partir de um gatilho. Relógios/cargas únicos, Skip não gira alvo. Próxima parte 3 integração orçamento/falha técnica/reações/overkill/fim/replay. Sem score/UI/engine, kernel inalterado. Contrato PAR-02.md.


PAR-02 perfil HP/AP único encerrado: 148 testes totais, 17 grupos compartilhados, nenhuma mudança de produção na parte 3. Próximo COM-02 interação textual. Não dizer que fases por HP/turno ou kit global foram implementadas: lacuna histórica separada nas tarefas futuras/PVE-09; área/dinâmica/score também pendentes. Falha na cadeia precede timeout; limite futuro de agendamento não é falha retroativa. Relatório PAR-02.md; matrizes e arquivos anteriores preservados.


COM-02 concluído, 155 testes totais/7 grupos textuais. .\scripts\jogar.ps1 executa binário próprio build/combate/jogar.exe; compilado por testar-combate.ps1. Controle A/AI básica B, default 3x3 AP teto200, sem skills equipadas/menu SEQ/PAR. Skills via comando integradas/testadas em configs de sessão. Kernel inalterado. Próximo HAB-01: uma família adicional por incremento. Contrato docs/batalhas/COM-02.md.


Entrada recomendada para usuário Windows: Jogar.cmd na raiz (ou build/combate/jogar.exe direto). Windows PowerShell comum bloqueou .ps1 por política; não alterar segurança nem usar bypass. CMD e executável direto via Windows PowerShell validados. Alteração somente de entrada/documentos, sem repetir 155 testes. Próximo HAB-01.


Plano HAB-01 atualizado após direção do usuário: padrão de equipes com até três posições e clássico preservado; separar menu/AP-SPD/manual-auto. Passivas e componentes híbridos precisam de gatilhos reais, sem rótulo fictício. Cinco partes em docs/batalhas/HAB-01.md; próxima parte 1 perfis/posições/contrato. Esta entrega só documental, sem mexer em Jogar.cmd ou repetir 155 testes. Pesquisa oficial pública limitada não certifica regras atuais completas; pedir amostras pontuais só quando necessárias.


Relatório de habilidades recebido: revisão docs/batalhas/REVISAO-HABILIDADES-USUARIO-R01.md. Usar como referência recebida, sem assumir patches/fichas verificados ou copiar roster pessoal. Suficiente para HAB-01 parte 1; passivas podem ter recarga interna e componentes híbridos. Não retomar busca extensa no outro chat (usuário interrompeu para economizar limite), nem pedir extração completa. Protótipo e 155 testes inalterados.


Contrato HAB-01-PERFIL-EQUIPE-R01.md recebido: padrão até3posições, básica A1 não silenciável/disponível se apto, passivas não clicáveis. CD/inatividade/condição separados. FreezeCooldowns não é congelar personagem; matriz silêncio/passivas/petrificar pendente, não inferir. Só terminologia defesa implementada no texto/demo, guardar alias oculto; 7 testes locais e demo aprovados sem repetir155. Clássico/itens/especial não ampliados. Próximo implementar HAB-01 parte1 com contrato fechado.
