# VIS-01 — plano da primeira arena funcional

19/11/2026. **Plano preparado; arena ainda não implementada, compilada ou testada.** Base: ENG-01-PACOTE-RUNTIME.md, núcleo C++20 e interfaces atuais. Esta entrega usa somente leitura/preparação documental pelo PowerShell; não abre editor/aplicativo e não inicia build.

## Resultado esperado

Uma batalha do perfil Equipes, com A1/A2/A3 contra B1/B2/B3, representada por formas simples em posições fixas, câmera fixa e painel de habilidades. Time A manual; time B segue a escolha automática existente. Exibir ator atual, HP/escudos, oportunidade/relógios com nomes corretos, últimas ações, motivo de indisponibilidade, resultado e reinício. Sem modelos/áudio externos, menus de progressão, saves ou novas regras.

O primeiro cenário de três contra três é um recorte de apresentação, não limite do núcleo. Usar IDs estáveis e uma lista de participantes sem assumir exatamente três em toda a arquitetura; a apresentação de1–5 por lado será verificada no aceite visual. SEQ/PAR/chefes compartilhados ficam para integrações próprias, sem presumir que este teste os cobre.

## Arquitetura proposta

| Parte | Responsabilidade | Limite |
|---|---|---|
| Cenário de referência compartilhado | Extrair team_reference_setup/text_reference_rules e dependências necessárias da apresentação textual para fonte neutra | Preservar dados/ordem/resultado; console e arena usam o mesmo cenário |
| Adaptador de sessão visual | Possuir uma rpg::LocalSession e oferecer comandos e visão para UI | Único dono mutável; não calcular dano/AP/recarga na UI |
| Apresentação da cena | Posições, formas provisórias, câmera e indicação de ator/alvo | HP, vida/morte e escudo vêm do estado do núcleo |
| Interface de batalha | Ler abilities/snapshot/progress; selecionar posição/alvo e enviar comando | Widgets não alteram HP nem persistência |
| Automação inimiga | choose_automatic_action seguido de LocalSession::apply | Uma ação por etapa de apresentação; sem laço bloqueante ilimitado |

O experimento de pacote existente permanece como evidência histórica; não editar cópias geradas. Implementação futura altera fontes versionados e prepara novo stage/pacote. Avaliar widget C++ com UMG/Slate/SlateCore fornecidos pela engine; nenhuma instalação/plugin externo está previsto. Dependências ainda precisam ser declaradas/compiladas no bloco de implementação. Evitar arquivo binário de mapa próprio no primeiro incremento: avaliar criação dos elementos por código sobre o mapa técnico existente. Isso não é decisão final de arte ou arquitetura de produção.

## Contrato de interação

- Perfil Equipes: até três posições; básica na posição1, sem defesa genérica, itens ou passar por escolha livre. Perfil clássico preservado no console, sem misturar seus comandos nesta tela.
- Selecionar habilidade ativa e alvo por clique. Atalhos1/2/3 podem selecionar posições, mas não executam contra um alvo presumido. Ação própria pode escolher o autor como alvo conforme TargetRule::Self, com indicação visível.
- Passiva pura aparece como informação não selecionável. Híbrida mantém componente ativo e informação passiva conforme o catálogo existente.
- Recarga aparece como contador; condição não atendida como bloqueio com explicação. Usar AbilityView/ActionError; não exibir silêncio como implementado, pois o núcleo ainda não oferece essa família.
- Alvos candidatos podem ser filtrados por lado/vida/TargetRule para orientar a seleção; o núcleo é autoridade final. Recusa mostra motivo e não avança combate. Não antecipar efeitos ao clique.
- Cada comando recebe identidade do ator/oportunidade observados. Descartar seleção antiga após avanço, morte ou reinício. Não deixar clique duplo pendente agir como comando do próximo ator.
- Durante ação automática ou conclusão, entrada manual bloqueada. O adaptador aplica a ação e só depois atualiza a cena; sem cronômetro próprio que altere AP ou recargas. Pausas visuais não são tempo de combate.
- Se a automação não produzir ação válida, apresentar falha técnica e parar a apresentação, sem inventar vitória. Definir limite de segurança por cadeia, preservando separação de limites do núcleo.
- Reinício reutiliza LocalSession::reset; limpar seleção/eventos visuais antigos e iniciar nova tentativa conforme contrato existente. Sem salvar/carregar; saldo em memória somente se exibido e explicitamente rotulado provisório.
- Rótulos distinguem rodada, oportunidade/turno da unidade e ações aceitas; não chamar todos de turno global. Não prometer barra de AP animada/contínua nesta primeira tela.

## Entregas em sequência

1. **VIS-01A — adaptador e dados compartilhados:** extrair cenário sem mudança de regras, expor estado/ações ao visual e testar recusa atômica, seleção antiga, passagem para automação, fim e reinício. Nenhuma UI declarada funcional só por compilar.
2. **VIS-01B — primeira cena interativa:** câmera/formas e painel com habilidades/alvos; inspeção no editor em inglês e testes de seleção/legibilidade. Somente após autorização de execução específica.
3. **VIS-01C — aceite fora do editor:** pacote próprio atualizado e roteiro visual, incluindo fim e reinício. Não repetir build/abertura sem mudança ou falha pertinente.

Cada entrega possui evidência própria e atualiza o painel. Não executar todos os incrementos como uma tarefa única por inferência deste plano. Primeiro incremento executável será VIS-01A.

## Matriz de aceite planejada

| Caso | Evidência esperada | Forma de verificação |
|---|---|---|
| Cenário compartilhado | Console e adaptador recebem mesmo elenco/regras/kits | Comparação de dados e regressão pertinente após extração |
| Ação aceita | HP/escudo/ator iguais ao LocalSession do mesmo cenário | Teste do adaptador sem duplicar fórmula |
| Recusa/condição/alvo inválido | Estado inalterado, motivo disponível | Teste automatizado do contrato |
| Passiva/recarga | Informação e contador fiéis a AbilityView | Automatizado e inspeção visual |
| Comando antigo/clique duplo | Não afeta oportunidade posterior | Teste de identidade do comando e teste visual |
| Inimigo/controle bloqueado | Não permite ação manual no lado B; automação sem travar interface | Teste e observação |
| Resultado/reinício | Resultado do núcleo, bloqueio após fim, nova tentativa limpa | Teste e observação |
| Formação1/3/5 | Sem IDs/vidas sobrepostos e seleção correta | Dados artificiais + inspeção, sem ampliar catálogo oficial |
| Pacote | Cena, habilidades e fluxo funcionam fora do editor | Execução autorizada e logs/saída |

Estes são testes planejados, não resultados. Os213 testes anteriores permanecem aprovados no escopo registrado; serão repetidos quando a extração/integração afetar dependências pertinentes. Os11 checkpoints da ponte não substituem a matriz visual.

## Idioma, ferramentas e limites

Ambiente Unreal prioritariamente inglês (-culture=en por processo). Texto da apresentação deve usar uma camada traduzível, preservando IDs técnicos; português do jogo continua separado do idioma do editor e ainda exige estratégia/validação. Não condicionar regras à tradução.

Preparação de fontes/documentos: PowerShell. Compilação posterior: dotnet/UAT/UBT/MSVC/UHT/shader workers existentes; testes próprios e engine conforme necessidade. Inspeção visual: Unreal Editor ou aplicativo gerado, com autorização específica antes da abertura. Git/commit somente no escopo autorizado; pushes ficam com o usuário. Sem downloads, ferramentas externas ou alterações de rede. Arquivos/caches normais e diretórios de teste devem ser explicitados em futuras execuções.

VIS-01 não valida servidor, rede offline garantida, desempenho de produção, acessibilidade completa, toque/mobile, loja, personagem final ou todos os modos PvE. Guardar pendências de animação/compressão ACL, Windows limpo e localização em suas áreas. Não reabrir contratos de batalha encerrados sem mudança/falha pertinente.
