# Estado do projeto

Atualizado em 19/11/2026. Esta é a síntese vigente; registros anteriores estão no [histórico preservado](historico/2026-11-18/ESTADO.md.txt).

## Resultado atual

O protótipo C++20 permanece jogável no console. HAB-01 e CIC-01 estão concluídos no escopo mínimo: habilidades, perfis, seleção de equipe, recompensa única e save v1 de progresso entre batalhas.

A ponte Windows foi compilada e executada na **Unreal Engine 5.8.4**, instalada em `D:\Unreal Engine\UE_5.8`. Dois processos independentes da commandlet própria passaram pelos mesmos **11 checkpoints**, com saída zero e resultado idêntico ao baseline standalone. Essa etapa está concluída.

| Verificação | Evidência atual | Limite |
|---|---|---|
| Regressão do protótipo | 213/213 testes aprovados | Não são 213 testes executados na Unreal |
| Ponte na Unreal | 11 checkpoints em cada uma de duas execuções | Mesmo cenário repetido, não 22 casos distintos |
| Compilador/SDK usados | MSVC 14.52.37029, SDK 10.0.26100.0 | Compilador não preferido; aviso mantido |
| Editor gráfico/GPU | Abertura, mapa, menus e encerramento confirmados; RTX 3060/D3D12/SM5 | Diagnóstico: três testes internos falham em pt-BR e passam em inglês; ENG-01-GRAFICO-DIAGNOSTICO.md |
| Pacote/Android/iOS | Ainda não validados | Instalar suporte de plataforma não equivale a validar o jogo |

Primeira inicialização geral atingiu o limite de tempo antes do teste. A rota final por commandlet passou; ambas as evidências foram preservadas. Não há revisão pendente da ponte entregue sem mudança ou falha pertinente.

## Experiência disponível

- `Jogar.cmd`: equipes; `Jogar-Classico.cmd`: clássico com defesa.
- `Jogar.cmd --equipe A1 A3`: seleção anterior à batalha. Catálogo atual A1/A2/A3; suporte a 1–5 validado com dados de teste.
- `salvar`: grava seleção, saldo e conclusões. `--continuar`: carrega e inicia outra batalha, sem retomar HP/turnos.
- Save v1 manual em `build/saves`, ignorado pelo Git. Recompensa provisória: 10 créditos por vitória A/eliminação; zero nos demais finais.
- A2/B2 têm passiva de abertura; A3/B3, habilidade híbrida e proteção condicionada. Time A manual, B com prioridades e básica como alternativa.

## Próximo incremento

Priorizar idioma e região em inglês no ambiente Unreal (`-culture=en` por processo nos testes). A comparação passou sem linhas Error:; pt-BR produziu 14 falhas em três testes internos de mensagens. [Diagnóstico e limites](ENG-01-GRAFICO-DIAGNOSTICO.md). Próximo: planejar o pacote Windows de avaliação, sem executá-lo nesta entrega. O jogo poderá ter interface/textos em português por uma camada de localização própria, a definir e validar separadamente; inglês no editor não determina o idioma do produto. Telemetria com opt-out não apresentou os marcadores anteriores; rede continua sem isolamento por decisão do usuário. Nova execução exige autorização específica. A ponte entregue não precisa ser refeita por rotina.

Não assumir que save de progresso é save de combate, que intercalação equivale a threads ou que o perfil de chefe compartilhado cobre todos os modos PvE. Pendências em [PENDENCIAS.md](PENDENCIAS.md), fila em [TAREFAS.md](TAREFAS.md) e [painel](../batalhas/PAINEL-BLOCOS.md).

## Evidências para retomada

- [Resultado standalone](../relatorios/atuais/COMBATE.json).
- [Compilação da ponte](../relatorios/atuais/ENG-01-BUILD.json).
- [Execução Unreal](../relatorios/atuais/ENG-01-RUNTIME.json) e [relatório do aceite](ENG-01-RUNTIME.md).
- [Habilidades](../batalhas/HAB-01-FINAL.md), [ciclo local](CIC-01.md) e [save v1](CIC-01-SAVE-R01.md).

Git: commits locais de entregas autorizados; pushes ficam com o usuário. Não alterar identidade/configuração/histórico. Conferir status antes de editar e comunicar conflitos reais. Esta entrega é somente documental: sem nova compilação, execução de engine ou alteração de saves.
