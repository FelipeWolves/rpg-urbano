# LAB-QA-01C — persistência compartilhada

Entrega de 22/11/2026. `include/rpg/persistence.hpp` contém a orquestração única de progresso e preferências, usada tanto pelo HUD Unreal quanto pelo laboratório nativo. O backend de arquivos continua separado: Unreal usa IFileManager/FFileHelper; testes usam Windows/filesystem em pasta nova de QA.

## Comportamento preservado

- Formatos v1, nomes dos arquivos principais e diretórios Saved permanecem os mesmos. Não há migração nem leitura de saves normais durante esta entrega.
- Progresso: validação, temporário, substituição, sem backup automático. Temporário órfão não é carregado/promovido. Em falha, o temporário pode permanecer para diagnóstico.
- Preferências: preservar primário válido em .bak; primário inválido legível é preservado em arquivo .invalid com identificador único, sem sobrescrever backup válido; falha na preservação impede gravação.
- Preferências com arquivo anterior ilegível ou acima do limite são preservadas. Falha de escrita/commit tenta remover somente o temporário da operação.
- Leitura limitada a 1 MiB no progresso e 4096 bytes nas preferências; codec valida conteúdo antes de aplicar. Backend Unreal mantém leitura textual compatível por FFileHelper para preferências, inclusive interpretação de BOM.
- Backend Unreal mantém a opção anterior de substituir arquivo de progresso marcado como somente leitura; preferências não usam essa opção. Nenhuma permissão do sistema foi modificada ou testada.
- Nomes internos são componentes únicos, sem caminhos; nomes coincidentes, inclusive diferindo só em maiúsculas, são rejeitados antes do I/O. Diretório raiz é definido pelo adaptador, não por documento externo.

## Evidência

16/16 grupos de I/O isolado aprovados: rodada em disco, substituição, dados inválidos/truncados/excessivos, recuperação e padrões, arquivos bloqueados, temporário interrompido, falha de backup, ausência, escrita parcial, falha de commit, quarentena, criação de diretório recusada e nomes inválidos. Falhas de escrita parcial/criação/commit são injetadas no backend de QA; bloqueios são reais em arquivos descartáveis.

Regressão de sessão/codec aprovada antes dos ajustes finais de validação de nomes. A bateria de I/O foi repetida após esses ajustes: 16/16. Compilação Editor aprovada, sem executá-lo; preparação e verificação de 69 entradas aprovadas. Relatório LAB-QA-01C.json; logs locais build/lab-qa-01c-*.log.

## Limites e aceites

Os testes cobrem a mesma ordem de operações usada no projeto; não executam IFileManager em runtime. Não comprovam queda de energia, disco cheio, segurança adversarial, concorrência entre processos, durabilidade física, Android ou Shipping. Não foram alteradas ACL, rede ou configurações do sistema.

LQA-09/10 (salvar/reabrir via interface) permanecem pendentes. LQA-14 permanece adiado apenas no recorte do backend Unreal em ambiente isolado: o contrato compartilhado deixou de ser uma dependência futura. Não provocar falhas nos saves normais. Os demais aceites visuais LAB-PVE-02–06 também permanecem pendentes.

## Continuidade

1. Usuário: aceites positivos de progresso e preferências pela interface, em paralelo aos modos PvE, sem corromper arquivos pessoais.
2. Próxima frente independente proposta: contratos de áudio/animação sobre eventos existentes, com placeholders e sem exigir recursos finais. Inspecionar a implementação atual antes de propor mudanças para evitar repetir AUT-13.
3. Teste runtime do backend Unreal em pasta isolada exige roteiro/ambiente próprio e autorização de execução; não está implicitamente autorizado por esta entrega.

Demo ~28% (28,03 ponderado) mantida: melhoria de verificabilidade não equivale a aceite de plataforma ou conteúdo final. Sem telas, jogo, Android, internet ou push.
