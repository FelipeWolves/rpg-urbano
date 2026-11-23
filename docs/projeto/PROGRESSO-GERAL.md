> **Reteste23/11:** usuário aceitou logo, PRAÇA e direção tipográfica. Letreiros agora Light/menores, hover só luminoso no padrão; alto contraste preservado. Content/Presentation excluído da importação automática apenas no projeto. Build/stage93 aprovados; acabamento e supressão do aviso aguardam reteste. Fonte docs/projeto/HUB-LETREIROS-E-IMPORTACAO-2026-11-23.md. Demo~28% mantida.

> **Integração23/11:** recursos ligados à HUD; tutorial padrão no Terminal; busca clássica no Lab - Exp; conclusão local própria, PRAÇA/letreiros corrigidos. Código/testes/build e stage92 aprovados; aceites gráficos/I-O real/mobile separados. Demo~28% mantida. Fonte: docs/projeto/INTEGRACAO-VIS-TUTORIAIS-2026-11-23.md. Próximo: aceite da versão integrada, sem depender da arte futura do Terminal.

> **LAB-QA-01C — 22/11:** persistência compartilhada entre HUD Unreal e QA; 16/16 testes isolados, compilação e stage69 aprovados. Formatos/políticas preservados; saves normais intocados. Aceites LQA-09/10 e runtime Unreal isolado de LQA-14 continuam separados. Demo ~28% mantida. Fonte: docs/projeto/LAB-QA-01C-PERSISTENCIA-COMPARTILHADA.md. Próxima frente independente proposta: revisar contratos de áudio/animação sobre eventos existentes, sem repetir AUT-13.

> **LAB-QA-01B — 22/11:** testes de disco em pasta nova de QA, com codecs reais e backend Windows próprio; saves normais intocados. Não equivale a executar IFileManager/interface Unreal. LQA-14 de integração continua adiado. Próximo independente LAB-QA-01C: contrato compartilhado de persistência; aceites visuais em paralelo. Demo ~28% mantida. Detalhes: docs/projeto/LAB-QA-01B-IO-ISOLADO.md.

> **LAB-QA-01 — 22/11:** feedback de ações e recusas integrado; detalhes consultáveis e matriz de 16 aceites preparada. Código/testes não equivalem a aceite visual ou I/O real. Próximo independente: LAB-QA-01B, I/O em diretório isolado; usuário pode preencher a matriz em paralelo. Demo ~28% mantida. Ver docs/projeto/LAB-QA-01-FEEDBACK-E-ACEITES.md.

> **LAB-PVE-06 — 22/11:** [três frentes com chefe único](LAB-PVE-06-FRENTES-COMPARTILHADAS.md) no Terminal, página3, ataques locais e especial global. Contadores por frente e limite global, sem limite local novo. LAB01–06 em código; aceites02–06 pendentes. Próximo recomendado LAB-QA-01/aceites. Demo ~28% mantida.

> **LAB-PVE-05 — 22/11:** [três equipes sequenciais no Terminal](LAB-PVE-05-EQUIPES-SEQUENCIAIS.md), variantes de chefe fixo/faseado, HP contínuo e HUD. Janela4ações totais/equipe é fixture explícita. Aceites02–05 pendentes; próximo06frentes compartilhadas. Demo ~28% mantida.

> **LAB-PVE-04 — 22/11:** [score/sobrevivência e recibo no Terminal](LAB-PVE-04-SCORE-SOBREVIVENCIA.md). Acessos na página2; janela12ações e meta3turnos naturais são parâmetros de laboratório. Aceites visuais02–04 pendentes; próximo05equipes sequenciais. Demo ~28% mantida.

> **LAB-PVE-03 — 22/11:** [ondas e fases no Terminal](LAB-PVE-03-ONDAS-FASES.md), com HUD/contadores e consulta de transições. Nova integração aguardando aceite visual; LAB-PVE-02 também não teve aceite informado ainda. Próximo LAB-PVE-04, score/sobrevivência. Demo ~28% mantida.

> **LAB-PVE-02 — 22/11:** [Ritual local integrado ao Terminal](LAB-PVE-02-RITUAL-TERMINAL.md), com objetivo, HUD, resultado e tentativas isoladas. Código/testes/build; aceite visual da nova rota pendente. Próximo LAB-PVE-03, ondas/fases. Demo ~28% mantida.

> **22/11 — LAB-PVE-01:** snapshot de encontro e contrato de HUD implementados/testados; mapeamento das demais pontes documentado. Sem novas telas ou aceite visual, percentuais mantidos. Próximo LAB-PVE-02. [Entrega](LAB-PVE-01-CONTRATO-HUD.md).

# Progresso geral do projeto

Atualizado em **22/11/2026**, após os aceites visuais de fe6060b e alinhamento e1197d5.

**Demo integrada: aproximadamente 28% (faixa orientativa 26–30%).** Soma ponderada: **28.03%**. Anterior: 25,15%. Mesmos pesos; diferença reconhece integração e aceites já realizados, não implementação nova nesta revisão.

O denominador é a primeira demo com Felipe, uma arena, apresentação e teste em celular-alvo. Não mede o jogo comercial, elenco/campanha completos, multiplayer ou lojas. Estimativas de maturidade; não são medição de horas nem promessa de prazo.

| Área | Progresso | Peso | Evidência / limite |
|---|---:|---:|---|
| Código core | 60% | 20% | ESC/SIS/PvE testados; catálogo completo e integração dos modos pendentes |
| Integração Unreal | 50% | 10% | Dois laboratórios compilados, placeholders e entrada aceitos; pacote limpo e plataformas pendentes |
| Front / interface | 30% | 10% | Cliques, seleção, painéis e seletor aceitos no recorte testado; HUD PvE, fluidez e matriz ampla pendentes |
| Hub / navegação | 30% | 7% | Terminal e piloto Felipe acessíveis pelo hub e aceitos; sete destinos não significam sete sistemas |
| Arte 2D | 25% | 5% | Panorama e recursos autorais recebidos/conectados; sem nova produção ou aceite integral de arte final |
| Cenários 3D | 5% | 10% | Arena geométrica provisória; sem cenário 3D final |
| Personagens / rig / animação | 0% | 12% | Sem personagem, rig ou animações finais integrados |
| VFX / cinematics / vídeo | 15% | 5% | Player, primeiro clique, hover e skip aceitos no Windows; vídeo por compilação; mobile/legendas/VFX pendentes |
| Áudio / música | 0% | 5% | Direção recebida; sem mixer, música ou voz finais integrados |
| Conteúdo / tutorial | 15% | 5% | Tutorial técnico Felipe implementado; sem aceite específico do roteiro/tutorial completo |
| Progressão / save | 25% | 5% | Save v1 e sessões isoladas; persistência real e recuperação via UI ainda não aceitas |
| Mobile / otimização | 5% | 4% | AVD com boot anterior; sem APK, toque real, desempenho ou backend mobile validados |
| QA integrado | 38% | 1% | Retestes Windows aceitos e regressões pertinentes aprovadas; save/resoluções/Shipping/mobile pendentes |
| Documentação | 85% | 1% | Estado reconciliado com commits e aceites; fila por dependência e recebíveis atualizados |

## Concluído no recorte atual

- Core de batalha: ponto fixo, funil, controles, Bleed/Poison, Felipe A1/A2/A3, escudos, ondas, score, fases e frentes em código e testes; nem todos têm interface integrada.
- Windows: abertura padrão do projeto, entrada com clique único, seleção/troca de alvos, recargas, placeholders e seletor; usuário aprovou os últimos ajustes no cenário testado.
- Vídeo: início com um clique e skip/hover aprovados; marca de assistido vinculada ao binário, sem apagar saves.
- Hub: Terminal genérico e piloto Felipe via Arena PvP temporária, ambos locais e com sessões/saves isolados. Nenhum PvP online implementado.
- Base Team comum para ambos; limite 200 ações/empate. Kits individuais preservados. Clássico fora desta frente.

## Ainda depende de aceite seu

Persistência real de equipe/progresso/preferências após fechar e reabrir; backup/recuperação; tutorial específico; resoluções/formações e acessibilidade amplas; futuras telas dos modos. Não repetir aceites de clique/vídeo/seletor por rotina. Compilação/teste nativo não substitui essas provas.

## Ainda depende de entregas ou decisões suas

Recursos finais 3D/rig/animações, áudio/voz, ícones/retratos e direitos, roteiro final e kits/encontros novos cujas regras não foram definidas. Pode enviar uma amostra parcial por área; não precisa completar tudo. Arte provisória e dados de laboratório permitem desenvolver os fluxos antes.

## Próximos passos independentes das entregas

1. Mapear os adaptadores dos modos já implementados e o contrato de dados do HUD, sem duplicar combate.
2. Integrar no Terminal um encontro Ritual simples com estados de objetivo e resultado.
3. Mostrar ondas e fases; depois ScoreAttack/Survival e resumo de score.
4. Integrar trocas sequenciais; depois frentes paralelas/chefe compartilhado, que exigem apresentação mais complexa.
5. Melhorar feedback de ações/efeitos/recusas e preparar a matriz de aceite de save/tutorial/resoluções.

Esta lista é planejamento, não execução nesta atualização. Android, instalações, MCP e controle de tela continuam fora da fila imediata. Regras inéditas não serão inventadas para desbloquear conteúdo final.

[Dependências e sua preparação](../../entregas/NECESSIDADES-ATUAIS.md) · [Fila do desenvolvimento](FILA-DESENVOLVIMENTO-INDEPENDENTE.md) · [Famílias e siglas](MAPA-FAMILIAS-E-PRIORIDADES.md) · [Pendências](PENDENCIAS.md).

Histórico de percentuais e estados anteriores permanece no Git e nos relatórios datados. Este painel substitui notas acumuladas que ainda diziam cliques/vídeo reabertos, tutorial inexistente ou AUT por iniciar. Nenhum teste, build ou jogo foi executado nesta revisão documental.

> Recursos22/11 preparados: 17icones transparentes e logo/avatar; fontes preservadas, catálogo e11gruposPilot aprovados. HUD e tutorial narrativo ainda não integrados. Demo~28% mantida; ver VIS-ICONS-PNG-2026-11-22.md.
