# Retestes do usuário — entrada e vídeo

Atualizado em 22/11/2026, após o teste de 2db703f no RpgEngineProbe.

| ID | Estado | Evidência e limite |
|---|---|---|
| RET-INPUT-01 | ACEITO no cenário testado | Usuário confirmou todos os cliques únicos, inclusive menus e batalha; inimigos selecionáveis com habilidades 1 e 2. Não representa todos os dispositivos/cenários |
| RET-MEDIA-01 | ACEITO: abertura com 1 clique | Confirmado pelo usuário; backend mobile continua não validado |
| RET-MEDIA-02 | PARCIAL | Hover confirmado. Skip imediato e ausência de barra observados; condizem com vídeo já marcado como assistido. Barra exige primeiro vídeo não assistido e segurar 800 ms. Essa condição ainda não foi confirmada visualmente |
| RET-HUB-FELIPE | PENDENTE | Arena PvP → Testar Felipe (local); conferir A1 ativa, A2 passiva, A3 em área, volta ao hub e Terminal separado |

## Esclarecimento do escudo no Terminal

O Terminal usa team_reference_setup: A3 Proteção tem TargetRule::Self. Só o próprio personagem é válido. A1/A2 ativas ofensivas usam inimigos; certos personagens têm A2 passiva. A3 do personagem A3 também exige PV <=50%.

Escudo padrão = 1% PV máximo do aplicador, mínimo 1. O elenco de referência tem 80/100/140 PV, portanto esse escudo gera 1 ponto. A faixa é muito pequena; ausência visual aparente não comprova ausência do efeito. Teste nativo confirmou A1 usando slot 3 em si: 1 ponto, recusa atômica de inimigo. Conferir no painel de efeitos durante a batalha.

Felipe é outro cenário: A1 Garra do Asfalto; A2 Postura de Vigília passiva, que gera escudo após contra-ataque válido; A3 Eclipse é ofensiva em área, não uma aplicação direta de escudo.

## Próximos retestes

Preservar os aceites acima; reabrir somente se houver regressão concreta. Testar acesso novo e transições. Se necessário observar barra de skip em primeira exibição, preparar futuramente um modo de QA explícito para ignorar a marca de assistido, sem apagar o estado persistente do usuário. Nenhuma flag/save foi apagada nesta entrega. Android e controle de tela continuam fora do escopo.