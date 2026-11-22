# Progressão, inventário e save — guia vigente

Atualizado em **22/11/2026**. Prioridade: **P2 — depois do núcleo; aceites agora**. Este guia substitui as perguntas antigas nos temas já respondidos; o histórico abaixo fica apenas para referência.

## Já recebido ou definido — não reenviar

CIC-01 e AUT-09 têm equipe, recompensa provisória idempotente e save v1 entre batalhas. Save v1 não contém HP/escudo/AP; SIS-02-A não exige v2 só por ampliar HP.

## O que você pode adiantar agora

- [ ] Quando puder testar: salvar, fechar/reabrir a prévia e carregar; relatar equipe/saldo e nova tentativa, sem restauração de batalha nem crédito duplicado.
- [ ] Para economia/inventário novos, enviar apenas os detalhes não cobertos nos documentos recebidos: fontes/sumidouros, limites, equipamentos e exemplos. Identificar valores provisórios.
- [ ] Uma proposta de save de combate/cloud é mudança de escopo: explicar necessidade, não enviar credenciais ou dados reais de conta.

## Trabalho do desenvolvimento — não precisa virar pesquisa sua

Preservar ledger e compatibilidade explícita na migração; score não é credits. Não mudar schema/recompensas ou migrar arquivo antigo silenciosamente.

## Testes e aceites ainda pendentes

I/O real do save/preferences/flag na Unreal e recuperação de falhas continuam com aceites próprios. Inventário/economia ampla não estão implementados.

## Onde entregar

Use **recebidos/** deste grupo; para revisão, crie versão nova e cite o arquivo substituído sem apagar o original. Informe ID/versão, finalidade, o que mudou, fonte/autoria e resultado esperado quando cabível. Entrega parcial é válida e não autoriza implementação, instalação ou execução automática.

[Resumo de necessidades](../NECESSIDADES-ATUAIS.md) · [Ordem de execução](../../docs/projeto/ORDEM-EXECUCAO-POS-AUT.md) · [Mapa SIS-02-A](../../docs/projeto/SIS-02-A-MAPA-NUMERICO.md) · [Modelo](../MODELO-ENTREGA.md)

<details>
<summary>Guia anterior — histórico; não preencher novamente o que já foi respondido</summary>

> **Revisão 21/11/2026:** materiais recebidos desta área reconciliados na [consolidação vigente](../../docs/projeto/CONSOLIDACAO-2026-11-21.md). Não reenviar respostas ENT já fechadas. Novos arquivos finais podem continuar em recebidos; implementação e testes permanecem separados. Prioridade atual: abertura padrão Unreal; depois fila AUT.

# Progressão, economia, equipamentos e saves

**Atualização20/11:** material desta área estudado; [matriz e pendências específicas](../../docs/projeto/REVISAO-ENTREGAS-2026-11-20.md). Guia abaixo preservado como roteiro; não precisa responder novamente o que já entregou. Revisão integrada ao planejamento, não ao código.

Prioridade: **P2/P3**. Guia de preparação, não autorização de implementação automática.

## Já existente — não reenviar decisões fechadas

Save v1 entre batalhas, seleção e recompensa de laboratório existem; não restauram combate. Sem economia/inventário completos.

## O que você pode reunir ou responder

- [ ] Descreva ciclo do jogador: entrar, escolher fase, gastar recurso, lutar, receber recompensa e desbloquear.
- [ ] Liste moedas, itens/estamina, limites, recuperação, fontes/sumidouros; números iniciais podem ser provisórios identificados.
- [ ] Para equipamento, informe atributos, slots, exclusividade, melhoria e efeito; separar atributo de proteção clássica de escudo por habilidade.
- [ ] Defina o que salvar: equipe/progresso/configuração ou também combate; quando salvar e o que fazer ao fechar no meio.
- [ ] Informe comportamento offline, migração de versões e conflito de progresso, sem exigir conta/cloud para o laboratório.

## Trabalho de desenvolvimento restante

Modelos de dados, inventário/economia, codec e migração; save de runtime integrado e eventual snapshot de batalha separado.

## Dependências e entrega

Proposta independente agora; balanceamento depende de conteúdo/regras; conta online é frente posterior.

Coloque documentos/imagens/vídeos desta área em **recebidos/**, nesta pasta. Pode entregar parcialmente; use NÃO SEI / PROPOSTA / REFERÊNCIA / DECISÃO DESEJADA. Copie o modelo geral se ajudar. Não sobrescreva versões anteriores. Desde21/11,Markdown recebido é versionado por autorização do usuário; imagens/vídeos ficam locais, fora doGit.

[Instruções gerais](../README.md) · [Modelo](../MODELO-ENTREGA.md) · [Pendências vigentes](../../docs/projeto/PENDENCIAS.md).

</details>
