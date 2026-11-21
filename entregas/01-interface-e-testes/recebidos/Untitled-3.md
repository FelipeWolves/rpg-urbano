# Entrega — Interface, Acessibilidade e Testes (P1)

## 1. Ambiente de Teste e Tela
- **DECISÃO DESEJADA (Ambiente de Referência PC):** Testes principais em Windows com resolução Full HD (1920x1080) e QHD (2560x1440), escala do Windows em 100% / 125%, validando janela maximizada (16:9) e janela/simulação vertical (9:16, ex.: 412x915 no Mobile Preview PIE da Unreal Engine).
- **NÃO SEI (Aparelho Físico):** Testes finais em aparelho físico Android/iOS dependem do empacotamento mobile futuro; por ora, validar via Mobile Preview PIE e perfis de Screen Size do UMG (Portrait 9:16 e Landscape 16:9) respeitando `USafeZone`.

## 2. Fluidez e Transições
- **PROPOSTA (Fluxo Hub <-> Evento / Arena):**
  - *O que esperamos:* Ao clicar/tocar em **Evento Especial**, o painel provisório abre de forma imediata (fade rápido de 150ms–200ms) sem travar o fundo; ao clicar em **Batalha**, transição direta para a arena existente com carregamento limpo e botão claro de retorno ao Hub.
  - *O que evitar:* Esperas longas sem feedback visual ou animações bloqueantes entre turnos/telas. Separar sempre velocidade de animação visual (1x / 2x) da lógica de combate.

## 3. Legibilidade, Estados e Gestos
- **DECISÃO DESEJADA (Contraste e Tipografia):** Textos e painéis sobre fundo escuro noturno translúcido (`#0B0D14` com 85% de opacidade) e texto em Branco Lunar (`#F8FAFC`) para garantir contraste alto sobre cenários ilustrados.
- **PROPOSTA (Indicação de Estados e Interação):**
  - *Clique / Toque curto:* Seleciona unidade, confirma ação, abre painel de Evento Especial ou entra na Arena. Área mínima de toque de `48x48px`.
  - *Passar o mouse (Hover no PC) ou Toque longo / Segurar (Hold no Mobile):* Exibe tooltip ou card compacto com detalhes da habilidade, descrição dos buffs/debuffs (com moldura diferenciada: Green, Red, Silver, Golden) e contagem de turnos restantes.
  - Compatibilidade com Modo Clássico: A interface compartilhada deve prever alternância limpa entre o HUD de Equipes (3 posições + barra AP) e o HUD Clássico (menu de 5 comandos: Atacar, Habilidades, Defender, Itens, Especial/Limite + barra ATB), mantendo suporte total a navegação por teclado/controle (Setas + Confirmar/Cancelar).

## 4. Teclado, Controle e Acessibilidade (Sem Movimento)
- **DECISÃO DESEJADA (Teclado / Controle):**
  - `ESC` ou botão direito/voltar: Fecha painéis abertos (como Evento Especial) ou abre pausa na arena.
  - `Setas / Tab` + `Enter / Espaço`: Navegação e confirmação entre acessos do Hub, unidades e habilidades (suporte básico a gamepad: D-Pad/Analógico + Confirmar/Cancelar).
- **DECISÃO DESEJADA (Movimento Reduzido / `prefers-reduced-motion`):** Quando o modo "Sem Movimento" / movimento reduzido estiver ativo, desligar pulsos, escalas e partículas de UI; manter destaque por borda dupla estática e brilho fixo (glow) em Âmbar (`#F59E0B`) e Carmesim (`#DC2626`).
- **Compatibilidade com Modo Clássico e Modo DJ (`EXP-01`):**
  - *Modo Clássico:* Alternância limpa entre o HUD de Equipes (3 posições + AP) e o HUD Clássico (menu de 5 comandos: *Atacar, Habilidades, Defender, Itens, Especial/Limite* + barra ATB), navegável por setas/controle.
  - *Modo DJ / Rítmico:* Suporte a toque multi-zona no mobile (`9:16`) e teclas dedicadas no PC (`D / F / J / K` no layout padrão de 4 pistas e `Q / E / S / Z / C` no layout diagonal de 5 zonas inspirado em *Pump It Up*). No modo **Sem Movimento**, desligar pulsos e flashes de fundo da pista mantendo o contraste e a leitura nítida das notas.