## Estado e dependências — 23/11/2026

- **O que posso fazer.** Manter o player atual e investigar persistência da marca de vídeo assistido em ambiente de teste.
- **O que você pode entregar.** Se houver fala, enviar texto e marcações de tempo para legendas.
- **Aceite ou condição para avançar.** Validar áudio, legendas quando houver e reprodução em aparelho físico.

[Central de entregas](../NECESSIDADES-ATUAIS.md). As notas abaixo preservam o histórico.

# Vídeo e cinematics — guia vigente

Atualizado em **22/11/2026**. Demo **~28%**. Sua preparação e o desenvolvimento são frentes paralelas: entregar amostras é suficiente; nada aqui pede para refazer material já definido.

## Sua próxima contribuição

Informar se há fala e se precisa legendas; entregar texto/timecodes se houver. Não reexportar vídeo para corrigir clique, já aceito.

## O que posso desenvolver sem sua próxima entrega

Manter player e marca por binário; preparar interfaces de legendas/áudio e fallback conforme contratos existentes.

## O que depende de aceite ou recurso final

Conteúdo de legenda final depende do texto; backend mobile e mixagem não estão aceitos.

## Como entregar

Coloque em recebidos/ desta pasta. Nome: AAAA-MM-DD_assunto_v02.md, com ID, versão, substituição/delta, decisão ou hipótese e origem. Pode enviar uma parte pronta por vez. Não apagar versões anteriores. Markdown pode ir ao Git; imagens, vídeos e recursos grandes continuam locais. Não incluir credenciais/dados pessoais.

Cliques únicos, vídeo/skip, esferas e seletor já foram aceitos no recorte Windows testado. Novos HUDs, save real e mobile têm aceites separados. [Central](../NECESSIDADES-ATUAIS.md) · [Progresso](../../docs/projeto/PROGRESSO-GERAL.md) · [Minha fila](../../docs/projeto/FILA-DESENVOLVIMENTO-INDEPENDENTE.md).

<details>
<summary>Guia anterior e histórico — não repetir pedidos já atendidos</summary>

> **22/11 — candidato a reteste:** correções de entrada/alvos e preparação da mídia em código. Testar primeira abertura/Rever e troca de alvo; aceite visual ainda pendente. Ver docs/projeto/VIS-ENG-HUB-P0-CORRECOES.md. Não é necessário reenviar os recursos já recebidos.

# Vídeo e cinematics — guia vigente

Atualizado em22/11/2026. **P0 reprodução; P2 acabamento.** Fonte vigente: [mapa das famílias](../../docs/projeto/MAPA-FAMILIAS-E-PRIORIDADES.md). Conteúdo deste guia distingue preparação independente de implementação/aceite. Atualizações de arquivos recebidos ficam preservadas.

## Já recebido — não reenviar

- MP4 desktop/mobile autorais já recebidos e player local existe; não reexportar para tentar mascarar o clique adicional.
- Primeiro Rever exige clique adicional em relato e permanece REABERTO. Skip/retomada e variantes têm contrato anterior.

## Você pode preparar em paralelo

- Após correção, retestar primeiro Rever, repetição, fim natural, som, skip e perda/retorno de foco.
- Se houver fala: SRT/VTT ou tabela início/fim/texto/idioma vinculada à versão; sem fala, basta registrar isso.
- Novas versões só por revisão de conteúdo: nome, duração, resolução, trilha e substituição; guardar matriz.

## Trabalho do desenvolvimento — não precisa virar pesquisa sua

- Corrigir disparo/foco/preparação do backend com diagnóstico local; não exigir troca de mídia sem evidência.
- Depois integrar volumes, legendas/arte lateral e ciclo de vida; abertura automática continua condicionada ao aceite.

## Dependências e aceites

- Reteste real continua necessário, inclusive primeira abertura após iniciar o app.
- Backend mobile e política de orientação precisam validação na plataforma, não apenas no PC.

## Como entregar

Use recebidos/ deste grupo. Nome sugerido: AAAA-MM-DD_assunto_v02.md; identifique ID/versão, o que substitui, decisão/proposta/referência e origem. Entrega parcial é válida. Não apagar versões anteriores; Markdown pode ser versionado, imagens/vídeos e fontes grandes ficam locais fora do Git. Não reenviar material já recebido apenas para preencher uma lista.

[Central de necessidades](../NECESSIDADES-ATUAIS.md) · [Retestes](../../docs/projeto/RETESTES-ENTRADA-E-VIDEO.md) · [Escopo técnico entregue](../../docs/projeto/ESC-SIS-PVE-CONSOLIDACAO.md) · [Modelo](../MODELO-ENTREGA.md)

<details>
<summary>Guia anterior — histórico; não preencher novamente o que já foi respondido</summary>

> **Revisão 21/11/2026:** materiais recebidos desta área reconciliados na [consolidação vigente](../../docs/projeto/CONSOLIDACAO-2026-11-21.md). Não reenviar respostas ENT já fechadas. Novos arquivos finais podem continuar em recebidos; implementação e testes permanecem separados. Prioridade atual: abertura padrão Unreal; depois fila AUT.

# Abertura, vídeo e cenas de evento

**Atualização20/11:** material desta área estudado; [matriz e pendências específicas](../../docs/projeto/REVISAO-ENTREGAS-2026-11-20.md). Guia abaixo preservado como roteiro; não precisa responder novamente o que já entregou. Revisão integrada ao planejamento, não ao código.

Prioridade: **P2 — contrato antes do player**. Guia de preparação, não autorização de implementação automática.

## Já existente — não reenviar decisões fechadas

Vídeos paisagem/retrato e arte lateral já recebidos. Política de proporção existe; player e rotação real não implementados.

## O que você pode reunir ou responder

- [ ] Defina quando reproduzir: toda abertura, primeira abertura ou botão; como pular e para qual tela seguir.
- [ ] Defina áudio, legenda, pausa, chamada/interrupção, aplicativo em segundo plano e retomada.
- [ ] Escolha a prioridade quando faltar vídeo da orientação: girar dispositivo, conter com barras ou usar composição lateral; decisões já registradas permanecem referência.
- [ ] Para versão nova, informe duração, resolução, orientação, trilha/legendas e fonte editável; pode enviar storyboard antes do vídeo.
- [ ] Descreva comportamento se arquivo faltar/corromper e se preferências de janela serão escolhidas antes do início.

## Trabalho de desenvolvimento restante

Player local, fallback, preferências da janela, proporção e orientação/restauração por plataforma; codec/tamanho/desempenho precisam de validação.

## Dependências e entrega

Roteiro e vídeo independem de combate completo; runtime depende do player e dispositivo.

Coloque documentos/imagens/vídeos desta área em **recebidos/**, nesta pasta. Pode entregar parcialmente; use NÃO SEI / PROPOSTA / REFERÊNCIA / DECISÃO DESEJADA. Copie o modelo geral se ajudar. Não sobrescreva versões anteriores. Desde21/11,Markdown recebido é versionado por autorização do usuário; imagens/vídeos ficam locais, fora doGit.

[Instruções gerais](../README.md) · [Modelo](../MODELO-ENTREGA.md) · [Pendências vigentes](../../docs/projeto/PENDENCIAS.md).

</details>


</details>