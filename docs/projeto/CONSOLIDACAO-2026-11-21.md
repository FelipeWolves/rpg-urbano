# Consolidação documental — 21/11/2026

Esta revisão reúne os 15 Markdown recebidos nos grupos 00–11, respostas ENT-01–15, adendos técnicos e decisões posteriores do usuário. Originais são preservados. Esta síntese prevalece sobre notas históricas de recebimento não analisado, instalação ausente ou perguntas já respondidas. Não transforma especificação em implementação.

## Matriz por área

| Área | Confirmado / decisão vigente | Próxima implementação / limite |
|---|---|---|
| Core / ESC | StandardShield: 1% do máximo do aplicador, mínimo 1, 3 turnos naturais; reaplicação mantém maiores força/duração; testes existentes preservados | Não reabrir ESC-01–07; verificar apresentação integrada |
| Combate / SIS | A2 de Felipe fechada; carga por aliado atingido/habilidade, reação após multi-hit, limite 1; escudo no menor HP%, empate Felipe/slot | Implementar kit após base numérica; não confundir com passivas de laboratório existentes |
| Numérico / REG | int64, saturação 10^12, ponto fixo 10000; double anterior substituído; Block, mitigação e dois trilhos definidos | Migração determinística, RNG, limites e regressão antes de novos efeitos; determinismo entre plataformas não comprovado |
| Efeitos | SilverShield especializado, supressão e imunidades respondidos nos adendos | Implementação própria; preservar StandardShield e R04 já aprovados |
| Modos | Ondas, score, clássico/ATB e DJ têm contratos futuros; exemplos SEQ/PAR atuais continuam válidos no seu escopo | Nova política limpa comuns antes de curar vivos; não atribuir a implementação atual |
| Interface | Seleção no chão corrigida e aceita; texto sem corte e batalha confirmados pelo usuário | AUT-01–04: apresentação explícita, proporções/regiões seguras, gesto e painéis; fluidez e toque real pendentes |
| Hub / 2D | Panorama atual e 2 acessos permanecem; 8 variantes novas e 2 guias locais; 7 locais futuros mapeados | Não trocar âncoras/panorama nem ativar 7 destinos nesta entrega. Separar imagem e controles; letreiros da cena ainda existem nas variantes sem widgets |
| Vídeo | Material autoral recebido; skip inicial 0,8s, replay 1 clique, retorno pausado; flag só ao concluir/pular | Player, fallback, orientação e áudio ainda não implementados; mídia recebida não é integração |
| 3D / animação | Demo: Felipe completo, Terraço da Vigília, Eco de Dissonância | Conceitos e contratos não são modelos/rig/animações entregues |
| Áudio / DJ | Direção musical, canais e rankings por pistas descritos | Recursos finais, reprodução sincronizada e validação de latência pendentes |
| Save / economia | Save v1 local e recompensa de laboratório existem; servidor autoritativo é futuro | Integrar UI ao save; economia, itens e nuvem separados; não criar contas/serviços nesta fase |
| Universo | 4 livros / 67 capítulos como direção; demo recortada em 1 personagem e 1 arena | Conteúdo total não entra no denominador da demo; textos completos fora do Git |
| Android | SDK D:\AndroidSDK; API36, NDK27.2.12479018, CMake3.22.1; JDK17 separado; boot AVD e adb confirmados pelo usuário | Reconhecimento Unreal observado com variáveis por processo; persistência ainda precisa correção e teste. Sem APK ou aceite de desempenho |
| Direitos | Markdown recebidos versionados; imagens/vídeos permanecem locais | Logo de terceiro continua provisória até licença/substituição; sem distribuição implícita |

## Ordem de trabalho

1. ENG-ESTAVEL: projeto Unreal de endereço fixo, fonte canônica, configuração persistente, abertura normal e mapa próprio; preservar stages anteriores e núcleo independente.
2. AUT-01–04: fechar política Desktop/Mobile/Auto, geometria, entrada e painéis. Não depende de nova arte.
3. SIS-02 / REG: base numérica determinística e funil; depois A2, supressão e SilverShield, em entregas separadas com testes pertinentes.
4. AUT-05–09: inspeção visual do escudo/formações, habilidade/recarga, acessibilidade, fluxo/save.
5. AUT-10–16: mídia, recursos/animação, dados e pacote; Android mínimo cedo após toolchain persistente, antes de arte final.

Podem avançar em paralelo como produção independente: modelos/rig, arte, roteiro e áudio, desde que respeitem IDs, escala, eventos e direitos. Implementações continuam por blocos; não executar todas as frentes nesta retomada. Faltas de arquivos finais não bloqueiam contratos/fallbacks e interface provisória.

## Evidências e limites

213 testes gerais e suítes incrementais são histórico aprovado, não executado novamente por documentação. Compilação, observação de runtime, relato do usuário e aceite em aparelho são evidências distintas. Os 11 checkpoints em 2 processos Unreal não equivalem à suíte completa nem a 22 casos distintos. Rede não foi isolada; não modificar rede, engine, instalações de jogos ou políticas globais.

[Progresso recalculado](PROGRESSO-GERAL.md) · [Fila detalhada AUT](FILA-DESENVOLVIMENTO-INDEPENDENTE.md) · [Pendências](PENDENCIAS.md).
