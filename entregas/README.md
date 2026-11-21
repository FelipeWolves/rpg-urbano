> **Entrega concluída:** [projeto Unreal estável e guia de abertura](../docs/projeto/UNREAL-PROJETO-ESTAVEL.md). Controle de tela encerrado por solicitação; nova interação com o editor exige nova liberação. Atualização documental/Git apenas após essa solicitação.

> **Estado vigente — 21/11/2026:** [consolidação das 15 entregas e decisões posteriores](../docs/projeto/CONSOLIDACAO-2026-11-21.md). ENG-ESTAVEL concluído no Windows local: projeto fixo, mapa próprio, abertura/reabertura e Play observados. Próximo: AUT-01–04. ESC comum e seletor já aceitos não são reabertos. Android SDK/NDK/JDK localizados e boot do AVD confirmado pelo usuário; caminhos já persistem, mas ValidatePlatforms retorna Android INVALID r27c; diagnóstico e APK pendentes. Percentuais recalculados no painel de progresso. As notas datadas abaixo preservam histórico e não reabrem decisões substituídas.

# Central de entregas do usuário

**Política vigente em21/11/2026:** os15Markdown recebidos passam a ser versionados por autorização do usuário; imagens/vídeos permanecem locais. [Novidades conferidas](../docs/projeto/RECEBIMENTO-NOVIDADES-2026-11-21.md). Menções anteriores de recebidos inteiramente fora doGit são históricas. Nenhum recurso foi aplicado ao jogo nesta recepção.

**Estado em20/11/2026:**12 arquivos estudados; [revisão por grupo, conflitos e retomada](../docs/projeto/REVISAO-ENTREGAS-2026-11-20.md). As notas anteriores de “não analisado” são históricas. Agora os materiais foram incorporados ao planejamento, sem implementação. Não preencher novamente perguntas já respondidas; faltam apenas os pontosENT ou dados técnicos específicos.

Comece aqui. Cada grupo tem um README com perguntas específicas, o que já existe e o trabalho de desenvolvimento restante. Coloque seus arquivos na subpasta **recebidos** correspondente. Se não souber onde, use **00-entrada-geral/recebidos**.

## Como trabalhar sem depender do assistente

1. Escolha uma área; não é necessário responder tudo para ajudar.
2. Leia o README dela e consulte a pendência citada se precisar de detalhes.
3. Crie um documento com suas respostas, referências ou proposta. Pode usar MODELO-ENTREGA.md ou texto simples.
4. Salve em recebidos com nome como AAAA-MM-DD_assunto_v01.md; imagens/vídeos/fontes editáveis podem ficar ao lado. Para revisão, crie v02 e diga o que mudou.
5. Quando voltar, informe apenas quais áreas receberam material. Os arquivos já estarão aqui; não precisa colar tudo no chat.

Uma captura acompanhada de duas frases pode ajudar mais que um relatório amplo sem conclusão. Pode usar Markdown, TXT, PDF, imagens, planilha ou vídeo; não precisa converter agora, nem instalar ferramentas. Prefira texto pesquisável para fórmulas/regras, mesmo quando houver imagem.

## Grupos

| Grupo | Onde colocar | Foco |
|---|---|---|
|00|[Entrada geral](00-entrada-geral/README.md)|Dúvidas ou material sem área clara|
|01|[Interface e testes](01-interface-e-testes/README.md)|Seletor, fluidez, detalhes, resoluções e acessibilidade|
|02|[Mobile e plataformas](02-mobile-e-plataformas/README.md)|Aparelho-alvo e apresentação mobile ausente|
|03|[Combate e habilidades](03-combate-habilidades/README.md)|Efeitos, fórmulas, supressão e fichas novas|
|04|[Modos PvE e clássico](04-modos-pve-e-classico/README.md)|Chefes, score, fases e contratos clássicos|
|05|[Hub e arte2D](05-hub-identidade-e-arte2d/README.md)|Imagem final, recortes, menus, ícones/retratos|
|06|[Vídeo e cinematics](06-video-e-cinematics/README.md)|Abertura, interrupção, legendas e player|
|07|[Cenários/personagens/animação](07-cenarios3d-personagens-animacoes/README.md)|Primeiro cenário e personagem, rig e ações|
|08|[Áudio e DJ](08-audio-musica-e-dj/README.md)|Trilhas, eventos sonoros e contrato rítmico|
|09|[Progressão e save](09-progressao-inventario-save/README.md)|Economia, itens, equipamento e persistência|
|10|[Universo e textos](10-universo-conteudo-textos/README.md)|História, tutorial, elenco autoral e localização|
|11|[Distribuição e direitos](11-distribuicao-servicos-e-direitos/README.md)|Objetivos de lojas/serviços e origem dos recursos|

## Prioridade sugerida para você

Primeiro: aparelho-alvo e esclarecer o que foi usado como modo mobile; exemplos de fluidez; fichas/matriz de efeitos que ainda faltam. Depois: conteúdo da primeira experiência, hub, roteiro de vídeo e áudio. Arte3D pode ser preparada em amostra pequena; produção extensa espera orçamento medido no celular. Lojas/servidores ficam depois.

## O que acontece ao retomar

O assistente confere nomes/versões novos, lê somente os grupos relevantes, registra o inventário e compara com contratos fechados. Separa fato confirmado, referência, hipótese e decisão sua; aponta conflitos e propõe uma entrega pequena. Só então copia recursos aprovados para assets/sources ou assets/runtime e dados revisados para data/docs. Documento recebido não é automaticamente regra aprovada, código executável ou autorização para instalar/rodar programas.

Estados: **recebido → revisado → esclarecimento necessário ou pronto para implementar → integrado → validado**. CONTROLE.csv começa como índice dos grupos, não inventário dos arquivos recebidos; só será atualizado para esses estados após conferência. Não marque revisão/integração por conta do simples envio.

## Preservação e Git

READMEs, modelo, índice e documentos **Markdown** de recebidos entram no Git. Imagens, vídeos e demais formatos ainda não autorizados ficam locais, ignorados. Portanto **push não faz backup das mídias recebidas**. Mantenha uma cópia sua desses recursos. Nenhum anexo é executado automaticamente por estar versionado.

Os recursos já enviados continuam em assets; não precisam ser movidos ou duplicados. Sem sobrescrever originais ou apagar histórico. Não entregar senhas, tokens, certificados privados, dados de conta nem arquivos protegidos do jogo de referência. Para referência pública, basta fonte/link/data/patch e observação; não precisa baixar clientes, extrair recursos ou pesquisar todos os sistemas antes de ajudar.

## O projeto não precisa esperar todas as entregas

Enquanto você reúne material, há uma [fila independente de engenharia](../docs/projeto/FILA-DESENVOLVIMENTO-INDEPENDENTE.md). Pesquisas opcionais não bloqueiam esses itens prontos. A fila não representa trabalho executado nem substitui decisões de produto ainda abertas.

Conferência em20/11/2026:7 documentos nos grupos00–06 constam em RECEBIMENTO.csv. O registro confirma presença/tamanho/hash, não aprovação ou aplicação das propostas. CONTROLE.csv atualizado; originais continuam locais e ignorados no Git.

Nova conferência em20/11/2026:11 arquivos nos grupos00–10;4 novos (07–10),4 alterados (00,01,04,05) e3 sem alteração (02,03,06). Grupo11 sem arquivo recebido. Presença/tamanho/SHA-256 conferidos; conteúdo não analisado nem aplicado. Inventário atual em entregas/RECEBIMENTO.csv; hashes anteriores preservados em entregas/RECEBIMENTO-HISTORICO.csv. Os originais continuam locais, fora do Git; o histórico de hashes não é backup dos conteúdos.

Recebimento final em20/11/2026: documento de Distribuição, Serviços e Direitos (grupo11) presente e registrado. Agora todos os12 grupos00–11 possuem1 documento recebido. Esta conclusão é somente de recebimento; conteúdo ainda não analisado/aprovado/integrado. Originais permanecem locais, fora do Git.

Adendo20/11: respostasENT-01–15 recebidas e estudadas no grupo00;13 originais locais ao todo. [Decisões consolidadas](../docs/projeto/RESPOSTAS-ENT-2026-11-20.md). Não editar documentos antigos para apagar alternativas:adendo prevalece nos pontos respondidos;implementação ainda pendente.

Novo adendo técnico20/11 estudado no grupo00;14originais locais registrados. [Consolidação](../docs/projeto/ADENDO-TECNICO-CONTRATOS-2026-11-20.md). Sem edição dos originais nem implementação;originais permanecem fora do Git.
