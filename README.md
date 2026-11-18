# RPG Urbano

Protótipo original de RPG por turnos com identidade urbana e musical. Núcleo em **C++20**, independente de engine; interface textual para validar regras antes da apresentação gráfica. Engine, nome final, arte e publicação ainda em definição.

## Começar

1. Leia o [estado atual](docs/projeto/ESTADO.md), a [fila de tarefas](docs/projeto/TAREFAS.md) e o [painel de blocos](docs/batalhas/PAINEL-BLOCOS.md).
2. Pré-requisito validado: Windows, Visual Studio 2026 com desenvolvimento C++ desktop, MSVC e Windows SDK. Versões registradas em [AMBIENTE.json](docs/relatorios/atuais/AMBIENTE.json); isso não garante compatibilidade com uma futura engine.
3. Em PowerShell com scripts permitidos pela política local, execute na raiz:

```powershell
.\scripts\testar-combate.ps1
```

O script descobre o compilador instalado, compila código próprio, executa testes e gera `build/combate/`. Não instala dependências nem muda a política de segurança. Se o Windows bloquear scripts, use um ambiente permitido pelo administrador; não use bypass. Os executáveis já compilados podem ser abertos normalmente.

4. Abra **Jogar.cmd** para equipes ou **Jogar-Classico.cmd** para o clássico. Binários são locais e não acompanham o Git; uma cópia nova precisa ser compilada.

No perfil de equipes: `1 B3` usa a básica; `2 B3` usa a segunda posição ativa; `3 A3` usa a proteção de emergência quando A3 age e está com até 50% da vida. `ajuda`, `estado`, `reiniciar` e `sair` orientam a sessão. A2/B2 têm a passiva Preparação; A3/B3 têm Golpe duplo preparado, com ataque ativo e escudo passivo de abertura. Notas sem definição executável continuam informativas. O time B de equipes escolhe especiais por prioridade configurada (padrão 3, depois 2) e usa a básica quando necessário; o log identifica a escolha. O time A continua manual. O clássico mantém ataque e defesa e a automação básica histórica.

## Organização

| Caminho | Responsabilidade |
|---|---|
| `include/rpg/` | Interfaces públicas do núcleo C++ |
| `src/` | Implementações das regras e sessão textual |
| `app/` | Pontos de entrada dos executáveis próprios |
| `tests/` | Testes de comportamento, cenários e invariantes |
| `tools/` | Diagnóstico e ferramentas próprias auxiliares |
| `scripts/` | Compilação, verificações e rotinas locais |
| `docs/projeto/` | Visão, arquitetura, decisões, estado e planejamento |
| `docs/batalhas/` | Contratos, pesquisa, evidências e relatórios por bloco |
| `docs/relatorios/atuais/` | Resultados consolidados mais recentes |
| `.github/` | Modelos para issues e revisão de contribuições |
| `build/` | Binários, objetos e logs gerados; ignorados pelo Git |

A estrutura C++ atual já separa interfaces, implementação e aplicação. Não há diretórios vazios de engine/assets nem dependências antecipadas. Ao introduzir recursos, registre autoria/licença e preserve os fontes; arquivos grandes devem ter estratégia própria antes do primeiro commit.

## Verificação e limites

A última entrega funcional aprovou **187/187 testes**; consulte o [resultado vigente](docs/relatorios/atuais/COMBATE.json) e o [aceite final HAB-01](docs/batalhas/HAB-01-FINAL.md). O protótipo não comprova servidor, desempenho gráfico ou equivalência a outro jogo. Próximo bloco funcional: CIC-01 parte 1, composição/seleção da equipe em memória.

[Guia de contribuição](CONTRIBUTING.md) · [Segurança e dados](SECURITY.md) · [Índice da documentação](docs/README.md) · [Histórico de organização](CHANGELOG.md).

## Direitos e conteúdo

Nenhuma licença de distribuição pública foi escolhida. O repositório não concede uma licença aberta para redistribuição; a decisão cabe ao titular. Conteúdo de terceiros precisa de autorização/licença própria registrada. Não incluir instalações de jogos, recursos protegidos, chaves, contas ou credenciais.
