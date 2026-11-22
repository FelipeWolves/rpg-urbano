# LAB-QA-01 — feedback e aceites

Entrega de código e roteiro de validação, em 22/11/2026. A execução visual continua com o usuário; esta entrega não abre editor nem jogo.

## Implementado

- Resultado de cada tentativa distingue ação aplicada, recusa e interrupção de segurança. Revisão antiga e turno de outra unidade nunca aparecem como sucesso.
- Dano no PV, absorção por escudo, esquiva, bloqueio e invencibilidade usam os eventos do núcleo, sem recalcular combate no HUD.
- Recusa exclusivamente por alvo permite escolher outro alvo mantendo a habilidade. Outros erros exigem nova seleção.
- Mensagem limitada ao espaço disponível; clicar em [i] abre detalhes paginados da última tentativa. Não é histórico persistido nem executa ações.
- Intervalo visual de 0,5 segundo após comando manual; não altera AP ou regras de turno.
- Nova tentativa também disponível após parada de segurança do adaptador.

## Validação e limites

Matriz canônica: `data/qa/lab-acceptance.json`; cópias CSV/JSON em `docs/relatorios/atuais/LAB-QA-01-MATRIZ.*`. Os 16 casos distinguem aceites pendentes e adiados. Teste nativo ou validação de schema nunca aprova caso manual.

Cliques básicos, placeholder/seletor e reprodução/skip já aceitos foram reconciliados em smoke.json sem exigir repetição rotineira. Isso não aprova os modos LAB-PVE-02–06.

Save de progresso usa arquivo temporário e substituição, mas não possui backup automático. Preferências possuem recuperação de backup no modelo; I/O real na Unreal permanece por aceitar. Não corromper nem excluir saves existentes para testar. Casos destrutivos/falha de escrita aguardam diretório isolado de QA.

## Próximos passos

1. LAB-QA-01B: adaptador/diretório isolado para testar falha de escrita e recuperação sem tocar saves normais. Independente de novas entregas do usuário.
2. Usuário: aceites seletivos da matriz e resoluções Windows; preencher o modelo em entregas/01-interface-e-testes.
3. Reconciliar resultados reais; somente então revisar estimativas. Demo permanece aproximadamente 28% (28,03 ponderado), sem crédito extra por roteiro.
4. Android, Shipping, backend mobile e conteúdo final continuam separados e adiados.

## Evidências desta entrega

Feedback: 12/12 grupos; sessão/codec: aprovados; matriz: schema aprovado; UnrealBuildTool: Succeeded; stage: 68 entradas verificadas. Avisos já conhecidos do compilador MSVC e Attribute.h da engine permanecem; sem instalação ou alteração da engine. Não executados: jogo, editor gráfico, Android e I/O real de save pela interface.
