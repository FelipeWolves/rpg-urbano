# Seletor de alvo no chão — correção de apresentação

O marcador anterior era projetado para coordenadas2D e desenhado pelo HUD: a profundidade da esfera não o ocultava. A correção usa segmentos no espaço3D com profundidade de mundo, centrados nos limites do modelo, junto da base (mínimoZ+1 unidade), com raio1,5× a maior extensão horizontal. Preserva círculo/losango (-TargetMarker=Diamond), cor e critérios de seleção; sem mudança em alvos, habilidades, dano ou confirmação.

A implementação usa DrawDebugLine, apropriado ao protótipo Development. Não é recurso final de distribuição Shipping: antes disso, substituir por malha/decal/material próprio com profundidade. Altura acompanha a base do modelo atual, não traça terreno; terrenos irregulares, personagens flutuantes e modelos definitivos exigirão âncora de pés/solo própria.

Build Editor: docs/relatorios/atuais/VIS-SELETOR-BUILD.json. Nenhum editor, jogo ou commandlet aberto nesta entrega, conforme solicitação do usuário. Compilação não comprova aparência. Sem mudança no núcleo, sem repetir suíte de combate.

## Conferência pelo usuário

1. Fechar a prévia antiga e abrir Abrir-Preview.cmd, que aponta para o novo build aprovado via PREVIEW-BUILD.json.
2. Entrar em Batalha, escolher habilidade e observar o alvo sugerido.
3. Verificar anel sob a esfera: parte traseira deve respeitar a ocultação pelo personagem, sem pintar sua superfície.
4. Trocar alvo e confirmar ação; o marcador deve acompanhar a seleção e desaparecer quando ela for limpa.
5. Se houver problema, informar captura e resolução em entregas/01-interface-e-testes/recebidos. Não é necessário repetir toda a batalha.

Aceite visual confirmado pelo usuário após a correção. Apresentação mobile e fluidez permanecem tarefas separadas; demais aceites VIS continuam parciais. Percentuais mantidos: o aceite pontual não fecha o conjunto VIS nem mobile.
