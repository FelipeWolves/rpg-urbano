# Letreiros e aviso de importação — 23/11/2026

## Aceites recebidos

Usuário confirmou a troca da logo, a correção de PRAÇA, a família/direção da fonte e a proposta de estilo. Relatou letras grandes/grossas e fundo preto chapado no hover. Esses pontos de acabamento foram reabertos; os demais aceites não foram descartados. Não houve aceite novo dos tutoriais por essa mensagem.

## Ajuste implementado

- Roboto Light da engine substitui a variante Bold Condensed somente nos letreiros do panorama. Tamanho-base22→20; teto de escala1,0→0,9. Cormorant e Lora mantidas nos demais usos.
- Removidas a caixa e a linha de hover padrão. O texto já tem luminosidade discreta; passagem do ponteiro aumenta gradualmente a luz azul/ciano e clareia o traço. Eliminado o contorno opaco duplicado que engrossava o desenho.
- Fade de entrada/saída por estado de cada letreiro, sem mudar o tamanho durante o hover. Áreas clicáveis mínimas48px preservadas. Perda de foco não mantém o alvo marcado como sob o mouse.
- Movimento reduzido mantém troca de luminosidade imediata, sem transição animada. Alto contraste é uma opção explícita e preserva seu fundo fixo/linha de contraste; não é uma caixa adicionada pelo hover.
- PRAÇA continua em maiúsculas; presença de Ç/Ã/Õ/É na fonte Light conferida no cmap local.

## Aviso de21alterações

É o monitor de fontes para importação/reimportação da Unreal, não uma falha de compilação. O protótipo lê PNG/JSON em Content/Presentation diretamente; esses arquivos não precisam virar uassets nesta arquitetura atual. Cancelar a solicitação não invalida a logo ou texturas já carregadas. Não foi acionada importação nem criada duplicação de assets pelo assistente.

Configuração replicável: Config/DefaultEditorPerProjectUserSettings.ini exclui **Presentation/** da entrada monitorada /Game/, preservando a exclusão preexistente Localization/*. O monitor e as outras pastas continuam habilitados. A mesma exclusão foi adicionada somente à entrada conhecida do cache de preferências deste projeto em Saved/Config/WindowsEditor/EditorPerProjectUserSettings.ini, com editor já fechado. Demais preferências foram preservadas; nenhuma preferência global, rede, jogo original ou instalação da engine foi alterada.

A eficácia da exclusão na próxima abertura ainda depende de reteste do editor, que não foi executado nesta entrega. Não confundir a exclusão de arquivos carregados pelo protótipo com desativar o fluxo normal de importação de futuros modelos/texturas nas outras pastas.

## Validação e reteste

Compilação Unreal aprovada; stage93entradas/hash/include verificado. 19texturas e4glifos conferidos. Sem alterações no núcleo de combate, tutoriais, saves de jogo ou mídias; não foram repetidas baterias de combate sem necessidade. Nenhum editor ou jogo foi aberto; não foi preciso forçar fechamento do processo.

Ao testar: passar e retirar o mouse dos nomes; conferir traço mais fino e menor, luz suave sem bloco preto no modo padrão e caracteres maiúsculos. Conferir o aviso de importação ao reabrir. Logo e família da fonte escolhida não exigem novo aceite por rotina; apenas o acabamento alterado permanece pendente. Opção alto contraste tem aparência própria.

Demo~28% mantida. Registros: HUB-LETREIROS-2026-11-23.json e UI-INTEGRACAO-2026-11-23-RECURSOS.json. Fontes e mídias continuam locais/fora do Git.

## Segundo reteste — luminosidade

Usuário relatou melhora e confirmou que o hover acende; pediu nomes cerca de30%mais visíveis e um glow leve atrás das letras. Aplicado ganho nominal1,3na cor em repouso, limitado ao intervalo válido; a aparência percebida depende do fundo e aguarda reteste. Halo próximo ligeiramente reforçado e halo externo azul/ciano de baixa opacidade somente no hover. Fonte, tamanho, caixa de clique, ausência de caixa preta padrão e configuração de importação preservados. Movimento reduzido continua sem fade animado. Sem mudança de combate ou mídias; validação por compilação e stage, sem abrir telas.

> Continuidade: HUB-HALO-E-MOUSE-2026-11-23.md substitui o halo por máscara Gaussiana dos glifos e revê GameOnly, que solicitava trava Slate. Brilho nominal1,3 e tipografia preservados; aparência e mouse livre aguardam reteste.
