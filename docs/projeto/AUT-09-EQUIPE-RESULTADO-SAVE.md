# AUT-09 — equipe, resultado e progresso local

Atualizado em 22/11/2026. Código implementado, testes nativos aprovados e adaptador Unreal compilado. Aceite interativo e gravação/reabertura pela interface ainda pendentes. Android adiado.

## Entrega

- Acesso **Equipe / Save** no hub, inclusive quando o background estiver indisponível.
- Rascunho de seleção separado da equipe aplicada: marcar/desmarcar não altera combate ou progresso. Aplicar valida IDs disponíveis, repetição e limite de 1–5, inicia outra tentativa e invalida comandos antigos.
- Catálogo atual permanece com A1/A2/A3; capacidade de cinco verificada com catálogo sintético próprio. Não foram criados personagens de produto. A composição preserva a ordem do catálogo já definida em TeamSelection.
- Perfil Equipes e perfil Clássico validam seus próprios loadouts. Escolher câmera clássica não altera regras.
- Resultado oferece painel com tentativa, ações, motivo, recompensa provisória e saldo. Consultar não liquida o resultado novamente.
- Salvar/carregar são ações manuais no menu. O save v1 mantém perfil, equipe aplicada e livro de progresso; não serializa HP, turnos ou batalha em andamento. Preferências do AUT-07 ficam separadas.
- Carregar valida tamanho, codec, perfil e membros do catálogo antes de substituir a sessão. Inicia nova tentativa e conserva o histórico carregado; não reexecuta recompensas. Falha de validação conserva sessão, revisão e progresso em memória.
- Troca de equipe e importação não são permitidas depois de ações aceitas em combate ainda ativo. Após abandono confirmado, abrir Equipe / Save prepara a próxima tentativa sem recompensa; voltar e entrar em batalha não a reinicia novamente por esse abandono.
- Controles do menu paginados; mensagens de sucesso/erro e detalhes do resultado usam o painel compartilhado. Aparência, texto ampliado, janela muito pequena e toque precisam de aceite próprio.

## Armazenamento

Caminho do experimento: `experiments/unreal/RpgEngineProbe/Saved/LocalProgress/progress-v1.save`, relativo ao projeto Unreal aberto. Diretório Saved e arquivos gerados continuam ignorados pelo Git. Não houve gravação de save de jogador durante esta entrega.

Adaptador usa IFileManager/FFileHelper da engine instalada: leitura limitada a 1 MiB por arquivo aberto; escrita em arquivo temporário próprio e substituição após gravação. Não foi comprovada atomicidade contra queda de energia/falha de disco. Em caso de falha pode restar um `.pending`; não há limpeza automática nem remoção de saves antigos por migração.

Salvar usa a equipe **aplicada**, não um rascunho ainda não confirmado. Carregar substitui o progresso local em memória pelo arquivo escolhido fixo; pode retornar a um checkpoint anterior por decisão manual. Não há conta, nuvem, sincronização, antifraude nem restauração de combate. A interface não carrega automaticamente no início.

## Evidências

- `scripts/testar-inspecao-arena.ps1`: 64 grupos aprovados, incluindo 7 novos grupos de contrato equipe/save; resultado em `AUT-09-TESTES.json`. Não somar como aumento da suíte geral histórica.
- Casos: seleção inválida sem mutação; seleção 1–5 e catálogo maior; comando antigo recusado; roundtrip v1 em memória; perfil/catálogo/arquivo inválidos sem mutação; combate em andamento protegido; recompensas e histórico preservados após conclusão, troca e restauração. Regressões de sessão clássica, Equipes, fluxo, layout, cartões e inspeção aprovadas.
- Primeiro ensaio identificou turno automático no cenário de teste; fixture corrigida. Ampliação para catálogo sintético apontou ausência de loadouts na fixture e incompatibilidade da validação fixa Equipes com o perfil Clássico; ambas corrigidas antes do resultado aprovado.
- UBT Editor Win64 Development: compilação final aprovada, 11 ações, saída 0. Log ignorado em `build/aut09-build-final.log`. Avisos conhecidos de MSVC não preferido e C4864 da engine permanecem.
- Sem abrir editor/aplicativo, controle de tela, APK, serviços externos ou instalação. Testes do codec em memória não comprovam o adaptador de arquivos executando na Unreal.

## Roteiro de aceite do usuário

1. Abrir normalmente o projeto e iniciar a prévia. No hub, abrir **Equipe / Save**.
2. Marcar/desmarcar o personagem da página; **Próximo personagem** percorre o catálogo. Confirmar que o rascunho só passa a valer em **Aplicar equipe**. Seleção vazia deve exibir recusa.
3. Aplicar um integrante, voltar ao hub e entrar em batalha. Conferir quantidade e alvos. Repetir com os três disponíveis. A opção **Anterior/Próxima** inferior pagina operações quando necessário.
4. Concluir uma batalha e abrir **Detalhes do resultado**. Conferir saldo e recompensa; fechar/reabrir e retornar ao hub não devem duplicá-los.
5. No menu, salvar. Fechar/reabrir a prévia e carregar manualmente. Conferir equipe e saldo preservados, mas batalha nova. Confirmar mensagem de sucesso/erro e retorno ao hub.
6. Repetir no perfil Clássico e em janela estreita/texto ampliado quando conveniente. Testes de arquivo ausente/corrompido e falha de escrita ficam pendentes de ensaio apropriado, sem mexer em saves importantes.

## Próximo

AUT-10: contrato/estados de mídia local e fallback para o hub. Reprodução efetiva fica no AUT-11. Nenhum aceite visual anterior é reaberto por este bloco; AUT-08/09 novos menus e I/O persistido permanecem com aceites próprios.
