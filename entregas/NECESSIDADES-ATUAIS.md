# Entregas, execução e aceites

Estado de 23/11/2026. Versão oficial **v0.28-pre-alpha**. A estimativa da demo é **28,43%**. Esta revisão atualiza planejamento e dependências.

## O que posso fazer com os recursos atuais

1. Acabamento da apresentação de combate sobre AUT-13. Melhorar a leitura de quem age, alvo, habilidade e impacto. Usar som e animação provisórios sem recalcular efeitos.
2. Preparar validação em janela e roteiros de QA para HUD, tutoriais, laboratórios e persistência isolada.
3. Refinar materiais provisórios dos sete destinos do Hub 3D. Depois, trabalhar movimento ambiental, câmera e presença provisória com Movimento Reduzido e alternativa leve. O mapa editável vem após o aceite dos materiais e a definição do formato.

## O que mais ajuda você a entregar

1. **Agora, se disponível.** Imagem final do Terminal. Pode anexar junto um gabarito dos pontos clicáveis e a autoria. Esta é a dependência direta para integrar a tela final.
2. **Em paralelo.** Uma amostra de personagem/rig e alguns clips, ou um loop e efeitos por evento. Os protótipos de apresentação avançam com os recursos atuais.
3. **Para uma etapa futura.** Um capítulo piloto de História ou uma ficha de encontro. O roteiro de História inclui 6 a 8 nós, falas, objetivo e desbloqueios. As diretrizes recebidas já servem de referência para o planejamento.

## Aceites com você

Após eu preparar cada roteiro, os aceites prioritários são leitura do combate e da HUD, tutoriais e modos locais, comportamento do Hub em janela e persistência. No Android físico, testar áudio, áreas seguras, toque, vídeo e desempenho. O Pixel 7 virtual já validou instalação, rotação, toque, vídeo e uma ação de combate.

## Quadro por grupo

| Pasta | Sua entrega útil | Desenvolvimento independente | Aceite ou dependência |
|---|---|---|---|
| 00-entrada-geral | Enviar apenas decisões novas, com versão e o trecho substituído. | Consolidar documentos, contratos e dependências. | Confirmar lacunas de design apenas quando afetarem um bloco autorizado. |
| 01-interface-e-testes | Depois de receber o roteiro, anotar somente falhas ou resultados do teste. | Preparar validação em janela, acessibilidade e QA de navegação, HUD, tutoriais e PvE. | Aceite visual em janela e validação de gravação, reabertura e recuperação. |
| 02-mobile-e-plataformas | Disponibilizar um Android físico ou enviar modelo, versão e resultados do APK. | Reaproveitar o APK e o AVD e preparar roteiro de retorno, áudio, áreas seguras, toque e desempenho. | O AVD já validou instalação, toque, rotação, vídeo e uma ação. Áudio, persistência e desempenho físico seguem abertos. |
| 03-combate-habilidades | Enviar novos kits, efeitos ou descrições apenas se houver mudança. | Consumir os eventos AUT-13 na apresentação, com sinais provisórios e regras atuais. | Aceitar leitura de ator, alvo, habilidade e impacto. Regras inéditas precisam de contrato. |
| 04-modos-pve-e-classico | Uma ficha final de encontro, quando quiser substituir as fixtures. Enviar objetivo, limite, resultado e composição. | Manter laboratórios e tutoriais existentes e preparar roteiro por modo. | Aceites específicos LAB-PVE-02 a 06 e dois tutoriais. Regras novas exigem definição. |
| 05-hub-identidade-e-arte2d | Prioridade de entrega. Enviar a imagem final do Terminal e, se possível, indicar os pontos clicáveis e a autoria. | Preparar integração, manter os acessos atuais e avançar com feedback visual provisório. | A integração da tela final depende dessa arte. Revisar leitura e toque depois. |
| 06-video-e-cinematics | Se houver fala, enviar texto e marcações de tempo para legendas. | Manter o player atual e investigar persistência da marca de vídeo assistido em ambiente de teste. | Validar áudio, legendas quando houver e reprodução em aparelho físico. |
| 07-cenarios3d-personagens-animacoes | Uma amostra de personagem com rig, materiais e clips, quando disponível. | Refinar materiais dos sete destinos, movimento ambiental, câmera e presença provisória com fallback leve. | Aceitar materiais e formato antes do mapa editável. Personagem e animação finais dependem de assets. |
| 08-audio-musica-e-dj | Uma amostra de loop e efeitos de ação, impacto e menu com autoria. | Ligar som provisório e animação ao início, impacto e fim dos eventos existentes. | Aceitar o resultado audiovisual. Mix e conteúdo finais dependem de recursos. Instrumentos e Games ficam para etapa futura. |
| 09-progressao-inventario-save | Enviar exemplos de itens e valores somente quando a economia entrar na fila. | Preparar testes com saves isolados e diagnosticar persistência sem alterar o contrato v1. | Validar reabertura e recuperação. Praça, câmbio, loja, equipamentos e presets aguardam contratos próprios. |
| 10-universo-conteudo-textos | Quando preparar História, entregar um capítulo piloto com 6 a 8 nós, falas, linha narrativa e desbloqueios. | Consolidar a proposta dos 67 capítulos e preparar critérios para trilhas e Atlas quando a frente for autorizada. | Campanha, recompensas, obtenção dos quatro primeiros personagens e fases canônicas exigem contrato e conteúdo. |
| 11-distribuicao-servicos-e-direitos | Confirmar autoria, licença e restrições dos recursos que entrarem na distribuição. | Manter documentação da pré-alfa interna e dos pacotes Git LFS. | Publicação e serviços têm decisão própria. Patreon e BETA_SUPPORTER entram na Beta. PvP online é futuro. |

[Ordem técnica](../docs/projeto/FILA-DESENVOLVIMENTO-INDEPENDENTE.md) · [Progresso](../docs/projeto/PROGRESSO-GERAL.md) · [Visão v04](../docs/projeto/2026-11-23_diretrizes-telas-modos-e-evolucao_v04.md) · [Política de versões](../docs/projeto/POLITICA-VERSIONAMENTO.md).
