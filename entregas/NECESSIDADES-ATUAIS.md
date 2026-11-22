> **Atualização22/11:** código ESC/SIS/PvE implementado; veja docs/projeto/ESC-SIS-PVE-CONSOLIDACAO.md na raiz. Não reenviar Silver/ondas/score fechados. O que ajuda: retestes clique/alvo/vídeo, propostas de menus e dados finais dos modos/chefes com gatilho, alcance, kit, duração, limites e exemplos, sem exigir catálogo inteiro.

# Necessidades atuais por área

22/11/2026. Central prática para preparar entregas enquanto o desenvolvimento segue. **Não é necessário responder tudo, reenviar recursos já existentes ou pesquisar regras fechadas.** Os READMEs de cada grupo trazem os detalhes e mantêm o guia anterior recolhido como histórico.

## O que mais ajuda agora

1. **Teste curto:** segunda conferência do primeiro clique em Rever abertura; novo marcador de alvo, equipe/alvos e leitura dos painéis. Se estiver correto, basta dizer o que foi testado. Se houver problema, indicar sequência, resolução/escala e resultado esperado.
2. **Mídia/textos:** legendas ou indicação de ausência de fala nos vídeos; trechos finais do tutorial e textos do piloto. Não precisa reenviar os MP4 ou a narrativa inteira.
3. **Amostra artística:** um personagem/rig e uma arena pequena, quando disponíveis, com escala/pivô/clips/fonte editável. Não produzir cidade ou elenco inteiro antes da primeira integração.
4. **Pesquisa opcional:** somente fórmulas/exceções ainda não fechadas, com fonte/data/versão e exemplo. Separar hipótese sobre Dislyte de regra escolhida para nosso jogo.

**Ponto1 entregue:** SIS-02-B/C, REG, Bleed/Poison e Felipe implementados. O que ajuda agora é o aceite do piloto pelos atalhos novos. Não pesquisar novamente regras fechadas; ESC/SIS/PvE será planejado separadamente.

## Quadro de entregas

| Grupo / guia | Pode preparar | Já temos — não pedir novamente | O que permanece com desenvolvimento/testes |
|---|---|---|---|
| [00 — Geral](00-entrada-geral/README.md) | Mudança/dúvida nova com contexto e versão anterior | ENT e complementos técnicos recebidos | Revisão localizada e classificação; envio não aplica regras |
| [01 — Interface](01-interface-e-testes/README.md) | Reteste do clique/marcador/menus e exemplos de fluidez | Perfis, entrada e painéis implementados; aceites antigos preservados | Aceites dos controles novos, persistência e resoluções |
| [02 — Mobile](02-mobile-e-plataformas/README.md) | Consulta oficial futura de backend/codec; relato do aparelho quando testar | AVD intermediário 8 GB e S23 Ultra posterior; ambiente já documentado | Toolchain/APK, Android/iOS, orientação, áudio/foco e desempenho; adiado |
| [03 — Combate](03-combate-habilidades/README.md) | Fórmula DEF/cura ainda não fechada, ficha/efeito novo ou delta | StandardShield, R04, A2, Block, ACC/RESIST e ponto fixo decididos | SIS-02-B/C → REG → A2, sem nova pesquisa obrigatória para B |
| [04 — Modos](04-modos-pve-e-classico/README.md) | Variações além dos perfis já aceitos, cronologia de fases/score | SEQ/PAR separados; HP compartilhado e frentes independentes não são o mesmo modo | Ondas novas, score, fases autônomas, área e clássico expandido |
| [05 — Hub/2D](05-hub-identidade-e-arte2d/README.md) | Revisões versionadas, ícones/retrato/camadas e zonas seguras | Panorama/locais/variantes documentados, fontes/codinome escolhidos | Composição final e aceites; destinos futuros continuam bloqueados |
| [06 — Vídeo](06-video-e-cinematics/README.md) | Reteste, SRT/VTT/tabela de legenda ou ausência de fala, revisão de conteúdo se houver | Dois vídeos e arte lateral; regra de skip/retomada recebida; playback Windows relatado | Mixer, legendas, arte lateral, flag/ciclo, autoabertura após aceite |
| [07 — 3D/animação](07-cenarios3d-personagens-animacoes/README.md) | Uma amostra de personagem/arena/rig/clips | Arena provisória, marcador runtime e canal de cues | Importação, consumidores visuais, VFX, fluidez e orçamento por plataforma |
| [08 — Áudio/DJ](08-audio-musica-e-dj/README.md) | Arquivos próprios e tabela evento/loop/volume; exemplos adicionais de DJ | Canal de mídia é direção recebida; componente de áudio existe | Mixer/consumidores e teste de som; DJ é sistema posterior |
| [09 — Progressão](09-progressao-inventario-save/README.md) | Teste salvar/reabrir/carregar; detalhes novos de inventário/economia | CIC-01/AUT-09 e save v1 entre batalhas, sem HP no formato | Aceite de IO, compatibilidade e expansão; score não é credits |
| [10 — Textos](10-universo-conteudo-textos/README.md) | Texto final do tutorial/piloto e chaves de localização | Direção narrativa recebida; piloto Felipe/um encontro | Integração do tutorial e localização; sem exigir todos os capítulos |
| [11 — Distribuição](11-distribuicao-servicos-e-direitos/README.md) | Novidades de escopo e fontes/licenças dos recursos finais | Prioridade de plataformas já recebida; stage preparado | Cook/Shipping/Windows limpo, serviços/lojas e direitos finais |

## Como entregar sem duplicar trabalho

- Salvar em `entregas/NN-area/recebidos/AAAA-MM-DD_assunto_v02.md` (ou versão apropriada). Não apagar/substituir a versão anterior.
- Informar o arquivo/trecho anterior a que se refere, o que mudou e o bloco/pendência se souber. Se a informação já está em um documento, citar nome e seção basta.
- Usar [MODELO-ENTREGA.md](MODELO-ENTREGA.md) ou texto simples; resposta parcial é válida. Recurso pode vir com nomes reais dos arquivos, dimensões/escala/idioma e origem.
- Para teste, separar observado de esperado. O status do primeiro clique é **provisório / REVER** até a segunda conferência; não precisa produzir relatório extenso.
- Não enviar credenciais/certificados privados/dados de conta ou conteúdo extraído de terceiros. Markdown de recebidos pode ser versionado conforme autorização anterior; imagens/vídeos continuam locais. Push não é backup dessas mídias.

## O que esta atualização não fez

Não houve nova análise integral dos arquivos recebidos, recebimento/inventário de mídias, execução de código, teste, compilação ou alteração de recursos. CONTROLE.csv foi reconciliado com entregas/contratos já documentados; campos anteriores de revisão são preservados e não aprovam todas as propostas de um grupo. RECEBIMENTO.csv e seu histórico não foram reescritos.

Não houve internet, telas, Android ou instalação. Uma pesquisa externa que for necessária pode ser entregue pelo usuário com fonte oficial/versão/data e resultado esperado; não executaremos material recebido automaticamente.

[Ordem vigente](../docs/projeto/ORDEM-EXECUCAO-POS-AUT.md) · [Mapa SIS-02-A](../docs/projeto/SIS-02-A-MAPA-NUMERICO.md) · [Pendências técnicas](../docs/projeto/PENDENCIAS.md) · [Controle](CONTROLE.csv)
