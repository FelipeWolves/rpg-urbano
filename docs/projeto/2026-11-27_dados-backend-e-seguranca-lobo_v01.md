# Dados, backend, segurança e cupons — proposta

## Catálogos e persistência

| Contrato | Identidade e dados mínimos |
|---|---|
| Moeda e carteira | currencyId, unidade inteira, escopo, exibição e limites; registro de movimentos por conta privada, origem e chave de idempotência |
| Rotas e drops | routeId, modalId, modo, encontro, requisitos, custo, rewardTableId e modificadores com janela de vigência; tabela de entradas itemId/peso/quantidade/faixa e revisão publicada |
| História e marco | BookId, StoryId, parte, NarrativeLine e NodeId conforme contrato existente. Declarar conjunto de histórias/nós exigidos por revisão e política de equivalência; conclusão de livro compara registros elegíveis com esse conjunto, incluindo secundários quando cabíveis |
| Malha remota | catálogo versionado com IDs de linha, estação, aresta e destino, dependências e versão mínima; atualização autenticada e compatível, cache anterior e rotas locais como fallback |

O saldo técnico de 10 créditos permanece isolado. Ouro, Gemas e Notas Musicais mantêm os usos da visão v04. Fichas Clássicas, Pontos PvP e o sentido de “Notas Musicais/Dados” exigem decisão de classificação. Drops, experiência, custos, câmbio e slots dependem de valores e regras aprovados. A UI deriva estados/ícones dos catálogos, e as recompensas persistentes têm registro de concessão separado. Catálogo remoto adiciona conteúdo dentro das capacidades do cliente; novas mecânicas podem exigir novo binário.

## Infraestrutura por fase

Proposta local, quando autorizada: Docker Compose em loopback, API pequena, PostgreSQL e cache em memória opcional, dados sintéticos e adaptador IBackendService. Colocar regras persistentes e emissão de benefícios atrás da API. Próximo recorte: comparar serviço gerenciado e VPS pequeno por região, quotas, TLS, segredos, limites, backup/restauração e custo real. Supabase, Firebase, PlayFab, Nakama e VPS são candidatos da especificação. Proxies e túneis são opções de implantação. Nenhum provedor, tier, túnel ou serviço foi selecionado. Pós-lançamento, medir antes de adotar workers sem estado, balanceador, réplicas, cache distribuído e CDN.

## Contas e privacidade

Preparar inventário e classificação de dados, finalidades, prazos e matriz por território (Brasil, UE, Reino Unido, Califórnia/EUA, Coreia do Sul e Japão). Antes do cadastro, prever triagem etária conforme território e público-alvo, restrição de comunicação e perfil para menores e mecanismo verificável de autorização parental onde aplicável. Definir política de acesso quando idade ou território forem indeterminados. Consentimento granular e revogável, com SDKs opcionais desligados até o aceite aplicável. Termos e política ficam acessíveis no app. Separar nick público e ID privado, aplicar TLS, proteção em repouso, MFA, segregação de segredos e auditoria de consentimentos. Diagnóstico de hardware local.

Consulta/exportação e exclusão devem ser acessíveis em Configurações. Após verificação de identidade, revogar sessões/credenciais, bloquear uso, executar exclusão nos sistemas vinculados e tratar backups conforme prazo aprovado e restauração com reaplicação de exclusões. Definir retenções obrigatórias, transferências internacionais, evidências e contestação antes de produção. Reservar caminho externo compatível com a loja. Revisão jurídica por território ainda necessária. [GDPR](https://eur-lex.europa.eu/eli/reg/2016/679/oj) · [Política de exclusão do Google Play](https://support.google.com/googleplay/android-developer/answer/13327111?hl=en).

## Cupons externos

Proposta futura. O servidor confirma conta verificada, integridade da progressão e conclusão de 100% conforme a revisão do marco. Emitir código aleatório com gerador criptográfico no servidor, chave de idempotência e restrição única por ID privado e livro. Validar concorrência, repetição e resgate; separar fases de solicitação, emissão, expiração e falha. Tokens e identificadores privados ficam fora da UI pública e dos logs. A conversão para Amazon e Mercado Livre depende de parceria, API, termos, mercados, códigos válidos e política de atendimento. Testar primeiro com emissor fictício. A remoção de conta e os direitos sobre BETA_SUPPORTER/cupom exigem definição de retenção e restauração; reset de balanceamento não apaga a marca Beta.
