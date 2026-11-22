# REG-03 — controles: base executável

22/11/2026. Implementação no modelo opt-in FunnelV1, sem mudar os baselines Legacy.

- Silence impede habilidades especiais; posição básica 1 do perfil Team continua disponível. Recusa não consome ação nem altera estado. Cards informam o motivo.
- Stun, Freeze e Petrify impedem agir, permitindo resolver a oportunidade com Skip. Não confundir com Block probabilístico do funil ou com FreezeCooldowns.
- Invincible e DebuffImmunity são efeitos distintos. Imunidade estrutural por tipo é explícita na Definition. Irresistível não ignora imunidade sem declaração própria.
- Debuffs de ataque consultam o resultado do hit; Miss/Block impedem aplicação. Habilidade sem dano usa seu próprio teste de chance/ACC/RESIST.
- Status pode declarar origem passiva nativa e borda. Suppression desabilita consequências nativas comuns/prateadas; douradas/estruturais ficam protegidas. Buff já convertido em status ativo não é desfeito por inferência.
- Reações são revalidadas antes da execução: morto ou impossibilitado não reage. Reação marcada nativa também respeita supressão.

Oito grupos focados aprovados, incluindo recusa atômica por silêncio, básica disponível, três controles, elegibilidade de passiva por borda, imunidades e Miss. Regressão geral aprovada; adaptador Unreal compilado em 18 ações, resultado Succeeded. Logs locais: build/reg03-regression.log e build/reg03-build.log. Nenhuma tela, engine em execução, Android ou internet utilizados.

## Limites e sequência

Este fechamento valida a base de controles, não todas as combinações futuras de habilidades. A2/Felipe ainda precisa conectar sua reação nativa e ser testada integralmente. Bleed e Poison têm contratos recebidos, ainda sem executor periódico; cleanse/transferência/WAVE novos também não são comprovados por este bloco. Catálogo completo, animação e aceite visual permanecem separados. Stage de pacote anterior não representa os fontes REG atuais; atualizar na entrega de integração, antes de empacotar.
