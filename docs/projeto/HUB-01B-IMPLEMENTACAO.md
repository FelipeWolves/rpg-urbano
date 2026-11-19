# HUB-01B — ligação por código na Unreal

19/11/2026. Desenvolvimento autorizado sem abrir/modificar preferências do editor, executar o aplicativo ou commandlets. Código compilado; aceite visual e fluxo em execução continuam pendentes.

## Implementação

- ProbeHub.cpp implementa leitura local do catálogo versionado e importação única do panorama JPEG pelo runtime da engine.
- Hub usa a geometria neutra de startup_presentation: panorama/foco, regiões configuradas e projeção/clipping de alvos.
- Os dois rótulos vêm do catálogo e ficam separados da imagem. O arquivo de fundo não foi editado.
- Evento Especial abre painel local de conteúdo em preparação; não cria evento real nem rede.
- Batalha alterna para o HUD/arena existentes. Voltar ao hub e Esc preservam a mesma VisualSession; não reiniciam nem concedem recompensa por navegação. No código, enquanto o hub está ativo, o fluxo retorna antes de agendar ações automáticas.
- Esc fecha primeiro uma consulta aberta da arena; fora de consulta volta ao hub. Há botão Voltar ao hub.
- Esquerda/Centro/Direita e setas permitem focar uma região; Panorama/Focar área alterna enquadramento sem alterar regras do combate.
- Textura referenciada por propriedade do HUD para seu ciclo de vida. Não é carregada a cada frame.
- Sem catálogo/imagem válidos, apresenta erro e oferece acesso à batalha; não fica carregando indefinidamente.
- Dependência Json da própria engine adicionada; sem plugin/SDK/download.

A navegação usa a sessão existente. Reinício de combate continua sendo ação explícita do botão Restart da arena; ao voltar/reentrar, uma batalha encerrada permanece encerrada até reiniciar. Progresso mantém as regras de laboratório já existentes; ainda sem save de runtime gráfico.

## Ativação e recursos

O hub é opt-in por **-Hub** em uma compilação atualizada. Sem essa opção, preserva-se a entrada direta na arena. Não executar só por ler este roteiro.

O runtime procura Content/Presentation/data/presentation/startup.json e resolve a imagem em relação a Content/Presentation. O preparador copia exatamente o catálogo e o panorama atuais, além dos fontes, conferindo hashes:37 entradas. Vídeos, arte lateral e fontes editáveis não foram copiados/importados nesta etapa.

DefaultGame.ini solicita incluir Presentation como diretório não-UFS no futuro staging do pacote. É configuração preparada; não houve cook/stage/archive de pacote nem comprovação dessa cópia em um pacote executado. Não abrir automaticamente a fonte canônica no editor.

O leitor aplica somente a parte do catálogo necessária ao hub (panorama, regiões, alvos). Preferências antes da janela, orientação real, player de vídeo e restauração do aparelho continuam fora desta ligação. Variantes de mídia não são reproduzidas.

## Validação

- [10 grupos anteriores de apresentação](../relatorios/atuais/HUB-01A.json) reexecutados por mudança do módulo compartilhado.
- [5 novos grupos de alvos](../relatorios/atuais/HUB-01B.json): projeção no panorama, esconder fora do recorte, clipping parcial, faixas/resize e dados inválidos. /W4 /WX aprovados.
- [Build Editor final](../relatorios/atuais/HUB-01B-BUILD.json), sem execução de engine/aplicativo.
- [Primeira tentativa](../relatorios/atuais/HUB-01B-BUILD-INICIAL.json) preservada: faltava incluir Engine/World.h para consultar tempo; corrigido no fonte canônico.
- Verificação estática da ativação -Hub, fluxo de retorno antes de automação e ausência de reset/settle por navegação. Isso não substitui observação do comportamento real.

Não foi repetida a suíte213 de combate, pois seus fontes/contratos não mudaram. Não afirmar teste do fluxo hub→arena→hub como realizado: só projeções nativas e compilação estão aprovadas.

## Limites e próximo aceite

Com abertura especificamente autorizada e pacote atualizado: confirmar carregamento, textura/cor, dois rótulos nos círculos, hitboxes, recortes/foco, resize, consulta do evento, ida/volta durante turno manual/automático e após vitória, sem ações/recompensa duplicadas. Verificar erro/fallback com recurso ausente em cópia de teste.

Interface usa Canvas, mouse e teclas/setas; toque/controle/acessibilidade ainda não integrados/aceitos. Áreas mínimas de toque, rótulos longos, telas muito pequenas, DPI e áreas seguras pedem validação própria. Alvo oculto não é projetado; rótulo visível fica limitado ao conteúdo, sem permitir clique em barras externas.

Protótipo mantém arena no mesmo mapa e a cobre no hub; ainda não é gerenciamento final de carregamento/renderização de cenas. Limites de imagem8K/32MiB e catálogo1MiB são guardas deste experimento, não requisitos de arte para o produto. Dimensões/arquivo decodificado precisam coincidir.

Vídeos, janela configurada antes da criação, mobile, animações, arte final e eventos continuam entregas próprias. Briefing visual opcional não bloqueia esta implementação.

Build final Editor/Win64/Development:21 ações, saída0,35,78s no stage991ce5de; avisos existentes do toolset/engine não suprimidos. Preparação e compilação permaneceram separadas. O alvo Game/pacote desta revisão ainda não foi produzido/executado.
