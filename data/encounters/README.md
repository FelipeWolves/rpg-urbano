# Manifestos de referência — schema 1

Formato textual UTF-8/ASCII `.rpgmanifest`, limitado a 64 KiB. Não contém scripts, caminhos de recursos, contas ou credenciais. Ordem: RPG_ENCOUNTER 1, scenario, profile, roster, skills, bindings, end. Contagens de cada seção limitadas a 64; números inteiros sem sinal e sem overflow silencioso. IDs conhecidos e ordem são comparados com o catálogo canônico; conteúdo extra é recusado.

Os dois arquivos são fotografias de **roster/atributos e referências de habilidades/slots/passivas**. Não permitem balanceamento livre: qualquer diferença é recusada. `compose_manifest` valida e usa a fábrica canônica, preservando todas as regras atuais. Não são configuração já carregada pelo HUD.

| Superfície | Schema 1 | Antes de migrar runtime |
|---|---|---|
| ID do cenário/perfil/versão | Explícitos | Versionar ao mudar contrato |
| Unidades: ID/lado/HP/ATK/SPD | Explícitos e comparados | Estatísticas amplas/ponto fixo são outro bloco |
| Habilidades: owner/ID | Referências comparadas | Fórmulas, alvos, hits e efeitos ainda em código |
| Slots: owner/posição/ativa/passiva | Referências comparadas | Nomes/localização, descrição/nível não migrados |
| Recarga, gatilhos, condições e payload de passiva | Permanecem em código | Schema específico, limites e regressões antes de editar por arquivo |
| Regras de tempo, recompensa e encontro | Permanecem em código | Nenhuma migração por inferência |

Validação: `scripts/testar-autonomo.ps1 -Block 14`. Sete grupos incluindo os dois arquivos reais, roundtrip, igualdade do EncounterSetup inteiro recomposto, limites, IDs, atributos e referências inválidas. Alteração dos arquivos não muda o jogo atual. Scripts externos não são executados pelo codec.
