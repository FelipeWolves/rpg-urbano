# HUB-01B — dois acessos provisórios

19/11/2026. Usuário confirmou manter somente dois acessos, sem terceiro botão por enquanto.

| Acesso | Local solicitado | Destino previsto |
|---|---|---|
| Evento Especial | Outdoor acima da cabeça do lobo | Painel provisório de evento, ainda sem conteúdo cadastrado |
| Batalha | Tela do dispositivo na mão do homem | Batalha de referência existente |

Rotas são ações internas, não links de internet. Texto adotado: “Batalha”.

## Cadastro e apresentação

data/presentation/startup.json contém hub.hotspots com IDs estáveis, rótulos, destino, âncora e área de toque em coordenadas0–1 da imagem inteira. IDs independem da arte; trocar imagem exige ajustar posições/recortes, não reescrever destinos.

Posições estimadas: outdoor luminoso sobre o lobo à esquerda e dispositivo do homem à direita. Como a imagem tem dois lobos, a escolha do outdoor à esquerda permanece provisória para aceite visual. A imagem original não foi editada.

Na ligação gráfica, textos ficarão em camada separada. Fundo, indicador e área clicável devem usar o mesmo enquadramento; partes ocultas pelo recorte e barras laterais não recebem cliques. Em tela estreita, os acessos ficam em regiões diferentes; navegação entre áreas precisa permanecer disponível. Tamanho de toque/foco/acessibilidade requer aceite próprio.

Evento Especial pode abrir aviso de conteúdo ainda não disponível; não criar evento funcional ou serviço de rede por inferência. Batalha deve encaminhar à sessão existente e permitir retorno ao hub sem reapurar recompensa.

## Estado e próximo passo

✅ Metadados dos dois acessos cadastrados e validados (IDs/rótulos/destinos, área dentro da imagem e âncora dentro da área).

⚠️ Ainda não desenhados nem clicáveis no aplicativo: faltam leitor do catálogo no runtime, renderer e roteador. Nenhum código de combate/engine modificado, aplicativo aberto ou teste visual realizado nesta definição. Não repetir suíte de combate por cadastro.

Próxima ligação gráfica: carregar panorama, desenhar os dois acessos e ligar painel provisório/arena. A observação visual exige autorização específica de abertura. Vídeo permanece integração separada.

Material opcional que o usuário pode adiantar: [briefing visual do hub](BRIEFING-VISUAL-HUB.md), principalmente uma cópia marcada com as duas posições exatas. Não bloqueia a implementação provisória.
