# AUT-15 — manifesto e stage de pacote

22/11/2026. Preparação isolada executada, não build/cook/jogo. `preparar-pacote-unreal.ps1` reuniu 53 entradas (fontes/headers, mapa, configuração, manifestos de cenário, catálogo, recursos autorais/fontes/licenças e dois vídeos) em build/p/79855ac5. O stage anterior foi preservado. Recursos continuam fora do Git.

Hash SHA-256 de cada entrada comparado origem/cópia; `verificar-stage-unreal.ps1` repetiu verificação independente de 53 entradas, fechamento dos includes rpg, limite de caminhos, decoder Win64 e plano ExecutionAuthorized=false. Não conclui que o executável já foi produzido nem faz análise de procedência pelo hash.

Três grupos de ensaio: stage válido, path que escaparia do limite recusado antes de acesso, hash divergente recusado. Casos negativos usam somente cópias JSON em build/aut15-verification; o stage não é alterado e o relatório válido é restaurado ao final. Relatórios AUT-15-STAGE.json/AUT-15-TESTES.json.

Detectado um detalhe no comando de invocação: LASTEXITCODE de comando nativo interno não representa sucesso de um script PowerShell. O stage havia terminado (PACKAGE_PREPARED_ONLY) apesar do guard externo indicar falha. Não foi repetida a cópia desnecessariamente: verificação independente confirmou a preparação. Orquestração AUT-16 usará processos PowerShell com saída explícita para evitar misturar os dois estados.

Pendências: build/cook/stage executável atualizado, validação de dependências em Windows limpo, reprodução empacotada/Shipping e ensaios de hardware. Nenhum editor, commandlet/cook, pacote, Android ou serviço externo foi executado. WmfMedia está restrito ao Windows e exige testes próprios no pacote; não é backend mobile.

O plano continua desautorizado para execução automática. Se houver mudanças futuras nos arquivos canônicos, o verificador acusa drift e exige novo stage; não sobrescrever hashes para mascarar a divergência.

Próximo AUT-16: seleção de verificações por área e roteiro de fumaça com evidências separadas de teste gráfico.
