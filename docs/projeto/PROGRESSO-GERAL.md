# Progresso geral do projeto

Atualizado em **19/11/2026**. Painel para acompanhar o jogo inteiro, não apenas o combate.

**Estimativa global atual: aproximadamente15%–20%.** Percentuais abaixo são estimativas de maturidade por área, não medição de horas, previsão de prazo ou garantia de entrega. Não somar nem tirar média simples: áreas têm tamanhos diferentes. Revisar quando definirmos quantidade de personagens, fases, modos e qualidade visual da primeira versão.

Referência de chegada: versão jogável com apresentação gráfica final do escopo escolhido, integrada e testada em um celular-alvo. Lançamento comercial em todas as lojas não está incluído como conclusão automática desse marco. O que já foi comprovado continua nos relatórios; porcentagem não substitui aceite.

## Painel por área

| Área | Estimativa | O que já existe | Principal trabalho restante |
|---|---:|---|---|
| Código core — combate e mecânicas | **55%** | Tempo/AP, ações, efeitos mínimos, causalidade, perfis de chefes/equipes e escudo padrão testados | Famílias restantes de efeitos, passivas, alvos/área, atributos, equipamentos e balanceamento |
| Código da engine — integração Unreal | **38%** | Ponte, arena, parser/rotas do hub e integração local de fontes/logo compilados; pacote Windows anterior | Fluxo completo, apresentação de eventos, animação/áudio, carregamento e integração final |
| Front — interface do jogador | **20%** | HUD da arena e hub com tema, emblema, movimento reduzido, fontes e logo conectadas | Arte/layout finais, navegação completa, acessibilidade, toque e diversas resoluções |
| Hub, entrada e navegação entre áreas | **20%** | Panorama/rotas locais, tema, identidade e20 grupos nativos de apresentação/projeção/estilo | Aceite gráfico do hub/arena, player, preferências/rotação reais, avatar e acabamento |
| Cenários3D e iluminação | **5%** | Composição/câmeras e formas provisórias da arena | Direção visual original, modelagem, materiais, luz e otimização dos ambientes |
| Personagens, rig e animações | **0%** | Identificação de atores e posições de apresentação | Modelos próprios/licenciados, esqueleto, poses, ações, expressões e integração |
| Efeitos visuais e cinematics | **0%** | Possíveis usos ainda em definição | Linguagem visual, efeitos, roteiro visual, montagem e integração/validação |
| Áudio e música | **0%** | Identidade musical como direção | Recursos próprios/licenciados, efeitos, trilhas, mixagem e integração |
| Conteúdo, narrativa e experiência | **5%** | Direção geral e exemplos funcionais de modos | Universo original, catálogo autoral, fases, encontros, tutorial e ritmo de progressão |
| Progressão e persistência | **25%** | Seleção, recompensa de laboratório e save v1 entre batalhas | Economia real, desbloqueios, inventário, migração e retomada conforme escopo |
| Serviços online e integrações de lojas | **0%** | Separação de responsabilidades planejada | Só após escopo próprio: contas, backend, PvP real, compras e SDKs; não bloqueia laboratório local |
| Mobile e otimização | **0%** | Objetivo multiplataforma registrado | Compilar/instalar em aparelho, toque, áreas seguras, memória, GPU, temperatura e bateria |
| Qualidade e validação do produto | **22%** | Suítes nativas, ponte, builds e verificações incrementais Windows; mídias com hashes | Matriz completa de modos/UI/assets, regressão integrada, dispositivos, desempenho e distribuição |
| Documentação e organização | **65%** | Contratos, pendências, evidências, manifestos, fases e entregas independentes organizados | Manter sincronizados com escopo/conteúdo novos, decisões e entregas das outras frentes |

**0% significa implementação não iniciada, não ausência de ideias.** Ter testes sólidos do core não significa que metade do jogo completo esteja pronta. Da mesma forma, um cenário renderizado não comprova desempenho de jogo no celular.

## Revisão desta entrega

Todos os percentuais foram revisados. Core55%, cenários5%, personagens0%, efeitos/cinematics0%, áudio0%, conteúdo5%, progressão25%, serviços0% e mobile0% permanecem: esta entrega não implementou esses escopos. Engine35→38%, Front15→20%, Hub5→20%, Qualidade20→22% e Documentação60→65% refletem os artefatos do hub e a organização incremental, sem equivaler a produto final. Estimativa global permanece15%–20%: falta escopo final e validação mobile para sustentar mudança precisa.

[Próximas fases, dependências e entregas independentes](PROXIMAS-FASES.md). [Integração e evidências da identidade](HUB-IDENTIDADE.md).

## Frentes que podem avançar separadamente

| Frente | Pode começar sem terminar todo o combate? | Acordo mínimo para integrar depois |
|---|---|---|
| História, universo e roteiro de abertura | Sim | Tom, duração, personagens presentes e direitos de conteúdo |
| Conceitos de personagem e ambiente | Sim | Identidade original, proporções/estilo, referências e formato de entrega |
| Modelagem de cenário e objetos | Sim | Escala, orçamento visual provisório, pivôs, nomes e organização dos fontes |
| Animação de apresentação | Sim | Modelo/esqueleto e ações desejadas; combate exige pontos de início/impacto/fim |
| Cinema de abertura ou evento | Sim | Roteiro visual, enquadramento, duração, áudio, orientação e possibilidade de pular |
| Música e efeitos sonoros | Sim | Licença/autoria, duração/loop, formato e eventos que os acionam |
| Layout do hub e menus | Sim | Estados/destinos, dados fictícios identificados e adaptação de telas |
| Novas regras de combate | Sim, dentro de seus contratos | Estados/eventos/IDs estáveis; não depender da cena para calcular regras |
| Teste técnico inicial no celular | Em parte, sem arte final | Ferramentas/aparelho/autorizações; começar cedo reduz risco, sem prometer aceite final |

Não existe independência absoluta: as frentes podem produzir entregas isoladas, mas precisam combinar interfaces e reservar integração/validação. Um clique que inicia combate, uma animação que representa um hit e um vídeo que leva ao hub precisam de pontos de conexão definidos.

Core e cenas3D podem ser trabalhos difíceis por razões diferentes. O primeiro exige consistência, testes e controle de combinações; os segundos exigem direção artística, produção de recursos, composição e desempenho. A ordem do projeto não deve obrigar terminar todas as habilidades antes de produzir arte/cinema.

## Abertura ou evento como vídeo local

É possível produzir a cena separadamente, renderizar e guardar o vídeo junto do aplicativo para reprodução local. Ele não precisa calcular combate nem manter toda a cena3D em execução enquanto é exibido.

Ainda precisamos planejar: formato/codec compatível com plataformas-alvo, resolução/orientação, tamanho do pacote, qualidade, legendas, volume, pular/pausar e comportamento ao interromper/retomar o aplicativo. Vídeo não é automaticamente mais leve em armazenamento e não permite a mesma interação de uma cena em tempo real.

Preservar projeto3D, recursos e licença/autoria, além do arquivo de vídeo final. Se desejarmos versões retrato e paisagem, prever enquadramentos próprios em vez de apenas esticar/cortar depois. Não instalar codecs/plugins nem adotar tecnologia antecipadamente por este planejamento.

## Como atualizar este painel

- Revisar ao concluir uma entrega relevante ou receber escopo/conteúdo novo.
- Percentual só sobe quando há artefato implementado/produzido e evidência correspondente; ideias ficam na coluna de planejamento.
- Registrar mudança de escopo que reduza a estimativa; não preservar número artificialmente.
- Usar [ESTADO.md](ESTADO.md) para a situação técnica, [TAREFAS.md](TAREFAS.md) para a próxima entrega e [PENDENCIAS.md](PENDENCIAS.md) para dúvidas.
- Referência nova de hub: [HUB-01-REFERENCIAS.md](HUB-01-REFERENCIAS.md). Escudo atual: [ESCUDO-PADRAO.md](ESCUDO-PADRAO.md).


Fechamento: texto sem corte e batalha/seleção confirmados pelo usuário; seletor/mobile/fluidez continuam pendentes. Percentuais acima revisados e mantidos nesta etapa documental, sem aumento por commit ou lançadores.
