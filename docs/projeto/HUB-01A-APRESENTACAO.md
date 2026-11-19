# HUB-01A — base de apresentação, imagens e vídeo inicial

19/11/2026. Implementação autorizada enquanto o usuário produz o panorama e seleciona o vídeo. Esta entrega prepara lógica/dados; não é o hub gráfico pronto nem um player de mídia funcionando no aparelho.

## Requisitos recebidos e registrados

- Um panorama pode abastecer as três áreas; cada enquadramento mostra uma região sem duplicar a regra de navegação.
- PC largo pode mostrar o panorama completo; janela estreita/retrato pode focar uma área.
- O usuário define janela/resolução ou tela cheia antes de criar a janela principal. Preferências devem ser aplicadas antes da abertura, não por mudança brusca após carregar.
- Vídeo horizontal no celular solicita apresentação em paisagem; ao terminar/pular/falhar, deve retornar à orientação anterior do hub.
- Vídeo vertical no PC fica centralizado, sem esticar nem cortar; arte lateral compõe o espaço restante.
- Podem existir vídeos diferentes para cada dispositivo; preferir a variante do perfil, usando a outra como alternativa quando disponível.
- Se rotação não puder ser aplicada, manter proporções com faixas na orientação disponível. O conteúdo não deve ficar inacessível.
- Falta/erro de vídeo não bloqueia a entrada local; pular, legendas e restauração de estado fazem parte do contrato do player futuro.

Essas regras de vídeo/tela foram solicitadas diretamente pelo usuário. Avatar, destinos funcionais, provedor de conta e lojas do texto anterior ainda mantêm suas questões próprias.

## O que foi implementado

| Parte | Estado real |
|---|---|
| Enquadrar mídia preservando proporção | Implementado em contain; sem cortar/esticar vídeo |
| Regiões do panorama | Recortes UV normalizados, três regiões configuráveis, centro inicial |
| Foco/panorama | Auto por perfil/aspecto ou preferência explícita; janela intermediária usa foco |
| Vídeo vertical no PC | Retângulo central e áreas laterais calculados; sinalização de composição lateral |
| Vídeo horizontal no celular | Gera pedido Landscape quando necessário; também há pedido Portrait para variante vertical |
| Rotação negada/indisponível | Função mantém enquadramento proporcional no viewport atual |
| Variante por dispositivo | Preferência Desktop/Mobile, fallback para variante existente; nenhuma mídia retorna nulo |
| Janela inicial | Estrutura e validação para resolução/mode; não cria janela nem selecionador de preferências |
| Catálogo de recursos | JSON versionado e validador de metadados/arquivo Ready local; quatro entradas Planned |
| Compilação Unreal | Fonte neutro incluído no preparador do módulo;34 entradas canônicas no stage |

Fonte: include/rpg/startup_presentation.hpp e src/startup_presentation.cpp. Nenhuma regra de combate foi alterada. Os nomes HubView/HubArea são separados de CombatProfile/ArenaLayout.

O limiar Auto de1,5 e os tamanhos1280×720/3240×1920 são valores de trabalho configuráveis, não obrigação de produto. Retrato/paisagem não determinam tipo de entrada; o adaptador futuro tratará mouse/toque/controle separadamente.

## Limites da entrega HUB-01A (atualização HUB-01B abaixo)

- Leitura automática do JSON pelo runtime Unreal e conversão para as estruturas C++; atualmente catálogo e política têm contratos compatíveis, mas não estão ligados por parser.
- Importação de arquivos para Content/pacote, carregar uma única textura real e efetivamente desenhar seus recortes.
- HUD do hub, áreas clicáveis, animação de câmera e transição hub↔arena.
- Player/decoder, descoberta de resolução/orientação reais do vídeo e compatibilidade de codec.
- Aplicação da orientação pelo sistema do celular e retorno após término/pular/erro. O plano emite intenção; não gira bytes do vídeo nem garante mudança física do aparelho.
- Aplicação das preferências antes da criação da janela principal; seletor externo ou preferência salva ainda precisam ser ligados. Não reutilizar o save de progresso de combate para isso.
- Renderização da arte lateral, legendas, acessibilidade e botões do player.
- Teste real de mobile, memória de textura, tempo de carregamento, decodificação, áudio, temperatura e consumo.

Se a rotação for aceita, o adaptador deve aguardar o novo tamanho/área segura informado pelo aparelho e recalcular. Enquanto isso, o plano atual já fornece enquadramento proporcional de fallback. Guardar a orientação de entrada na sessão do player para restaurá-la depois, sem sobrescrever esse registro a cada redimensionamento.

## Receber os arquivos

Catálogo: [startup.json](../../data/presentation/startup.json). Organização e direitos: [assets/README.md](../../assets/README.md).

- HubPanorama: fundo contínuo; regiões Left/Center/Right em coordenadas0–1.
- IntroDesktop: vídeo horizontal.
- IntroMobile: vídeo vertical opcional, pode ter conteúdo distinto.
- SideArt: arte que compõe o desktop quando o vídeo é vertical.

Para iniciar com três faixas iguais em retrato9:16,3240×1920 é uma referência útil (3×1080×1920), não resolução mínima obrigatória. As regiões são configuráveis se a arte tiver outras divisões. No panorama largo, a imagem inteira é enquadrada preservando proporção; diferenças de aspecto deixam áreas de composição em vez de deformação.

Preferir fundo sem números, textos, logotipo de terceiros ou botões de interface; HUD e rótulos serão separados para localização e adaptação. A imagem pode ser cenário2D/2.5D; profundidade real e câmera3D exigem modelagem própria posterior.

Entradas Planned não precisam de arquivo. Ao receber material, preencher origem/direitos, dimensões reais e caminho local antes de marcar Ready. O validador verifica catálogo e presença de arquivos Ready, não licencia conteúdo nem decodifica vídeo. Direitos e metadados declarados precisam de revisão. Vídeos/fontes grandes não serão adicionados ao Git sem definir estratégia.

## Testes e relatórios

Executar scripts/testar-apresentacao-inicial.ps1:10 grupos nativos /W4 /WX, aprovados. Cobrem paisagem no desktop; retrato central com laterais; pedido/fallback/reenquadramento após rotação mobile; retrato mobile; três regiões; panorama/foco e regiões personalizadas; resize; variantes; caminhos relativos; configurações inválidas. [Resultado](../relatorios/atuais/HUB-01A.json).

Executar scripts/validar-recursos-apresentacao.ps1: catálogo aprovado com4 recursos Planned e0 Ready. Nenhum arquivo de mídia foi aberto/decodificado; dimensões ainda são declarações/exemplos. [Resultado](../relatorios/atuais/HUB-01A-CATALOGO.json).

[Build Unreal](../relatorios/atuais/HUB-01A-BUILD.json) registra compilação da base no módulo, sem ligação ao renderer/player, janela aberta ou pacote novo. Não confundir com funcionamento em telefone.

A bateria de combate anterior não foi repetida: não houve mudança em seus fontes/contratos. A nova suíte é independente e não aumenta artificialmente o número de testes de gameplay.

## Próxima integração

Após receber arte/vídeo e autorização específica de abertura, escolher uma primeira ligação gráfica: carregar o panorama e alternar regiões com HUD independente; depois integrar o player e suas preferências de inicialização. Dados/câmera/vídeo usam esta mesma política em vez de duplicar fórmulas por tela. Backend, autenticação real e SDKs de lojas continuam fora desta entrega.

## Recebimento posterior

Mídias recebidas e catálogo atualizado com nomes/dimensões reais,4 Ready por presença. Autoria própria declarada pelo usuário. [Conferência e limitações](HUB-01A-RECURSOS.md). A marcação anterior4 Planned/0 Ready descreve o catálogo inicial. Player, decodificação em engine e rotação real continuam pendentes.

## Integração posterior por código

HUB-01B escreveu o parser do subconjunto hub, carregamento do panorama, HUD e rotas locais; compilação sem execução. [Estado preciso](HUB-01B-IMPLEMENTACAO.md). Vídeo, preferências de janela e orientação nativa continuam pendentes. A lista anterior preserva o escopo da fundação, não deve ser usada para dizer que ainda falta escrever esse parser do hub.
