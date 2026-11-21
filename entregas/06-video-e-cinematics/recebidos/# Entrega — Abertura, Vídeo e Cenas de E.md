# Entrega — Abertura, Vídeo e Cenas de Evento (P2)

**Recursos Já Existentes (Preservados):**
- Vídeos de abertura em paisagem (`16:9`) e retrato (`9:16`) e arte lateral já recebidos no repositório local; não reenviar nesta entrega.

---

## 1. Quando Reproduzir, Como Pular e Destino Pós-Vídeo
**DECISÃO DESEJADA (Fluxo de Entrada e Cenas):**
- **Vídeo de Abertura Principal:**
  - **Primeira Abertura (First Launch):** Reproduz automaticamente na primeira vez que o jogador abre o jogo instalado.
  - **Aberturas Seguintes:** Não força repetição obrigatória a cada inicialização (vai direto para a tela de Título / Hub após carregamento rápido), mas permanece acessível a qualquer momento através de um botão discreto **"Rever Abertura"** na tela de Título ou no menu *Configurações / Guia dos Livros*.
  - **Cenas de Evento e Capítulos dos 4 Livros:** Reproduzidas automaticamente na primeira entrada de um capítulo inédito ou evento especial (incluindo introduções da linha de **Evento Especial / Modo Clássico**), com opção de rever pelo *Guia do Livro* correspondente.
- **Como Pular (`Skip`):**
  - Exibir botão **"Pular" (`Skip`)** no canto superior direito (respeitando `USafeZone`), ativável por clique/toque ou tecla `ESC` / `Espaço` / botão cancelar do controle.
  - **Comportamento:** Na primeira exibição de um vídeo inédito, exige confirmação simples (ex.: primeiro toque exibe o botão `Pular >>`, segundo toque confirma) ou segurar por `0.8s` para evitar pulos acidentais; em vídeos já assistidos anteriormente, o pulo é imediato com 1 toque/clique.
- **Destino Pós-Vídeo:**
  - *Abertura Principal:* Segue diretamente para o **Hub Principal** (`Mundus Lupis Disco` / `LOBO`) se já houver sessão local validada, ou para a tela de **Toque para Iniciar / Login** em produção.
  - *Cena de Capítulo / Evento:* Segue imediatamente para a **Arena de Batalha** correspondente (Modo Equipes ou Modo Clássico) ou retorna ao *Guia do Livro* caso tenha sido aberta em modo de leitura/revisão.

---

## 2. Áudio, Legendas, Pausa e Interrupções (Mobile / PC)
**DECISÃO DESEJADA (Controle de Mídia e Ciclo de Vida do Aplicativo):**
- **Áudio:** Respeita o volume mestre e o canal de *Mídia/Cutscenes* das Configurações do jogo, além do interruptor físico de silencioso/volume do celular. Caso o dispositivo esteja com áudio mutado no sistema, inicia o vídeo normalmente sem travar a reprodução.
- **Legendas (`Subtitles`):**
  - Carregadas por arquivo de texto externo sincronizado (`.srt` ou tabela localizada `PT-BR` / `EN`), nunca queimadas dentro do arquivo de vídeo, permitindo troca de idioma sem duplicar vídeos.
  - Exibidas dentro da `USafeZone` inferior sobre faixa escura semitransparente (`#0B0D14` a 75% de opacidade) na fonte **Lora**, com tamanho legível em telas `9:16`.
- **Pausa Manual:**
  - Tocar no centro da tela (ou pressionar `P` / `Pause`) exibe controles discretos de **Pausar / Retomar** e **Pular** por 3 segundos antes de ocultá-los novamente.
- **Chamadas, Notificações e Segundo Plano (`App Lifecycle` no Android / iOS / Windows):**
  - *Chamada telefônica recebida ou envio do app para segundo plano (`OnApplicationWillDeactivate` / `onPause`):* **Pausa imediatamente** o vídeo e o áudio no exato timestamp atual.
  - *Retomada ao primeiro plano (`OnApplicationHasReactivated` / `onResume`):* Mantém o vídeo pausado com um botão central **"Toque para Continuar"** (evitando que o jogador perca trechos da história enquanto desbloqueia o celular) ou retoma suavemente após 1 segundo se houver áudio contínuo de fundo.
  - *Notificações flutuantes (banners sem perda de foco):* Não interrompem o vídeo.

---

## 3. Prioridade de Orientação e Rotação Automática no Mobile
**DECISÃO DESEJADA (Rotação Cinematográfica Estilo *XXL Woofia* + Preservação de Proporção):**
1. **1ª Prioridade — Vídeo Nativo da Orientação Atual:** Se o vídeo possuir versão dedicada para retrato (`9:16`), reproduz diretamente em retrato sem girar o aparelho.
2. **2ª Prioridade — Rotação Automática Forçada para Paisagem (`16:9`):** Quando uma abertura, trailer ou cena de evento possuir apenas versão panorâmica/paisagem (`16:9`), o player **rotaciona automaticamente a orientação da tela do celular para Paisagem (`Landscape`)** durante a reprodução para aproveitar toda a tela, e **restaura automaticamente para Retrato (`9:16`)** no instante em que o vídeo termina ou é pulado.
3. **3ª Prioridade — Composição Lateral (`Arte Lateral Já Recebida`) ou Contenção (`Letterbox`):** Em telas com proporções atípicas (como monitores ultrawide `21:9` no PC ou tablets/dobráveis `4:3`), preenche as faixas excedentes com a **arte lateral dedicada** (ou barras em Preto Obsidiana `#0B0D14` se não houver arte lateral para aquela cena), nunca esticando ou cortando personagens e legendas.

---

## 4. Padrão Técnico para Versões Novas e Storyboards
**DECISÃO DESEJADA (Arquivo Único em Alta Definição + Adaptação Automática pelo Motor):**
- **Arquivo Único por Cena:** Subir **apenas uma versão final em alta qualidade** (ex.: Full HD `1920x1080` ou Ultra HD `3840x2160`, `30/60 FPS`, codec **H.264 / MP4**). Não produzir múltiplas versões manuais de baixa/média qualidade por vídeo.
- **Adaptação por Dispositivo e Qualidade:** O player da Unreal Engine realizará o redimensionamento (*downscaling*) em tempo real conforme a tela e a predefinição gráfica escolhida pelo jogador (ou transcodificação automática no empacotamento mobile, se necessário no futuro), garantindo leveza em celulares intermediários sem trabalho duplicado.
- **Duração Recomendada:**
  - *Abertura Principal:* `45s` a `90s`.
  - *Vinhetas de Capítulo / Evento Clássico:* `10s` a `25s`.
- **Fluxo de Produção:** Aceita entrega prévia de **storyboard em imagens estáticas sequenciais** (com legendas/diálogos) antes da renderização do vídeo final. Projetos editáveis ficam guardados apenas localmente em `assets/sources/`, fora do Git.

---

## 5. Falha de Arquivo e Preferências de Janela Antes do Início
**DECISÃO DESEJADA (Resiliência e Inicialização Limpa):**
- **Se o Arquivo de Vídeo Faltar ou Estiver Corrompido:**
  - O player **nunca pode travar o jogo nem deixar tela preta infinita**.
  - **Fallback Imediato:** Registra aviso silencioso no log técnico (`VideoAssetMissingOrCorrupt`), exibe por `2.5s` uma **tela estática ilustrada de transição** (usando a arte lateral ou imagem chave da cena com o título/sinopse do momento) e avança automaticamente para o destino previsto (Hub ou Arena).
- **Preferências de Janela Antes do Início (PC / Windows):**
  - Na primeira execução no Windows, o jogo inicia automaticamente em **Janela sem bordas / Tela cheia adaptada à resolução nativa do monitor (`16:9`)** com predefinição gráfica equilibrada, sem bloquear o jogador com um launcher técnico complexo antes da abertura.
  - Caso o jogador já tenha salvo preferências locais anteriormente (ex.: janela vertical de teste, resolução específica, modo **Movimento Reduzido** ou volume), essas preferências são carregadas **antes** da inicialização do player de abertura.