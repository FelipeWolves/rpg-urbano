# TerminalCore — mapa de navegação para revisão

**Data:** 28/11/2026

**Estado:** primeiro corte do TerminalCore integrado à prévia. O catálogo C++ fornece os cinco bilhetes, seus destinos e a disponibilidade. O núcleo aceita apenas o Monotrilho, preserva a página durante a sessão e abre a primeira página em um novo embarque. O reteste da navegação em jogo continua previsto.

**Verificação:** a bateria nativa de apresentação passou nas 12 suítes, incluindo catálogo, recusa de embarque, retorno de batalha e tutorial e reinício de página. O módulo Win64 do RpgEngineProbe compilou sem abrir o Editor.

## Bilhetes

| Bilhete | Estado atual | Ação atual | Destino planejado |
| --- | --- | --- | --- |
| Monotrilho | Disponível, roxo | Abre o menu de testes locais na primeira página | Rotas especiais, expedições e PvE alternativo |
| Trem Regional | Em breve, azul | Sem transição | Modo História, trilhas e capítulos |
| Ônibus | Em breve, verde | Sem transição | Chefes de equipamentos |
| Metrô | Em breve, laranja | Sem transição | Farm rápido, recursos e Torre/Provações |
| Loja de Conveniência | Reservada, vermelha | Sem transição | Utilidades da estação, função em aberto |

## Monotrilho na prévia

| Página | Opções existentes |
| --- | --- |
| Primeiros modos | Laboratório de equipe, Ritual local, Ritual com fases e Ondas consecutivas |
| Mais modos | Pontuação em 12 ações, Sobrevivência por 3 turnos do chefe e 3 equipes em duas variantes (chefe com fases e chefe com kit fixo) |
| Frentes locais | 3 frentes com ataques locais e 3 frentes com especial global |
| Tutorial padrão | A Bola Perdida, com conclusão local separada |

O atalho do tutorial está nas três páginas de laboratórios. No tutorial, **Outros laboratórios** leva à primeira página e **Voltar aos bilhetes** fecha o menu de testes. Na arena, **Pausar / Voltar** abre a saída durante a sessão e **Voltar ao Terminal** aparece após o encerramento. O fluxo de retorno passa pelo menu do Monotrilho, pelos bilhetes e pelo Hub de origem. O tutorial clássico continua acessível pelo Lab - Exp.

## Contrato do primeiro corte para 2D e 3D

- Cada bilhete tem um identificador estável, rótulo, estado e destino. O TerminalCore fornece esses dados às duas apresentações.
- Ao solicitar embarque, o núcleo informa se a ação está disponível e devolve o destino ou o motivo da indisponibilidade. A Unreal conduz a navegação às sessões existentes.
- O primeiro corte reproduz as rotas acima. Custos, multiplicadores, recompensas e desbloqueios ficam para contratos próprios.
- O reteste deve cobrir a entrada pelos dois Hubs, os retornos durante e após uma sessão, o tutorial, os quatro bilhetes indisponíveis e as abas futuras. Conferir também se um clique repetido atravessa telas.

## Decisões para revisar juntos

1. **Rotas futuras (decisão aprovada):** manter os quatro bilhetes visíveis com **Em breve**, sem abrir uma ficha ao clicar. **Aplicado na prévia 2D:** camada escura translúcida estática e ícone de proibição ao lado de **Em breve**. Aceite visual concedido pelo usuário em 28/11/2026.
2. **Retorno da sessão (decisão aprovada em 28/11/2026):** ao sair de uma batalha ou do tutorial, voltar à página do Monotrilho de onde a sessão foi iniciada. Ao embarcar novamente pela tela de bilhetes, abrir a primeira página. O reteste em jogo deve verificar os dois caminhos.
3. **Abas Trilhas e Expedição (decisão aprovada em 28/11/2026):** cada aba abrirá uma tela com conteúdo próprio quando liberada. Na prévia, **Rotas** fica iluminada e as duas abas futuras aparecem escurecidas, com ícone de proibição. A faixa translúcida sobre a tela 2D e o painel no espaço 3D usam o retrato de teste do Felipe. Aceite visual dos moldes 2D e 3D concedido em 28/11/2026. Seta **Voltar** validada pelo usuário com mouse nos moldes 2D e 3D em 28/11/2026. A interação por toque segue no roteiro de reteste.

## Referências

- [Diretrizes de telas e modos](2026-11-23_diretrizes-desenvolvimento-lobo_v01.md)
- [Parecer de arquitetura](2026-11-27_parecer-arquitetura-lobo_v01.md)
- [Referência e roteiro de teste do Terminal 2D](2026-11-27_terminal-2d-referencia-visual_v01.md)
