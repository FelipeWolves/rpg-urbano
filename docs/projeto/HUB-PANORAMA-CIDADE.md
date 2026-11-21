# Hub — panorama da cidade e movimento dos textos

21/11/2026. Implementado e compilado; **aceite visual reservado ao usuário**. Nenhum editor ou prévia foi aberto nesta entrega. Android foi adiado.

## O que mudou

- Fundo diurno autoral `bg_home_middle_day_sunny.png`, 1670×942, copiado sem edição para `assets/runtime/hub/bg-home-day-sunny.png`. Não usamos o gabarito como imagem do jogo.
- Sete pontos separados do fundo: Terminal, Arena PvP, Pub, Praça, Games, Lab - Exp e Biblioteca. Âncoras normalizadas provisórias seguem o guia e continuam ajustáveis pelo catálogo.
- Terminal abre um painel de destinos e oferece explicitamente a batalha de laboratório existente. Não simula viagem, História ou PvP. Lab abre o painel de evento provisório; os outros cinco locais informam “Em breve”. Nenhum sistema comercial/online foi ativado.
- Textos das placas flutuam até2px, em ciclo4,8s, com brilho suave e fases diferentes. A área de clique permanece parada. Movimento reduzido remove deslocamento e variação de brilho; comando `-ReducedMotion` continua disponível e força o estado estático.
- Atalhos inferiores mantêm todos os locais alcançáveis em recortes. Usam uma linha em janelas largas e três colunas em janelas estreitas, com48px de altura por alvo. Controles de área e movimento ficam separados do panorama.
- Tipografia Cormorant Garamond/Lora e paleta executável preservadas. Não é uma migração para UMG: controles continuam em Canvas/C++.

A arte anterior não foi apagada. Catálogo antigo preservado em `data/presentation/archive/startup-terraco-2026-11-21.json`. Os materiais recebidos não foram alterados. Mídias permanecem locais e fora do Git; manifesto registra origem, tamanho e SHA-256 do novo fundo.

## Como testar

1. **Abrir-Preview.cmd**: prévia atual,1280×720, sem abrir o editor para edição.
2. **Abrir-Preview-Mobile.cmd**: janela450×800 com apresentação Mobile forçada. É o mesmo executável Windows; não é Android, não simula GPU/mobile nem comprova toque.
3. **Abrir-Editor.cmd**: projeto estável; clique Play. Não abrir stages antigos em build/p para avaliar esta alteração.
4. Conferir correspondência das sete placas com a imagem, leitura, cliques e painéis. Terminal → Batalha de laboratório → Voltar ao hub deve preservar a sessão existente.
5. Testar Esquerda/Centro/Direita e panorama/foco; todos os destinos também ficam no dock inferior.
6. Alternar movimento: os textos devem ficar imóveis em “Sem movimento”. Testar redimensionamento, principalmente retrato; anotar sobreposições ou placas fora do edifício esperado.

Em janelas menores que280×360, o hub solicita ampliar a janela. Formações, HUD de combate, silêncio/efeitos e desempenho no celular não foram alterados nem validados por esta entrega.

## Código independente — AUT-01

A política `PresentationMode=Auto` está em DefaultGame.ini, seção RpgPrototype. `-Presentation=Desktop|Mobile|Auto` tem precedência. Entrada desconhecida usa Auto e avisa uma vez; não deve encher o log por frame. A política neutra distingue dispositivo real de apresentação solicitada. Testes cobrem parsing, combinações, valor inválido e recorte mobile forçado no desktop. Alterar o enquadramento não muda regras de combate ou sistema operacional.

AUT-01: código e compilação concluídos, aceite visual pendente. AUT-02: dock responsivo é incremento parcial; regiões seguras configuráveis/notch e matriz ampla ainda faltam. AUT-03/04 seguem próximos; esta entrega não comprova toque, gesto longo ou rolagem integral dos painéis.

## Evidência

-26 grupos nativos aprovados:10 apresentação inicial,5 projeção,5 estilo e6 novos de expansão/política/motion.
-8 ações no UBT Editor Win64 Development, saída0; `build/hub-panorama-build.log`.
-Catálogo, manifesto de recursos e cópias locais conferidos. Preparador de pacote validou45 entradas em stage separado; nenhum cook/pacote novo executado.
-Dois processos sem janela do teste anterior foram identificados pelo projeto e encerrados antes da compilação, após usuário confirmar editor fechado. Nenhum controle da tela foi utilizado.

Resultados em HUB-EXPANSAO.json e HUB-PANORAMA-BUILD.json. O aceite visual da versão anterior não se transfere automaticamente ao novo layout. Android, rede, dependências externas e regras do resolvedor permanecem fora desta entrega.
