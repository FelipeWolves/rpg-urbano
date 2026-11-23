# Mapeamento e Entrega de Ícones de Status — Combate e Controles (`VIS / HUD`)
**Projeto:** Contos de um Lobo na Cidade (`LOBO` / *Mundus Lupis Disco*)  
**Data:** 22 de novembro de 2026  
**Status:** `MAPEADO / PENDENTE DE ARQUIVOS PNG`  
**Pasta Raiz:** `entregas/05-hub-identidade-e-arte2d/recebidos/`

---

## 1. Diretrizes de Arte e Organização
- **Organização:** Arquivos separados em subpastas dentro de `icones-status/` (`debuffs/` e `buffs-defesas/`) para manter a pasta de recebidos limpa e fácil de indexar.
- **Formato e Estilo:** Ícones sempre **quadrados (`1:1`)**, sem texto embutido na imagem (turnos, contadores e rótulos são desenhados pela HUD da Unreal), seguindo a linguagem de moldura/glifo no estilo de *Dislyte*.
- **Regra de Cores por Natureza do Efeito:**
  - **Debuffs:** Paleta vermelha (`Red`).
  - **Buffs e Defesas:** Paleta verde (`Green`), respeitando bordas de destaque mapeadas no sistema (`Silver` para escudo prateado e `Golden` para imunidade/invencibilidade).

---

## 2. Debuffs (`icones-status/debuffs/` — Paleta Vermelha)

| Caminho Relativo | Efeito no Motor | Descrição Visual |
|---|---|---|
| `icones-status/debuffs/ico_status_bleed.png` | `Bleed` (*Sangramento*) | Corte de garra em vermelho com gotas de sangue. |
| `icones-status/debuffs/ico_status_poison.png` | `Poison` (*Veneno*) | Glifo tóxico / gotas ou crânio estilizado em vermelho. |
| `icones-status/debuffs/ico_status_silence.png` | `Silence` (*Silêncio*) | Onda sonora / voz cortada em vermelho (bloqueia especiais; mantém `A1`). |
| `icones-status/debuffs/ico_status_suppression.png` | `Suppression` (*Supressão*) | Runa de passiva bloqueada / corrente partida em vermelho (desativa passivas nativas). |
| `icones-status/debuffs/ico_status_freeze.png` | `Freeze` (*Congelamento*) | Cristal de gelo em vermelho. |
| `icones-status/debuffs/ico_status_petrification.png` | `Petrification` (*Petrificação*) | Rocha / pedra endurecida em vermelho. |

---

## 3. Buffs e Defesas (`icones-status/buffs-defesas/` — Paleta Verde)

| Caminho Relativo | Efeito no Motor | Descrição Visual / Borda |
|---|---|---|
| `icones-status/buffs-defesas/ico_status_shield_standard.png` | `StandardShield` (*Escudo Padrão*) | Escudo simples em verde. |
| `icones-status/buffs-defesas/ico_status_shield_silver.png` | `SilverShield` (*Escudo Prateado*) | Escudo reforçado em verde com destaque/borda prateada. |
| `icones-status/buffs-defesas/ico_status_immunity_invincible.png` | `Invincible` / `DebuffImmunity` (*Invencibilidade / Imunidade*) | Barreira protetora em verde com destaque/borda dourada. |