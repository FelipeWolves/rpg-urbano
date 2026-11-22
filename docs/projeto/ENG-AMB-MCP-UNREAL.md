# ENG/AMB — integração MCP com Unreal

22/11/2026. Resposta ao alinhamento do usuário; não é instalação nem ativação.

## Estado confirmado

O usuário informou que ainda não instalou plugin/servidorMCP. O catálogo de ferramentas desta sessão não expôs operações daUnreal. Portanto não há integraçãoMCP daUnreal confirmada/disponível aqui. Esta conferência usou o contexto da sessão; não consultou internet nem alterou configurações doCodex/editor.

MCP significa Model Context Protocol: uma forma de um servidor expor ferramentas/contexto ao assistente. Não é uma engine, linguagem nova nem substituto do nossoC++ ou dos recursosUE.

## O que muda e o que fica

Hoje o fluxo confirmado é editar fontes/dados do projeto, testar regras localmente, compilar a ponteUBT e preparar um stage verificável, sem controle de tela. Uma integraçãoMCP poderia oferecer operações do editor se o plugin específico as implementar. Não é correto prometer editar cenas, importar assets, modificarWidgets, executarPython ou compilarBlueprint sem conferir essas ferramentas.

| Possibilidade futura | Condição antes de afirmar suporte |
|---|---|
| Consultar mapas/atores/assets | Servidor precisa expor operações de leitura compatíveis |
| Criar/editar cena ou propriedades | Operações de escrita devem existir e ser limitadas ao projeto |
| Importar materiais/modelos/animação | Suporte concreto do plugin e cadeia de importação |
| Widgets/Blueprints | Métodos próprios; não decorrem automaticamente do protocolo |
| Funcionar sem tela | Depende da implementação. Pode exigir editor em execução; janela minimizada não é processo headless |
| Testar aparência/fluidez | Continua exigindo observação/ensaio apropriado; chamada bem-sucedida não equivale a aceite visual |

O core permanece eminclude/src e é a autoridade de combate. MCP é ferramenta de desenvolvimento, não dependência que o jogador precise instalar e não nova família de gameplay. Classificação: ENG (adaptador/fluxo daengine) eAMB (configuração/manutenção). Não refazer VIS/SIS/HAB por adotar uma ferramenta.

## Próxima decisão, quando priorizada

Identificar candidato e fonte/licença, conferir compatibilidade com a engine instalada, ferramentas realmente expostas, necessidade de processoEditor e alcance das escritas. Definir conexão local/permissões/logs e um teste pequeno no projeto antes de produção. Sem candidato não há promessa de compatibilidade, segurança ou economia de uso.

Nenhum plugin foi pesquisado online, instalado, habilitado ou conectado nesta rodada. A autorização atual de editar/compilar o projeto sem tela não foi convertida em ativação automática de um servidor.

## Sequência atual

Primeiro atualizar entregas; depois VIS (entrada/alvos), ENG (vídeo/foco/backend) eHUB (rotas/retorno/interações). Atualizar o binário de teste por compilação é compatível com o fluxo atual. Alterações de assets/cenas por editor/headless/MCP devem ter escopo concreto, sem usar a tela por inferência.

O agente não conhece o saldo diário; não pode garantir que haverá limite para um lote amplo de edição. Melhorar a automação reduz trabalho repetitivo, mas não garante menor consumo nem substitui validação.
