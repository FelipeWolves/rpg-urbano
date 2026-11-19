# HUB — estilo visual por código

19/11/2026. Incremento de apresentação do hub; não altera combate nem encerra o aceite visual da arena VIS-01C.

## Decisões vigentes

- Título: Mundus Lupis Disco. LOBO é codinome.
- Cormorant Garamond principal e Lora complementar; Cinzel excluída. Integração posterior das fontes locais e da logo registrada em [HUB-IDENTIDADE.md](HUB-IDENTIDADE.md); evidências abaixo referem-se ao incremento original.
- Logo recebida e preservada sem conversão em assets/sources/logo-lobo-silhouette.webp: WebP,132×132,1234 bytes. Origem indicada pelo usuário no briefing; recebimento não constitui auditoria independente de direitos. Posteriormente foi obtido PNG no endereço informado pelo usuário e conectado ao renderizador; qualidade ampliada continua limitada pela resolução. O pacote distribuível ainda não foi validado nesta revisão. Binário permanece local, ignorado no Git.
- Batalha: octógono chanfrado original com presas geométricas e legenda BATALHA, sem copiar ícone do jogo de referência.

## Implementação

Tema separado da imagem em data/presentation/startup.json: identidade, paleta, fundo translúcido80%, texto de evento, parâmetros de pulso e opção reducedMotion. ProbeHub.cpp lê esses dados e desenha moldura neon aproximada por linhas, emblema e anel. O efeito luminoso é provisório, sem material de glow dedicado.

Pulso em ciclo2,4s: escala1–1,06; anel âmbar #F59E0B de1,5px, expansão1–1,35 e opacidade0,7–0, com interpolação suave de cosseno. Área de interação estável, ampliada para pelo menos48×48 dentro da imagem visível; acessos fora do recorte são ocultados. Conteúdo menor que48px não comporta o acesso e não gera alvo menor. A legibilidade e o tamanho físico em telas com DPI diferente precisam de aceite real.

Movimento reduzido: opção no catálogo, botão local Com pulso/Sem pulso ou argumento -ReducedMotion. Desliga escala/anel animados e mantém contorno duplo âmbar/carmesim. A preferência não é persistida; leitura automática da acessibilidade do sistema ainda não implementada. prefers-reduced-motion é referência de comportamento, não uma consulta CSS executada pela Unreal.

Texto do evento atualizado, com quebra simples de linhas. Em janelas pequenas pode ser cortado para preservar o botão Voltar; leitura integral/rolagem e foco por teclado permanecem no aceite de interface. Navegação e a mesma sessão de batalha foram preservadas. Toque ainda não integrado.

## Verificações realizadas

- Catálogo e parâmetros visuais aprovados pelo validador local.
-10 grupos anteriores de apresentação e5 de projeção aprovados;5 novos grupos de pulso/movimento reduzido/alvo mínimo aprovados. Não somar estes números aos213 testes gerais de combate, que não foram repetidos.
- Build RpgEngineProbeEditor Win64 Development aprovado:22 ações,46,50s, stage c6d1b900. Preparação com39 entradas. Relatórios HUB-01C.json e HUB-01C-BUILD.json em docs/relatorios/atuais.
- A primeira tentativa teve acesso ao diretório da engine negado pelo ambiente restrito; a compilação autorizada fora dessa restrição passou. Avisos prévios de toolset/C4864 permanecem.
- Nenhum editor, aplicativo, commandlet, player ou pacote executado; sem cook, instalação ou download.

## Próximo aceite

Observar contraste, cortes, sobreposição, hover, animação e retorno à arena em execução autorizada. Fontes e logo conectadas posteriormente; conferir resultado vigente em HUB-IDENTIDADE.md e ampliar aceite para escalas/dispositivos. Player, orientação real mobile e preferências anteriores à janela continuam em entregas próprias.
