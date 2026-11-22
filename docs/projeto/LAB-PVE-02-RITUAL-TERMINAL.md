# LAB-PVE-02 — Ritual local no Terminal

22/11/2026. Primeiro encontro PvE integrado à apresentação, sem abrir o editor ou executar o jogo nesta entrega. Compilação/testes não equivalem a aceite visual. Demo ~28% mantida.

## Acesso e comportamento

Hub → Terminal → Ritual local — Felipe e ecos. O Terminal mantém Laboratório de equipe; Arena PvP mantém o piloto Felipe temporário, sem PvP online.

Felipe mantém A1 Garra do Asfalto, A2 Postura de Vigília passiva e A3 Eclipse. ECO-1 é o chefe declarado; ECO-2 é auxiliar com PV próprios. Objetivo: derrotar ECO-1; não é necessário eliminar ECO-2 para concluir. Base Team/FunnelV1 e limite de 200 ações/empate compartilhados. Chefe recebe as imunidades estruturais já existentes de EntityRole::Boss. Valores são de laboratório, não balanceamento final.

HUD exibe objetivo/chefe, PV, ações/limite e estado do encontro. A seleção/ícones/recargas/painéis existentes são reutilizados. Resultado do Ritual permite consultar encerramento; Nova tentativa reinicia com outro ID/revisão. Pausa/abandono/retorno funcionam pelo fluxo existente. Sem recompensa comercial, crédito do laboratório genérico ou persistência de progresso do Ritual nesta etapa.

## Arquitetura e segurança

PveVisualSession adapta estados/comandos, revisão de entrada, cues e automação. PveEncounter continua sendo o único executor do encontro. A UI não chama Battle.apply diretamente, não decide objetivo nem liquida score. Ledger e encontro têm donos estáveis; mover o adaptador não invalida o ledger.

Preparação de ações/slots, leitura de habilidades e escolha automática foram extraídas para funções compartilhadas em ability_profile. ProfileBattle e PveEncounter com loadouts declarados usam as mesmas funções. Resolução de dano/efeitos permanece no Battle. ProfileBattle valida os loadouts e inicializa passivas de abertura no PvE; A2 reativa de Felipe continua no núcleo.

Compatibilidade preservada: PvE sem loadouts mantém a API de ações existente. Combinação de loadouts com invocações reservadas exige adaptador de inicialização próprio e é recusada explicitamente por enquanto; o Ritual não usa reservas. Mudanças futuras de kit por fase deverão mapear slots/loadouts no LAB-PVE-03, sem pressupor que o ID de habilidade antigo continue válido.

Ritual, laboratório genérico e piloto não compartilham runtime, créditos ou saves. Voltar pelo resultado ou Escape desativa o Ritual e restaura os placeholders do laboratório ativo. Abandono do Ritual marca sua sessão para reinício sem alterar progresso alheio; a tentativa genérica abandonada continua exigindo reset antes de iniciar outro fluxo. Gestos/seleção são cancelados nas fronteiras.

## Testes

Ritual: 12 grupos — configuração, passiva não selecionável, A1/A3/recarga, revisão antiga, alvo inválido, chefe derrotado com auxiliar vivo, nova tentativa, abandono, move/isolamento, batalha completa, passiva de abertura/condição e equivalência da base de dano. Comparações de entidades por ID, sem depender da posição em fighters.

Regressão geral de combate/perfis, PvE7 e HUD10 executados. Build e stage em LAB-PVE-02.json. Stage cresce de66 para68 entradas pelo novo header/source canônico; wrapper da engine compila a mesma implementação. Sem cook/Shipping, Android, tela, acesso externo ou instalações.

## Roteiro de aceite do usuário

1. Abrir projeto normalmente, Play → Terminal. Conferir os dois acessos, abrir Ritual e ver três esferas.
2. A1 seleciona inimigo; trocar entre ECO-1/ECO-2 antes de confirmar. A2 é informativa/passiva. A3 atinge área e entra em recarga.
3. Conferir objetivo, PV do chefe e ações. Ao derrotar ECO-1, nenhum novo comando deve ser aceito; consultar resultado, tentar de novo e voltar ao hub.
4. Em outra tentativa, pausar e confirmar abandono. Entrar novamente: tentativa nova; nenhuma recompensa concedida.
5. Alternar para Laboratório de equipe e Arena PvP local: elenco, regras/kit e progresso próprios preservados.
6. Se houver corte/sobreposição, informar resolução, modo janela/PIE e sequência. Não é necessário revalidar todos os controles já aceitos, somente a nova integração.

## Próximo

LAB-PVE-03: apresentar ondas/fases e fronteiras já implementadas; não habilitado nesta entrega. Conteúdo final/arte/áudio podem chegar em paralelo. Esta integração não depende de uma nova ficha do usuário para o laboratório funcionar, mas a ficha ajudará a substituir os dados provisórios.
Evidência final: Ritual12/12, regressão geral245/245, PvE7/7 e HUD10/10 aprovados. Build inicial67,36s; incremental de retorno/abandono23,11s. Stage68 verificado. Sem execução visual ou cook. Avisos conhecidos do toolchain/header da engine permanecem, sem instalar ferramentas.
