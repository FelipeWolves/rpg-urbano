# Documentação — índice vigente

Atualizado em 18/11/2026. Consulte a síntese atual antes dos relatórios de etapas anteriores.

## Retomar o trabalho

- [Estado atual](projeto/ESTADO.md), [tarefas](projeto/TAREFAS.md) e [prioridades](projeto/PLANO-PRIORIDADES.md).
- [Painel de blocos e legenda](batalhas/PAINEL-BLOCOS.md): aceites concluídos e pendências separados.
- [Visão](projeto/VISAO.md), [arquitetura](projeto/ARQUITETURA.md), [decisões](projeto/DECISOES.md) e [questões abertas](projeto/PENDENCIAS.md).
- [Ferramentas](projeto/FERRAMENTAS.md), [guia de contribuição](../CONTRIBUTING.md) e [regras de trabalho](../AGENTS.md).

## Aceites funcionais

| Área | Relatório | Evidência estruturada |
|---|---|---|
| Núcleo/console | [Fundação](projeto/COMBATE.md) e [painel](batalhas/PAINEL-BLOCOS.md) | [213 testes standalone](relatorios/atuais/COMBATE.json) |
| Habilidades | [HAB-01 final](batalhas/HAB-01-FINAL.md) | Aceites por parte em batalhas/ |
| Ciclo local | [CIC-01](projeto/CIC-01.md) e [save v1](projeto/CIC-01-SAVE-R01.md) | [CIC-01 final](batalhas/COMBATE-CIC-01-FINAL.json) |
| Instalação Unreal | [Metadados](projeto/ENG-01-INSTALACAO.md) | [Instalação](relatorios/atuais/ENG-01-INSTALACAO.json) |
| Build/ponte | [Build](projeto/ENG-01-BUILD.md) e [ponte](projeto/ENG-01-PONTE.md) | [Build atual](relatorios/atuais/ENG-01-BUILD.json) |
| Execução Unreal | [Aceite runtime](projeto/ENG-01-RUNTIME.md) | [Dois processos/11 checkpoints](relatorios/atuais/ENG-01-RUNTIME.json) |

A ponte Windows está concluída. [Editor gráfico parcialmente validado](projeto/ENG-01-GRAFICO.md), com [JSON](relatorios/atuais/ENG-01-GRAFICO.json): renderização e interação confirmadas, ENG-G01/ENG-G02 pendentes. Arena, pacote e plataformas adicionais continuam pendentes. Os 213 testes gerais e os 11 checkpoints repetidos na Unreal são medidas diferentes.

## Fonte dos experimentos

[Projeto de avaliação Unreal](../experiments/unreal/RpgEngineProbe/README.md). Arquivos gerados, DLLs, logs e saves ficam em build ignorado pelo Git; não são documentação nem fonte editável.

## Histórico e referências antigas

[Snapshot anterior à consolidação](projeto/historico/2026-11-18/README.md) preserva integralmente as antigas sínteses/checklists em arquivos .md.txt, sem promovê-las a instruções atuais. Relatórios técnicos, CSV e JSON datados continuam nos caminhos originais.

Documentos que antes ficavam na raiz estão em projeto/; AMBIENTE.json e COMBATE.json estão em relatorios/atuais/. Menções textuais históricas a esses nomes seguem essa correspondência. Links do arquivo histórico são registros da época, não navegação vigente.

Ao atualizar: substitua a síntese atual, preserve aceites datados e evite acrescentar sucessivos estados contraditórios no topo de vários documentos.
