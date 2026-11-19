# RPG Urbano

Protótipo original de RPG por turnos com identidade urbana e musical. Núcleo em **C++20**, independente de engine; interface textual para validar regras antes da apresentação gráfica. Unreal Engine 5.8.4 já tem integração mínima de núcleo validada no Windows; apresentação gráfica, nome final, arte e publicação seguem em definição.

**[📊 Progresso geral — percentuais por área e frentes de trabalho](docs/projeto/PROGRESSO-GERAL.md)**

## Começar

1. Leia o [estado atual](docs/projeto/ESTADO.md), a [fila de tarefas](docs/projeto/TAREFAS.md) e o [painel de blocos](docs/batalhas/PAINEL-BLOCOS.md).
2. Pré-requisito validado: Windows, Visual Studio 2026 com desenvolvimento C++ desktop, MSVC e Windows SDK. Versões registradas em [AMBIENTE.json](docs/relatorios/atuais/AMBIENTE.json); o teste real de integração com a Unreal está registrado separadamente, conforme o escopo abaixo.
3. Em PowerShell com scripts permitidos pela política local, execute na raiz:

```powershell
.\scripts\testar-combate.ps1
```

O script descobre o compilador instalado, compila código próprio, executa testes e gera `build/combate/`. Não instala dependências nem muda a política de segurança. Se o Windows bloquear scripts, use um ambiente permitido pelo administrador; não use bypass. Os executáveis já compilados podem ser abertos normalmente.

4. Opcionalmente escolha a equipe com `.\Jogar.cmd --equipe A1 A3` (catálogo atual: A1/A2/A3, sem repetições). Abertura sem argumentos mantém os três; reiniciar mantém a seleção. Contrato em [CIC-01](docs/projeto/CIC-01.md).

5. Abra **Jogar.cmd** para equipes ou **Jogar-Classico.cmd** para o clássico. Binários são locais e não acompanham o Git; uma cópia nova precisa ser compilada.

No perfil de equipes: `1 B3` usa a básica; `2 B3` usa a segunda posição ativa; `3 A3` usa a proteção de emergência quando A3 age e está com até 50% da vida. `ajuda`, `estado`, `reiniciar` e `sair` orientam a sessão. A2/B2 têm a passiva Preparação; A3/B3 têm Golpe duplo preparado, com ataque ativo e escudo passivo de abertura. Notas sem definição executável continuam informativas. O time B de equipes escolhe especiais por prioridade configurada (padrão 3, depois 2) e usa a básica quando necessário; o log identifica a escolha. O time A continua manual. O clássico mantém ataque e defesa e a automação básica histórica.

Conclusões agora registram recompensa provisória: +10 créditos de teste por vitória A por eliminação; demais finais concedem zero. Estado mostra tentativa/saldo, e reiniciar mantém o saldo dentro da mesma sessão. Use `salvar` para gravar equipe/progresso. Depois abra `.\Jogar.cmd --continuar` ou `.\Jogar-Classico.cmd --continuar`. A continuação começa outra batalha; não retoma HP/turnos. Sem salvar, avanços desde o último checkpoint são perdidos. Slots em `build/saves`, ignorados pelo Git. [Contrato do save](docs/projeto/CIC-01-SAVE-R01.md). [Contrato da recompensa](docs/projeto/CIC-01-RECOMPENSA-R01.md).

## Organização

| Caminho | Responsabilidade |
|---|---|
| `include/rpg/` | Interfaces públicas do núcleo C++ |
| `src/` | Implementações das regras e sessão textual |
| `app/` | Pontos de entrada dos executáveis próprios |
| `tests/` | Testes de comportamento, cenários e invariantes |
| `experiments/unreal/` | Fonte de experimentos Unreal isolados, sem mover o protótipo |
| `tools/` | Diagnóstico e ferramentas próprias auxiliares |
| `scripts/` | Compilação, verificações e rotinas locais |
| `docs/projeto/` | Visão, arquitetura, decisões, estado e planejamento |
| `docs/batalhas/` | Contratos, pesquisa, evidências e relatórios por bloco |
| `docs/relatorios/atuais/` | Resultados consolidados mais recentes |
| `.github/` | Modelos para issues e revisão de contribuições |
| `build/` | Binários, objetos e logs gerados; ignorados pelo Git |

A estrutura C++ atual já separa interfaces, implementação e aplicação. Não há diretórios vazios de engine/assets nem dependências antecipadas. Ao introduzir recursos, registre autoria/licença e preserve os fontes; arquivos grandes devem ter estratégia própria antes do primeiro commit.

## Verificação e limites

- **213/213 testes gerais standalone aprovados:** [resultado vigente](docs/relatorios/atuais/COMBATE.json).
- **Ponte Windows compilada e executada na Unreal 5.8.4:** os mesmos11 checkpoints passaram em dois processos independentes, com saída zero e resultado idêntico ao baseline. [Aceite da execução](docs/projeto/ENG-01-RUNTIME.md) e [JSON](docs/relatorios/atuais/ENG-01-RUNTIME.json).
- **HAB-01 e CIC-01 mínimos concluídos:** [habilidades](docs/batalhas/HAB-01-FINAL.md) e [ciclo local](docs/projeto/CIC-01.md).

A validação Unreal usou commandlet sem interface gráfica, com NullRHI. Abertura do editor, mapa, menus e encerramento foram observados em RTX 3060/D3D12/SM5; [validação gráfica parcial](docs/projeto/ENG-01-GRAFICO.md), com diagnóstico regional posterior e opt-out observado; isolamento de rede não validado. O [pacote Windows básico](docs/projeto/ENG-01-PACOTE-RUNTIME.md) foi gerado, abriu o mapa fora do editor e encerrou com saída0. Arena, Windows limpo, desempenho, distribuição e mobile não foram validados. Os11 checkpoints não são toda a suíte de213 testes, nem22 casos distintos por terem rodado duas vezes.

Padrão de trabalho Unreal: priorizar inglês (`-culture=en` nos testes). Os 14 erros em pt-BR pertencem a três testes internos que passaram em inglês; [diagnóstico](docs/projeto/ENG-01-GRAFICO-DIAGNOSTICO.md). O jogo poderá ser em português por localização separada, ainda a definir e validar. O [fluxo visual3×3](docs/projeto/VIS-01C.md) já foi observado no pacote até vitória e reinício; 6 testes nativos do adaptador aprovados além dos213 anteriores. Próximo incremento: fechar as pendências de leitura/alvos e formações/resoluções do VIS-01; uso futuro de programas exige autorização específica. O protótipo não comprova servidor, desempenho gráfico ou equivalência a outro jogo. Não há revisão pendente da ponte entregue sem mudança/falha pertinente.

[Guia de contribuição](CONTRIBUTING.md) · [Segurança e dados](SECURITY.md) · [Índice da documentação](docs/README.md) · [Histórico de organização](CHANGELOG.md).

## Direitos e conteúdo

Nenhuma licença de distribuição pública foi escolhida. O repositório não concede uma licença aberta para redistribuição; a decisão cabe ao titular. Conteúdo de terceiros precisa de autorização/licença própria registrada. Não incluir instalações de jogos, recursos protegidos, chaves, contas ou credenciais.


Apresentações da arena: padrão em profundidade (aliados próximos, inimigos ao fundo) e lateral clássica preservada por -ArenaLayout=Classic. [Detalhes e validação pendente](docs/projeto/VIS-01-LAYOUTS.md). Essas opções visuais não mudam as regras de combate e não alteram os comandos do console.

Interação gráfica compartilhada com o clássico: `-CombatProfile=Classic` habilita suas regras de ataque/defesa; `-ArenaLayout=Classic` continua independente e só muda a câmera. [Implementação, validação e próximo aceite](docs/projeto/VIS-01-CLASSICO-INTERACAO.md). Compilação aprovada; pacotes anteriores não incluem a revisão e seu aceite visual está pendente.

Pesquisa e decisões: [questionário central por prioridade](docs/projeto/PENDENCIAS.md). ESC-01–07 estão definidos e implementados no caso comum; demais pontos continuam organizados por prioridade. [Índice atualizado](docs/README.md).

Escudo padrão por habilidade:1% do PV máximo do aplicador, para baixo com mínimo de1 ponto na aplicação válida, por3 turnos naturais do protegido, com reaplicação por maior força/duração. [Contrato, testes e limites](docs/projeto/ESCUDO-PADRAO.md). A terceira habilidade ativa usa essa regra; passivas fixas históricas não foram convertidas. Aceite visual pendente.
