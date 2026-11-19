# ENG-01 — execução da ponte concluída

18/11/2026. **✅ Etapa de compilação e execução da ponte Windows concluída.** A avaliação gráfica/empacotamento/multiplataforma não faz parte deste aceite e permanece pendente no ENG-01 e marcos seguintes.

## Resultado observado dentro da Unreal

UE5.8.4, CL58902052, executou URpgCoreProbeCommandlet em dois processos independentes. Ambos encerraram normalmente com código0, um único marcador por processo e correspondência exata com o baseline standalone:

```text
RPG_CORE_PROBE passed=1 checkpoints=11 failures=0 accepted=1 credits=10
```

Execuções:40,15s e8,13s, sem timeout. A diferença inclui efeitos de inicialização/cache; não é benchmark de gameplay, gráficos ou mobile. São os mesmos onze checkpoints em duas execuções, não22 testes diferentes.

Seleção, recusa atômica, dependências removidas, passiva inicial, condição bloqueada, alvo inválido, escolha automática, eliminação, crédito único, formato v1 em memória e restauração sem concessão duplicada foram observados pelo código nativo dentro da engine. Os213 testes gerais anteriores permanecem como regressão standalone; não foram todos executados na Unreal.

## Execução delimitada

A commandlet própria deriva de UCommandlet, desativa os papéis de cliente/servidor/editor e chama a mesma função CoreProbe.cpp. O retorno dessa função determina a saída0/1. Não duplica regras de combate. Não abre interface gráfica, usa NullRHI e não precisa de mapa, arena ou assets próprios.

scripts/testar-unreal-runtime.ps1 confere versão da engine, baseline e hashes de18 arquivos canônicos/stage antes de iniciar. Executa duas vezes, exige exatamente um marcador atual por processo e saída zero, grava logs novos por execução e impõe180s por processo. Em timeout, encerra somente sua árvore de processos.

Não houve acesso a saves do jogador ou execução do protótipo original externo. Estado e roundtrip do save do teste são dados artificiais em memória. Ferramentas da Unreal podem criar seus caches/traces/configurações normais; não houve pedido de alteração dos fontes/binários da engine. Nenhum launcher, serviço de jogo, plugin externo, SDK adicional ou conteúdo proprietário foi introduzido.

## Ajuste necessário e histórico preservado

A primeira abordagem abriu a inicialização geral do editor de linha de comando, sem interface/NullRHI, e atingiu180s carregando módulos antes do marcador do núcleo. Ela foi encerrada pelo limite e não foi tratada como teste aprovado. Registro ENG-01-RUNTIME-INICIAL.json preserva timeout/ausência do marcador.

Foi criada a commandlet específica, recompilada a ponte (13 ações UBT, saída0) e repetido o baseline standalone. A rota final usa -run=RpgCoreProbe, sem o opt-in de startup simultâneo, evitando execução dupla. A opção antiga -RpgCoreProbe continua disponível para diagnóstico futuro explícito, não é a rota do teste final.

## Evidências e reprodução

- docs/relatorios/atuais/ENG-01-RUNTIME.json: duas saídas, marcadores e comparação ao baseline.
- ENG-01-BUILD.json: build atual, baseline e manifesto de18 arquivos.
- ENG-01-RUNTIME-INICIAL.json: tentativa inicial não aprovada.
- Logs completos em build/engine-eval/runtime, ignorados pelo Git.

Comandos na raiz, em ambiente com scripts permitidos:

```powershell
.\scripts\testar-unreal.ps1 -EngineRoot 'D:\Unreal Engine\UE_5.8'
.\scripts\testar-unreal-runtime.ps1 -EngineRoot 'D:\Unreal Engine\UE_5.8'
```

Não é necessário recompilar quando fonte/engine não mudaram e o manifesto continua válido. A execução pode exigir liberação de acesso ao diretório/caches da engine no ambiente do Codex. Não alterar política do Windows ou requisitos do compilador para evitar essa restrição.

## Aceite e próximo limite

Concluído: instalar/verificar engine fornecida pelo usuário, compilar módulo mínimo e ponte, rodar a mesma bateria sob runtime Unreal e comparar duas execuções ao baseline. MSVC14.52/SDK26100 continuam com evidência deste escopo; aviso de compilador não preferido preservado, sem prometer estabilidade irrestrita.

Pendente para uso visual/produção: abertura gráfica controlada e comportamento da GPU, ponte/UI da arena, save usando caminhos/APIs da aplicação final, pacote distribuível, Android/iOS e modos SEQ/PAR dentro da engine. Próximo incremento deve tratar abertura gráfica/arena em escopo próprio, sem repetir esta bateria sem mudança ou falha pertinente.
