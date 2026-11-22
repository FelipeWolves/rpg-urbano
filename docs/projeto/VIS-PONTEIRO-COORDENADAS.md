# Ponteiro — conversão viewport / Canvas

22/11/2026. Usuário confirmou alinhamento correto após a compilação. Correção aceita no cenário relatado; não implica teste em todos os DPIs/dispositivos.

O mouse/toque chegava no espaço do viewport, enquanto o HUD era desenhado com transformação própria do Canvas. Agora a transformação de desenho, incluindo origem/recorte/margens e DPI do Canvas, é invertida antes dos testes de clique. Projeção das etiquetas usa o mesmo espaço; raycast3D converte de volta para viewport. Mudança da transformação durante um gesto cancela e exige nova pressão, sem compensação fixa em pixels e sem mudar o cursor do Windows.

Código local da Unreal consultado: GameViewportClient::Draw, UCanvas::ApplySafeZoneTransform e FCanvas::GetTransform/GetDPIScale. Isso fundamenta a conversão; o aceite do caso real veio do usuário. Nenhuma engine/configuração de sistema foi alterada.

Compilação8ações/saída0 em build/pointer-coordinate-build.log. Depois foram aprovados72grupos de apresentação, incluindo8novos de coordenadas (identidade, deslocamento vertical, escala, rotação/cisalhamento, pontos externos, transformações inválidas, cancelamento e clique por mouse/toque). Não somar como validação de Android. Relato visual registrado separadamente da execução dos testes nativos. Editor não foi aberto pelo assistente.
