## Estado e dependências — 23/11/2026

- **O que posso fazer.** Preparar validação em janela, acessibilidade e QA de navegação, HUD, tutoriais e PvE.
- **O que você pode entregar.** Depois de receber o roteiro, anotar somente falhas ou resultados do teste.
- **Aceite ou condição para avançar.** Aceite visual em janela e validação de gravação, reabertura e recuperação.

[Central de entregas](../NECESSIDADES-ATUAIS.md). As notas abaixo preservam o histórico.

> **Halo/mouse23/11:** glow por mascara Gaussiana dos glifos, texto nitido; GameOnly substituido por GameAndUI/DoNotLock e foco explicito. Fonte/brilho anterior preservados. Ponteiro10/10, build e stage101 aprovados; confinamento real/clique/halo aguardam reteste. Fonte docs/projeto/HUB-HALO-E-MOUSE-2026-11-23.md. Demo~28% mantida.

> **Reteste23/11:** usuário aceitou logo, PRAÇA e direção tipográfica. Letreiros agora Light/menores, hover só luminoso no padrão; alto contraste preservado. Content/Presentation excluído da importação automática apenas no projeto. Build/stage93 aprovados; acabamento e supressão do aviso aguardam reteste. Fonte docs/projeto/HUB-LETREIROS-E-IMPORTACAO-2026-11-23.md. Demo~28% mantida.

> **Integração23/11:** recursos ligados à HUD; tutorial padrão no Terminal; busca clássica no Lab - Exp; conclusão local própria, PRAÇA/letreiros corrigidos. Código/testes/build e stage92 aprovados; aceites gráficos/I-O real/mobile separados. Demo~28% mantida. Fonte: docs/projeto/INTEGRACAO-VIS-TUTORIAIS-2026-11-23.md. Próximo: aceite da versão integrada, sem depender da arte futura do Terminal.

> **LAB-QA-01C — 22/11:** persistência compartilhada entre HUD Unreal e QA; 16/16 testes isolados, compilação e stage69 aprovados. Formatos/políticas preservados; saves normais intocados. Aceites LQA-09/10 e runtime Unreal isolado de LQA-14 continuam separados. Demo ~28% mantida. Fonte: docs/projeto/LAB-QA-01C-PERSISTENCIA-COMPARTILHADA.md. Próxima frente independente proposta: revisar contratos de áudio/animação sobre eventos existentes, sem repetir AUT-13.

> **LAB-QA-01B — 22/11:** testes de disco em pasta nova de QA, com codecs reais e backend Windows próprio; saves normais intocados. Não equivale a executar IFileManager/interface Unreal. LQA-14 de integração continua adiado. Próximo independente LAB-QA-01C: contrato compartilhado de persistência; aceites visuais em paralelo. Demo ~28% mantida. Detalhes: docs/projeto/LAB-QA-01B-IO-ISOLADO.md.

> **LAB-QA-01 — 22/11:** feedback de ações e recusas integrado; detalhes consultáveis e matriz de 16 aceites preparada. Código/testes não equivalem a aceite visual ou I/O real. Próximo independente: LAB-QA-01B, I/O em diretório isolado; usuário pode preencher a matriz em paralelo. Demo ~28% mantida. Ver docs/projeto/LAB-QA-01-FEEDBACK-E-ACEITES.md.

> **22/11 — LAB-PVE-06:** página3doTerminal com chefe único/três frentes, variantes local/global. Roteiro em docs/projeto/LAB-PVE-06-FRENTES-COMPARTILHADAS.md. Aceites02–06 pendentes; próximo recomendado QA/feedback. Recursos finais podem continuar em paralelo.

> **22/11 — LAB-PVE-05:** página2doTerminal com duas variantes de3equipes sequenciais. Roteiro em docs/projeto/LAB-PVE-05-EQUIPES-SEQUENCIAIS.md: equipe ativa, HP contínuo, fase fixa/variável, janela e resultado. Aceites02–05 pendentes; próximo06independe de arte final.

> **22/11 — LAB-PVE-04:** Terminal → Mais modos > → Pontuação/Sobrevivência. Roteiro em docs/projeto/LAB-PVE-04-SCORE-SOBREVIVENCIA.md; testar paginação, contadores e consulta de recibo, sem concessão de créditos. Fixtures não esperam conteúdo final; próximo05equipes sequenciais. Aceites02–04 ainda pendentes.

> **22/11 — LAB-PVE-03:** novos testes de Ondas consecutivas e Ritual com fases no Terminal, aguardando seu aceite conforme docs/projeto/LAB-PVE-03-ONDAS-FASES.md. Recursos finais não bloqueiam o próximo LAB-PVE-04. Ritual simples também segue com aceite pendente.

> **22/11 — LAB-PVE-02:** Ritual disponível pelo Terminal na compilação atual; roteiro em docs/projeto/LAB-PVE-02-RITUAL-TERMINAL.md. Sua próxima avaliação é da nova rota/HUD, resultado, abandono e volta aos outros laboratórios. Conteúdo final pode continuar em paralelo. Próximo desenvolvimento: ondas/fases (LAB-PVE-03).

# Interface e aceites — guia vigente

Atualizado em **22/11/2026**. Demo **~28%**. Sua preparação e o desenvolvimento são frentes paralelas: entregar amostras é suficiente; nada aqui pede para refazer material já definido.

## Sua próxima contribuição

Preparar testes específicos de save/reabertura, tutorial e telas amplas quando houver roteiro. Não repetir clique/vídeo/seletor aceitos.

## O que posso desenvolver sem sua próxima entrega

Integrar HUD PvE e feedback com placeholders; montar roteiros de QA.

## O que depende de aceite ou recurso final

Você avalia cada tela nova depois; não depende de arte final.

## Como entregar

Coloque em recebidos/ desta pasta. Nome: AAAA-MM-DD_assunto_v02.md, com ID, versão, substituição/delta, decisão ou hipótese e origem. Pode enviar uma parte pronta por vez. Não apagar versões anteriores. Markdown pode ir ao Git; imagens, vídeos e recursos grandes continuam locais. Não incluir credenciais/dados pessoais.

Cliques únicos, vídeo/skip, esferas e seletor já foram aceitos no recorte Windows testado. Novos HUDs, save real e mobile têm aceites separados. [Central](../NECESSIDADES-ATUAIS.md) · [Progresso](../../docs/projeto/PROGRESSO-GERAL.md) · [Minha fila](../../docs/projeto/FILA-DESENVOLVIMENTO-INDEPENDENTE.md).

<details>
<summary>Guia anterior e histórico — não repetir pedidos já atendidos</summary>

> **22/11 — retorno mais recente:** cliques únicos e hover aceitos. Próximo reteste: Arena PvP → Testar Felipe (local), depois voltar ao Terminal; barra de skip somente na condição ainda não assistido. Ver docs/projeto/RETESTES-ENTRADA-E-VIDEO.md.

> **22/11 — candidato a reteste:** correções de entrada/alvos e preparação da mídia em código. Testar primeira abertura/Rever e troca de alvo; aceite visual ainda pendente. Ver docs/projeto/VIS-ENG-HUB-P0-CORRECOES.md. Não é necessário reenviar os recursos já recebidos.

# Interface e retestes — guia vigente

Atualizado em22/11/2026. **P0 — ajustes confirmados.** Fonte vigente: [mapa das famílias](../../docs/projeto/MAPA-FAMILIAS-E-PRIORIDADES.md). Conteúdo deste guia distingue preparação independente de implementação/aceite. Atualizações de arquivos recebidos ficam preservadas.

## Já recebido — não reenviar

- Cliques da batalha/troca de alvo e primeiro Rever estão REABERTOS; não são considerados aceitos.
- Geometria, marcador e layouts possuem bases anteriores; não pedir nova especificação de todos os controles.

## Você pode preparar em paralelo

- Após receber a compilação corrigida: testar habilidade de alvo único, trocar A→B antes de confirmar e clicar uma vez.
- Informar atalho usado, janela/PIE, resolução/escala e passos. Testar primeiro Rever e uma repetição; sem exigir gravação.

## Trabalho do desenvolvimento — não precisa virar pesquisa sua

- Diagnosticar foco, origem/soltura do gesto, consumo do clique e seleção do ator; manter regra de ação no core.
- Adicionar verificações locais dos casos e compilar o adaptador, sem controlar a tela.

## Dependências e aceites

- RET-INPUT-01 e RET-MEDIA-01 só recebem aceite depois de reteste real.
- Não confundir área que atinge todos com alvo único, nem clique de foco com confirmação.

## Como entregar

Use recebidos/ deste grupo. Nome sugerido: AAAA-MM-DD_assunto_v02.md; identifique ID/versão, o que substitui, decisão/proposta/referência e origem. Entrega parcial é válida. Não apagar versões anteriores; Markdown pode ser versionado, imagens/vídeos e fontes grandes ficam locais fora do Git. Não reenviar material já recebido apenas para preencher uma lista.

[Central de necessidades](../NECESSIDADES-ATUAIS.md) · [Retestes](../../docs/projeto/RETESTES-ENTRADA-E-VIDEO.md) · [Escopo técnico entregue](../../docs/projeto/ESC-SIS-PVE-CONSOLIDACAO.md) · [Modelo](../MODELO-ENTREGA.md)

<details>
<summary>Guia anterior — histórico; não preencher novamente o que já foi respondido</summary>

> **Revisão 21/11/2026:** materiais recebidos desta área reconciliados na [consolidação vigente](../../docs/projeto/CONSOLIDACAO-2026-11-21.md). Não reenviar respostas ENT já fechadas. Novos arquivos finais podem continuar em recebidos; implementação e testes permanecem separados. Prioridade atual: abertura padrão Unreal; depois fila AUT.

# Interface, acessibilidade e testes

**Atualização20/11:** material desta área estudado; [matriz e pendências específicas](../../docs/projeto/REVISAO-ENTREGAS-2026-11-20.md). Guia abaixo preservado como roteiro; não precisa responder novamente o que já entregou. Revisão integrada ao planejamento, não ao código.

Prioridade: **P1 — primeiro**. Guia de preparação, não autorização de implementação automática.

## Já existente — não reenviar decisões fechadas

Texto sem corte aceito; seleção/batalha funcionais no relato. Seletor no chão corrigido e aceito visualmente pelo usuário. VIS-01/02/03 mantêm aceite parcial.

## O que você pode reunir ou responder

- [x] Seletor no chão corrigido e aceito; não precisa reenviar prova nem nova preferência para fechar esse defeito.
- [ ] Para fluidez, descreva uma sequência concreta: o que ocorre hoje, o que espera, qual espera/transição incomoda; vídeo curto é opcional.
- [ ] Informe resolução, escala do Windows e tamanho da janela nos problemas visuais; não precisa enviar dados pessoais.
- [ ] Liste textos que ficam difíceis de ler, estados sem indicação e gestos desejados de clique/toque/segurar.
- [ ] Sugira comportamento de teclado/controle, tamanho de texto, contraste e opção sem movimento.

## Trabalho de desenvolvimento restante

Corrigir posição do marcador; verificar efeitos/detalhes/alvos, 1/3/5 unidades, resoluções, foco e toque. Separar feedback visual de alteração de regra/velocidade de combate.

## Dependências e entrega

Referências podem chegar agora. Testes em aparelho dependem do alvo mobile.

Coloque documentos/imagens/vídeos desta área em **recebidos/**, nesta pasta. Pode entregar parcialmente; use NÃO SEI / PROPOSTA / REFERÊNCIA / DECISÃO DESEJADA. Copie o modelo geral se ajudar. Não sobrescreva versões anteriores. Desde21/11,Markdown recebido é versionado por autorização do usuário; imagens/vídeos ficam locais, fora doGit.

[Instruções gerais](../README.md) · [Modelo](../MODELO-ENTREGA.md) · [Pendências vigentes](../../docs/projeto/PENDENCIAS.md).

</details>


</details>
