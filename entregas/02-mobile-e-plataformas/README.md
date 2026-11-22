# Mobile e plataformas — guia vigente

Atualizado em **22/11/2026**. Prioridade: **P2 — etapa dedicada, atualmente adiada**. Este guia substitui as perguntas antigas nos temas já respondidos; o histórico abaixo fica apenas para referência.

## Já recebido ou definido — não reenviar

AVD com perfil intermediário de 8 GB e Galaxy S23 Ultra posterior já são direção recebida. SDK/JDK/boot e diagnóstico da Unreal possuem registros; não reenviar toda a configuração.

## O que você pode adiantar agora

- [ ] Quando houver reteste autorizado, entregar apenas aparelho/SO efetivamente usados, versão do build e resultado observado. Não precisa redefinir o aparelho-alvo.
- [ ] Consulta futura: documentação oficial da versão da Unreal/plataforma sobre backend de vídeo habilitado no APK/IPA, codecs/perfis, áudio/mute, legendas, rotação/restauração, foco e memória. Guardar fonte/data/versão.

## Trabalho do desenvolvimento — não precisa virar pesquisa sua

Retomar diagnóstico/toolchain e pacote somente na etapa autorizada. Preview mobile no Windows não comprova Android/iOS.

## Testes e aceites ainda pendentes

Backend WMF é somente Win64. APK, orientação física, desempenho e backends Android/iOS continuam pendentes; nenhum Android deve ser iniciado agora.

## Onde entregar

Use **recebidos/** deste grupo; para revisão, crie versão nova e cite o arquivo substituído sem apagar o original. Informe ID/versão, finalidade, o que mudou, fonte/autoria e resultado esperado quando cabível. Entrega parcial é válida e não autoriza implementação, instalação ou execução automática.

[Resumo de necessidades](../NECESSIDADES-ATUAIS.md) · [Ordem de execução](../../docs/projeto/ORDEM-EXECUCAO-POS-AUT.md) · [Mapa SIS-02-A](../../docs/projeto/SIS-02-A-MAPA-NUMERICO.md) · [Modelo](../MODELO-ENTREGA.md)

<details>
<summary>Guia anterior — histórico; não preencher novamente o que já foi respondido</summary>

> **Revisão 21/11/2026:** materiais recebidos desta área reconciliados na [consolidação vigente](../../docs/projeto/CONSOLIDACAO-2026-11-21.md). Não reenviar respostas ENT já fechadas. Novos arquivos finais podem continuar em recebidos; implementação e testes permanecem separados. Prioridade atual: abertura padrão Unreal; depois fila AUT.

# Mobile, dispositivos e desempenho

**Atualização20/11:** material desta área estudado; [matriz e pendências específicas](../../docs/projeto/REVISAO-ENTREGAS-2026-11-20.md). Guia abaixo preservado como roteiro; não precisa responder novamente o que já entregou. Revisão integrada ao planejamento, não ao código.

Prioridade: **P1 — reunir cedo**. Guia de preparação, não autorização de implementação automática.

## Já existente — não reenviar decisões fechadas

Só há validações Windows delimitadas. Janela vertical não muda plataforma; apresentação mobile ausente tem causa aberta.

## O que você pode reunir ou responder

- [ ] Informe modelo do celular, Android/iOS e versão, memória se souber, orientação preferida e aparelho principal de teste.
- [ ] Descreva o que chamou de modo mobile: onde ativou, imagem esperada e imagem que apareceu; uma captura com dados pessoais ocultos ajuda.
- [ ] Defina aparelhos mínimos desejados e prioridade Android/iOS/Windows; pode responder não definido.
- [ ] Informe espaço de instalação aceitável, preferência de qualidade/FPS e necessidade de jogar sem internet.
- [ ] Diga se haverá disponibilidade para instalar uma versão de teste quando o build existir; não envie credenciais, certificados ou chaves.

## Trabalho de desenvolvimento restante

Diagnóstico de toolchain quando necessário, primeiro build em aparelho, toque/áreas seguras, pausa/retomada, orientação, memória/GPU/bateria e desempenho medidos.

## Dependências e entrega

Pesquisa disponível agora; instalação/configuração só com autorização própria. Não comprar hardware por antecipação.

Coloque documentos/imagens/vídeos desta área em **recebidos/**, nesta pasta. Pode entregar parcialmente; use NÃO SEI / PROPOSTA / REFERÊNCIA / DECISÃO DESEJADA. Copie o modelo geral se ajudar. Não sobrescreva versões anteriores. Desde21/11,Markdown recebido é versionado por autorização do usuário; imagens/vídeos ficam locais, fora doGit.

[Instruções gerais](../README.md) · [Modelo](../MODELO-ENTREGA.md) · [Pendências vigentes](../../docs/projeto/PENDENCIAS.md).

[Revisão das sugestões e limites atuais](../../docs/projeto/MOBILE-PLANO-REVISAO.md): não precisa instalar BlueStacks; ferramentas/perfis devem ser verificados antes do uso. Canvas atual não tem Designer UMG; preview Windows não comprova Android/iOS.

</details>
