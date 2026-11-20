# Briefing visual do hub — material que pode ser adiantado

Arquivo opcional para o usuário preencher enquanto a ligação gráfica é implementada. Não bloqueia o hub provisório: campos sem resposta usam apresentação temporária. Não é necessário refazer a imagem ou produzir3D agora.

## Já definido — não precisa responder novamente

- Fundo atual: assets/runtime/hub/panorama.jpg.
- Dois acessos: Evento Especial no outdoor e Batalha no dispositivo na mão do homem.
- Terceiro acesso adiado.
- Textos/botões separados da imagem, com posições configuráveis.
- Evento abre painel provisório; Batalha usa arena existente.
- Vídeos desktop/mobile e arte lateral recebidos; player é integração posterior.

## 1. Marcação da imagem — recebida

✅ Recebida em19/11/2026; dois alvos confirmados e coordenadas atualizadas. Referência local: assets/sources/hub-accesses-marked-2026-11-19.png. Não é necessário reenviar esta decisão.

Enviar uma cópia da imagem com dois círculos/setas: ponto exato de Evento Especial e ponto exato de Batalha. Isso resolve qual outdoor sobre qual lobo e permite ajustar o toque sem modificar o original. Pode ser PNG/JPG de referência, sem precisão de pixels.

Nome/caminho da imagem marcada:
Resposta:

## 2. Aparência dos acessos — opcional

Preferência: texto luminoso, botão sólido, ícone com legenda ou outra.
Resposta: Texto luminoso com borda/moldura sutil em estilo neon (letreiro urbano holográfico) para o Evento Especial no outdoor, e ícone pulsante com legenda curta (estilo interface de smartphone) para Batalha no dispositivo na mão do personagem — ambos com área de clique ampliada e feedback visual ao passar o mouse (hover) ou tocar.

Cores desejadas (descrição ou códigos):
Resposta: Paleta neon urbana de alto contraste: Magenta/Roxo elétrico (#D946EF / #A855F7) e Ciano luminoso (#06B6D4 / #22D3EE) para o acesso de Evento Especial, Dourado/Âmbar (#F59E0B / #FACC15) para destaque de ação/Batalha, e fundo escuro translúcido (#0F172A com 80% de opacidade) para garantir legibilidade sobre a arte.

Exemplo visual próprio/de referência, se houver:
Resposta: Letreiros flutuantes estilizados do Hub do Dislyte (como Eventos e Provações sobre os prédios e o ícone do comunicador Swiftspace no canto), mantendo os textos separados da imagem de fundo.

## 3. Nome e identidade provisória — opcional

Nome a exibir no hub:
Resposta: Mundus Lupis Disco / CODINOME: LOBO

Logotipo próprio, se já existir (PNG transparente/SVG e fonte editável quando disponível):
Resposta:Silhueta oficial do lobo do site: https://contosdeumlobo.com.br/wp-content/uploads/2018/07/klemzo123_silhouette-wolf-2-e1534524681539.png (salvar cópia local em assets/sources/logo-lobo-silhouette.png; combinar com tipografia provisória gerada por código).

Fonte desejada, se já houver escolha; informar origem e permissão de uso. Não precisa escolher agora:
Resposta: Fontes serifadas gratuitas sob licença aberta SIL Open Font License (Google Fonts): "Cinzel" (https://fonts.google.com/specimen/Cinzel) para títulos, logotipo e letreiros do Hub, combinada com "Lora" (https://fonts.google.com/specimen/Lora) ou "Cormorant Garamond" (https://fonts.google.com/specimen/Cormorant+Garamond) para textos de painéis e descrições.

## 4. Painel Evento Especial — opcional

Título a mostrar:
Resposta: EVENTO ESPECIAL — EM PREPARAÇÃO

Texto curto do painel provisório:
Resposta: Área reservada para campanhas, desafios temporários e rotações especiais. Este painel valida a navegação do Hub e será conectado ao sistema de eventos em uma etapa futura.

Ilustração própria para o painel, se houver:
Resposta: Nenhuma por enquanto — utilizar fundo estilizado padrão da UI com a paleta neon do Hub.

Isso não exige criar recompensas, calendário ou um modo novo. Esses sistemas precisam de contrato separado; o painel pode apenas informar que o conteúdo está em preparação.

Nota de integração futura: Este acesso de Evento Especial abrigará a linha de eventos em Modo Clássico (estilo RPG PS1/PS2), separada do modo padrão de equipes.

## Entrega e limites

Pode entregar somente a imagem marcada ou este arquivo parcialmente preenchido. Recursos/versões originais ficam localmente em assets/sources; mídia grande não entra no Git automaticamente. Não substituir arquivos já recebidos sem manter uma versão anterior.

Ligação por código de catálogo/panorama/dois acessos/evento/arena/volta implementada e compilada em HUB-01B-IMPLEMENTACAO.md; falta observar com autorização de abertura. Código/runtime e decisões de gameplay não dependem de logo/arte final para essa entrega.

## Decisões posteriores vigentes —19/11/2026

As respostas originais acima ficam preservadas como histórico. O usuário confirmou Cormorant Garamond principal e Lora complementar, excluindo Cinzel; LOBO é codinome de Mundus Lupis Disco. Logo recebida em assets/sources/logo-lobo-silhouette.webp, sem conversão.

Batalha passa a usar emblema geométrico original em octógono chanfrado, presas/lâminas minimalistas e legenda BATALHA, em lugar da sugestão de smartphone. Pulso1–1,06 com anel âmbar #F59E0B de1,5px até135%, alpha0,7–0 e ciclo2,4s ease-in-out. Área mínima48×48. Movimento reduzido desliga animação e usa borda dupla e brilho fixo âmbar/carmesim #DC2626. [Implementação, verificações e limites](HUB-01C-VISUAL.md).

Implementação posterior: fontes e logo locais conectadas e verificações registradas em [HUB-IDENTIDADE.md](HUB-IDENTIDADE.md). Decisões de identidade não aguardam novas respostas.
