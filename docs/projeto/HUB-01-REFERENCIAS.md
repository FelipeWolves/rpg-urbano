# HUB-01 — referências recebidas e análise de integração

19/11/2026. Leitura das quatro capturas anexadas e do texto enviado pelo usuário. Material de referência para um jogo original; sem copiar interface, marca, personagens, modelos ou áudio do Dislyte. Não houve navegação no cliente, consulta aos links de patch, instalação, abertura de aplicativos, alteração de código ou novo teste nesta recepção.

Texto recebido preservado em [HUB-PROPOSTA-RECEBIDA.txt](referencias/HUB-PROPOSTA-RECEBIDA.txt). Capturas mantidas em seus caminhos de origem, sem copiar as imagens para o repositório. O texto recebido preserva a transcrição original; seus dados de perfil/recursos não são adotados como dados do produto. O rótulo “Decisão” dentro do texto de outro assistente identifica o que ele propõe/consolida; não aprova automaticamente escopo comercial, tecnologia ou requisito de loja.

## Associação por conteúdo visual

| Referência no texto | Arquivo efetivamente recebido | Conteúdo observado |
|---|---|---|
| image_fa6094.jpg / captura1 | Captura de tela 2026-11-19 171306.png | Entrada/título; personagens ao fundo, triângulo luminoso, Conta, perfil/servidor, INICIAR e versão no rodapé |
| image_fa641c.jpg / captura2 | Captura de tela 2026-11-19 171434.png | Cidade/ala central; História, Praça, Eco, Provações, Show Temático e Lab Miamon; HUD periférico e barra inferior |
| image_fa6b3b.jpg / captura3 | Captura de tela 2026-11-19 171716.png | Salão interno; personagem em destaque, equipamento musical, Caçadas e Atlas; acesso de DJ parcialmente encoberto |
| image_fa6bbd.jpg / captura4 | Captura de tela 2026-11-19 171806.png | Torre competitiva; Embatidão, Guerra dos Pontos, Nocaute, Arena e recompensas |

Caminho-base das capturas: C:/Users/Ergo_/Pictures/Screenshots/. A associação usa o conteúdo, não os nomes antigos. A versão3.4.49.453824 está legível na captura de entrada; as outras não exibem rodapé de versão. Sua atribuição à mesma sessão vem do material recebido, não de um número visível em cada imagem.

Não transcrever perfil, saldos, canal ou identificadores de conta para usar como dados do produto. Os recursos visíveis são evidência de composição/contexto, não valores de balanceamento a importar.

## O que as capturas sustentam e o que não demonstram

✅ Quatro composições distintas, HUDs com estrutura visual semelhante nas três alas, recursos superiores que diferem, atalhos periféricos e navegação inferior recorrente. Salão tem personagem e objetos musicais; cidade e torre têm destinos apresentados dentro do cenário.

⚠️ Imagens estáticas não comprovam loop animado, swipe, ordem espacial das alas, botões funcionais, câmera contínua, uma única cena carregada, três planos técnicos, cache ou ausência de carregamento. “Esquerda/centro/direita” é a organização descrita na proposta. A tela não permite identificar todos os personagens/objetos com segurança; identificação nominal do personagem na sala não é necessária ao nosso requisito.

O texto “Petição de DJ” não deve virar nome de funcionalidade por transcrição automática: o acesso na imagem está parcialmente encoberto. Nosso espaço musical terá nome e identidade próprios.

## Proposta para nosso jogo, separada da referência

A direção descrita é um hub urbano com três áreas conectadas, protagonista como presença narrativa, comunicador acessível pelo cenário e pela UI, HUD independente da câmera, foco em uma área na tela estreita e possibilidade de panorama no PC. Tela de entrada levaria ao hub central; clima, cosméticos, música e transições cinematográficas seriam evoluções.

Essas escolhas não estão implementadas. Constituem proposta recebida para organizar o próximo contrato; não interpretar automaticamente todos os itens classificados como “MVP” como requisitos necessários para validar o protótipo atual.

## Compatibilidade com o projeto existente

| Tema | Reaproveitamento possível | Limite atual |
|---|---|---|
| Regras independentes da apresentação | Núcleo C++20 e adaptador visual existentes | HubState e câmera do hub não existem |
| Entrar em combate e voltar | VisualSession pode iniciar encontro e expor progresso | Fluxo hub→arena→hub e navegação de telas ainda precisam ser criados |
| Personagens/equipe/progresso | IDs, seleção e save v1 de progresso entre batalhas | Não há avatar de exploração, roupas, mapa ou save de posição no hub |
| Entrada e consulta | Experiência atual de mouse/seleção/painel ajuda a definir convenções | Não é biblioteca de UI responsiva, touch ou controle já validada |
| Layout e modo de combate | Opções já separadas | Retrato/panorama do hub não devem reutilizar CombatProfile::Classic ou ArenaLayout |
| Recursos contextuais | Modelo de apresentação pode mostrar dados locais de teste | Vigor, bilhetes, chaves, ofertas e loja não são sistemas existentes |
| Menu social/comunicador | Dois atalhos podem encaminhar à mesma ação de interface | Chat, mensagens, contas e backend não existem |
| Publicação em lojas | Separação conceitual entre produto e SDK é compatível | Nenhum SDK, provedor ou loja adicional adotado |

HUB é navegação/apresentação, não substituição do núcleo de combate. A arena continua tendo seus próprios modos/câmeras. Evitar chamar orientação retrato de “modo clássico”: clássico já identifica um perfil de combate. Nomes de proposta: HubFocused (uma área) e HubPanoramic (conjunto), a confirmar no contrato.

## Pontos que precisam de ajuste técnico no texto

1. **Panorama não é apenas afastar câmera.** Sala fechada, terraço e torre precisam ser compostos para que os três permaneçam visíveis e clicáveis. Ampliar FOV pode diminuir alvos/textos e deformar a composição. Fazer estudo com volumes provisórios antes de arte.
2. **Um hub lógico não exige todo conteúdo carregado sempre.** Separar experiência contínua de estratégia de carregamento/visibilidade permite otimização posterior, sem decidir plugins ou reconstruir três aplicações.
3. **Aspecto não determina dispositivo ou entrada.** PC pode ter janela estreita; mobile/tablet pode ter tela larga; mouse, toque, teclado e controle podem coexistir. Definir faixa intermediária1,0–1,5, redimensionamento, áreas seguras e preferência do jogador. O limiar1,5 é sugestão do texto, não valor validado.
4. **Um protagonista, posições de apresentação definidas.** Se PC mostrar as três áreas simultaneamente, definir onde o avatar fica; não duplicá-lo silenciosamente em três lugares. Animações de deslocamento/câmera são trabalho próprio.
5. **Recursos precisam de contexto mesmo no panorama.** Mostrar todos pode poluir o topo; prototipar foco, agrupamento e seleção. Não assumir moeda/vigor/bilhetes já implementados.
6. **HUD fixo não significa idêntico em qualquer tela.** Deve preservar acesso e hierarquia, admitindo reorganização, leitura ampliada e ícones/arte originais.
7. **Login visual e autenticação são entregas diferentes.** O laboratório local pode demonstrar entrada/perfil de teste sem servidor ou credenciais. Seleção real de conta/servidor e compras precisam de escopo/backend próprios.
8. **Lojas e provedores citados não estão aprovados por este documento.** Epic/Samsung aparecem além dos alvos conversados anteriormente. A afirmação de obrigatoriedade de login Apple exige verificação oficial atualizada, com suas condições/exceções, antes de integrar; não adotada como requisito automático nesta leitura.
9. **Baixo desempenho não escolhe tecnologia antecipadamente.** Pose estática/arte2D são alternativas; Live2D/Spine têm dependências/licenças próprias não avaliadas nem autorizadas. Validar primeiro solução com recursos existentes.
10. **Links de patch não especificam o hub.** Datas/mecânicas citadas no texto não foram revalidadas nesta tarefa visual e não mudam contratos de combate.

## Próxima entrega proposta — antes de arte final

Após concluir/priorizar o aceite VIS vigente, formalizar HUB-01A: mapa de destinos com IDs originais, estados de navegação, contrato de dados locais, regras de foco do avatar e enquadramento. Só então uma cena de volumes provisórios com HUD, três áreas e transição para a arena existente. Essa é uma proposta de sequência, sem iniciar código ou abrir editor nesta recepção.

Critérios da primeira verificação: entrada local→hub central→navegar três áreas→abrir/fechar painel→entrar em batalha→voltar; retrato e janela larga redimensionáveis; mesma origem de dados para botões equivalentes; sem login real, compra ou serviço de rede implícitos. Destinos ainda não implementados devem ser identificados honestamente; não prometer PvP/caçadas funcionais só pelo acesso visual.

Arte/avatar finais, animar comunicador, transições sem corte, música, climatização, economia e integrações comerciais entram em entregas separadas. Não fixar prazo/percentual de conclusão com base no número de telas.

## Dúvidas a devolver antes do contrato

- A descrição do protagonista e as três áreas refletem suas decisões finais ou ainda são proposta do outro assistente?
- No panorama, o avatar permanece no centro e os outros ambientes são visíveis, ou muda conforme a área em foco?
- Quais destinos da primeira versão precisam funcionar além da batalha existente?
- Podemos tratar login/servidor/social/lojas como apresentação local provisória até contratar esses sistemas?
- A visão tripla no PC é requisito obrigatório ou pode haver foco em uma área quando a leitura exigir?

Registrar respostas nesta seção para não misturar questões do hub com ESC/VIS de combate. Nenhuma decisão anterior foi revogada por receber o documento.

## Evolução posterior autorizada

Requisitos de panorama/vídeo por dispositivo recebidos diretamente do usuário e base neutra implementada em [HUB-01A-APRESENTACAO.md](HUB-01A-APRESENTACAO.md). Essa autorização permite a fundação antes do aceite visual, sem aprovar automaticamente todos os destinos, avatar, lojas ou tecnologias da proposta recebida. Renderer/player/rotação real ainda não ligados.
