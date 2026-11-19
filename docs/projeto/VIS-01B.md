# VIS-01B — cena compilada, inspeção pendente

19/11/2026. ⚠️ Implementação inicial compilada; nenhum aceite visual ainda.

ProbeArenaMode configura câmera e seis esferas provisórias da engine; ProbeArenaHUD usa Canvas e cliques para conectar VisualSession. Não instala UMG/plugin. Painel apresenta HP/escudo, ator, rodada/oportunidade/ações, estados de habilidade e resultado/reinício. Seleção segue revisão da sessão e é descartada após ação. Inimigo avança uma ação por intervalo visual, sem alterar relógios do núcleo.

Foram declarados InputCore e GameMode. O manifesto de preparação inclui28 arquivos (antes22), com os novos headers/fontes compartilhados. O stage novo não substitui o pacote aceito anteriormente.

Build Editor/Win64/Development passou:17 ações, saída0,44,22s no ajuste visual. Foram mantidos aviso MSVC não preferido e C4864 em Attribute.h da engine; não houve supressão ou alteração da engine. [Resultado](../relatorios/atuais/VIS-01B.json).

Ainda não confirmado: visibilidade, foco/clique, enquadramento, legibilidade e fim/reinício. As esferas são provisórias; desaparecem na morte. Os rótulos projetados acompanham sua posição e indicam ator atual. Não há atalhos, animação ou tooltip. Rejeição mostra mensagem genérica; ampliar explicação antes do aceite. Não marcar VIS-01B concluído por compilar. A condição aparece em texto; não usa cadeado gráfico nesta etapa.

Sem mudança no núcleo desde VIS-01A, portanto seus213+6 testes não foram repetidos. Esses testes não validam Canvas. Não gerado pacote desta versão; VIS-01C continua pendente.

Próximo: abertura autorizada do stage em modo de jogo, em inglês, para testar posição1/alvo, passiva desabilitada, condição, automação e reinício. Logs em build, sem saves. Abertura não autorizada por inferência da compilação; nenhuma janela foi aberta nesta entrega.


Composição ajustada: equipes nos lados opostos, câmera inclinada, barras de HP em verde/vermelho e realce do ator; habilidades à direita e reinício à esquerda. Só a organização remete à referência; não foram copiados assets. Enquadramento, possível sobreposição e leitura em outras resoluções continuam pendentes de inspeção. Nenhuma previsão de ordem/AP foi inventada.
