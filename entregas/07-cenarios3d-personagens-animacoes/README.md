## Estado e dependências — 23/11/2026

- **O que posso fazer.** Refinar materiais dos sete destinos, movimento ambiental, câmera e presença provisória com fallback leve.
- **O que você pode entregar.** Uma amostra de personagem com rig, materiais e clips, quando disponível.
- **Aceite ou condição para avançar.** Aceitar materiais e formato antes do mapa editável. Personagem e animação finais dependem de assets.

[Central de entregas](../NECESSIDADES-ATUAIS.md). As notas abaixo preservam o histórico.

# 3D e animação — guia vigente

Atualizado em **22/11/2026**. Demo **~28%**. Sua preparação e o desenvolvimento são frentes paralelas: entregar amostras é suficiente; nada aqui pede para refazer material já definido.

## Sua próxima contribuição

Uma amostra Felipe/Umbra + Terraço: modelo, unidade/escala/pivô, rig, materiais/texturas, lista de clips e origem. Não precisa cidade/elenco inteiro.

## O que posso desenvolver sem sua próxima entrega

Contrato de importação, IDs e cues; placeholders para todos os modos.

## O que depende de aceite ou recurso final

Visual final precisa asset/rig/clips; fluidez real de animação pede aceite após integração.

## Como entregar

Coloque em recebidos/ desta pasta. Nome: AAAA-MM-DD_assunto_v02.md, com ID, versão, substituição/delta, decisão ou hipótese e origem. Pode enviar uma parte pronta por vez. Não apagar versões anteriores. Markdown pode ir ao Git; imagens, vídeos e recursos grandes continuam locais. Não incluir credenciais/dados pessoais.

Cliques únicos, vídeo/skip, esferas e seletor já foram aceitos no recorte Windows testado. Novos HUDs, save real e mobile têm aceites separados. [Central](../NECESSIDADES-ATUAIS.md) · [Progresso](../../docs/projeto/PROGRESSO-GERAL.md) · [Minha fila](../../docs/projeto/FILA-DESENVOLVIMENTO-INDEPENDENTE.md).

<details>
<summary>Guia anterior e histórico — não repetir pedidos já atendidos</summary>

# Cenários 3D, personagem e animação — guia vigente

Atualizado em22/11/2026. **P2 — amostra independente.** Fonte vigente: [mapa das famílias](../../docs/projeto/MAPA-FAMILIAS-E-PRIORIDADES.md). Conteúdo deste guia distingue preparação independente de implementação/aceite. Atualizações de arquivos recebidos ficam preservadas.

## Já recebido — não reenviar

- Direção do Terraço da Vigília e Felipe/Umbra recebida; não pedir novamente protagonista ou arena.
- Escala proposta 1unidade=1cm, material PBR parametrizado e referências frente/lado/costas já orientam produção; propostas técnicas não são orçamento mobile validado.

## Você pode preparar em paralelo

- Entregar uma amostra de personagem/arena, não cidade/elenco inteiro: fonte editável e exportação existente FBX/glTF, escala, pivô/pés, rig e materiais/texturas.
- Clips mínimos idle/ação/impacto/derrota/vitória com início/contato/fim e esqueleto associado. Informar polígonos/texturas quando conhecidos.
- Separar itens fixos/modulares e fases visuais Umbra/Luz/Natureza sem duplicar ficha de personagem.

## Trabalho do desenvolvimento — não precisa virar pesquisa sua

- Preparar importação/IDs, placeholders e ligação dos cues Begin/Impact/State/End.
- Animação não aplica dano nem muda relógio; validar um recurso antes de produzir lotes.

## Dependências e aceites

- Rig precede animação final; orçamento gráfico depende de ensaio no aparelho.
- Imagem/conceito não é malha/rig pronto. Guardar fontes grandes fora do Git e origem no grupo11.

## Como entregar

Use recebidos/ deste grupo. Nome sugerido: AAAA-MM-DD_assunto_v02.md; identifique ID/versão, o que substitui, decisão/proposta/referência e origem. Entrega parcial é válida. Não apagar versões anteriores; Markdown pode ser versionado, imagens/vídeos e fontes grandes ficam locais fora do Git. Não reenviar material já recebido apenas para preencher uma lista.

[Central de necessidades](../NECESSIDADES-ATUAIS.md) · [Retestes](../../docs/projeto/RETESTES-ENTRADA-E-VIDEO.md) · [Escopo técnico entregue](../../docs/projeto/ESC-SIS-PVE-CONSOLIDACAO.md) · [Modelo](../MODELO-ENTREGA.md)

<details>
<summary>Guia anterior — histórico; não preencher novamente o que já foi respondido</summary>

> **Revisão 21/11/2026:** materiais recebidos desta área reconciliados na [consolidação vigente](../../docs/projeto/CONSOLIDACAO-2026-11-21.md). Não reenviar respostas ENT já fechadas. Novos arquivos finais podem continuar em recebidos; implementação e testes permanecem separados. Prioridade atual: abertura padrão Unreal; depois fila AUT.

# Cenários 3D, personagens e animações

**Atualização20/11:** material desta área estudado; [matriz e pendências específicas](../../docs/projeto/REVISAO-ENTREGAS-2026-11-20.md). Guia abaixo preservado como roteiro; não precisa responder novamente o que já entregou. Revisão integrada ao planejamento, não ao código.

Prioridade: **P2 — começar pequeno**. Guia de preparação, não autorização de implementação automática.

## Já existente — não reenviar decisões fechadas

Arena usa formas e câmeras provisórias; personagens/rig/animações finais não implementados.

## O que você pode reunir ou responder

- [ ] Proponha uma arena pequena e um personagem original para a primeira fatia jogável, antes de produzir elenco/cidade inteira.
- [ ] Envie conceitos frente/lado/costas quando possível, escala, materiais, fonte editável e exportação disponível; não precisa escolher ferramenta nova.
- [ ] Se já houver modelo, informe polígonos, texturas, esqueleto/rig e licença, sem assumir orçamento mobile definitivo.
- [ ] Liste idle, ataque, impacto, derrota e vitória; marque início, contato e fim, distinguindo duração visual de tempo da regra.
- [ ] Indique câmera/enquadramento, chão de contato, origem/pivô e itens modulares; não substituir a apresentação clássica preservada.

## Trabalho de desenvolvimento restante

Pipeline de importação, materiais/luz, sincronização por eventos, animação e otimização; validar primeiro recurso antes de lote.

## Dependências e entrega

Conceito/cena independente agora; rig precede animação final; orçamento depende de ensaio mobile.

Coloque documentos/imagens/vídeos desta área em **recebidos/**, nesta pasta. Pode entregar parcialmente; use NÃO SEI / PROPOSTA / REFERÊNCIA / DECISÃO DESEJADA. Copie o modelo geral se ajudar. Não sobrescreva versões anteriores. Desde21/11,Markdown recebido é versionado por autorização do usuário; imagens/vídeos ficam locais, fora doGit.

[Instruções gerais](../README.md) · [Modelo](../MODELO-ENTREGA.md) · [Pendências vigentes](../../docs/projeto/PENDENCIAS.md).

</details>


</details>