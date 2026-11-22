# SIS-02-A — mapa numérico e compatibilidade

22/11/2026. **Diagnóstico concluído; nenhuma migração de código realizada.** Fontes revisadas no commit registrado em SIS-02-A-MAPA.json. Foram conferidas 22 âncoras de caminhos críticos; isso é inspeção estática, não execução de testes, prova de ausência de overflow ou auditoria de cada operação possível.

Mapa detalhado: [CSV](../relatorios/atuais/SIS-02-A-MAPA.csv) e [JSON](../relatorios/atuais/SIS-02-A-MAPA.json). Cada linha identifica arquivo/linha, domínio, representação atual, mudança planejada e risco. Nenhum arquivo do jogo de referência foi acessado.

## Resultado principal

No alvo atual Windows/MSVC, HP/ATK, HP vivo/inicial, magnitudes, dano, absorção, reflexão/explosão, revive e Event.amount ainda passam por int de 32 bits. Não basta ampliar Definition: locals, estruturas de passagem entre encontros e eventos também precisam acompanhar.

Já existem domínios de 64 bits: AP usa int64; revisões/IDs/turnos/ações e o ledger usam uint64; a apresentação soma escudos em int64. Isso não torna os demais caminhos amplos nem significa que todos devem receber o mesmo teto.

Não foi identificado acumulador de score/overkill nas interfaces de batalha/progresso revisadas. Event.amount recebe dano efetivo no HP, já limitado pelo HP restante: não é fonte suficiente de overkill para o futuro score. Score é implementação nova, distinta dos créditos provisórios.

## Domínios — preservar a diferença

| Domínio | Direção aprovada / tratamento planejado |
|---|---|
| HP, escudo, dano e score | int64, valores não negativos, saturação segura em 10^12; regras de mínimo/zero dependem da operação |
| Percentuais | Inteiros, escala 10000. 150% = 15000; não impor teto universal de 10000 a todo multiplicador. Chance/mitigação têm limites próprios |
| Modificadores assinados | Domínio separado. Divisão C++ de negativos não equivale a floor matemático; helpers não podem converter negativos em unsigned ou aceitá-los como HP |
| AP e SPD | AP tem escala própria bar_max=1000000; ActionBar limita SPD a 10000. Não converter AP para a escala de porcentagens por coincidência de nome |
| Contadores, revisões e IDs | Preservar uint64/limites e erros de esgotamento. Saturar um ID duplicaria identidade; teto de dano não se aplica aqui |
| Duração, recarga, slot, hit/count e índices | Manter tipos/limites adequados ao domínio; não trocar todos os int/int32 por int64 indiscriminadamente |
| Créditos de laboratório | uint64 com ledger validado e overflow guardado; não é score. Teto 10^12 não foi automaticamente aprovado para moedas |
| Coordenadas, proporções, câmera e gesto de UI | double/float podem continuar na apresentação. Conversão visual não pode retroalimentar HP, alvo determinístico ou resultado |

## Operações confirmadas e pontos de migração

1. **Dano e consequências:** Battle::apply_damage trabalha com power/absorbed/applied int. Guarda clássica faz divisão por 2 antes do escudo; preservar arredondamento para baixo e o caso 1→0. Absorção completa pode deixar dano no HP igual a zero; não recriar um ponto de dano depois da camada de escudo.
2. **StandardShield:** calcula max(1, max_health/100), sem multiplicação intermediária. A fórmula atual é segura nessa operação dentro do domínio atual; ampliar seu tipo não autoriza alterar relógio, geração, reaplicação ou o mínimo específico aprovado.
3. **Efeitos:** EffectDefinition.amount transporta pontos, modificadores AP assinados e SPD conforme o kind; Status.magnitude transporta valores de consequências/proteção. Na migração, validar por discriminante ou introduzir wrappers específicos; não aplicar saturação genérica a toda duração/SPD/AP.
4. **Condição de HP:** ability_profile já compara health*100 com maxHP*percent usando casts int64. Para HP≤10^12 e percent≤100, esses produtos cabem; a função não demonstra um overflow atual. Porém a comparação entre duas frações HP/maxHP da futura A2 pode chegar a 10^24 por produto cruzado e precisa de algoritmo exato seguro.
5. **AP:** produto SPD*wait é promovido para int64; escala e limite de velocidade atuais, AP inicial/reset e checks do relógio são relevantes. Não foi observado aqui motivo para acusar overflow do cenário atual por esse produto. Novos limites exigirão revalidação, não remoção dos guards.
6. **Persistência entre confrontos:** Stage usa local int current; SEQ transporta HP/runtime e PAR compartilha o estado do Battle. Migrar esses caminhos em conjunto com os tipos, preservando políticas e identidades existentes. Nada introduz cura/ressurreição implícita.
7. **Eventos e cues:** Event.amount precisa acompanhar o dano; VisualCue copia Event. Preservar causalidade, hit_index, revisões e deduplicação. Quantidade ampla em HP com evento estreito continuaria incorreta.
8. **Apresentação:** shield_display tem soma int64 e local health int; razões são double para desenhar. HUD usa `%d / %d` para HP. Atualizar formato e argumentos juntos; simplesmente passar int64 a `%d` é incompatível. std::to_string/streams possuem overloads amplos, mas consumidores e intermediários devem ser conferidos.

## Compatibilidade e versões

| Fronteira | Observação atual | Ação posterior |
|---|---|---|
| Save v1 / SavedGame | Perfil, equipe por IDs e ledger entre batalhas. Não grava HP, dano, escudo ou AP de batalha | **Ampliar HP não exige por si só save v2.** Manter v1 se semântica/dados de progresso continuarem compatíveis; testar roundtrip e recusa. Regras/score/recompensas novas podem exigir versão própria e migração explícita |
| Ledger | Validate_progress verifica totais, ordem, recompensa provisória e overflow; restore inicia nova tentativa | Preservar idempotência; não reapurar resultados antigos por novas fórmulas |
| Manifesto AUT-14 | Parser numérico usa int e recusa fora do domínio; valida referência estrita. Ainda não é configuração carregada pelo HUD | Separar quantidades amplas de contagens/versão; documentar alteração de schema quando a superfície mudar. Não aceitar ficheiro arbitrário como regra de produto |
| Snapshot / InitialHealth / runtime | Estado em memória, não formato de save | Mudar tipos em todos os participantes e recompilar; não tratá-lo como migração de save já existente |
| IO Windows/Unreal | Limites de bytes (1 MiB, DWORD/streamsize/TArray) são outra dimensão | Não elevar limites de arquivo para acomodar HP maior; falhas de IO/recuperação continuam contratos próprios |
| Ponte Unreal e stage | Fontes canônicas compiladas pela ponte; stage contém snapshot/hash | Recompilar os consumidores após mudança de header; não misturar DLL/objetos antigos. Stage atual só fica obsoleto se as entradas canônicas mudarem, não por este relatório |
| Replay e regras | Ordem/IDs atuais têm contratos; não há comprovação bit a bit em ARM/iOS | Versionar regras/RNG/dados ao evoluir. Inteiros são necessários, mas não provam sozinhos determinismo entre plataformas |

Não há motivo para instalar bibliotecas. O MSVC alvo não deve depender de um `__int128` universal. A escolha concreta entre algoritmo portátil e intrínseco isolado será justificada no SIS-02-B; testes de equivalência antecedem qualquer alternativa por plataforma.

## Próxima entrega — SIS-02-B

Entregar utilitários mínimos, ainda sem migrar o Battle:

- domínio de quantidade não negativa e teto 10^12; validação separada de inputs inválidos e saturação de resultado positivo;
- soma saturada e multiplicação/divisão inteira com floor, protegendo intermediário antes de overflow e saturando somente o resultado final;
- percentuais em basis points com escala explícita; não quantizar razões de HP antes de comparar;
- comparação exata de frações com denominadores positivos; igualdade real para desempate, sem double nem produtos cruzados inseguros;
- tratamento explícito de zero/denominador zero/negativos; documentar que modificadores assinados não usam implicitamente o helper de quantidade.

Exemplo obrigatório: (10^12 × 10^12) / 10^12 deve resultar em 10^12, embora o produto não caiba em int64. Saturar o produto antes da divisão daria resultado errado.

## Cortes propostos para SIS-02-C — somente depois de B

1. **C1 — armazenamento e executor:** tipos de quantidade, entrada/validação, HP inicial/runtime, status/efeitos quantitativos, dano/guard/escudo/revive e Event. Sem reescalar rosters históricos nem introduzir DEF/crítico por inferência.
2. **C2 — coordenadores e consumidores:** Stage/SEQ/PAR, perfis de habilidade, cues, seleção por HP e texto/HUD. Pode ser necessário um único corte compilável com C1 para não deixar fronteiras estreitas; não entregar um estado intermediário que truncaria valores.
3. **C3 — codecs, versões e integração:** revisar manifesto, save/ledger, versão de regras e stage; manter v1 quando compatível. Não criar migração desnecessária nem quebrar leitura antiga silenciosamente.
4. **Depois:** outcomes/RNG e funil da fila REG. Comparação exata da A2 usará B; score receberá payload/algoritmo próprios quando for seu bloco.

## Matriz de testes futura — não executada nesta entrega

| Área | Casos necessários | Evidência existente a reutilizar |
|---|---|---|
| Utilitários B | 0/1, teto−1/teto/teto+1; soma; fração com floor; produto grande e quociente que cabe; quociente que satura; denominador zero e negativos | Nova bateria pequena de fronteiras; não executar engine para aritmética pura |
| Razões B | Frações equivalentes; HP quase igual no teto; desigualdade nos dois sentidos; denominadores positivos; sem falso empate por exibição arredondada | Casos da A2 no complemento final |
| C: dano/escudo | 1→0 em Defesa clássica, absorção completa, excesso, multi-hit, mínimo1 do gerador padrão, reaplicação e expiração; valores >INT32_MAX e até teto | standard_shield_tests, classic_visual_tests, battle_tests e resolution_tests conforme impacto |
| C: perfis/R04 | Reflexão/explosão/revive e morte simultânea com dados amplos, relógios inalterados | ability_profile_tests / resolution_tests / visual_session_tests; escolher nomes existentes na hora de executar |
| C: encontros | Persistência HP/recargas/efeitos sem nova cura; IDs, budgets e ordem inalterados | stage_tests, sequential_tests, shared_boss_tests e demais cenários PAR pertinentes |
| C: fronteiras | Save v1 roundtrip, histórico sem crédito duplicado, manifesto válido e recusa de números malformados/overflow, formatação ampla sem cast estreito | save_codec_tests, save_game_tests, visual_save_tests, encounter_manifest_tests |
| Integração | Build da ponte com mesmos resultados pequenos; novo stage somente após código alterado; aceites de leitura em tela separados | AUT-16 por área; compilar não autoriza teste gráfico |

Não foi reexecutada nenhuma bateria nem aberto editor/aplicativo. Nenhum defeito novo de runtime foi reproduzido; os riscos descritos são bloqueios da expansão para a escala aprovada, não uma declaração de que os baselines atuais estejam quebrados.

## Estado e limites

SIS-02-A encerrado no escopo de inventário/plano. SIS-02-B e C ainda não implementados. Sem alteração de valores, fórmulas, saves, recursos, percentuais do projeto ou instalação. Internet/Android/telas não utilizados. A primeira conferência da intro continua provisória e independente deste bloco.
