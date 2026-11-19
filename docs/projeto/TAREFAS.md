# Tarefas — fila vigente

Atualizado em 19/11/2026. [Checklist histórico integral](historico/2026-11-18/TAREFAS.md.txt); [blocos e legenda](../batalhas/PAINEL-BLOCOS.md).

## Concluído

- [x] Fundação, ambiente, regras/tempo/R04 e modos no escopo dos aceites registrados no painel.
- [x] COM-02: protótipo textual, comandos, resultado e reinício.
- [x] HAB-01: cinco partes, kits ativo/passivo/híbrido, condição e automação.
- [x] CIC-01: composição de equipe, recompensa única e save v1 entre batalhas.
- [x] Preparação da persistência: codec separado do armazenamento Windows; 213 testes gerais aprovados.
- [x] ENG-01 — ponte Windows: instalação conferida, módulo/ponte compilados e 11 checkpoints aprovados em dois processos Unreal independentes.

Não repetir verificações concluídas sem mudança, falha ou dependência pertinente.

## Próximo bloco executável

- [x] ENG-01 — observação gráfica inicial: mapa, menus, RTX 3060/D3D12/SM5 e encerramento normal confirmados. Aceite integral permanece parcial; [relatório](ENG-01-GRAFICO.md).
- [x] ENG-G01 — diagnóstico: três testes internos de mensagens identificados (7+4+3 falhas em pt-BR); passaram em inglês. Inglês priorizado no ambiente Unreal; causa detalhada não corrigida.
- [x] ENG-G02 — observação delimitada: bSendUsageData=False solicitado nas duas comparações, sem os marcadores anteriores. Sem auditoria ou isolamento de rede; usuário não autorizou mudanças de rede.
- [ ] ENG-01 — pacote: definir e validar um executável próprio distribuível em incremento separado.
- [ ] VIS-01: arena provisória com ação/alvo, turno/vida/resultado e reinício, reutilizando o núcleo sem duplicar regras.

Uma entrega por vez. Não abrir editor, instalar componentes nem iniciar serviços por inferência de uma tarefa documental. Download/instalação e escolhas definitivas continuam com decisão específica.

## Áreas posteriores

- [ ] Localização do jogo em português: definir textos traduzíveis e recursos separados, seleção de idioma, fontes/acentos, formatação regional e testes no pacote. Não depende de usar o editor em português; ainda não implementada nem validada.
- [ ] ART-01: direção artística original, personagem representativo, fontes e direitos.
- [ ] AUD-01: áudio próprio/licenciado e integração funcional.
- [ ] EXP-01: delimitar campanha e experiência rítmica antes da implementação.
- [ ] Multiplataforma: confirmar ferramentas/dispositivos e validar Android/iOS; Linux e Windows ARM64 conforme decisão de produto. Sem presumir validação pela presença de opções no instalador.

## Extensões acrescentadas ao longo do projeto

- [ ] PVE-09: fases de kit por HP/turno e eventos únicos.
- [ ] PVE-06: sobrevivência/score/recompensas; detalhar o exemplo relatado de Treva Fluxo.
- [ ] PVE-07/08: identificar o exemplo dos Milagres e confirmar eventual retorno cíclico de times.
- [ ] Seletores de área/dinâmicos e efeitos entre times, com alcance explícito.
- [ ] Silêncio/supressão, passivas durante combate/recarga interna, custos/condições compostas e automação estratégica.
- [ ] Save de runtime, migrações, backups/concorrência e economia de produção: contratos próprios antes de implementar.

As questões e IDs históricos permanecem em [PENDENCIAS.md](PENDENCIAS.md). Não tratar essas extensões como reprovação dos blocos já aceitos.

## Encerramento de uma entrega

Atualizar estado, tarefa/painel pertinentes e evidências; relatar somente testes executados. Preservar aceites datados e evitar acumular resumos contraditórios na síntese vigente. Não iniciar automaticamente a próxima tarefa.
