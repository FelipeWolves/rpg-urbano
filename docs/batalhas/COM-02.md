# COM-02 — Interação textual

18/11/2026. **Concluído:** escolher ações/alvos, resposta automática do inimigo, mostrar resultado e reiniciar; motor de combate inalterado.

## Como usar

Entrada recomendada: dê dois cliques em **Jogar.cmd** na raiz rpg-urbano. Também funciona no PowerShell comum:

```powershell
.\Jogar.cmd
```

O executável já foi compilado nesta entrega em build/combate/jogar.exe. Em uma cópia recém-obtida pelo Git, os binários ignorados não existem: compile e verifique primeiro com ` .\scripts\testar-combate.ps1 ` e depois execute jogar.ps1. Jogar.cmd localiza o executável a partir da própria pasta, inclusive quando o terminal está em outro diretório. Não instala, baixa ou compila. scripts/jogar.ps1 continua disponível nos ambientes que permitem scripts PowerShell.

Time A controlado pelo jogador; time B usa ataque/Skip automático existente. Perfil padrão: seis combatentes 3x3 do reference_roster, AP/SPD e orçamento explícito de 200 comandos principais aceitos, empate por limite. Não equivale ao contador universal de um modo do Dislyte. A demo automática simular.exe e sua referência fixa foram preservadas.

## Comandos

| Comando | Função |
|---|---|
| `atacar B3` | Ataca o ID escolhido com o personagem atual; use IDs vivos mostrados no estado |
| `guardar` | Guarda opcional conforme perfil do motor |
| `habilidade ID ALVO` | Usa uma habilidade cadastrada do personagem atual; recarga/alcance validados no motor |
| `passar` | Permitido somente quando o personagem está bloqueado |
| `estado` | Mostra vida/maxHP, SPD, guarda/derrotados, ator atual, habilidades/recargas e saldo |
| `ajuda` | Mostra os comandos sem avançar |
| `reiniciar` | Recria a batalha original e volta às regras/dados iniciais; permitido durante e após fim |
| `sair` | Encerra a sessão sem alterar o resultado do combate |

Comandos em minúsculas, separados por espaços; IDs respeitam a grafia mostrada. Linha vazia não avança. Parâmetros extras ou ausentes são rejeitados. `atacar` exige alvo explícito, sem escolher automaticamente pelo jogador. Após batalha encerrada, ações são recusadas; estado/ajuda/reinício/saída continuam disponíveis. Fechar a entrada (EOF) encerra sem ciclo infinito.

O cenário padrão não equipa habilidades especiais: o comando está integrado e testado por configurações de sessão, mas não é um catálogo/seleção de personagens. HAB-01 poderá fornecer conteúdo próprio posteriormente. Não há editor de dados ou carregamento de arquivos de configuração nesta entrega.

## Separação de responsabilidades

Código de apresentação em rpg::presentation: include/rpg/text_session.hpp e src/text_session.cpp. app/jogar.cpp liga entrada/saída padrão; scripts/jogar.ps1 executa o binário próprio. Não há leitura de instalações externas, rede, plugins, engine ou salvamento.

A sessão converte comandos em Action e mostra eventos/consultas constantes do Battle. Dano, guarda, recargas, bloqueio, vitória e timeout não foram reimplementados na interface. Inimigo resolve oportunidades consecutivas até o jogador voltar a agir ou a batalha terminar. Rejeição não inicia resposta inimiga. Contadores da sessão identificam ações do jogador, automáticas, rejeições e reinícios; eles não são o orçamento da batalha e não substituem snapshot/save.

A proteção da apresentação limita uma sequência a 256 ações automáticas antes de devolver o controle ao usuário. Se esse limite for atingido, a sessão encerra com código 2 e mantém o estado real da batalha, sem criar vencedor/empate. No perfil padrão, o limite de batalha de 200 normalmente encerra antes. Essa proteção não é uma regra de derrota nem um novo teste de capacidade de servidor.

Resultado distingue vitória A/B, empate por limite e interrupção técnica sem vencedor. Não há animações, interface gráfica, som ou exibição completa de todos os status; a interface atual é textual.

## Verificação

Sete grupos por entrada/saída simulada, sem teclado durante os testes:

- input_atomics: linhas vazias, ajuda/estado, sintaxe, alvo inválido/amigo e Skip indevido não avançam;
- target_enemy: alvo realmente escolhido e respostas dos dois inimigos correspondem ao motor;
- guard_restart: mitigação e reinício durante a batalha;
- victory_restart: nenhum inimigo age depois da vitória, ação pós-fim recusada e reinício íntegro;
- skill_skip_clock: habilidade, recarga recusada, Skip automático do inimigo e Skip manual do jogador bloqueado;
- eof_timeout: EOF, empate no limite sem resposta extra e derrota do jogador corretamente apresentada;
- safety_technical: proteção de apresentação sem resultado inventado e mensagem de limite técnico sem vencedor.

**155/155 testes aprovados**, sete novos de interação, MSVC /W4 /WX. Matrizes anteriores 168/25 AP/replays, 1000 simulações funcionais e demo preservadas. Executável real compilado e verificado com entrada sair; roteiro adicional `atacar B3`, `guardar`, `reiniciar`, `sair` executado com sucesso. Logs da execução ficam em build/combate/, ignorados pelo Git.

Evidências: COMBATE.json, COMBATE-COM-02-FINAL.json e interacao-testes.csv. A entrega testa apresentação textual do Battle, não menus para Stage/SequentialSquads/ParallelFronts/SharedBoss. Esses modos continuam com suas coberturas de núcleo, sem seleção nesta tela.

## Próximo bloco

HAB-01: escolher uma família adicional de habilidades/recurso por incremento, com contrato e testes próprios, aproveitando recargas/status existentes. Fases autônomas de chefes e modos por score permanecem no catálogo PvE; não foram implementados implicitamente ao adicionar interação.

## Correção de abertura no Windows PowerShell

Erro de autorização reproduzido ao executar scripts/jogar.ps1 no Windows PowerShell comum: scripts desabilitados. O executável próprio existe e funcionou. Nenhuma política de execução, perfil ou configuração de segurança foi alterada.

Adicionado Jogar.cmd, que chama diretamente build/combate/jogar.exe com caminho relativo ao próprio lançador. Falta de binário ou saída de erro fica visível antes de fechar. Validado com entrada sair via cmd.exe e executável direto via Windows PowerShell, ambos saída zero. Não houve recompilação nem nova execução dos 155 testes por esta correção de entrada.

Alternativa no terminal, já dentro de rpg-urbano: ` .\build\combate\jogar.exe `. Não é necessário executar o .ps1 nem liberar scripts para experimentar o binário compilado.
