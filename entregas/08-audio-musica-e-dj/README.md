## Estado e dependências — 23/11/2026

- **O que posso fazer.** Ligar som provisório e animação ao início, impacto e fim dos eventos existentes.
- **O que você pode entregar.** Uma amostra de loop e efeitos de ação, impacto e menu com autoria.
- **Aceite ou condição para avançar.** Aceitar o resultado audiovisual. Mix e conteúdo finais dependem de recursos. Instrumentos e Games ficam para etapa futura.

[Central de entregas](../NECESSIDADES-ATUAIS.md). As notas abaixo preservam o histórico.

# Áudio e música — guia vigente

Atualizado em **22/11/2026**. Demo **~28%**. Sua preparação e o desenvolvimento são frentes paralelas: entregar amostras é suficiente; nada aqui pede para refazer material já definido.

## Sua próxima contribuição

Uma amostra de BGM em loop e efeitos por evento (ação, impacto, menu); formato, loop/pontos, volumes relativos e autoria. Voz/stems se disponíveis.

## O que posso desenvolver sem sua próxima entrega

IDs/eventos e arquitetura de áudio provisória sem esperar todas as faixas.

## O que depende de aceite ou recurso final

Mix final/vozes exigem recursos e audição; DJ é expansão separada.

## Como entregar

Coloque em recebidos/ desta pasta. Nome: AAAA-MM-DD_assunto_v02.md, com ID, versão, substituição/delta, decisão ou hipótese e origem. Pode enviar uma parte pronta por vez. Não apagar versões anteriores. Markdown pode ir ao Git; imagens, vídeos e recursos grandes continuam locais. Não incluir credenciais/dados pessoais.

Cliques únicos, vídeo/skip, esferas e seletor já foram aceitos no recorte Windows testado. Novos HUDs, save real e mobile têm aceites separados. [Central](../NECESSIDADES-ATUAIS.md) · [Progresso](../../docs/projeto/PROGRESSO-GERAL.md) · [Minha fila](../../docs/projeto/FILA-DESENVOLVIMENTO-INDEPENDENTE.md).

<details>
<summary>Guia anterior e histórico — não repetir pedidos já atendidos</summary>

# Áudio, música e DJ — guia vigente

Atualizado em22/11/2026. **P2 áudio em paralelo; P3 DJ.** Fonte vigente: [mapa das famílias](../../docs/projeto/MAPA-FAMILIAS-E-PRIORIDADES.md). Conteúdo deste guia distingue preparação independente de implementação/aceite. Atualizações de arquivos recebidos ficam preservadas.

## Já recebido — não reenviar

- Direção autoral/licenciada, canais Master/BGM/SFX/UI/VO, loops e eventos de áudio já recebidos.
- Matrizes WAV48kHz/24bit e stems são direção de produção recebida; compressão/backend final precisa validação, sem instalar codecs/plugins.

## Você pode preparar em paralelo

- Uma amostra por função: hub/combate/UI/impacto, com ID, versão, fonte, duração, idioma, loop intro/start/end e autoria/licença.
- Quando houver voz, indicar texto/idioma e arquivo correspondente. Preservar stems/projeto editável.
- DJ: enviar somente decisões novas de julgamento/input/latência se necessário; não reenviar a direção narrativa inteira.

## Trabalho do desenvolvimento — não precisa virar pesquisa sua

- Estruturar canais, volume/mute, preferências e cues sem depender de trilha final.
- Integrar mixer/crossfade/ducking e segundo plano de forma verificável. Modo DJ não é consequência automática de tocar música.

## Dependências e aceites

- Áudio real exige aceite auditivo; silêncio físico/interrupções precisam aparelho.
- Propostas de concorrência/ducking e compressão não comprovam desempenho; validar sem misturar com regras de combate.

## Como entregar

Use recebidos/ deste grupo. Nome sugerido: AAAA-MM-DD_assunto_v02.md; identifique ID/versão, o que substitui, decisão/proposta/referência e origem. Entrega parcial é válida. Não apagar versões anteriores; Markdown pode ser versionado, imagens/vídeos e fontes grandes ficam locais fora do Git. Não reenviar material já recebido apenas para preencher uma lista.

[Central de necessidades](../NECESSIDADES-ATUAIS.md) · [Retestes](../../docs/projeto/RETESTES-ENTRADA-E-VIDEO.md) · [Escopo técnico entregue](../../docs/projeto/ESC-SIS-PVE-CONSOLIDACAO.md) · [Modelo](../MODELO-ENTREGA.md)

<details>
<summary>Guia anterior — histórico; não preencher novamente o que já foi respondido</summary>

> **Revisão 21/11/2026:** materiais recebidos desta área reconciliados na [consolidação vigente](../../docs/projeto/CONSOLIDACAO-2026-11-21.md). Não reenviar respostas ENT já fechadas. Novos arquivos finais podem continuar em recebidos; implementação e testes permanecem separados. Prioridade atual: abertura padrão Unreal; depois fila AUT.

# Áudio, música e eventual modo DJ

**Atualização20/11:** material desta área estudado; [matriz e pendências específicas](../../docs/projeto/REVISAO-ENTREGAS-2026-11-20.md). Guia abaixo preservado como roteiro; não precisa responder novamente o que já entregou. Revisão integrada ao planejamento, não ao código.

Prioridade: **P2/P3**. Guia de preparação, não autorização de implementação automática.

## Já existente — não reenviar decisões fechadas

Identidade musical definida como direção; sistema de áudio e DJ jogável não implementados.

## O que você pode reunir ou responder

- [ ] Entregue músicas/efeitos próprios ou licenciados com fonte editável, duração, loop e ponto de uso; WAV é boa fonte de trabalho, não requisito final de pacote.
- [ ] Defina grupos de volume, mute, áudio em segundo plano e prioridade de sons/vozes.
- [ ] Liste eventos que disparam som: clique, habilidade, impacto, vitória, hub, transição.
- [ ] Para modo DJ, proponha comandos, timing/tolerância, pontuação, dificuldade, sincronismo e correção de latência; separar referência de implementação própria.
- [ ] Informe se trilha será vídeo ou faixa separada e como tratar legendas/voz/localização.

## Trabalho de desenvolvimento restante

Player/mixagem de áudio, eventos, pausa/retomada, direitos e modo rítmico como contrato próprio.

## Dependências e entrega

Composição e tabela de eventos podem chegar agora; DJ exige medição de latência e protótipo específico.

Coloque documentos/imagens/vídeos desta área em **recebidos/**, nesta pasta. Pode entregar parcialmente; use NÃO SEI / PROPOSTA / REFERÊNCIA / DECISÃO DESEJADA. Copie o modelo geral se ajudar. Não sobrescreva versões anteriores. Desde21/11,Markdown recebido é versionado por autorização do usuário; imagens/vídeos ficam locais, fora doGit.

[Instruções gerais](../README.md) · [Modelo](../MODELO-ENTREGA.md) · [Pendências vigentes](../../docs/projeto/PENDENCIAS.md).

</details>


</details>