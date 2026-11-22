# SIS/PvE C2 — política WaveV2

22/11/2026. RuntimeCarry::WaveV2 exige config_version>=2 e PreserveHealth. As opções Preserve/Reset e SEQ/PAR não são alteradas. Primeiro remover StandardShield e buffs/debuffs comuns dos remanescentes, depois curar aliados vivos30%MaxHP, com floor/cap e sem mínimo artificial/ressurreição. Silver/Golden/Estrutural e estados nativos protegidos são conservados; durações não se renovam. Marcadores neutros não viram buff/debuff por inferência.

Recargas e cargas da A2 são carregadas explicitamente por InitialRuntime; AP/guard/relógios da nova onda reiniciam conforme Stage existente. Evento de transição registra limpeza antes da cura. GeradorSilver não muda StandardShield. Validação global de orçamento foi separada da validação de kits para não invalidar personagens com passiva por testar uma cópia sem suas habilidades.

Sete grupos focados aprovados: limpeza/classificação, Silver/Golden, cargas, recargas, AP, cura/semrevive e versão/política anterior. Não houve UI, engine em execução, Android ou internet. A campanha continua limitada a5ondas pelo contrato existente.
