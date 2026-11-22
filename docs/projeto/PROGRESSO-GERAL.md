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