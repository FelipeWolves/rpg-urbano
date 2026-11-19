# Próximas fases e frentes independentes

Plano vigente em19/11/2026. Uma entrega funcional por vez; esta lista não inicia automaticamente os blocos. Fontes e logo locais estão conectadas ao código; evidências de execução ficam em HUB-IDENTIDADE.md. Não reabrir contratos de combate encerrados sem defeito concreto.

## Sequência prioritária

| Ordem | Entrega | Dependência mínima | Critério de encerramento | Esforço relativo |
|---|---|---|---|---|
|1| Fechar aceite visual Windows do hub e da arena revisada | Build atual e execução autorizada | Hub/evento/batalha/volta, animação/estático, acentos,1/3/5 unidades, resoluções e escudo conferidos | Pequeno/médio; corrigir defeitos primeiro |
|2| Interface utilizável em telas pequenas | Resultado da fase1 | Texto integral/rolagem, foco, toque, áreas seguras, opção de acessibilidade persistida | Médio |
|3| Primeiro ensaio mobile sem arte final | Aparelho-alvo, SDK e autorização de instalação/configuração se necessário | Compilar/instalar, abrir arena e hub, medir memória/tempo de quadro, encerrar/retomar | Médio/alto; antecipar antes de produção artística extensa |
|4| Entrada e vídeo local | Catálogo atual e contrato de interrupção/pular/áudio | Player com fallback, proporção, preferências antes da janela; orientação/restauração testadas por plataforma | Médio |
|5| Fatia vertical autoral | Fases1–4 e pequeno conjunto de conteúdo original | Abertura→hub→encontro→resultado→save/reentrada em Windows e celular | Alto, dividir em entregas |
|6| Expansão de sistemas/conteúdo | Fatia vertical estável e contratos REG/SIS/PvE pertinentes | Incrementos testados de habilidades, equipamentos, modos e progressão | Alto, contínuo |
|7| Preparação de distribuição | Desempenho, direitos, localização e escopo fechados | Pacotes e requisitos específicos das lojas; serviços apenas se necessários | Alto; não bloqueia laboratório local |

As fases2/3 podem preparar contratos e diagnóstico em paralelo à fase1; isso não declara experiência mobile aprovada. Windows Store/Steam/backend não são pré-requisitos do protótipo local.

## Entregas que podem ser preparadas agora

| Prioridade | Frente independente | Entrega útil para implementação | Depende de quê depois? |
|---|---|---|---|
|Alta/rápida| Fichas de habilidades e efeitos | Markdown/JSON proposto com ID, alvo, gatilho, base de cálculo, relógio, duração, limites e exemplos | Revisão do contrato antes de virar dados executáveis; não copiar personagens oficiais |
|Alta/rápida| Matriz de interface e textos | Estados, mensagens de recusa, descrição completa, tradução pt-BR/en e ações de cada tela | UI/foco/rolagem; traduções reais ainda precisam localização |
|Alta/rápida| Identidade e acessibilidade | Tamanhos mínimos, contraste, foco, estados sem cor/movimento e teste por resolução | Validação visual/dispositivo, sem depender de combate novo |
|Alta| Definição do aparelho-alvo | Modelo, SO, memória, orientação preferida e disponibilidade para teste | Ferramentas e primeiro build mobile |
|Média| Universo e conteúdo original | Briefing, roteiro, elenco mínimo, encontro/tutorial e condição de vitória | IDs/implementação de conteúdo e balanceamento |
|Média| Hub final e arte2D | Imagem fonte, zonas seguras/recortes, camadas, ícones próprios e licença/autoria | Substituição no catálogo e aceite em resoluções reais |
|Média| Música e efeitos | Arquivos próprios, fontes editáveis, loop, volume e tabela de eventos | Integração de áudio, interrupção/retomada e testes |
|Média| Vídeo de abertura/evento | Arquivos paisagem/retrato existentes ou revisados, storyboard, legendas, duração e regra de pular | Player e validação de codec/plataforma |
|Média/alta| Cenário3D | Uma cena pequena original, unidade/escala, pivôs, materiais e fontes editáveis | Orçamento mobile e integração na arena; não produzir cidade completa antes do ensaio |
|Alta complexidade| Personagem/rig/animação | Um modelo de teste, esqueleto, idle/ação/impacto/morte com pontos de sincronismo | Contrato de animação e integração; sem esperar catálogo inteiro de habilidades |
|Separada| Progressão/economia | Proposta de recompensas/desbloqueios com limites e dados fictícios identificados | Revisão de save/migração e implementação própria |

Não há independência absoluta. Entregar fontes editáveis, arquivo final, versão, dimensões/escala, autoria/licença e critérios de aceite junto de cada recurso. Não se deve começar vários grandes blocos de código simultaneamente na mesma árvore: manter interfaces estáveis, uma entrega por commit e reservar integração/regressão.

## Organização e consumo

- Estado técnico: ESTADO.md; dúvidas por prioridade: PENDENCIAS.md; percentuais: PROGRESSO-GERAL.md; tarefa imediata: TAREFAS.md.
- Regras em src/include; fontes Unreal canônicas em experiments/unreal; dados em data; mídia local em assets/runtime e fontes editáveis em assets/sources.
- Preparação gera cópias em build; nunca editar essas cópias como origem. Manifestos e scripts permitem conferir/reproduzir sem commitar mídia grande.
- Revisar somente o escopo alterado; testes de combate completos apenas se houver mudança/dependência pertinente. Arte recebida não significa integração aprovada.
- O usuário acompanha o saldo de uso; não há acesso do assistente ao limite diário nem garantia de quantos blocos caberão nele.

Prioridade imediata após o fechamento: corrigir seletor sobreposto à esfera, validar sob a unidade e então continuar a matriz visual. Texto sem corte confirmado pelo usuário; seleção e batalha funcionais no relato. Apresentação mobile e fluidez permanecem separadas, sem causa/regra inventada.
