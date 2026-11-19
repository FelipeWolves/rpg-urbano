# Escudo padrão por habilidade — contrato e implementação

19/11/2026. Regras próprias fechadas com o usuário em ESC-01 a ESC-07. Não representam paridade certificada com Dislyte. Estado de validação e limites abaixo.

## Regra comum

| Tema | Comportamento |
|---|---|
| Origem | Habilidade aplicada em si ou em aliado vivo |
| Força | floor(HP/PV máximo do aplicador /100), uma vez na aplicação |
| Duração oferecida |3 oportunidades naturais do protegido |
| Zero | Aplicação calculada em0 é ignorada, sem criar/renovar escudo |
| Reaplicação válida | max(pontos restantes,novos pontos) e max(duração restante,3), independentemente; sem soma |
| Mitigação | Reduções aplicáveis antes de consumir escudo; excesso atinge vida |
| Multi-hit | Cada hit consome pontos separadamente; proteção0 remove o efeito |
| Relógio | Descontar1 no encerramento da oportunidade natural do protegido, inclusive impossibilitado de agir |
| Aplicação no próprio turno | Não descontar ao encerrar a oportunidade que concedeu/reaplicou o efeito |
| Extras/reações | Não descontam, mesmo quando a fonte extra pede tick de outros efeitos |
| Morte do protegido/revive | Remover na morte; revive não restaura |
| Morte/HP do aplicador | Não alteram proteção já concedida |
| Mudança de confronto/onda/equipe | Não conservar por padrão; alternar ação entre frentes não é transição |
| Expiração | Remoção simples; sem efeito extra de gameplay |
| Exceções | Outros escudos, bypass e futuras políticas de encontro exigem contrato próprio |

Exemplos: aplicador com1000PV máximos dá10 pontos;2000 dá20;55 dá0;550 dá5.10 pontos/3 turnos restantes + nova aplicação20/3 resulta20/3.20/1 + novo10/3 resulta20/3. Se uma duração maior estiver explicitamente presente no estado validado, a reaplicação não a reduz.20 de dano com redução50% e5 de escudo resulta5 de perda de PV. Escudo não fornece piso de1PV.

## Representação e compatibilidade

Novo EffectKind::StandardShield identifica o contrato fechado. Definition.max_health é o HP máximo disponível hoje; não há atributo de máximo efetivo mutável nem cura implementados por esta entrega. O valor armazenado é independente dessas futuras operações e não há caminho de recálculo automático.

EffectKind::Shield permanece o escudo fixo histórico dos testes/passivas de abertura; StructuralShield permanece camada técnica, sem virar atributo de equipamento clássico. Não reinterpretamos esses casos retroativamente. A ordem técnica existente foi estendida para StandardShield, Shield fixo histórico, StructuralShield. Isso preserva os casos legados; não é contrato de coexistência para categorias prateada/dourada ou equipamentos futuros.

A terceira habilidade ativa do encontro de referência agora usa StandardShield, mantendo alvo próprio, recarga e requisito de HP já cadastrados. As passivas de abertura continuam fixas de10 pontos e estão fora da conversão. Com os valores modestos do elenco atual, A1/A2 fornecem1 ponto e A3 fornece0 (80PV máximos); a ação pode ser válida e consumir a recarga, embora sua aplicação de escudo zero seja ignorada. Ignorar efeito zero não torna gratuita uma habilidade utilizada.

Nada foi adicionado ao clássico em termos de inventário ou habilidade especial. O executor compartilhado aceita uma habilidade StandardShield declarada no perfil clássico, mas seu encontro atual continua sem especiais cadastrados. Bloqueio automático probabilístico não foi implementado.

## Relógio e transições

A flag NaturalOnly genérica não bastava: uma fonte de extra pode solicitar avanço de efeitos. StandardShield ignora esse pedido e avança somente em oportunidade natural. applied_turn evita perda imediata em autoaplicação/reaplicação no próprio turno.

Stage filtra apenas StandardShield na passagem mesmo sob RuntimeCarry::Preserve; outros estados continuam obedecendo à política existente. SEQ já elimina estados temporários nas trocas de equipe; testes cobrem suas três políticas, inclusive perfil classificado. PAR-02 mantém um único Battle vivo: alternância de ações entre frentes não remove escudo. Reset de sessão reinicia a batalha sem conservar o escudo obtido durante a tentativa.

Não foi acrescentada uma opção de conservação desse novo escudo entre confrontos: futura exceção precisará de configuração e testes próprios. Não se alteraram os contratos gerais de persistência para os demais efeitos.

## Limites numéricos

Divisão inteira de HP máximo positivo por100 calcula o resultado final para baixo sem multiplicação intermediária ou overflow. Não foi inventado um cap adicional de gameplay; a força deriva do HP máximo válido do núcleo, representado por int. INT_MAX é coberto no teste. A faixa de UI limitada à área de vida preenchida não limita os pontos mecânicos. Arredondamento de fórmulas futuras, especialmente valores negativos, será definido em seus contratos.

## Interface e terminologia

Console e painel visual mostram pontos reais do novo efeito. Consulta de habilidade explica1% do máximo do aplicador,3 turnos naturais e conservação dos maiores valores. Textos de impedimento de ação usam “impossibilitado de agir”/“unable to act”; o identificador técnico Block foi preservado para não misturar uma renomeação com a futura defesa probabilística.

A faixa continua azul-clara sobre a parte preenchida da vida; ícone/painel representam o estado ativo. Aplicação zero não cria status para exibir. Não se importaram ícones ou conteúdos do jogo de referência.

## Verificação e evidências

- [14 grupos do escudo padrão](../relatorios/atuais/ESC-01-PADRAO.json): cálculo/limites, independência de HP atual, autoaplicação/expiração, turno do protegido, maiores propriedades, zero/alvo morto, extras com tick e impossibilidade de agir, mitigação/multi-hit, morte/revive/aplicador, ondas Preserve/Reset, apresentação/detalhes, configuração/replay, três políticas SEQ e alternância/reset PAR-02.
- [Regressão geral](../relatorios/atuais/COMBATE.json), adaptador6, apresentação6 e clássico6 são medidas separadas. A geometria anterior6 não foi repetida por não ter mudado.
- [Compilação Unreal](../relatorios/atuais/ESC-01-BUILD.json): Editor/Win64/Development,19 ações, saída0,39,92s. Sem editor gráfico/aplicativo aberto ou pacote novo.
- Primeira compilação nativa parou por nome de teste ambíguo; depois um cenário SEQ foi corrigido para declarar as fases exigidas por PreserveClassifiedStates. Nenhum contrato de produção foi relaxado para acomodar o teste. Logs em build ignorado.

## O que ainda falta

Aceite visual autorizado da revisão atual, formações1/5 e resoluções; auditoria externa pontual das fontes se necessária; bypass/categorias futuras, silêncio/supressão, equipamentos/atributos clássicos e novas políticas de persistência. Cura, HP máximo mutável e atributos/equipamentos não foram criados para simular condições inexistentes. Em suas futuras implementações, incluir teste de preservação do valor fixado do escudo.

As decisões ESC são contratos fechados para este caso comum; não precisam ser respondidas novamente. A lista única de extensões e aceites é [PENDENCIAS.md](PENDENCIAS.md).

A regressão textual também exigiu atualizar duas expectativas antigas: rótulo “bloqueio” passou a “impossibilitado de agir”; o teste de B3 agora confirma que1% de80 resulta0 e não restaura o escudo de abertura consumido. Essas alterações verificam o novo contrato, sem remover testes.
