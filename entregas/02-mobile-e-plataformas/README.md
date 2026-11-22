# Mobile e plataformas — guia vigente

Atualizado em22/11/2026. **P2 — etapa adiada.** Fonte vigente: [mapa das famílias](../../docs/projeto/MAPA-FAMILIAS-E-PRIORIDADES.md). Conteúdo deste guia distingue preparação independente de implementação/aceite. Atualizações de arquivos recebidos ficam preservadas.

## Já recebido — não reenviar

- SDK/JDK/AVD e aparelho-alvo têm registros; não reenviar configuração inteira.
- WMF do player atual é Win64; preview retrato no PC não prova APK ou backend Android/iOS.

## Você pode preparar em paralelo

- Guardar modelo/SO reais e versão do build quando houver teste; registrar toque, orientação, safe area e interrupções.
- Pesquisa opcional futura deve ter fonte oficial/data/versão sobre mídias/plataformas; não instalar ou trocar SDK por iniciativa desta lista.

## Trabalho do desenvolvimento — não precisa virar pesquisa sua

- Manter interfaces independentes de plataforma e roteiros de teste reproduzíveis.
- Diagnóstico Android, empacotamento e aparelho apenas na etapa especificamente retomada.

## Dependências e aceites

- Nenhum Android será iniciado nesta rodada.
- iOS, assinatura, lojas, desempenho e determinismo no aparelho são validações próprias.

## Como entregar

Use recebidos/ deste grupo. Nome sugerido: AAAA-MM-DD_assunto_v02.md; identifique ID/versão, o que substitui, decisão/proposta/referência e origem. Entrega parcial é válida. Não apagar versões anteriores; Markdown pode ser versionado, imagens/vídeos e fontes grandes ficam locais fora do Git. Não reenviar material já recebido apenas para preencher uma lista.

[Central de necessidades](../NECESSIDADES-ATUAIS.md) · [Retestes](../../docs/projeto/RETESTES-ENTRADA-E-VIDEO.md) · [Escopo técnico entregue](../../docs/projeto/ESC-SIS-PVE-CONSOLIDACAO.md) · [Modelo](../MODELO-ENTREGA.md)

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
