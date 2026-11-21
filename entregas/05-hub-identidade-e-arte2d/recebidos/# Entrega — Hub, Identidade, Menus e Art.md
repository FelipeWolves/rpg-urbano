# Entrega — Hub, Identidade, Menus e Arte 2D (P2)

**Decisões Já Fechadas (Preservadas):**
- **Identidade e Tipografia:** *Mundus Lupis Disco* / codinome `LOBO`; fontes **Cormorant Garamond** (títulos/letreiros) e **Lora** (textos de painéis), sem *Cinzel*.
- **Acessos Ativos:** Dois acessos ativos no Hub (**Evento Especial** no outdoor e **Batalha** no dispositivo na mão do personagem); terceiro acesso adiado.
- **Paleta Oficial (Noturno Urbano & Místico):** Fundo escuro `#0B0D14` (85% opacidade), Azul Celestial `#38BDF8` + Púrpura `#9333EA` (Evento Especial), Âmbar Dourado `#F59E0B` + Carmesim `#DC2626` (Batalha), e Branco Lunar `#F8FAFC` / Prata `#CBD5E1`.

---

### Entrega Visual — Pasta `BG-tela-inicial-Prototipo-02` (`05-hub-e-visual/recebidos/`)

Foi adicionada a subpasta `BG-tela-inicial-Prototipo-02` dentro de `entregas/05-hub-e-visual/recebidos/` contendo **9 arquivos**:
- **8 variantes de fundo limpas (sem texto):** variações de clima e horário do cenário da tela inicial seguindo o padrão `bg_home_<variante>.png`.
- **2 imagem indicativa/gabarito (com textos):** referência visual mostrando apenas o posicionamento e o estilo desejados para os botões/textos clicáveis do Hub (não utilizar como textura de fundo; os botões devem ser implementados como elementos interativos reais da interface).

## 1. Zonas de Recorte e Proteção em Retrato (`9:16` vs. `16:9`)
**DECISÃO DESEJADA (Imagem Única Panorâmica com Câmera / Focal Point Responsivo):**
- **Relação entre Paisagem e Retrato:** Não existe uma segunda imagem estática separada para mobile no momento — o Hub utiliza a mesma arte panorâmica (`assets/runtime/hub/panorama.jpg`) em todas as proporções, ajustando o enquadramento pela câmera/viewport (`USafeZone` no UMG).
- **Áreas Críticas que Não Podem Sumir no Recorte Retrato (`9:16`):**
  1. **O Personagem Principal (O Lobo em primeiro plano) e o Dispositivo em sua Mão** (âncora do acesso **Batalha**).
  2. **O Outdoor Iluminado** (âncora do acesso **Evento Especial**).
- **Comportamento de Enquadramento e Rolagem:**
  - Em telas **16:9 (Desktop)**, exibe o panorama completo horizontalmente.
  - Em telas **9:16 (Mobile Retrato)**, a câmera centraliza a zona útil entre o outdoor e o personagem na mão do dispositivo, mantendo margem interna segura (`Safe Zone`) contra entalhes de câmera (*notch*) e barra de gestos inferior, permitindo leve arrasto/panorâmica horizontal (*pan/drag*) caso o jogador queira observar as laterais sem perder os botões de vista.

---

## 2. Mapa de Menus, Destinos Futuros, Botão Voltar e Estados Bloqueados
**PROPOSTA / DECISÃO DESEJADA (Estrutura de Navegação do Hub — Sem Simular Backend/Loja Ativos):**

| Acesso / Menu | Estado Atual | Destino e Comportamento | Texto do Painel / Aviso |
| :--- | :--- | :--- | :--- |
| **1. Batalha** *(Dispositivo na mão)* | **Ativo (MVP)** | Abre diretamente a Arena de Combate configurada (perfil padrão de Equipes), com opção de alternar para o **Modo Clássico** nos cenários de teste habilitados. | Legenda no Hub: `BATALHA` (ícone geométrico original com pulso suave de `1.0` a `1.06` e modo estático em movimento reduzido). |
| **2. Evento Especial** *(Outdoor)* | **Ativo (Painel Provisório)** | Abre painel modal sobreposto ao Hub, reservado para a **Linha de Eventos em Modo Clássico (estilo RPG PS1/PS2)**, eventos de purificação e exibição rotativa de **artes vencedoras da comunidade** no Outdoor. | **Título:** `EVENTO ESPECIAL — EM PREPARAÇÃO`<br>**Texto:** `Área reservada para a linha de eventos especiais em Modo Clássico e desafios temporários do universo Contos de Um Lobo na Cidade. Este painel valida a navegação do Hub e será conectado aos eventos em etapa futura.` |
| **3. Campanha / Guia dos Livros** | **Planejado (Bloqueado no Hub)** | Destino futuro que reunirá os **4 Livros Fechados** (67 capítulos), as **3 Linhas Narrativas** (*Luminosa*, *Crepuscular*, *Sombria*) e os **Círculos Conectados** (que concedem expansão permanente de inventário ao fechar 100%). | **Estado Bloqueado:** Ícone com cadeado discreto e aviso: `GUIA DOS LIVROS — EM BREVE (Capítulo em preparação)`. |
| **4. Elenco / Sintonias** | **Planejado (Bloqueado no Hub)** | Destino futuro para visualizar os personagens (*Felipe, O Filhote, Michel, O Dálmata*), alternar **Fases Elementais** e consultar habilidades (`A1/A2/A3` no modo Equipes e `Limit/SP` no Modo Clássico). | **Estado Bloqueado:** `ELENCO E SINTONIAS — EM PREPARAÇÃO`. |
| **5. Configurações / Acessibilidade** | **Ativo (Local)** | Painel leve para alternar qualidade gráfica (*Baixo / Médio / Alto / Ultra HD*, *HDR*, física/suavização), FPS (`30/60`), **Movimento Reduzido** e ritmo **ATB (`Wait` / `Active`)** do Modo Clássico. | `CONFIGURAÇÕES — Ajustes gráficos, acessibilidade e ritmo de combate.` |
| **6. Estúdio Sonoro (Modo DJ) & Lojas** | **Adiado (Fora do MVP)** | Destinos futuros vinculados ao curador musical do *Mundus Lupis Disco* (**Modo DJ / Purificação**) e às **Lojas Separadas** (*Ouro 2h, Forja, Fichas Clássicas, Dados Sonoros e PvP*), exibidos apenas quando o backend autoritativo estiver ativo. | `RECURSO ONLINE — DISPONÍVEL APENAS NA VERSÃO CONECTADA`. |

- **Regra Universal do Botão Voltar (`Back / ESC`):**
  - Todo painel modal (como *Evento Especial* ou *Configurações*) exibe um botão **Voltar / Fechar (`X`)** no canto superior direito dentro da `USafeZone` e responde à tecla `ESC` (PC) ou botão/gesto *Voltar* do Android, retornando instantaneamente ao Hub sem recarregar a cena.
  - Dentro da Arena de Batalha (tanto no modo Equipes quanto no Modo Clássico), o menu de pausa oferece **Retornar ao Hub**, encerrando a sessão local de forma limpa.

---

## 3. Recursos Gráficos, Autoria e Licenciamento
**REFERÊNCIA / AUDITORIA DE DIREITOS (Pendente de Substituição ou Confirmação Final):**

| Recurso | Arquivo / Origem | Autoria e Situação de Licença | Ação Recomendada para Produção |
| :--- | :--- | :--- | :--- |
| **Panorama do Hub** | `assets/runtime/hub/panorama.jpg` (e versões em `assets/sources/`) | Acervo próprio do projeto gerado/dirigido pelo autor (*Felipe Sena Pereira*). | Uso liberado no projeto; manter arquivo matriz em alta resolução em `assets/sources/` para futuros ajustes de recorte. |
| **Silhueta do Lobo (Logo Provisório)** | `contosdeumlobo.com.br` (`klemzo123_silhouette-wolf-2...png`) | Baixada do site do autor em 2018; **distribuição comercial ainda não auditada** (origem de banco/terceiro *klemzo123*). | **NÃO usar em build comercial final sem substituição:** manter apenas como placeholder local provisório e substituir por logotipo vetorial (`SVG` + `PNG` transparente) 100% autoral ou com licença comercial comprovada antes do lançamento. |
| **Tipografia (`Cormorant Garamond` e `Lora`)** | Google Fonts | **SIL Open Font License (OFL 1.1)** — gratuita e auditada para uso comercial em jogos PC, Android e iOS. | Liberada para empacotamento definitivo no jogo. |
| **Ícones de UI e Retratos de Personagens** | A produzir (`ART-01`) | Ainda não entregues em alta resolução/vetor; usar formas geométricas originais criadas em código/vetor próprio na etapa atual. | Entregar futuramente com arquivos editáveis (`.svg` / `.psd` / `.kra`) e registro de autoria antes de entrar no build final. |


