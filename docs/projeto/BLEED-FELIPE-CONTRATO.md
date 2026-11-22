# Sangramento / Bleed — contrato recebido em 22/11/2026

Decisão direta do usuário durante SIS/REG/HAB. Ainda não implementado por este registro.

- Tick no início da oportunidade natural ou extra do portador, antes de escolher/executar sua ação, inclusive impossibilitado de agir.
- Logo após o tick, duração diminui 1 e efeito é removido em 0. Reação/assistência/perseguição fora de turno não dispara tick nem decremento.
- Pontos por tick: floor(ATK do aplicador na aplicação ×5000/10000), mínimo 1 em aplicação válida. Snapshot não recalcula se ATK mudar depois.
- A1 do Felipe aplica por 2 turnos/ticks.
- Debuff comum sem borda, purificável/transferível e removido entre ondas. Fonte/categoria/persistência não substituem políticas SEQ/PAR já encerradas.
- É DoT de **True Damage**: ignora DEF, mitigação percentual, Defesa clássica e Block; respeita escudos e invencibilidade. Não generalizar esse tratamento a todos os DoTs.
- Reaplicação ficou apresentada em alternativas (regra da mesma fonte ou instâncias independentes até slots SIS-01); solicitada escolha concreta para mesma fonte e número de slots do piloto. Fontes diferentes independentes constam no material anterior. Enquanto sem resposta, não escolher política/número implicitamente.

Este contrato passa a preencher a pendência da fórmula e do relógio da A1; não é declaração de implementação/teste. Próximas respostas de empilhamento/slots deverão ser incorporadas sem apagar este registro.

## Fechamento de instancias — resposta posterior de 22/11

Cada reaplicacao cria instancia independente, inclusive da mesma fonte. Piloto: ate 3 slots por debuff (nao 3 debuffs totais). No limite, nao substituir instancias existentes para introduzir outra. Direcao final: Sangramento/Veneno sem teto de gameplay de quantidade, podendo receber restricoes por situacao; demais debuffs ocupam um slot renovado. Limite tecnico de recursos e formula de Veneno sao contratos separados. A pergunta anterior de empilhamento/numero foi respondida.

Implementação 22/11: executor e dez grupos focados aprovados; ver HAB-PERIODICOS.md. Os parágrafos anteriores sobre ausência de implementação são histórico.
