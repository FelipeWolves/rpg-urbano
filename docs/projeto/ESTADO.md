# Estado do projeto

Atualizado em 19/11/2026. Esta é a síntese vigente; registros anteriores estão no [histórico preservado](historico/2026-11-18/ESTADO.md.txt).

**Visão geral:** [percentuais por área e frentes independentes](PROGRESSO-GERAL.md). Estimativas de planejamento, separadas das evidências técnicas abaixo.

## Prioridade vigente

Entrega documental: [central de materiais por grupo](../../entregas/README.md) criada para pesquisa/produção independente do usuário. Guias e índice versionados; recebidos locais ignorados até revisão. Na retomada, ler apenas novidades pertinentes antes de implementar. Seletor posteriormente corrigido e aceito; próxima frente proposta é a apresentação mobile testável. Nenhum programa/teste de gameplay executado nesta entrega.

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

## Referência recebida — hub urbano

Quatro capturas associadas ao texto por conteúdo visual em [HUB-01-REFERENCIAS.md](HUB-01-REFERENCIAS.md). Proposta de três áreas, avatar e enquadramentos responsivos separada da evidência estática e da implementação existente. Nenhum hub, login, serviço ou loja implementado/adotado por esta recepção; prioridade executável VIS permanece. Capturas não foram copiadas; o texto recebido foi preservado como referência, sem adotar perfil/recursos como dados do produto.

## HUB-01A — base de apresentação

Política C++ de panorama/recortes e vídeo com proporção, intenção de orientação mobile e composição lateral desktop implementada;10 grupos próprios aprovados. Catálogo local validado com4 Planned/0 Ready. [Relatório e limites](HUB-01A-APRESENTACAO.md). Ainda sem carregamento real de imagem/player, preferências aplicadas à janela ou execução mobile. Nenhuma regra de combate mudou.

Mídias HUB-01A recebidas: panorama JPEG, arte lateral PNG e dois MP4; nomes/dimensões reais cadastrados, autoria própria declarada. [Conferência](HUB-01A-RECURSOS.md). Vídeos não reproduzidos; arquivos permanecem locais e ignorados no Git até estratégia de versionamento. Recortes do terraço e integração de player ainda pendentes.

HUB-01B: dois acessos definidos/cadastrados no panorama atual (Evento Especial no outdoor, Batalha no dispositivo), sem terceiro botão. [Definição e limites](HUB-01B-ACESSOS.md). Renderer/roteador pendentes; nenhuma interface aberta ou alteração da imagem.

Imagem marcada recebida: outdoor com rosto feminino acima do lobo alado→Evento Especial; tela do dispositivo do homem→Batalha. Alvos confirmados e coordenadas atualizadas; referência preservada localmente sem alterar o panorama. Aceite visual e ligação gráfica continuam pendentes.

## HUB-01B — ligação gráfica por código

Parser do catálogo local, panorama único, dois acessos e navegação evento/arena/volta implementados para opção -Hub.10 grupos anteriores e5 novos de projeção aprovados; build Editor registrado sem abrir editor/aplicativo/commandlet.37 entradas preparadas, incluindo JSON e JPEG; sem vídeos ou cook/pacote. [Relatório](HUB-01B-IMPLEMENTACAO.md). Aceite visual/fluxo e integração do player continuam pendentes.

## Hub — tema e decisões visuais vigentes

Cormorant Garamond/Lora confirmadas, Cinzel excluída; LOBO é codinome. Logo WebP recebida localmente, sem importação de runtime. Tema, texto do evento, emblema original, pulso e modo estático implementados;5 grupos novos nativos e build Editor aprovados, sem editor/aplicativo aberto. Preparação39 entradas. Fontes reais/logo, aceites visuais, toque e preferência do sistema continuam pendentes. [Relatório](HUB-01C-VISUAL.md). Esta entrega de hub não encerra VIS-01C da arena.

Atualização vigente da identidade: fontes Cormorant Garamond/Lora e PNG da logo conectados ao HUD; fontes locais com OFL, sem instalação global.44 entradas preparadas; ver [resultado e limites](HUB-IDENTIDADE.md). [Sequência e entregas independentes](PROXIMAS-FASES.md); [percentuais revisados](PROGRESSO-GERAL.md). Esta atualização prevalece sobre menções anteriores de fontes/logo ainda não recebidas.

### Fechamento e próxima entrega

✅ Usuário confirmou texto sem corte, seleção funcional e batalha conforme combinado. Fontes/logo conectadas; aceite integral ainda parcial. ✅ Seletor no chão corrigido e aceito visualmente pelo usuário. Investigar apresentação mobile separadamente; fluidez aguarda critérios. Prévia por Abrir-Preview.cmd; editor por Abrir-Editor.cmd, ambos usando build aprovado. Nesta entrega final só se validaram os lançadores sem abrir programas. [Evidências](HUB-IDENTIDADE.md).

Seletor: correção por linhas3D com profundidade junto à base do modelo implementada e posteriormente aceita visualmente pelo usuário; o assistente não abriu a prévia nessa entrega. [Escopo e roteiro](VIS-SELETOR-CHAO.md). Não altera regras nem resolve mobile/fluidez; não repetir esse aceite sem mudança ou defeito novo.

Mobile: [proposta revisada e sequência mínima](MOBILE-PLANO-REVISAO.md). HUD atual é Canvas, não UMG; preview/renderizador não troca automaticamente DeviceProfile Windows. Próxima implementação a planejar: apresentação Desktop/Mobile/Auto separada de plataforma, antes de presumir migração UMG ou instalar SDK. Sem implementação nova por esta revisão.

Fila independente documentada em [AUT-01–16](FILA-DESENVOLVIMENTO-INDEPENDENTE.md), sem implementação nesta entrega. Inclui trabalho pronto e preparação condicionada; primeiro bloco proposto AUT-01. Não aguardar todos os materiais para avançar nas tarefas prontas.
