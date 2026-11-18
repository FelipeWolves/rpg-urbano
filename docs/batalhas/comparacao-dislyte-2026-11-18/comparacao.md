# Comparação de evidências — Dislyte e RPG Urbano

18/11/2026. **Nosso núcleo tem cobertura reproduzível no escopo contratado; não há evidência suficiente para afirmar que o jogo é melhor, mais rápido ou equivalente ao Dislyte.**

## O que foi conferido agora

Relatórios de analise-dislyte reaproveitados. Oito arquivos locais conferidos: 8/8 com presença, tamanho e data de modificação iguais ao inventário anterior. Isso não certifica igualdade bit a bit nem o restante da instalação. Engine.ini contém 38 referências Paths=, das quais 36 são caminhos de conteúdo de plugins; as outras são caminhos-base. Confirma a contagem histórica de 36 referências de plugins, não 38 plugins ativos.

As contagens históricas de 6.572 arquivos, 5.576 PAKs e seus índices criptografados não foram recontadas nem revalidadas. Os dois chunks ausentes e a referência XGame.exe são divergências históricas de manifestos; não explicam limitações de combate ou qualidade do produto.

Foram reutilizados 131 testes aprovados do projeto, 168 cenários/replays e 25 formações AP/replays. Não houve nova compilação, execução do cliente, acesso à rede, extração ou escrita na origem. Leitura direta de conteúdo limitada às linhas Paths= do Engine.ini. Não foram abertos saves, logs, dumps ou credenciais.

## Matriz comparativa

“Validado” significa testado contra nosso contrato. Não significa paridade externa. “Não implementado” é trabalho futuro, não um defeito dos blocos encerrados. CSV/JSON detalham evidências, fontes e próximos dados úteis por linha.

| ID | Área | Situação própria | Conclusão comparativa |
|---|---|---|---|
| C01 | Engine e arquitetura | Implementado no núcleo | Arquiteturas diferentes; superioridade não mensurável |
| C02 | Integridade e distribuição | Entrega parcial de produto | Não comparável diretamente |
| C03 | Formações 1 a 5 | Validado no perfil próprio | Paridade de tamanho parcial; regras não verificadas |
| C04 | SPD e barra de ação | Validado no perfil próprio | Sem equivalência de fórmula demonstrada |
| C05 | Recargas e turnos extras | Validado no perfil próprio | Contrato próprio; paridade completa não verificada |
| C06 | Turno geral e limites | Validado no perfil próprio | Não chamar orçamento de ações de contador universal do Dislyte |
| C07 | Multi-hit e alvos dinâmicos | Validado no perfil próprio | Bounce aleatório não implementado; equivalência não verificada |
| C08 | Reflexão, explosão e FIFO | Validado no perfil próprio | R04 não é especificação oficial comprovada do Dislyte |
| C09 | Prevenção, revive e vitória | Validado no perfil próprio | Decisão autoral fechada; equivalência externa não certificada |
| C10 | Escudos, dano e atributos | Implementação parcial | Não comparável em balanceamento |
| C11 | Guarda voluntária | Diferença por decisão | Extensão própria; não prova vantagem de experiência |
| C12 | Confrontos consecutivos | Validado no perfil próprio | Cobertura estrutural; paridade de conteúdo não verificada |
| C13 | Três times sucessivos | Validado no perfil próprio | Estrutura testada; habilidades e balanceamento distintos |
| C14 | Três frentes independentes | Validado no perfil próprio | Não demonstra threads nem equivalência à simultaneidade original |
| C15 | Chefe compartilhado | Não implementado | Sem evidência suficiente para escolher modelo por arquivos |
| C16 | Segurança de cadeias | Validado no perfil próprio | Proteção demonstrada apenas no nosso motor |
| C17 | Reprodutibilidade e desempenho | Validação funcional | Nenhuma prova de maior desempenho ou estabilidade geral |
| C18 | IA e catálogo de habilidades | Implementação parcial | Cobertura própria limitada; catálogo original não comparado |
| C19 | Personagens 3D e animações | Não implementado | Recursos protegidos não avaliados nem reaproveitados |
| C20 | Artes 2D e interface | Não implementado como interface jogável | Não há comparação de experiência ou qualidade visual |
| C21 | Músicas e áudio | Não implementado | DLL de áudio não demonstra trilha, latência ou qualidade musical |
| C22 | DJ Contest | Não implementado | Sem base de comparação funcional |
| C23 | Progressão e persistência | Não implementado | Sem comparação de economia/progressão |
| C24 | Offline, serviços e plataformas | Laboratório local validado | Vantagem de controle do nosso código, não superioridade do jogo completo |

## O que já podemos afirmar

Temos controle do código, contratos explícitos, testes e liberdade para criar perfis próprios sem depender de serviços. Essa é uma capacidade demonstrada do laboratório, não prova de que a arquitetura interna do outro jogo seja inferior. Guarda opcional e regras configuráveis são diferenças de design, não melhorias comprovadas.

Ainda faltam chefe compartilhado, interação jogável, catálogo amplo de habilidades, balanceamento, arte, animação, áudio, progressão e distribuição. Por isso a comparação com um jogo completo não tem resultado de superioridade. O lote de 1000 simulações sem renderização não mede desempenho gráfico, servidor ou consumo mobile.

## Próximos passos que trazem evidência útil

1. PAR-02: definir HP único, partes ou avatares/score; os arquivos atuais não resolvem PEN-05.
2. Para fidelidade, obter casos observáveis com modo, versão/data, atributos, sequência de ações e resultado esperado; manter decisões autorais separadas de regras verificadas externamente.
3. Após interface e habilidades ampliadas, comparar usabilidade, balanceamento e desempenho em condições equivalentes. Não repetir hashes ou inspecionar conteúdo protegido para responder questões que dependem de comportamento.

## Proveniência e preservação

- A01: C:\Users\Ergo_\.codex\visualizations\2026\11\10\01a1c6b1-a9ff-77a3-885b-1f6b2311fa2e\analise-dislyte\relatorio.md
- A02: C:\Users\Ergo_\.codex\visualizations\2026\11\10\01a1c6b1-a9ff-77a3-885b-1f6b2311fa2e\analise-dislyte\etapa-final-01\relatorio-final.md
- A03: C:\Users\Ergo_\.codex\visualizations\2026\11\10\01a1c6b1-a9ff-77a3-885b-1f6b2311fa2e\analise-dislyte\resumo.json
- A04: C:\Users\Ergo_\.codex\visualizations\2026\11\10\01a1c6b1-a9ff-77a3-885b-1f6b2311fa2e\analise-dislyte\etapa-final-01\plugins.csv
- A05: C:\Users\Ergo_\.codex\visualizations\2026\11\10\01a1c6b1-a9ff-77a3-885b-1f6b2311fa2e\analise-dislyte\fase-1\relatorio-preparatorio.md
- P01: C:\Users\Ergo_\.codex\visualizations\2026\11\10\01a1c6b1-a9ff-77a3-885b-1f6b2311fa2e\rpg-urbano\COMBATE.json
- P02: C:\Users\Ergo_\.codex\visualizations\2026\11\10\01a1c6b1-a9ff-77a3-885b-1f6b2311fa2e\rpg-urbano\docs\batalhas\ANALISE.md
- P03: C:\Users\Ergo_\.codex\visualizations\2026\11\10\01a1c6b1-a9ff-77a3-885b-1f6b2311fa2e\rpg-urbano\docs\batalhas\TEMPO-PESQUISA.md
- P04: C:\Users\Ergo_\.codex\visualizations\2026\11\10\01a1c6b1-a9ff-77a3-885b-1f6b2311fa2e\rpg-urbano\docs\batalhas\REVISAO-PESQUISA-USUARIO-R03.md
- P05: C:\Users\Ergo_\.codex\visualizations\2026\11\10\01a1c6b1-a9ff-77a3-885b-1f6b2311fa2e\rpg-urbano\docs\batalhas\CONTRATO-CAUSALIDADE-R04.md
- L01: leitura pontual autorizada do Engine.ini da cópia de testes; somente caminhos de conteúdo.

comparacao.json inclui checks e SHA-256 dos relatórios de referência; comparacao.csv contém 24 áreas. O procedimento está em scripts/comparar-evidencias-dislyte.ps1. Relatórios anteriores preservados. Nenhum ativo ou componente do jogo foi reutilizado no projeto.
