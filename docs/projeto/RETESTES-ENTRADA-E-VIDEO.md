> **Novo aceite LAB-PVE-04:** [pontuação/sobrevivência](LAB-PVE-04-SCORE-SOBREVIVENCIA.md), página2doTerminal, resultado/recibo sem repetição e meta de turnos do chefe. Não substitui os aceites pendentes02/03 nem reabre controles já aceitos.

> **Novo aceite LAB-PVE-03:** [roteiro de ondas e fases](LAB-PVE-03-ONDAS-FASES.md). Conferir contadores, reconstrução dos inimigos, transições sem/com alteração de kit, detalhes, resultado e abandono. Não reabre os aceites de cliques/vídeo/placeholder já confirmados.

> **Novo aceite pendente — LAB-PVE-02:** Terminal → Ritual local — Felipe e ecos. Conferir objetivo ECO-1, auxiliar independente, resultado/nova tentativa e abandono/retorno. Não reabre os aceites anteriores dos outros cenários. [Roteiro](LAB-PVE-02-RITUAL-TERMINAL.md).

> **Diretriz vigente — 22/11:** [base comum dos laboratórios](BASE-COMUM-LABORATORIOS.md). Terminal e Felipe compartilham regras Team e limite de 200 ações/empate; personagens mantêm kits/atributos próprios. Modo clássico fora desta frente. Ajustes visuais de fe6060b aceitos pelo usuário; não reabrir por rotina.

> **22/11 — retorno mais recente:** vídeo/skip e conclusão do piloto sem crash aceitos no teste do usuário. [Placeholders, seletor e escala do Terminal](PLACEHOLDERS-TERMINAL.md): mobilidade corrigida, marcador mais espesso/emissivo e PV do Terminal ampliados. Aceite visual desses ajustes ainda pendente; demo ~25% mantida.

> **Correção vigente — 22/11:** [crash do marcador, placeholders, regras gerais do Terminal e vídeo por compilação](TERMINAL-MARCADOR-MIDIA-CORRECOES.md). Funil compartilhado com Felipe, geometria visual protegida e marca de vídeo vinculada ao binário. Reteste visual pendente; aceite dos cliques únicos preservado.

# Retestes do usuário — entrada e vídeo

Atualizado em 22/11/2026, após o teste de 2db703f no RpgEngineProbe.

| ID | Estado | Evidência e limite |
|---|---|---|
| RET-INPUT-01 | ACEITO no cenário testado | Usuário confirmou todos os cliques únicos, inclusive menus e batalha; inimigos selecionáveis com habilidades 1 e 2. Não representa todos os dispositivos/cenários |
| RET-MEDIA-01 | ACEITO: abertura com 1 clique | Confirmado pelo usuário; backend mobile continua não validado |
| RET-MEDIA-02 | ACEITO pelo usuário: vídeo e skip corretos; descrição anterior histórica | Hover confirmado. Skip imediato e ausência de barra observados; condizem com vídeo já marcado como assistido. Barra exige primeiro vídeo não assistido e segurar 800 ms. Essa condição ainda não foi confirmada visualmente |
| RET-HUB-FELIPE | PARCIAL: batalha até o fim sem crash e recargas aceitas; esferas/seletor pendentes | Arena PvP → Testar Felipe (local); conferir A1 ativa, A2 passiva, A3 em área, volta ao hub e Terminal separado |

## Esclarecimento do escudo no Terminal

O Terminal usa team_reference_setup: A3 Proteção tem TargetRule::Self. Só o próprio personagem é válido. A1/A2 ativas ofensivas usam inimigos; certos personagens têm A2 passiva. A3 do personagem A3 também exige PV <=50%.

Escudo padrão = 1% PV máximo do aplicador, mínimo 1. O elenco de referência tem 80/100/140 PV, portanto esse escudo gera 1 ponto. A faixa é muito pequena; ausência visual aparente não comprova ausência do efeito. Teste nativo confirmou A1 usando slot 3 em si: 1 ponto, recusa atômica de inimigo. Conferir no painel de efeitos durante a batalha.

Felipe é outro cenário: A1 Garra do Asfalto; A2 Postura de Vigília passiva, que gera escudo após contra-ataque válido; A3 Eclipse é ofensiva em área, não uma aplicação direta de escudo.

## Próximos retestes

Preservar os aceites acima; reabrir somente se houver regressão concreta. Testar acesso novo e transições. Marca de assistido agora vinculada ao binário: primeira exibição após nova compilação deve mostrar a barra ao segurar. Ver TERMINAL-MARCADOR-MIDIA-CORRECOES.md. Nenhuma flag/save foi apagada nesta entrega. Android e controle de tela continuam fora do escopo.