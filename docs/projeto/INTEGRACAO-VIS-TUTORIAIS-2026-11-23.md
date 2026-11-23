# Integração VIS / tutoriais / letreiros — 23/11/2026

## Entrega atual

| Corte | Implementado e compilado | Aceite ainda necessário |
|---|---|---|
| VIS-INT-01 | Logo, retrato, habilidades adulto/jovem, estados A2 e nove sprites de status ligados ao HUD comum dos laboratórios | Aparência, contraste, proporção, popup/pressão longa e janelas pequenas |
| TUT-01 | Tutorial padrão A Bola Perdida no Terminal, duas ondas, jovem com ficha própria, preparação de Sangramento, orientações por eventos e pausas sem consumir ações | Fluxo guiado, seleção, reinício, abandono e gravação/reabertura pela interface |
| EVT-EXP-01 | Busca clássica no Lab - Exp, três ações, barras, diálogo50%, vitória/derrota, reinício e abandono | Composição e usabilidade; balanceamento provisório |
| HUB-TIP-01 | Maiúsculas portuguesas explícitas, PRAÇA no catálogo, fonte Roboto Bold Condensed da engine nos letreiros e desenho com brilho/contorno azul/ciano/prata | Comparação estética com a referência e legibilidade no panorama |

LAB-PVE01–06 e LAB-QA01/B/C não foram reabertos como novos blocos. São bases reutilizadas. O evento clássico é a busca não combatente aprovada, não uma expansão automática do sistema clássico de combate.

## Como testar

Abra normalmente o projeto RpgEngineProbe no editor e dê Play, ou use o atalho de preview já existente. Nenhum deles foi aberto nesta entrega.

1. Hub: conferir logo nova, letreiros e PRAÇA. Cormorant/Lora continuam nos outros usos; somente letreiros recebem fonte sem serifa já incluída na engine. Não houve download ou instalação de fonte.
2. Terminal: botão **Tutorial A Bola Perdida >** nas páginas de laboratórios → **Tutorial padrão — A Bola Perdida**. Continuar a introdução, selecionar o inimigo e usar A1 no primeiro passo. A3 fica didaticamente indisponível somente nesse primeiro passo; depois seguem as regras normais. A2 continua passiva.
3. Ler os painéis conforme eventos; eles pausam o envio de ações e o avanço automático. Na onda2, Rajada tem buff de ATK e Trovão começa com Sangramento. Conferir as duas ondas e a conclusão.
4. Lab - Exp: **Iniciar busca clássica**. Reconhecimento aumenta busca; Concentrar melhora a próxima busca sem acumular; Avançar contém a tempestade. Conferir pausa do diálogo, retomada, vitória, derrota e nova tentativa.
5. Ao vencer, conclusão local em **Saved/Tutorials/bola-perdida-v1.txt**, com flags separadas padrão/clássico. Nenhuma moeda e nenhum write em LocalProgress ou saves dos outros modos. Fechar/reabrir e conferir gravação real é aceite pendente. Arquivo inválido/ilegível não é sobrescrito. Clássico permite tentar salvar novamente pela tela de resultado.
6. Reentrar em laboratórios antigos: personagens/HP/status individuais devem continuar próprios. Esta conferência é de integração nova; não é pedido para repetir por rotina vídeo/skip já aceitos.

## Contratos aprovados e implementados

Padrão: CHAR_FELIPE_JOVEM11500PV, ATK1350, DEF950, SPD115; inimigos8500/9200/10800PV e demais atributos da ficha recebida. Nomes e mecânicas A1/A2/A3 do Felipe atual preservados conforme aprovação; ícones jovens são apresentação da variante, não nova fórmula. Retrato fornecido é provisório também para a variante jovem, sem alegar arte jovem final.

Sangramento inicial: snapshot675 (50%ATK1350), 2turnos, fonte/side declarados; somente nos inimigos didáticos previstos de cada onda. Não altera A1, ACC/RESIST, escudos ou regras de DoT. O teste final demonstra vitória com a ficha atual; não certifica dificuldade ou todos os resultados aleatórios. Não se fabricam acertos/ativações para forçar demonstrações.

SPD-15% e ATK+20%: efeitos comuns temporários, 2oportunidades naturais do portador, reaplicação renova uma instância sem somar, nenhum decremento imediato da aplicação no próprio turno. SPD afeta ganho real de AP e iniciativa; ATK afeta dano/snapshot de DoT quando aplicável. Atributos-base não são sobrescritos. Limpeza/dissipação usam o executor comum. Ondas usam WaveV2 com limpeza/cura30% e teto global200ações/empate.

Clássico: busca+25; concentração+25na próxima busca, sem empilhar; tempestade+15poração; Avançar reduz20antes do avanço; busca100vence antes de checar tempestade100; diálogo suspende ações em50%; limite técnico200. Valores aprovados como provisórios e centralizados em SearchSettings no núcleo. O diálogo é apenas uma vez por tentativa. Derrota/abandono não concedem conclusão.

## Recursos e fonte

19 cópias de runtime: 8habilidades512x512, 9status128x128, logo/avatar256x256. Canvas inteiro e relação de aspecto preservados; originais e PNG preparados maiores permanecem intocados. Aproximadamente9,1MiB de pixels RGBA brutos, sem alegar uso total de memória/VRAM ou desempenho.

Catálogo `data/presentation/ui-runtime.json`, arquivos em `assets/runtime/ui/2026-11-23-runtime-v01/`, cópias locais em Content/Presentation. Mídias ignoradas pelo Git. Uma outra máquina/cópia precisa desses arquivos separadamente; o commit contém catálogo/scripts, não as imagens. Scripts de preparação recusam sobrescrever derivados existentes. Fonte: Engine/Content/Slate/Fonts/Roboto-BoldCondensed.ttf; cmap confirma Ç/Ã/Õ/É. String em maiúsculo não depende somente da conversão incompleta de uma plataforma.

A2 ativa usa evento PassiveTriggered real para destaque breve; não inventa carga5/5 permanente. Contadores continuam sobrepostos pela HUD. Imunidade e invencibilidade podem compartilhar sprite, mas seguem efeitos distintos nos detalhes. Faixa azul-clara de escudo mantida. Efeitos sem sprite próprio continuam disponíveis no painel [i].

## Validação e limites

Regressão em lote: 13executáveis, 142grupos aprovados; núcleo compilado uma vez nessa bateria. Depois dos últimos ajustes didáticos e conclusão local, Tutorial foi reexecutado:18/18grupos, incluindo vitória, flags de conclusão e preservação em falha. As contagens se sobrepõem e não devem ser somadas. Compilação Unreal final aprovada; stage92entradas verificado. 19texturas e4glifos de acento conferidos, cópias por hash.

Sem telas, editor/game/commandlet executados, cook/Shipping, Android, rede, ferramentas externas, mudanças de SO ou push. Persistência validada em backend simulado nesta nova modalidade e contrato compartilhado; I/O pela interface Unreal continua pendente. Avisos conhecidos do MSVC14.52 e Attribute.h da engine não foram tratados como falhas novas nem motivaram instalação.

Sem novo aceite visual informado e sem validação em celular, demo global~28% mantida. Próximo passo recomendado: aceitar esta versão integrada e tratar defeitos concretos; a arte futura do Terminal não bloqueia os acessos atuais. Roteiro de QA em data/qa/tutorial-acceptance.json.

> Atualização do reteste: logo/fonte/direção/PRAÇA confirmados pelo usuário. Acabamento revisto com Roboto Light, escala menor e hover luminoso; estágio93 com exclusão local de reimportação. Fonte atual HUB-LETREIROS-E-IMPORTACAO-2026-11-23.md prevalece sobre a menção anterior a Bold Condensed. Tutoriais continuam aguardando aceite próprio.
