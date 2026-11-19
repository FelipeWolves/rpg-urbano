# Tarefas — fila vigente

Atualizado em 19/11/2026. [Checklist histórico integral](historico/2026-11-18/TAREFAS.md.txt); [blocos e legenda](../batalhas/PAINEL-BLOCOS.md).

## Próxima ação vigente

- [x] Receber, preservar e revisar a pesquisa de habilidades/itens.
- [x] Registrar explicitamente todos os pontos da devolutiva em [PENDENCIAS.md](PENDENCIAS.md), com prioridades, exemplos e campos de resposta.
- [x] P1: ESC-01 a ESC-07 respondidos; escudo padrão implementado e testado. [Contrato e limites](ESCUDO-PADRAO.md).
- [ ] Em paralelo documental, pesquisar VIS/REG/SIS se desejado; não precisa fechar todas as extensões para avançar.
- [ ] Aceite visual da revisão atual, após autorização específica de abertura; independe do catálogo completo.
## Concluído

- [x] Fundação, ambiente, regras/tempo/R04 e modos no escopo dos aceites registrados no painel.
- [x] COM-02: protótipo textual, comandos, resultado e reinício.
- [x] HAB-01: cinco partes, kits ativo/passivo/híbrido, condição e automação.
- [x] CIC-01: composição de equipe, recompensa única e save v1 entre batalhas.
- [x] Preparação da persistência: codec separado do armazenamento Windows; 213 testes gerais aprovados.
- [x] ENG-01 — ponte Windows: instalação conferida, módulo/ponte compilados e 11 checkpoints aprovados em dois processos Unreal independentes.

Não repetir verificações concluídas sem mudança, falha ou dependência pertinente.

## Entregas recentes e aceite visual restante

- [x] ENG-01 — observação gráfica inicial: mapa, menus, RTX 3060/D3D12/SM5 e encerramento normal confirmados. Aceite integral permanece parcial; [relatório](ENG-01-GRAFICO.md).
- [x] ENG-G01 — diagnóstico: três testes internos de mensagens identificados (7+4+3 falhas em pt-BR); passaram em inglês. Inglês priorizado no ambiente Unreal; causa detalhada não corrigida.
- [x] ENG-G02 — observação delimitada: bSendUsageData=False solicitado nas duas comparações, sem os marcadores anteriores. Sem auditoria ou isolamento de rede; usuário não autorizou mudanças de rede.
- [x] ENG-01 — geração do pacote: Development/Win64, build/cook/stage/archive com saída0; 45 arquivos (~501,6 MiB com PDB). ENG-01-PACOTE-BUILD.md.
- [x] ENG-01 — execução básica do pacote: mapa renderizado fora do editor e encerramento normal; entrada/processo principal com saída0. ENG-01-PACOTE-RUNTIME.md; não valida Windows limpo ou gameplay.
- [x] VIS-01 — plano: [arquitetura, etapas e matriz de aceite](VIS-01-PLANO.md). Plano inicial preservado; implementação posterior registrada nas linhas seguintes.
- [x] VIS-01A: adaptador de sessão visual e cenário compartilhado com o console; preservar regras e testar contratos.
- [x] VIS-01B/C — entrega funcional3×3: cena compilada, vitória após20 ações, passiva/condição/recarga e reinício observados no pacote; processos com saída0. [Aceite parcial](VIS-01C.md).
- [ ] VIS-01B/C — fechamento visual: inspecionar a revisão já compilada de HP/escudo, descrições/alvos e formas geradas do elenco; validar1/3/5 e outras resoluções. Não iniciar arte/áudio por inferência.

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


- [x] Separar apresentação Standard em profundidade e Classic lateral; testes nativos de posições aprovados. Não altera perfis de regras.
- [x] Inspeção inicial das duas apresentações no pacote; legibilidade integral continua pendente. [Contrato e chamadas](VIS-01-LAYOUTS.md). Não marcar como visualmente aprovado por compilar.


Apresentações verificadas no pacote em19/11/2026: Standard sem argumento e Classic por chamada explícita, ação/resposta/reinício observados com mesmos valores. Entrada e processo principal de ambas saíram0. Aceite visual integral parcial por sobreposição de etiquetas ao fundo e demais pendências. Relatório: docs/projeto/VIS-01-LAYOUTS-RUNTIME.md. Aplicativos encerrados; próximo ajuste é legibilidade/áreas de seleção.


Contrato inicial de interação: [VIS-01-INTERACAO-R01.md](VIS-01-INTERACAO-R01.md). Implementação posterior registrada abaixo; sexta posição permanece aberta.


Interação inicial implementada e compilada em19/11/2026: ícone/painéis, faixa de escudo real, gesto prolongado, nível opcional e seleção padrão com marcador/sugestão/confirmação. Regressão213+6 e6 novos grupos aprovados. Ainda sem teste visual desta revisão; cálculo percentual do escudo segue separado dos escudos fixos antigos. Relatório: docs/projeto/VIS-01-INTERACAO-IMPLEMENTACAO.md. Próximo: inspeção autorizada do pacote atualizado.

## Incremento compartilhado com o clássico

- [x] Adaptador explícito Classic, ataque/defesa do núcleo, efeitos e consultas compartilhados;6 novos grupos nativos aprovados e build Editor aprovado.
- [ ] Gerar pacote atualizado e realizar aceite visual específico dos dois perfis, formações e resoluções, após autorização de abertura.
- [x] Fechar ESC-01 a ESC-07 e implementar o caso comum; extensões futuras permanecem separadas.

Roteiro e limites: [VIS-01-CLASSICO-INTERACAO.md](VIS-01-CLASSICO-INTERACAO.md). Sem inventário/especiais cadastrados no encontro clássico e sem equivalência declarada com Dislyte.

## Pesquisa de habilidades recebida —19/11/2026

Documento do usuário preservado e comparado aos contratos. [Revisão e matriz](REVISAO-PESQUISA-HABILIDADES-ITENS.md). Q07/Q08 e proposta de frentes não substituem R04, defesa atual ou PAR-02. Fontes externas ainda não auditadas; escudo percentual permanece em especificação (base, instante/recalculo, reaplicação e consumo), não mais aguardando envio do documento. Próximo passo: fechar esse contrato mínimo. Sem código/testes novos nesta recepção; aceite visual atual permanece pendente.
