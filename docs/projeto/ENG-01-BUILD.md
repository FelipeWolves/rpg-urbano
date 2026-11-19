# ENG-01 — primeiro build Unreal aprovado

18/11/2026. **✅ Compilação/vinculação de módulo próprio mínimo aprovada; ENG-01 continua aberto.**

## Resultado

- Engine instalada: UE5.8.4, CL58902052.
- Alvo: RpgEngineProbeEditor, Win64, Development.
- Ferramentas efetivamente selecionadas: MSVC14.52.37029 (pasta14.52.37025), SDK10.0.26100.0 e dotnet10 empacotado.
- Resultado UBT: Succeeded, saída0. Produzida UnrealEditor-RpgEngineProbe.dll no projeto de avaliação gerado.
- Seis ações de build, concorrência limitada a2; executor local, remoto desabilitado. Tempo UBT desta execução49,73s; não é benchmark geral.
- Editor não aberto e módulo não carregado/executado. Nenhuma ação de combate, mapa, shader ou pacote testado nesta entrega.

Houve aviso explícito de compilador mais novo que a versão preferida14.50.35717. MSVC14.29 foi diagnosticado como indisponível por estar abaixo do mínimo; ele não foi usado. O sucesso é evidência para este módulo/headers/bibliotecas, não certificação irrestrita do toolset14.52. Não houve instalação ou downgrade de ferramentas, supressão de checagem ou alteração dos requisitos da engine.

## Isolamento e reprodução

Fonte: experiments/unreal/RpgEngineProbe. Saídas: build/engine-eval/RpgEngineProbe. Script próprio scripts/testar-unreal.ps1 recebe EngineRoot explicitamente e confere5.8.4 antes de agir. Cópia gerada reproduz fonte versionada; não manter segunda cópia editável do jogo.

Projeto sem plugins adicionais, C++20, exceções por módulo, unidades separadas e sem PCH do módulo. Configuração V7 e includes Unreal5_8 conferidos no modelo instalado. Não há main próprio nem core de combate no módulo ainda; ele usa headers/bibliotecas da engine.

A chamada direta ao UBT empacotado evita a etapa opcional de recompilar UBT do Build.bat. NoEngineChanges impede solicitar rebuild de componentes da engine, mas não torna todo o ambiente somente leitura: a ferramenta usa seus caches/traces/configurações normais. Primeiro acesso foi bloqueado pelo isolamento, antes da compilação; houve liberação da ferramenta para executar o build necessário, sem abrir o editor.

Não foram alterados os fontes de combate, saves do jogador ou configurações globais de VS. Variáveis temporárias do processo foram restauradas pelo script. Os213 testes standalone existentes não foram repetidos, pois a mudança ficou no projeto de avaliação e na automação desse build.

## Evidências

- docs/relatorios/atuais/ENG-01-BUILD.json: versão, alvo, código de saída, DLL presente e linhas da toolchain.
- build/engine-eval/ubt.log e console.log: logs locais ignorados pelo Git.
- DLL/PDB/receipts permanecem em build ignorado, não entram no commit.

## Próximo aceite

Conectar os fontes do núcleo por ponte/adaptador pequeno sem duplicar regra na UI; primeiro compilar essa ponte. Depois, em execução autorizada separada, comparar ação aceita, rejeição sem mutação e resultado com o standalone. Só então testar arena/editor, empacotamento e plataformas adicionais nos respectivos escopos. ENG-01 não foi encerrado por este build básico.
