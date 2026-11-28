# Parecer técnico sobre a especificação consolidada — 27/11/2026

Referência de código v0.28-pre-alpha. Este parecer registra decisões de planejamento e propostas técnicas para validação. A fila de execução continua sendo o ponto de partida para código de produção.

## 1. Auditoria e fronteira do TerminalCore

A âncora do Terminal no catálogo é (0,56; 0,40). Em ProbeHub.cpp, DrawHub hoje mistura desenho Canvas, tratamento do ponteiro, paginação, escolha do encontro, transição e reinicialização de sessões. A referência recebida conta três páginas de laboratórios. O código tem essas três mais uma quarta página dedicada ao tutorial padrão. ProbeHub3D.cpp aproxima a câmera do Terminal e compartilha o menu local. O piloto Felipe na Arena continua local. Em src/local_session.cpp, 10 créditos técnicos são concedidos à equipe A em vitória por eliminação. Os créditos ficam fora da futura economia comercial.

Proposta incremental: extrair primeiro o catálogo e os estados das rotas para C++ puro, com IDs estáveis. O TerminalCore recebe um snapshot local, resolve visibilidade, requisitos, custo, multiplicador e motivo de indisponibilidade e retorna uma intenção de embarque. O adaptador executa a transição pelas sessões existentes. Os dois visuais consultam a mesma projeção de dados. A UI não aplica recompensas nem inicia o combate diretamente. Preservar os caminhos atuais, saves isolados e retorno ao Hub enquanto se testa paridade por rota, inclusive repetição de clique, opção bloqueada e tutorial.

## 2. Terminal 2D e câmera 3D

O design de destino usa um cartão-bilhete reutilizável em lista rolável. Manter barra compacta de moedas/menu, cabeçalho TERMINAL URBANO, cartões com modal, texto localizado, uma faixa contínua de micro-ícones de recompensa e ação; rodapé com retrato ativo e Rotas, Trilhas e Expedição. Na Unreal, avaliar ScrollBox ou ListView, Box horizontal e pincel 9-Slice. Os termos Prefab e Layout Group descrevem o comportamento visual; a migração de Canvas para UMG exige um piloto próprio. Preservar tap único, cursor livre, áreas seguras e Movimento Reduzido. Acabamento final depende da arte de fundo, cinco ilustrações, moldura tintável e retrato/ícones sem texto embutido. A barra lê dados do jogador quando o contrato de economia existir.

Sinalização 2D/3D pelo mesmo token por modal: monotrilho roxo para PvE especial; trem regional azul-cobalto para História; ônibus verde-esmeralda para chefes de equipamentos; metrô laranja para recursos, farm e Torre; loja vermelha com espaço reservado para utilidades. Badges e ícones leem os dados publicados. Preservar rótulos e formas para que a navegação não dependa da cor.

Recomendação: câmeras cinematográficas por setor como primeira opção, pois reutilizam o foco atual, têm percurso previsível e exigem menos controles, colisões e animações. Movimento livre pode ser avaliado após o mapa editável. Desktop 3D com atalho 2D (tecla a validar) e mobile 2D padrão são objetivos do Terminal de produção. No protótipo, manter os pontos de entrada e o seletor existentes. O 2D rápido deve preservar o setor/rota, funcionar sem movimento e devolver o foco ao ponto anterior ao fechar.

## 3–6. Contratos especializados

- [Pacotes e diagnóstico local](2026-11-27_terminal-pacotes-e-hardware_v01.md)
- [Economia, narrativa e segurança](2026-11-27_dados-backend-e-seguranca-lobo_v01.md)

## 7. Ordem e documentação até a Stable

1. Pré-Alfa imediata: apresentação AUT-13, aceites em janela e materiais/movimento provisório do Hub 3D. Receber a arte final do Terminal em paralelo. Teste físico Android cobre persistência, áudio, toque e desempenho.
2. Corte técnico futuro do Terminal: contrato TerminalCore e paridade de rotas; depois piloto 2D com dados simulados e, com os materiais e o formato de cena aprovados, adaptação 3D e atalho. Fracionar em três documentos (núcleo, UI mobile e cena desktop) quando entrar em execução. Validar juntas as duas camadas visuais e verificar acessibilidade.
3. Alfa Plena: integrar amostra piloto 3D/rig/animação e áudio; validar esquema de save. Programar prova local de pacotes e diagnóstico, piloto narrativo, economia e infraestrutura local conforme ordem aprovada. Documentos de pacotes e dados/segurança serão desdobrados em contratos quando houver decisões sobre formatos e serviços.
4. Beta: escopo integrado, compatibilidade de saves, conta e elegibilidade Patreon, segurança e privacidade verificadas. Validar publicação, acesso e recompensa persistente na transição.
5. Stable: concluir escopo, auditar a operação, validar recuperação e resgate. Malha remota ampliada, clima e horário, cooperação, rankings, coleção musical e telemetria entram na visão pós-lançamento.

Candidatos de nomenclatura, sujeitos a escolha por sistema: Compasso, Andamento, Ressonância, Acorde, Selo, Rotação, Sulco; Eufonia, Prosódia, Verbo, Métrica, Estrofe, Fólio; Antologia, Cânone, Compêndio, Códice, Coletânea, Álbum, Itinerário e Linha. Nenhum candidato renomeia efeitos ou contratos existentes nesta revisão.

[Especificação recebida](../../entregas/00-entrada-geral/recebidos/2026-11-27_especificacao-consolidada-lobo_v01.md).
