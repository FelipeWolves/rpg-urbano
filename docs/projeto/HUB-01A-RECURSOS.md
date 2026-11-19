# HUB-01A — recursos recebidos

19/11/2026. Usuário declarou que todas as imagens e vídeos foram criados/gerados por ele. Arquivos originais preservados sem renomear, converter, editar ou reproduzir vídeos. Origem registrada como declaração do titular, sem auditoria dos serviços de geração.

## Conferência

| Papel | Caminho real | Dimensões | Tamanho | Duração / taxa declaradas pelo Windows |
|---|---|---|---|---|
| Panorama | assets/runtime/hub/panorama.jpg |2752×1536|2,84MiB|Imagem JPEG |
| Vídeo desktop | assets/runtime/video/intro-desk.mp4 |3840×2160|176,23MiB|30,906s /60fps |
| Vídeo mobile | assets/runtime/video/intro-mobile.mp4 |2160×3840|32,34MiB|9,405s /30fps |
| Arte lateral | assets/runtime/video/intro-side-art.png |1672×941|2,47MiB|Imagem PNG |

Catálogo atualizado para os nomes reais panorama.jpg e intro-desk.mp4, sem renomear arquivos. Dimensões efetivas substituem os exemplos anteriores. Quatro entradas Ready significam presentes e cadastradas, não importadas no runtime nem aprovadas em aparelho.

Metadados de imagem obtidos por System.Drawing; vídeos por propriedades Windows Shell, sem player ou frames decodificados pelo nosso script. Ambos informam identificador de compressão {34363248-0000-0010-8000-00AA00389B71}, dois canais de áudio e44100Hz. Codec de áudio não foi identificado; compatibilidade de reprodução na Unreal/mobile não foi testada. Rotação embutida/SAR e demais particularidades do contêiner não foram auditadas por esse método.

Tamanhos/resoluções justificam avaliar posteriormente versões de distribuição e custo de decodificação. Não reduzir qualidade nem converter material automaticamente; fontes4K preservadas. Nenhum vídeo foi executado.

## Observação visual

Panorama: cidade contínua ao pôr do sol, terraço/parapeito e quatro personagens em primeiro plano. Não mostra as três áreas diferenciadas da proposta (salão musical, cidade e setor competitivo). É uma base visual utilizável, mas ainda precisamos confirmar composição/recortes e destinos antes de considerar o hub final aceito. Não inventar botões ou hotspots pelo nome do arquivo.

Arte lateral: composição celeste com aves luminosas; recurso próprio distinto da cena urbana, cadastrado para o vídeo vertical no desktop. A escolha estética e o enquadramento lateral serão avaliados quando houver player/renderer.

Três recortes iguais do panorama permanecem como valores provisórios. A largura de cada terço é aproximadamente917,33px, não1080px; o código usa coordenadas normalizadas e preserva proporção. Não redimensionar a imagem para caber na dimensão de exemplo3240×1920.

## Fontes e Git

Também recebida assets/sources com arquivos de mesmos nomes e tamanhos dos dois vídeos e da arte lateral. Não comparados byte a byte; não excluídos nem movidos. A pasta foi mantida como enviada, embora o roteiro anterior mencionasse assets/source.

assets/runtime, assets/source e assets/sources ficam ignorados no Git enquanto não houver estratégia de grandes recursos. Catálogo, autoria declarada e relatórios pequenos continuam versionáveis. Uma cópia nova do repositório precisará receber as mídias separadamente; não alegar pacote autossuficiente.

## Evidências e próximo passo

- [Inventário técnico](../relatorios/atuais/HUB-01A-RECURSOS.json).
- [Validação do catálogo](../relatorios/atuais/HUB-01A-CATALOGO.json).
- Repetir inspeção: scripts/inspecionar-recursos-apresentacao.ps1; valida metadados atuais contra o catálogo.
- Repetir validação: scripts/validar-recursos-apresentacao.ps1; confere presença e estrutura, sem decodificar vídeo.

Recebimento técnico concluído. Próximo passo é ligar catálogo/renderização/player, confirmar papel/enquadramento da imagem e, com autorização específica, realizar o aceite gráfico. Nenhuma mudança de core/combate, instalação, conversão, execução de vídeo ou engine nesta conferência.

Atualização posterior: a ligação por código do catálogo/panorama/acessos foi escrita e compilada em [HUB-01B-IMPLEMENTACAO.md](HUB-01B-IMPLEMENTACAO.md). Não significa vídeo executado ou fluxo gráfico aceito; próximo passo é conferência autorizada.
