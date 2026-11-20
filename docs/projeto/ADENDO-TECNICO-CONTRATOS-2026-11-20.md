# Adendo técnico — contratos consolidados

Recebido em20/11/2026. Fonte original preservada em entregas/00-entrada-geral/recebidos/2026-11-20_adendo-tecnico-contratos_v01.md. **Decisões documentadas; implementação e validação ainda pendentes.** Complementa RESPOSTAS-ENT-2026-11-20.md e pontos1–3. Substitui os diagnósticos de falta de resposta nos temas explicitamente resolvidos, sem alterar silenciosamente código/testes históricos.

## Regras adicionais recebidas

| Área | Fechamento recebido | Trabalho futuro |
|---|---|---|
| FelipeA2 | Passiva por aliado atingido/habilidade,MaxPartySize,consumo total,A1 eStandardShield;pré-cargaMaxPartySize−1no tutorial | Implementar contagem por ação/alvo e integrar reaçõesR04;6×6 continua extensão não implementada |
| Destino do escudoA2 | A expressão alternativa do adendo não substitui a confirmação específica anterior:aliado vivo de menor percentualHP,incluindoFelipe | Manter esse destino como regra vigente;empate ainda precisa desempate determinístico no contrato técnico |
| Block/mitigação | Base3%/Michel10%total,tetos50%/100%explícito;anulação completa;TrueDamage/DoT não sorteiamBlock;mitigação somente por fonte,somada até50% | Novo sistemaREG;Block técnico legado não é esse bloqueio já validado |
| Trilho de debuff | Só após hit acertar e não ser esquivado/bloqueado;entãoImmunity→chance→ACC/RESIST,piso20%para nãoirresistível | Elegibilidade porMiss/Block agora respondida;separar estado de imunidade a dano e a debuff |
| Supressão | Impede reativas/auto-revive/prevenção de morte internos ainda não ativados;estados já ativados convertidos embuff continuam,assim como bordas protegidas eestruturais | Distinguir origem de passiva versus estado ativo;preservar efeitos de morte já enfileirados conformeENT-06/R04 |
| Freeze/Petrification | CancelamCounter/Assist/Pursuit pendentes antes de executar;Silenceisolado não cancela básicaA1 | Formalizar cancelamento sem consumir ação/alterar causalidade indevidamente |
| Categorias | Comum:dissipável/purificável,roubável/transferível;operações contra inimigo comACC/RESIST,remoção direta no aliado. Silver:removível,semroubo/transferência. Golden/Estrutural:não removíveis por operação comum;estados ativos preservadossobSupressão | Formalizar tabelas de permissão por operação;categoriaEstrutural não é sinônimo automático do efeito técnico legadoStructuralShield |
| SilverShield | 15%MaxHPaplicador,2naturais;coexiste comStandardShield;consumirStandardprimeiro;reaplicarmax(pontos),max(duração),sem soma;dissipável,nãoroubável/nãotransferível;preservadoentreondas se durar | Criar tipo/gerador novo e casos de consumo emcamadas;não alterarESCpadrão |
| Ondas | Primeiro removerbuffs/debuffscomuns eStandardShieldde remanescentes;preservarSilver/Golden/Estrutural,cargas/CD;depois curar30%MaxHPdealiadosvivos,comfloor/cap,semrevive | Implementar versão explícita de políticaWAVE;persistênciasSEQ/PAR permanecem nos contratos próprios |
| Score | Acumular totais e aplicarfloor(totalefetivo+25%overkill)uma vez no fim;int64,teto10^12,idempotência porAttemptID;SurvivalBonusopcional,sem padrão universal | Implementar agregação/teto e configuração do modo,sem pontuação porhit arredondada prematuramente |
| Interface/vídeo/Android | Tarefas de engenharia sem nova pesquisa de produto exigida;AUT-01–11 e diagnóstico do ambiente continuam | Não pedir novamente decisões que chegaram;conferir dependências e realizar testes apropriados quando autorizado |
| Clássico/DJ | SP,Limite,ATB e julgamento musical ficam nos contratos dedicados;rankings separados3/4/5mantidos | Não simular esses sistemas como implementados |

A matriz de ondas passa a abranger explicitamente debuffs comuns, além dosbuffs. Preservação de estados especiais entre ondas não os torna eternos nem restaura duração consumida.

## Distinções técnicas que permanecem obrigatórias

- **Determinismo numérico:** int64/teto10^12/double/floor são escolhas recebidas. Double não comprova sozinho igualdade de replay entre compiladores/plataformas;testar e controlar operações/versões. Não declarar esse objetivo como alcançado nem mudar para outra representação sem registrar a decisão.
- **AmbienteAndroid:** D:\Android é instalação confirmada doAndroidStudio. SDK/NDK/JDKcompatível/AVD devem ser localizados/verificados;nenhuma nova evidência de instalação/configuração chegou neste adendo. ANDROID-DEPENDENCIAS.md permanece vigente.
- **Limites/resolução:** formalizar equaçãoACC/RESIST,comportamento acima do teto numérico e finitude/conversão,sem clamp silencioso;escopo deirresistível frenteImmunity e regras específicas deDoT não se deduzem da ordem dofunil.
- **BordasdeA2:** empate no menorHP%,contagem de aliado atingido porhitletal/escudo/bloqueio,momento da reação emmultihit/área,refill na mesma ação eescudo quandoA1não puder ocorrer ainda precisam de regra concreta de execução. Não são ausência de fórmula/cargas/destino,que já foram respondidos.
- **Compatibilidade:** StandardShield,R04 e baselines anteriores preservados;novas políticas/bloqueio/supressão/escudos exigem implementação e regressão próprias. Nenhuma frase do adendo certifica sistemas inexistentes como já validados.

Esses detalhes devem ser fechados no bloco afetado,sem impedirAUT-01–04 e infraestrutura neutra. Não reabrir perguntas principais nem exigir nova pesquisa externa para a engenharia já especificada. O título “fechamento” registra decisões;não substitui critérios/verificações de software.

## Atualização da fila para quando houver limite

1. AUT-01→02→03→04 continuam base de apresentação e interação com recursos existentes.
2. AUT-06/REG-03 passam a usar a matriz consolidada,supressão e cancelamentos declarados,em bloco separado com testes.
3. HABFelipe/ESCespecializado/SISnumérico/WAVEscore podem receber contratos de implementação com a nova matriz;fechar somente bordas reais acima,sem repetir o estudo dos12documentos.
4. AUT-10/11 eAndroid seguem planos internos já registrados;instalação/execução só no escopo autorizado da retomada. Clássico/DJ/servidor mantêm fases próprias.

Sem código,catálogo executável,teste,build ou programa executado nesta recepção. Originais intocados e percentuais mantidos.

**Complemento final recebido20/11:** [A2, ACC/RESIST e ponto fixo](COMPLEMENTO-FINAL-A2-ACC-PONTO-FIXO.md). Bordas de A2 e equação/irresistível respondidos; teto10^12 por saturação; double substituído por ponto fixo de escala10000 no núcleo futuro. Não repetir as perguntas resolvidas. Determinismo bit a bit é meta a verificar, não aceite existente. Nenhum código/teste executado nesta recepção.
