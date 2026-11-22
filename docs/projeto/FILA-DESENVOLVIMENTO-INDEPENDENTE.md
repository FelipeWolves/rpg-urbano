> **LAB-PVE-04 — 22/11:** [score/sobrevivência e recibo no Terminal](LAB-PVE-04-SCORE-SOBREVIVENCIA.md). Acessos na página2; janela12ações e meta3turnos naturais são parâmetros de laboratório. Aceites visuais02–04 pendentes; próximo05equipes sequenciais. Demo ~28% mantida.

> **LAB-PVE-03 — 22/11:** [ondas e fases no Terminal](LAB-PVE-03-ONDAS-FASES.md), com HUD/contadores e consulta de transições. Nova integração aguardando aceite visual; LAB-PVE-02 também não teve aceite informado ainda. Próximo LAB-PVE-04, score/sobrevivência. Demo ~28% mantida.

> **LAB-PVE-02 — 22/11:** [Ritual local integrado ao Terminal](LAB-PVE-02-RITUAL-TERMINAL.md), com objetivo, HUD, resultado e tentativas isoladas. Código/testes/build; aceite visual da nova rota pendente. Próximo LAB-PVE-03, ondas/fases. Demo ~28% mantida.

> **LAB-PVE-01 entregue — 22/11:** [observação e contrato de HUD](LAB-PVE-01-CONTRATO-HUD.md), sem nova tela/rota habilitada. Próximo: **LAB-PVE-02, Ritual no Terminal**; não depende de nova entrega. Demais itens da fila continuam planejados. Progresso global ~28% mantido.

# Fila de desenvolvimento independente das entregas

22/11/2026. Planejamento vigente, sem executar os itens nesta revisão. Não espera arte, áudio ou pesquisa adicionais para os cortes abaixo. Cada entrega segue código → teste pertinente → compile se aplicável → documentação/commit → aceite visual posterior. Sem processo autônomo em segundo plano.

| Ordem / ID de planejamento | Família | Entrega concreta | Pode fazer sem material novo? | Dependência real / aceite |
|---|---|---|---|---|
| 1 LAB-PVE-01 | ENG / PvE / VIS | Mapear adaptadores existentes, snapshots e eventos; contrato de HUD e rota do Terminal | Sim, usando casos de código existentes | Inspeção local; sem decisão nova de combate |
| 2 LAB-PVE-02 | HUB / ENG / PvE | Um Ritual simples acessível pelo Terminal, iniciar/retornar e resultado | Sim, fixtures identificadas como teste | Depende do contrato 01; você avalia a tela depois |
| 3 LAB-PVE-03 | WAVE / VIS | Indicadores de onda/fase e resumo das transições já aprovadas | Sim | Encontro integrado em 02; conferir visualmente depois |
| 4 LAB-PVE-04 | PvE / CIC | ScoreAttack e Survival com objetivo, limite e score/recibo | Sim | Reutiliza 01–03; balanceamento final pode vir depois |
| 5 LAB-PVE-05 | SEQ / VIS | Equipe ativa, próxima equipe, transição e estado persistido entre confrontos | Sim | Snapshot/transição; aceite após implementação |
| 6 LAB-PVE-06 | PAR / VIS | Três frentes, HP compartilhado e relógio global do chefe com identificação clara | Sim, no modelo local aprovado | Depois de SEQ/HUD; não promete simultaneidade de servidor |
| Paralelo LAB-QA-01 | VIS / QA | Feedback de ações/efeitos/recusas e roteiro de validação de save/tutorial/resolução | Sim | Usa eventos e contratos existentes; aceites reais ficam com você |

IDs LAB-PVE são cortes de planejamento novos, não blocos já implementados nem nova família. Registrar a evidência individual ao entregar. Não implementar todos os menus antes de validar o primeiro corte utilizável. Não duplicar regras no HUD.

## Fora da dependência das suas entregas

Preparar validadores de catálogo já definido, contratos de recursos, eventos de áudio/animação com placeholders, cenários de teste e documentação. Produção final de assets não é necessária para esses contratos. Priorização continua na tabela, para evitar dispersão.

## Exige sua entrega/decisão para fechar conteúdo final

Amostra de personagem/rig/animação/arena final; sons e vozes finais; ícones/retratos e procedência; roteiro curto final; kits/encontros inéditos não definidos. Não inventar regras faltantes, licenças ou recursos finais. Entrega parcial por ID/versão é suficiente.

## Exige aceite posterior, não material novo

Telas novas; persistência real após reabrir; backup/recuperação; resoluções/formaçōes; tutorial específico e acessibilidade. Você pode testar pelos programas; o assistente não deve abrir telas automaticamente. Cliques/vídeo/placeholder/seletor já aceitos não voltam à fila sem regressão concreta.

## Adiado por prioridade/permissão

Android/APK e backend mobile, cook/Shipping limpo, lojas/servidor, MCP opcional e clássico ampliado. Não instalar ou mudar rede. Build sem tela é possível com editor fechado; controle visual requer nova autorização.

[Progresso ~28%](PROGRESSO-GERAL.md) · [Sua preparação](../../entregas/NECESSIDADES-ATUAIS.md) · [Famílias](MAPA-FAMILIAS-E-PRIORIDADES.md).