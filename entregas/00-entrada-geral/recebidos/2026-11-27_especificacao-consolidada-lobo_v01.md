# Especificação Arquitetural, UX e Roadmap Técnico — Mundus Lupis Disco (LOBO)

**Versão de Referência do Código:** `v0.28-pre-alpha` (Alvo de Entrega: `v1.0.0`)  
**Objetivo do Documento:** Consolidar as diretrizes de design, arquitetura de software, requisitos de segurança, infraestrutura e sistemas temáticos aprovados até o momento, servindo como base de análise técnica para o agente de desenvolvimento antes da geração dos contratos finais de implementação.

---

## 1. Contexto Atual do Projeto (`v0.28-pre-alpha`)

O projeto encontra-se em fase de protótipo funcional com foco em combate e fluxos de laboratório:
* **Ponto de Entrada no Hub:** No panorama principal (`startup.json`), o acesso ao **Terminal** está ancorado na região central superior (`x: 0.56, y: 0.40`).
* **Navegação Provisória:** Enquanto a interface definitiva do Terminal não é integrada, o código utiliza um menu provisório paginado (3 páginas) contendo modos de teste (*Laboratório de equipe*, *Ritual local*, *Ondas consecutivas*, *Ritual — fases do chefe*, *Tutorial padrão — A Bola Perdida*, *Pontuação*, *Sobrevivência*, *3 equipes* e *3 frentes*).
* **Economia Provisória:** Existe apenas uma recompensa de teste entregando **10 créditos** após vitórias por eliminação. Inventário, lojas, experiência, drops de equipamentos e moedas comerciais ainda não estão implementados no código.

---

## 2. Direção Visual, UX e Anatomia Técnica (Terminal Urbano 2D)

### 2.1. Identidade Visual e Layout (Mobile-First / Estilo Limpo)
* **Atmosfera:** Estética urbana noturna contemporânea inspirada em São Paulo e Rio de Janeiro (tons de azul-noturno, ardósia e prata metálico), com acentos neon discretos e minimalistas. Evitar poluição visual e excesso de notificações (*red dots*).
* **Estrutura em 4 Faixas:**
  1. **Barra Superior Compacta:** Exibe apenas recursos essenciais em escala reduzida e discreta (**Ouro**, **Gemas**, **Notas Musicais**) e botão de menu.
  2. **Cabeçalho:** Título principal (`TERMINAL URBANO`) e subtítulo funcional (`Destinos e Modos Internos`).
  3. **Lista Vertical Rolável de Bilhetes:** Cartões horizontais no formato de bilhete ferroviário com picote lateral, ilustração do modal à esquerda, título/descrição/recompensas ao centro e botão de ação (`EMBARCAR` / `ENTRAR`) à direita.
  4. **Barra Inferior de Navegação:** Retrato circular do personagem ativo à esquerda e abas principais (`ROTAS`, `TRILHAS`, `EXPEDIÇÃO`).
* **Recompensas Fluidas:** Dentro de cada cartão, os ícones de recompensa devem flutuar em uma **única linha horizontal contínua**, sem caixas/divisórias individuais ao redor de cada ícone e sem legendas de texto abaixo deles.

### 2.2. Mapeamento Oficial dos Modais e Código de Cores
As cores abaixo identificam cada modal tanto nos cartões 2D quanto na sinalização luminosa do cenário 3D:

| Modal / Destino | Cor Temática | Posição na Composição | Modo de Jogo e Função | Observações de Migração |
| :--- | :--- | :--- | :--- | :--- |
| **Monotrilho** | Roxo | Superior esquerdo | Rotas especiais, expedições e modos alternativos / PvE | Absorverá *Ritual*, *Sobrevivência* e *3 Frentes* |
| **Trem Regional** | Azul Cobalto | Andar intermediário (saída à direita) | **Modo História** (trilhas, livros e capítulos) | Acesso principal à progressão narrativa |
| **Ônibus** | Verde Esmeralda | Térreo (fundo central) | **Bosses / Chefes de Equipamentos** | Equivalente aos *Guardiões do Milagre* |
| **Metrô** | Laranja | Térreo (escadas à direita) | **Farm Rápido, Recursos e Torre / Provações** | Suporta badges dinâmicos (ex.: `Drops +200%`) |
| **Loja de Conveniência** | Vermelho | Térreo (esquerda) | Reserva da estação, consumíveis e utilidades | Área modular expansível |

### 2.3. Separação entre Arte Estática (2D) e Lógica de Engine
Para manter o aplicativo leve e data-driven:
* **Camada de Arte 2D (Assets Estáticos):** Cenário urbano noturno de fundo, ilustrações dos 5 modais, moldura base dos cartões-bilhete configurada em **9-Slice** (para esticar sem distorcer e receber tintura de cor por código), retrato e moldura do personagem, e micro-ícones limpos de itens (`32x32` ou `64x64` px, sem texto embutido).
* **Camada de Engine e Código (UI Dinâmica):**
  * Instanciação e rolagem via `Scroll View` + `Vertical Layout Group` clonando um único `Prefab` de cartão.
  * Todos os títulos, descrições e valores renderizados via fontes digitais e tabelas de localização (nunca achatados nas imagens).
  * Barra superior conectada reativamente ao estado do jogador.
  * Geração dinâmica dos micro-ícones de recompensa via `Horizontal Layout Group` lendo a tabela de drops da rota.
  * Estados de botão (`Normal`, `Pressionado`, `Bloqueado`), badges de evento e partículas/shaders sutis.

---

## 3. Arquitetura Híbrida Multiplataforma (3D Desktop / 2D Mobile)

### 3.1. Desacoplamento (`TerminalCore` vs. Camada Visual)
* **Núcleo Único (`TerminalCore` — Padrão MVVM):** Toda a lógica de desbloqueio de rotas, multiplicadores ativos, custos, requisitos e disparo de embarque reside em uma camada pura de dados e regras, agnóstica à interface.
* **Desktop (PC Windows):** Carrega por padrão a cena do **Terminal Multimodal em 3D** (estação noturna interativa com os 5 pontos de embarque), mas oferece um **atalho rápido opcional** (ex.: tecla `Tab` ou `M`) para abrir a tela 2D de bilhetes por cima do cenário, evitando fadiga de deslocamento durante o farm diário.
* **Mobile (Android / iOS):** Carrega por padrão a **tela 2D de cartões-bilhete**, priorizando ergonomia de toque, leitura imediata, economia de bateria e estabilidade térmica.

### 3.2. Entrega Modular de Recursos (`Addressables` / `Asset Bundles`)
* **Instalação Inicial Enxuta:** O pacote base publicado nas lojas mobile contém apenas o núcleo do jogo e os assets 2D do Terminal Urbano.
* **Diagnóstico Local de Hardware na Primeira Abertura:** O cliente verifica localmente RAM disponível, GPU e espaço livre em armazenamento para recomendar ao jogador o modo adequado (**Modo Leve 2D** ou **Modo Completo 3D**).
* **Gerenciamento Sob Demanda nas Configurações:**
  * O jogador mobile pode baixar e instalar o pacote externo do **Terminal 3D** a qualquer momento.
  * O jogador pode **apagar os arquivos externos do pacote 3D** dentro das configurações do jogo para liberar espaço e retornar ao modo 2D sem precisar reinstalar o aplicativo.
  * A troca entre 2D e 3D deve descarregar a cena atual e carregar a nova em tempo de execução sem exigir reinicialização forçada do app.
  * Mesmo com o pacote 3D instalado no celular, um botão discreto de **Acesso Rápido 2D** permanece disponível na HUD.

---

## 4. Sistemas Temáticos: Música, Palavra, Narrativa e Economia

### 4.1. Diretrizes Terminológicas (Identidade Própria)
Para evitar sobreposição direta com mecânicas de *Dislyte* (como a passiva *Cadência* ou o kit literal de *Norah*), o projeto adotará terminologia própria conectando música, cultura do disco, literatura e transporte urbano:
* **Eixo Musical e Sonoro (Efeitos, Habilidades e Rewards):**
  * **Compasso** / **Andamento:** Mecânicas rítmicas de turno, contagem cíclica de ações e manipulação de velocidade/barra de ação.
  * **Ressonância** / **Acorde:** Sinergias entre personagens ou fragmentos combináveis no inventário.
  * **Selo** / **Rotação** / **Sulco:** Medidores de acúmulo em combate ou itens raros de recompensa ligados à estética de disco/vinil.
* **Eixo da Palavra (Fala, Prosa e Poesia):**
  * **Eufonia:** Harmonia sonora aplicada a efeitos de suporte, mitigação, sinergia vocal ou essências de recompensa lírica.
  * **Prosódia:** Ritmo, entonação e pausa da fala/verso aplicados a habilidades de comando vocal, quebra de postura ou ordem de impacto.
  * **Candidatos Complementares:** *Verbo* (habilidades de comando), *Métrica* / *Estrofe* (etapas de combo poético) e *Fólio* (itens de desbloqueio de prosa/histórias paralelas).
* **Eixo Narrativo (Sequência de Livros, Contos e Histórias — Substituindo "Cronologia"):**
  * Candidatos principais em avaliação para nomear a estrutura dos livros e arcos: **Antologia**, **Cânone**, **Compêndio**, **Códice**, **Coletânea / Álbum** (ponte musical) ou **Itinerário / Linha** (ponte com o Trem Regional).

### 4.2. Estado da Economia e Inventário (Em Definição)
* **Famílias de Moedas Planejadas:** Ouro, Gemas, Fichas Clássicas, Notas Musicais/Dados e Pontos de PvP.
* **Pendências:** Definição das tabelas de drop por modal, curvas de experiência, custos de evolução, câmbio entre lojas e expansão de slots de inventário.

---

## 5. Segurança, Privacidade Global (Compliance) e Cupons Externos

### 5.1. Conformidade Legal Internacional
A arquitetura de dados e contas deve atender às legislações aplicáveis conforme a localização do jogador:
* **Escopo Regulatório:** **LGPD** (Brasil), **GDPR** (União Europeia e Reino Unido), **CCPA/CPRA** (Califórnia/EUA), **COPPA** (menores de 13 anos nos EUA), além de estrutura preparada para **PIPA** (Coreia do Sul) e **APPI** (Japão).
* **Minimização de Dados:** O diagnóstico de hardware para recomendar o pacote 2D ou 3D deve rodar **100% localmente no dispositivo**, sem transmitir *fingerprinting* de hardware ao servidor sem consentimento explícito.
* **Triagem Etária (*Age Gate*) e Proteção de Menores:** Verificação de idade no primeiro acesso/cadastro, aplicando bloqueio de coleta comportamental, restrições de comunicação/exposição de perfil e exigência de consentimento parental verificável conforme a maioridade digital local (13 anos no COPPA, 13 a 16 no GDPR e regras específicas para menores de 18 na LGPD).
* **Gestão de Consentimento (CMP):** Consentimento granular antes de inicializar SDKs de *analytics*, *crash reporting* ou rastreamento, com acesso permanente aos Termos de Uso e Política de Privacidade.
* **Direitos do Titular e Exclusão Nativa de Conta:** Fluxo obrigatório dentro das configurações do jogo (exigência legal e das lojas Google Play / App Store) para consulta/exportação de dados e **exclusão definitiva** da conta, credenciais 2FA, nicks vinculados e registros pessoais em banco e backups.
* **Segurança Técnica e Cross-Save:** Criptografia em trânsito (TLS) e em repouso, separação estrita entre **Nick Público** e **ID Interno Privado**, autenticação de dois fatores (2FA), logs de auditoria de consentimento/revogação e salvaguardas para transferência internacional de dados.

### 5.2. Sistema Criptográfico de Cupons (Amazon e Mercado Livre)
* **Gatilho:** Conclusão de 100% de um marco narrativo integral (ex.: finalizar um livro completo com todas as histórias principais e secundárias cabíveis).
* **Recompensa:** Emissão de um **código único e aleatório** conversível em cupom de desconto para produtos/livros oficiais na **Amazon** e no **Mercado Livre**.
* **Arquitetura Antifraude:**
  * Geração estritamente **server-side** com alta entropia criptográfica (nunca no cliente).
  * Vínculo único ao ID privado da conta (limite de 1 emissão por livro/conta).
  * Exigência de conta verificada (e-mail confirmado e/ou 2FA ativo) e validação de integridade da progressão antes da emissão.

---

## 6. Infraestrutura, Requisitos de Hardware e Roadmap de Expansões

### 6.1. Estratégia de Implantação e Escalabilidade
* **Fase 1 — Ambiente Local de Testes:** Stack containerizada via Docker (API + PostgreSQL + cache em memória) para desenvolvimento e testes em rede local sem custo de nuvem.
* **Fase 2 — Alpha/Beta em Servidores Pequenos ou Tiers Gratuitos Seguros:** Uso de serviços gerenciados com camada gratuita ou VPS enxutos (ex.: Supabase, Firebase, PlayFab, Nakama ou VPS Linux) protegidos desde o dia zero com HTTPS/TLS obrigatório, proxy reverso/túnel seguro (Cloudflare), isolamento de variáveis de ambiente, *rate limiting* e backups automatizados.
* **Fase 3 — Escala Pós-Lançamento:** Cliente desacoplado via interface única (`IBackendService`), permitindo migrar para servidores *stateless* atrás de balanceador de carga, réplicas de leitura no banco, Redis para sessões/rankings e CDN dedicada para distribuição dos pacotes modulares (`Addressables` 2D/3D).

### 6.2. Exigências Mínimas de Hardware até o Lançamento (`v1.0.0`)
* **Orçamento de Desempenho:** Compressão de texturas (ASTC no mobile, BC7 no desktop), controle rigoroso de *draw calls*, carregamento assíncrono e monitoramento contínuo de pico de RAM.
* **Mobile — Modo Leve (2D Padrão):** Android 10 ou superior / iOS 14 ou superior, 3 GB a 4 GB de RAM mínimos, GPU com suporte básico a OpenGL ES 3.0 ou Vulkan, instalação inicial enxuta apenas com o pacote 2D.
* **Mobile — Modo Completo (Terminal 3D Opcional):** 6 GB ou mais de RAM recomendados, GPU moderna com suporte estável a Vulkan ou Metal, verificação prévia de armazenamento livre antes do download do bundle 3D.
* **Desktop (PC Windows — Terminal 3D + Atalho 2D):** Windows 10/11 64-bit, processador quad-core, 8 GB de RAM mínimos (16 GB recomendados), GPU dedicada de entrada ou integrada moderna (DirectX 11/12 ou Vulkan) e SSD.

### 6.3. Direções para Futuras Expansões (Pós-`1.0.0`)
* **Malha Ferroviária Orientada a Dados:** Adição de novas linhas urbanas, ramais metropolitanos, trens regionais e conexões interestaduais por catálogo remoto no servidor, sem exigir atualização binária nas lojas.
* **Evolução do Terminal 3D:** Ciclos de horário, clima dinâmico (chuva noturna, neblina), eventos sazonais e pontos interativos de narrativa na estação.
* **Social e Telemetria:** Base preparada para expedições cooperativas, desafios semanais, rankings sazonais, coleção de discos/trilhas sonoras, telemetria anônima compatível com LGPD/GDPR e opções ampliadas de acessibilidade.

---

## 7. Diretrizes de Análise Solicitadas ao Agente

Antes de iniciar a escrita de código de produção, o agente deve analisar este documento frente ao repositório atual (`v0.28-pre-alpha`) e apresentar um parecer técnico cobrindo os seguintes pontos:

1. **Auditoria de Acoplamento Atual:** Avaliar como as telas e menus provisórios do `v0.28-pre-alpha` estão estruturados hoje e propor a refatoração para isolar o `TerminalCore` (dados/regras) das camadas visuais 2D e 3D.
2. **Arquitetura de Pacotes Modulares (`Addressables` / `Asset Bundles`):** Definir a árvore de pastas, grupos de build e fluxo de download, verificação de integridade, troca em runtime (2D <-> 3D) e limpeza segura de armazenamento no mobile.
3. **Critérios de Diagnóstico Local de Hardware:** Propor a tabela de thresholds (RAM, modelo de GPU/API gráfica e espaço livre) que acionará a recomendação automática entre Modo Leve 2D e Modo Completo 3D na primeira inicialização.
4. **Design de Navegação do Terminal 3D (Desktop):** Comparar e recomendar o modelo de câmera e interação no cenário 3D (movimentação livre do personagem vs. câmeras cinematográficas com transição suave entre os 5 setores) e o gatilho do atalho rápido 2D.
5. **Contratos de Dados (JSON / Banco) para Economia, Narrativa e Expansões:** Estruturar os esquemas de dados para moedas, drops dinâmicos (sem texto fixo), livros/histórias (verificação de conclusão 100%) e expansão remota da malha ferroviária.
6. **Arquitetura de Backend, Segurança e Cupons:** Recomendar a stack inicial para testes locais (Docker) e Alpha gratuito/seguro, detalhando o fluxo de *Age Gate*, consentimento LGPD/GDPR, rotina de exclusão definitiva de conta e emissão criptográfica de cupons Amazon/Mercado Livre.
7. **Plano de Documentação Modular (`.md`):** Propor a divisão final dos documentos técnicos especializados (ex.: `terminal_core.md`, `ui_2d_mobile.md`, `scene_3d_desktop.md`, `security_compliance_coupons.md`, `economy_narrative_schema.md`) e a ordem recomendada de implementação até a `v1.0.0`.