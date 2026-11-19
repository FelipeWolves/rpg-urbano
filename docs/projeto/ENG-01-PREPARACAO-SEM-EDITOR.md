# ENG-01 — preparação independente do editor: persistência

18/11/2026. Usuário está instalando manualmente a UE5.8.4 e autorizou avançar no que independe dela. Entrega delimitada: fronteira do formato de progresso versus armazenamento da plataforma. Sem editor, launcher, instalação ou alteração de regras de batalha.

## Separação implementada

| Componente | Responsabilidade |
|---|---|
| include/rpg/save_game.hpp e src/save_game.cpp | SavedGame, limites e transformação validada entre dados e texto v1; C++20 padrão |
| include/rpg/save_store.hpp | Operações de carregar/gravar por caminho, sem tipos Win32 públicos |
| src/save_store_windows.cpp | Leitura de arquivos e temporário/flush/substituição Win32 do console |
| src/text_session.cpp | Orquestra comando salvar pelo adaptador configurado |

O codec não inclui windows.h nem filesystem, não conhece diretório de instalação e não acessa arquivos. O limite de bytes é compartilhado pelo codec e armazenamento. Nenhum virtual/interface genérico adicional foi criado sem necessidade.

Número de versão, tags, IDs e códigos de motivo do formato v1 permanecem. Mapeamento de motivo agora é explícito, em vez de serializar o valor interno do enum por cast. Streams usam locale clássico: a linguagem/configuração regional global não introduz separadores de milhar nos contadores.

Gravação Windows e preservação de arquivos anteriores mantidas. Não houve leitura, migração ou reescrita de slots do jogador. As funções de arquivo agora são declaradas em save_store.hpp; consumidores que usavam só save_game.hpp para I/O precisam incluir o novo header. Aplicativo e testes foram ajustados.

## Evidência

213/213 testes aprovados. Dois novos grupos: arquivo v1 fixo com motivos e ordem de seleção preservados; independência de locale com agrupamento de milhares customizado. O executável desses grupos é vinculado ao codec/núcleo, sem save_store_windows.obj. Todos os sete grupos de arquivo/restauração e dezoito textuais permanecem aprovados, assim como as regressões anteriores.

Evidências: docs/batalhas/COMBATE-ENG-01-PREPARACAO.json e codec-testes.csv; resultado corrente em docs/relatorios/atuais/COMBATE.json. Log local ignorado build/combate/verificacao-fronteira-save.log. Não é compilação Linux/Android/iOS ou teste Unreal.

## Fronteira para o experimento Unreal

1. Primeira ponte usa núcleo e estado em memória; excluir app/main, console e armazenamento Win32 do módulo inicial.
2. O formato do progresso pode ser reutilizado; a aplicação da engine deverá fornecer armazenamento/caminho apropriados, mantendo versão, validação e concessão única.
3. Não ligar os objetos /MT do console diretamente ao editor. Recompilar fontes com a configuração aprovada do módulo e tratar exceções na fronteira.
4. Executar a ponte e depois o save adaptado em cenário isolado; comparar regras/resultado e testar falha de gravação sem tocar slots do console.

São passos planejados, não módulo/plugin criado ou compilado. ENG-01 continua aberto até verificar instalação, ferramentas e integração real. Nenhum plugin, SDK mobile ou integração de IA habilitado por esta preparação.

Próximo: receber a conclusão da instalação e o caminho da UE5.8.4; conferir metadados antes do teste de engine autorizado. Não avançar em arena gráfica ou novas famílias de combate por antecipação.
