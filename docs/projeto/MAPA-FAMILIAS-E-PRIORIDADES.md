# Mapa vigente das famílias e prioridades

Atualizado em22/11/2026. Organização documental: sem implementação, testes, compilação, telas, Android ou internet nesta rodada.

## Como interpretar as porcentagens

A demo integrada permanece em aproximadamente25% (23–27%), conforme PROGRESSO-GERAL.md. Os percentuais por família são estimativas de maturidade até uma primeira experiência integrada, não contagem medida de tarefas/horas. Interface22%, hub20%, integraçãoUnreal45%, ciclo/save25% e áudio0% reutilizam o painel existente. Os demais são estimativas novas de planejamento; sua origem está explícita no JSON. Não representam novo avanço de código.

Famílias se sobrepõem: VIS/ENG/AUT podem participar da mesma entrega. Não somar, tirar média nem substituir a ponderação oficial das áreas por esta lista. Os 85–90% deCOM/TIME/RES não reabrem os blocos mínimos encerrados; a parcela restante representa integração/validação e extensões delimitadas da família. FUN100% significa fundação inicial entregue, não toda a documentação nem todo o projeto.

P0=problema que impede uso/confiabilidade. P1=desbloqueia a demo. P2=expansão ou frente paralela. P3=ampliação posterior. Manutenção=preservar, sem revisão/bateria rotineira. Uma mudança concreta pode exigir regressão pertinente.

## Quadro por família

| Família | Estimativa | Prioridade | Pode avançar agora sem nova entrega | Ajustes/revisões pendentes | Por que priorizar assim |
|---|---:|---|---|---|---|
| VIS — Visual e interação | 22% | P0 | Diagnosticar eventos de mouse/toque e corrigir seleção, troca de alvo e confirmação; preparar cenários automatizados de entrada. | Duplo clique inesperado, alvo que não muda, resoluções, leitura e acessibilidade; aceite visual depois da correção. | É o bloqueio relatado no uso diário e afeta todas as próximas telas. |
| ENG — Integração Unreal | 45% | P0/P1 | Investigar ciclo/foco/callbacks do player por código; conectar os controladores PvE ao adaptador sem duplicar regras. | Primeira reprodução com clique adicional; integração real dos modos, pacote limpo, localização e plataformas. | Faz a ponte entre o núcleo testado e a experiência utilizável; integrações frágeis propagam problemas. |
| HUB — Central de navegação | 20% | P1 | Organizar rotas e estados dos acessos existentes e destinos para os modos; reutilizar painéis e preferências. | Feedback de acesso, retorno entre telas e destinos ainda bloqueados; não anunciar sistemas sem implementação. | Permite chegar às funções e evita implementar modos que o jogador não consegue acessar. |
| PvE — Encontros e chefes | 65% | P1 | Integrar um Ritual de laboratório ao fluxo atual, depois ScoreAttack e Survival; usar dados provisórios identificados. | Menus/HUD, objetivo/resultado, indicadores de fase, kits reais e balanceamento; classes compiladas não são telas prontas. | Grande retorno por reaproveitar os controladores recém-testados; começar por um encontro simples reduz esforço. |
| WAVE — Ondas | 75% | P1 | Conectar contador de ondas e resumo de limpeza/cura ao encontro; usar WaveV2 já testado. | Mostrar transições, vida, efeitos e recargas corretamente; testar o fluxo completo sem trocar as políticas antigas. | Entrega menor que um novo sistema de combate e reutiliza a política pronta de até cinco ondas. |
| CIC — Ciclo de jogo e persistência | 25% | P1 | Definir identidade de encontro/modo nos dados e preparar persistência versionada de resultados/recibos; manter idempotência. | Save atual é entre tentativas; score em memória não é persistência em disco; validar salvar/carregar pela interface. | Evita perda ou duplicação de progresso quando os novos modos entrarem na interface. |
| HAB — Habilidades | 65% | P1/P2 | Organizar catálogo/dados do kit recebido e descrições; facilitar novos kits sobre os executores existentes. | Exibir duração, recarga, cargas e indisponibilidade; integrar os efeitos visuais sem alterar a autoridade do core. | Transforma regras prontas em conteúdo; expandir com catálogo consistente reduz retrabalho por personagem. |
| SIS — Sistemas gerais | 55% | P1/P2 | Formalizar dados e validação de atributos/resultados e interfaces de inventário/equipamento; separar sistemas por responsabilidade. | Não confundir contratos com implementação; fórmula universal de cura, MaxHP mutável e novos gatilhos exigem definição própria. | Sustenta progressão e conteúdo; fazer apenas o necessário à próxima entrega evita expansão prematura. |
| ESC — Escudos | 80% | P2 / integração | Reutilizar Standard/Silver e ligar suas camadas/descrições ao HUD e à inspeção. | Conferir leitura visual, coexistência e transições; não reimplementar fórmulas aprovadas. | Base madura; o retorno atual está na clareza e integração, não em refazer o executor. |
| REG — Regras e contratos de combate | 80% | P2 / sob demanda | Garantir motivos de recusa e resultados observáveis; validar apenas novas regras que entrem no recorte. | Exceções futuras, equipamentos e combinações não contratadas; prova entre plataformas continua separada. | Preserva consistência sem repetir investigação de dano, Block e ACC/RESIST já implementados. |
| SEQ — Equipes sequenciais | 75% | P2 / após Ritual e ondas | Preparar apresentação da equipe ativa, troca e resultado, usando o coordenador e a política v2. | Mostrar persistência/recargas/kit estável ou variável e evitar sugerir retorno cíclico. | Código pronto no recorte; a apresentação é mais complexa e rende mais depois do fluxo simples. |
| PAR — Frentes paralelas locais | 70% | P2 / depois de SEQ | Preparar seleção/indicadores de frente, HP global e sinalização de área local/global. | Apresentar três equipes e auxiliares sem confundir alcance; não representar intercalação como servidor/threads. | Tem maior custo visual e de entendimento; começar por ele ampliaria os problemas de seleção existentes. |
| ART — Arte e animação | 10% | P2 / paralelo | Preparar convenções de nomes, escala, pivô, importação e pontos de conexão; usar placeholders existentes. | Integração de modelos/rig/clips, enquadramento, orçamento visual e VFX quando os recursos chegarem. | Pode avançar em paralelo ao core; um personagem e uma arena evitam produzir um catálogo antes de validar o fluxo. |
| AUD — Áudio, música e vozes | 0% | P2 / paralelo | Estruturar canais/volumes e eventos de áudio; preparar preferências, mute e movimento/ciclo de vida sem áudio final. | Mixer, volumes reais, recuperação de preferências e transições; vídeo não equivale a sistema musical implementado. | Infraestrutura independente, mas sem urgência maior que entrada e navegação. |
| EXP — Expansão: eventos, campanha e DJ | 5% | P3 | Organizar contratos de evento e rascunhos de fluxos; preservar o perfil clássico existente. | Clássico completo com itens/SP/Limit/ATB Active e minijogo DJ ainda não estão implementados. | Amplia muito o produto; priorizar agora atrasaria uma demo principal estável. |
| AMB — Ambiente e ferramentas | 70% | P2 / quando retomar mobile | Manter instruções e verificadores reproduzíveis das ferramentas instaladas; diagnosticar por leitura quando necessário. | Compatibilidade Android/pacote/aparelho e requisitos de distribuição; não instalar nem atualizar versões por antecipação. | Windows já permite desenvolver; preparar tudo antecipadamente consome recursos sem liberar o gargalo atual. |
| COM — Motor de combate | 85% | Manutenção | Reutilizar o motor; adicionar adaptadores ou casos apenas quando a entrega exigir. | Aceites integrados e extensões concretas; sem refazer a bateria inteira por organização documental. | Base funcional testada; preservá-la economiza esforço para a apresentação. |
| TIME — Tempo, AP, recargas e extras | 90% | Manutenção | Conectar indicadores/pausa ao relógio existente sem criar relógios concorrentes. | Somente integração e mudanças concretas; ATB Active/Wait completo é extensão do clássico, não defeito da base. | Reabrir a base já validada não resolve os cliques nem os novos menus. |
| RES — Resolução causal e R04 | 90% | Manutenção | Consumir a fila de eventos na apresentação, mantendo o core como autoridade. | Testar novas consequências somente quando adicionadas; não antecipar vitória na animação. | Protege morte, revive e reações; mudança rotineira aqui aumentaria risco sem ganho imediato. |
| FUN — Fundação e organização inicial | 100% | Manutenção | Manter arquitetura, decisões e instruções atualizadas conforme entregas reais. | Nenhuma reconstrução da fundação; documentação viva tem medição própria. | Escopo inicial encerrado; criar trabalho novo aqui não aumenta a demo. |

## Dependências e limites

| Família | Dependência |
|---|---|
| VIS | Reteste do usuário; execução gráfica somente com liberação específica. |
| ENG | Aceite de reprodução pelo usuário; Android adiado; sem abrir editor nesta organização. |
| HUB | Primeiro estabilizar VIS/player; arte definitiva só para acabamento. |
| PvE | VIS estável para aceite; dados finais podem chegar depois. |
| WAVE | Um encontro integrado; conteúdo definitivo não impede a primeira versão. |
| CIC | Definições econômicas para recompensas finais; armazenamento local pode avançar sem lojas/servidor. |
| HAB | Fichas novas somente para habilidades não definidas; não exigir o elenco inteiro. |
| SIS | Pode estruturar contratos técnicos; regras novas de produto precisam de decisão antes da execução funcional. |
| ESC | Novos tipos de escudo dependem de ficha; os dois atuais não dependem de pesquisa nova. |
| REG | Não depende de pesquisa para os contratos já fechados; regra inédita exige definição. |
| SEQ | Interface de um encontro e seus resultados integrada primeiro. |
| PAR | Experiência de uma frente e seleção de alvos estáveis. |
| ART | Recursos autorais/licenciados para acabamento; planejamento e placeholders não dependem deles. |
| AUD | Sons/músicas/vozes próprios para integração final e teste auditivo. |
| EXP | Recorte e regras de cada experiência; dados narrativos recebidos devem ser reutilizados. |
| AMB | Android está adiado; testes/instalações exigem escopo específico. |

Código pode ser analisado e estruturado sem novas imagens, sem abrir telas e sem retomarAndroid. Aceite gráfico, reprodução/áudio reais e toque/aparelho dependem de execução apropriada e liberação correspondente. Esta organização não inicia implementação, instalações ou plataformas.

## Ordem recomendada de entregas

1. **P0 / VIS — entrada e alvo:** diagnosticar mouse/toque; clique válido seleciona, alvo muda antes da confirmação e um gesto não gera dois comandos. Preservar revisão/idempotência e geometria existente.
2. **P0 / ENG–HUB–mídia — primeira reprodução:** conferir foco, ticket/callback, preparação e fechamento. Primeiro Rever deve funcionar com um clique, sem reprodução por trás ou flag de assistido falsa.
3. **Aceite curto do usuário:** repetir RET-INPUT-01 e RET-MEDIA-01 após as correções. Não marcar resolvido por compilar.
4. **P1 / PvE–VIS–ENG–HUB — um Ritual:** integrar objetivo, chefe, alvos e resultado ao fluxo atual. É o menor novo modo verificável.
5. **P1 / WAVE — ondas:** integrar indicadores e transição, reaproveitando limpeza/cura/persistência testadas.
6. **P1 / score–CIC:** apresentar ScoreAttack/Survival e preparar persistência por modo sem duplicar recompensas. Economia final é configuração própria.
7. **HAB–SIS–ESC–REG:** catálogo recebido, descrições e contratos técnicos necessários à próxima entrega. Não cadastrar elenco inteiro ou implementar atributos indefinidos.
8. **P2 / SEQ, depoisPAR:** visualizar primeiro trocas sequenciais; depois três frentes, alcance eHPglobal. A interface mais complexa não deve vir antes da seleção estável.
9. **Paralelo / ART–AUD–conteúdo:** uma amostra de personagem/arena/áudio, rig, descrições e roteiro. Preparação pode usar placeholders.
10. **P2 condicionado / AMB–ENG mobile:** quando priorizado, ambiente, pacote, toque/orientação, vídeo e desempenho no aparelho.
11. **P3 / EXP:** clássico ampliado, campanha eDJ em recortes próprios após estabilizar a experiência principal.

Dentro de uma prioridade, escolher o menor corte funcional que libera o seguinte. Dependências podem alterar a ordem; não há autorização implícita para iniciar tudo. Esta sequência substitui sugestões históricas de reiniciar AUT ou SIS-02-B.

## Frentes transversais e siglas auxiliares

| Frente | Estado | Próximo trabalho |
|---|---|---|
| QA integrado | 30% no painel de áreas | Regressão pertinente e aceite curto a cada entrega; depois aparelhos/desempenho |
| Documentação | 80% no painel de áreas | Reconciliar estados e reduzir instruções históricas conflitantes |
| Conteúdo/tutorial | 10% no painel de áreas | Integrar roteiro guiado recebido; pré-carga4/5 não é tutorial completo |
| Vídeo/VFX/cinematics | 5% no painel de áreas | Reprodução éP0; animações/cinematics finais vêm depois. A porcentagem cobre a frente completa, não só o player |
| Mobile/otimização | 5% no painel de áreas | Adiado; boot doAVD não equivale a APK/desempenho aprovado |
| AUT | Fila transversal, sem porcentagem própria somável | AUT-01–16 têm entregas por escopo; AUT-11 e aceites seguem parciais/reabertos. Não reiniciar a lista |
| PESQ | Pesquisa, sem denominador fixo | Dúvida real necessária a uma regra nova; não repetir contratos aprovados |
| PEN | ID de pendência, não família | Registro central emPENDENCIAS.md |
| R01–R04 | Revisões/contratos, não famílias | RES continua sendo a família de resolução |

Siglas internas: FUNFundação; AMBAmbiente; COMCombate; TIMEtempo; RESResolução; WAVEondas; SEQsequência; PARfrentes paralelas locais; HABHabilidades; CICCiclo; ENGEngine; VISVisual/interação; HUBcentral de navegação; ARTArte; AUDÁudio; EXPExpansão; ESCescudos; SISsistemas; REGregras; PvEplayer versus environment. Não são nomes de ferramentas.

## Fontes e preservação

PROGRESSO-GERAL.md; TAREFAS.md; ../batalhas/PAINEL-BLOCOS.md; ESC-SIS-PVE-CONSOLIDACAO.md; HAB-FELIPE-PILOTO.md; RETESTES-ENTRADA-E-VIDEO.md. Cliques/alvos/vídeo estão REABERTOS; o aceite antigo do alinhamento do ponteiro não os encerra.

Registros anteriores preservam o histórico dos escopos entregues. Nenhum arquivo recebido foi reescrito. Estimativas e justificativas estruturadas: ../relatorios/atuais/MAPA-FAMILIAS.json.
