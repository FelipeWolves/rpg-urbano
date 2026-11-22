> **Mapa vigente das famílias — 22/11:** [percentuais, tarefas independentes, ajustes, dependências e prioridades](MAPA-FAMILIAS-E-PRIORIDADES.md). Organização somente; demo geral mantida em ~25%. Próximo recomendado: cliques/alvos e primeira reprodução. Bases encerradas não são reabertas por rotina.

> **Vigente22/11 — ESC/SIS/PvE concluído no recorte de código:** [entregas e limites](ESC-SIS-PVE-CONSOLIDACAO.md). Doze baterias, regressão geral, compilaçãoUE e stage66 aprovados, sem executar o jogo. Cliques/alvos e vídeo continuam **REABERTOS**. Menus/HUD novos, conteúdo final e plataformas são próximos trabalhos separados.

> **Vigente — 22/11, ponto 1 entregue:** [Felipe e efeitos contínuos](HAB-FELIPE-PILOTO.md): Bleed/Poison, A1/A2/A3 e piloto local implementados e testados. Aceites visuais/mobile separados. SIS-02-B/C, REG e A2 não estão mais por iniciar. Próximo grupo possível: ESC/SIS/PvE, **fora da autorização desta entrega**. Notas anteriores são histórico.

> **Atualização 22/11 — REG-03:** base de silêncio, incapacidade, imunidades, teste de debuff e revalidação de reações entregue; oito grupos focados, regressão geral e build Unreal aprovados, sem executar editor/jogo. Ver [REG-03](REG-03-CONTROLES.md). **Ainda pendentes:** executor Bleed/Poison, conexão/testes da A2 e piloto HAB; não confundir contratos recebidos com implementação. Próxima integração segue por esses componentes. Stage anterior não representa REG atual.
> **SIS-02-A concluído — 22/11:** [mapa numérico e compatibilidade](SIS-02-A-MAPA-NUMERICO.md): 22 âncoras conferidas, cortes de migração e testes futuros definidos. Código/fórmulas/saves intactos; nenhuma bateria ou engine executada. Próxima entrega: **SIS-02-B, aritmética segura**, ainda não iniciada. Save v1 não contém HP; migração de HP não força nova versão por si só. Esta atualização prevalece sobre a indicação histórica de A ainda por iniciar.

# Ordem de execução após AUT-01–16

22/11/2026. **Plano reconciliado, ainda não executado.** Esta ordem substitui sugestões históricas de começar novamente pelos AUT. Somente documentação nesta rodada: sem implementação, testes, compilação, stage novo, telas, Android ou internet.

Os sufixos A/B/C abaixo são **subentregas propostas**, não novos blocos já implementados. Esforço pequeno/médio/alto é comparação de escopo, não prazo ou previsão do limite do Codex. Dentro da criticidade, priorizar a menor entrega que libera a seguinte. Uma entrega funcional e um commit por vez; não abrir frentes concorrentes na mesma árvore.

## Reconciliação — o que preservar

| Família | Estado que orienta a retomada |
|---|---|
| TIME-01–03, RES-01/R04, SEQ-01, PAR-01 e perfil PAR-02 | Encerrados no escopo dos relatórios. Sem revisão geral ou repetição de bateria por rotina; regressão pertinente quando uma mudança atingir esses contratos. PAR é intercalação local, não threads/servidor |
| ESC-01–07 / StandardShield | Contrato e caso comum implementados. Preservar mínimo 1 para aplicação válida, fórmula/relógio/reaplicação/absorção e remoção aprovados; não reabrir decisões como pesquisa |
| HAB-01 e CIC-01 | Bases mínimas encerradas. Não equivalem a catálogo completo de habilidades, economia de produto ou save de combate |
| VIS/HUB/AUD/CIC através dos AUT | Há código/testes/compilações; aceites de UI, disco e plataforma continuam próprios. Não refazer esses adaptadores por outra sigla |
| AUT-12/13/14 | Marcador runtime, canal de cues e esquema limitado de referência entregues. Material/animadores/migração completa de dados continuam pendentes |
| AUT-15/16 | Stage verificável e QA por área entregues; pacote novo/Shipping/mobile/smoke não aprovados por consequência |
| AUT-11 | Reprodução relatada; primeiro clique **aparentemente correto / REVER**. Segunda conferência, áudio/ciclo/flag, legendas, mixer, arte lateral e autoabertura permanecem separados |
| SIS/REG e Felipe A2 | Decisões posteriores recebidas, implementação ampliada ainda pendente. O complemento final de ponto fixo prevalece sobre a proposta antiga de double |

Fontes: [consolidação AUT](CONSOLIDACAO-AUT-12-16.md), [painel](../batalhas/PAINEL-BLOCOS.md), [pendências](PENDENCIAS.md), [complemento final](COMPLEMENTO-FINAL-A2-ACC-PONTO-FIXO.md), [adendo técnico](ADENDO-TECNICO-CONTRATOS-2026-11-20.md). Histórico não substitui a decisão consolidada mais recente.

## Fila A — crítica: ordem principal de código

| Ordem / subentrega proposta | Escopo e motivo | Dependência | Esforço | Saída verificável |
|---|---|---|---|---|
| **1 — SIS-02-A: mapa numérico e compatibilidade** | Inventariar HP, escudo, dano, percentuais, acumuladores, conversões, tipos gráficos e codecs; delimitar unidades/versões antes de migrar | Contratos já recebidos | Pequeno | Matriz de pontos reais no código, migração por partes, testes de fronteira selecionados e impacto em save/adapter. Ainda sem reescalar personagens ou trocar fórmulas |
| **2 — SIS-02-B: aritmética segura** | Utilitários int64, teto 10^12, basis points 10000, floor e comparação exata de HP% sem overflow intermediário | 1 | Médio | Casos abaixo/no/acima do teto, produto antes de divisão, razões quase iguais e valores inválidos. Sem plugins ou double autoritativo |
| **3a — SIS-02-C: tipos no executor** | Migrar quantidades por cortes pequenos; preservar cenários de laboratório e contratos ESC/R04 | 2 | Alto, dividir | Paridade dos cenários pequenos e limites numéricos; regressões somente das áreas afetadas |
| **3b — SIS-02-C: fronteiras e versões** | Ajustar apresentação, codecs/save/manifestos e stage conforme a migração; conversões gráficas não podem alterar cálculo de combate | 3a | Médio | Armazenamento amplo sem truncamento, política explícita de versão/compatibilidade e mensagens. Não migrar saves antigos silenciosamente |
| **4 — REG-01/SIS-02: resultados por hit e RNG** | Separar hit/Miss/Block/invencibilidade/escudo/HP; preparar sorteio determinístico versionado e resultado observável para passivas/replay | 3a/3b | Médio | Identidade por ação/hit, isolamento dos sorteios e outcomes testáveis. Não declarar determinismo ARM/iOS sem executar nesses ambientes |
| **5 — REG-01/SIS-02: funil de dano e debuff** | Implementar por camadas os contratos recebidos: bloqueio automático, crítico, mitigação, True Damage/DoT e trilho ACC/RESIST | 4; fórmula específica só onde fechada | Alto, dividir por camada | Ordem, pisos, bypass, escudos e chance testados por hit; Defesa clássica preservada. Fórmula DEF/curas ainda não fechada não será inventada para preencher o funil |
| **6 — REG-03: incapacidade/silêncio/supressão** | Implementar cancelamentos e elegibilidade de ações/passivas conforme adendo, usando outcomes claros | 4/5 | Médio/alto | Casos de controle, básica, passivas e reações R04. Não reutilizar silenciosamente o legado EffectKind::Block (impedimento de ação) como o novo Block probabilístico |
| **7 — HAB / Felipe A2: Postura de Vigília** | Entregar a primeira passiva representativa com contagem por aliado/habilidade, quota, desempate e StandardShield | 2, 4–6 | Médio/alto | AoE/multi-hit/letal/absorção/Block/Miss/invencibilidade; um disparo após habilidade, empates exatos e janela R04. Não pedir novamente destino do escudo ou regras de cargas já respondidos |
| **8 — HAB / piloto e CIC: integração do personagem** | Integrar as habilidades já especificadas do recorte Felipe e o encontro de demonstração, refletindo resultados na UI/save existentes | 7; detalhes da ficha no contrato próprio | Médio | Um personagem/encontro ponta a ponta no escopo local; não cadastrar todo o elenco ou economia. Lacuna real de conteúdo vira pendência localizada |

**Próxima tarefa concreta: SIS-02-B (ordem 2).** SIS-02-A foi concluído em revisão estática e documentação; a implementação de utilitários e a migração continuam não iniciadas. Ver SIS-02-A-MAPA-NUMERICO.md.

Block: 3% base universal e 10% total no Michel, tetos/exceções conforme contrato; não confundir com Defesa. ACC/RESIST e irresistível seguem o complemento final. Os números recebidos não serão reaprovados por perguntas genéricas; dúvidas técnicas novas serão delimitadas no ponto de implementação.

## Fila B — importantes, menores e independentes do núcleo numérico

Executar uma destas somente se a fila A estiver bloqueada por informação real, se houver janela curta ou se o usuário priorizar a experiência visual. Não exige aguardar todas as habilidades; também não autoriza controles de tela.

| Ordem nesta trilha | Família / entrega | Pré-requisito e limite |
|---|---|---|
| B1 | VIS-01–03 / consolidar aceites existentes | Segundo olhar do usuário no clique, novo marcador, equipe/alvos, painéis e resoluções. Não refazer o escudo comum nem usar aceite antigo do marcador debug para aprovar a nova malha |
| B2 | AUD/HUB / volumes de mídia e preferências | Integrar canal Mídia/mestre e recuperação de preferências com componentes já instalados; teste de áudio real separado. Não instalar sistema de áudio antecipadamente |
| B3 | VIS/AUD / legendas da mídia | Preparar parser/estados de legenda e UI acessível; conteúdo/tradução e sincronismo real só com arquivo/ensaio apropriado. Não inventar falas para os vídeos recebidos |
| B4 | VIS/HUB / composição lateral e acessibilidade | Usar arte autoral existente, ajustar layout/configuração sem distorcer vídeo; texto ampliado/contraste e janelas estreitas exigem aceite visual |
| B5 | HAB/VIS / consumidor do canal de cues | Primeiro consumidor provisório e cancelamento de apresentação; rig/modelos/VFX finais são dependências artísticas. Não recolocar dano em callbacks de animação |
| B6 | HUB/AUD / primeira abertura automática | Só ativar após aceite de reprodução e flag/ciclo; abrir .uproject não é iniciar o jogo. Manter falha recuperável e rever manual |

A ordem B2–B4 privilegia entregas menores usando estruturas existentes. B5 pode caminhar antes delas se um recurso artístico pronto justificar a prioridade; mudança deve ser registrada, não feita implicitamente.

## Fila C — evoluções após as bases críticas

| Ordem | Família | Dependências e limite |
|---|---|---|
| C1 | ESC especializado / SilverShield, cura e geradores específicos | Tipos/funil, fontes e contratos de cada efeito; preservar StandardShield. Silver coexistente/consumo após Standard conforme adendo. Cura/HP máximo mutável não aparecem como implementados por esta fila |
| C2 | SIS-03 / WAVE: política nova de passagem | Camadas de efeitos e cura: limpar comuns/Standard, preservar categorias aprovadas e então curar 30% de aliados vivos. Política versionada própria; não substituir persistências SEQ/PAR encerradas |
| C3 | SIS-03 / score e modos PvE por dano/sobrevivência | Numérico e eventos de dano/resultado; agregação/overkill/floor final/teto/AttemptID conforme contrato. Não somar score a todo modo por padrão |
| C4 | PVE / SEQ-PAR: fases autônomas, área e alcance entre frentes | Contrato por modo, seletores, limites e transições; não reabrir testes dos perfis concluídos sem alteração pertinente. Não inferir arquitetura de servidor a partir de testes de Dislyte |
| C5 | CIC/SIS-04 / inventário e progressão ampliada | Contratos de produto e schema/migração próprios; save v1 não vira save de combate ou conta sincronizada por inferência |
| C6 | Clássico expandido, ATB/SP e DJ/EXP | Conteúdo e contratos específicos antes da implementação; são produtos de regras distintos, não opções de câmera |

## Fila D — condicionada a ambiente, recursos ou autorização

- **ENG / pacote:** stage já verificado; build/cook/Shipping e execução em Windows limpo apenas em etapa autorizada. Atualizar stage depois de mudanças nas fontes; não repetir hashes por rotina documental.
- **Mobile:** manter SDK/NDK/JDK e diagnóstico já registrados. Backend Android/iOS, codecs/perfis, áudio, foco, memória, orientação e aparelhos exigem validação por plataforma. WMF é Win64. Não instalar, testar Android ou alterar rede nesta rodada.
- **ART/AUD/conteúdo:** um modelo/rig e pequena cena representativos, áudio próprio, legendas e conteúdo piloto podem ser preparados em paralelo pelo usuário, sem exigir catálogo final. Confirmar arquivos recebidos antes de pedir novamente.
- **Serviços/lojas/PvP online:** decisão de escopo, ferramentas e autorização próprias; Steam não é decoder de vídeo. Sem contas, servidores, publicação ou custos iniciados implicitamente.
- **Internet:** se surgir necessidade de fonte externa, registrar pergunta, documento oficial/versão a consultar e impacto; o usuário verificará depois. Não acessar internet para preencher a fila.

## Como executar a partir deste plano

1. Selecionar a primeira subentrega pronta da fila A; conferir Git e apenas os arquivos pertinentes.
2. Confirmar dependências no código, preservando trabalho do usuário. Se surgir uma dúvida impeditiva, registrar pergunta localizada e continuar somente trabalho independente autorizado.
3. Implementar uma unidade verificável; usar o seletor AUT-16 e regressões compatíveis com o impacto. Não repetir baterias por editar documentação.
4. Atualizar tarefa/estado/pendências e commitar; separar código, teste nativo, compilação, observação e aceite do usuário.
5. Revisitar a ordem somente por dependência descoberta, defeito real ou preferência do usuário. Não trocar a fila crítica inteira por uma melhoria cosmética mais rápida.

Nenhum percentual foi recalculado: esta rodada organiza trabalho, não implementa funcionalidades. Os aceites antigos continuam válidos nos seus limites; contratos numericamente alterados exigirão regressão dirigida, sem reabrir decisões de produto já recebidas.
