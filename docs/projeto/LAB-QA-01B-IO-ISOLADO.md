# LAB-QA-01B — gravação em laboratório isolado

## Escopo entregue

Bateria Windows nativa, sem Unreal, com codecs reais `encode_save`/`import_save` e `encode_preferences`/`read_preferences`. Testes em `tests/save_io_lab_tests.cpp`; execução por `scripts/testar-save-io-isolado.ps1` ou área `SaveIO` do verificador por área.

Cada execução cria `build/qa-save-io/run-<GUID>`, inicialmente vazio. Nomes de arquivos são fixos, sem entradas de caminho fornecidas por anexos. A pasta normal Saved não é lida nem alterada. O script recusa junções nos caminhos de QA examinados. Os arquivos descartáveis permanecem para diagnóstico, ignorados pelo Git; não há limpeza recursiva automática.

## Casos automatizados

| Caso | Verificação |
|---|---|
| IO-01 | Gravar progresso, ler do disco e importar equipe |
| IO-02 | Substituir arquivo existente por temporário completo |
| IO-03 | Arquivo corrompido rejeitado sem alterar estado em memória |
| IO-04 | Truncamento e tamanho acima de 1 MiB rejeitados |
| IO-05 | Preferência inválida recupera valores de backup válido |
| IO-06 | Primário e backup inválidos retornam padrões |
| IO-07 | Arquivo aberto com exclusividade impede substituição; original e temporário preservados |
| IO-08 | Temporário interrompido não substitui progresso válido; nenhum backup de progresso é inventado |
| IO-09 | Backup bloqueado impede cópia e preserva arquivos |
| IO-10 | Arquivo ausente produz erro de leitura sem alterar arquivo válido |

## O que esta evidência permite afirmar

Há I/O real em arquivos de QA no Windows, com bloqueios de arquivo e recuperação pelos codecs reais. O executável usa backend de teste próprio: streams C++/filesystem e MoveFileExW, não IFileManager/FFileHelper. Portanto não comprova a integração da interface Unreal, a implementação interna de Move da engine ou o comportamento de outro sistema operacional.

Não foram simulados disco cheio, falta real de energia, alteração de ACL, corrupção de saves pessoais ou perda de durabilidade física. Não há certificado de atomicidade sob queda de energia. Não adiciona backup automático ao progresso nem promove arquivos temporários no jogo. Não muda formato v1, regras, saldo ou personagens.

## Próximo corte independente

LAB-QA-01C: centralizar a orquestração de persistência em contrato testável compartilhado, mantendo o backend Unreal separado; somente então os testes de falha poderão cobrir diretamente a mesma lógica de commit da aplicação. Não ampliar a política de backup ou recuperação de progresso por inferência.

Em paralelo, usuário pode validar LQA-09/10 na interface e os modos novos. LQA-14 continua adiado no recorte de integração Unreal; o laboratório nativo desta entrega é evidência complementar, não aceite automático. Android permanece adiado. Demo ~28% (28,03 ponderado) mantida.

## Resultado registrado

10/10 grupos aprovados, execução pelo verificador por área com código de saída zero. Plano de QA validado separadamente. Relatório: docs/relatorios/atuais/LAB-QA-01B.json. Nenhuma recompilação Unreal necessária: não houve mudança de código runtime, stage ou configuração do jogo.

> Atualização LAB-QA-01C: contrato compartilhado implementado/testado e adaptador Unreal compilado; ver LAB-QA-01C-PERSISTENCIA-COMPARTILHADA.md. A indicação anterior de contrato ainda futuro é histórica. Aceites reais de interface continuam abertos.
