# Entrega — Mobile, Dispositivos e Desempenho (P1)

## 1. Fluxo de Testes e Aparelhos de Referência
- **DECISÃO DESEJADA (Ambiente Principal de Validação Mobile):** Utilizar o **Android Studio (AVD / Emulador oficial)** configurado com um **perfil de aparelho mediano de 8 GB de RAM** como ambiente principal de testes mobile durante o desenvolvimento, deixando a validação em aparelho físico (Samsung Galaxy S23 Ultra) para uma versão mais madura do projeto.
- **DECISÃO DESEJADA (Orientação Preferida):** Retrato (`9:16` / vertical) no mobile, compartilhando o mesmo núcleo lógico e assets do modo Paisagem (`16:9` / horizontal) do desktop, sem duplicar sistemas.
- **DISPONIBILIDADE DE TESTE:** Sim — há disponibilidade para testar o `.apk` no emulador do Android Studio agora e no Galaxy S23 Ultra físico em etapa mais avançada, sem envio de chaves ou credenciais pelo repositório.

## 2. Esclarecimento sobre "Modo Mobile" e Estado Atual
- **REFERÊNCIA / DIAGNÓSTICO:** O redimensionamento vertical testado anteriormente no Windows ocorreu no protótipo atual em Canvas (que não possui Designer UMG e onde mudar o tamanho da janela não altera a plataforma para Android/iOS).
- **DECISÃO DESEJADA (Separação de Escopo):** Manter o protótipo atual focado na validação funcional no Windows; realizar o empacotamento Android (`.apk`) e validação no AVD do Android Studio como etapa dedicada, sem instalar BlueStacks nem comprar hardware adicional.

## 3. Prioridade de Plataformas e Requisitos Mínimos
- **DECISÃO DESEJADA (Ordem de Prioridade):**
  1. **Windows (PC — Steam / Epic Games Store):** Prioridade imediata para desenvolvimento, testes rápidos e validação do núcleo de combate e Hub.
  2. **Android (Google Play Store / Samsung Galaxy Store):** Primeira plataforma mobile alvo (validação contínua via Android Studio AVD 8 GB RAM e posterior teste em aparelho físico).
  3. **iOS (Apple App Store):** Suporte planejado para etapa posterior após estabilização do build Android.
- **DECISÃO DESEJADA (Perfil Mínimo de Referência):** Aparelho intermediário com **8 GB de RAM** e tela Full HD+ 60 Hz (usado como perfil padrão no Android Studio AVD), garantindo que o jogo rode fluido antes de escalar para dispositivos topo de linha.

## 4. Escalabilidade Gráfica (Ultra HD / HDR até Modo Desempenho), FPS e Modo Offline
- **DECISÃO DESEJADA (Escalabilidade Gráfica Completa — PC e Mobile):**
  - **Teto Gráfico Máximo (PC / Celulares Topo de Linha):** Suporte opcional a resolução **Ultra HD (4K)**, renderização em **HDR**, simulação de **física avançada** (tecidos, partículas, ambiente) e **suavização completa (anti-aliasing e pós-processamento)**.
  - **Modo Desempenho / Compatibilidade (Desativação Granular):** Permitir que resolução interna (escala de renderização), HDR, física visual, sombras, partículas e suavização possam ser **reduzidos ou totalmente desabilitados** nas configurações (ou via predefinições: *Desempenho / Baixo*, *Médio*, *Alto*, *Ultra HD*), garantindo fluidez mesmo em celulares intermediários ou PCs sem placa dedicada.
  - **Regra Arquitetural:** As opções gráficas, física visual e suavização atuam **exclusivamente na camada de apresentação**, sem alterar o determinismo, os turnos ou o cálculo do combate.
- **DECISÃO DESEJADA (Taxa de Quadros):** Suporte a **30 FPS** (economia de bateria / aparelhos modestos), **60 FPS** (padrão) e desbloqueado/alta taxa no PC, além do modo de **Movimento Reduzido**.
- **DECISÃO DESEJADA (Conexão Online e Segurança Contra Fraudes):**
  - **Produto Final (Produção — Always-Online):** O jogo publicado exigirá conexão com a internet (modelo *server-authoritative*, seguindo o padrão do *Dislyte*), evitando manipulação de saves locais, edição de memória ou vantagens irregulares. O núcleo determinístico de combate permitirá validação de *replay/seed* no servidor antes da entrega de recompensas e registro de placares.
  - **Ambiente de Desenvolvimento (Dev / QA):** Apenas durante a fase de desenvolvimento e testes locais (PC e Android Studio AVD), o simulador de combate e o Hub operam de forma autônoma sem depender de backend ativo.
  - Acessibilidade de Ritmo no Mobile (Modo Clássico): Para telas sensíveis ao toque (9:16), o Modo Clássico utilizará ATB Wait como padrão (pausando a barra enquanto o jogador navega nos submenus de Habilidades ou Itens), deixando o ATB Active como opção selecionável nas configurações.