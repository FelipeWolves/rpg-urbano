# Catálogo de exemplos e famílias PvE

18/11/2026. Organização documental, sem alteração de mecânicas ou nova execução de testes. Cobertura atual: **148 testes aprovados** após PAR-02 parte 3; o catálogo continua evidência dos contratos próprios, não paridade externa. Este catálogo não é uma lista completa dos chefes/modos atuais do Dislyte.

Objetivo: preservar todos os exemplos e pistas já registrados, separar suas regras e evitar que implementar uma família seja interpretado como cobertura de todas as outras. Referência estruturada: modos-pve.csv e modos-pve.json. Foram preservadas as versões anteriores de cenarios.json e mecanicas.csv em historico-catalogos-2026-11-18/; IDs antigos e seus exemplos continuam existentes.

## Eixos independentes de classificação

| Eixo | O que registrar |
|---|---|
| Formação | Quantos times, integrantes por time, repetição de personagem, reservas e invocação |
| Participação dos times | Um time; times sucessivos uma vez; retorno cíclico; participação conjunta |
| Inimigos | Ondas independentes; chefe persistente; HP único; partes; avatares separados |
| Fase do chefe | Gatilho por HP, turno, entrada de time, evento ou outro; mudanças de kit/estado |
| Relógio e limites | Ações aceitas, turnos do portador, turnos do chefe, contador do modo ou tempo real; local/global |
| Troca e persistência | Quem troca, quando; HP, status, recargas, cargas e AP conservados ou reiniciados |
| Resultado | Eliminar, sobreviver, cumprir objetivo, alcançar classificação ou encerrar por limite |
| Pontuação e recompensas | Dano efetivo, dano declarado, overkill, multiplicadores, faixas e entrega idempotente |
| Evidência | Relato, fonte histórica, comportamento observado, decisão autoral ou teste executado; versão/data |

Esses eixos podem se combinar. Ter três times não determina se há simultaneidade, rodízio, HP compartilhado ou pontuação. Trocar um time não significa mudar uma fase do chefe. Um contador global de comandos não é automaticamente o contador de turno de um modo externo.

## Exemplos preservados e cobertura real

| ID | Exemplo/família | Cobertura própria e lacuna |
|---|---|---|
| PVE-01 | Formações 1–5 e sondagens com seis | COM-01A testado; tamanho é eixo configurável, não modo completo |
| PVE-02 | Mesmo time em até cinco confrontos | WAVE-01 testado; persistência particular de cada modo ainda precisa de descrição |
| PVE-03 | Três times sucessivos, cada um entra uma vez | SEQ-01 testado; sem retorno repetido, score ou todas as fases por HP/turno |
| PVE-04 | Season 0: três frentes contra três chefes | PAR-01 testado no perfil independente cooperativo, não a arquitetura original |
| PVE-05 | Season 0: três frentes contra o mesmo chefe | PAR-02 perfil encerrado: 17 grupos, HP único, skills com escopo/R04 e fronteiras; escolhas próprias |
| PVE-06 | Treva Fluxo, nome informado pelo usuário | Relato de até três equipes, limite/sobrevivência e dano influindo nos ganhos; modo completo não implementado |
| PVE-07 | Chefes dos Milagres, exemplo específico a identificar | Relato de fases e até três grupos com até cinco integrantes; SEQ-01 cobre parte da estrutura, não certifica esse modo |
| PVE-08 | Rodízio com retorno de um time | Extensão possível, não existência confirmada; não inferir de relato em que cada time entra uma vez |
| PVE-09 | Fases por HP, turno ou gatilho próprio | Família separada da troca de times; kit por entrada existe, gatilhos autônomos ainda não |
| PVE-10 | Partes ou avatares com placar | Alternativas técnicas não escolhidas para o primeiro PAR-02; não afirmar como regra original |
| PVE-11 | Withering Flame/Chimera e Lion/Goat/Viper | Nomes e pistas históricas preservados em ANALISE.md/fontes.json, sem inferir entidades/HP por aparência |
| PVE-12 | Shademother e hipóteses de desempenho | Pista histórica de outro modo; não prova causa de mudanças em Season 0 nem capacidade do servidor |

Os 8 cenários anteriores e as 26 famílias do checklist de mecânicas foram mantidos. O lote funcional LOAD-01 continua em cenarios.json como diagnóstico, não foi transformado em um modo PvE. Catálogo e checklist não são configurações carregadas pelo motor.

## Distinção importante: sobreviver, vencer, pontuar e receber recompensa

São resultados diferentes. Um chefe pode sobreviver à janela enquanto o jogador obtém classificação/recompensa por dano. Também pode existir derrota por eliminação do time com pontuação preservada, dependendo do modo. Essas possibilidades não foram decididas para nosso projeto.

No exemplo Treva Fluxo, registrar separadamente o encerramento da tentativa, sobrevivência, cálculo de dano/score, faixa atingida e recompensas. Ainda não sabemos a fórmula ou a unidade do contador. Não implementar um enorme HP fictício, um timeout como derrota automática ou uma soma de danos como se isso já reproduzisse o modo.

Limites locais/globais e eventos de dano existentes são peças úteis, mas não um sistema completo de pontuação. Não reutilizar sem decisão os contratos de vitória por eliminação de SEQ-01/PAR-02 para esse exemplo.

## Alcance da escolha já feita no PAR-02

HP único, uma barra global e ataques comuns alternados entre times vivos continuam aprovados **para o perfil experimental do PAR-02**. Não são regra universal dos chefes atuais ou dos modos por pontuação. O chefe ignora times **sem sobreviventes**; não ignora os times vivos. Não revogar a escolha, nem aplicá-la automaticamente ao Treva Fluxo ou ao exemplo dos Milagres.

PAR-02 partes 1/2: único estado/AP, inscrição, skills de alvo único com escopos e R04. Parte 3 validou as fronteiras integradas, encerrando apenas esse perfil. Skills comuns de área e dinâmica do chefe não estão incluídas. SEQ-01 permanece o coordenador sucessivo; PAR-01 o independente. Nenhuma mudança desses contratos nesta reorganização.

## Pesquisa complementar

Um relatório do usuário será útil para ampliar e identificar os exemplos, sobretudo Treva Fluxo e o chefe dos Milagres. Modelo em ROTEIRO-PVE-CHEFES.md. Para uma relação completa e atual, registrar versão/data e uma lista de modos para conferir cobertura. Ausência no relatório significa desconhecido, não inexistente.

Não houve nova pesquisa online nesta entrega: referências históricas não foram reapresentadas como fatos atuais. Dados futuros entram com procedência e divergências; não substituem decisões autorais automaticamente. Não solicitar dados de contas, credenciais, saves ou conteúdo protegido.

## Próximo passo

Próximo bloco: COM-02, interação textual; PAR-02 encerrado no perfil de alvo único, sem implicar fases autônomas. Pontuação/sobrevivência e fases autônomas permanecem requisitos separados a especificar; não entram silenciosamente na implementação de habilidades. Blocos concluídos não precisam de revisão sem mudança pertinente.
