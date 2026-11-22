# ESC/SIS/PvE — consolidação técnica

22/11/2026. Entregas de código conforme os contratos recebidos. Relatórios finais: ESC-SIS-PVE-TESTES.json e ESC-SIS-PVE-FECHAMENTO.json em docs/relatorios/atuais. Não somar baterias sobrepostas como medida única de cobertura.

| Corte | Entrega verificada | Limite |
|---|---|---|
| C1 / ESC | SilverShield15%/2naturais/mínimo1 confirmado, camada apósStandard e reaplicação por máximos | Não é kit completo do Filhote |
| C1 / SIS | Permissões por categoria, remoção/roubo/transferência, cura em pontos explícitos | Não inventa fórmula universal de cura |
| C2 / WAVE | WaveV2: limpar comuns antes de curar30% dos vivos; preservar especiais/CD/cargas | Política explícitaversão2; Preserve/Reset antigos continuam |
| C3 / SCORE | Efetivo=absorção+HP;25%overkill com piso final; teto1e12; epoch+ação e AttemptID idempotentes | Recibo/faixa local; sem ranking, moeda universal ou save novo |
| C4 / PvE | Ritual por chefe obrigatório, janelaScoreAttack e Survival por turnos declarados do chefe | AP/SPD. Score desativado por padrão em outros modos |
| C4 / SEQ | Sequência pontuada de3equipes, sem retorno cíclico | Adaptador separado dos resultados do coordenador histórico |
| C5 / fases | HP, turnos e ciclos; kit estável ou substituição; recargasKeep/Reset/Map; limpeza/cura/estados declarados | Fronteira após cadeia/R04; sem turno artificial; ciclo+lista sem precedência é recusado |
| C5 / entidades | Boss/Auxiliary/Summon, reservas/dissipação, imunidades de chefe e resistênciaAP configurável | Summon não evita derrota nem volta implicitamente ao reviver o invocador |
| C5 / frentes | SharedBossv2: área local/global, auxiliares e passiva/escudo restritos à própria frente | HPúnico/barra global; não é multithreading, rede ou servidor |
| C5 / SEQv2 | PreserveSpecialBordersV2, com kit estável ou variável | Não altera PreserveClassifiedStates/KeepCooldownsClearStatuses anteriores |
| C6 / engenharia | Doze baterias de famílias, regressão geral, UBT e stage66entradas aprovados | Compilar não é executar ou aprovar UI/Shipping/mobile |

## Código e execução local dos testes

Battle contém camadas/entidades/mutação atômica; status_operations.hpp contém permissões; score.hpp contém acumulação e recibos; stage contém ondas; pve contém modos/fases; sequential e shared_boss contêm seus coordenadores versionados.

scripts/testar-esc-sis-pve.ps1 executa as12baterias compilando o core apenas uma vez. scripts/testar-regras.ps1 mantém seleção individual. scripts/testar-combate.ps1 executa a regressão anterior. Não instalam nada nem iniciamUnreal/Android. Fixtures emtests não são configurações finais de chefes.

As pontes Unreal novas compilamStage,Sequential,SharedBoss ePvE usando os fontes canônicos. O stage inclui esses módulos e seus headers. Não houve cook nem pacote executado.

## Retestes e trabalho que permanece

1. **Entrada e vídeo:** cliques da batalha, impossibilidade de trocar alvos e duplo clique do vídeo continuam REABERTOS em RETESTES-ENTRADA-E-VIDEO.md. Não corrigidos nem aceitos nesta rodada.
2. **Apresentação dos modos:** ligação aos menus/HUD, seleção visual, barras e telas de score/ondas/fases. Classes compiladas não criam botões novos automaticamente. PilotoFelipe e atalhos anteriores preservados.
3. **Conteúdo/balanceamento:** kits reais, duração dePoison por aplicador, chefes/limiares, geradores/auras não especificados e tabelas de dificuldade/bônus/faixas/recompensas. Não reenviar contratos fechados nem exigir catálogo inteiro para avançar.
4. **Regras futuras:** precedência entre ciclo de turnos e lista de fasesHP; novos gatilhos/alcances por habilidade/equipamento e outros escudos precisam de contrato próprio. Os perfis recebidos já são configuráveis/testados.
5. **Persistência/plataformas:** recibos de score em disco/conta, migração de save, pacotes/Shipping, Android/iOS, desempenho e determinismo medido fora do Windows.
6. **Fora deste grupo:** Clássico completo com itens/SP/Limit/ATBActive, DJ, lojas e serviços. StructuralShield legado não foi convertido em categoria/cor.

A regressão detectou aceitação indevida de seleção dinâmica no chefe compartilhado; a rejeição legada foi restaurada e a regressão completa passou. Operação resistida não pode emitir aplicação falsa; esse caso ganhou teste. Objetos dos testes ficaram separados dos objetos do core para evitar colisãoPve.obj no Windows. Falhas iniciais não foram usadas como aceite.

Sem telas, execução de jogo/editor/commandlet, Android, internet, instalações, alteração de rede ou push. Mídias não entraram nos commits.
