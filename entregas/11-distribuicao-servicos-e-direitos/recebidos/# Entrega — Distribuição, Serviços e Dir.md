# Entrega — Distribuição, Serviços e Direitos (P3 — Posterior)

**Estado Atual (Preservado):**
- O laboratório e protótipo atual operam **100% offline** no Windows (`UE 5.8.4`), sem exigir conta, servidor ou loja. Lojas, serviços online e publicação comercial são etapas futuras condicionadas à maturidade do produto[cite: 9].

---

## 1. Prioridade de Plataformas, Conectividade e Multiplayer
**DECISÃO DESEJADA / PROPOSTA (Roadmap de Distribuição e Arquitetura de Rede):**

- **Ordem de Prioridade das Plataformas:**
  1. **Prioridade 1 — PC (Windows x64) via Steam & Epic Games Store:** Base principal de desenvolvimento e lançamento inicial, aproveitando a política de preços regionais e cupons da **Epic Games Store** e o alcance da **Steam**.
  2. **Prioridade 2 — Android via Google Play & Samsung Galaxy Store:** Foco em acessibilidade mobile e economia para jogadores no Brasil/América Latina (aproveitando cupons e benefícios recorrentes da **Samsung Galaxy Store**), validado previamente em emulador de referência (Android Studio AVD — 8 GB RAM) e aparelhos físicos intermediários/topo de linha.
  3. **Prioridade 3 — iOS (App Store) & Lojas Secundárias (Windows Store / Launcher próprio se necessário):** Etapa posterior após estabilização do pacote Android e aquisição/configuração de ambiente de assinatura Apple.
- **Modo Offline vs. Online (Laboratório vs. Produção):**
  - *Fase de Laboratório / Protótipo Local:* **Offline obrigatório**, sem tela de login, permitindo testar combate (Equipes e Modo Clássico), Hub, vídeos e save local sem internet.
  - *Versão Comercial Publicada:* Operará como **jogo conectado com validação autoritativa no servidor** para proteger progressão, invocações, moedas, as **5 Lojas Separadas**, o **Modo DJ** e o **PvP**, mantendo acesso offline apenas para leitura local do *Guia dos Livros* já desbloqueados e ajustes de configuração.
- **Conta, Sincronização e Multiplayer:**
  - *Conta e Cross-Save:* Conta única do jogador vinculada a provedores padrão (Google, Apple, Steam/Epic ou e-mail) para **sincronização de progresso entre PC e Android** (`Cross-Progression`).
  - *Multiplayer:* **PvP Assíncrono** (Arena / Campeonato contra equipes de defesa configuradas por outros jogadores, estilo *Point War*) como foco competitivo principal, evitando a complexidade e latência crítica de PvP em tempo real na primeira versão pública.

---

## 2. Escopo de Lançamento, Hardware Alvo, Classificação Etária e Suporte
**DECISÃO DESEJADA / PROPOSTA (Diretrizes de Produto):**

- **Escopo de Lançamento em Fases:**
  - *Marco 1 (Demo / Vertical Slice Offline):* Hub funcional (`16:9` e `9:16`), vídeo de abertura, tutorial jogável do Capítulo 1, 1 arena (*Terraço da Vigília*) e elenco piloto de 5 personagens.
  - *Marco 2 (Lançamento Comercial v1.0):* Livro 1 completo (15 capítulos) + capítulos iniciais dos demais livros, sistema de **Linhas Narrativas** e **Fases de Sintonia Elemental**, eventos em **Modo Clássico (PS1/PS2)**, **Modo DJ (`EXP-01`)**, **Arena PvP** e **5 Lojas Separadas**.
  - *Marco 3 (Expansões Contínuas / Live Ops):* Conclusão dos 67 capítulos dos **4 Livros**, chefes de múltiplas frentes (`SEQ-01` / `PAR-02` / *Treva Fluxo*) e campanhas comunitárias de música e arte.
- **Faixas de Hardware Alvo:**
  - *PC (Windows 10/11 64-bit):* Mínimo: 8 GB RAM, GPU dedicada de entrada (ex.: GTX 1050 Ti / equivalente) no preset *Baixo/Desempenho*; Recomendado: 16 GB+ RAM, RTX 3060 ou superior para presets *Alto / Ultra HD / HDR*.
  - *Mobile (Android 11+ / iOS):* Mínimo funcional: 6 a 8 GB RAM (validação base no Android Studio AVD 8 GB RAM) com escalabilidade gráfica reduzida e `30 FPS` estáveis; Recomendado: aparelhos intermediários/topo de linha atuais a `60 FPS`.
- **Classificação Indicativa Alvo e Tratamento de Temas Sensíveis:**
  - **Faixa Etária Alvo:** **16+ anos** (ClassInd Brasil `16` / PEGI `16` / IARC equivalente), calibrada pelo conteúdo mais intenso da **Linha Sombria** (uma vez que as lojas classificam o produto inteiro pelo pico de intensidade, mesmo em capítulos pontuais).
  - **Descritores Previstos no Questionário IARC / ClassInd:**
    - *Violência / Armas / Sangue:* Confrontos armados, lutas intensas e presença pontual de sangue e sofrimento dramático (sem gore gratuito ou violência sexual).
    - *Temas Sensíveis / Saúde Mental:* Presença pontual de angústia psicológica e sugestão simbólica de autoextermínio dentro de um pesadelo narrativo — tratada estritamente sem representação gráfica/detalhada do ato, acompanhada de **aviso opcional de conteúdo sensível** antes do capítulo correspondente.
    - *Diversidade e Temática LGBT+:* Representatividade natural de personagens assumindo sua identidade e vivências afetivas/sociais (de forma explícita ou reservada conforme o conto), sem conteúdo sexual explícito, mantendo total conformidade com as políticas do Brasil, Américas e Europa (com declaração de metadados regionais onde exigido pelas lojas).
- **Idiomas:**
  - *Lançamento Base:* **Português do Brasil (`pt-BR`)**.
  - *Expansão Internacional:* **Inglês (`en`)** estruturado via `StringTables` (seguido por Espanhol `es-LATAM` conforme demanda).
- **Necessidades de Suporte e Conformidade:**
  - Canal de suporte ao jogador (e-mail/formulário integrado no menu *Configurações*), Política de Privacidade e Termos de Uso claros (conformidade com **LGPD** no Brasil e **GDPR** internacional), e mecanismo de exclusão de conta/dados exigido pela Google Play e App Store.

---

## 3. Inventário de Autoria, Licenças e Restrições por Recurso
**REFERÊNCIA / AUDITORIA DE PROPRIEDADE INTELECTUAL (Sem Credenciais ou Documentos Pessoais):**

| Categoria de Recurso | Origem e Autoria | Situação de Licença e Restrições | Ação Necessária Antes de Publicar |
| :--- | :--- | :--- | :--- |
| **Obra Literária, Roteiros e Universo** (*Contos de Um Lobo na Cidade* — 4 Livros) | Autoria exclusiva de **Felipe Sena Pereira**. | **100% Direitos Autorais do Autor.** Manuscritos integrais ficam fora do repositório público. | Manter registro de autoria literária arquivado localmente. |
| **Código-Fonte C++20 e Projeto Unreal Engine** | Desenvolvimento próprio do projeto (`LOBO` / *Mundus Lupis Disco*). | **100% Autoral**, sob termos padrão de licenciamento comercial da **Unreal Engine 5.8.4** (Epic Games). | Garantir que nenhum código ou asset protegido de terceiros entre no build. |
| **Tipografia (`Cormorant Garamond` e `Lora`)** | Google Fonts. | **SIL Open Font License (OFL 1.1)** — livre para uso e empacotamento comercial. | Incluir o arquivo `OFL.txt` nos créditos/licenças do jogo. |
| **Panorama do Hub e Vídeos de Abertura** | Criados/dirigidos pelo próprio autor em seu fluxo de geração visual. | Uso liberado no projeto; originais preservados em `assets/sources/`. | Manter termos de uso comercial das ferramentas de geração arquivados. |
| **Silhueta Provisória do Lobo (`klemzo123_silhouette-wolf-2...png`)** | Obtida do site do autor em 2018 (origem de terceiro/banco). | **Distribuição comercial NÃO auditada** (uso restrito como placeholder local). | **Substituição obrigatória** por logotipo vetorial 100% autoral antes de qualquer build comercial. |
| **Modelos 3D, Texturas e Animações (`ART-01`)** | A produzir via fluxo autoral + ferramentas *Image-to-3D* com plano comercial. | Exige licença comercial ativa da plataforma de geração utilizada. | Arquivar comprovante de licença comercial em `assets/sources/` por lote gerado. |
| **Músicas, Efeitos Sonoros e Vozes (`AUD-01`)** | Composições próprias ou bibliotecas comerciais licenciadas. | Proibido usar áudios extraídos de outros jogos. | Arquivar certificados/notas de licença por faixa antes do empacotamento. |

---

## 4. Requisitos Funcionais de Serviços Futuros (Sem Travar Fornecedor)
**PROPOSTA (Especificação Técnica Agnóstica — Implementação Apenas Sob Contrato Futuro):**

Caso o projeto avance para operação online, o backend precisará atender apenas às seguintes funções essenciais, mantendo o núcleo de regras C++20 desacoplado de fornecedores específicos:
1. **Autenticação e Identidade (`Auth`):** Gerar um `PlayerID` único e vincular tokens de login (Steam, Epic, Google Play, Apple, E-mail) sem expor dados sensíveis no cliente.
2. **Persistência em Nuvem e Inventário (`Cloud Save & Economy`):** Armazenar estado da conta (progresso nos 67 capítulos, moedas separadas, equipamentos, expansão de slots, músicas do Modo DJ) com controle de versão (`SaveSchemaVersion`) e resolução de conflitos.
3. **Validação Autoritativa de Resultados (`Match & Rhythm Verification`):** Receber o relatório/replay determinístico das batalhas (PvE, Clássico, PvP) e sessões do Modo DJ para validar integridade antes de creditar recompensas ou atualizar o ranking diário de PvP.
4. **Catálogo Remoto e Lojas (`Live Config & Shops`):** Gerenciar a rotação automática de 2 horas da **Loja de Ouro**, os estoques das demais 4 lojas, a conversão de excedentes e a ativação de eventos sazonais sem exigir atualização binária do executável a cada evento.
5. **Telemetria Mínima de Estabilidade (`Crash & Performance Logs`):** Coleta anônima opcional de falhas de execução e métricas de FPS para diagnóstico de compatibilidade em PC e Android.