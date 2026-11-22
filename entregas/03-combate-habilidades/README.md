# Combate, habilidades e efeitos — guia vigente

Atualizado em22/11/2026. **P1/P2 — dados e apresentação.** Fonte vigente: [mapa das famílias](../../docs/projeto/MAPA-FAMILIAS-E-PRIORIDADES.md). Conteúdo deste guia distingue preparação independente de implementação/aceite. Atualizações de arquivos recebidos ficam preservadas.

## Já recebido — não reenviar

- A1/A2/A3 do Felipe, Bleed/Poison, Standard/Silver, ponto fixo, RNG, controles e operações por categoria têm implementação testada.
- Silver também tem mínimo1 confirmado; não reenviar fórmulas, destino/desempate do escudo ou quotas já fechadas.

## Você pode preparar em paralelo

- Revisar nome/descrição/nível/ícone do kit recebido, indicando só diferenças.
- Para efeito novo: fonte, alvo, gatilho, borda, duração/relógio, recarga/custo, exceções e exemplo; não exigir o elenco completo.

## Trabalho do desenvolvimento — não precisa virar pesquisa sua

- Organizar catálogo, dados/validações e apresentação das habilidades sobre o executor pronto.
- Ajustar VIS/ENG/HUB antes de expandir profundamente o combate; sem inventar fórmula universal de cura/equipamento.

## Dependências e aceites

- Regras inéditas e atributos mutáveis precisam contrato; as bases encerradas não precisam nova pesquisa.
- Aceites visuais e provas em outras plataformas continuam separados dos testes nativos.

## Como entregar

Use recebidos/ deste grupo. Nome sugerido: AAAA-MM-DD_assunto_v02.md; identifique ID/versão, o que substitui, decisão/proposta/referência e origem. Entrega parcial é válida. Não apagar versões anteriores; Markdown pode ser versionado, imagens/vídeos e fontes grandes ficam locais fora do Git. Não reenviar material já recebido apenas para preencher uma lista.

[Central de necessidades](../NECESSIDADES-ATUAIS.md) · [Retestes](../../docs/projeto/RETESTES-ENTRADA-E-VIDEO.md) · [Escopo técnico entregue](../../docs/projeto/ESC-SIS-PVE-CONSOLIDACAO.md) · [Modelo](../MODELO-ENTREGA.md)

<details>
<summary>Guia anterior — histórico; não preencher novamente o que já foi respondido</summary>

> **Revisão 21/11/2026:** materiais recebidos desta área reconciliados na [consolidação vigente](../../docs/projeto/CONSOLIDACAO-2026-11-21.md). Não reenviar respostas ENT já fechadas. Novos arquivos finais podem continuar em recebidos; implementação e testes permanecem separados. Prioridade atual: abertura padrão Unreal; depois fila AUT.

# Core, habilidades, efeitos e fórmulas

**Atualização20/11:** material desta área estudado; [matriz e pendências específicas](../../docs/projeto/REVISAO-ENTREGAS-2026-11-20.md). Guia abaixo preservado como roteiro; não precisa responder novamente o que já entregou. Revisão integrada ao planejamento, não ao código.

Prioridade: **P1/P2 — antes de cada família**. Guia de preparação, não autorização de implementação automática.

## Já existente — não reenviar decisões fechadas

ESC-01–07, básica A1 não silenciada e causalidade R04 já definidos. TIME/RES/SEQ/PAR não precisam de revalidação genérica. Referências: PENDENCIAS REG-01/03 e SIS-01/02.

## O que você pode reunir ou responder

- [ ] Produza matriz de silêncio, supressão, congelamento e petrificação: pode agir, ativas bloqueadas, passivas afetadas, relógio e exceções.
- [ ] Para cada efeito novo, informe alvo, momento, duração, quem conta, coexistência, dissipação/roubo/imunidade e limite de ativações.
- [ ] Para fórmula, informe atributos usados, ordem, arredondamento, mínimo/teto e exemplo numérico; se desconhecido, marque hipótese.
- [ ] Para bloqueio automático futuro, defina origem/chance, por hit ou ataque, ataques elegíveis e efeitos ao acertar; não confundir com Defesa clássica.
- [ ] Priorize 3–5 fichas originais pequenas, com básica, segunda posição ativa/passiva e terceira habilidade; indique custo/condição/recarga.
- [ ] Se pesquisar Dislyte, anote fonte, data/patch e teste observável; separar referência externa de regra escolhida para o nosso jogo.

## Trabalho de desenvolvimento restante

Implementar famílias por contrato/testes; cura, atributos mutáveis, fórmulas e supressão ainda exigem trabalho próprio. Converter ficha revisada em dados executáveis depois.

## Dependências e entrega

Fichas e pesquisa independem de cena final. Não ampliar catálogo inteiro antes de validar amostra.

Coloque documentos/imagens/vídeos desta área em **recebidos/**, nesta pasta. Pode entregar parcialmente; use NÃO SEI / PROPOSTA / REFERÊNCIA / DECISÃO DESEJADA. Copie o modelo geral se ajudar. Não sobrescreva versões anteriores. Desde21/11,Markdown recebido é versionado por autorização do usuário; imagens/vídeos ficam locais, fora doGit.

[Instruções gerais](../README.md) · [Modelo](../MODELO-ENTREGA.md) · [Pendências vigentes](../../docs/projeto/PENDENCIAS.md).

</details>
