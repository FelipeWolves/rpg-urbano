# CIC-01 — ciclo local: composição, resultado e persistência

18/11/2026. Partes 1–2 concluídas: composição de equipe e resultado/recompensa em memória. Parte 3 de persistência ainda pendente. Contratos históricos preservados por seção; não há save, inventário ou conta.

## Etapas do bloco

| Parte | Entrega delimitada | Situação |
|---|---|---|
| 1 | Catálogo disponível, seleção válida e composição de encontro | ✅ Concluída, 195 testes acumulados |
| 2 | Resultado/recompensa mínima em memória, com concessão única | ✅ Concluída, 203 testes acumulados |
| 3 | Save local versionado, validação e restauração | Planejada; formato e migração ainda a fechar |

## Contrato da parte 1

- EncounterSetup contém definições, regras e loadouts; é dado de configuração, não runtime nem save. TeamSelection possui cópia privada do catálogo e um lado controlado.
- IDs disponíveis são os personagens do lado controlado no catálogo configurado. Isso não representa propriedade de conta nem inventário persistente. A demo oferece A1, A2 e A3; B1–B3 são adversários fixos.
- Seleção aceita de 1 a 5 IDs conhecidos e distintos, com comparação exata/sensível a maiúsculas. Vazia, maior que cinco, desconhecida, adversária ou repetida é recusada sem modificar a seleção anterior.
- O catálogo pode ter mais de cinco personagens. Estado inicial seleciona os primeiros até cinco IDs em ordem ordinal; na demo, os três disponíveis. ID disponível além dos cinco iniciais pode ser selecionado normalmente.
- Ordem solicitada fica no snapshot da seleção, mas não implementa posições táticas, bônus de líder ou prioridade de ação. O encontro mantém ordem canônica do catálogo e o kernel conserva os desempates AP/SPD existentes.
- compose cria cópia independente com os membros escolhidos e todos os adversários do cenário. Remove definições, skills e loadouts dos membros excluídos, conservando regras/orçamentos e kits restantes.
- O catálogo completo é validado com o perfil de equipes antes de aceitar seleções. Na API, pode-se escolher lado A ou B; o fluxo jogável desta entrega seleciona somente A.
- Uma alteração posterior de seleção não altera encontros já criados. Nova batalha começa com HP/recargas/status iniciais normais; não transporta estado de uma batalha anterior.
- Reiniciar dentro da sessão mantém a equipe selecionada e reinicia seu combate. Selecionar outra equipe exige nova abertura nesta parte; não há edição durante a batalha.

## Uso

Abertura normal continua Jogar.cmd, com A1/A2/A3. Para escolher no PowerShell, a partir da raiz:

```powershell
.\Jogar.cmd --equipe A1 A3
.\Jogar.cmd --equipe A2
```

Ou executar build/combate/jogar.exe com os mesmos argumentos. O lançador encaminha argumentos ao executável. Equipe inválida é recusada antes de iniciar sessão e retorna código 1; o lançador mantém a pausa de erro já existente. Modo clássico permanece separado e não recebe seleção nesta entrega.

A demo não oferece cinco personagens selecionáveis ainda; os tamanhos 1–5 e catálogo com seis membros são cobertos por dados autorais de teste. Não ampliar o catálogo jogável apenas para preencher o limite.

## Aceite e limites

Sete grupos próprios: catálogo/default/cópias; erros atômicos; composições de 1–5 com batalhas/replay; ordem versus iniciativa; remoção de dependências e conservação de regras; isolamento de configurações/encontros; catálogo/lado inválidos. Integração textual: seleção de A3, batalha, reset fiel e rejeição de adversário antes de qualquer saída de combate.

Não valida ainda recompensas, concessão única, save, coleção persistente, equipamento, formação espacial, múltiplos times, chefe compartilhado ou servidor. Não é sistema de seleção gráfica. SEM novos efeitos de combate, alterações de AP/SPD ou mudanças dos contratos HAB/R04.

Próxima parte: fechar o resultado/recompensa mínima em memória, especialmente identidade da conclusão e proteção contra concessão duplicada; somente depois definir persistência.

## Resultado da validação

✅ Parte 1 concluída: 195/195 testes aprovados, incluindo 7/7 grupos de seleção e 16/16 textuais. Mantidas matrizes/replays anteriores. Lançador com A1/A3 abriu sem A2; executável recusou B1 com código 1 antes de iniciar combate; clássico manteve defesa com dano 12. Evidência COMBATE-CIC-01-PARTE-1.json e equipes-testes.csv em docs/batalhas; log local ignorado em build/combate/verificacao-equipe.log. Não houve escrita de save.

## Parte 2 — resultado/recompensa validada

Contrato [CIC-01-RECOMPENSA-R01.md](CIC-01-RECOMPENSA-R01.md). LocalSession mantém tentativas e conclusões, credita 10 somente por vitória A por eliminação e não aceita resultados externos. Sete grupos novos de recompensa e uma integração textual nova; 203/203 testes (17 textuais). Reapuração, consulta e comando pós-fim não duplicam crédito; reset conserva saldo em memória e muda o ID. Identidades e saldo não sobrevivem ao encerramento da sessão/programa.

Lançadores confirmaram saldo/tentativa, seleção e defesa clássica com dano 12. Evidência COMBATE-CIC-01-PARTE-2.json e recompensas-testes.csv em docs/batalhas. Próxima parte 3: contrato de persistência antes de escrever saves, incluindo validação de versão e resultado já contabilizado.
