# TIME-03 — Revisão incremental do tempo

Estado em 18/11/2026: **bloco concluído, três partes implementadas e validadas**. Total final: 67 testes aprovados. As seções anteriores registram o histórico incremental; a parte 3 abaixo encerra o bloco.

## Parte 1 — Recarga no encerramento do uso

O turno elegível de uso agora faz parte do ciclo de recarga. A habilidade recebe N ao ser usada; ao encerrar a oportunidade elegível do dono, reduz para max(0, N − 1), junto das outras recargas pertinentes. Comandos recusados não atribuem/reduzem recarga, não avançam turno e não gastam limite.

| Recarga base | Após o uso elegível | Reutilização sem modificadores |
|---|---|---|
| 0 | 0 | Na próxima oportunidade em que puder escolher uma habilidade |
| 1 | 0 | Na próxima oportunidade em que puder escolher uma habilidade |
| 3 | 2 | Oportunidade 2 começa com 2; oportunidade 3 começa com 1; oportunidade 4 começa com 0 e permite uso |

Redução ocorre no fim; escolher outra habilidade no turno 3 não pode reutilizar uma recarga que só chega a zero no encerramento desse turno. Turnos de outros personagens e reações fora do turno do dono não reduzem seu contador por passagem de tempo.

Alteração mínima em Battle::end_turn: removida a exceção que ignorava a habilidade recém-usada. O parâmetro usado apenas por essa exceção também foi removido. Os testes anteriores com habilidade foram ajustados ao novo contrato, inclusive saldo conservado entre confrontos.

### Fronteira desta entrega

O significado de turno elegível continua o existente no TIME-02: NaturalOnly ou EveryTurn por habilidade. As flags independentes por origem de extra ainda serão implementadas na parte 2. Não atribuir ao estado intermediário suporte completo a Astralis, petrificação ou passivas de início de turno.

Nenhuma mudança de duração de status, AP, guarda, fila de reação, vitória, fórmula de dano ou seletor de alvos nesta parte. O novo contrato causal R04 pertence a RES-01.

## Verificação executada

scripts/testar-combate.ps1 compilou com MSVC /W4 /WX, sem avisos/erros na execução final. Resultado:

- 13 testes básicos, 11 de orçamento, 9 de fase e 20 grupos de tempo aprovados: **53 no total**.
- 168 cenários/replays anteriores e 25 formações AP/replays aprovados.
- Demo de referência preservada: 40 ações, 14 rodadas, vencedor A.
- Isolamento intercalado e lote funcional local anteriores aprovados; sem alegação de desempenho de servidor.

Quatro grupos novos: recarga 0, recarga 1, recarga 3 e replay do novo ciclo. Os três primeiros verificam ambos os perfis (fixo/AP), alvo inválido sem consumo, bloqueio de reutilização prematura, ausência de redução pelo oponente, reutilização correta e saturação em zero. O replay compara eventos/snapshots de entradas invertidas em 24 comandos e quatro usos de recarga 3.

Os testes existentes de extra, reação e passagem de runtime também passaram com os saldos atualizados. Não repetir bateria após atualizações exclusivamente documentais.

## Relatórios e preservação

- Resultado atual consolidado: ../../COMBATE.json.
- Evidência desta entrega: COMBATE-TIME-03-RECARGA.json.
- Matriz atual de grupos: tempo-testes.csv.
- Evidências anteriores preservadas: COMBATE-ANTES-TIME-03.json e tempo-testes-antes-TIME-03.csv.
- Documento TIME-02.md permanece como histórico; esta entrega substitui sua regra de não reduzir recarga recém-usada.

## Próxima entrega interna

Origem dos extras: a fila deve carregar fonte/contexto e flags independentes para avanço de recarga e duração; preservar AP e os limites já existentes. Implementar e testar somente essa parte. Depois separar controle simples de congelamento de recarga e verificar integração de fase. TIME-03 só será marcado concluído após essas partes.

## Parte 2 — Origem e flags dos extras concluída

Esta atualização substitui a indicação anterior de que a parte 2 ainda aguardava implementação. Parte 1 permanece preservada como histórico; TIME-03 continua aberto somente para parte 3.

### Contrato implementado

ExtraTurnSource informa ID estável, categoria (Ability/Equipment/Custom), advances_cooldowns e advances_effect_durations independentes e limite opcional de concessões por cadeia natural. Categoria identifica procedência; as flags declaram o comportamento, sem inferi-lo pelo nome.

ExtraTurnContext registra beneficiário, personagem concedente, habilidade concedente, turno de concessão, oportunidade natural originadora e política da fonte. A fila FIFO preserva esse contexto. Snapshots exibem contexto atual, fila completa e contadores de concessão. Eventos de término/início recebem o contexto da oportunidade a que pertencem, sem trocar a origem dos eventos da oportunidade anterior.

Para extras com fonte explícita, suas duas flags determinam o avanço das recargas e durações do portador naquela oportunidade, substituindo a seleção NaturalOnly/EveryTurn legada para esse extra. Não afetam relógios dos outros combatentes. A proteção de status aplicado no próprio turno continua: ele não perde duração no mesmo encerramento. A recarga usada ainda participa do ciclo da parte 1 quando aquela oportunidade permite redução.

Exemplos configuráveis: fonte de habilidade (true/true); fonte de equipamento (false/true). false/false e true/false também foram testados. Não existe catálogo ou ativação aleatória de equipamentos nesta entrega: são fontes declarativas concedidas pelos efeitos configurados das habilidades de teste.

Extras antigos sem fonte explícita mantêm a regra legada por habilidade/status (NaturalOnly/EveryTurn), preservando as referências anteriores. Novas configurações devem declarar fonte para usar o contrato R02. Não afirmar que um extra legado foi automaticamente convertido em extra de equipamento/habilidade.

### Limites por fonte

max_procs_per_natural_turn = 0 significa ausência de teto específico da fonte; valores 1–1024 impõem teto de concessões. A chave é (concedente, beneficiário, ID da fonte), dentro da cadeia iniciada pela oportunidade natural originadora. Assim, duas habilidades do mesmo concedente que referenciem a mesma fonte não contornam o teto. Fontes de mesmo ID e mesmo dono precisam declarar política consistente.

Ao atingir o teto, a concessão extra é suprimida com ExtraTurnSuppressed. O comando principal continua aceito e seus outros efeitos podem resolver; não é TechnicalLimit e não concede derrota/empate. O contador é reiniciado na próxima seleção natural. Extras concedidos a partir de extras continuam na mesma cadeia natural. Esta é regra de configuração do nosso projeto, não reprodução comprovada de um equipamento comercial.

Proteção técnica de comprimento de cadeia/fila continua independente. Ações inválidas não consomem concessões nem orçamento. Beneficiário morto é removido da fila; timeout não inicia um extra pendente. Entre confrontos, filas, contexto e limites de fonte reiniciam; estado conservado de vida/status/recarga segue as políticas já existentes. reset() da fase reinicia esses metadados também.

### Validação efetiva

Executado scripts/testar-combate.ps1 com /W4 /WX sem avisos: 13 básicos + 11 limites + 9 fases + 27 grupos de tempo = **60 aprovados**. Mantidos 168 cenários/replays e 25 formações/replays AP, isolamento intercalado, lote funcional local e demo 40 ações/14 rodadas.

Sete grupos novos: quatro combinações de flags nos dois perfis; FIFO/proveniência e contextos dos eventos; teto compartilhado por fonte e renovação da cadeia; configuração inválida; morte/orçamento; fronteira e reset de fase; replay completo com fontes explícitas. Nas oportunidades extras, tempo lógico e AP passivo dos demais permanecem estáveis até a próxima seleção natural.

Evidência incremental: COMBATE-TIME-03-EXTRAS.json. Resultado corrente em ../../COMBATE.json e tempo-testes.csv. COMBATE-TIME-03-RECARGA.json permanece preservado com o resultado da parte 1. Não repetir a bateria depois de mudanças somente documentais.

### Próxima entrega

Parte 3: separar bloqueio de ação de congelamento de recarga e verificar sua precedência perante flags de extras e passagem/reset de confronto. Não implementar reflexões/revive/guarda por habilidade nem passivas externas de redução de recarga nesta parte. RES-01 permanece posterior.


## Parte 3 — Controle e congelamento concluídos; TIME-03 encerrado

Block e FreezeCooldowns agora são status independentes, aplicáveis por configuração e importáveis na passagem de runtime validada. Somente Block impede escolher ataque/habilidade/guarda e exige Skip. Somente FreezeCooldowns bloqueia o decremento natural/final das recargas. Combinar os dois produz o controle com congelamento desejado para o perfil semelhante à petrificação, sem presumir demais propriedades do efeito original de outro jogo.

| Bloqueia ação | Congela recarga | Oportunidade |
|---|---|---|
| Não | Não | Age e reduz recarga conforme relógio/fonte |
| Sim | Não | Resolve Skip; reduz recarga conforme relógio/fonte |
| Não | Sim | Pode agir; recarga não diminui |
| Sim | Sim | Resolve Skip; recarga não diminui |

### Precedência e expiração

O motor verifica FreezeCooldowns antes de reduzir durações/remover status. Assim, a última oportunidade congelada não reduz recarga, mesmo que o status expire nesse encerramento. A redução só volta em oportunidade elegível posterior. Duração do próprio status continua avançando conforme relógio/fonte; congelar recarga não congela automaticamente duração.

A flag de um extra pode permitir redução, mas não anula FreezeCooldowns. A outra flag continua controlando duração separadamente. Se o extra não avança durações, o status mantém seu saldo naquela oportunidade.

Aplicar congelamento ao próprio dono durante a habilidade já impede o decremento de sua recarga recém-atribuída naquele encerramento; a proteção contra perda imediata de duração mantém o status recém-aplicado. Esse comportamento foi testado com recarga 3. Quando não há congelamento, o ciclo implementado na parte 1 permanece intacto.

O escopo é **decremento de fim de oportunidade**. Ainda não existe operação externa de reduzir/resetar recarga nem passiva OnTurnStart; não atribuir a FreezeCooldowns uma semântica sobre APIs ainda ausentes. Essa família futura terá seu próprio contrato, conforme PENDENCIAS.md.

### Integração e resultados

RuntimeCarry::Preserve conserva o congelamento e seu saldo de duração junto da recarga. Reset remove ambos explicitamente. A importação normaliza a aplicação para o novo confronto e valida tipos/durações; agora existem quatro tipos de status mínimos. Bloqueio/congelamento não alteram a contagem de comandos aceitos e não permitem ação após o fim da batalha.

Executado scripts/testar-combate.ps1: compilação /W4 /WX sem avisos; 13 básicos + 11 limites + 9 fases + 34 grupos de tempo = **67 aprovados**. Preservados 168 cenários/replays anteriores e 25 formações/replays AP. Demo de referência mantém 40 ações/14 rodadas. Isolamento e lote funcional local passaram, sem inferência de servidor/FPS.

Sete grupos novos: matriz bloqueio/congelamento nos dois perfis; aplicação no próprio turno e expiração; precedência perante as quatro combinações de flags de extras; 1/2/5 confrontos com preservação/reset e retomada do decremento após expiração; última oportunidade bloqueada/congelada no orçamento; replay; validação de status/configuração e coexistência dos quatro tipos.

Evidência final: COMBATE-TIME-03-FINAL.json; consolidado ../../COMBATE.json; tempo-testes.csv com 34 grupos. Evidências das partes 1/2 permanecem preservadas. Não repetir testes após esta atualização exclusivamente documental.

### Próximo bloco

RES-01, primeira entrega interna: identidade/instância de ação, múltiplos acertos e guarda por habilidade. Depois reflexão/mortes FIFO e, por fim, prevenção/auto-revive/Victory Lock. Consultar ../../PLANO-PRIORIDADES.md e CONTRATO-CAUSALIDADE-R04.md. Nenhuma dessas famílias foi implementada pelo TIME-03; a revisão de tempo encerra no escopo acima.
