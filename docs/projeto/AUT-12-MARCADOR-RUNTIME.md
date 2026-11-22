# AUT-12 — marcador de alvo de runtime

22/11/2026. Substituído DrawDebugLine por ator próprio com componentes UStaticMeshComponent, geometria em barras de cubo, sem colisão/sombra. Círculo (28 segmentos) e losango (4) usam a mesma função pura de geometria; nenhuma regra, alvo ou dano foi modificado.

Âncora: base do bounds do personagem +1 unidade, raio 1,5 vezes maior extensão XY. O marcador é ocultado a cada desenho e exibido só para alvo vivo selecionado (ou hover válido clássico), removido ao reconstruir formações e no EndPlay. Geometria é reaproveitada enquanto estilo/raio não mudarem. Cubo é referência do objeto padrão do HUD para rastreamento de recurso no cook. Não adiciona plugin.

Cinco grupos nativos aprovados: anel/losango fechados, comprimento de aresta, limites de raio e segmentos. Compilação da integração registrada em AUT-12-BUILD.json. Marcador usa material básico da engine; acabamento/cor final não são equivalentes ao ciano do debug. Aceite visual anterior não prova aparência desta substituição.

Pendências: visual no chão/atrás do modelo, colisão/limpeza em execução, contraste e cor configurável final, solo inclinado/irregular, cook/Shipping. Nenhuma execução gráfica, Android ou internet. O plano não afirma que Shipping foi validado.

Primeiro clique da abertura: usuário informou que aparentemente está correto; **REVER / aguardando segunda conferência**, sem fechar aceite integral.

Próximo: AUT-13, adaptador de eventos visuais sem autoridade de combate.
