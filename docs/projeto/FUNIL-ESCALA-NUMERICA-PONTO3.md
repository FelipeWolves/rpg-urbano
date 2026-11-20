# SIS-02 — funil por hit e escala numérica (ponto3)

Decisão consolidada recebida em20/11/2026. Complementa os pontos1/2 e ENT-07. **Definido para implementação futura; nenhuma migração numérica ou alteração de combate executada.** Os cenários pequenos e aceites atuais permanecem preservados.

## Dois trilhos separados

### A — Dano

1. Imunidade a Dano / Invencibilidade.
2. Esquiva / Miss.
3. Bloqueio probabilístico (base3%+, não aplicável a True Damage; DoT também inelegível conforme ponto2).
4. Crítico: base150% do dano normal, multiplicador total1,5; escalável. Não interpretar150% como bônus adicional de150%. Exceções de True Damage seguem contrato próprio, sem crítico por inferência.
5. Mitigação: DEF + redução percentual por fontes + Defesa Clássica, conforme elegibilidade do tipo de dano.
6. Absorção de escudo.
7. HP: piso1 para golpes comuns não anulados; Defesa Clássica pode produzir0.

As exceções do ponto2 continuam vigentes: True Damage ignora DEF, redução percentual, Defesa Clássica e bloqueio; respeita invencibilidade e escudo. O piso não pode transformar dano bloqueado/invencível/defendido em1 nem gerar dano ao HP depois de escudo absorver integralmente. Não atribuir essa propriedade a todos os DoTs por serem inelegíveis ao Block.

Preservar explicitamente Defesa floor(dano/2), inclusive golpe1→0. A posição exata dos arredondamentos na futura implementação deve respeitar esses exemplos e o fechamento por hit, sem um novo piso após Defesa ou após escudo.

### B — Debuff / Controle

1. Imunidade a Debuffs (Immunity).
2. Chance base da habilidade.
3. Teste ACC vs. RESIST, com resistência natural mínima20% para efeitos não irresistíveis.

Imunidade a Dano e Imunidade a Debuff são estados distintos: não usar um mesmo bloqueio lógico para ambos. Efeito irresistível precisa declaração explícita; piso de resistência não determina sozinho se ele ignora Immunity. Falhar no trilho de dano não significa automaticamente falhar ou passar no trilho de debuff: cada operação deve ter sua elegibilidade declarada. Não restaurar implicitamente o funil anterior que misturava dano e estados.

## Valores para fichas novas

- Referência de escala desejada: personagens10.000–50.000+HP; ATK/DEF1.000–6.000+; chefes com milhões deHP. São faixas de conteúdo, não valores universais de balanceamento.
- Armazenamento de HP, escudo, dano e score em int64, com teto de gameplay10^12 (1.000.000.000.000).
- Cálculos intermediários em double, aplicando floor no fechamento de cada hit, preservadas as exceções explícitas acima.
- Não reescalar cenários de laboratório nem converter saves existentes automaticamente. Novas fichas e sistemas usam a nova escala após migração/testes apropriados.

Teto de gameplay é diferente de limite de representação int64. Double não torna todos os intermediários exatos nem garante replay idêntico em plataformas diferentes. Produtos/razões podem crescer muito além do valor final; validar domínio, finitude e conversão antes de armazenar. Nenhum clamp silencioso, wrap, NaN ou overflow aceito por inferência.

## Preparação técnica no bloco de implementação

- Rastrear tipos/interfaces/serialização e pontos de soma/conversão do núcleo e adaptadorUnreal; testar v1 antes de decidir migração de schema. Atualizar UI/formatação sem confundir valor real e abreviação visual.
- Especificar fórmula exata ACC/RESIST (piso20% não define a equação completa), unidade percentual e limites de atributos/chances. Definir destino de entrada/resultado acima de10^12 (recusa/cap explícito por operação), negativos e fórmulas com valor perdido/excesso.
- Definir se Miss evita completamente o golpe ou produz variante fraca quando prevista; quais debuffs exigem contato, se Block cancela on-hit e alcance de irresistível perante Immunity. São detalhes de elegibilidade, não novas perguntas sobre os dois trilhos já decididos.
- Ordenar sorteios e registrar PRNG/versões para futuro replay; verificar precisão nos alvos realmente suportados. Não declarar determinismo entre dispositivos apenas por escolher double.

## Verificações futuras — não executadas

- Invencibilidade versus Immunity separadas; chance0/100%; resistência natural20%; irresistível conforme flag definida.
- Crítico base100→150 antes dos demais modificadores; nenhum crítico/bloqueio indevido em True Damage.
- Hit comum1 com Defesa→0; escudo absorve tudo→0 noHP; excesso atravessa corretamente; anulados continuam0.
- Casos0/1, faixas de fichas novas, chefes em milhões,10^12 e valor acima do teto com tratamento explícito. Produtos intermediários grandes/NaN/infinito/conversão fora de faixa rejeitados ou tratados por regra definida.
- Regressão dos cenários pequenos e R04/ESC; round-trip de save e ponte de apresentação; comparação de replay em plataformas quando houver builds.

Sem código, teste, execução de programa ou aumento de percentuais nesta recepção.

Atualização20/11: adendo explicitou que debuff porhit só tenta aplicar após acerto não esquivado/bloqueado. Essa elegibilidade deixou de estar indefinida. EquaçãoACC/RESIST,irresistível e tratamento numérico continuam formalização técnica;double não comprova determinismo. Ver ADENDO-TECNICO-CONTRATOS-2026-11-20.md.
