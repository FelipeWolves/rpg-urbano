# Clara — análise de escudo associado à básica e comparação com VIS-01

19/11/2026. Pesquisa pública documental e comparação estática com o projeto. Sem executar Dislyte, acessar serviços de jogo, abrir Unreal, alterar regras de combate ou importar conteúdo. Cenários abaixo são cálculos ilustrativos, não medições do cliente.

## Decisão posterior do projeto

Após esta comparação, o usuário aprovou mínimo de1 ponto para a aplicação válida do escudo direto de1%/3 turnos. Exemplos abaixo de100PV máximos que produziam zero descrevem a revisão anterior; agora produzem1. A análise de Clara e a falta de confirmação de arredondamento do Dislyte permanecem inalteradas. Não aplicar esse mínimo a ausência de cura excedente.

## Conclusão e qualidade da evidência

Não confundir habilidade geradora com o efeito aplicado. Nas descrições comunitárias consultadas, a básica Queen's Blessing cura com base no HP máximo de Clara; a passiva Queen's Protection, após ascensão, converte excedente de cura em proteção temporária. Não se trata de uma concessão direta de1% do máximo.

[Gacha.gg](https://www.gacha.gg/dislyte/characters/clara-hera) informa cura de5% do máximo de Clara. [Dislyte Wiki](https://dislyte.fandom.com/wiki/Clara_(Hera)) descreve conversão de120% da cura excedente em escudo por2 turnos, condicionada à ascensão da passiva. A [reprodução histórica de patch3.1.1](https://playdislyte.com/?p=4820) também registra essa conversão. São fontes secundárias, não auditoria de código ou certificação da versão atual.

Há divergência na quantidade/seleção de curas: wiki apresenta1 alvo, Gacha.gg2, guias podem apresentar3 ou misturar ressonância. A [lista oficial de ressonâncias](https://dislyte.farlightgames.com/detail_12_21.html) aparece indexada com modificadores de cura/quantidade, mas a abertura retornou apenas a estrutura da página. Não foi obtido texto primário completo atualizado do kit. Portanto, não fixar quantidade de alvos, repetição por alvo ou kit live por esta pesquisa. A comparação abaixo é por um único evento de cura, sem equipamentos, ressonâncias ou modificadores.

## Modelo matemático de comparação

Tomando os coeficientes publicados como hipótese documental:
- cura oferecida H = 0,05 × máximo de Clara;
- vida faltante F = máximo do protegido − vida atual;
- cura efetiva = min(H,F);
- excedente E = max(0,H−F);
- escudo resultante S = 1,20 × E.

Não é1% do HP do recebedor; tampouco120% de toda cura se parte foi usada para restaurar vida. A borda do ícone não determina por si só a fórmula geradora.

## Cenários por alvo

Clara com1000PV máximos oferece50 de cura. Protegido com1000PV máximos, sem escudo inicial e sem modificadores:

| PV antes | Cura efetiva | Excedente | Novo escudo, modelo consultado | PV depois |
|---|---|---|---|---|
|900|50|0|0|950|
|980|20|30|36|1000|
|1000|0|50|60|1000|

Esses valores são inteiros para não pressupor o arredondamento interno do Dislyte. Não inferir reaplicação/acúmulo a partir do exemplo; o modelo calcula somente o novo escudo oferecido por um evento.

No nosso contrato vigente, o mesmo aplicador com1000PV máximos concede10 pontos diretamente, sem cura, por3 turnos naturais. Seu resultado não depende da vida faltante do protegido. Ambos são capazes de gerar zero por motivos diferentes: nenhuma cura excedente no modelo de Clara; valor inferior a1 após nosso cálculo inteiro no contrato atual.

## A ambiguidade de “pouca vida”

O código atual usa máximo do aplicador, não vida atual do portador. Aplicador com10000PV máximos e1PV atual ainda gera100 pontos; aplicador com99PV máximos gera0. Protegido com1PV continua elegível; ele não faz a força virar0 só por estar quase morto.0PV do protegido significa alvo inelegível. Não arredondar HP positivo já armazenado para0 ao calcular uma porcentagem de escudo.

Arredondamento interno da cura/conversão no jogo de referência não foi comprovado. Exemplo apenas matemático:2,5 de cura excedente ×1,2 =3; se truncar cedo2,5→2, o resultado final pode virar2. Isso mostra a importância da fronteira de arredondamento, não demonstra qual algoritmo Dislyte executa. Nosso efeito atual /100 usa uma única divisão final e não precisa mudar por esse exemplo sozinho.

## VIS-01 — cálculo e aparência são responsabilidades distintas

A faixa atual recebe pontos reais já calculados pelo núcleo. Vida preenchida usa PV atual/PV máximo do protegido; sobreposição usa min(pontos de proteção,PV atual)/PV máximo do protegido. Assim, ela pode mostrar10,36 ou60 pontos sem saber se vieram de percentual direto ou cura excedente.

Nos cenários acima:950/1000PV sem escudo não tem faixa/ícone de escudo;1000/1000PV com36 pontos cobre3,6% da barra total; com60,6%. Se a proteção exceder a vida atual, a área sobreposta satura na vida preenchida; o painel continua mostrando todos os pontos.

Isso descreve a UI que decidimos/implementamos, não medição da geometria da barra de Dislyte. Não alterar pontos do combate por serem pequenos ou pouco visíveis: uma faixa mínima, se desejada futuramente, seria decisão visual explícita.

## Ajuste arquitetural recomendado para decisão

Manter separado:
1. Tipo de proteção e regras de absorção.
2. Origem/fórmula da aplicação (percentual do máximo, excedente de cura, outra base).
3. Duração e políticas de reaplicação.
4. Ícone/categoria e representação visual.

Nossa implementação StandardShield ainda reúne1% e3 turnos em um tipo fechado. Ela está correta para o contrato anterior, mas não é um modelo genérico de todo escudo sem borda. Antes de ampliar, decidir se vira uma aplicação específica do efeito comum, mantendo suas evidências, enquanto outras habilidades fornecem outros valores/durações.

Não adicionar mínimo automático de1, converter o escudo atual em cura ou adotar duração2 por inferência. Cure/overheal ainda não existem no núcleo; introduzi-los requer contrato/testes próprios. Não importar o kit de Clara nem declarar regras comuns de bordas, roubo, acúmulo ou persistência sem evidência/decisão.

## Pendências específicas desta comparação

- Confirmar com fonte primária atualizada as descrições e modificadores relevantes, se for necessária fidelidade do exemplo externo.
- Decidir se1%/3 turnos continuam uma aplicação de referência ou se o usuário deseja mudar seus coeficientes; separar essa decisão da aparência.
- Definir eventual geração por excesso de cura e momento de arredondar somente quando essa família for implementada.
- Realizar aceite visual dos casos zero, baixo valor, vida parcial e excesso de proteção. A pesquisa não substitui teste do pacote.

Nenhum contrato foi modificado nesta análise. VIS-01 fica com revisão de escopo da fórmula/gerador registrada, além do aceite visual pendente.
