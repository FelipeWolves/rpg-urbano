# SIS-02-B — aritmética segura

22/11/2026. Implementado `include/rpg/numeric.hpp`, utilitários puros sem mudança no Battle nesta subentrega.

- Quantity int64, teto 10^12, quantidade negativa recusada; entrada positiva acima do teto pode ser normalizada pelo helper quantity.
- Soma saturada sem overflow; domínio não negativo.
- mul_div_floor aceita operandos não negativos int64 e divisor positivo; calcula floor(a*b/d) e limita o resultado a um cap explícito de 0..10^12. Não satura produto antes de dividir. Representa prefixos binários por quociente/resto com operações seguras; se o quociente de um prefixo já atinge o teto, prefixos seguintes não o reduzem. Sem extensões de inteiro largo ou biblioteca externa.
- Basis points: 10000=100%, 15000=150%; percent_floor não impõe teto universal de taxa. Limites semânticos de chance/mitigação pertencem às camadas futuras.
- compare_ratio usa frações contínuas/quocientes e restos, sem produto cruzado e sem quantização prévia. Denominadores positivos; negativos e zero inválidos são recusados.
- Modificadores assinados/AP, IDs/revisões e moedas não são automaticamente convertidos a Quantity.

Verificação: dez grupos aprovados, 67.240 combinações pequenas e 256 vetores de operandos grandes. Oráculo independente via System.Numerics.BigInteger local gera quocientes saturados e comparações de razões. Inclui produto 10^24 antes da divisão, frações equivalentes/quase iguais, 150%, floor, teto, zero e entradas inválidas. Primeiro compile apontou resultados nodiscard descartados nos testes negativos; corrigidos, mantendo /W4 /WX.

Comando: scripts/testar-numerico.ps1 (Arithmetic). Resultados em SIS-02-Arithmetic-TESTES.json. Vetores/logs/binários em build ignorado. Não executar engine para testar aritmética pura; Windows nativo não comprova ARM/iOS.

Próximo SIS-02-C: migrar as quantidades e fronteiras de forma compilável, preservando cenários pequenos. Nenhuma migração, fórmula de DEF/RNG ou reescala foi aplicada neste commit. Manifesto de preparo futuro inclui o header; não se afirma que um novo pacote foi construído.
