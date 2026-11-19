# RpgEngineProbe — fonte do teste de compilação

Projeto próprio mínimo para ENG-01, alvo RpgEngineProbeEditor / Win64 / Development. Sem mapa, conteúdo ou dependência externa; fontes selecionados do núcleo agora compilados no módulo. Não é o projeto visual final.

Esta pasta é a fonte versionada. scripts/testar-unreal.ps1 copia os seis arquivos do projeto e cinco fontes/cinco headers canônicos do núcleo para build/engine-eval/RpgEngineProbe, onde ficam resultados gerados. Edite esta fonte; não edite a cópia gerada. Não abrir esta pasta no editor como parte do teste de compilação.

Comando validado a partir da raiz, em ambiente com scripts permitidos:

```powershell
.\scripts\testar-unreal.ps1 -EngineRoot 'D:\Unreal Engine\UE_5.8'
```

O script valida a versão5.8.4, usa dotnet/UBT distribuídos com a engine e os binários precompilados dela. Não recompila UBT nem chama o editor. Flags incluem NoEngineChanges e desativação de executores distribuídos; dois processos de ação em paralelo no máximo. UBA pode aparecer como executor local, com execução remota desabilitada.

Logs em build/engine-eval; resultado resumido em docs/relatorios/atuais/ENG-01-BUILD.json. UBT também pode criar caches/configuração/traces normais fora do projeto. No ambiente restrito do Codex, houve bloqueio inicial de acesso ao diretório da engine; execução foi autorizada pela ferramenta antes da compilação real. Não alterar política de scripts, engine ou compilador para contornar rejeições.

Configuração V7/ordem de includes Unreal5_8 foi conferida no modelo instalado; C++20/exceções configurados por módulo. Unity e PCH do módulo desligados para reduzir variáveis no primeiro ensaio. Isso não escolhe as configurações finais do jogo.

O script também compila/executa CoreProbe.cpp em standalone e exige11 checkpoints antes do UBT. Mesmo código é ligado ao módulo; a flag opt-in RpgCoreProbe dispara sua execução quando o módulo for carregado. O script de build não abre a engine. Próximo: execução controlada autorizada dentro da Unreal e comparação com baseline. Build bem-sucedido não executa o teste de batalha nem prova renderização, pacote, mobile ou estabilidade geral do compilador.
