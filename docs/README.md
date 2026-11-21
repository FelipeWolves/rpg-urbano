> **Entrega concluída:** [projeto Unreal estável e guia de abertura](projeto/UNREAL-PROJETO-ESTAVEL.md). Controle de tela encerrado por solicitação; nova interação com o editor exige nova liberação. Atualização documental/Git apenas após essa solicitação.

> **Estado vigente — 21/11/2026:** [consolidação das 15 entregas e decisões posteriores](projeto/CONSOLIDACAO-2026-11-21.md). ENG-ESTAVEL concluído no Windows local: projeto fixo, mapa próprio, abertura/reabertura e Play observados. Próximo: AUT-01–04. ESC comum e seletor já aceitos não são reabertos. Android SDK/NDK/JDK localizados e boot do AVD confirmado pelo usuário; caminhos já persistem, mas ValidatePlatforms retorna Android INVALID r27c; diagnóstico e APK pendentes. Percentuais recalculados no painel de progresso. As notas datadas abaixo preservam histórico e não reabrem decisões substituídas.

# Documentação — índice vigente

Atualizado em19/11/2026. Comece pelas sínteses atuais; relatórios datados delimitam a evidência de cada versão.

## Visão do projeto inteiro

[Progresso geral por área](projeto/PROGRESSO-GERAL.md): estimativas de maturidade, trabalho restante e frentes que podem avançar separadamente.

## Retomada e pesquisa

- [Estado](projeto/ESTADO.md), [tarefas](projeto/TAREFAS.md) e [prioridades](projeto/PLANO-PRIORIDADES.md).
- [Pendências e formulário por prioridade](projeto/PENDENCIAS.md): ESC-01 a ESC-07 estão fechados para o escudo padrão; VIS/REG/SIS organizam aceites/extensões pendentes.
- [Painel de blocos e legenda](batalhas/PAINEL-BLOCOS.md): aceites separados das extensões.
- [Pesquisa recebida sobre habilidades/itens](batalhas/PESQUISA-HABILIDADES-ITENS-HIERARQUIA-USUARIO.md), [revisão técnica](projeto/REVISAO-PESQUISA-HABILIDADES-ITENS.md) e [proveniência/hash](relatorios/atuais/PESQUISA-HABILIDADES-RECEPCAO.json). Alegações externas ainda não auditadas.
- [Visão](projeto/VISAO.md), [arquitetura](projeto/ARQUITETURA.md), [decisões](projeto/DECISOES.md), [ferramentas](projeto/FERRAMENTAS.md), [contribuição](../CONTRIBUTING.md) e [orientações](../AGENTS.md).

## Evidências atuais

| Área | Relatório | Alcance e limite |
|---|---|---|
| Núcleo/console | [Fundação](projeto/COMBATE.md), [resultado](relatorios/atuais/COMBATE.json) |213 gerais; não são testes de interface Unreal |
| Habilidades/ciclo | [HAB-01](batalhas/HAB-01-FINAL.md), [CIC-01](projeto/CIC-01.md), [save](projeto/CIC-01-SAVE-R01.md) | Escopos mínimos encerrados; save de progresso entre batalhas |
| Instalação/ponte | [Instalação](projeto/ENG-01-INSTALACAO.md), [build](projeto/ENG-01-BUILD.md), [runtime](projeto/ENG-01-RUNTIME.md) |11 checkpoints iguais em dois processos; não são22 casos distintos |
| Ambiente gráfico | [Diagnóstico](projeto/ENG-01-GRAFICO-DIAGNOSTICO.md) | Inglês priorizado nas ferramentas; português do produto separado; sem isolamento de rede validado |
| Pacote Windows básico | [Preparação](projeto/ENG-01-PACOTE-PREPARACAO.md), [build](projeto/ENG-01-PACOTE-BUILD.md), [runtime](projeto/ENG-01-PACOTE-RUNTIME.md) | Build/abertura/encerramento aprovados nesta máquina; sem Windows limpo ou distribuição final |
| Arena inicial | [Plano](projeto/VIS-01-PLANO.md), [adaptador](projeto/VIS-01A.md), [cena](projeto/VIS-01B.md), [aceite3×3](projeto/VIS-01C.md) | Batalha/reinício observados; aceite visual integral parcial |
| Câmeras | [Layouts](projeto/VIS-01-LAYOUTS.md), [observação no pacote](projeto/VIS-01-LAYOUTS-RUNTIME.md) | Profundidade padrão e lateral preservada; geometria6 grupos; legibilidade ainda pendente |
| Interação revisada | [Contrato inicial](projeto/VIS-01-INTERACAO-R01.md), [implementação](projeto/VIS-01-INTERACAO-IMPLEMENTACAO.md) |6 grupos nativos; build aprovado; sem inspeção visual desta revisão |
| Perfil clássico gráfico | [Entrega e roteiro](projeto/VIS-01-CLASSICO-INTERACAO.md), [testes](relatorios/atuais/VIS-01-CLASSICO.json), [build](relatorios/atuais/VIS-01-CLASSICO-BUILD.json) |6 grupos próprios; câmera e regras independentes; pacote atualizado ainda pendente |

## Escudo padrão vigente

[Contrato e implementação](projeto/ESCUDO-PADRAO.md), [14 grupos nativos](relatorios/atuais/ESC-01-PADRAO.json) e [build Editor da revisão de mínimo1](relatorios/atuais/ESC-01-MINIMO-BUILD.json). Caso comum fechado; sem pacote/aceite visual desta revisão.

## Referências do hub recebidas

[Associação das quatro capturas e análise de integração](projeto/HUB-01-REFERENCIAS.md). Proposta recebida, sem implementação de hub ou adoção automática de tecnologias/lojas.

[Base de apresentação HUB-01A](projeto/HUB-01A-APRESENTACAO.md): políticas nativas de panorama/vídeo e catálogo local; renderer/player/mobile ainda não ligados.

[Mídias recebidas do HUB-01A](projeto/HUB-01A-RECURSOS.md): inventário real, catálogo e observações visuais, sem reprodução dos vídeos.

[Acessos provisórios HUB-01B](projeto/HUB-01B-ACESSOS.md): Evento Especial e Batalha cadastrados; [ligação gráfica escrita/compilada](projeto/HUB-01B-IMPLEMENTACAO.md), execução pendente.

[Briefing visual opcional para o usuário](projeto/BRIEFING-VISUAL-HUB.md): imagem marcada, aparência dos acessos e identidade/painel provisórios.

[Revisão do briefing visual preenchido](projeto/HUB-BRIEFING-REVISAO.md): decisões recebidas, licenças consultadas e escolhas restantes; sem aplicação do estilo ainda.

## Organização dos arquivos

Núcleo em include/rpg e src; testes em tests; automação em scripts. [Experimento Unreal](../experiments/unreal/RpgEngineProbe/README.md) contém fonte versionada. Stages, objetos, pacotes, logs e saves ficam em build ignorado. Não editar fontes gerados ou mover núcleo para a engine.

- docs/projeto: sínteses, decisões, contratos de integração e planejamento.
- docs/batalhas: contratos/evidências de mecânicas e pesquisas recebidas, com procedência na revisão correspondente.
- docs/relatorios/atuais: resultados estruturados pequenos; caminhos locais identificam artefatos não distribuídos.
- [Histórico de consolidação](projeto/historico/2026-11-18/README.md): documentos antigos preservados, sem virar instruções atuais.

Atualize a síntese vigente e os links; preserve aceites datados. Não copie a lista de perguntas para vários arquivos: o registro central é PENDENCIAS.md. Pesquisa recebida não deve sobrescrever contratos aprovados. Evite repetir testes para mudanças apenas documentais.

[Estilo e decisões visuais do hub](projeto/HUB-01C-VISUAL.md): código/testes/build aprovados; fontes/logo de runtime e aceite visual pendentes.

Atualização vigente da identidade: fontes Cormorant Garamond/Lora e PNG da logo conectados ao HUD; fontes locais com OFL, sem instalação global.44 entradas preparadas; ver [resultado e limites](projeto/HUB-IDENTIDADE.md). [Sequência e entregas independentes](projeto/PROXIMAS-FASES.md); [percentuais revisados](projeto/PROGRESSO-GERAL.md). Esta atualização prevalece sobre menções anteriores de fontes/logo ainda não recebidas.

Fechamento do hub: [aceite do usuário e acesso independente](projeto/HUB-IDENTIDADE.md). Próxima correção: seletor no chão; mobile em investigação separada.

[Central de entregas do usuário](../entregas/README.md): guias por área, modelo, índice e pastas locais para materiais ainda não revisados. Não duplica nem substitui contratos fechados.

[Seletor no chão](projeto/VIS-SELETOR-CHAO.md): correção3D com profundidade, aceite visual do usuário pendente.

[Planejamento mobile revisado](projeto/MOBILE-PLANO-REVISAO.md): Canvas versus UMG, preview versus dispositivo, ferramentas e sequência proposta.

[Fila independente de desenvolvimento](projeto/FILA-DESENVOLVIMENTO-INDEPENDENTE.md): tarefas possíveis com recursos/contratos existentes, sem aguardar a coleta do usuário; plano não executado.

[Revisão das12 entregas](projeto/REVISAO-ENTREGAS-2026-11-20.md): informações recebidas, conflitosENT e roteiro de retomada. [Android Studio e dependências de teste](projeto/ANDROID-DEPENDENCIAS.md): instalação confirmada,toolchain/AVD ainda não validados.

[Respostas ENT-01–15](projeto/RESPOSTAS-ENT-2026-11-20.md): decisões recebidas, mudanças futuras de contrato e fila preservada;sem implementação nesta recepção.

[FelipeA2 — Postura de Vigília](projeto/FELIPE-A2-POSTURA-VIGILIA.md):cargas/contra-ataque/escudo e destino definidos;implementação pendente.

[Ponto2 — bloqueio, mitigação e True Damage](projeto/BLOQUEIO-MITIGACAO-DANO-PONTO2.md): regras respondidas, implementação futura.

[Ponto3 — trilhos de dano/debuff e escala numérica](projeto/FUNIL-ESCALA-NUMERICA-PONTO3.md): int64/teto10^12, regras e testes futuros,sem implementação.

[Adendo técnico consolidado](projeto/ADENDO-TECNICO-CONTRATOS-2026-11-20.md): regras adicionais de supressão,escudos,ondas e score;planejamento,sem execução.

[CheckAndroid realizado](projeto/ANDROID-CHECK-2026-11-20.md):Studio instalado,toolchain incompleta/não localizada e incompatibilidadeJava25/Gradle8.13 identificada por metadados;sem execução.

[Complemento final — A2, ACC/RESIST e ponto fixo](projeto/COMPLEMENTO-FINAL-A2-ACC-PONTO-FIXO.md): casos de borda, equação, saturação e representação futura definidos; implementação e regressão pendentes.

[Novidades de recebimento21/11](projeto/RECEBIMENTO-NOVIDADES-2026-11-21.md):nova síntese de7pontos doHub,documento alterado e Markdown autorizado noGit;imagens locais,sem implementação.
