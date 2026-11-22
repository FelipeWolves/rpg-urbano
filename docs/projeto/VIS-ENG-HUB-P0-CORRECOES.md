# VIS / ENG / HUB — correções P0

22/11/2026. Entrega limitada à entrada, seleção e preparação de vídeo. Não encerra as famílias inteiras.

## Alterações

- VIS: hitboxes projetadas do corpo e rótulo dos alvos vivos/válidos; prioridade do rótulo e profundidade para sobreposição. Trocar alvo mantém a habilidade selecionada e não executa a ação. No perfil padrão, confirmar é uma ação separada; no layout clássico, preservado o envio direto.
- ENG: Canvas considera a janela em primeiro plano, sem exigir foco interno no widget da viewport. Coordenadas fora da viewport são rejeitadas. Mantidas conversão DPI/Canvas, cancelamento por arrasto/revisão/perda de foco e deduplicação toque/mouse.
- HUB: Rever abertura inicia preparação local e isola a camada de entrada da mídia. Pedido duplicado durante abertura é ignorado. Readiness do backend não é confundida com reprodução efetivamente iniciada; tentativas internas espaçadas em 100 ms, sem clique adicional, respeitam o timeout existente de 15 s.
- Pausa por perda real de foco continua exigindo retomada do usuário. Autoplay permanece desativado; assistido não é concedido por falha.

## Evidências e limites

Testes nativos aprovados: roteamento 8/8, ciclo de mídia 16/16, ponteiro 10/10, coordenadas 8/8, fluxo 10/10, além das demais regressões de apresentação no runner. Relatórios JSON em docs/relatorios/atuais. O teste puro cobre os contratos; não reproduz o foco/decoder real da Unreal.

Compilação RpgEngineProbeEditor Win64 Development aprovada (60,62 s); stage preparado e verificado com 66 entradas, sem cook. Evidência no JSON VIS-ENG-HUB-P0. Avisos do MSVC 14.52 não preferencial e C4864 em header da engine permanecem; nenhuma instalação/alteração de toolchain. Nenhum editor, jogo, player, Android ou tela será executado nesta entrega. Nenhuma configuração de rede ou instalação.

RET-INPUT-01 e RET-MEDIA-01 continuam ABERTOS para aceite visual. A discrepância entre foco da janela/widget foi confirmada no código da engine, mas não se afirma causa exclusiva do relato. O comportamento real do backend ainda exige reteste.

## Reteste do usuário

1. Abrir o projeto normalmente, executar o cenário habitual e selecionar uma habilidade de alvo único.
2. Escolher A e depois B antes de confirmar; observar se o destaque acompanha cada clique completo. Confirmar uma vez. Separar escolha de habilidade/alvo/confirmação de um clique adicional inesperado.
3. Testar Rever abertura no primeiro acesso, fechar e repetir; verificar vídeo e áudio. Perda real de foco pode exigir Retomar e não é o mesmo defeito.
4. Se persistir, informar cenário/atalho, janela ou PIE, resolução/escala e sequência. Não enviar credenciais ou logs pessoais.

## Continuidade

Prioridade: aceite dessas correções; depois integração visual seletiva dos modos PvE já disponíveis no core, fluidez/feedback e HUD. Android continua adiado. Preparação do pacote não equivale a gerar nova versão Shipping nem a validar a demo em execução.

MCP permanece opcional, sem plugin instalado ou ferramenta Unreal disponível. Ver ENG-AMB-MCP-UNREAL.md. Nenhuma dependência de gameplay foi introduzida. As 12 frentes recebíveis já têm próximos passos independentes e insumos necessários em seus READMEs (commit 4bbd4ef).

Percentuais mantidos: demo ~25% (23–27%). Não atribuir avanço visual aceito a testes nativos ou compilação.