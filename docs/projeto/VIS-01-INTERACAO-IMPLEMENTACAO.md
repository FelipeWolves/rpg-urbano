# VIS-01 — implementação inicial de interação

19/11/2026. **✅ Contratos nativos e compilação aprovados. ⚠️ Interface ainda sem inspeção desta revisão.** Nenhuma janela aberta ou pacote novo gerado nesta entrega.

## Recursos escritos

- Rótulos de unidade sem fundo/borda agregadora, mantendo nome, HP e barra; tentativa de separar rótulos projetados sobrepostos com linhas de ligação. Efeito visual ainda precisa ser observado em1/3/5 e outras resoluções.
- Ícone [i] próximo à vida abre lista dos efeitos ativos reais. Painel paginado informa valores/cargas/duração/relógio disponíveis, inclusive ausência de efeitos; não inventa propriedades. Não é arte final de ícone.
- Faixa de proteção azul-clara sobre a parte preenchida da vida. Usa escudo restante real do núcleo; excesso é mostrado no painel e a faixa é limitada à área de vida visível, sem reduzir a proteção real.
- Habilidades com rótulos curtos1/BASIC,2/PASSIVE ou SKILL,3/SKILL. Gesto prolongado de0,5s abre detalhes; clique curto seleciona somente ativas disponíveis. Passivas e habilidades bloqueadas/em recarga continuam consultáveis. Soltar após gesto prolongado não seleciona/executa. Limiar é provisório.
- Metadados opcionais description/level em AbilitySlot. Nome completo e nível somente quando cadastrado; sem nível inventado ou sistema de evolução implementado. Descrições do cenário atual derivam dos dados já existentes e notas; tradução final pendente.
- Enquanto painel/consulta está aberto, a apresentação não agenda novas ações automáticas. Isso pausa a emissão local de comandos, não altera clocks, AP ou regras do núcleo. Fechar informação não consome oportunidade.
- No padrão, escolher habilidade sugere um alvo válido por sorteio apenas de apresentação; clicar outro candidato ou sua forma troca a seleção. Anel no chão identifica alvo; -TargetMarker=Diamond usa losango. Imagem/símbolo próprio é extensão futura. Confirm action é o único botão que envia a ação nesse perfil.
- No clássico, preservado o fluxo de escolher habilidade e depois alvo para executar, sem confirmação adicional ou sorteio. Informações de efeitos/habilidades e rótulos limpos são compartilhados.

## Escudo: limite importante

O pedido percentual foi registrado:10% da vida atual100 gera10 pontos; com vida atual50 gera5. A camada visual recebe **pontos reais já calculados**. Se o núcleo fornecer5 para50HP, a faixa ocupa10% da parte preenchida; com HP máximo100 isso corresponde a5% da largura total.

As habilidades antigas do laboratório continuam fornecendo escudo **fixo de10 pontos**, conforme seus contratos anteriores. Não foram convertidas silenciosamente em10%. Nesse caso, com50HP,10 pontos cobrem20% da parte preenchida. A interface não deve falsificar5 pontos enquanto o núcleo absorve10. A definição percentual, instante de cálculo/recalculo e demais modificadores serão implementados/revisados com o documento do usuário; aguardá-lo não bloqueia o trabalho de UI autorizado.

## Testes realizados

Roteiro testar-combate.ps1 recompilou dependências alteradas e terminou com sucesso:213 casos gerais anteriores,6 casos do adaptador e6 novos grupos de apresentação. Os6 de geometria anteriores não foram repetidos porque a geometria não mudou.

Novos grupos: frações de HP/proteção e excesso/HP zero; efeitos múltiplos/ausência; candidatos por tipo/lado/vida/controle; passiva/detalhes e nível ausente/presente; gesto curto/prolongado/revisão antiga; recusa de nível zero. Compilação nativa /W4 /WX. Primeira tentativa parou por comparação signed/unsigned no teste; corrigida sem desativar avisos e regressão concluída.

Build Editor/Win64/Development passou:19 ações na primeira compilação; recompile incremental de4 ações, saída0,14,31s após inicializar coordenadas do marcador. C4701 do código próprio corrigido; avisos MSVC não preferido/C4864 da engine mantidos. Stage de32 entradas canônicas; cópia atualizada do fonte corrigido e hashes reconferidos, sem editar código diretamente no stage.

- [Contratos nativos](../relatorios/atuais/VIS-01-INTERACAO.json).
- [Build final](../relatorios/atuais/VIS-01-INTERACAO-BUILD.json).
- [Build inicial preservado](../relatorios/atuais/VIS-01-INTERACAO-BUILD-INICIAL.json).

## Aceite restante

Empacotar a revisão e testar mouse: abrir/fechar/paginar efeitos; gesto prolongado em ativa/passiva/bloqueada; confirmar que soltura não seleciona; trocar sugestão de alvo sem agir; confirmar e verificar revisão, automação e reinício. Verificar colisão de rótulos, faixa de proteção, aparência dos marcadores e clássico preservado. Nenhum desses resultados visuais foi declarado nesta entrega.

Toque/mobile, acessibilidade, imagens finais, habilidades em português/localização, catálogo/níveis reais, sexta posição e resoluções diversas permanecem pendentes. Nenhuma mudança de rede, instalação ou execução de componente de outro jogo. Os pacotes antigos não contêm esta revisão. Próxima abertura continua exigindo autorização específica.
