# Recepção da pesquisa — habilidades, itens e hierarquia

19/11/2026. Revisão documental comparada aos contratos e à implementação atual. Documento fornecido pelo usuário preservado em [pesquisa original](../batalhas/PESQUISA-HABILIDADES-ITENS-HIERARQUIA-USUARIO.md). Suas propostas são material de pesquisa, não instruções para substituir contratos vigentes. Sem consulta externa, execução de testes, abertura da Unreal ou alteração de código nesta entrega.

## Resultado

O material amplia o mapa de requisitos, sobretudo separação de origem dos efeitos, âncoras temporais, escopos de disparo e exceções de chefes. Não fecha todas as lacunas e não demonstra a arquitetura interna do Dislyte: observar que um efeito desliga equipamentos permite propor nossa separação de módulos, mas não comprova como o jogo foi programado.

As informações de patches, datas, personagens e números são alegações do documento, ainda sem auditoria externa nesta recepção. O próprio texto apresenta conflito de datas para3.4.50 e admite catálogo incompleto. Não converter essas alegações em catálogo executável ou identificação da versão oficial atual. “Atual” nas linhas de patches antigos deve significar regra alegadamente ainda vigente, não versão instalada.

## Matriz de incorporação

| Tema | Comparação com o projeto | Tratamento |
|---|---|---|
| Até3 posições, básicas/ativas/passivas | Compatível com perfil Equipes | Preservar; não exige refazer HAB-01 |
| Habilidade/equipamento/divinate/regra de modo | Boa separação conceitual; camadas externas ainda não implementadas | Proposta de origem explícita por efeito, sem instalar plugins |
| Contadores de oportunidade e ações | Já existem contadores com contratos próprios | Mapear significados antes de novos contadores; não renomear rodada para onda |
| Extras e clocks de recarga | NaturalOnly/EveryTurn já existem; descritores ampliados são extensão | Não substituir contratos por inferência de exemplos |
| Efeitos por início/fim e vida do aplicador | Mais amplo que os clocks mínimos atuais | Novo bloco de âncoras/persistência, com especificação e testes próprios |
| Limites por fonte/alvo/time/cadeia | Amplia famílias já delimitadas | Evolução declarativa futura, sem alegar cobertura atual de todos os escopos |
| Q07: morte/vitória e reações | Conflita com R04/RES-01 | Preservar reflexão e explosões FIFO inline, revive e Victory Lock antes de reações; não mover vitória para EndCheck pós-counter |
| Q08: defesa até turno natural | Difere da defesa consumida na ação que a utiliza ou expirada na próxima oportunidade | Manter regra atual; variante clássica exige decisão explícita; usar nome Defesa, não Guarda |
| Q10: início de ronda | “Round” pode significar onda no texto de habilidade | Distinguir onda, rodada e oportunidade; não ligar passiva de onda ao contador round sem confirmação |
| Q11: três equipes | SEQ-01 já implementado em seu escopo | Não reiniciar bloco nem impor equipamento exclusivo inexistente; novos resets/persistências seriam políticas adicionais |
| Q12: frentes independentes | É proposta; não confirma comportamento histórico | Preservar perfil PAR-02 de HP único/barra global escolhido pelo usuário; não substituí-lo por três chefes independentes |
| Afinidade, ACC/RESIST, crítico e dano verdadeiro | Não fazem parte do dano mínimo atual | Extensão futura; faltam contratos e auditoria de fórmulas/exceções |
| Molduras Green/Red/Silver/Golden | O próprio texto informa divergências | Não inferir permissões só da cor; propor capacidades explícitas de dissipar/roubar/transferir/bloquear |
| Bosses, invocações e área | Catálogo de casos amplia planejamento | Sem importar personagens/arte; exemplos viram cenários originais quando implementados |
| Itens, runas e bênçãos | Equipamento não equivale a consumível | Não implementar estamina como item de combate por este documento |
| ATB, servidor, desempenho e mobile | Continuam extensões distintas | Não presumir entrega ou limite técnico a partir de relatos do outro jogo |

## Inferências que não devem virar regras confirmadas

- Ocean reduzir recargas no início do turno demonstra um gatilho do equipamento; sozinho não estabelece o instante da redução natural de todas as habilidades. Manter nossa fronteira validada até evidência/decisão específica.
- Uma condição de Toland aceitar igualdade de SPD não demonstra prioridade universal do lado atacante em toda iniciativa empatada.
- Ausência de contraexemplo para timeout não comprova a regra oficial. ResolveChainBeforeTimeout permanece decisão própria do projeto.
- Percentuais, “uma vez por turno”, comportamento Silver/Golden e True Damage exigem contexto e exceções por efeito. Não generalizar invencibilidade, escudo e imunidade como uma única propriedade.
- Referências Q/PEN e contagens49/168/25 correspondem a uma fase histórica do projeto. O baseline vigente continua213 gerais, mais6 do adaptador,6 de apresentação e6 clássicos; nada foi reexecutado nesta recepção.
- Duas fórmulas estão em imagens incorporadas ao Markdown. Não transcrever fórmulas por suposição nem implementar a partir de espaços vazios na leitura textual.

## O que ainda falta para o próximo efeito

### Escudo percentual

O exemplo já aprovado continua:10% de100HP=10 pontos;10% de50HP=5 pontos. Isso não decide sozinho seu ciclo de vida. O novo documento cita escudos com bases diversas, inclusive HP perdido; não estabelece uma regra única para nosso escudo de referência.

Antes de codificar, fechar: HP atual de quem (portador/aplicador); cálculo na aplicação ou recalculado durante a duração; arredondamento; reaplicação (troca, maior valor, soma ou instâncias); consumo e prioridade entre escudos; duração/tick; morte/revive; limite e ataques que ignoram proteção. A interface já aceita os pontos reais calculados, sem exigir que todos os escudos tenham a mesma base.

Proposta para decisão posterior, ainda não adotada: exemplo original simples de10% do HP atual do portador na aplicação, arredondado para baixo, convertido em pontos consumíveis e sem recálculo automático. Isso não deve ser confundido com uma regra comprovada do Dislyte.

### Silêncio e passivas

Ainda falta uma matriz inequívoca: bloqueio de ação, silêncio de ativas, supressão de passivas, congelamento/petrificação e exceções. Manter básica na posição1 conforme decisão do projeto; não tratar Block como silêncio e não tratar FreezeCooldowns como unidade congelada. Cor do ícone não resolve essas diferenças.

### Origem e permissões dos efeitos

Preparar contrato independente para origem (habilidade, equipamento, modificador de modo), fase de disparo, alvo/alcance, quota e permissões. Essa preparação não habilita automaticamente inventário, equipamentos, divinates ou bênçãos.

## Sequência recomendada

1. Preservar R04, SEQ/PAR e a defesa atual. Não repetir testes já aprovados só pela chegada do documento.
2. Fechar um contrato pequeno para escudo percentual, seguido de implementação nativa e testes de absorção/arredondamento/reaplicação, sem copiar kits proprietários.
3. Reutilizar a apresentação de escudo já pronta e realizar o aceite visual autorizado da revisão atual; não é necessário esperar catálogo completo.
4. Especificar separadamente silêncio/supressão e âncoras temporais. Só depois equipamentos e efeitos de modo.
5. Auditar fontes públicas pontualmente quando uma afirmação externa for necessária para fechar uma regra; não instalar ferramentas, executar jogos ou assumir arquitetura interna.

Documento recebido e analisado; regra percentual ainda em especificação. Nenhum bloco encerrado foi reaberto automaticamente.

## Devolutiva consolidada

As perguntas detalhadas, prioridades e campos de resposta estão no registro único [PENDENCIAS.md](PENDENCIAS.md): ESC-01–07, VIS-01–03, REG-01–03 e SIS-01–04. O usuário pode responder todos juntos. Receber a pesquisa não aprova automaticamente as recomendações.
