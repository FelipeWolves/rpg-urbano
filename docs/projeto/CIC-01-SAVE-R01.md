# CIC-01 parte 3 — save de progresso, formato v1

## Escopo

Salvar composição da equipe e progresso entre batalhas. Não restaurar batalha em andamento: não persistem HP, AP/SPD de runtime, filas R04, status, recargas, RNG ou comandos. Personagens/habilidades vêm do catálogo autoral atual, nunca de definições arbitrárias no arquivo.

O usuário pediu continuidade do CIC-01, incluindo persistência. Escritas deste incremento ficam no projeto: código/documentos/relatórios e arquivos de teste em build. Slots do jogador são usados somente mediante comando salvar; carregar não cria nem reescreve arquivos.

## Operação no protótipo

- Jogar.cmd: sessão nova de equipes, com três personagens por padrão.
- Jogar.cmd --equipe A1 A3: sessão nova com a seleção indicada.
- Comando **salvar**: grava progresso/equipe do perfil atual.
- Jogar.cmd --continuar: carrega o slot de equipes e abre uma batalha nova.
- Jogar-Classico.cmd --continuar: faz o mesmo no perfil clássico.
- Slots: build/saves/equipes.save e build/saves/classico.save, relativos à raiz deduzida da localização do executável build/combate/jogar.exe, não do diretório de trabalho. Área build já ignorada pelo Git.

Salvamento é manual. Vitória/reinício/saída não gravam automaticamente. Sem salvar, avanços desde o último checkpoint são perdidos. Abrir uma sessão nova não carrega saldo implicitamente; se já houver slot, o programa informa que salvar nessa sessão substituirá o progresso anterior. Não há fusão de carteiras nem migração automática.

## Formato e limites

Texto ASCII (subconjunto UTF-8), extensão .save; formato próprio pequeno, não JSON. Não há biblioteca externa, código executável ou caminhos no conteúdo. Versão independente da versão da engine.

```text
RPG_URBANO_SAVE 1
profile team
attempt 3
credits 10
team 2 A1 A3
completions 2
1 A 0 1 10
3 none 1 200 0
end
```

Cada conclusão contém: ID, vencedor A/B/none, motivo, ações aceitas e créditos. Motivos v1: 0 eliminação, 1 limite de ações, 2 limite técnico. Perfil team/classic, tentativa >=1 e equipe 1–5 IDs únicos. IDs de até 64 caracteres ASCII alfanuméricos, hífen ou sublinhado. Clássico só aceita a composição histórica A1/A2/A3 na entrada do aplicativo; equipes passam pelo catálogo atual de seleção.

Máximo de leitura: 1 MiB, histórico de até 4.096 conclusões. Contadores são uint64, com overflow de parse recusado. Não truncar/descartar registros para caber; se exceder o limite, salvar falha e preserva o arquivo anterior. Esse teto e o formato podem evoluir por nova versão/migração explícita, não dependem de trocar a engine.

Leitor recusa campos desconhecidos, ordem inesperada, lixo final, arquivo truncado, perfil/versão/motivo desconhecidos e contagens inválidas. Números não aceitam sinal, fração ou expoente. Espaços e quebras de linha são separadores; escritor gera saída canônica.

## Validação de progresso

Validar tudo antes de importar: IDs de conclusão estritamente crescentes, sem duplicação e <= tentativa salva; ações positivas; vencedor compatível com motivo; soma dos créditos igual ao saldo; regra provisória +10 só para vitória A/eliminação e zero nos demais finais. IDs de tentativas abandonadas podem ficar ausentes.

Restaurar somente em LocalSession ainda intacta, sem comandos nem importação anterior. Copiar histórico/saldo e abrir ID = tentativa salva + 1. Sem espaço numérico para uma nova tentativa persistível, recusar antes de mutar. Uma conclusão carregada não é reapurada, não gera mensagem de concessão e não paga novamente. Passivas de abertura são aplicadas para a nova batalha, com HP/recargas iniciais.

O arquivo não comprova que os combates ocorreram: consistência de dados não é autenticação/antifraude. Alterações manuais coerentes, rollback voluntário e sessões concorrentes não são resolvidos por este save local. Uso suportado: um processo escritor por slot, sem sincronização remota.

## Gravação e falhas

Validar/serializar em memória antes da escrita. Se o destino existir, exigir que seja um save legível de versão conhecida; não substituir silenciosamente arquivos corrompidos/desconhecidos. Criar temporário .pending exclusivo, gravar, FlushFileBuffers, fechar e substituir por MoveFileExW com REPLACE_EXISTING e WRITE_THROUGH.

Falha antes da substituição não trunca o destino. Temporário criado pela operação é removido em falha tratada; arquivo .pending já existente não é sobrescrito/removido. Um processo interrompido pode deixá-lo para revisão manual; não é promovido automaticamente. Não há histórico de backups nem promessa de recuperação contra falha de disco/energia. Adaptador de escrita atual é Windows; futura plataforma exige adaptador próprio e testes apropriados.

Save ausente, corrompido, perfil incompatível ou equipe fora do catálogo: saída de erro, sem fallback silencioso para dados novos e sem gravar o slot. Comando salvar recusado/falho não gasta ação nem concede recompensa.

## Verificação

Sete grupos de persistência: roundtrip; sintaxe/tamanho/versão; invariantes semânticas; carregar/concluir/carregar sem duplicação; restauração rejeitada sem mutação; arquivos/substituição/temporário ocupado; bloqueio de substituição/versão futura preservada. Integração textual cobre duas sessões, crédito preservado, nova tentativa, mensagem histórica não repetida e equipe desconhecida recusada.

Arquivos de testes usam diretórios próprios ignorados em build, sem ler ou alterar slots do jogador. Verificações de executável usam cópia própria em raiz de teste isolada. Nenhum jogo externo, servidor ou ferramenta adicional é acessado.

## Fechamento

Este incremento completa o escopo mínimo de CIC-01: composição, recompensa e progresso durável entre batalhas. Não é sistema completo de campanha/inventário, save de combate, migração de versões passadas inexistentes ou segurança de produção. Próximo bloco planejado: ENG-01, avaliação e escolha de engine; instalações/downloads e decisão definitiva continuam sujeitos a autorização específica.
