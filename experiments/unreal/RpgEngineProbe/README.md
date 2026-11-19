# RpgEngineProbe — fonte do teste de compilação

Experimento próprio ENG-01/VIS-01, com alvos Editor e Game / Win64 / Development. Arena provisória em código e mapa Template_Default da engine; sem conteúdo do Dislyte ou dependência externa instalada. Não é o projeto visual final.

Esta pasta é a fonte versionada. scripts/testar-unreal.ps1 copia os oito arquivos do projeto e cinco fontes/cinco headers canônicos do núcleo para build/engine-eval/RpgEngineProbe, onde ficam resultados gerados. Edite esta fonte; não edite a cópia gerada. Não abrir esta pasta no editor como parte do teste de compilação.

Comando validado a partir da raiz, em ambiente com scripts permitidos:

```powershell
.\scripts\testar-unreal.ps1 -EngineRoot 'D:\Unreal Engine\UE_5.8'
```

O script valida a versão5.8.4, usa dotnet/UBT distribuídos com a engine e os binários precompilados dela. Não recompila UBT nem chama o editor. Flags incluem NoEngineChanges e desativação de executores distribuídos; dois processos de ação em paralelo no máximo. UBA pode aparecer como executor local, com execução remota desabilitada.

Logs em build/engine-eval; resultado resumido em docs/relatorios/atuais/ENG-01-BUILD.json. UBT também pode criar caches/configuração/traces normais fora do projeto. No ambiente restrito do Codex, houve bloqueio inicial de acesso ao diretório da engine; execução foi autorizada pela ferramenta antes da compilação real. Não alterar política de scripts, engine ou compilador para contornar rejeições.

Configuração V7/ordem de includes Unreal5_8 foi conferida no modelo instalado; C++20/exceções configurados por módulo. Unity e PCH do módulo desligados para reduzir variáveis no primeiro ensaio. Isso não escolhe as configurações finais do jogo.

O script também compila/executa CoreProbe.cpp em standalone e exige11 checkpoints antes do UBT. Mesmo código é ligado ao módulo; a flag opt-in RpgCoreProbe dispara sua execução quando o módulo for carregado. O script de build não abre a engine. Essa execução foi validada pela commandlet descrita abaixo. Build bem-sucedido não executa o teste de batalha nem prova renderização, pacote, mobile ou estabilidade geral do compilador.

## Execução da ponte validada

A rota automatizada agora usa uma commandlet própria: scripts/testar-unreal-runtime.ps1 -EngineRoot seguido da pasta da engine. Ela chama Run uma vez, sem opt-in de startup, e exige duas execuções iguais ao baseline. Saída0 e11 checkpoints aprovados em ambos. Limite180s por processo e logs em build/engine-eval/runtime. Não abre interface gráfica nem usa saves do jogador. Resultado em ENG-01-RUNTIME.json; detalhes em docs/projeto/ENG-01-RUNTIME.md. Inicialização geral anterior teve timeout e não deve ser repetida como bateria padrão.

## Preparação de pacote Windows

Alvo Game e Config compilados na etapa de pacote Development/Win64, build/cook/archive com saída0. Aplicativo abriu Template_Default fora do editor e encerrou com saída0; aceite básico limitado ao mapa; a arena foi observada em uma revisão posterior descrita abaixo. scripts/preparar-pacote-unreal.ps1 cria stage novo e plano sem invocar ferramentas. Detalhes em docs/projeto/ENG-01-PACOTE-PREPARACAO.md, ENG-01-PACOTE-BUILD.md e ENG-01-PACOTE-RUNTIME.md. O script empacotar-unreal.ps1 executa o plano somente com autorização, sem abrir aplicativo. Stage de pacote curto em build/p, preservando a tentativa inicial de caminhos longos. O stage anterior e o build do editor permanecem separados. Priorizar inglês nas ferramentas; localização do jogo em português continua independente e pendente.


## Arena provisória VIS-01

ProbeArenaMode/HUD usam Canvas e InputCore, sem plugin novo. O adaptador e o cenário são fontes canônicos adicionais copiados pelo preparador de pacote (32 entradas no preparador atual). Batalha3×3 até vitória/reinício foi observada no pacote revisado, sem editor. O script antigo testar-unreal.ps1 continua próprio da ponte histórica e não inclui automaticamente esses novos arquivos. Para a arena usar preparar-pacote-unreal.ps1/empacotar-unreal.ps1 com autorização específica. Aceite visual integral permanece parcial em docs/projeto/VIS-01C.md; inglês no ambiente e localização do produto separados.

## Revisão de apresentação e interação

Fontes compartilhados canônicos: arena_layout e battle_presentation em include/rpg e src, copiados para o stage com os demais arquivos. O preparador atual confere/copia32 entradas; não editar as cópias em build/p. O script histórico da ponte não é o fluxo de pacote da arena.

- Padrão: câmera em profundidade e CombatProfile Team.
- `-ArenaLayout=Classic`: apenas câmera lateral; não habilita defesa.
- `-CombatProfile=Classic`: regras clássicas com ataque/defesa, combináveis com qualquer câmera. Referência sem especiais/inventário cadastrados.
- `-TargetMarker=Diamond`: variante geométrica do marcador; Ring é o padrão.

As câmeras anteriores foram observadas no pacote; a interação revisada (painéis, escudo real, consulta prolongada e novo perfil clássico) passou nos testes nativos e build Editor, sem novo pacote/inspeção visual. Não usar o pacote anterior para validar código que ele não contém. [Entrega e evidências](../../../docs/projeto/VIS-01-CLASSICO-INTERACAO.md), [questionário](../../../docs/projeto/PENDENCIAS.md).

Preparar não compila nem abre; empacotar compila/cook/stage/archive sem abrir o aplicativo. O roteiro testar-apresentacao-arena.ps1 abre o pacote de layout registrado e depende de autorização específica; ele não é um teste genérico da revisão mais recente. Não abrir editor, pacote ou mudar rede por inferência documental.
## Escudo padrão

A terceira ativa do cenário usa StandardShield (1% do máximo do aplicador,3 turnos naturais do protegido). Consultas/vida/escudo recebem os pontos reais do mesmo núcleo. Build Editor desta revisão aprovado; pacote anterior não contém a mudança. Passivas fixas de abertura preservadas. Contrato em [ESCUDO-PADRAO.md](../../../docs/projeto/ESCUDO-PADRAO.md); sem autorização implícita para abrir editor ou aplicativo.
