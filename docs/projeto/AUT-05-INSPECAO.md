# AUT-05 — cenários de inspeção de formações e escudo

21/11/2026. Preparação em código, testes e compilação concluídas. **Aceite visual pendente do usuário.** Não houve abertura do editor, prévia ou Android.

## Como usar depois

Na raiz principal, abrir **Abrir-Inspecao.cmd** para a disposição padrão em profundidade, ou **Abrir-Inspecao-Classico.cmd** para a lateral clássica. Anterior/Próximo alternam os casos; Detalhes mostra o valor esperado e os efeitos reais do foco. O [i] junto à barra também usa o painel compartilhado. Feche a janela da prévia para sair; a inspeção não volta a uma sessão jogável por um botão implícito.

Não precisa jogar uma batalha inteira. O modo é explicitamente somente leitura, com snapshots estáticos. A partida normal continua3×3 e não usa esses dados sem a opção de inspeção. Também é possível indicar `-Inspection=ShieldApplied`, por exemplo; nome desconhecido avisa e usa o primeiro caso de inspeção, sem iniciar uma partida normal.

## Casos disponíveis

| ID | O que conferir |
|---|---|
| Formation1v1 | Uma unidade por lado |
| Formation2v1 | Assimetria2contra1 |
| Formation3v3 | Referência3contra3 |
| Formation5v5 | Cinco unidades por lado |
| Formation1v5 | Uma contra cinco |
| Formation5v1 | Cinco contra uma |
| ShieldBefore | A2 com50/100PV, sem escudo |
| ShieldApplied | A1 com1000PV máximos concede10 pontos, duração3; A2 continua50PV |
| ShieldConsumed | Ataque8 deixa2 de escudo e50PV |
| ShieldReapplied | Reaplicação restaura força10/duração3, sem somar2+10 |
| ShieldExhausted | Ataque18 absorve10 e deixa42PV, escudo0 |
| ShieldExpired | Após3turnos naturais do protegido, escudo0 e50PV |
| ShieldDeath | Ataque letal deixa0PV e remove escudo; modelo deve ficar oculto |

No caso aplicado,10 pontos sobre50PV de um máximo100 representam10% da barra inteira e20% da parte preenchida. A força não é recalculada usando50% da vida do protegido. Fórmula mínima1, reaplicação por maiores força/duração e demais ESC permanecem as mesmas; não foram reimplementadas.

## Implementação e isolamento

Fixtures em reference_encounter.hpp/.cpp usam o Battle real para produzir snapshots. Casos usam relógio de rodadas fixas para um roteiro curto de reprodução; isso não substitui os testes do sistema AP/SPD nem comprova jogabilidade5×5. Dano parcial/excedente/letal vem do ATK configurado do inimigo (8/18/100), não de um dano fictício na apresentação. Expiração avança por ataques básicos a outros alvos sem ferir A2, sem introduzir Defender como comando do perfil padrão.

A arena reaproveita a mesma disposição, barras, cálculo shield_display e painel effect_details. Apenas atores criados pelo protótipo com tag reservada RpgPrototypeActor são substituídos ao trocar o caso; nenhum mapa/asset é alterado. Sessão normal não recebe ações, reset, liquidação ou recompensa durante a inspeção. A escolha de alvo é desabilitada e não há automação. Foco/modal/caso participam do contexto de entrada AUT-03.

Foi removida a pré-quebra legada por78 caracteres de OpenInfo; a quebra medida do AUT-04 agora recebe os parágrafos completos. Assim a inspeção de efeitos usa o mesmo mecanismo, sem cortes artificiais antes da medição da fonte.

## Evidências

-10 grupos novos para13casos: identidades, contagens, posições nos dois layouts, aplicação, consumo, reaplicação, excesso, expiração, morte e repetibilidade/preservação do roster normal.
-6 grupos existentes de layout e6 de apresentação aprovados; total desta bateria focada:22grupos. Não somar como casos exclusivos às54verificações da outra bateria nem aos213gerais.
-UBT Editor Win64 Development:9ações, saída0; `build/aut05-build.log`.
-Valores capturados em AUT-05-CENARIOS.csv e resultados em AUT-05-TESTES.json. Comando de teste: scripts/testar-inspecao-arena.ps1.
-Os testes detectaram dados iniciais incompletos e o uso inadequado de amount como dano fixo nos primeiros roteiros. Fixtures foram corrigidas conforme o executor existente; o núcleo não foi alterado. Logs dessas tentativas permanecem em build/arena-inspection.

## Limitações e próximo bloco

Preparar snapshots não valida leitura, enquadramento, sobreposição de barras, animação ou desempenho. Conferir visualmente1/3/5, assimetrias, foco/efeitos e os dois layouts. Morte remove o modelo; barras de mortos seguem o comportamento atual do renderer, ainda sujeito ao aceite. Não inclui chefes compartilhados, equipes paralelas,6×6, teste de servidor, produção ou Android.

AUT-05: cenários e adaptador compilados, aceite visual pendente. Próximo AUT-06: apresentação consistente de básica, passiva, recarga, condição e indisponibilidade conforme APIs existentes, sem inventar silêncio/supressão ainda não implementados. Progresso geral mantido aproximadamente22% até novos aceites.
