> **Atualização22/11:** código ESC/SIS/PvE implementado; veja docs/projeto/ESC-SIS-PVE-CONSOLIDACAO.md na raiz. Não reenviar Silver/ondas/score fechados. O que ajuda: retestes clique/alvo/vídeo, propostas de menus e dados finais dos modos/chefes com gatilho, alcance, kit, duração, limites e exemplos, sem exigir catálogo inteiro.

# Core, habilidades e fórmulas — guia vigente

Atualizado em **22/11/2026**. Prioridade: **aceite do piloto Felipe e planejamento posterior de ESC/SIS/PvE**. Este guia substitui as perguntas antigas nos temas já respondidos; o histórico abaixo fica apenas para referência.

## Já recebido ou definido — não reenviar

StandardShield, R04, regras de A2 do Felipe, Block por hit, ACC/RESIST e ponto fixo já têm decisões recebidas. SIS-02-B/C, funil, controles, Bleed/Poison e A2/piloto implementados/testados no recorte local.

## O que você pode adiantar agora

- [ ] Não repetir chance de Block, destino/desempate do escudo da A2, contagem multi-hit, teto numérico ou arredondamento já fechados.
- [ ] Se reunir fórmulas ainda não fechadas, especialmente DEF/cura, informar entradas, ordem, exceções e exemplo numérico; separar referência externa de regra desejada.
- [ ] Para habilidade fora do recorte conhecido, fornecer somente a ficha nova/delta: alvo, gatilho, duração/relógio, recarga, limites e exemplo. Felipe continua o piloto; não é preciso produzir elenco inteiro.

## Trabalho do desenvolvimento — não precisa virar pesquisa sua

Ponto1 entregue. Próximo grupo possível: escudos especializados, operadores de efeitos/ondas e score, em entregas próprias. Não reenviar regras fechadas dos DoTs.

## Testes e aceites ainda pendentes

Piloto: Abrir-Piloto-Felipe.cmd e Abrir-Tutorial-Felipe.cmd. Aceite visual, balanceamento, tutorial guiado e determinismo medido em Android/iOS continuam pendentes.

## Onde entregar

Use **recebidos/** deste grupo; para revisão, crie versão nova e cite o arquivo substituído sem apagar o original. Informe ID/versão, finalidade, o que mudou, fonte/autoria e resultado esperado quando cabível. Entrega parcial é válida e não autoriza implementação, instalação ou execução automática.

[Resumo de necessidades](../NECESSIDADES-ATUAIS.md) · [Ordem de execução](../../docs/projeto/ORDEM-EXECUCAO-POS-AUT.md) · [Mapa SIS-02-A](../../docs/projeto/SIS-02-A-MAPA-NUMERICO.md) · [Modelo](../MODELO-ENTREGA.md)

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
