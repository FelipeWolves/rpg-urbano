# Estado do projeto

Atualizado em 19/11/2026. Esta é a síntese vigente; registros anteriores estão no [histórico preservado](historico/2026-11-18/ESTADO.md.txt).

## Prioridade vigente

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
