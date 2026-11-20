# Felipe — A2 Postura de Vigília

Contrato de produto recebido em20/11/2026; implementação e testes pendentes. Atualiza ENT-02 e o ponto1 dos detalhes de RESPOSTAS-ENT-2026-11-20.md. Substitui a versão anterior de Olhar da Garoa quanto a nome, cargas, gatilho e escudo; A2 continua passiva não selecionável.

## Regra recebida

- Ganha1carga por membro da equipe (Felipe ou aliado vivo) atingido por uma habilidade de ataque inimiga. Cada alvo conta uma vez nessa habilidade, independentemente de quantos hits recebe. Exemplo: Felipe+4aliados atingidos em área geram5cargas; multihit em1alvo gera1carga.
- MaxCharges é a capacidade máxima de formação do modo, não a quantidade de sobreviventes ou de personagens selecionados:5no perfil padrão;6somente se existir futuro perfil6×6. Esse contrato não implementa nem amplia o limite atual de1–5.
- Ao atingir o limite, consome todas as cargas e dispara contra-ataque A1 Garra do Asfalto contra o autor do ataque que completou as cargas, seguido de escudo automático.
- Destino do escudo confirmado separadamente pelo usuário: **aliado vivo com menor percentual de HP, incluindo Felipe**. Não é obrigatoriamente o próprio Felipe nem quem recebeu o último golpe.
- Usa StandardShield:1% do HP máximo de Felipe, floor com mínimo1 na aplicação válida,3oportunidades naturais do protegido. Preservar absorção, reaplicação, remoção e relógio ESC-01–07; não restaurar o escudo ao curar.
- Tutorial: cenário específico pode começar com4/5cargas para que o golpe do turno2 demonstre a passiva antes de A3. Formação reduzida não reduz MaxCharges. Pré-carga é configuração de tutorial, não bônus de abertura global.
- O texto consolidado não mantém automaticamente as antigas2cargas iniciais ou recuperação de1carga a cada turno: a regra recebida é acúmulo por aliados atingidos. Não misturar as duas versões.

## Integração e casos para formalizar no bloco de código

Deduplicar por identidade da ação/habilidade e do alvo, não por frame de animação. Preservar quotas/estado por ação e não modificar R04: auto-reação deve passar pelo executor apropriado; sem contra-ataque de unidade morta ou após vitória. Supressão e controles devem seguir o contrato da família quando implementados.

Escolhas principais encerradas; detalhes de borda ainda não especificados no texto e a tratar antes do aceite desse bloco: empate no menor percentual deHP; definição de atingido quandoMiss/bloqueio/escudo absorve tudo ou golpe életal; momento da reação dentro de uma habilidade multialvo/multihit e eventual segundo preenchimento na mesma ação; execução do escudo se o contra-ataque não puder ocorrer ou terminar a batalha. Não inventar respostas nem transformar essas lacunas em bloqueio para AUT-01/interface.

## Matriz de verificação futura — não executada

- Ataque simples em aliado e em Felipe:1carga; multihit no mesmo alvo:1carga;área em5membros:5cargas, respeitando limite.
- Formação1–4no perfil5:limite continua5; mortes não reduzem capacidade do modo.
- Ao completar4→5:consumir5, direcionar A1 ao autor causal e aplicar StandardShield ao aliado elegível de menor proporçãoHP.
- Comparar percentuais deHP, não pontos absolutos; excluir mortos; Felipe é candidato.
- Aplicador1000PVmáximos gera10pontos;aplicador55gera1;duração3naturais;reaplicação preserva maiores propriedades.
- Tutorial4/5somente no cenário configurado;runtime normal não recebe carga inicial por inferência.
- Integração morte/vitória/supressão/controle após fechar os detalhes de borda;sem laços reativos ou recompensa duplicada.

Nenhum código/catálogo executável foi alterado por este documento. Nenhum teste ou programa executado. Não reabrir ESC ou aumentar percentuais por especificação.
