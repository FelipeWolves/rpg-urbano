# ENG-01 — diagnóstico regional dos testes e telemetria

19/11/2026. Execuções autorizadas somente por PowerShell e Unreal Editor. Sem alterações da rede, firewall, Windows, fontes da engine ou regras do jogo. Na execução dos testes não foi usado Git. A consolidação documental e o commit local foram autorizados posteriormente.

## Procedimento

Mantidos projeto/mapa, GPU e região pt-BR na primeira execução; acrescentados registro `LogAutomationTest Log` e override de processo `-ini:EditorSettings:[/Script/UnrealEd.AnalyticsPrivacySettings]:bSendUsageData=False`. O manifesto de 18 arquivos canônicos/stage foi conferido antes da primeira abertura; não houve mudança de fonte ou recompilação entre execuções.

Na segunda execução, mesmos argumentos mais `-culture=en`. Idioma e região em inglês foram confirmados pelo log e interface. Não foram alteradas preferências globais de idioma ou rede. O editor pode produzir caches/configurações normais; não foi feita comparação completa de todas as preferências gravadas pela ferramenta.

Ambas carregaram visualmente Template_Default e encerraram normalmente por Alt+F4, com saída 0 capturada pelo processo PowerShell. Inicialização da engine: 13,44s em pt-BR e 14,89s em inglês; caches já preparados, não comparar como benchmark com a primeira abertura fria de 403,54s. Não houve Play, arena própria, pacote, mobile ou repetição de testes do núcleo.

## ENG-G01 — identificação e contorno de diagnóstico

O log detalhado associa as 14 falhas em pt-BR a três testes internos da engine:

| Teste | Falhas em pt-BR | Resultado em inglês |
|---|---|---|
| FUnifiedErrorTest_CreateErrorMessage | 7 | Success |
| FUnifiedErrorTest_CreateErrorMessageWithContext | 4 | Success |
| FStructuredLogFormatTest | 3 | Success |

Os nomes e contagens extraídos do log estão nos JSONs vinculados abaixo. A hipótese inicial de que apenas StructuredLogFormat explicaria todas as falhas foi substituída pela identificação de três testes.

A categoria LogAutomationTest tem nível Warning por padrão em AutomationTest.cpp:31. O resumo com nome do teste usa nível Log em DumpAutomationTestExecutionInfo; isso explica o diagnóstico incompleto anterior. O adaptador LowLevelTestAdapter.h:130 emite Condition failed sem expressão detalhada. RunSmokeTests é chamado na inicialização, e normalmente ignora commandlets.

A comparação estabelece sensibilidade ao idioma/região no ambiente observado: mesmos binários, falhas em pt-BR e zero linhas Error: em inglês. Não separa idioma de região, nem identifica cada valor divergente. Não é correção da engine, nem prova de ausência de outros defeitos. Para prosseguir com avaliação gráfica, inglês por processo é um contorno delimitado; validação de localização pt-BR continua aberta antes de produção. Não suprimir testes nem modificar a engine.

## ENG-G02 — telemetria

A preferência oficial consultada por UEditorEngine::AreEditorAnalyticsEnabled (EditorServer.cpp:1231) é bSendUsageData, em AnalyticsPrivacySettings (config EditorSettings); o caminho FEngineAnalytics::Initialize examinado não consulta NoAnalytics.

Nas duas sessões com opt-out solicitado não foram encontrados os marcadores datarouter ou outstanding Http Requests presentes na sessão original. Isso é evidência favorável ao opt-out do mecanismo observado, não auditoria de tráfego. Não houve captura de rede, bloqueio ou garantia de ausência de qualquer comunicação externa. Entrega histórica de dados continua desconhecida. Isolamento de rede não foi testado por decisão explícita do usuário e não será implementado por inferência.

## Evidências e próximos limites

- [Execução pt-BR](../relatorios/atuais/ENG-01-GRAFICO-DIAGNOSTICO.json).
- [Comparação em inglês](../relatorios/atuais/ENG-01-GRAFICO-DIAGNOSTICO-EN.json).
- [Relatório original preservado](ENG-01-GRAFICO.md).

Logs integrais permanecem em build ignorado, sem copiar identificadores/URLs de telemetria para documentação. Inspeção visual somente no editor autorizado. Nenhuma interação com outras aplicações; nenhum launcher ou componente do jogo de referência executado.

Estado: abertura gráfica básica confirmada nas condições avaliadas; diagnóstico regional estabelecido com contorno em inglês. ENG-G01 deixa de ter origem desconhecida, mas localização pt-BR continua pendente. ENG-G02 tem opt-out observado sem isolamento de rede. Pacote Windows e VIS-01 continuam tarefas separadas, ainda não executadas. Decisão posterior do usuário: priorizar a configuração de trabalho Unreal em inglês, mantendo localização do jogo como tarefa separada. Não iniciar automaticamente nova etapa.

## Padrão de trabalho e idioma do produto

Priorizar inglês no idioma e região do ambiente Unreal, com `-culture=en` por processo para reproduzir o teste aprovado. Não alterar idioma do Windows ou preferências globais automaticamente. O resultado é específico deste ambiente/build e não significa que a Unreal seja incapaz de trabalhar em português.

O idioma da ferramenta não determina o idioma do jogo. O objetivo de oferecer o produto em português permanece possível por localização separada da apresentação: investigar textos traduzíveis e tabelas de strings da Unreal, recursos de tradução, idioma selecionável e tratamento de fontes, acentos, pluralização, números e datas. Esta é uma proposta técnica a avaliar, não integração implementada ou validada. IDs, regras e dados persistidos devem permanecer independentes do texto exibido; validar a experiência em português no pacote final e nas plataformas alvo.

Nenhum teste de localização do produto foi executado. O contorno em inglês trata os testes de inicialização observados; não deve ser convertido em restrição de idioma para os jogadores.
