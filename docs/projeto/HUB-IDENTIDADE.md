# Hub — fontes e logo integradas

19/11/2026. Continuação de HUB-01C-VISUAL.md. Cormorant Garamond aplicada ao título; Lora aos textos e etiquetas do hub. Navegação auxiliar ainda usa fonte padrão. LOBO permanece codinome. Não altera regras do combate.

## Recursos e reprodução

Fontes variáveis TTF e licenças OFL1.1 obtidas do repositório oficial Google Fonts: https://github.com/google/fonts/tree/main/ofl/cormorantgaramond e https://github.com/google/fonts/tree/main/ofl/lora. Não instaladas no Windows. Arquivos locais em assets/runtime/fonts; fontes carregadas por UFont/FCompositeFont com LazyLoad e referência mantida por UPROPERTY.

PNG da logo obtido da URL indicada pelo usuário no briefing e conferido visualmente: silhueta branca do lobo,132×132,3019 bytes. WebP original preservado em assets/sources; PNG de runtime em assets/runtime/hub/logo-lobo.png. Não houve edição/conversão ou reconstrução por IA. O fundo escuro faz parte da referência recebida; não declarar transparência removida ou vetor. Direitos de distribuição da logo não foram auditados independentemente; antes de publicação, confirmar autorização ou substituir por arte própria.

data/presentation/hub-resources.json registra URLs, tamanhos, SHA-256 e licenças. scripts/validar-identidade-hub.ps1 confere presença, hashes, assinatura TrueType e licenças localmente, sem rede. Preparação copia fontes, licenças e logo para Content/Presentation:44 entradas ao todo, com hashes. Build nunca baixa recursos automaticamente.

Mídias e fontes continuam ignoradas no Git. Em outro computador, restaurar os cinco arquivos do manifesto nas mesmas pastas e executar o validador; conservar as licenças junto das fontes. Commit de código/documentação não é backup desses recursos locais. URLs apontam para versões externas que podem mudar; comparar os hashes antes de aceitar substituições.

## Verificação e correções

O primeiro build passou, mas a abertura falhou por uso de Inline para fonte externa. A API da engine reserva Inline para Font Face asset; corrigido para LazyLoad. Registro inicial preservado em build/p/8e384457/hub-identity-runtime.log. Nova abertura carregou fontes e logo; não mascarar falha inicial como aprovação.

Em1280×720 foram observados título/codinome/logo, evento com acentos, Batalha, alternância para modo estático e fluxo hub→evento→hub→arena→hub. Não houve ação de combate nesta verificação. A nova tipografia revelou espaçamento insuficiente; etiquetas passaram a medir altura real e o texto do painel usa entrelinha medida; Lora ajustada para14pt, título18pt.

Build e relatório finais em docs/relatorios/atuais/HUB-IDENTIDADE-BUILD.json; resumo de execução em HUB-IDENTIDADE-RUNTIME.json. Capturas permanecem na conversa; logs detalhados em build, ignorados no Git. Não há novo pacote/cook nem teste mobile nesta entrega. Não se afirma aceite integral VIS-01C, todos os efeitos, DPI ou todas as resoluções.

## Restante organizado

- Expandir aceite para outras proporções/DPI e formação1/5; texto integral/rolagem em janela pequena, foco de teclado, toque e áreas seguras.
- Ler preferência de movimento reduzido do sistema e salvar opção de jogo; atual opção é local, via catálogo/botão/-ReducedMotion.
- Player, preferências antes da janela, orientação mobile e restauração continuam pendentes.
- Logo final em resolução maior/vetor, eventual autorização de distribuição e validação das fontes/acentos no pacote e aparelhos.

Próxima ordem e entregas independentes: [PROXIMAS-FASES.md](PROXIMAS-FASES.md). Percentuais revisados: [PROGRESSO-GERAL.md](PROGRESSO-GERAL.md). Não executar todos esses blocos por inferência desta documentação.

## Aceite do usuário e fechamento

O usuário confirmou na compilação final que o texto não corta mais, a seleção responde e a batalha funciona conforme o combinado. A última inspeção do assistente foi interrompida pelo Escape físico; o aceite do texto é relato explícito do usuário, não inspeção final automatizada.

Pendências: seletor aparece sobre a esfera e deve ficar no chão sob a unidade; apresentação esperada no modo mobile não carregou (causa não identificada, janela vertical não troca plataforma); fluidez de batalha precisa de exemplos/critério antes de alterar tempos ou regras. Próxima entrega pequena: corrigir apenas a posição do seletor e validar visualmente. Mobile permanece investigação separada.

Abrir-Preview.cmd inicia a última compilação aprovada indicada por HUB-IDENTIDADE-BUILD.json; Abrir-Editor.cmd abre o mesmo projeto no editor sem iniciar Play automaticamente. Ambos validam caminhos, não compilam/instalam nada e respeitam a política de scripts. Se o Windows bloquear scripts, não contornar a política. O editor pode gerar caches normais. O projeto em build/p é cópia gerada: alterações nele não voltam ao código canônico; cenas/assets que quisermos preservar precisam de transferência revisada para a estrutura canônica antes de nova preparação. Não substituir a origem por arquivos gerados implicitamente.

Build corrente da prévia agora em PREVIEW-BUILD.json (fallback do lançador: HUB-IDENTIDADE-BUILD.json). A correção posterior do seletor está em VIS-SELETOR-CHAO.md; a evidência histórica desta integração foi preservada.
