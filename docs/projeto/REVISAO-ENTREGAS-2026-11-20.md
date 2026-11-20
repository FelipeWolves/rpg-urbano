# Revisão das 12 entregas —20/11/2026

**Estudadas para planejamento; não implementadas.** Leitura integral dos12 arquivos locais recebidos, comparada aos contratos vigentes e à fila AUT. Originais preservados em entregas/*/recebidos, sem movê-los ou executar instruções contidas neles. Propostas/decisões desejadas são registradas como direção recebida, sem substituir silenciosamente regra fechada. Fontes externas citadas pelos documentos, marcadores [cite:...] e alegações jurídicas/comerciais não foram auditados nesta entrega.

## Atualização posterior — respostas recebidas

Os 15 pontosENT abaixo receberam respostas do usuário. A matriz de conflitos permanece como histórico do diagnóstico; para o estado vigente usar [RESPOSTAS-ENT-2026-11-20.md](RESPOSTAS-ENT-2026-11-20.md). Respondido não significa implementado; somente detalhes de contrato técnico apontados no adendo permanecem por formalizar.

## Síntese que orienta a retomada

- Manter C++ independente + adaptador Unreal C++; não migrar automaticamente para Blueprints/UMG por textos de referência. Canvas atual preservado enquanto uma migração pequena não for justificada.
- Laboratório local sem exigir conta/backend; produção conectada/servidor autoritativo é objetivo futuro. Ausência de dependência de backend não comprova ausência de tráfego da engine; rede não foi isolada/auditada.
- Prioridade desejada Windows → Android → iOS; AVD como ensaio mobile inicial, Galaxy S23 Ultra disponível mais tarde. Emulador8 GB é perfil de teste, não medida de desempenho de celular intermediário nem mínimo de produto já comprovado.
- Demo pequena: hub com2 acessos e configurações, abertura/legendas,1 arena Terraço da Vigília, personagem Felipe e tutorial do capítulo1. Elenco narrativo piloto contém5 nomes (um NPC); quantos terão kit/modelo jogável na demo ainda requer recorte. Não interpretar67 capítulos/5 lojas/PvP/DJ como próxima entrega.
- Cânone proposto:4 livros,67 capítulos (15+22+15+15),3 linhas narrativas, fases elementais em ficha única, círculos/desbloqueios. Manuscritos integrais ficam fora do código; adaptação por sinopses/diálogos/IDs.
- Seletor no chão e corte do texto já corrigidos/aceitos. ESC padrão e R04 não são reabertos; há novas propostas incompatíveis a resolver antes de ampliar regras.

## Matriz por grupo

| Grupo | Informação que já chegou | Trabalho nosso na retomada | Limite / esclarecimento restante |
|---|---|---|---|
|00|4 livros/67 capítulos, BookCodex/NarrativeLines, dificuldades e círculos | Esquema de IDs/progresso e recorte da primeira fase, depois de contrato; não duplicar catálogo por dificuldade | Catálogo capítulo→linha/ambiente, desbloqueios e limites dos bônus ainda não fechados |
|01|PC1080p/1440p,100/125% DPI, vertical412×915; fade150–200ms; foco/teclas/hold; sem movimento | AUT-01–07 com matrizes e testes pertinentes | 412×915 não é9:16 exato; ESC/confirmar/hold têm conflitos abaixo; painel/foco/gamepad não estão prontos só por descritos |
|02|Android Studio/AVD desejado,8 GB, S23 Ultra futuro;30/60FPS; gráficos escaláveis | Conferência de dependências, apresentação testável Windows e depois APK/AVD compatível | SDK/ABI/renderizador/virtualização e recursos do host não validados;4K/HDR/física e mínimos são metas, não garantias |
|03|Matriz silêncio/supressão/Freeze/Petrification; fórmulas, classes de efeitos, bloqueio;4 fichas autorais | Dividir em contratos REG/SIS, implementação por família com regressão R04/ESC | Bloqueio/escudos/tutorial divergem; âncoras/extras, fórmulas e quotas precisam fechar semântica |
|04|Separação equipes/clássico, matriz PvE/score/fases/auxiliares;5 comandos e ATB | Roteiros por família e contratos de alcance/score, depois implementações próprias | SEQ/PAR não cobrem automaticamente novas regras; Defesa+SP/True Damage/PvP/encerramento imediato divergem de contratos atuais |
|05|Uma imagem de hub; nova paleta; menus futuros/eventos clássicos; logo placeholder | AUT-01/02/08, medição de recortes e mapa de navegação com recursos atuais | Novo fundo/cor não aplicado; geometria do retrato/identidade do personagem precisa conciliar; menus descritos como ativos ainda não implementados |
|06|Primeira abertura/rever/pular; legendas externas; fallback2,5s; rotação/restauração; preferências antes do player | AUT-10/11 agora têm base de requisitos; contrato de ciclo de vida antes do player | Pular/retomar com alternativas; canal de mídia; downscale não reduz custo de decode/arquivo automaticamente; nenhum transcode pronto |
|07|1 arena+1 personagem piloto; realismo, escala/pivô/sockets, eventos visuais | AUT-12/13 e pipeline mínimo com recurso provisório; medir antes de lote | Model sheets/rig/arquivos3D não recebidos; altura não garante rig compatível. Ferramentas IA citadas não aprovadas/instaladas; LOD não garante desempenho |
|08|5 canais de áudio, eventos, trilhas/loop; DJ3/4/5 pistas, notas/timing, moedas | Contrato de áudio, relógio/eventos e protótipo rítmico futuro separado | Nenhuma faixa/chart/codec validado; parágrafos contraditórios sobre regeneração e layouts competitivos |
|09|6slots, moedas/lojas separadas, save atômico/migração, offlineQA/onlineprodução | AUT-09 com v1 preservado; modelar economia/catálogo como expansão separada | Arquivo termina em frase incompleta sobre conflito local/nuvem; valores provisórios, int64 tem limite; schema completo exige migração futura |
|10|Universo/facções, elenco, primeiro tutorial, textospt-BR/en, recorteMVP | Preparar localização/IDs, roteiro executável após resolver A2; preservar textos como dados propostos | Ficha/tutorial incompatíveis; primeiro núcleo gráfico não tem gameplay integral do elenco nem menus/ATB declarados como implementados |
|11|Roadmap comercial, PvPassíncrono/cross-save, lojas, direitos/privacidade desejados | Checklist de publicação e fronteiras cliente/servidor, só depois de contrato próprio | Custos/licenças/classificação16+/políticas/conformidade são metas/alegações não auditadas; determinismo não elimina fraude; sem backend/conta nova |

## Pontos para esclarecer antes da implementação afetada

Não bloqueiam AUT-01–04 e engenharia neutra. Podem ser respondidos em lote quando o usuário quiser; não há solicitação de confirmação pendente nesta revisão.

| ID | Prioridade / relação | Divergência ou detalhe necessário | O que preservar até resolver |
|---|---|---|---|
| ENT-01 | Alta / REG-01 | Bloqueio anterior anulava dano; grupo03 propõe redução30% (até50%), chance3% geral/10% Michel e tetos50–100%. Qual semântica substitui a anterior? | Não implementar novo bloqueio nem tratar Block legado como chance |
| ENT-02 | Alta / HAB/tutorial | Felipe A2 é passiva de cargas/contra-ataque na ficha, mas ativa de escudo no tutorial. Qual adaptar? | Passiva não selecionável; cenário de laboratório atual preservado |
| ENT-03 | Alta / ESC/SIS | Escudo do Filhote15%/2 turnos é padrão ou gerador novo? Percentuais/duração não substituem1%/3turnos. Especificar gerador e coexistência | ESC-01–07 vigente, mínimo1 e maiores propriedades |
| ENT-04 | Alta / VIS/HUB | Antiga ESC retorna ao hub; nova proposta abre pausa/encerra sessão. Toque curto também diz confirmar, enquanto seleção+confirmar eram separados. Definir ordem cancelar→fechar→pausa→sair e efeitos sobre tentativa | Sem reset/recompensa por simples navegação; consulta longa não executa ação |
| ENT-05 | Alta / recorte | Acessos atuais estão muito separados: x765 e2166 na imagem2752×1536. Crop vertical com altura integral412×915 mostra ~692px de largura, menor que1401px entre âncoras. Como manter ambos: zoom/contain, reposicionar controles ou pan com acessos fixos? A imagem atual mostra dispositivo na mão do homem, não do lobo | Uma imagem confirmada; não pedir segunda arte nem deformar imagem por hipótese |
| ENT-06 | Alta / REG-03 | CarrierTurnEnd conta extra ou só natural? Supressão afeta auto-revive/prevenção/on-death já enfileirado? Freeze lista Counter/Assist mas não Pursuit; definir ataques enfileirados/auras e ordem R04 | Relógio do escudo e causalidade já fechados; ausência de nova regra não autoriza cancelamento genérico |
| ENT-07 | Alta / SIS-02 | Piso1 de todo dano contradiz Defesa floor(dano/2), que pode zerar. True Damage ignorando Defesa é novo. FunilACC/Miss/Immunity, chances, limites e arredondamentos precisam de contrato próprio | Não aplicar piso/ignorar redução universalmente. Double/int64 não garantem overflow/determinismo sem limites/representação |
| ENT-08 | Média / WAVE/SEQ/PAR | Preservar estados entre ondas é genérico e não revoga remoção de StandardShield. Morte do chefe deve passar por fila/revive/Victory Lock. Pontuação25% overkill pede base, arredondamento, limite e idempotência próprios | Contratos de transições e R04; score não muda HP. Nenhuma regra PvP nova troca desempate atual implicitamente |
| ENT-09 | Média / vídeo | Pular primeira vez:2toques ou hold0,8s? Retoma pausado ou automático1s?5canais não incluem Mídia: canal próprio ou mapeamento? Como marcar assistido após skip/falha? | Não escolher silenciosamente alternativa; protótipo sem login. Rotação/codec/áudio dependem da plataforma |
| ENT-10 | Média / áudio/economia | Grupo08 repete a loja e em uma versão permite buff de regeneração, contrariando grupos02/09 e o próprio texto de taxa fixa. Escolher regra e registrar a duplicação | Não alterar taxa nem economia por parágrafo duplicado |
| ENT-11 | Média / demo | Uma arena/personagem piloto versus5 personagens na demo; curador é NPC. Determinar quantos kits/modelos são necessários no primeiro aceite | Começar pelo pipeline de1 personagem, não transformar elenco literário em5kits automaticamente |
| ENT-12 | Média / save | Grupo09 termina em “em caso de divergência entre save local e nuvem ao”. Completar regra de conflito; separar preferências locais de progresso e estado de combate | Savev1 continua sem snapshot/migração/cloud implementados; preservar backup antes de futura migração |
| ENT-13 | Média / visual | Paleta atual#0F172A80%/magenta/ciano versus nova#0B0D1485%/azul/púrpura; transição e tamanhos precisam aceite após aplicação | Nova direção registrada, mas runtime não alterado por revisão documental; tipografia e2acessos permanecem |
| ENT-14 | Posterior / clássico/DJ | Defesa+15%SP, limite, ATBWait/Active, DJlayouts3/4/5 e multiplicadores introduzem regras novas; definir custo, clocks, simultaneidade e equivalência de rankings | Não expor controles como funcionais antes da implementação. Defesa atual sem recuperaçãoSP |
| ENT-15 | Posterior / produção | Snapshot assinado, verificação replay/seed, cross-save e anticheat precisam autoridade sobre entradas/RNG/catálogo, transações e ameaça definidos | Determinismo ajuda verificação; não prova segurança ou resultados impossíveis de falsificar |

## O que deixou de ser “não informado”

Aparelho futuroS23 Ultra; prioridadeWindows/Android/iOS; AVD desejado; retrato mobile; imagem única; destinos de evento clássico; comportamento de primeira abertura/legendas/fallback como proposta; cânone/elenco/arena iniciais; moedas/slots e modelos de distribuição como direção. Não reenviar perguntas genéricas desses temas. Perguntar somente ENT específico ou dado técnico realmente ausente.

## Dependências e primeira retomada quando houver limite

1. Usuário informa disponibilidade de limite e retomada; não executar blocos automaticamente.
2. Ler esta síntese e a fila AUT atualizada; verificar apenas versões de arquivos recebidos que mudaram.
3. AUT-01 apresentaçãoDesktop/Mobile/Auto, depois AUT-02 recorte com opções conservadoras de teste (não declarar ENT-05 resolvido), AUT-03 entrada, AUT-04 painéis.
4. Android: diagnóstico de compatibilidade da instalação existente, sem instalar nada implicitamente; em seguida plano APK/AVD com ABI/renderizador compatível. [Dependências reais e testes](ANDROID-DEPENDENCIAS.md).
5. Famílias REG/HAB, tutorialFelipe, economia e player somente após reconciliar os pontos que afetam seu contrato. Arte/lojas/servidor permanecem fases separadas.

Nenhum código, catálogo executável, preferência da engine ou arquivo recebido foi alterado. Revisão integrada significa documentação atualizada, não conteúdo implementado. Percentuais preservados; escopo comercial ainda não é escopo fechado.
