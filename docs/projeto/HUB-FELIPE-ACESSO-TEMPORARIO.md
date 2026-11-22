> **Correção vigente — 22/11:** [crash do marcador, placeholders, regras gerais do Terminal e vídeo por compilação](TERMINAL-MARCADOR-MIDIA-CORRECOES.md). Funil compartilhado com Felipe, geometria visual protegida e marca de vídeo vinculada ao binário. Reteste visual pendente; aceite dos cliques únicos preservado.

# Arena PvP — acesso temporário ao piloto Felipe

22/11/2026. Solicitação do usuário após aceite dos cliques únicos.

## Como acessar

No hub, abrir Arena PvP e escolher Testar Felipe (local). A identificação original do destino permanece; o painel e o dock deixam explícito que é um teste local contra ECO-1/ECO-2, sem PvP online. Terminal continua acessando a batalha genérica anterior. Atalhos separados continuam disponíveis.

## Isolamento

Alternância mantém duas VisualSession em memória, sem importar save/progresso de uma na outra. Seleção de habilidade/alvo e gestos são limpos; atores são reconstruídos. Saves continuam progress-v1.save e felipe-pilot-v1.save (ou felipe-tutorial-v1.save se iniciado nesse modo). A escolha do save acompanha o cenário ativo, em vez de depender somente dos argumentos da inicialização. O progresso só é persistido pelos controles existentes de salvar; não há novo autosave.

O fluxo existente de retorno, reinício e abandono continua aplicável. Trocar cenário não constitui recompensa, PvP ou viagem. Nenhum recebível original ou configuração oficial do Dislyte é alterado.

## Evidências

Bateria nativa Pilot: 10/10; dois grupos adicionais verificam troca/restauração de sessões e execução após move, além de A3 do laboratório recusando inimigo e concedendo 1 ponto de escudo ao próprio usuário. Compilação e stage em HUB-FELIPE-ACESSO.json. Integração visual depende do reteste do usuário.

## Continuidade

Cliques únicos e hover aceitos no RpgEngineProbe; não repetir revisão rotineira desses aceites. Novo acesso temporário e barra de skip na condição não-assistido continuam para reteste. Backend mobile/Android adiado. Percentual global ~25% mantido: não se considera PvP implementado.