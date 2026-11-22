# Vídeo e cinematics — guia vigente

Atualizado em **22/11/2026**. Prioridade: **P1 — reteste; P2 — integração restante**. Este guia substitui as perguntas antigas nos temas já respondidos; o histórico abaixo fica apenas para referência.

## Já recebido ou definido — não reenviar

Vídeos desktop/mobile e arte lateral já recebidos. Player Windows WMF preparado; usuário relatou reprodução. Pulo 800 ms na primeira exibição, revisão imediata e retomada pausada são decisões já recebidas.

## O que você pode adiantar agora

- [ ] Segunda olhada no primeiro clique: registrar um clique logo após Play e outro ensaio posterior, sem redefinir a regra do botão.
- [ ] Conferir som, fim natural, skip, perder/recuperar foco e comportamento ao reabrir. Não precisa alterar arquivos de vídeo para simular falhas.
- [ ] Se houver falas/legendas, enviar SRT/VTT ou tabela com início/fim/texto, idioma e ID/versão do vídeo; se não houver fala, informar apenas isso.
- [ ] Nova renderização só quando desejar revisar conteúdo: nome/versão, duração, resolução, trilha e o que substitui. Originais atuais não precisam ser reenviados.

## Trabalho do desenvolvimento — não precisa virar pesquisa sua

Legenda, volumes mestre/Mídia, arte lateral e flag/ciclo; autoabertura somente após aceite pertinente. Arquivo presente não comprova decoder por plataforma.

## Testes e aceites ainda pendentes

Primeiro clique provisório / REVER. Áudio/flag/lifecycle completo, mixer/legendas/arte lateral e abertura automática seguem pendentes.

## Onde entregar

Use **recebidos/** deste grupo; para revisão, crie versão nova e cite o arquivo substituído sem apagar o original. Informe ID/versão, finalidade, o que mudou, fonte/autoria e resultado esperado quando cabível. Entrega parcial é válida e não autoriza implementação, instalação ou execução automática.

[Resumo de necessidades](../NECESSIDADES-ATUAIS.md) · [Ordem de execução](../../docs/projeto/ORDEM-EXECUCAO-POS-AUT.md) · [Mapa SIS-02-A](../../docs/projeto/SIS-02-A-MAPA-NUMERICO.md) · [Modelo](../MODELO-ENTREGA.md)

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
