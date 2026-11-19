# VIS-01 — interação e apresentação R01

19/11/2026. Contrato documental recebido do usuário; nenhuma implementação, compilação ou abertura nesta entrega.

## 1. Efeitos e escudos — definido

Substituir o texto Shield no rótulo por ícone próximo à vida. Clique/toque abre painel de efeitos ativos da unidade, com valores, duração/relógio/cargas quando cadastrados; não inventar dados ausentes. Consulta não consome ação ou altera o combate.

Correção do usuário: no exemplo percentual,10% sobre100 de vida atual gera10 de proteção;10% sobre50 gera5. A faixa azul-clara tendendo ao branco cobre a fração protegida da parte preenchida da barra. A barra usa HP máximo como escala de largura, mas isso não define a base de cálculo do efeito. Não confundir escudo fixo em pontos com porcentagem. Momento de aplicação/recalculo e modificadores aguardam o documento do usuário. Outros efeitos terão identidade própria.

Tratamento de escudo acima de100% da vida máxima e agregação de categorias ainda aberto. Proposta: limitar só a faixa visual a100% e mostrar valor integral no painel, sem reduzir proteção real. Não alterar absorção, duração ou causalidade do núcleo.

## 2. Habilidades — definido e proposta de rótulo

Clique curto seleciona habilidade ativa disponível; gesto prolongado abre detalhes sem selecionar/executar. Informar nome completo, funcionamento, recarga/condições e nível quando cadastrado; sem nível1 inventado. Passivas puras, habilidades bloqueadas e em recarga permitem consulta, mas não execução indevida.

AbilitySlot atual não tem nível ou descrição completa estruturada. Serão necessários metadados opcionais e textos traduzíveis, sem converter nota informativa em efeito implementado.

Usuário sugeriu ícones/rótulos como ATK1/PASSIVA/ATK2. Proposta: preservar a posição1/2/3 e combinar com tipo/ícone (1/Básica,2/Passiva ou Técnica,3/Especial), evitando renumerar a terceira porque a segunda é passiva. Convenção final ainda a avaliar.

Definir duração/gesto e comportamento da automação enquanto o painel estiver aberto antes de implementar. Soltar após gesto prolongado não deve selecionar ou executar acidentalmente. Painel identifica unidade/habilidade consultada; comandos antigos não podem alcançar o próximo ator.

## 3. Alvos — esclarecimento para o usuário

Hoje a interface permite apontar cartões incompatíveis e só depois mostra recusa genérica. Exemplo: básica exige inimigo vivo; proteção atual exige o próprio autor. Proposta: realçar candidatos válidos e explicar os demais; manter validação final no núcleo, inclusive para comandos antigos/mortes/reinício.

Não muda quem uma habilidade pode atingir. Apenas comunica as regras existentes. Recusa mantém estado e mostra motivo específico. Usuário pediu esclarecimento; não tratar esta proposta como nova regra de combate já fechada.

## 4. Formações — pendente

Validar visualmente1/3/5 e assimetrias nos dois perfis, com disposição própria conforme quantidade. Até agora3×3 foi observado; testes nativos de coordenadas1–5 não são aceite visual.

Possível sexta posição no padrão foi levantada pelo usuário como extensão. Não alterar limite global implicitamente: seleção/posicionamento1–5 precisam de revisão e testes para suportar6. Não é só criar uma esfera. A decisão pode ficar separada do fechamento visual1–5.

## 5. Resoluções — pendente

Definir matriz desktop e verificar áreas de clique/toque, sobreposição, limites da tela e escalonamento de painéis/retrato/habilidades ao redimensionar. Mobile/retrato terão aceites próprios;1280×720 não comprova responsividade.

## Sequência sugerida

Implementar ícone/painel de efeitos e faixa de escudo; depois detalhes/metadados de habilidade; fechar orientação de alvos após esclarecimento; validar formações/resoluções. Standard/Classic e regras existentes preservados. Novas execuções continuam exigindo autorização específica.


## Implementação inicial

Ver VIS-01-INTERACAO-IMPLEMENTACAO.md. Parte dos recursos foi escrita e compilada; gestos e apresentação ainda sem inspeção visual desta revisão. O cálculo de efeito percentual não foi introduzido por esta camada de UI; escudos fixos existentes continuam em pontos.
