# HAB-01 parte 2 — passiva de abertura R01

## Contrato do incremento

Passiva autoral **Preparação**: uma vez ao criar um combate novo, seu dono recebe escudo comum de 10 pontos por dois turnos naturais próprios. Números são provisórios de laboratório, não balanceamento nem reprodução de um personagem externo.

- Gatilho suportado: BattleStart. Destinatário: o próprio dono, vivo na abertura. ProfileBattle cria todos os participantes vivos; importação de mortos/combates em andamento não é suportada por essa API.
- O disparo acontece na montagem do runtime inicial, antes de comandos e sem consumir AP, ação, recarga ou orçamento. Como escudo não altera iniciativa, a ordem de entrada permanece a do kernel.
- Limite: uma ativação por dono/configuração por combate. Consultar estado, recusar comandos, receber golpes, ganhar extras, morrer ou reviver não dispara novamente. Reiniciar cria outro combate e reaplica uma única vez.
- Duração NaturalOnly: o fim da primeira oportunidade natural do dono reduz 2 para 1; o fim da segunda remove o escudo. Ações de inimigos e extras não elegíveis não contam. Flags explícitas de extras continuam regidas por TIME-03, inclusive quando autorizam avançar durações.
- O escudo entra na camada comum existente: absorção, consumo, expiração, reflexão, explosões e vitória permanecem no kernel. Uma proteção ativa posterior substitui o escudo comum segundo a regra já existente, sem soma implícita.
- R04 não é reordenado: esta passiva não abre reação, ataque, prevenção ou revive. Não há nova fila de eventos entre acertos.
- Posição passiva não selecionável; híbrida mantém sua ativa selecionável. Uma nota sem definição executável continua informativa, com aviso explícito.
- Registro imutável por combate: dono, posição, ID e definição da ativação no ProfileSnapshot. Não é log de ação e não consome IDs de ação. Reset substitui o registro, sem acumular entradas antigas.
- Validação: ID não vazio/único na configuração, gatilho conhecido, magnitude positiva, duração de 1 a 1.000; no máximo uma passiva de escudo de abertura por dono para evitar sobrescrita ambígua.

## Limites

Não implementa event bus genérico, gatilhos durante combate, supressão/silêncio, recarga interna de passiva, RNG, importação/restauração de snapshots, persistência entre etapas ou adaptação dos coordenadores SEQ/PAR. O runtime inicial existente é reutilizado somente para este gatilho de preparação; gatilhos por impacto/morte exigirão integração causal própria, não devem ser simulados por esse mecanismo.

A demo equipa Preparação na posição 2 de A2 e B2. Os componentes híbridos de A3/B3 continuam exemplos informativos, sem efeitos passivos. O clássico permanece sem essas passivas por padrão.

## Aceite

Validar estado inicial e identidade, ausência de custo, não seleção, absorção/expiração, rejeição de configuração conflitante, híbrida com ativa, replay/reset/isolamento, proteção contra reflexão e término único após dano letal. Preservar os testes anteriores de R04/tempo/coordenação.
