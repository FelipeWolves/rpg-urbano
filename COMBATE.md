# COM-01 — primeiro núcleo de combate

Atualização TIME-02: este documento descreve a referência fixa anterior. O perfil AP, extras/reações, habilidades/status/recargas mínimos, guarda opcional e integração de fases estão em docs/batalhas/TIME-02.md. Os limites antigos abaixo não devem ser tratados como estado atual desses recursos.

Concluído em 18/11/2026. C++20 original, biblioteca padrão, sem engine/rede ou recursos externos. Ainda não é uma interface interativa ou protótipo 3D.

## Executar e verificar

Na raiz do projeto, pelo PowerShell:

```powershell
.\scripts\testar-combate.ps1
```

O comando descobre o Visual Studio instalado, inicializa o ambiente x64 somente no processo filho, compila o núcleo uma vez, executa testes e então compila/executa a demonstração. Não instala nem baixa ferramentas. Falha com erro se compilação, teste ou demonstração falhar. Usa /std:c++20 /permissive- /Zc:__cplusplus /EHsc /W4 /WX e /MT.

Saídas temporárias: build/combate/; ignoradas pelo Git. Resultado resumido da última execução bem-sucedida: COMBATE.json, com horário. Ao falhar, consultar a saída atual e build/combate/resultado.log; um JSON de sucesso anterior não comprova uma tentativa posterior.

Depois de compilar, a demonstração pode ser repetida sem recompilar:

```powershell
.\build\combate\simular.exe
```

## Implementado

- Equipes, definições e estado próprios por combatente; IDs únicos, atributos positivos e duas equipes obrigatórias.
- Ordem por velocidade, desempate ordinal por ID e rodadas. Estado inicial canônico, independente da ordem dos dados de entrada.
- Ataque básico, vida limitada a zero e guarda de um acerto, com metade do dano arredondada para baixo (decisão atual do usuário). Guarda não atingida expira no início do próximo turno do defensor.
- Derrotados não agem e não podem ser atacados. Vitória é imediata ao eliminar adversários; não há ação ou novo turno depois dela.
- Validação antes de mutação: comando inválido não muda vida, guarda, rodada, turno ou resultado e não emite eventos.
- Eventos de ataque, guarda, consumo/expiração, derrota, turno, rodada e vitória. O dano do evento é o efetivamente aplicado, limitado à vida restante.
- Política provisória: ataque ao adversário de menor vida atual, com desempate por ID, sem chamadas externas ou aleatoriedade.

Núcleo: include/rpg/battle.hpp e src/battle.cpp. Interface pública: Battle, apply(Action), consultas constantes e snapshot copiado. As regras não dependem de console, Windows ou Unreal; o aplicativo de apresentação é separado em app/simular.cpp. As definições podem ter outros tamanhos de equipe para testes; a formação de referência e a demo são 3x3.

## Evidência da execução

13/13 testes aprovados; compilação sem avisos/erros com /W4 /WX. Cobertura de comportamento: definições inválidas, IDs, ordem/desempate, comandos inválidos sem mutação, ataque/eventos, rodadas, guarda par/ímpar, consumo em um acerto, expiração, derrota, overkill, turno/alvo morto, vitória sem ações posteriores, inteiros grandes sem overflow de dano e política de alvo.

Teste de referência independente da demo: ambas as equipes sempre atacam. Resultado esperado conferido: A vence em 35 ações, rodada 12, apenas A2 vivo com 50 de vida. Repetição com dados iniciais invertidos na entrada produz o mesmo estado e toda a mesma sequência de eventos. Os limites de vida e o estado do ator/vencedor são verificados após cada ação dessa simulação.

Demonstração: A3 usa guarda primeiro; depois ambos os lados seguem a política de ataque. Resultado observado: equipe A vence em 40 ações, rodada 14. Essa escolha inicial é roteirizada para mostrar guarda; não representa controle do jogador nem estratégia avançada.

## Limites conhecidos

Atualização TIME-01: orçamento opcional local implementado; motivo de término é Elimination ou ActionLimit. Política por esgotamento escolhida explicitamente: empate, derrota de A ou de B. Vitória na última ação prevalece. API e testes em docs/batalhas/TIME-01.md. A demo continua sem orçamento para preservar o cenário de referência. Dúvidas de próximas etapas ficam exclusivamente em PENDENCIAS.md.

Sem input do usuário, reinício interativo, UI, câmera, animação, som, habilidades extras, progressão ou salvamento. Estatísticas simétricas e desempate por ID favorecem a equipe A neste cenário; nenhum balanceamento competitivo foi validado. O limite de 256 ações é proteção do aplicativo/teste, não uma regra de empate do núcleo. Uma política que só guarda pode prolongar indefinidamente o combate; regra de empate não faz parte desta versão.

Não foi avaliada integração com Unreal/mobile. Snapshot serve para leitura/testes, não é um formato de save com compatibilidade prometida. Memória insuficiente e uso concorrente não fazem parte das garantias de atomicidade de comandos inválidos.

## Próximo bloco

Atualização: o usuário pediu análise de formações/fases/paralelismo antes da entrada interativa. COM-02 fica em espera; TIME-01 local concluída e próxima entrega WAVE-01. O script executa 13 testes originais, 11 de limite, matriz e lote funcional. O núcleo recebeu as regras de orçamento de TIME-01; a descrição de COM-01A em ANALISE.md é histórica.

Matriz adicional: 150 cenários para os 25 pares de tamanhos 1–5, mais 18 sondagens com seis, repetidos com entrada invertida. Três contextos 5x1 independentes intercalados sem interferência. Lote de 1000 batalhas é ensaio funcional local. Limite local foi implementado; demais capacidades/decisões estão em PENDENCIAS.md. CSV em docs/batalhas/formacoes-testadas.csv.



## Atualização vigente — COM-02

Interação textual concluída em app/jogar.cpp e rpg::presentation, sem alterar regras do motor. Executar .\scripts\jogar.ps1; default 3x3 AP com teto 200/empate. As seções anteriores sobre falta de input e espera são históricas, superadas por docs/batalhas/COM-02.md. Demo fixa continua preservada. Sete grupos de interação e 155 testes totais aprovados; próximo HAB-01.


## Arredondamento atual da defesa genérica

Metade do dano por hit, arredondada para baixo; 25 vira 12 e 1 vira 0. Sem mudança de consumo/expiração ou ordem dos escudos. Contrato e anotações de extensões clássicas em docs/batalhas/DEFESA-PADRAO.md. Arquivos históricos anteriores podem conter a regra antiga para cima.
