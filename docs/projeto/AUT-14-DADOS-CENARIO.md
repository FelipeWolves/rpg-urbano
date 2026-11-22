# AUT-14 — esquema versionado do cenário

22/11/2026. Preparação prevista neste bloco: contrato nativo e dois manifestos de referência, sem conversão global da arquitetura nem carregamento novo no jogo.

`encounter_manifest.hpp` oferece estrutura tipada, encode/decode, validação com mensagens e composição do cenário canônico após verificação. Equipes/Clássico têm IDs próprios e schema 1. Dados limitados e parse inteiro com from_chars; sem biblioteca ou serviço adicional.

Sete grupos aprovados. Manifestos reais lidos dos arquivos e comparação do EncounterSetup recomposto com o baseline completo; malformed/truncado/extra/contagens/versões/IDs/atributos/bindings divergentes são recusados. Nada substitui a sessão em execução. Ver data/encounters/README.md para matriz de diferenças.

Superfície explícita: roster/atributos, IDs/owners de skills e referências por slot/ativa/passiva. Semântica, fórmulas, cooldowns, descrições/níveis, gatilhos/payload/condições, tempo e recompensas continuam nas fábricas em código. Não afirmar que esses dados já foram migrados, protegidos por hash ou editáveis por JSON. Não habilita fichas externas recebidas automaticamente.

Pendências: ampliar schema somente para regras fechadas, validar campos novos, formalizar migração e paridade antes de carregar dados de produto; localização/editor de dados e schema JSON são posteriores. Sem teste de engine/Android/internet necessário para este contrato nativo; não foi executada compilação Unreal adicional por um header ainda sem consumidor runtime.

Próximo AUT-15: manifesto e preparo de pacote reproduzível, sem executar editor/cook/jogo sob as restrições atuais.
