> **Mapa editável do Hub 3D — adiado em 28/11/2026.** Retomar quando solicitado. O molde com acabamentos e sinalização ambiental aprovados serve de base. O primeiro corte do TerminalCore está integrado e aguarda reteste em jogo.

> **Prioridade vigente — 27/11/2026.** Retomar mais adiante os pacotes Windows/Android com o Terminal 2D, o reteste de navegação e toque do Terminal e o acabamento de apresentação do combate sobre AUT-13. O aceite de enquadramento do Terminal no RpgEngineProbe segue registrado. Os acabamentos provisórios dos sete destinos do Hub 3D foram compilados e receberam aceite visual. [Registro do Hub 3D](HUB-PREVIA-3D.md). A sinalização ambiental do Hub 3D recebeu aceite visual em 28/11/2026. A presença 3D do Felipe, inclusive o placeholder, foi adiada. Para o mapa editável, falta definir o formato. O catálogo e o retorno do Monotrilho já estão integrados no TerminalCore.

> **Terminal 2D — 27/11/2026.** A prévia funcional dos cinco bilhetes está implementada. Monotrilho abre os laboratórios e o tutorial já disponíveis. A seleção do layout conforme o parâmetro de apresentação foi aceita no RpgEngineProbe. O reteste de toque em um novo APK segue pendente. Camadas separadas, retrato dinâmico e economia ficam para o acabamento. [Recebimento e critérios de teste](2026-11-27_terminal-2d-referencia-visual_v01.md).

> **27/11/2026 — Terminal e arquitetura.** A [revisão técnica](2026-11-27_parecer-arquitetura-lobo_v01.md) separa o código existente das novas frentes. AUT-13, QA em janela e Hub 3D provisório seguem disponíveis com os recursos atuais. A revisão orientou os primeiros cortes do Terminal. Os novos pacotes seguem em etapa futura. A interface final do Terminal depende da arte. Hardware mínimo, regras comerciais, provedor, dados de conta e cupons ainda precisam de validação ou decisão. A estimativa da demo segue em 28,43%.

> **Execução e dependências.** O próximo trabalho independente é a apresentação de combate sobre AUT-13. Validação em janela e refinamento dos materiais e do movimento do Hub 3D são frentes paralelas. A arte final libera a tela do Terminal. O usuário valida o visual, a persistência e o aparelho físico depois dos roteiros. [Central de entregas e aceites](../../entregas/NECESSIDADES-ATUAIS.md).

> **Escopo da Pré-Alfa.** A fila preserva AUT-13, LAB-PVE e LAB-QA. Apresentação de combate e validação em janela avançam com os recursos atuais. Os materiais provisórios dos sete destinos antecedem o mapa editável. A tela do Terminal depende da arte final. [Diretrizes de design e execução](2026-11-23_diretrizes-desenvolvimento-lobo_v01.md).

> **Validação de 23/11/2026.** Arraste e fade dos Hubs 2D e 3D entregues e aprovados no Windows no recorte testado. Toque, rotação, vídeo e uma ação de combate validados no Pixel 7 virtual. O mapa editável, materiais finais e acabamento de combate seguem em aberto.

# Fila vigente — 23/11/2026

Esta ordem substitui as prioridades de execução do histórico abaixo. LAB-PVE-01 a 06 e LAB-QA-01, 01B e 01C já foram entregues em código. A transição por botões no panorama foi confirmada pelo usuário. As três primeiras entregas passaram pelo recorte Windows e pelo teste de toque no emulador. As próximas frentes independentes são o acabamento de combate e a preparação do mapa editável.

| Ordem | Bloco | Próxima entrega e critério |
|---|---|---|
| Validado | HUB / VIS — Fade do panorama | Fazer os letreiros da área atual desaparecerem na saída. Exibir por fade apenas os destinos visíveis depois do encaixe. Reutilizar o comportamento para botões e arraste. Retomar cliques dos letreiros quando a vista estiver estável e legível. Movimento reduzido usa troca imediata. |
| Validado | HUB / VIS — Arraste e encaixe | Permitir arraste horizontal com mouse e toque, usando o recorte atual. A imagem acompanha o gesto. Ao soltar, distância e velocidade horizontal determinam encaixe na área vizinha ou retorno à área atual. Preservar os três botões, o clique único e a distinção entre toque curto e arraste. Tratar cancelamento, perda de foco, bordas e redimensionamento. Pressão do toque fica fora do gesto. |
| Reteste pontual | VIS / QA — Validação em janela | Verificar arraste curto, rápido, cancelado e nos extremos, sequência de cliques rápidos, fade após encaixe e movimento reduzido. Compilar a prévia para aceite em janela horizontal e vertical, com teste de mouse. Calibrar toque físico na etapa mobile. |
| 1 | VIS / AUD / ENG — Apresentação de combate | Rever os eventos já expostos por AUT-13 e ligar som e animação provisórios ao início, impacto e fim da ação, sem reaplicar efeitos de combate. Arte, rig e áudio finais podem chegar em paralelo. |
| Com entrega de arte | HUB — Tela do Terminal | Integrar a composição visual quando o material chegar. Reusar as rotas existentes para laboratório, tutorial padrão e demais encontros. O trabalho de apresentação pode avançar enquanto a arte do Terminal está pendente. |

## Aceites em paralelo

Conferir os dois tutoriais, os novos modos PvE do Terminal, a navegação, a HUD em outras proporções e o registro local das conclusões pela interface. Revisitar clique, vídeo e cursor conforme regressões relatadas. O teste de falhas do backend Unreal continua planejado em ambiente isolado, separado dos saves normais.

## Hub 3D — ordem de trabalho

**Antes do mapa editável, aplicar materiais provisórios aos sete destinos do molde atual.** Usar o panorama como referência de paleta e acabamento. Começar com pedra na Biblioteca, metal e vidro no Terminal, ladrilhos na Praça e concreto e metal nos trilhos. Diferenciar também Lab - Exp, Arena PvP, Pub e Games. Conferir a leitura dos materiais no enquadramento geral e na aproximação do Terminal, com a iluminação atual. Preparar os materiais para reaproveitamento no mapa editável.

### Mapa e câmera 3D

- Validar o enquadramento com a Praça e Games mais ao centro, fachadas voltadas para o meio, trilho direito baixo e grosso, monotrilho à esquerda e Pub rebaixado à direita.
- Após validar os materiais provisórios e decidir o formato editável, preparar uma cena separada. O mapa passa a guardar as posições, rotações e escalas das peças, os pontos dos letreiros e as referências da câmera. O código usa identificadores estáveis para associar cada destino à rota existente. Os pontos de cada fachada acompanham a movimentação do respectivo grupo. O carregamento passa a usar os elementos salvos para evitar peças duplicadas.
- Depois de definir o mapa, avaliar a câmera 3D entre esquerda, centro e direita, com percurso e enquadramento por ponto de vista, aproximação do Terminal e movimento reduzido. Coordenar edições simultâneas no mesmo mapa binário.
- A câmera lateral já desliza entre as áreas. A edição do mapa e o acabamento do percurso dependem da avaliação do usuário. A apresentação de combate pode avançar em paralelo.

### Evolução visual e interativa do Hub 3D — a fazer

- **Cena e câmera.** Partir do molde atual, com a disposição espacial e as cores sólidas já existentes. Animar discretamente luzes, sinais, elementos distantes ou atmosfera. Refinar as transições já implementadas entre alas. Manter o panorama em telas largas e o foco com deslize em telas estreitas, respeitando a orientação vigente. Com Movimento Reduzido, trocar de enquadramento imediatamente e evitar balanço e animações ambientais que prejudiquem a leitura.
- **Protagonista — adiado.** Quando retomado, adicionar presença e animação básica em reação à ala ativa ou ao foco em um destino. Prever uma representação leve para o modo de baixo desempenho.
- **Leitura e interação.** Usar Dislyte como referência de clareza para uma interface urbana original em azul-noturno, preto e prata. Destacar os destinos do fundo, manter a HUD periférica limpa e dar feedback de hover, foco e clique ou toque. Preservar o cursor livre com GameAndUI e DoNotLock.
- **Limites.** Preservar as regras de combate e os fluxos vigentes. Customização completa de roupas, clima avançado e mini-player de DJ ficam fora desta etapa.
- **Aceite da entrega.** Testar navegação por botões e deslize em telas largas e estreitas. Conferir Movimento Reduzido, reação e fallback do protagonista, hover/foco, clique único, toque e saída livre do cursor. Registrar o que foi animado ou ajustado, o que ficou pendente e como reproduzir os testes.

### Modo História — piloto de trilhas a fazer

Preparar para uma etapa futura um capítulo orientado a dados com 6 a 8 nós, partes opcionais e variação entre linhas narrativas. Integrar ao combate existente e ao save isolado de QA após definir o contrato da Campanha. [Contrato proposto e critérios de aceite](2026-11-23_trilhas-modo-historia_v01.md).

### Biblioteca, Atlas e Lab - Exp — frente futura

Ao iniciar a Biblioteca, definir primeiro os contratos do Atlas Geral e das Árvores de Correlação. A Ala Privada entra como módulo experimental. O Lab - Exp mantém uma âncora independente no Hub. [Escopo e critérios de aceite](2026-11-23_arquitetura-biblioteca-atlas-lab-exp_v01.md).

### Pub e Games — frente futura

Planejar Quadro Negro, caçadas com ameaça e requisitos visíveis e instrumentos com efeitos de jogo. A posição dos dois acessos pode mudar no Hub. Balcão e consumíveis seguem em aberto. [Diretriz e aceite](2026-11-23_pub-cacadas-instrumentos-games_v01.md).

### Personagem ativo, equipamentos, Praça e PvP — design futuro

Registrar a seleção livre do personagem obtido e sua moldura no rodapé do Hub. Avaliar a personalização 2D → 3D em etapas. Planejar presets compartilhados de até cinco integrantes no jogo base, equipamentos próprios dos perfis Clássico e Padrão, PvP em tempo real no duelo e assíncrono por equipes. A Praça concentra comércio e trocas. O intervalo de 3 ou 6 horas da Loja de Ouro e os valores de câmbio ficam para calibração. [Consolidação de design v04](2026-11-23_diretrizes-telas-modos-e-evolucao_v04.md).

## Terminal e arquitetura — próximos cortes condicionados

| Frente | Preparação possível com os recursos atuais | Dependência para execução ou aceite |
|---|---|---|
| TerminalCore | [Catálogo compartilhado e retorno de sessões integrados](2026-11-28_terminalcore-mapa-navegacao_v01.md) | Reteste de navegação em jogo e toque. Trilhas e Expedição terão telas próprias em etapa futura. |
| Terminal 2D | [Piloto de QA com arte atual e dados simulados compilado em Win64](2026-11-28_terminal-2d-piloto-qa_v01.md). Aceite visual em jogo pendente. | Arte final para integração visual; economia real para valores e drops |
| Terminal 3D | [Câmera por setor e atalho 2D integrados no molde](2026-11-28_terminal-3d-setores-atalho_v01.md) | Reteste em jogo. Mapa editável e cena autoral seguem em outra etapa. |
| Pacote opcional e hardware | Definir manifesto, fluxo e matriz de testes | Prova por plataforma, aparelho físico e canal de distribuição |
| Economia e História | Modelar catálogos, conclusão por revisão e migrações | Regras de moeda e drops, capítulo piloto e critérios de conclusão do livro |
| Backend e segurança | Preparar arquitetura e casos de teste com dados sintéticos | Autorização para infraestrutura, mercado inicial, provedor e revisão de privacidade |
| Cupons externos | Definir elegibilidade, idempotência e testes usando emissor fictício | Termos de parceria e canais de resgate aprovados |

[Auditoria, critérios técnicos e ordem até a Stable](2026-11-27_parecer-arquitetura-lobo_v01.md).

## Mobile e distribuição

APK de desenvolvimento validado no Pixel 7 virtual. Próximo teste Android em aparelho físico: persistência, áudio, áreas seguras, sensibilidade do gesto e desempenho. Panorama mobile experimental. Shipping, lojas, serviços e MCP seguem frentes próprias. [Resultados Android](ANDROID-DEPENDENCIAS.md).

---

## Histórico da fila
> **Integração23/11:** recursos ligados à HUD; tutorial padrão no Terminal; busca clássica no Lab - Exp; conclusão local própria, PRAÇA/letreiros corrigidos. Código/testes/build e stage92 aprovados; aceites gráficos/I-O real/mobile separados. Demo~28% mantida. Fonte: docs/projeto/INTEGRACAO-VIS-TUTORIAIS-2026-11-23.md. Próximo: aceite da versão integrada, sem depender da arte futura do Terminal.

> **LAB-QA-01C — 22/11:** persistência compartilhada entre HUD Unreal e QA; 16/16 testes isolados, compilação e stage69 aprovados. Formatos/políticas preservados; saves normais intocados. Aceites LQA-09/10 e runtime Unreal isolado de LQA-14 continuam separados. Demo ~28% mantida. Fonte: docs/projeto/LAB-QA-01C-PERSISTENCIA-COMPARTILHADA.md. Próxima frente independente proposta: revisar contratos de áudio/animação sobre eventos existentes, sem repetir AUT-13.

> **LAB-QA-01B — 22/11:** testes de disco em pasta nova de QA, com codecs reais e backend Windows próprio; saves normais intocados. Não equivale a executar IFileManager/interface Unreal. LQA-14 de integração continua adiado. Próximo independente LAB-QA-01C: contrato compartilhado de persistência; aceites visuais em paralelo. Demo ~28% mantida. Detalhes: docs/projeto/LAB-QA-01B-IO-ISOLADO.md.

> **LAB-QA-01 — 22/11:** feedback de ações e recusas integrado; detalhes consultáveis e matriz de 16 aceites preparada. Código/testes não equivalem a aceite visual ou I/O real. Próximo independente: LAB-QA-01B, I/O em diretório isolado; usuário pode preencher a matriz em paralelo. Demo ~28% mantida. Ver docs/projeto/LAB-QA-01-FEEDBACK-E-ACEITES.md.

> **LAB-PVE-06 — 22/11:** [três frentes com chefe único](LAB-PVE-06-FRENTES-COMPARTILHADAS.md) no Terminal, página3, ataques locais e especial global. Contadores por frente e limite global, sem limite local novo. LAB01–06 em código; aceites02–06 pendentes. Próximo recomendado LAB-QA-01/aceites. Demo ~28% mantida.

> **LAB-PVE-05 — 22/11:** [três equipes sequenciais no Terminal](LAB-PVE-05-EQUIPES-SEQUENCIAIS.md), variantes de chefe fixo/faseado, HP contínuo e HUD. Janela4ações totais/equipe é fixture explícita. Aceites02–05 pendentes; próximo06frentes compartilhadas. Demo ~28% mantida.

> **LAB-PVE-04 — 22/11:** [score/sobrevivência e recibo no Terminal](LAB-PVE-04-SCORE-SOBREVIVENCIA.md). Acessos na página2; janela12ações e meta3turnos naturais são parâmetros de laboratório. Aceites visuais02–04 pendentes; próximo05equipes sequenciais. Demo ~28% mantida.

> **LAB-PVE-03 — 22/11:** [ondas e fases no Terminal](LAB-PVE-03-ONDAS-FASES.md), com HUD/contadores e consulta de transições. Nova integração aguardando aceite visual; LAB-PVE-02 também não teve aceite informado ainda. Próximo LAB-PVE-04, score/sobrevivência. Demo ~28% mantida.

> **LAB-PVE-02 — 22/11:** [Ritual local integrado ao Terminal](LAB-PVE-02-RITUAL-TERMINAL.md), com objetivo, HUD, resultado e tentativas isoladas. Código/testes/build; aceite visual da nova rota pendente. Próximo LAB-PVE-03, ondas/fases. Demo ~28% mantida.

> **LAB-PVE-01 entregue — 22/11:** [observação e contrato de HUD](LAB-PVE-01-CONTRATO-HUD.md), sem nova tela/rota habilitada. Próximo: **LAB-PVE-02, Ritual no Terminal**; não depende de nova entrega. Demais itens da fila continuam planejados. Progresso global ~28% mantido.

# Fila de desenvolvimento independente das entregas

22/11/2026. Planejamento vigente, sem executar os itens nesta revisão. Não espera arte, áudio ou pesquisa adicionais para os cortes abaixo. Cada entrega segue código → teste pertinente → compile se aplicável → documentação/commit → aceite visual posterior. Sem processo autônomo em segundo plano.

| Ordem / ID de planejamento | Família | Entrega concreta | Pode fazer sem material novo? | Dependência real / aceite |
|---|---|---|---|---|
| 1 LAB-PVE-01 | ENG / PvE / VIS | Mapear adaptadores existentes, snapshots e eventos; contrato de HUD e rota do Terminal | Sim, usando casos de código existentes | Inspeção local; sem decisão nova de combate |
| 2 LAB-PVE-02 | HUB / ENG / PvE | Um Ritual simples acessível pelo Terminal, iniciar/retornar e resultado | Sim, fixtures identificadas como teste | Depende do contrato 01; você avalia a tela depois |
| 3 LAB-PVE-03 | WAVE / VIS | Indicadores de onda/fase e resumo das transições já aprovadas | Sim | Encontro integrado em 02; conferir visualmente depois |
| 4 LAB-PVE-04 | PvE / CIC | ScoreAttack e Survival com objetivo, limite e score/recibo | Sim | Reutiliza 01–03; balanceamento final pode vir depois |
| 5 LAB-PVE-05 | SEQ / VIS | Equipe ativa, próxima equipe, transição e estado persistido entre confrontos | Sim | Snapshot/transição; aceite após implementação |
| 6 LAB-PVE-06 | PAR / VIS | Três frentes, HP compartilhado e relógio global do chefe com identificação clara | Sim, no modelo local aprovado | Depois de SEQ/HUD; não promete simultaneidade de servidor |
| Paralelo LAB-QA-01 | VIS / QA | Feedback de ações/efeitos/recusas e roteiro de validação de save/tutorial/resolução | Sim | Usa eventos e contratos existentes; aceites reais ficam com você |

IDs LAB-PVE são cortes de planejamento novos, não blocos já implementados nem nova família. Registrar a evidência individual ao entregar. Não implementar todos os menus antes de validar o primeiro corte utilizável. Não duplicar regras no HUD.

## Fora da dependência das suas entregas

Preparar validadores de catálogo já definido, contratos de recursos, eventos de áudio/animação com placeholders, cenários de teste e documentação. Produção final de assets não é necessária para esses contratos. Priorização continua na tabela, para evitar dispersão.

## Exige sua entrega/decisão para fechar conteúdo final

Amostra de personagem/rig/animação/arena final; sons e vozes finais; ícones/retratos e procedência; roteiro curto final; kits/encontros inéditos não definidos. Não inventar regras faltantes, licenças ou recursos finais. Entrega parcial por ID/versão é suficiente.

## Exige aceite posterior, não material novo

Telas novas; persistência real após reabrir; backup/recuperação; resoluções/formaçōes; tutorial específico e acessibilidade. Você pode testar pelos programas; o assistente não deve abrir telas automaticamente. Cliques/vídeo/placeholder/seletor já aceitos não voltam à fila sem regressão concreta.

## Adiado por prioridade/permissão

Android/APK e backend mobile, cook/Shipping limpo, lojas/servidor, MCP opcional e clássico ampliado. Não instalar ou mudar rede. Build sem tela é possível com editor fechado; controle visual requer nova autorização.

[Progresso ~28%](PROGRESSO-GERAL.md) · [Sua preparação](../../entregas/NECESSIDADES-ATUAIS.md) · [Famílias](MAPA-FAMILIAS-E-PRIORIDADES.md).
