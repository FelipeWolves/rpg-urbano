# VIS-01A — adaptador concluído

19/11/2026. ✅ Contratos nativos entregues: 6/6 casos novos e213 testes anteriores aprovados. Sem cena, widgets ou execução na Unreal.

Cenário compartilhado extraído de text_session.cpp para reference_encounter.cpp, mantendo os dados. O header textual reexporta os nomes para preservar seus consumidores. VisualSession possui uma LocalSession, fornece cópias do estado, executa uma ação por chamada e expira comandos após ação aceita ou reset. Nenhuma fórmula de dano/AP/recarga foi duplicada.

Revisão antiga ou lado incorreto são recusados antes de tocar o núcleo. Recusa do núcleo preserva estado; resultado e recompensa seguem LocalSession. Automação avança uma ação por chamada e para após256 ações seguidas sem entrada manual, sem declarar vencedor. Reset limpa a parada e preserva o contrato de progresso. Estado nativo usa IDs e não depende de tradução.

Testes: recusa atômica; comando antigo/lado B; equivalência com LocalSession; fim/recompensa/reinício; passiva pura e condição bloqueada; limite automático. A última verificação recompilou apenas os seis casos após completar a condição. Compilação /W4 /WX aprovada. [Resultado](../relatorios/atuais/VIS-01A.json). Não somar esses casos como execuções na engine.

O roteiro geral passa a incluir os novos casos. A extração exigiu repetir a regressão; não repetir por rotina. A configuração UAT existente ainda não inclui esses novos fontes: integração no manifesto e no módulo será feita no VIS-01B. Pacote anterior continua evidência da versão anterior, não contém o adaptador.

Próximo: VIS-01B, cena e painel. Ainda faltam o vínculo de eventos, a seleção visual e a compilação Unreal. Não foram validados interface, toque ou resolução variável. Sem novos saves ou serviços.
