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

Git local com origin cadastrado pelo usuário. Ele faz os pushes; não alterar identidade/configuração/histórico ou fazer push automático. Confira status antes de editar, preserve mudanças dele e avise se conflitarem. Commits locais de entregas concluídas/testadas estão autorizados. Não commitar conteúdo externo, segredos ou binários gerados.

## Estado funcional vigente

HAB-01 e CIC-01 estão concluídos no escopo mínimo. São 213 testes gerais standalone aprovados; a ponte também passou 11 checkpoints em 2 processos Unreal independentes, saída0 e resultado igual ao baseline. Não somar essas medidas nem afirmar que a suíte inteira rodou na engine.

Unreal 5.8.4/CL58902052 em D:\Unreal Engine\UE_5.8. Build validado com MSVC14.52.37029/SDK26100, mantendo aviso de compilador não preferido. Fonte do experimento em experiments/unreal/RpgEngineProbe, stage gerado em build/engine-eval. Script de build confere 18 hashes e compila o baseline antes do UBT. Runtime usa commandlet própria (-run=RpgCoreProbe), sem startup opt-in simultâneo, interface gráfica ou saves do jogador; limite180s por processo. Ferramentas podem criar caches/traces normais fora do projeto, mediante as permissões necessárias. Tentativa inicial de inicialização geral teve timeout e está preservada como não aprovada.

A ponte Windows está concluída. Abertura gráfica básica confirmada em RTX3060/D3D12/SM5. ENG-G01 identificado em três testes internos de mensagens: 14 falhas em pt-BR, zero linhas Error: na comparação em inglês; ambas as saídas0. Priorizar inglês no idioma/região do ambiente Unreal, usando -culture=en nos testes por processo; não mudar Windows ou preferências globais implicitamente. Esta é uma mitigação observada, não correção da engine. Idioma do editor e do jogo são decisões separadas: o produto poderá ser em português via localização a definir/implementar/validar (textos traduzíveis, fontes, formatação e testes no pacote). ENG-G02: opt-out bSendUsageData=False sem marcadores anteriores, sem garantia de ausência de tráfego; rede não isolada por decisão do usuário. Evidências em docs/projeto/ENG-01-GRAFICO-DIAGNOSTICO.md. Pacote Windows Development básico aprovado: build/cook/archive e abertura/renderização/encerramento nesta máquina, saída0 no build e nos dois processos; ENG-01-PACOTE-RUNTIME.md. Stage novo de pacote em build/p (22 entradas com hashes); engine-eval anterior preservado. Scripts preparar-pacote-unreal.ps1 só prepara; empacotar-unreal.ps1 executa build autorizado, sem abrir aplicativo. Falha inicial de caminhos longos preservada. Avisos MSVC/ACL e espera Zen registrados. Gameplay, Windows limpo, desempenho, distribuição, arena/mobile e localização continuam pendentes. Próximo: planejar VIS-01. Futuro uso de programas exige autorização específica; esta entrega autoriza PowerShell/Git para documentação e commit, sem editor. Não instalar componentes, alterar rede ou iniciar tarefas por inferência.

Núcleo continua em src/include; não editar cópias geradas nem ligar objetos /MT do console à engine. Codec do save é separado do I/O Windows. CIC-01 usa seleção de 1–5 IDs (demo A1/A2/A3), recompensa única provisória e save manual v1 entre batalhas, em build/saves ignorado. Carregar valida equipe/progresso e inicia nova tentativa, sem restaurar batalha nem reapurar histórico. Limites 1 MiB/4096 registros, sem concorrência/autenticação/migração implícita.

Silêncio/supressão, passivas durante combate, custos/condições compostas, inventário, save de runtime, arena e plataformas adicionais não estão validados. Block não é silêncio; FreezeCooldowns não é congelamento da unidade. Estado e próxima ação em docs/projeto/ESTADO.md e TAREFAS.md; histórico documental anterior à consolidação em docs/projeto/historico/2026-11-18/.

Contratos TIME/RES/SEQ/PAR permanecem válidos no escopo de cada relatório. R04 é contrato próprio, não paridade oficial. Não confundir intercalação cooperativa com threads/servidor, nem perfil experimental de chefe compartilhado com todos os modos PvE. Área/dinâmica/score/fases autônomas por HP continuam lacunas explícitas. Defesa atual arredonda dano/2 para baixo por hit.

## Encerramento

Atualize estado, tarefa e painel pertinentes com resultado, verificações efetivamente executadas, limites e próximo passo. Não duplicar o relatório completo entre documentos. Português nos documentos e explicações, inglês técnico consistente no código. Nunca declarar teste, compilação ou recurso funcional que não ocorreu.
