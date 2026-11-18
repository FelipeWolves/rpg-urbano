# Regras do projeto

## Retomada

Leia README.md, docs/projeto/ESTADO.md e a tarefa ativa em docs/projeto/TAREFAS.md. O painel é docs/batalhas/PAINEL-BLOCOS.md; dúvidas ficam em docs/projeto/PENDENCIAS.md. Consulte visão, arquitetura e decisões apenas quando relevantes. Estado verificado dos arquivos prevalece sobre histórico; não reconstruir decisões relendo chats ou relatórios externos. Histórico anterior de orientações preservado em docs/projeto/HISTORICO-ORIENTACOES.md.

## Escopo e segurança

RPG original, urbano e musical; C++20/MSVC x64, engine ainda não escolhida. Não acessar instalações de jogos, launchers, contas ou conteúdo protegido; não copiar recursos proprietários. Usar recursos próprios ou licenciados, registrando origem. Não contornar proteções. A autorização pontual histórica de investigação externa não amplia o escopo atual.

Dentro da tarefa autorizada, editar arquivos próprios e executar verificações pertinentes. Instalações, downloads, escolha definitiva de engine, serviços, publicação e operações destrutivas exigem decisão específica. Não habilitar integrações de IA/API ou envio de dados implicitamente. Não modificar PATH global, política de scripts ou código de engine.

## Unidade de trabalho e consumo

Uma entrega funcional por bloco com aceite explícito. Não usar subagentes sem solicitação. Não executar todos os blocos numa entrega. Preferir leituras seletivas e automação local; não repetir baterias sem mudança/falha pertinente. Não criar testes para simples documentação. O agente não conhece saldo de uso; a reserva de 30% é acompanhada pelo usuário, sem promessas de estimativa/parada automática.

## Estrutura e engenharia

Siga CONTRIBUTING.md. Interfaces em include/rpg, implementações em src, entradas em app, testes em tests, utilitários próprios em tools e automação em scripts. Separe regras de apresentação, áudio, persistência e serviços; use IDs estáveis e dados/saves versionados. Binários/objetos/logs ficam em build ignorado. Relatórios consolidados: docs/relatorios/atuais; aceites históricos: docs/batalhas.

Poucas dependências/plugins, somente por necessidade demonstrada e procedência/licença verificadas. Não instalar ferramentas antecipadamente. Migração de engine exige avaliação isolada e regressão, sem garantir ausência de retrabalho. Não escolher preview só por ser recente.

## Comandos e Git

Compilar/testar: .\scripts\testar-combate.ps1. Diagnóstico de ambiente: .\scripts\verificar-ambiente.ps1, somente quando necessário. Não usar bypass de política. Jogar.cmd abre equipes, Jogar-Classico.cmd abre clássico; ambos dependem de build/combate/jogar.exe local. Scripts descobrem VS instalado com vswhere e usam VsDevCmd em processo filho; não instalam nada.

Git local com origin cadastrado pelo usuário. Ele faz os pushes; não alterar identidade/configuração/histórico ou fazer push automático. Confira status antes de editar, preserve mudanças dele e avise se conflitarem. Commits locais de entregas concluídas/testadas estão autorizados. Não commitar conteúdo externo, segredos ou binários gerados.

## Estado funcional vigente

HAB-01 partes 1–2 concluídas, 169/169 testes aprovados: perfis equipe/clássico e slots 1–3, básica canônica sem CD, defesa apenas no clássico. Preparação é passiva funcional de escudo BattleStart, uma vez por combate, registrada no snapshot; outras notas sem definição continuam informativas. Contrato HAB-01-PASSIVA-ABERTURA-R01.md. Próximo: parte 3, fechar uma primeira condição adicional de habilidade antes de implementar. Silêncio/supressão, condições especiais e itens ainda não implementados. Block não é silêncio; FreezeCooldowns não é congelamento da unidade.

Contratos TIME/RES/SEQ/PAR permanecem válidos no escopo de cada relatório. R04 é contrato próprio, não paridade oficial. Não confundir intercalação cooperativa com threads/servidor, nem perfil experimental de chefe compartilhado com todos os modos PvE. Área/dinâmica/score/fases autônomas por HP continuam lacunas explícitas. Defesa atual arredonda dano/2 para baixo por hit.

## Encerramento

Atualize estado, tarefa e painel pertinentes com resultado, verificações efetivamente executadas, limites e próximo passo. Não duplicar o relatório completo entre documentos. Português nos documentos e explicações, inglês técnico consistente no código. Nunca declarar teste, compilação ou recurso funcional que não ocorreu.
