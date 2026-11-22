> **LAB-PVE-05 — 22/11:** [três equipes sequenciais no Terminal](LAB-PVE-05-EQUIPES-SEQUENCIAIS.md), variantes de chefe fixo/faseado, HP contínuo e HUD. Janela4ações totais/equipe é fixture explícita. Aceites02–05 pendentes; próximo06frentes compartilhadas. Demo ~28% mantida.

> **LAB-PVE-04 — 22/11:** [score/sobrevivência e recibo no Terminal](LAB-PVE-04-SCORE-SOBREVIVENCIA.md). Acessos na página2; janela12ações e meta3turnos naturais são parâmetros de laboratório. Aceites visuais02–04 pendentes; próximo05equipes sequenciais. Demo ~28% mantida.

> **LAB-PVE-03 — 22/11:** [ondas e fases no Terminal](LAB-PVE-03-ONDAS-FASES.md), com HUD/contadores e consulta de transições. Nova integração aguardando aceite visual; LAB-PVE-02 também não teve aceite informado ainda. Próximo LAB-PVE-04, score/sobrevivência. Demo ~28% mantida.

> **LAB-PVE-02 entregue em código:** Ritual no Terminal e ponte Team/PvE, sem novas regras de dano. Aceite visual pendente; próximos ondas/fases em LAB-PVE-03. Percentuais mantidos.

> **22/11 — LAB-PVE-01:** snapshot de encontro e contrato de HUD implementados/testados; mapeamento das demais pontes documentado. Sem novas telas ou aceite visual, percentuais mantidos. Próximo LAB-PVE-02. [Entrega](LAB-PVE-01-CONTRATO-HUD.md).

# Famílias, siglas e prioridades

Atualizado em22/11/2026 após os aceites Windows. **Demo ~28%**, ponderação em PROGRESSO-GERAL.md. Percentuais por família se sobrepõem: não somar nem tirar média. VIS22→30, ENG45→50, HUB20→30; demais estimativas mantidas por ausência de nova evidência no respectivo escopo.

| Sigla e significado | Estimativa | Prioridade | Próximo desenvolvimento possível | Dependência |
|---|---:|---|---|---|
| VIS — Visual e interação | 30% | P1 | HUD dos modos e feedback de ações/efeitos, com recursos provisórios. | Não requer nova arte para código; cada tela nova recebe aceite posterior. |
| ENG — Integração Unreal | 50% | P1 | Adaptar controladores PvE ao projeto padrão e expor snapshots/eventos para UI. | Pode avançar sem MCP, Android ou recursos finais. |
| HUB — Central de navegação | 30% | P1 | Rotas do Terminal para testes PvE, objetivo/resultado e retorno seguros. | Código com placeholders independente; conteúdo final depende de amostras. |
| PvE — Encontros e chefes | 65% | P1 | Integrar um Ritual de laboratório ao fluxo atual, depois ScoreAttack e Survival; usar dados provisórios identificados. | Regras dos controladores prontas; integração independente de arte final; aceite após UI. |
| WAVE — Ondas | 75% | P1 | Conectar contador de ondas e resumo de limpeza/cura ao encontro; usar WaveV2 já testado. | Um encontro integrado; conteúdo definitivo não impede a primeira versão. |
| CIC — Ciclo de jogo e persistência | 25% | P1 | Definir identidade de encontro/modo nos dados e preparar persistência versionada de resultados/recibos; manter idempotência. | Definições econômicas para recompensas finais; armazenamento local pode avançar sem lojas/servidor. |
| HAB — Habilidades | 65% | P1/P2 | Organizar catálogo/dados do kit recebido e descrições; facilitar novos kits sobre os executores existentes. | Fichas novas somente para habilidades não definidas; não exigir o elenco inteiro. |
| SIS — Sistemas gerais | 55% | P1/P2 | Formalizar dados e validação de atributos/resultados e interfaces de inventário/equipamento; separar sistemas por responsabilidade. | Pode estruturar contratos técnicos; regras novas de produto precisam de decisão antes da execução funcional. |
| ESC — Escudos | 80% | P2 / integração | Reutilizar Standard/Silver e ligar suas camadas/descrições ao HUD e à inspeção. | Novos tipos de escudo dependem de ficha; os dois atuais não dependem de pesquisa nova. |
| REG — Regras e contratos de combate | 80% | P2 / sob demanda | Garantir motivos de recusa e resultados observáveis; validar apenas novas regras que entrem no recorte. | Não depende de pesquisa para os contratos já fechados; regra inédita exige definição. |
| SEQ — Equipes sequenciais | 75% | P2 / após Ritual e ondas | Preparar apresentação da equipe ativa, troca e resultado, usando o coordenador e a política v2. | Interface de um encontro e seus resultados integrada primeiro. |
| PAR — Frentes paralelas locais | 70% | P2 / depois de SEQ | Preparar seleção/indicadores de frente, HP global e sinalização de área local/global. | Experiência de uma frente e seleção de alvos estáveis. |
| ART — Arte e animação | 10% | P2 / paralelo | Preparar convenções de nomes, escala, pivô, importação e pontos de conexão; usar placeholders existentes. | Recursos autorais/licenciados para acabamento; planejamento e placeholders não dependem deles. |
| AUD — Áudio, música e vozes | 0% | P2 / paralelo | Estruturar canais/volumes e eventos de áudio; preparar preferências, mute e movimento/ciclo de vida sem áudio final. | Sons/músicas/vozes próprios para integração final e teste auditivo. |
| EXP — Expansão: eventos, campanha e DJ | 5% | P3 | Organizar contratos de evento e rascunhos de fluxos; preservar o perfil clássico existente. | Recorte e regras de cada experiência; dados narrativos recebidos devem ser reutilizados. |
| AMB — Ambiente e ferramentas | 70% | P2 / quando retomar mobile | Manter ambiente Windows; não instalar MCP nem retomar Android nesta frente. | Android está adiado; testes/instalações exigem escopo específico. |
| COM — Motor de combate | 85% | Manutenção | Reutilizar o motor; adicionar adaptadores ou casos apenas quando a entrega exigir. | Nenhuma nova pesquisa para a base entregue. |
| TIME — Tempo, AP, recargas e extras | 90% | Manutenção | Conectar indicadores/pausa ao relógio existente sem criar relógios concorrentes. | Nova mecânica temporal exige contrato; atuais não precisam ser pesquisadas novamente. |
| RES — Resolução causal e R04 | 90% | Manutenção | Consumir a fila de eventos na apresentação, mantendo o core como autoridade. | Não depende de novas entregas para as regras vigentes. |
| FUN — Fundação e organização inicial | 100% | Manutenção | Manter arquitetura, decisões e instruções atualizadas conforme entregas reais. | Somente mudanças de direção que sejam efetivamente aprovadas. |

AUT significa tarefas autônomas de preparação/integração que atravessam várias famílias; AUT-01–16 foram entregues em recortes próprios, com aceites residuais específicos, não uma nova fila para repetir. PESQ significa pesquisa, sem percentual fixo. PvE: jogador contra ambiente; AP: pontos/barra de ação; HUD: informações exibidas durante a batalha; MCP: ponte de ferramentas opcional, vinculada a ENG/AMB, não família de gameplay. SIS/REG/HAB não estão por iniciar; catálogo completo e extensões continuam abertos.

P1 libera a demo. P2 amplia ou prepara frentes paralelas. P3 é expansão posterior. Manutenção preserva o escopo encerrado e só repete testes quando uma alteração concreta justifica. FUN100% refere-se à fundação inicial, não ao projeto inteiro.

Ordem imediata: adaptadores/HUD PvE → Ritual → ondas/fases → ScoreAttack/Survival → sequenciais → frentes paralelas. Feedback e documentação acompanham cada entrega. Clássico, Android e instalação de MCP não entram nessa sequência.

[Progresso](PROGRESSO-GERAL.md) · [Fila detalhada](FILA-DESENVOLVIMENTO-INDEPENDENTE.md) · [Preparação do usuário](../../entregas/NECESSIDADES-ATUAIS.md).