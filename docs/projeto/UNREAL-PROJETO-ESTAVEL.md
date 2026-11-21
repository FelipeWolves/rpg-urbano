# Projeto Unreal estável — ENG-ESTAVEL

21/11/2026. **Concluído no Windows local:** compilação, abertura no editor, mapa salvo, Play com hub e acesso à arena, encerramento do Play e reabertura pelo próprio `.uproject` com mapa e hub preservados. Não é validação Android nem pacote comercial.

## Como abrir sem comandos

Abra `experiments/unreal/RpgEngineProbe/RpgEngineProbe.uproject` pelo Explorador ou pelo navegador de projetos da Unreal. Alternativa: `Abrir-Editor.cmd` na raiz, agora chama o editor diretamente, sem PowerShell. Clique Play para ver o hub. Antes de Play, o mapa mostra o cenário geométrico: os controles e atores de laboratório são criados durante a execução.

A pasta continua com o nome histórico `experiments`, mas este projeto é agora o endereço fixo para edição. Não editar `build/p/...`: são snapshots de compilação anteriores. Não movemos nem removemos nenhum stage antigo. Arquivos Config, Source e o mapa próprio são versionados; Saved, Intermediate e Binaries continuam locais/ignorados.

## Implementação

- `.uproject`, alvos Editor/Game e módulo C++ padrão; sem plugin externo novo.
- O módulo compila 11 fontes canônicas de `src` por pequenas unidades de ligação. Headers continuam em `include/rpg`; não há uma segunda cópia editável do núcleo nem ligação aos objetos do console.
- Build.cs acompanha dependências externas do núcleo. Stages isolados continuam usando snapshots com hashes, sem incluir as unidades de ligação, evitando duplicação.
- `Content/PrototypeHub.umap` foi salvo pelo editor a partir do cenário básico da engine instalada; DefaultEngine e MapsToCook apontam para `/Game/PrototypeHub`.
- `StartInHub=True` em DefaultGame; `-Arena` permite iniciar direto na arena, `-Hub` força hub. Perfil de combate e layout clássico continuam opções separadas.
- 7 recursos locais de apresentação foram copiados e conferidos por hash. `preparar-editor-estavel.ps1` recompõe somente arquivos ausentes e recusa sobrescrever arquivos diferentes. Não apaga conteúdo, não compila e não instala.
- Mídias permanecem fora do Git; uma cópia nova precisa dos recursos locais e de compilação C++. Abrir o projeto completo não exige redigitar comandos diariamente, mas alterações C++ ainda exigem compilação.

## Evidências desta entrega

| Verificação | Resultado |
|---|---|
| UBT Editor Win64 Development | Saída 0; 22 ações; log local `build/stable-editor-build.log` |
| Ponte canônica dentro da Unreal | 11 checkpoints, 0 falhas, saída 0; `build/stable-core-probe.log` |
| Editor / salvar mapa | Observado; `PrototypeHub.umap`, 48.850 bytes |
| Play | Hub com imagem/fontes/logo e acesso à arena observados |
| Reabertura padrão Windows | `.uproject` aberto por associação normal; mapa persistiu; Play voltou ao hub |
| Parar Play | Observado; editor voltou ao modo de edição |
| Suíte geral de 213 testes | Histórico preservado; não repetida, pois regras do núcleo não mudaram |
| Novo pacote Windows / Android | Não produzido nesta entrega |

Os testes não validam todos os tamanhos de tela, formações, toque ou fluidez. As marcações de aceite existentes continuam restritas aos seus cenários.

## Android — descoberta importante

Os caminhos já estavam persistidos em `C:/Users/Ergo_/AppData/Local/Unreal Engine/Engine/Config/UserEngine.ini`. A engine declara AndroidSDKSettings como `globaluserconfig`; SDK/NDK/JDK não são configurações portáveis do repositório. Não houve alteração desse arquivo, de PATH ou da instalação da engine.

SDK `D:/AndroidSDK`, NDK `27.2.12479018`, JDK `D:/AndroidJava/jdk-17.0.21.1+2`. Usuário confirmou boot do AVD e adb anteriormente. Apesar disso, uma inicialização sem variáveis injetadas e ValidatePlatforms retornaram **Android INVALID r27c**. Portanto, caminhos gravados não comprovam toolchain reconhecido. Causa ainda não isolada; SDKAPILevel está `latest` com API37 também instalada. Antes de APK: resolver descoberta, fixar API36 na matriz de build e validar ABI/renderizador. Não reinstalar componentes nem modificar a engine por inferência.

## Ocorrências e limitações

O lançamento da engine sem indicar projeto tentou abrir um projeto anterior e mostrou aviso de módulo ausente. A abertura explícita do projeto estável e a posterior abertura por associação funcionaram. Três instâncias duplicadas abertas durante o diagnóstico foram encerradas; não usar a ausência inicial de janela como prova de falha nem abrir várias cópias simultaneamente. Houve avisos transitórios de disputa de preferências entre essas instâncias; nenhuma alteração de permissões foi feita.

Permanecem avisos conhecidos de MSVC não preferido e C4864 da engine; commandlet terminou com 0 erros e 4 avisos de ACL/compressão de animação. Abertura por associação usa o idioma preferido atual do editor (observado português); o lançador da raiz e verificações automatizadas usam inglês. Não alteramos idioma global. Jogo em português e localização continuam separados.

O controle de tela foi encerrado a pedido do usuário. Não houve nova interação visual depois disso. Rede não isolada; nenhuma configuração de rede, download ou instalação feito. Não acessamos instalações de jogos.

## Próximos blocos

1. AUT-01–04: apresentação Desktop/Mobile explícita, regiões seguras, mouse/toque e painéis; não dependem de arte nova.
2. Android: diagnóstico focal de descoberta e primeiro build mínimo em entrega própria.
3. SIS-02/REG e kit Felipe: ponto fixo e funis antes dos efeitos novos; preservar ESC/R04.
4. AUT-05–16 conforme dependências da fila; não ativar os sete locais futuros do panorama nesta revisão.
