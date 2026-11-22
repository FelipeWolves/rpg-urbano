# AUT-04 — painéis e leitura integral dos textos

21/11/2026. Implementação e compilação concluídas na pasta principal. Sem abrir editor, prévia ou Android; aceite visual pendente do usuário.

## Entrega

`wrap_text` recebe texto UTF-8 e uma função de medição da fonte. Respeita a largura medida, quebra palavras longas por escalares Unicode completos e preserva parágrafos/quebras de linha (inclusive CRLF). Espaços/tabs repetidos entre palavras são normalizados. Não corta bytes de acentos pré-compostos; não é um segmentador completo de grafemas, emoji com ZWJ ou escrita complexa. Esses casos continuam dependentes do shaping/fontes da engine e de validação específica.

Texto não vazio não é descartado para caber no painel: o restante segue para páginas. `text_page` limita a página pedida à faixa existente e calcula limites sem somas que possam exceder size_t. Layout reserva áreas distintas para corpo, Anterior, Próxima e Fechar. No Terminal existe uma segunda linha separada para Batalha de laboratório. Botões têm pelo menos48px na geometria viável.

O HUD compartilha `DrawInfoPanel` entre habilidades, efeitos e locais do hub. Título, nome, nível quando informado e descrição entram no fluxo paginado. A medição utiliza a mesma fonte e escala da renderização do corpo; fonte de corpo local quando carregada, padrão da engine no acesso direto sem catálogo. Texto refluído e página atual são recalculados após redimensionar; não se garante preservar a mesma palavra no topo.

Abrir/fechar/paginar não chama submit nem concede recompensa. A seleção básica/recarga/passivas e resolvedor não foram modificados. O fluxo de ponteiro AUT-03 continua obrigatório para cliques; painéis modais impedem ações por trás conforme os bloqueios já existentes.

## Entradas inválidas e limites

Largura não positiva/não finita, medição negativa/não finita, UTF-8 inválido, NUL embutido, glifo individual maior que a largura e texto acima de1MiB são recusados explicitamente pela API. O adaptador mostra “Texto indisponível.” em caso de falha; não exibe silenciosamente uma descrição parcial. Área de tela insuficiente pede ampliação, com Escape disponível para fechar; isso não valida usabilidade em janelas muito pequenas. Recursos de rolagem por gesto, seleção/cópia de texto e localização completa não foram adicionados.

A implementação usa a medição injetada, não uma estimativa de número fixo de caracteres por linha. Testes nativos usam métricas determinísticas de teste; a aparência final com fontes reais ainda requer aceite visual. O layout não substitui UMG/USafeZone nem valida notch, DPI ou aparelho.

## Verificação

-10 grupos novos: palavras comuns, acentos, palavra longa, parágrafos/whitespace, métricas proporcionais, medidas inválidas, UTF-8 inválido, paginação/limites, regiões dos painéis e espaço insuficiente.
-54 grupos nativos de apresentação aprovados (44 anteriores +10 novos).
-UBT Editor Win64 Development:8 ações, saída0. Log local `build/aut04-build.log`.
-Regras de combate intactas; suíte geral não repetida por esta mudança de apresentação.
-Nenhuma execução gráfica, alteração Android, instalação, rede ou controle de tela.

## Próximo bloco

AUT-04: código neutro testado e integração compilada. Aceite visual/interativo pendente. Próximo AUT-05: preparar cenários repetíveis de formações1/3/5, assimetrias e apresentação do escudo, preservando as regras ESC já aprovadas. Parte de preparação/testes pode avançar sem editor; aceite final dos cenários exige observação apropriada. Percentuais gerais mantidos aproximadamente22% até novos aceites.
