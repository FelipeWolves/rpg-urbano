# HUB-01B — dois acessos provisórios

19/11/2026. Usuário confirmou manter somente dois acessos, sem terceiro botão por enquanto.

| Acesso | Local solicitado | Destino previsto |
|---|---|---|
| Evento Especial | Outdoor acima da cabeça do lobo | Painel provisório de evento, ainda sem conteúdo cadastrado |
| Batalha | Tela do dispositivo na mão do homem | Batalha de referência existente |

Rotas são ações internas, não links de internet. Texto adotado: “Batalha”.

## Cadastro e apresentação

data/presentation/startup.json contém hub.hotspots com IDs estáveis, rótulos, destino, âncora e área de toque em coordenadas0–1 da imagem inteira. IDs independem da arte; trocar imagem exige ajustar posições/recortes, não reescrever destinos.

Alvos confirmados pela imagem marcada enviada pelo usuário: outdoor com rosto feminino acima do lobo alado e tela do dispositivo do homem à direita. A marcação2752×1536 foi preservada localmente em assets/sources/hub-accesses-marked-2026-11-19.png. Âncoras e áreas normalizadas atualizadas a partir dos dois círculos. A dúvida sobre qual outdoor foi resolvida; geometria de toque/rótulos ainda exige inspeção em execução. A imagem de fundo original não foi editada.

Na ligação gráfica, textos ficarão em camada separada. Fundo, indicador e área clicável devem usar o mesmo enquadramento; partes ocultas pelo recorte e barras laterais não recebem cliques. Em tela estreita, os acessos ficam em regiões diferentes; navegação entre áreas precisa permanecer disponível. Tamanho de toque/foco/acessibilidade requer aceite próprio.

Evento Especial pode abrir aviso de conteúdo ainda não disponível; não criar evento funcional ou serviço de rede por inferência. Batalha deve encaminhar à sessão existente e permitir retorno ao hub sem reapurar recompensa.

## Estado e próximo passo

✅ Metadados dos dois acessos cadastrados e validados (IDs/rótulos/destinos, área dentro da imagem e âncora dentro da área).

✅ Leitor do catálogo, renderer e roteamento escritos no incremento posterior [HUB-01B-IMPLEMENTACAO.md](HUB-01B-IMPLEMENTACAO.md). ⚠️ Compilação não comprova clique/visual: aplicativo ainda não executado nesta revisão. Os fontes do combate não mudaram.

Próximo aceite: executar cópia/pacote atualizado com -Hub e conferir panorama, acessos, painel e retorno. A observação visual exige autorização específica de abertura. Vídeo permanece integração separada.

Material opcional que o usuário pode adiantar: [briefing visual do hub](BRIEFING-VISUAL-HUB.md), principalmente uma cópia marcada com as duas posições exatas. Não bloqueia a implementação provisória.
