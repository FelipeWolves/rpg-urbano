# Complemento final — A2, ACC/RESIST e ponto fixo

Decisões recebidas do usuário em 20/11/2026. Este complemento prevalece sobre as indefinições anteriores dos temas abaixo. **Contrato documentado; código e testes ainda pendentes.** Não executa migração, build ou teste nem reabre os aceites históricos.

## 1. Postura de Vigília — casos de borda respondidos

- Destino do escudo: aliado vivo de menor razão HP/HP máximo, incluindo Felipe. Em empate nesse menor percentual, Felipe tem prioridade; se o empate for só entre outros aliados, menor índice de slot da formação.
- Ganho de carga: um por aliado atingido na habilidade inimiga, incluindo golpe que causa dano no HP, absorção integral por escudo, Block e golpe letal. Miss e Imunidade a Dano/Invencibilidade não contam. A reação exige que Felipe sobreviva e permaneça apto conforme a janela R04.
- Deduplicar por ação/habilidade e alvo: multihit no mesmo aliado não multiplica cargas. Morte do aliado atingido não invalida a contagem daquele golpe letal.
- Contabilizar durante os hits; enfileirar a reação A1 + StandardShield somente ao fim da habilidade inimiga, na janela R04 de sobreviventes, após o encerramento efetivo do loop de hits e as etapas anteriores de morte/revive/Victory Lock. Não inserir contra-ataque entre hits.
- Máximo de um disparo por habilidade inimiga. Ao consumir MaxPartySize, descartar excedentes gerados por essa mesma habilidade, sem segundo disparo/refill residual dela.
- Capacidade máxima do modo continua sendo o limite de cargas, não número de vivos. Perfil6×6 permanece futuro. Tutorial usa MaxPartySize−1; não atribuir pré-carga ao restante do jogo.

As definições respondem desempate, acerto, janela e quota. Preservar R04: não executar reação após vitória ou quando o autor estiver impossibilitado de agir. Não usar este complemento para inventar comportamento de uma habilidade sem alvo válido ou para remover proteções de morte já contratadas; integração de comandos precisa respeitar suas validações.

## 2. ACC/RESIST e irresistível

Depois de satisfazer elegibilidade do efeito, Immunity e chance base da habilidade:

```text
ResistEfetiva = clamp(RESIST_alvo - ACC_atacante, 20%, 100%)
ChanceAplicacao = 100% - ResistEfetiva
```

A chance do teste ACC/RESIST tem teto80%; isso é condicionado ao sucesso da chance base, não probabilidade total de80% para toda habilidade. Em ponto fixo: piso2000, teto10000, ChanceAplicacao=10000−ResistEfetiva.

Irresistible ignora apenas o teste ACC/RESIST e seu piso natural. Não ignora Immunity nem imunidade estrutural do chefe, salvo permissão explícita da habilidade. Continua sujeito à chance base e ao acerto/Miss quando houver ataque; habilidades puras sem ataque/dano não exigem teste de contato inexistente. Efeitos associados a hit bloqueado continuam inelegíveis conforme adendo anterior, salvo regra específica futura.

## 3. Saturação e representação determinística

- HP, escudo, dano e score: int64 com saturação explícita em 10^12 (1.000.000.000.000). Valores acima do teto não geram wrap nem falha técnica só por exceder esse teto.
- A decisão **substitui double por aritmética inteira de ponto fixo no núcleo lógico**, com escala10000 para percentuais:100%=10000;0,01%=1. Não usar double como intermediário do dano/probabilidades para depois converter.
- Multiplicação/divisão intermediária deve ser protegida contra overflow, com int64 quando seguro, alternativa128-bit ou algoritmo de divisão/multiplicação exata equivalente quando necessário. Não assumir que um tipo128-bit nativo está disponível em todos os compiladores; manter implementação portátil, sem dependência nova antecipada.
- Aplicar floor/truncamento inteiro ao resultado final conforme contrato de cada operação/hit. Para quantidades não negativas, divisão inteira implementa floor; C++ trunca em direção a zero em valores negativos, portanto diferenças/modificadores assinados exigem tratamento explícito.
- Saturar o resultado da operação no limite de gameplay, sem saturar prematuramente intermediário antes de divisão e mudar o resultado. NaN/infinito de fontes externas, texto malformado e domínio negativo não foram autorizados como valores válidos por esta política.
- Meta: determinismo exato bit a bit em Windows x64, Android ARM64 e iOS e em replays. **Não comprovado nesta documentação.** Exige execução equivalente, ordem de eventos/sorteios/iterações estável, PRNG e catálogos versionados, além da representação numérica.
- A mudança é do núcleo de regras. Posição de câmera, projeção e desenho na Unreal podem continuar usando tipos gráficos apropriados, sem influenciar cálculo autoritativo.

Comparar percentuais de HP para A2 por razões exatas, sem double ou produtos que transbordem; não criar empates por arredondamento do número exibido na interface ou por quantizar previamente o HP% em basis points.

## 4. Implementação futura por partes

1. Mapear tipos/serialização/interfaces e operações de taxas no núcleo; preservar cenários pequenos, ESC/R04 e contratos existentes até a migração explícita.
2. Entregar utilitários mínimos de aritmética segura, saturação e comparação de razões, com casos de fronteira. Não adotar biblioteca/plugin automaticamente.
3. Introduzir os tipos e o funil nos blocos SIS/REG apropriados, verificando compatibilidade do save/adapter e registrando versão de regras. Não reescalar valores históricos por inferência.
4. Implementar A2 com deduplicação e quota por habilidade, usando R04 e seleção determinística do protegido.
5. Verificar replays entre builds/plataformas quando esses ambientes estiverem disponíveis. Compilação Windows ou teste nativo isolado não prova determinismo em ARM/iOS.

AUT-01–04 e apresentação podem seguir independentemente dessa migração. Android permanece com o diagnóstico de toolchain pendente; este complemento não instala ferramentas nem informa componentes novos.

## 5. Critérios de teste planejados — não executados

- A2: empate com Felipe; empate só entre outros por slot; percentuais quase iguais sem empate falso; excluir mortos do escudo, contar hit letal recebido por aliado; bloqueio/escudo contam, Miss/invencibilidade não.
- A2: área + multihit, nenhuma reação intermediária, um disparo após habilidade, excesso descartado; autor morto/controle/vitória respeitando R04.
- ACC/RESIST: diferença negativa→20%resistência/80%aplicação; diferença100%+→100%/0%; chance base0/100%; irresistível com e sem Immunity e com habilidade sem ataque.
- Numérico: abaixo/no/acima de10^12; produto intermediário grande que só depois da divisão cabe; divisão fracionária/floor; soma de score e saturação; Defesa1→0 e dano totalmente absorvido sem piso aplicado novamente ao HP.
- Regressão de save e ponte; replay equivalente em todas as plataformas de destino antes de anunciar garantia bit a bit.

Nenhuma implementação/teste executado; percentuais de progresso mantidos. As escolhas de produto acima deixam de ser perguntas abertas. Trabalho restante é codificação, formalização das integrações realmente necessárias e verificação.
