# RpgEngineProbe — projeto Unreal estável

Abra RpgEngineProbe.uproject normalmente e clique Play para ver o hub. O mapa próprio é Content/PrototypeHub.umap. Esta é a fonte editável, não o stage temporário em build/p.

[Guia de abertura, estrutura e validação](../../../docs/projeto/UNREAL-PROJETO-ESTAVEL.md). Núcleo canônico em include/rpg e src na raiz do repositório, compilado dentro do módulo por Private/CoreBridge. Não editar cópias geradas nem ligar objetos do console.

Source/Config/mapa são versionados; Content/Presentation contém cópias locais de recursos, preparadas por scripts/preparar-editor-estavel.ps1 a partir da raiz. Não sobrescrever mapas ou preferências com arquivos de stages. O preparador de pacote continua gerando um snapshot separado e conferido.

Win64 Editor compilado; abertura, reabertura e Play observados. Android ainda não validado; ver relatório do projeto. Testes históricos ENG-01 permanecem em docs/projeto e não precisam ser repetidos por documentação.
