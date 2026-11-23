# Recebimento e revisão — 22/11/2026

## Resultado

24 novidades em recebidos: 5 Markdown e 19 imagens. Total atual: 49 arquivos, sendo 20 Markdown e 29 imagens. Os 25 arquivos anteriores não apresentaram diferenças na comparação utilizada: hashes dos Markdown, presença e tamanho das mídias antigas. Não houve cálculo retroativo de hashes das mídias antigas.

Os 5 documentos foram lidos integralmente. As 19 imagens novas foram decodificadas localmente para conferir formato/dimensões. Inspeção visual amostral: logo, retrato, A2 adulta inativa e ícone compartilhado de imunidade/invencibilidade. Isso não constitui aceite visual de todas as artes na HUD nem prova procedência/licença.

Originais preservados. Nenhum anexo executado, recurso convertido, editor aberto, código runtime alterado ou teste de batalha executado. Receber documento não significa aprovar suas instruções como operações nem implementar automaticamente propostas.

## Material e compatibilidade

| Entrega | Confirmado | Ajuste / limite |
|---|---|---|
| Evento clássico A Bola Perdida | Proposta narrativa separada do tutorial padrão e destinada ao Lab-Exp | Manter futuro; ganhos de busca/tempestade, limites, simultaneidade das metas e relógio ainda não contratados. Não criar missão jogável por inferência |
| Tutorial padrão A Bola Perdida | Duas ondas, fichas de 4 unidades, cinco textos/etapas, pré-carga 4/5, A3 CD4 | Novo encontro desejado, não o tutorial técnico já implementado. Exige os pontos técnicos abaixo |
| Logo/avatar | logo-lobo-v01.png e portrait-felipe-v01.png, ambos 1254×1254 | Documento cita avatar-felipe-v01.png, ausente; provável correspondência com portrait, sem renomear. Logo possui canal alpha; retrato é RGB sem alpha. Conferir corte e brilho na HUD antes de substituir |
| Habilidades | 8 JPG 2048×2048, 4 adulto e 4 jovem | Documento cita PNG e subpasta adulta. Adultos estão na raiz de recebidos; jovens na subpasta declarada. Mapear pelo nome-base, sem fingir que PNG existe. JPG não tem alpha; branco externo permanece |
| Status | 9 PNG 353×307, RGB sem alpha | Arquivos já presentes apesar de documento dizer pendentes. Não são quadrados; preservar proporção ou preparar derivados com margem, sem esticar. Ícone verde não muda a faixa azul-clara dos escudos na barra de PV |

Amostra da A2 inativa é escura: validar contraste em tamanho real antes de declarar legibilidade. O arquivo de logo tem alpha no formato, não foi auditado pixel a pixel. Exportações menores e máscaras/alpha serão derivados futuros; nenhuma remoção automática de branco foi realizada. O material de origem permanece intacto.

## Pontos técnicos antes de implementar o tutorial

1. **P1 — Sangramento:** `src/reference_encounter.cpp` define a A1 com `requires_target_debuff=true`. O roteiro põe redução de SPD no Felipe e buff de ATK na Rajada, não debuff no inimigo; não há caminho declarado para a A1 infligir Sangramento no Trovão. Definir debuff de treino/forma de aplicação ou outra solução explícita, preservando o contrato da A1.
2. **P1 — Ordem pedagógica:** pré-carga 4/5 pode disparar na primeira habilidade inimiga, antes do painel da etapa 3. Esquiva/invencibilidade não contam como atingido. Definir gatilhos de avanço, elegibilidade/recusas e sequência controlada para demonstrar o roteiro sem inventar invulnerabilidade ou acertos garantidos.
3. **P1 — IDs/variante jovem:** piloto atual usa FELIPE/kit adulto e atributos20.000PV/2.500ATK/SPD110; recebido propõe CHAR_FELIPE_JOVEM com11.500/1.350/950DEF/SPD115. Registrar variante/IDs e ficha própria, não sobrescrever os laboratórios atuais. Tutorial usa nomes adultos enquanto mapa visual jovem dá nomes alternativos: confirmar se são aliases visuais ou habilidades diferentes antes de mudar semântica.
4. **P1 — Buffs temporários:** especificar alvo, início/fim e oportunidades de redução da duração dos novos -15%SPD/+20%ATK, política de reaplicação e aplicação inicial versus ação inimiga. `SetSpeed` existente não comprova automaticamente esse sistema percentual temporário.
5. **P1 — Persistência/resultado:** documento cita progress-v1.save para conclusão; atualmente piloto/tutorial são isolados em felipe-pilot-v1.save e felipe-tutorial-v1.save. Definir conclusão específica sem misturar saves, recompensas ou saldo genérico. HP0 é derrota e teto200 é empate conforme regra existente; preservar essa distinção.
6. **P2 — Demonstração de A3:** CD4 e dissipação até2buffs comuns já existem no kit, mas Rajada precisa ter o buff no momento da demonstração; dano multi-hit, bleed e cura entre ondas precisam simulação com a ficha nova. Valores declarados calibrados pelo texto não são balanceamento comprovado.
7. **P2 — Transição:** WaveV2 já existe, mas o adaptador visual não implica suporte a qualquer abertura/BattleStart e kit inédito. Reusar limpeza/cura30% sem duplicar regra no HUD; conferir sobrevivência, reset de etapa, morte e repetição.

## Mapeamentos visuais que não mudam regras

- A2_active representa feedback visual de disparo; duração/instante de brilho deve vir de evento existente. Não manter estado5/5 permanente nem inventar clique para passiva.
- Arquivo ico_status_immunity_invincible.png pode ser recurso visual compartilhado, mas Invincible e DebuffImmunity continuam efeitos distintos, com textos/duração/contadores próprios.
- Nome de arquivo petrification mapeia para EffectKind::Petrify, freeze para Freeze. O nome da imagem não cria enum novo.
- Cores/bordas da arte não redefinem categoria mecânica, imunidades ou as cargas douradas da A2. Separar sprite, moldura de categoria e estado de interface.

## Ordem recomendada

1. Registrar recursos e correspondências reais; preparar contrato de catálogo/importação sem modificar originais.
2. Integrar logo/retrato/ícones por derivados e adaptador visual em entrega própria; exige compilação/aceite posterior, não nova fórmula de combate.
3. Resolver P1 do tutorial e só então montar fixture e testes guiados. Evento clássico continua proposta futura.

Inventário e metadados: docs/relatorios/atuais/RECEBIMENTO-2026-11-22.csv e .json. Originais Markdown podem entrar no Git; imagens permanecem locais. Percentual da demo mantido em ~28%: nova entrega de material não é implementação nem aceite.
