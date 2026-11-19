# ENG-01 — ponte do núcleo compilada

18/11/2026. **Ponte compilada/vinculada na UE5.8.4; execução dentro da engine ainda pendente.**

## Fonte única e fronteira

O script de avaliação copia de src/include para o stage gerado os fontes battle, ability_profile, team_selection, local_session e save_game. Não copia armazenamento Win32, console, main do protótipo ou coordenadores SEQ/PAR. Cópias geradas não são fonte editável: cada execução confere SHA-256 dos dez arquivos nativos (cinco cpp/cinco headers) e dos seis arquivos do projeto de avaliação.

UE compila esses fontes como unidades separadas do mesmo módulo, com C++20/exceções e runtime escolhido pelo UBT. Não são ligados os objetos /MT do console. Os próprios fontes canônicos permaneceram inalterados.

CoreProbe.h expõe somente resultado simples, sem tipos Unreal ou STL na fronteira de chamada. CoreProbe.cpp usa as APIs reais do núcleo e captura exceções antes de retornar. A camada Unreal só chama essa função quando o argumento opt-in RpgCoreProbe estiver presente; abertura comum não a dispara. Nesta entrega, o módulo não foi carregado/executado.

## Mesma bateria, dois ambientes

O executável standalone usa exatamente CoreProbe.cpp. Ele foi compilado e executado antes do UBT, com resultado:

```text
RPG_CORE_PROBE passed=1 checkpoints=11 failures=0 accepted=1 credits=10
```

Os onze checkpoints cobrem seleção válida, seleção repetida recusada sem mutação, remoção de dependências do membro excluído, passiva de abertura, requisito não atendido, alvo inválido, escolha automática pura, ação aceita/eliminação, recompensa única, roundtrip de dados v1 e restauração com tentativa nova sem crédito adicional. É um cenário pequeno de referência, não novo catálogo de habilidades ou ensaio completo de modos.

A DLL da engine contém a mesma função e os fontes nativos, porém **o resultado acima foi observado somente no standalone**. A próxima execução autorizada deverá produzir os mesmos campos no log Unreal; build não substitui essa comparação.

## Resultado da compilação

UBT: Succeeded, saída0, onze ações de build. MSVC14.52.37029/SDK10.0.26100.0, concorrência máxima2, remoto desabilitado. Tempo UBT observado16,06s nesta compilação incremental; não comparar diretamente com o primeiro build frio.

Compilados no módulo: CoreProbe.cpp, módulo Unreal e os cinco fontes nativos. DLL gerada em build/engine-eval/RpgEngineProbe/Binaries/Win64. Aviso de compilador não preferido preservado. Nenhuma nova ferramenta instalada, nenhum editor aberto, nenhuma regra de gameplay reescrita para Unreal e nenhum slot de jogador lido/escrito.

## Evidências

- docs/relatorios/atuais/ENG-01-BUILD.json: resultado atual, baseline e manifesto de16 arquivos.
- docs/relatorios/atuais/ENG-01-BUILD-MINIMO.json: aceite anterior do módulo sem núcleo, preservado.
- build/engine-eval/baseline/console.log: compilação/execução standalone da ponte.
- build/engine-eval/console.log e ubt.log: build UE, ignorados no Git.

Os213 testes standalone gerais foram preservados e não repetidos, pois não houve alteração dos fontes canônicos. Os onze checkpoints da ponte são uma bateria adicional específica; não foram somados aos213 como se fossem testes gerais da engine.

## Próximo e limites

Executar o opt-in em ambiente Unreal controlado, sem interface gráfica se tecnicamente adequado, comparando o marcador completo e o código de saída. Requer etapa de execução explicitamente autorizada; o script atual apenas compila e executa o baseline próprio.

Ainda sem validação de runtime UE, UMG, arena, empacotamento, salvamento com APIs da engine, Android/iOS, coordenadores SEQ/PAR ou desempenho gráfico. ENG-01 permanece aberto. Um resultado diferente deve ser investigado antes de prosseguir para interface visual.
