# Diretrizes de desenvolvimento — Mundus Lupis Disco (LOBO)

Adotadas em 23/11/2026. A [fila vigente](FILA-DESENVOLVIMENTO-INDEPENDENTE.md) define a ordem de execução. A [política de versionamento](POLITICA-VERSIONAMENTO.md) define as fases, o acesso e os saves.

## Pré-Alfa — execução imediata

- Preservar as regras centrais de combate, os fluxos testados, AUT-13, LAB-PVE-01 a 06 e LAB-QA-01 a 01C. Ligar apresentação, áudio e animação provisória aos eventos existentes.
- Preservar GameAndUI, DoNotLock, Movimento Reduzido e as regras de áreas seguras. Manter as margens atuais do Canvas e aplicar USafeZone onde a interface usar UMG.
- Validar a apresentação em janela. Integrar a arte do Terminal quando a imagem final chegar, usando os acessos atuais para os laboratórios.
- Refinar os materiais provisórios dos sete destinos do molde 3D antes de preparar o mapa editável. Preservar o deslize e a rotação já validados no escopo atual.

> **Consolidação v04.** Use a [visão de telas e modos](2026-11-23_diretrizes-telas-modos-e-evolucao_v04.md) para protagonismo, obtenção canônica, Praça, formações, equipamentos, PvP e economia.

## Direção de design — etapas futuras

A arquitetura abaixo orienta Alfa Plena e Beta. Estruturas modulares e stubs entram somente quando solicitados na fila. Os nomes e percursos descrevem o projeto original.

| Ponto do Terminal | Posição | Destino planejado |
|---|---|---|
| Monotrilho | Superior esquerdo | Rotas especiais, expedições e PvE alternativo, incluindo Ritual, Sobrevivência e 3 Frentes |
| Trem Regional / Metropolitano | Andar intermediário, saída à direita, ponto final | Modo História, trilhas e capítulos |
| Ônibus | Térreo, fundo central | Chefes de Equipamentos |
| Metrô | Térreo, escadas à direita | Farm Rápido, Recursos e Torre/Provações |
| Loja de Conveniência | Térreo esquerdo | Reserva em aberto |

- **História.** Mapa 2D com nós e trilha luminosa. A seleção entre Luminosa, Crepuscular e Sombria altera a trilha ativa e seus nós. A interface prevê dificuldade, progresso e estrelas.
- **Biblioteca.** O Elenco do Jogador reúne personagens obtidos, sintonias, equipamentos e evolução. O Atlas cataloga o universo e abriga Árvores de Correlação que concedem fichas, itens e bônus. A barra fixa da interface reserva espaço para inventário ou bolsa.
- **Formações.** Predefinições reutilizáveis entre PvP e PvE para uma equipe, três equipes sequenciais e três frentes.
- **Economia.** Ouro para evolução e custos gerais, Gemas para convocações e acessos especiais, Notas Musicais para trocas musicais e partituras, e equipamentos com conjuntos e peças clássicas. Valores, regras de aquisição e balanceamento ficam para seus contratos.
- **Pub.** Quadro Negro para diárias, semanais e caçadas. Piano e instrumentos para bônus relevantes de jogo. Caçadas exibem ameaça relativa e requisitos explícitos. [Contrato proposto do Pub e de Games](2026-11-23_pub-cacadas-instrumentos-games_v01.md).
- **Arena PvP e Lab - Exp.** PvP começa com 1x1 em tempo real e pode evoluir para equipes predefinidas assíncronas, com recompensas distintas das do duelo em tempo real. A Arena atual hospeda o piloto Felipe local. Eventos e shows temáticos ficam no Lab - Exp. União fica para uma etapa posterior.

A Biblioteca tem uma **Ala Geral definitiva** e uma **Ala Privada experimental e plugável**. O Lab - Exp tem acesso independente e móvel. [Arquitetura da Biblioteca, do Atlas e do Lab - Exp](2026-11-23_arquitetura-biblioteca-atlas-lab-exp_v01.md). O acesso ao inventário pelo menu fixo é experimental. As trilhas e o piloto de História seguem o [contrato proposto do Modo História](2026-11-23_trilhas-modo-historia_v01.md).

## Campanha — proposta recebida na pasta 00

A nota recebida cataloga **67 capítulos em quatro volumes** com 15, 22, 15 e 15 capítulos. Propõe que cada capítulo aconteça em seu cenário e registre a conclusão em dois lugares. O Guia do Livro (BookCodex) arquiva a entrada no volume correspondente e permite consulta, releitura e repetição. As Linhas Narrativas registram o avanço por tom.

A proposta relaciona esse avanço às fases de sintonia de Felipe, O Filhote e O Dálmata em suas fichas existentes. Também propõe Círculos Narrativos, com expansão permanente de inventário de 100 a 200 slots e liberação de faixas temáticas ao completar um círculo.

A dificuldade usa níveis globais de estatísticas e recompensas na mesma estrutura de capítulos. Variações narrativas são opcionais por capítulo. Normal segue o livro, Difícil pode acrescentar diálogos sem alterar o desfecho canônico e níveis superiores podem oferecer encontros alternativos e registros secretos em capítulos selecionados. Catálogo, nós, desbloqueios, recompensas e persistência dependem do contrato de Campanha.

## Documentação e fechamento

Novos documentos de design ou mapeamento usam AAAA-MM-DD_assunto_v01.md. Distinguir decisões futuras, propostas recebidas, implementação e testes. Preservar os originais recebidos. Ao concluir uma tarefa, relatar a entrega, os contratos preservados e os testes ou aceites pendentes.
