# AUT-06 — estados das habilidades

21/11/2026. Código, testes focados e compilação concluídos. Sem abrir editor ou Android. Aceite visual dos novos cartões pendente.

## Apresentação

| Estado | Cartão | Interação |
|---|---|---|
| Básica disponível | Posição1 / BASICA, Pronta | Selecionar, escolher alvo e confirmar |
| Ativa disponível | HABILIDADE, Pronta | Mesma confirmação atual |
| Passiva | PASSIVA com indicador informativo | Não selecionável; pressão longa consulta o painel |
| Recarga | Em recarga + contador positivo | Não selecionável; detalhes mostram recarga restante |
| Condição não atendida | Cadeado + Condição | Motivo indica o limite de PV quando cadastrado |
| Impossibilitado/turno alheio/fim/parada | Círculo riscado + Indisponível | Consulta explica o motivo; não executa |

Contador e motivo são dimensões separadas: uma ativa impossibilitada de agir pode continuar mostrando sua recarga positiva. Zero não cria contador. Passivas puras não recebem temporizador de ação. Ativas híbridas permanecem selecionáveis quando permitidas; sua parte passiva continua descrita no painel, sem virar um botão adicional.

A seleção atual tem destaque explícito. Toque curto em cartão não selecionável limpa uma seleção anterior e mostra o motivo, evitando confirmar por engano a habilidade anteriormente escolhida. Pressão longa preserva a consulta sem executar ação. O painel mantém nome, nível quando cadastrado, descrição, condição e estado atual. Não se cadastraram níveis fictícios nas fichas de laboratório.

## Contrato e limites

`ability_card` é um read model puro, derivado das APIs existentes. Não decide dano, recarga ou aplicação de efeitos; o núcleo permanece a autoridade final. Confere turno, fase, parada, ator vivo, vínculo da skill com o ator e disponibilidade. Ausência/metadados incoerentes permanecem não selecionáveis.

Silêncio, supressão, custos novos e o bloqueio probabilístico3% não foram implementados neste bloco. A básica não ganhou uma regra de silêncio. O identificador legado Block continua representando impossibilidade de agir, conforme contrato anterior. O perfil clássico mantém seus comandos próprios e não é convertido para três posições por esta atualização.

Nem todo estado é alcançável naturalmente com as habilidades do catálogo atual; os testes também exercitam views de apresentação controladas. Isso não cria habilidades ou efeitos de combate para demonstrar os símbolos. As dimensões gerais da barra de combate ainda são as do protótipo; esta entrega não conclui adaptação integral da arena para retrato, DPI ou acessibilidade.

## Verificação

-10 grupos novos: básica, híbrida, passiva, recarga, condição, incapacidade com contador, fim/parada, turno adversário, metadados ausentes e consulta sem mutação.
-32 grupos focados aprovados:10 de cartões +22 de cenários/layout/detalhes. Não somar como casos exclusivos à bateria geral ou à de startup.
-UBT Editor Win64 Development:7ações, saída0; `build/aut06-build.log`.
-Sem execução visual do AUT-06, alteração de regras de batalha, instalação, rede ou Android.

## Aceite anterior e próximo bloco

Usuário confirmou os dois atalhos/layouts e os detalhes do AUT-05, sem problemas observados. Registro separado em AUT-05-ACEITE-USUARIO.json; não atribuir execução visual ao assistente nem afirmar checklist individual de todos os casos sem esse relato. O novo cartão não reabre por si só a inspeção de formações/escudo aceita.

Próximo AUT-07: preferências de apresentação/acessibilidade e persistência local separada do progresso. Usar recursos existentes; integração automática com preferências do SO só após validação própria. Android permanece adiado.
