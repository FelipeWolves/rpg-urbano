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

HAB-01 e CIC-01 encerrados no escopo mínimo; total atual 213/213 testes. Perfis equipe/clássico, slots 1–3, básica canônica sem CD/requisito, defesa só no clássico. Kits autorais ativo/passivo/híbrido, escudo BattleStart, condição de vida e IA de prioridade estão integrados; contratos em HAB-01-FINAL.md e documentos R01.

CIC-01: TeamSelection valida 1–5 IDs; demo oferece A1/A2/A3 e --equipe. LocalSession controla tentativa, ledger e recompensa provisória (+10 só por vitória A/Elimination). Save manual v1 de seleção/progresso em build/saves; --continuar valida tudo e abre nova tentativa, sem restaurar combate nem reapurar conclusões antigas. Importação só em sessão intacta, não aceitar saldo/ledger incoerentes. Temporário exclusivo/flush/substituição no Windows; arquivo inválido/futuro ou .pending preexistente não é sobrescrito. Limites 1 MiB/4096 registros; sem concorrência de escritores/autenticação/migração implícita. Contratos CIC-01.md e CIC-01-SAVE-R01.md.

ENG-01 parte 1 documental concluída: usuário confirmou editor não instalado; candidata UE5.8.4, ainda não aprovada. VS2026 e SDK alinhados documentalmente; MSVC14.52 não validado em UBT (referência14.50). Relatório ENG-01.md e ENG-01.json/matriz.csv; nenhuma engine executada e 211 testes anteriores não repetidos. Usuário está instalando manualmente UE5.8.4; não abrir launcher/instalador ou iniciar download pelo agente. Preparação sem editor concluída: save_game.hpp/.cpp contém só formato/validação C++20; I/O em save_store.hpp e save_store_windows.cpp. Formato v1 preservado, locale fixado e 213 testes aprovados; codec testado sem vincular adaptador Windows. Não alegar validação Unreal/mobile. Instalação confirmada em D:\Unreal Engine\UE_5.8:5.8.4/CL58902052, editor/UBT e runtime.NET10 presentes. Configuração local:MSVC14.52 não proibido, não preferido;SDK26100 no intervalo. Metadados não provam build ou integridade completa. Relatório ENG-01-INSTALACAO.md. Build mínimo RpgEngineProbeEditor aprovado com MSVC14.52.37029/SDK26100, aviso não preferido mantido. scripts/testar-unreal.ps1 usa UBT/dotnet empacotados e stage em build/engine-eval, sem abrir editor; fonte em experiments/unreal/RpgEngineProbe. Ferramenta exigiu liberação por acesso à engine; pode criar caches/traces normais, NoEngineChanges não implica zero I/O. Relatório ENG-01-BUILD.md. Próximo: ponte mínima do núcleo e compilação; execução/editor em etapa específica, ainda sem validação de combate UE. Sem escolha definitiva, instalação/download ou abertura de engine automáticos. Silêncio/supressão, passivas durante combate, custos/condições compostas, inventário e save de runtime permanecem futuros. Block não é silêncio; FreezeCooldowns não é congelamento da unidade.

Contratos TIME/RES/SEQ/PAR permanecem válidos no escopo de cada relatório. R04 é contrato próprio, não paridade oficial. Não confundir intercalação cooperativa com threads/servidor, nem perfil experimental de chefe compartilhado com todos os modos PvE. Área/dinâmica/score/fases autônomas por HP continuam lacunas explícitas. Defesa atual arredonda dano/2 para baixo por hit.

## Encerramento

Atualize estado, tarefa e painel pertinentes com resultado, verificações efetivamente executadas, limites e próximo passo. Não duplicar o relatório completo entre documentos. Português nos documentos e explicações, inglês técnico consistente no código. Nunca declarar teste, compilação ou recurso funcional que não ocorreu.
