# COM-01A — formações, fases e confrontos paralelos

Análise e execução local em 18/11/2026. Antecede COM-02 por solicitação do usuário.

**Nota de atualização:** este documento registra a pesquisa de COM-01A. TIME-01 posteriormente implementou orçamento local de ações, descrito em TIME-01.md. Todas as dúvidas/decisões em aberto passaram para [PENDENCIAS.md](../../PENDENCIAS.md); usar esse registro para o estado vigente.

## Conclusão

O núcleo atual passou na matriz de formações de 1 a 5 de cada lado, com variações de força, guarda e replay. A separação de estado permite intercalar três confrontos independentes sem interferência. Isso é uma base útil, mas **não implementa fases, limites de turnos de jogo, três times coordenados ou chefe compartilhado**.

O risco maior para expansão não é o número cinco. É confundir time com lado, batalha com fase, vida de chefe com pontuação e simultaneidade visual com execução concorrente. A arquitetura precisa explicitar esses conceitos antes de UI e habilidades mais complexas.

Esta entrega conclui a análise dos formatos solicitados e os testes que o núcleo existente permite, não a implementação de todos os formatos. Nenhuma engine, backend, conta ou instalação de jogo foi acessada. A pesquisa foi pública; não houve extração, datamining, downloads de cliente ou consulta a APIs privadas.

## 1. Esclarecimento do usuário e qualidade da evidência

O usuário identificou o evento como **Celestial Anomaly Season 0** e esclareceu três casos: confrontos simultâneos contra três chefes; confrontos simultâneos contra o mesmo chefe; três times sucessivos contra o mesmo chefe com habilidades diferentes. Também descreveu formações usuais de 1–5 e até cinco confrontos numa fase com limites.

Essas descrições são requisitos de investigação (U01). Não serão reduzidas a três times entrando um depois do outro. Falta confirmar se “mesmo chefe” era HP compartilhado, partes ou avatares com pontuação conjunta. O formato observado não revela isso sozinho.

fontes.json registra procedência, links e limites. **Publicação primária do fornecedor** e evidência local fundamentam as afirmações técnicas verificadas. Arquivos comunitários foram usados como pistas históricas, não como especificação do código do Dislyte. Não se buscou nem obteve seu código-fonte.

## 2. O que a pesquisa pública permite registrar

| Achado | Evidência | Grau e limite |
|---|---|---|
| Season 0 é associada a Withering Flame e às formas Lion/Goat/Viper | U01 e C01 | Identificação histórica compatível em arquivo comunitário; sem reconstrução visual quadro a quadro |
| Há publicação histórica de formações de cinco, seis e múltiplos times | C02 | Reprodução comunitária de notas; não prova todos os comportamentos atuais |
| Há registros de vídeo datados do período de Chimera | V01/V02 | Título/data/descrição localizados; conteúdo não assistido integralmente |
| Mudanças em AP/SPD, gatilhos, efeitos e recargas existem em notas do fornecedor | P01 | Fonte primária de versões; não especifica todo o combate nem revela backend |
| Um adiamento de Shademother é atribuído a questões de desempenho | C03 | Transcrição comunitária atribuída à equipe; fonte original não autenticada nesta consulta e modo diferente |
| Há percepções de fluxo/sincronização de Chimera em discussões de jogadores | C04 | Relatos, não diagnóstico nem confirmação de bugs ou contadores específicos |

O arquivo histórico de Withering Flame ajuda a identificar o evento, sem transformar a nomenclatura usada em todas as temporadas numa cronologia técnica definitiva. [Referência comunitária C01](https://dislyte.fandom.com/wiki/Withering_Flame). As notas reproduzidas registram composições diferentes e limites por confronto, úteis para localizar documentação futura; não obrigam o nosso jogo a copiar seus números. [C02](https://dislyte.fandom.com/wiki/Version/3.3.3).

O histórico publicado pelo fornecedor mostra que ordem/barra de ação e gatilhos merecem análise própria: a versão simples deste projeto dá só uma ação por vivo por rodada. Isso não equivale a um sistema dinâmico de AP. [P01](https://apps.apple.com/pl/app/dislyte/id1590319959?platform=ipad).

Sobre Shademother: foi localizado um texto reproduzido e atribuído aos desenvolvedores que relaciona o adiamento de um modo multiplayer a desempenho. Sem o comunicado original verificado, é uma pista de pesquisa. Mesmo se confirmado, não identifica CPU, GPU, memória ou servidor como causa, nem explica uma mudança de Season 0. [C03](https://www.reddit.com/r/Dislyte/comments/15oas8w/).

**Não foi encontrada evidência primária suficiente para concluir que Season 0 foi um teste de estresse, que sua alteração decorreu de servidor ou que a Unreal impôs o limite.** Ausência ou remoção de um modo também pode ter razões de design, produção, manutenção ou engajamento; são alternativas, não conclusões sobre a equipe original.

## 3. Matriz executada no nosso código

Código novo: tests/scenario_tests.cpp. Comando ampliado e validado: `.\scripts\testar-combate.ps1`.

| Cobertura | Execução |
|---|---|
| Formações básicas | Todos os 25 pares de 1x1 até 5x5 |
| Variação por par | Três perfis de ataque: simétrico, A mais forte, B mais forte; duas políticas: ataque e guarda a cada sete ações |
| Total básico | 150 cenários |
| Sondagens de extensão | 1x6, 6x1 e 6x6, nas mesmas variações: mais 18 cenários |
| Repetibilidade | 168 replays com ordem dos dados invertida; comparação de estado final e sequência completa de eventos |
| Invariantes | Vida válida; vivos na ordem uma vez; ator vivo; vencedor com sobreviventes; perdedor sem vivos; comando inválido sem efeito, inclusive após fim |
| Isolamento | Três confrontos 5x1 independentes, IDs repetidos entre contextos e duas ordens de intercalação; comparação com cada execução individual |
| Lote adicional | 1.000 batalhas 5x5 sequenciais com perfis/políticas alternados e invariantes ativos |

**Resultado: 168/168 cenários e 168/168 replays aprovados, mais 13/13 testes originais.** Não foi necessário alterar regras de produção em src/battle.cpp ou a API do núcleo neste bloco. A demo de COM-01 manteve o resultado anterior.

Exemplos da matriz sintética (perfil equilibrado, política sempre atacar):

| Formação | Vencedor | Ações | Rodada final |
|---|---|---:|---:|
| 1x1 | A | 19 | 10 |
| 1x5 | B | 6 | 1 |
| 5x1 | A | 5 | 1 |
| 5x5 | A | 50 | 11 |

Nos 168 casos, A venceu 87 e B 81; isso não mede balanceamento, pois as formações e os perfis são deliberadamente artificiais. O caso mais longo teve 66 ações. Não são todos os atributos, personagens, habilidades ou combinações possíveis.

Lote local: **1.000 batalhas, 32.352 ações, 180 ms observados** nessa execução, build /Od, uma thread, sem engine e com verificações. É medição incidental de ensaio funcional, não benchmark de produção. Não foram coletados consumo de memória, percentis, aquecimento, GPU, rede ou sessões concorrentes. Não converter esse tempo em capacidade de servidor ou FPS.

Os CSVs detalhados ficam em formacoes-testadas.csv e os resumos em COMBATE.json. O executor tem limite técnico de 4096 ações; atingir esse limite falha o ensaio, não produz uma derrota de jogo. Guarda infinita ainda não tem política de empate.

## 4. O que “paralelo” exige

Três frentes visíveis ao mesmo tempo não requerem necessariamente três threads. Uma fila de simulação pode avançar as três frentes em ordem estável, enquanto a apresentação anima todas. Nosso teste atual confirma somente **isolamento e repetibilidade da intercalação de três contextos independentes**. Não mostra três janelas/cenas, não usa relógio conjunto e não troca efeitos entre frentes.

Para chefes independentes, um coordenador agrega resultados sem compartilhar vida por acidente. Para chefe único, é necessário definir autoridade de vida, ataques, efeitos, etapas de habilidade e score. Clonar o chefe em três Battle e somar dano depois não resolve morte simultânea, limiar de fase ou debuff global duplicado.

O mesmo personagem de catálogo em frentes diferentes precisa de instâncias distintas. “Todos os aliados” deve indicar time, frente ou lado. Números de combatentes, reservas e invocações são configurações por modo, não um `MAX_TEAM_SIZE=5` espalhado pelo código.

## 5. Sequência e limites antes da interface

CONTRATOS.md detalha a ordem proposta:

1. **TIME-01:** ações principais, rodadas e limites por confronto/fase; vitória na última ação tem precedência sobre timeout.
2. **WAVE-01:** até cinco confrontos num perfil de fase, com passagem explícita de vida, derrotados, efeitos, energia, recargas e contadores.
3. **SEQ-01:** três times sucessivos e perfis de habilidades de chefe, com transição atômica.
4. **PAR-01:** coordenador de frentes independentes, sem starvation e com resultado global explícito.
5. **PAR-02:** chefe com estado compartilhado/partes/avatares, após escolher o modelo.

Neste bloco os contratos foram analisados, **não implementados ou testados como sistemas completos**. TIME-01 é a próxima entrega de código proposta. COM-02 permanece em espera, por solicitação de analisar esses formatos antes dela.

As decisões abertas desta análise foram centralizadas em PENDENCIAS.md, com IDs e momento de resolução. Não manter outra lista ativa aqui nem preencher essas questões silenciosamente com suposições.

## 6. Catálogo preventivo de mecânicas

mecanicas.csv registra 26 famílias, suas origens e os testes necessários: composições, invocação, ordem/AP, turnos extras, reações, recargas, estados, imunidade, múltiplos golpes, críticos, cura, escudos, identidade, transições, score, replay, concorrência, reinício e apresentação.

Os itens com origem `proposta_de_engenharia` são riscos e possibilidades do nosso projeto, não afirmações verificadas de que o Dislyte implementou exatamente aquela regra. Itens de P01 se limitam à existência das famílias mencionadas nas notas; fórmulas e atributos de personagens não foram copiados. Não é uma enciclopédia de todos os personagens ou um inventário exaustivo de patches.

Os contratos evitam armadilhas concretas: reaproveitar vida restante como vida máxima; curar ao reconstruir Battle; três cópias do orçamento global; uma vitória/premiação por frente para uma única morte; gatilhos recursivos sem limite; contadores que dependem de FPS; score que estoura inteiro; depender de UI para decidir dano.

## 7. Desempenho e plano futuro de medição

Antes de pensar em servidor, medir separadamente núcleo, coordenação, renderização/UI e persistência. Comparar uma frente a três, com cenas/VFX representativos no PC e dispositivo móvel-alvo. Gráficos de alto custo podem limitar antes do cálculo de turnos; isso precisa ser medido, não presumido.

Um teste de servidor só terá sentido com implementação de sessões, validação de comandos, rede e persistência. Ele deverá medir concorrência e percentis, testar duplicação/reordenação, desconexão, reconexão e conclusão idempotente. Não escolher Azure ou outro fornecedor de infraestrutura por associação ao jogo de referência ou pelo tempo do lote atual.

## 8. Limites da pesquisa e próximos dados úteis

Foi realizada pesquisa seletiva de páginas oficiais, histórico publicado em loja, registros históricos e candidatos de gameplay. Notícias dinâmicas e alguns subartigos não forneceram conteúdo completo; vídeos não foram inspecionados integralmente. Não foi autenticado o texto atribuído ao Discord. Nenhum acesso com conta ou download foi feito.

Para elevar os relatos a reprodução verificável: registrar vídeo/patch/mode exatos, regras exibidas, contador antes/depois de ação principal/reação, comportamento na troca de time e se dano numa frente altera imediatamente o mesmo HP nas outras. Esses dados podem ser obtidos de material público autorizado ou demonstrações fornecidas pelo usuário; não exigem abrir a instalação analisada anteriormente.

Não é possível garantir antecipadamente que nenhum problema aparecerá no futuro ou descobrir motivos privados de decisões do estúdio. O que podemos entregar é cobertura rastreável, contratos explícitos, testes reproduzíveis e ausência de promessas falsas de compatibilidade/capacidade.

## Arquivos e preservação

ANALISE.md resume os achados; CONTRATOS.md especifica comportamento a decidir/testar; cenarios.json cataloga cobertura; fontes.json registra procedência; mecanicas.csv contém o checklist; formacoes-testadas.csv contém os resultados executados. Nenhum ativo ou código de outro jogo foi usado. O núcleo de produção permanece igual ao de COM-01; somente testes, script de verificação e documentação foram ampliados.
