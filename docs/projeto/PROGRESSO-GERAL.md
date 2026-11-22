> **Vigente — VIS/ENG/HUB P0, 22/11:** [correções, evidências e reteste](VIS-ENG-HUB-P0-CORRECOES.md). Entrada/alvos e preparação do player atualizados em código; testes nativos aprovados. Aceites visuais continuam abertos; Android adiado e demo ~25% mantida. Próximo: reteste do usuário, depois integração visual dos modos já disponíveis. Esta nota prevalece sobre próximos passos históricos abaixo.

> **Mapa vigente das famílias — 22/11:** [percentuais, tarefas independentes, ajustes, dependências e prioridades](MAPA-FAMILIAS-E-PRIORIDADES.md). Organização somente; demo geral mantida em ~25%. Próximo recomendado: cliques/alvos e primeira reprodução. Bases encerradas não são reabertas por rotina.

# Progresso geral do projeto

Atualizado em **22/11/2026**. **Demo integrada estimada em aproximadamente 25% (faixa orientativa: 23–27%).**

Referência: demo com Felipe, uma arena e apresentação integrada testada em um celular-alvo. Não mede o jogo comercial completo, 67 capítulos, todas as lojas ou serviços online. Percentuais são estimativas de maturidade; não medem horas nem garantem prazo.

## Atualização ESC/SIS/PvE — 22/11

Core52→60%, QA28→30% e documentação79→80%. Soma orientativa25,15%; demais áreas mantidas. São estimativas de maturidade, não horas ou porcentagem medida de esforço. Nenhuma arte/mobile nova foi aceita. Clique, troca de alvos e vídeo reabertos para reteste. Novos modos estão em código/ponte, sem menu/HUD próprio.

## Áreas

| Área | Progresso | Peso na demo | Base / limite |
|---|---:|---:|---|
| Código core | 60% | 20% | ESC/SIS/PvE de código testado: escudos, ondas, score, modos/fases/frentes; catálogo completo ainda pendente |
| Integração Unreal | 45% | 10% | Projeto estável compilado, mapa salvo e reabertura/Play observados |
| Front / interface | 22% | 10% | Inspeção dos dois layouts/detalhes aceita; novos cartões compilados aguardam aceite |
| Hub / navegação | 20% | 7% | Sete locais e painéis informativos implementados; aceite visual pendente |
| Arte 2D | 25% | 5% | Panorama novo conectado ao catálogo; aceite visual pendente |
| Cenários 3D | 5% | 10% | Arena geométrica provisória |
| Personagens / rig / animação | 0% | 12% | Sem recursos finais integrados |
| VFX / cinematics / vídeo | 5% | 5% | Player local parcial; aceites e acabamento pendentes |
| Áudio / música | 0% | 5% | Direção documentada; integração pendente |
| Conteúdo / tutorial | 10% | 5% | Recorte e universo definidos; tutorial não implementado |
| Progressão / save | 25% | 5% | Save v1/recompensa; menu gráfico compilado, gravação/reabertura pela interface pendentes |
| Mobile / otimização | 5% | 4% | AVD iniciou; APK e desempenho não validados |
| QA integrado | 30% | 1% | Doze baterias e regressão geral aprovadas; cliques/alvos/vídeo reabertos, mobile/desempenho pendentes |
| Documentação | 80% | 1% | Consolidação ESC/SIS/PvE, retestes e limites de UI/plataformas atualizados |

Pesos somam 100; soma ponderada estimada = 25,15%, arredondada para comunicação. Pesos são premissas de planejamento revisáveis, não precisão de medição. Serviços e lojas ficam fora da demo (implementação 0%).

## Fechamento SIS/REG/HAB — 22/11

Core45→52%, QA26→28%, documentação78→79%. Demais áreas mantidas; sem avanço validado de arte3D/mobile ou aceite visual do piloto. São estimativas de maturidade, não medição de esforço restante. [Entrega](HAB-FELIPE-PILOTO.md).

## Histórico de atualizações

A faixa histórica 15–20% não tinha pesos explícitos. Agora a arte recebida e o boot do AVD são reconhecidos dentro de limites pequenos; core passa de 55 para 45% pelo escopo ampliado de atributos, ponto fixo, bloqueio e efeitos, não por perda de funcionalidades. Não comparar diretamente as duas estimativas como velocidade de produção. Novas regras respondidas não recebem crédito de implementação.

Seletor no chão corrigido e aceito; texto e combate confirmados pelo usuário. Não reabrir esses defeitos sem evidência nova. Mobile, fluidez, player e formação/resolução ampla permanecem separados. Nenhum teste foi repetido por esta atualização documental.

[Consolidação e próximas fases](CONSOLIDACAO-2026-11-21.md) · [Tarefas](TAREFAS.md) · [Estado](ESTADO.md) · [Fila independente](FILA-DESENVOLVIMENTO-INDEPENDENTE.md)

Arte, roteiro, áudio e recursos 3D podem ser produzidos sem terminar todas as habilidades. Integração depende de contratos de IDs, dimensões, escala, eventos e direitos. Código/UI provisórios podem avançar sem novas entregas. Abertura padrão Unreal concluída no Windows local; não representa APK pronto.

Integração Unreal 38→45%, QA22→24% e documentação75→78% nesta etapa. Soma ponderada 21,87%, arredondada para22%. [Evidências e limites](UNREAL-PROJETO-ESTAVEL.md).

[Hub da cidade](HUB-PANORAMA-CIDADE.md): código/compilação e26grupos nativos aprovados; percentuais mantidos até aceite visual, sem confundir os sete painéis com sete sistemas completos. AUT-01 implementado; Android adiado.

[AUT-02](AUT-02-GEOMETRIA.md): geometria/margens simuladas implementadas,34grupos de apresentação e build na pasta nova aprovados. Percentuais mantidos até aceite visual; safe area real e Android não validados.

[AUT-03](AUT-03-ENTRADA.md): entrada unificada e cancelamentos implementados/testados,44grupos nativos e build aprovados; percentual mantido até aceite interativo. Android adiado.

[AUT-04](AUT-04-PAINEIS.md): painéis unificados, quebra medida e paginação implementados,54grupos de apresentação e build aprovados. Estimativas mantidas até aceite visual; Android segue adiado.

[AUT-05](AUT-05-INSPECAO.md):13cenários de inspeção,22grupos focados e build aprovados. Aceite visual de formações/escudo continua pendente; percentuais mantidos, sem alteração de regras ESC ou do roster padrão.

AUT-05 aceito pelo usuário nos dois layouts/detalhes. Front20→22% e QA24→26%; soma ponderada22,09%, mantendo comunicação de aproximadamente22% da demo. AUT-06 compilado, aparência ainda pendente; Android adiado. [Registro](AUT-06-HABILIDADES.md).

[AUT-07](AUT-07-PREFERENCIAS.md):64grupos de apresentação e compilação aprovados; codec/recuperação testados, gravação real/menu ainda não executados. Estimativas mantidas, sem antecipar aceite de persistência ou acessibilidade completa.

22/11: ponteiro corrigido e aceito; [AUT-08](AUT-08-FLUXO-TELAS.md) testado e compilado, menus novos ainda sem aceite interativo. Estimativas mantidas em aproximadamente22% da demo; não antecipar conclusão de save gráfico, equipe ou Android.

## AUT-09 — 22/11/2026

Equipe, resumo de resultado e adaptador de save implementados/compilados; 64 grupos focados aprovados. Aceite interativo e persistência pela interface ainda pendentes. Estimativa ponderada mantida em 22,09% (aproximadamente 22%); compilação não equivale a demo aceita em celular. Ver [entrega e roteiro](AUT-09-EQUIPE-RESULTADO-SAVE.md).

## AUT-10 — 22/11/2026

Contrato de mídia local implementado e testado (12 grupos novos; 94 de apresentação no total desta bateria). Sem reprodução ou alteração visual. Percentuais mantidos, incluindo estimativa ponderada de 22,09% da demo. Próximo: AUT-11, backend e integração. Ver AUT-10-MIDIA-LOCAL.md.

## AUT-11 — 22/11/2026

Ensaio manual de player local compilado, sem reprodução observada. Flag, codecs, áudio, legendas/mixer e aceites continuam pendentes. Estimativa mantida em 22,09% da demo; integração compilada não recebe aceite de mídia. Ver AUT-11-PLAYER-LOCAL.md.

## AUT-12 a AUT-16 — 22/11/2026

Entregas consolidadas em CONSOLIDACAO-AUT-12-16.md: runtime do marcador, canal de cues, schema de referência, stage e QA por área. Percentuais mantidos até aceites pertinentes; estimativa continua 22,09% (aproximadamente 22%). Sem teste visual/mobile/Shipping nesta rodada. Clique inicial da intro provisório, REVER. Não somar as baterias distintas como suite única.
