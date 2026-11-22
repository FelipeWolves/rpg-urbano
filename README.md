> **Estado vigente — 22/11/2026:** demo **~28% (26–30%)**, ponderado28,03%. Cliques/vídeo/skip/esferas/seletor aceitos no Windows. LAB-PVE-02 entregue em código (Ritual no Terminal), aguardando aceite visual; próximo: LAB-PVE-03, ondas/fases; não depende de nova entrega. [Progresso](docs/projeto/PROGRESSO-GERAL.md) · [Tarefas](docs/projeto/TAREFAS.md) · [Sua preparação](entregas/NECESSIDADES-ATUAIS.md).
# RPG Urbano — Mundus Lupis Disco / codinome LOBO

Protótipo original de RPG urbano e musical. Núcleo C++20 independente, apresentação atual na Unreal Engine 5.8.4 e laboratório textual preservado. Demo recortada: Felipe, uma arena e inimigos provisórios; não é o escopo comercial completo.

**[Progresso por área: aproximadamente 22% da demo](docs/projeto/PROGRESSO-GERAL.md)** · [Estado](docs/projeto/ESTADO.md) · [Próximas tarefas](docs/projeto/TAREFAS.md) · [Entregas](entregas/README.md)

## Abrir na Unreal sem comandos

1. Abra [RpgEngineProbe.uproject](experiments/unreal/RpgEngineProbe/RpgEngineProbe.uproject), pelo Explorador ou pelo navegador de projetos da Unreal.
2. Alternativa nesta máquina: **Abrir-Editor.cmd**, na raiz. Chama a Unreal diretamente, sem PowerShell, priorizando inglês no editor.
3. O mapa próprio `PrototypeHub` abre para edição. Clique **Play** para mostrar o hub; Batalha leva à arena. Pare Play para voltar a editar.

Compilação, mapa salvo, reabertura normal pelo `.uproject` e Play foram observados em21/11. [Guia completo, evidências e limitações](docs/projeto/UNREAL-PROJETO-ESTAVEL.md). A associação normal usa o idioma preferido atual do editor; localização do jogo é independente.

O projeto fixo está em `experiments/unreal/RpgEngineProbe`, apesar do nome histórico experiments. Edite esse projeto; `build/p` contém snapshots antigos preservados. Source, Config e o mapa próprio são versionados; caches, binários e preferências locais não. Imagens/vídeos não acompanham o Git. Em outro checkout, recursos locais precisam ser fornecidos e o C++ compilado; `scripts/preparar-editor-estavel.ps1` prepara somente recursos ausentes, conferindo hashes e preservando arquivos diferentes.

**Abrir-Preview.cmd** abre a prévia do projeto estável atual. **Abrir-Preview-Mobile.cmd** abre sua apresentação retrato no Windows; não é um APK. Mudanças C++ precisam de compilação; salvar um mapa não recompila código.

## Organização

| Caminho | Responsabilidade |
|---|---|
| include/rpg / src | Interfaces e implementações canônicas do núcleo |
| app / tests | Executáveis e testes próprios |
| experiments/unreal/RpgEngineProbe | Projeto Unreal estável, apresentação e mapa |
| data | Catálogos e configurações de dados versionados |
| assets / entregas | Recursos locais e materiais recebidos; mídias fora do Git |
| scripts / tools | Automação e verificações próprias |
| docs/projeto | Estado, decisões, tarefas e estimativas |
| docs/batalhas / docs/relatorios | Contratos, aceites e resultados consolidados |
| build | Compilados, snapshots e logs locais ignorados |

O módulo Unreal compila fontes canônicas por unidades de ligação; o núcleo não depende da engine. A apresentação ainda usa Canvas/C++; não é um conjunto completo de Widgets/Blueprints editáveis visualmente. Não migrar para UMG sem avaliar um bloco concreto.

## Testes e laboratório textual

Em ambiente PowerShell que permita scripts: `scripts/testar-combate.ps1`. Não instala ferramentas nem altera a política do Windows. Jogar.cmd e Jogar-Classico.cmd usam o compilado local; seleção/save e recompensa continuam com os contratos do laboratório, não economia comercial.

Histórico:213 testes gerais e suítes incrementais aprovados; ponte anterior11 checkpoints em2 processos. Nesta entrega,11 checkpoints passaram no novo projeto estável, saída0. Não somar esses números nem afirmar que213 rodaram dentro da engine. [Painel de blocos](docs/batalhas/PAINEL-BLOCOS.md).

## Estado e próximo trabalho

Base Team compartilhada no Terminal e piloto Felipe; personagens e saves individuais. Core SIS/REG/HAB e ESC/SIS/PvE entregues em seus escopos; menus/HUD dos modos avançados ainda por integrar. [Fila atual](docs/projeto/FILA-DESENVOLVIMENTO-INDEPENDENTE.md). Save/reabertura, tutorial, resoluções e futuras telas aguardam aceites próprios. Android, MCP e clássico ampliado adiados.

## Colaboração e direitos

[Contribuição](CONTRIBUTING.md) · [Segurança](SECURITY.md) · [Índice](docs/README.md) · [Histórico](CHANGELOG.md).

Commits locais autorizados; push fica com o usuário. Preservar mudanças dele, não acessar instalações de jogos, contas ou conteúdo protegido. Sem licença pública escolhida; recursos de terceiros requerem direitos próprios. Logo atual continua provisória até licença/substituição. O controle de tela foi encerrado a pedido do usuário.
