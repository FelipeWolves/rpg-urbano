# VIS-01 — interação compartilhada e perfil clássico

19/11/2026. Implementação do incremento autorizado enquanto o documento de efeitos permanece em elaboração. Este relatório distingue regras, apresentação e evidência; não declara equivalência com Dislyte.

## Entrega

| Recurso | Equipes | RPG clássico |
|---|---|---|
| Ícone e painel de efeitos reais | Compartilhado | Compartilhado; inclui defesa ativa |
| Barra de HP e faixa azul-clara de escudo | Compartilhada | Compartilhada, mesmos pontos reais |
| Consulta prolongada e painel de detalhes | Posições de habilidades e passivas | Ataque e defesa; lista informativa de especiais |
| Candidatos vivos e recusas explicadas | Conforme alvo e disponibilidade da habilidade | Ataque contra inimigo vivo; defesa sem alvo |
| Regras dos comandos | Até três posições; sem defesa genérica | Ataque e defesa existentes do núcleo |
| Passar | Fluxo existente | Somente quando bloqueado; não é espera livre |
| Itens e habilidade especial adicional | Fora deste incremento | Não cadastrados; não simulados por botões funcionais |

A defesa continua reduzindo cada hit à metade com arredondamento para baixo. É consumida ao encerrar uma ação que a utilizou; se não utilizada, expira na próxima oportunidade do dono. Nenhuma fórmula de dano foi duplicada na interface.

O adaptador visual agora aceita um perfil explicitamente e encaminha ações clássicas para a mesma LocalSession usada pelo protótipo. Recusas não avançam revisão ou combate; reinício invalida comandos antigos. O inimigo conserva a automação do respectivo perfil. A informação consultada não consome ação; painéis suspendem apenas o agendamento local da automação.

## Câmera e regras são opções independentes

No código anterior, `-ArenaLayout=Classic` alterava apenas a câmera; a sessão gráfica ainda era de equipes. O novo seletor deixa a diferença explícita:

| Argumentos para uma compilação atualizada | Regras | Câmera |
|---|---|---|
| Sem opções | Equipes | Profundidade padrão |
| `-ArenaLayout=Classic` | Equipes | Lateral preservada |
| `-CombatProfile=Classic` | Clássicas | Profundidade padrão |
| `-CombatProfile=Classic -ArenaLayout=Classic` | Clássicas | Lateral preservada |

O HUD identifica as regras ativas. Valor desconhecido de CombatProfile gera aviso e conserva Team. Os comandos do console Jogar.cmd/Jogar-Classico.cmd permanecem disponíveis e separados. Estes argumentos são do aplicativo próprio, não de qualquer jogo de terceiros.

Na apresentação em profundidade, ataque/habilidade sugere um alvo válido, permite trocar e exige confirmação. Na lateral, permanece selecionar comando e clicar o alvo para executar. O marcador geométrico também pode indicar o candidato sob o mouse na lateral. Defesa é comando direto sem alvo. `Skills / info` informa honestamente que não há especiais cadastrados no encontro clássico atual. Gesto prolongado de0,5s é provisório; toque não validado.

## Validação

`testar-combate.ps1` concluído: **213 testes gerais**, **6 do adaptador**, **6 da apresentação compartilhada** e **6 novos grupos clássicos**, todos aprovados. Contagens separadas; não são testes de interface Unreal. Geometria não mudou e seus6 grupos anteriores não foram repetidos.

Grupos clássicos: separação de perfil/defesa recusada em equipes; defesa e resposta automática equivalentes à sessão nativa, incluindo9/2=4; ataque/revisões/reinício; recusas atômicas de alvo/ator/passar/defesa com alvo; candidatos/informação/ausência de especiais; batalha completa e recompensa única. Compilação C++ com /W4 /WX.

Resultados: [contratos clássicos](../relatorios/atuais/VIS-01-CLASSICO.json), [apresentação](../relatorios/atuais/VIS-01-INTERACAO.json), [regressão geral](../relatorios/atuais/COMBATE.json). Compilação Unreal registrada separadamente em [build](../relatorios/atuais/VIS-01-CLASSICO-BUILD.json).

## Escudo e limites

A faixa representa pontos de escudo recebidos do núcleo. O exemplo percentual solicitado é10% do HP atual:100→10 e50→5. Os escudos antigos são fixos de10 pontos e continuam assim; sua faixa não finge que protegem5. Instante de cálculo, atualização, acúmulo, prioridade de absorção e interações serão fechados com o documento do usuário. A defesa clássica é distinta do escudo e continuará admitindo futuras regras próprias.

Não houve janela de editor/aplicativo aberta, instalação, alteração de rede, acesso a jogos/servidores externos ou push. Compilar não equivale a aceitar visualmente. Pacotes anteriores não contêm este incremento.

## Próximo aceite, com abertura autorizada separadamente

1. Gerar pacote atualizado; observar equipes e clássico com as duas câmeras, começando pelos dois pares mais usados (Team/Standard e Classic/Classic).
2. Conferir defesa, consumo/expiração e indicação de efeito; atacar, receber resposta, reiniciar e concluir batalha.
3. Abrir efeitos e detalhes, segurar/soltar sem executar, trocar alvo e confirmar; verificar que painéis impedem ações automáticas durante a consulta.
4. Avaliar sobreposição, marcador e recusas; depois formações1/5, assimetrias e redimensionamento. A geometria1–5 já testada não substitui esses aceites.
5. Incorporar o documento pendente antes de novos efeitos, silêncio/supressão, recálculo percentual ou inventário. Mobile, acessibilidade e localização continuam próprios blocos de validação.

### Evidência do build desta entrega

Editor/Win64/Development:19 ações, saída0,37,39s; stage e2394b7f preparado a partir de32 entradas canônicas. Primeira tentativa restrita não conseguiu acessar Engine/Source e saiu6; após autorização do acesso necessário, o build passou. Logs anteriores preservados. Avisos do compilador não preferido e C4864 da engine mantidos; nenhum código da engine alterado. O processo compilador não iniciou o editor gráfico.
