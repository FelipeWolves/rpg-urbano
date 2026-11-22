> **22/11 — LAB-PVE-05:** página2doTerminal com duas variantes de3equipes sequenciais. Roteiro em docs/projeto/LAB-PVE-05-EQUIPES-SEQUENCIAIS.md: equipe ativa, HP contínuo, fase fixa/variável, janela e resultado. Aceites02–05 pendentes; próximo06independe de arte final.

> **22/11 — LAB-PVE-04:** Terminal → Mais modos > → Pontuação/Sobrevivência. Roteiro em docs/projeto/LAB-PVE-04-SCORE-SOBREVIVENCIA.md; testar paginação, contadores e consulta de recibo, sem concessão de créditos. Fixtures não esperam conteúdo final; próximo05equipes sequenciais. Aceites02–04 ainda pendentes.

> **22/11 — LAB-PVE-03:** novos testes de Ondas consecutivas e Ritual com fases no Terminal, aguardando seu aceite conforme docs/projeto/LAB-PVE-03-ONDAS-FASES.md. Recursos finais não bloqueiam o próximo LAB-PVE-04. Ritual simples também segue com aceite pendente.

> **22/11 — LAB-PVE-02:** Ritual disponível pelo Terminal na compilação atual; roteiro em docs/projeto/LAB-PVE-02-RITUAL-TERMINAL.md. Sua próxima avaliação é da nova rota/HUD, resultado, abandono e volta aos outros laboratórios. Conteúdo final pode continuar em paralelo. Próximo desenvolvimento: ondas/fases (LAB-PVE-03).

# Sua preparação e o desenvolvimento — quadro vigente

22/11/2026. Demo ~28%. Não precisa concluir todas as entregas antes de eu desenvolver. Pode enviar amostras parciais na pasta recebidos/ de cada grupo; nenhum original recebido foi modificado nesta revisão.

## O que mais ajuda você a priorizar

1. **04 Modos:** uma ficha completa de encontro para substituir os valores provisórios após a integração.
2. **05 e 03 Interface/kit:** ícones, retratos e descrições por ID, indicando apenas novidades.
3. **07 e 08 Produção:** uma amostra de personagem/rig/arena e áudio por evento. Não produzir todo o elenco de uma vez.
4. **10 e 06 Conteúdo:** roteiro curto do piloto e indicação de fala/legendas.
5. **11 Direitos:** procedência por recurso; resolver/substituir logo externa antes de uso final.

LAB-PVE-01 (observação/HUD) já foi entregue em código. Nenhum item acima bloqueia meu próximo corte LAB-PVE-02: Ritual no Terminal com dados existentes. Ritual, ondas/fases, score e sequências podem avançar com placeholders. Save/reabertura, tutorial e telas novas precisam de seu aceite depois de preparados; não exigem que você crie arte.

## Quadro por grupo

| Pasta | Sua entrega útil | Desenvolvimento independente | Aceite/dependência final |
|---|---|---|---|
| 00-entrada-geral | Indicar ID, versão, o que mudou e se é decisão/proposta/referência. Não precisa reunir tudo de uma vez. | Consolidar deltas e manter índices/contratos. | Só confirmar decisões novas quando faltarem; nenhuma nova pesquisa geral é pré-requisito. |
| 01-interface-e-testes | Preparar testes específicos de save/reabertura, tutorial e telas amplas quando houver roteiro. Não repetir clique/vídeo/seletor aceitos. | Integrar HUD PvE e feedback com placeholders; montar roteiros de QA. | Você avalia cada tela nova depois; não depende de arte final. |
| 02-mobile-e-plataformas | Guardar modelo/Android/GPU/RAM do aparelho-alvo e resultados existentes. Não reinstalar SDK/AVD. | Manter contratos de layout e backend portáveis no código. | APK/backend de vídeo/toque/desempenho precisam etapa futura autorizada; Android adiado. |
| 03-combate-habilidades | Deltas de fichas, descrições/níveis/ícones por ID; efeitos inéditos com exemplos. Felipe e fórmulas fechadas não precisam reenvio. | Reutilizar base Team para Terminal/Felipe, catálogo e executores; manter kits individuais. | Só regras novas indefinidas dependem de resposta; catálogo inteiro não bloqueia a UI. |
| 04-modos-pve-e-classico | Prioridade: uma ficha de encontro representativo com objetivo, vitória/derrota/limite, ondas/fases/times, kit e valores finais ou faixas. | LAB-PVE-01–06: adaptadores, Ritual, ondas/fases, score/sobrevivência, sequenciais e frentes locais, com fixtures. | Novas regras só após definição; telas novas pedem aceite. Clássico preservado, fora desta frente. |
| 05-hub-identidade-e-arte2d | Ícones/retratos/estados de habilidades e efeitos, textos e arquivos por ID; origem/licença. Panorama só se houver mudança real. | Rotas locais, painéis e HUD provisórios com recursos atuais. | Arte final depende de recursos; implementação de navegação não precisa esperar. Logo externa continua provisória até regularização/substituição. |
| 06-video-e-cinematics | Informar se há fala e se precisa legendas; entregar texto/timecodes se houver. Não reexportar vídeo para corrigir clique, já aceito. | Manter player e marca por binário; preparar interfaces de legendas/áudio e fallback conforme contratos existentes. | Conteúdo de legenda final depende do texto; backend mobile e mixagem não estão aceitos. |
| 07-cenarios3d-personagens-animacoes | Uma amostra Felipe/Umbra + Terraço: modelo, unidade/escala/pivô, rig, materiais/texturas, lista de clips e origem. Não precisa cidade/elenco inteiro. | Contrato de importação, IDs e cues; placeholders para todos os modos. | Visual final precisa asset/rig/clips; fluidez real de animação pede aceite após integração. |
| 08-audio-musica-e-dj | Uma amostra de BGM em loop e efeitos por evento (ação, impacto, menu); formato, loop/pontos, volumes relativos e autoria. Voz/stems se disponíveis. | IDs/eventos e arquitetura de áudio provisória sem esperar todas as faixas. | Mix final/vozes exigem recursos e audição; DJ é expansão separada. |
| 09-progressao-inventario-save | Somente valores/faixas e regras novas de itens/economia ainda indefinidos; uma ficha exemplificada. Não reenviar decisões de save v1. | Roteiro de gravação/reabertura/backup e integração de recibos de score do laboratório. | Persistência real precisa teste seu; economia comercial/cross-save não estão implementados. |
| 10-universo-conteudo-textos | Roteiro curto final do piloto: ordem de telas/falas, IDs, objetivo e condição de concluir cada passo. Preferível uma sequência pronta a muitos capítulos. | Fluxos provisórios e validação de IDs com tutorial técnico existente. | Texto/voz finais dependem de entrega; tutorial completo ainda precisa aceite específico. |
| 11-distribuicao-servicos-e-direitos | Tabela de recurso → origem → autoria/licença → usos permitidos/restrições. Atenção à logo externa; não presumir licença comercial. | Separar dependências, documentar procedência e contratos de plataforma. | Publicar/instalar/plugin/rede/lojas exigem etapa própria; nenhum desses itens bloqueia os testes locais com assets autorizados. |

## Não precisa reenviar

Kit Felipe A1/A2/A3, Bleed/Poison, Standard/Silver, fórmulas/relógios fechados, panorama/variantes atuais, vídeos já funcionando ou configuração completa do SDK. Mudanças novas devem indicar o que substituem.

## Estado dos testes

Cliques únicos, vídeo/skip, placeholders, seletor e conclusão da batalha Felipe aceitos no cenário Windows testado. Ainda não comprovados: persistência real após reabrir/backup, tutorial específico, matriz ampla de telas/acessibilidade, APK/desempenho/backend mobile e Shipping. Android e MCP seguem adiados.

[Minha fila e ordem](../docs/projeto/FILA-DESENVOLVIMENTO-INDEPENDENTE.md) · [Progresso e pesos](../docs/projeto/PROGRESSO-GERAL.md) · [Famílias/siglas](../docs/projeto/MAPA-FAMILIAS-E-PRIORIDADES.md). Matrizes CSV/JSON em docs/relatorios/atuais/DEPENDENCIAS-ENTREGAS.