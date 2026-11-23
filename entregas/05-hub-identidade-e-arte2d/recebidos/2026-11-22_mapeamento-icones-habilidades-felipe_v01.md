# Mapeamento e Entrega de Ícones de Habilidades — Felipe (`VIS / HUD`)
**Projeto:** Contos de um Lobo na Cidade (`LOBO` / *Mundus Lupis Disco*)  
**Data:** 22 de novembro de 2026  
**Status:** `ORIGINAIS RECEBIDOS / DERIVADOS PNG PREPARADOS`  
**Pasta Raiz:** `entregas/05-hub-identidade-e-arte2d/recebidos/`

---

## 1. Estrutura de Pastas e Especificações Técnicas
- **Organização:** Os 8 originais JPG estão em `recebidos/`: os 4 adultos na raiz e os 4 jovens em `icones-habilidades-jovem-lobo/` para manter a entrega organizada por fase do personagem.
- **Formato Visual:** 8 arquivos individuais quadrados (`1:1`), estilo *squircle*/losango inclinado inspirado na linguagem visual de *Dislyte*, sem texto embutido na arte (a HUD da Unreal sobrepõe os textos `PASSIVA`, `CD` e contadores como `4/5` dinamicamente).
- **Nota de Processamento (Canal Alpha):** Os arquivos entregues nesta remessa estão isolados individualmente em fundo branco sólido (`#FFFFFF`) externo à moldura; os derivados já receberam remoção do branco externo; usar o catálogo consolidado, mantendo os originais e conferindo a futura importação para a Unreal Engine.

---

## 2. Lobo Idade Normal (`CHAR_FELIPE`)
*Local dos originais:* raiz de `recebidos/`  
*Tema cromático:* Azul-Escuro / Safira e Prata (`#070D1C` / `#4D7CFE` / `#C8D3E6`)

| Caminho Relativo | Habilidade / Estado | Descrição Visual |
|---|---|---|
| `ico_skill_felipe_a1.jpg` | `A1` — *Garra do Asfalto* (Básica) | Três cortes diagonais de garra afiada com rastro cinético em arco azul-prateado. |
| `ico_skill_felipe_a2.jpg` | `A2` — *Postura de Vigília* (Passiva — Estado Padrão `0/5` a `4/5`) | Emblema da cabeça de lobo com escudo angular e asa única em tom azul-noturno escuro/inativo. |
| `ico_skill_felipe_a2_active.jpg` | `A2` — *Postura de Vigília* (Passiva — Estado Ativo `5/5`) | Mesmo emblema com brilho safira intenso, ondas concêntricas de contra-ataque e borda acesa. |
| `ico_skill_felipe_a3.jpg` | `A3` — *Eclipse na Floresta de Pedra* (Especial AoE) | Emblema completo iluminado em prata-safira com aura de vigília. |

---

## 3. Jovem Lobo (`CHAR_FELIPE_JOVEM`)
*Subpasta:* `icones-habilidades-jovem-lobo/`  
*Tema cromático:* Azul-Céu e Gelo (`#0E2A47` / `#7CE0FF` / `#B8F2FF`)

| Caminho Relativo | Habilidade / Estado | Descrição Visual |
|---|---|---|
| `icones-habilidades-jovem-lobo/ico_skill_jovem_lobo_a1.jpg` | `A1` — *Garra Celeste / Investida* (Básica) | Três cortes ascendentes em azul-céu cercados por redemoinhos de vento e nuvens estilizadas. |
| `icones-habilidades-jovem-lobo/ico_skill_jovem_lobo_a2.jpg` | `A2` — *Instinto das Nuvens* (Passiva — Estado Padrão) | Silhueta ágil do jovem lobo cercada por espirais de corrente de ar em azul-céu. |
| `icones-habilidades-jovem-lobo/ico_skill_jovem_lobo_a2_active.jpg` | `A2` — *Instinto das Nuvens* (Passiva — Estado Ativo) | Mesmo símbolo em azul-gelo luminoso com anéis concêntricos de vento no fundo e borda acesa. |
| `icones-habilidades-jovem-lobo/ico_skill_jovem_lobo_a3.jpg` | `A3` — *Salto da Tempestade / Impulso Aéreo* (Especial) | Raio alado descendente cortando nuvens de tempestade em azul-céu vibrante. |
## Derivados PNG preparados

Os originais JPG permanecem intactos. Os 8 derivados PNG com alpha estão em assets/runtime/ui/2026-11-22-v02/, nas subpastas icones-habilidades-felipe-adulto/ e icones-habilidades-jovem-lobo/, com os mesmos nomes-base e extensão .png. Mantidos 2048x2048, sem corte ou escala. Fundo branco conectado ao exterior removido; detalhes claros internos preservados. O catálogo data/presentation/ui-assets-2026-11-22.json mapeia fonte real e saída. Ainda não integrados à Unreal. Nomes alternativos do kit jovem continuam sujeitos a alinhamento mecânico.
