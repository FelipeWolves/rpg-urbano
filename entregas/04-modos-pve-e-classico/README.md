> **Integração23/11:** recursos ligados à HUD; tutorial padrão no Terminal; busca clássica no Lab - Exp; conclusão local própria, PRAÇA/letreiros corrigidos. Código/testes/build e stage92 aprovados; aceites gráficos/I-O real/mobile separados. Demo~28% mantida. Fonte: docs/projeto/INTEGRACAO-VIS-TUTORIAIS-2026-11-23.md. Próximo: aceite da versão integrada, sem depender da arte futura do Terminal.

> **Ícones22/11:** 17ícones transparentes preparados em assets/runtime/ui/2026-11-22-v02/, logo/avatar nomeados e catálogo conferido; não integrados à HUD. Sangramento inicial do inimigo aprovado para tutorial e validado no núcleo; sequência guiada/variante/save ainda pendentes. Fonte docs/projeto/VIS-ICONS-PNG-2026-11-22.md. Demo~28% mantida.

> **Recebidos22/11:** 5documentos e19imagens novos revisados; originais preservados, sem integração. Tutorial padrão requer fechar debuff prévio para Sangramento, ordem guiada, variante jovem e persistência isolada. Ícones JPG/caminhos e status353×307 diferem das fichas; avatar recebido chama-se portrait-felipe-v01.png. Ver docs/projeto/RECEBIMENTO-NOVIDADES-2026-11-22.md. Demo~28% mantida.

> **22/11 — LAB-PVE-06:** página3doTerminal com chefe único/três frentes, variantes local/global. Roteiro em docs/projeto/LAB-PVE-06-FRENTES-COMPARTILHADAS.md. Aceites02–06 pendentes; próximo recomendado QA/feedback. Recursos finais podem continuar em paralelo.

> **22/11 — LAB-PVE-05:** página2doTerminal com duas variantes de3equipes sequenciais. Roteiro em docs/projeto/LAB-PVE-05-EQUIPES-SEQUENCIAIS.md: equipe ativa, HP contínuo, fase fixa/variável, janela e resultado. Aceites02–05 pendentes; próximo06independe de arte final.

> **22/11 — LAB-PVE-04:** Terminal → Mais modos > → Pontuação/Sobrevivência. Roteiro em docs/projeto/LAB-PVE-04-SCORE-SOBREVIVENCIA.md; testar paginação, contadores e consulta de recibo, sem concessão de créditos. Fixtures não esperam conteúdo final; próximo05equipes sequenciais. Aceites02–04 ainda pendentes.

> **22/11 — LAB-PVE-03:** novos testes de Ondas consecutivas e Ritual com fases no Terminal, aguardando seu aceite conforme docs/projeto/LAB-PVE-03-ONDAS-FASES.md. Recursos finais não bloqueiam o próximo LAB-PVE-04. Ritual simples também segue com aceite pendente.

> **22/11 — LAB-PVE-02:** Ritual disponível pelo Terminal na compilação atual; roteiro em docs/projeto/LAB-PVE-02-RITUAL-TERMINAL.md. Sua próxima avaliação é da nova rota/HUD, resultado, abandono e volta aos outros laboratórios. Conteúdo final pode continuar em paralelo. Próximo desenvolvimento: ondas/fases (LAB-PVE-03).

> **22/11 — desenvolvimento:** LAB-PVE-01 entregue em código; próximo Ritual no Terminal (LAB-PVE-02) sem esperar ficha final/arte. Sua ficha de encontro poderá substituir os valores provisórios depois. Nenhuma nova tela disponível nesta etapa.

# Modos e encontros — guia vigente

Atualizado em **22/11/2026**. Demo **~28%**. Sua preparação e o desenvolvimento são frentes paralelas: entregar amostras é suficiente; nada aqui pede para refazer material já definido.

## Sua próxima contribuição

Prioridade: uma ficha de encontro representativo com objetivo, vitória/derrota/limite, ondas/fases/times, kit e valores finais ou faixas.

## O que posso desenvolver sem sua próxima entrega

LAB-PVE-01–06: adaptadores, Ritual, ondas/fases, score/sobrevivência, sequenciais e frentes locais, com fixtures.

## O que depende de aceite ou recurso final

Novas regras só após definição; telas novas pedem aceite. Clássico preservado, fora desta frente.

## Como entregar

Coloque em recebidos/ desta pasta. Nome: AAAA-MM-DD_assunto_v02.md, com ID, versão, substituição/delta, decisão ou hipótese e origem. Pode enviar uma parte pronta por vez. Não apagar versões anteriores. Markdown pode ir ao Git; imagens, vídeos e recursos grandes continuam locais. Não incluir credenciais/dados pessoais.

Cliques únicos, vídeo/skip, esferas e seletor já foram aceitos no recorte Windows testado. Novos HUDs, save real e mobile têm aceites separados. [Central](../NECESSIDADES-ATUAIS.md) · [Progresso](../../docs/projeto/PROGRESSO-GERAL.md) · [Minha fila](../../docs/projeto/FILA-DESENVOLVIMENTO-INDEPENDENTE.md).

<details>
<summary>Guia anterior e histórico — não repetir pedidos já atendidos</summary>

# Modos PvE e clássico — guia vigente

Atualizado em22/11/2026. **P1 integração simples; expansões depois.** Fonte vigente: [mapa das famílias](../../docs/projeto/MAPA-FAMILIAS-E-PRIORIDADES.md). Conteúdo deste guia distingue preparação independente de implementação/aceite. Atualizações de arquivos recebidos ficam preservadas.

## Já recebido — não reenviar

- WaveV2, Ritual/ScoreAttack/Survival, score/recibo, fases, reservas, SEQv2 e áreas locais/globais testados no core.
- Três equipes sem retorno, HP global e auxiliares separados estão modelados; kits estáveis continuam possíveis.

## Você pode preparar em paralelo

- Escolher/entregar uma ficha representativa de encontro: objetivo, formação, inimigos/auxiliares, limite e kit; usar o que já foi entregue como base.
- Para novo limiar/kit/faixa, informar delta e exemplos. Não precisamos de relatório de todos os chefes de uma vez.

## Trabalho do desenvolvimento — não precisa virar pesquisa sua

- Depois de VIS/ENG/HUB, integrar um Ritual ao menu/HUD; depois ondas e score/resultado, antes de SEQ/PAR.
- Clássico com itens/SP/Limit/ATB Active e DJ têm recortes próprios; não confundir defesa existente com modo completo.

## Dependências e aceites

- Classes compiladas não criam telas novas automaticamente.
- Combinação de ciclo por turnos e lista de fases precisa precedência definida; balanceamento/recompensas finais são dados próprios.

## Como entregar

Use recebidos/ deste grupo. Nome sugerido: AAAA-MM-DD_assunto_v02.md; identifique ID/versão, o que substitui, decisão/proposta/referência e origem. Entrega parcial é válida. Não apagar versões anteriores; Markdown pode ser versionado, imagens/vídeos e fontes grandes ficam locais fora do Git. Não reenviar material já recebido apenas para preencher uma lista.

[Central de necessidades](../NECESSIDADES-ATUAIS.md) · [Retestes](../../docs/projeto/RETESTES-ENTRADA-E-VIDEO.md) · [Escopo técnico entregue](../../docs/projeto/ESC-SIS-PVE-CONSOLIDACAO.md) · [Modelo](../MODELO-ENTREGA.md)

<details>
<summary>Guia anterior — histórico; não preencher novamente o que já foi respondido</summary>

> **Revisão 21/11/2026:** materiais recebidos desta área reconciliados na [consolidação vigente](../../docs/projeto/CONSOLIDACAO-2026-11-21.md). Não reenviar respostas ENT já fechadas. Novos arquivos finais podem continuar em recebidos; implementação e testes permanecem separados. Prioridade atual: abertura padrão Unreal; depois fila AUT.

# Chefes, encontros, eventos e modo clássico

**Atualização20/11:** material desta área estudado; [matriz e pendências específicas](../../docs/projeto/REVISAO-ENTREGAS-2026-11-20.md). Guia abaixo preservado como roteiro; não precisa responder novamente o que já entregou. Revisão integrada ao planejamento, não ao código.

Prioridade: **P2**. Guia de preparação, não autorização de implementação automática.

## Já existente — não reenviar decisões fechadas

Perfis SEQ/PAR existentes preservados; chefe compartilhado HP único/barra global é um perfil, não todos os chefes. Referências SIS-03/04, MODOS-PVE e roteiro de chefes.

## O que você pode reunir ou responder

- [ ] Para cada modo, informe equipes/unidades, simultâneo ou sequencial, vida compartilhada ou separada e condição de término.
- [ ] Defina se vence por eliminar, sobreviver ou pontuar; limite de turnos, contador responsável e fórmula de pontuação/recompensa se conhecida.
- [ ] Descreva fases do chefe: gatilho por HP, turno ou troca de equipe, habilidades que mudam e o que persiste.
- [ ] Diferencie auxiliares, invocações e chefe; quem pode atingir qual frente, e se equipes retornam ao combate.
- [ ] Para clássico, proponha itens/estamina, atributo de proteção, especial/quarta habilidade e ATB Active/Wait em ficha separada.

## Trabalho de desenvolvimento restante

Novos coordenadores/políticas, alvos em área, score, fases por limiar e exceções explícitas; não alterar perfis fechados implicitamente.

## Dependências e entrega

Especificação pode chegar agora; execução depende das regras usadas por cada modo.

Coloque documentos/imagens/vídeos desta área em **recebidos/**, nesta pasta. Pode entregar parcialmente; use NÃO SEI / PROPOSTA / REFERÊNCIA / DECISÃO DESEJADA. Copie o modelo geral se ajudar. Não sobrescreva versões anteriores. Desde21/11,Markdown recebido é versionado por autorização do usuário; imagens/vídeos ficam locais, fora doGit.

[Instruções gerais](../README.md) · [Modelo](../MODELO-ENTREGA.md) · [Pendências vigentes](../../docs/projeto/PENDENCIAS.md).

</details>


</details>
