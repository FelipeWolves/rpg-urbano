# AUT-16 — verificações por área e fumaça

22/11/2026. Seletor `scripts/verificar-por-area.ps1` permite planejar sem executar e executar somente áreas pertinentes. Não contém comando de abrir Unreal, cook, pacote, emulador, instalador, rede ou controles de tela.

## Uso

- Planejar: `./scripts/verificar-por-area.ps1 -Areas Marker,Events,Data,Media,Session,Stage`
- Executar uma área: `./scripts/verificar-por-area.ps1 -Areas Stage -Execute`
- Executar testes de uma mudança no codec: `./scripts/verificar-por-area.ps1 -Areas Data -Execute`

Rodar na pasta principal do projeto via PowerShell 7 já instalado. Não muda política de execução, PATH ou instala componentes. Areas é lista fechada. Scripts são conhecidos, argumentos não vêm dos arquivos recebidos. Cada execução usa processo separado do PowerShell e código de saída próprio, registra log ignorado em build/qa e resultado por área. Script que falha não é confundido com LASTEXITCODE antigo de outro comando. Nenhum relatório pode conferir aceite visual automaticamente.

| Área | Verificação | Não comprova |
|---|---|---|
| Marker | Geometria nativa AUT-12 | Aparência, terreno, Shipping |
| Events | Contrato nativo AUT-13 | Animações/VFX/áudio |
| Data | Codec/paridade AUT-14 | Migração de regras ou editor de dados |
| Media | Bateria nativa apresentação/mídia | Playback, codec do dispositivo, flag na Unreal |
| Session | Bateria focada de sessão/equipe/save | I/O/menus reais na Unreal |
| Stage | Hashes/includes do stage já preparado | Build/cook/executável/Windows limpo |

`data/qa/smoke.json` mantém dez cenários manuais com status Pending/Deferred/ProvisionalUser/AcceptedUser. A conferência do primeiro clique é ProvisionalUser e exige a segunda olhada solicitada. Aceites anteriores não são apagados nem estendidos a recursos novos.

Evidência desta entrega: plano das seis áreas gerado sem execução; caminho Execute exercitado apenas em Stage, aprovado. Contrato Events repetido ao endurecer o runner AUT-12/13/14 para comparar passed==total (não apenas presença de marcador); sete grupos aprovados. Não foi repetida a bateria geral por organização documental. Relatórios AUT-16-PLANO.json e AUT-16-EXECUCAO.json.

Pendências: execução dos casos manuais no programa/aparelho, desempenho, Android/iOS, Windows limpo e pacote real; instrumentação de animação/recursos finais. Teste nativo, build, observação gráfica e aceite são colunas distintas. Não interpretar Passed do Stage como aprovação da demo inteira.
