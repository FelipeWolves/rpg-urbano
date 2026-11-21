# AUT-02 — geometria e margens seguras do hub

21/11/2026. Código e compilação concluídos em `F:/Testes de jogos/Contos de Um Lobo - Jogo/rpg-urbano`. Sem execução gráfica; aceite visual continua com o usuário. Android adiado.

## Entrega

`plan_hub_frame` calcula uma área segura, cabeçalho, região da imagem, botões dos destinos e cinco controles de navegação. O HUD utiliza esse mesmo resultado para desenhar e testar cliques, em vez de repetir cálculos locais. Áreas principais não se sobrepõem; destinos e navegação têm pelo menos48×48px quando o plano é viável.

Configuração local em DefaultGame.ini, seção RpgPrototype: `SafeLeft`, `SafeTop`, `SafeRight`, `SafeBottom`, em pixels do viewport, padrão0. São margens simuladas; não constituem descoberta automática de notch, DPI ou gestos do sistema. Valores negativos/não finitos na configuração são recusados com aviso e fallback0. A API nativa rejeita metadados inválidos; espaço válido porém insuficiente retorna ausência de plano, sem dimensões negativas ou conversões indevidas para inteiros sem sinal.

O hub pede ampliação da janela/redução das margens quando a área útil for menor que280×360 ou não sobrar pelo menos64px de altura para a imagem. Até16 destinos são suportados pela geometria; isso não cadastra novos locais. Formatos largos têm até7 colunas, estreitos até3. Cabeçalho, modal e acesso de contingência também foram deslocados para respeitar as margens. A leitura integral dos textos pertence a AUT-04.

## Verificação

-34 grupos nativos aprovados:10 apresentação,5 hotspots,5 estilo,6 expansão e8 geometria.
-Casos novos: paisagem, retrato, margens assimétricas, limiares de largura,0/1/5/7/16 destinos, espaço insuficiente, margens excessivas, entradas inválidas e repetibilidade.
-Invariantes: regiões dentro da área segura, sem sobreposição entre regiões principais/botões; alvos com dimensão mínima; imagem com tamanho positivo.
-UBT Editor Win64 Development:22 ações, saída0; `build/aut02-build.log`. Respostas de compilação conferidas sem referências à raiz antiga.
-Não repetida suíte de combate: regras do resolvedor não mudaram. Sem abrir editor, preview, emulador ou serviços; sem alteração de rede.

## Correção dos caches copiados

A primeira compilação reutilizou comandos absolutos presentes no Intermediate copiado da pasta antiga. Embora tenha terminado com saída0, compilou fontes do endereço antigo e regravou artefatos gerados ali; esse resultado foi descartado como evidência do código novo. Não houve edição das fontes antigas.

Binaries e Intermediate da pasta nova foram preservados em `build/migration-cache-backup/aa6314f6`, junto do log dessa tentativa. Os caminhos de origem/destino foram verificados dentro do repositório novo antes da movimentação. Não se apagaram mapas, fontes ou Saved/preferências. A compilação limpa recriou os artefatos no endereço novo. Não reutilizar caches de compilação ao copiar/mover um projeto; conservar fontes/Config/Content e regenerar derivados. Relatórios históricos conservam seus caminhos originais como evidência, não como comandos ativos.

## Situação e próximo bloco

AUT-02: geometria configurável implementada/testada e adaptador Unreal compilado. Aceite visual, safe area física e aparelhos pendentes. Não declarar o conjunto VIS ou mobile finalizado. AUT-03 (entrada comum mouse/toque, cancelamento e prevenção de duplicação) é o próximo bloco de código; AUT-04 trata painéis/textos.

O usuário pode testar com Abrir-Preview.cmd ou Abrir-Preview-Mobile.cmd na raiz nova. O segundo continua sendo Windows em retrato. Percentual geral permanece aproximadamente22% da demo até novos aceites;34 testes de apresentação não significam34 testes de combate nem paridade Android.
