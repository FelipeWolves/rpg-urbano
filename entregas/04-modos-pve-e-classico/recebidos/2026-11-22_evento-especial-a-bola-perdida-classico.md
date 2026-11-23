# Proposta Conceitual — Evento Especial: *A Bola Perdida* (Modo Clássico / `Lab - Exp`)
**Projeto:** Contos de um Lobo na Cidade (`LOBO`)  
**Obra de referência:** Volume 1 — *A Bola Perdida*  
**Local de Acesso no Hub:** `Lab - Exp` (Eventos Especiais / Modo Clássico — `ENT-14`)  
**Classificação:** `PROPOSTA` (Evento Especial Futuro — fora do tutorial padrão inicial)

---

## 1. Premissa Narrativa (Fiel ao Volume 1)
- **Abertura:** Acima das nuvens, crianças humanas e animais jogam futebol. Um garoto chuta em direção ao gol, e o jovem Lobo goleiro espalma a bola para fora das nuvens, fazendo-a cair em direção à Terra.
- **Missão Discreta:** Com as mães proibindo a descida, as crianças organizam uma busca secreta: o jovem Lobo desce à Terra, um garoto o orienta de cima e os demais vigiam o retorno das mães.
- **Interrupção Narrativa no Meio do Encontro:** O jovem Lobo encontra um homem na Terra que percebe sua presença luminosa sem vê-lo diretamente; o Lobo o alerta sobre a tempestade e brinca sobre construir um barco.
- **Desfecho:** A bola é encontrada junto a um pedregulho, o jovem Lobo retorna às nuvens para retomar o jogo, e o epílogo revela com humor que a Terra enfrentou uma chuva extraordinária de 45 dias.

---

## 2. Estrutura Mecânica do Evento (Objetivo de Recuperação em Turnos)
Em vez de derrotar inimigos, o encontro utiliza a interface de turnos e barras de progresso para simular a busca sob tempestade crescente:
- **Dois Indicadores Principais:**
  1. **Progresso da Busca (`0%` → `100%`):** Meta de vitória ao atingir `100%` (disparando o evento do homem na Terra em `50%` e o encontro do pedregulho em `100%`).
  2. **Intensidade da Tempestade (`0%` → `100%` / Limite de Turnos):** Aumenta a cada turno; se atingir `100%` antes da recuperação, a busca precisa ser reiniciada.
- **3 Ações Exclusivas do Jovem Lobo no Evento:**
  1. **`Voo de Reconhecimento`:** Aumenta diretamente o *Progresso da Busca*.
  2. **`Concentrar Sentidos`:** Localiza pistas (aumenta o ganho de progresso da próxima ação e revela marcos).
  3. **`Avançar pela Tempestade`:** Reduz ou contém o avanço da *Intensidade da Tempestade*, permitindo continuar a procura sob clima severo.

---

## 3. Objetivo Técnico e Encaixe no Cronograma
- **Objetivo:** Validar câmera, seleção de ações, barras de progresso, animações, gatilhos de diálogo no meio do encontro e encerramento narrativo em eventos especiais.
- **Cronograma:** Mantido como `PROPOSTA` vinculada ao `Lab - Exp` (`ENT-14`), após a estabilização do **Tutorial de Batalha Padrão** e da HUD principal (`LAB-PVE-01` a `06`).