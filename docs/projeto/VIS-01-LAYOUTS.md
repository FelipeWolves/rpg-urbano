# VIS-01 — apresentações padrão em profundidade e clássica lateral

19/11/2026. Decisão do usuário: preservar a arena lateral e adotar profundidade como apresentação padrão. Sem copiar recursos de terceiros. Esta alteração não escolhe nem modifica regras de combate.

## Organização

- **Standard / padrão:** aliados no primeiro plano, inimigos ao fundo; até cinco posições por lado em leque escalonado: três posições mais próximas e duas mais avançadas, com profundidade e deslocamentos laterais. A imagem posterior do usuário esclareceu que não se trata de uma fila vertical; a proposta foi ajustada para esse leque. Não é reprodução exata de uma câmera externa.
- **Classic / clássico:** mantém a câmera e, para três unidades, as mesmas coordenadas laterais do pacote anteriormente observado. Continua disponível por chamada explícita.
- Os dois lados se orientam para o adversário. Esferas provisórias não demonstram visualmente frente/costas; o yaw será relevante para personagens com orientação reconhecível.
- Número de formas vem do elenco da sessão, em vez de seis objetos fixos e IDs A1/B1 fabricados pela cena. Existem cinco posições possíveis; o cenário de referência permanece3×3, sem adicionar novos personagens.

A função nativa arena_placements fica em include/rpg/arena_layout.hpp e src/arena_layout.cpp, independente de Unreal. A HUD cria formas no seu BeginPlay a partir do próprio estado. A câmera é escolhida pelo GameMode. O núcleo de combate e VisualSession não recebem novas regras de dano, turnos ou habilidade.

## Chamadas distintas

Nos próximos aplicativos gerados com esta mudança:

```text
RpgEngineProbe.exe -culture=en
RpgEngineProbe.exe -culture=en -ArenaLayout=Standard
RpgEngineProbe.exe -culture=en -ArenaLayout=Classic
```

Sem argumento, Standard é o padrão. Classic seleciona somente apresentação lateral. A escolha de regras clássicas (ataque/defesa etc.) é um eixo separado: não ativar defesa genérica no perfil Equipes só porque a arena ficou lateral. Os comandos existentes Jogar.cmd e Jogar-Classico.cmd e a experiência textual foram preservados; a câmera não se aplica ao console.

Na Unreal, o mesmo argumento se aplica à execução em modo de jogo do projeto de avaliação, quando especificamente autorizada. Abrir o editor em modo de edição não equivale a executar o GameMode/HUD. Um editor aberto ou pacote antigo não recebe automaticamente os novos fontes; usar novo stage/build e não editar artefatos gerados.

## Validação e limites

Seis grupos nativos passaram com /W4 /WX: padrão/cinco profundidades, igualdade das três coordenadas clássicas anteriores, formações1–5 nos dois perfis/determinismo/ausência de posições duplicadas, formação assimétrica, recusa de entradas inválidas e nomes explícitos de chamada. Os dados de entrada não são alterados. [Resultado](../relatorios/atuais/VIS-01-LAYOUT.json).

A preparação agora copia30 entradas para stage novo em build/p, preservando os anteriores. [Resultado da compilação Unreal](../relatorios/atuais/VIS-01-LAYOUT-BUILD.json) registra separadamente o build, sem confundir com testes visuais. Compilação Editor/Win64/Development aprovada:18 ações, saída0,27,27s na versão ajustada pela imagem. Mantidos os avisos MSVC não preferido/C4864 da engine, sem supressão. Não repetidos213 testes gerais nem6 do adaptador, pois regras e adaptador não mudaram. Os6 grupos de layout são adicionais e não são testes na interface.

Esta entrega não abre a Unreal/aplicativo nem empacota a revisão. A aprovação visual3×3 registrada no VIS-01C pertence à disposição lateral anterior; não transferir esse aceite para a nova câmera padrão. A nova composição exige inspecionar separação/enquadramento, rótulos e clique, particularmente com5 unidades. Permanecem as pendências de HP/escudo, descrição/seleção de alvo, resoluções e localização.

## Próxima verificação

Com autorização específica, gerar pacote desta revisão e observar Standard e Classic com os mesmos dados, verificando que só muda a apresentação. Confirmar visualmente primeiro plano/fundo conforme intenção do usuário. Não sobrescrever o pacote clássico já observado nem alterar configurações globais, rede, launcher ou instalações de outros jogos. Inglês permanece prioritário nas ferramentas; português do produto será localização independente.


## Referência visual fornecida pelo usuário

A imagem anotada foi usada somente como referência de composição, sem copiar/importar a imagem, personagens, ícones ou interface para o projeto. Vermelho: cinco aliados distribuídos em profundidade; retrato2D do personagem ativo próximo às suas três habilidades. Azul: dois auxiliares com vida independente. Amarelo: chefe, sua barra principal e suas habilidades no topo.

A HUD padrão ganhou uma área original provisória para identificar o ator ativo ao lado das habilidades. Ela ainda mostra identificação/texto, não retrato2D final. O clássico lateral não recebe esse painel novo, preservando sua composição. Ilustração própria/licenciada e escala/layout em várias resoluções continuam pendentes.

A apresentação de chefe terá de identificar explicitamente qual unidade é o chefe e quais são auxiliares. A barra principal deverá ler a vida desse chefe, sem somar ou compartilhar a vida dos auxiliares por inferência. As habilidades do topo deverão vir do kit do chefe. Isso não autoriza alterar a mecânica dos modos existentes nem usar o modelo de HP compartilhado PAR-02 indiscriminadamente. Essa UI de chefes ainda não foi implementada; o cenário atual continua o3×3 de referência, com HP individual.
