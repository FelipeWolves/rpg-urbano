# ENG-01 — avaliação da engine, parte 1

18/11/2026. **Avaliação documental concluída; ENG-01 permanece aberto.** Não foi instalado nem aberto editor, alterado código de gameplay ou executado build da engine. Reutilizados os 211 testes anteriores; não repetidos por esta mudança documental.

## Recomendação para decisão

Avaliar **Unreal Engine 5.8.4 para Windows**, em experimento separado. É uma candidata, não escolha definitiva aprovada nem integração demonstrada. A publicação oficial do hotfix registra melhorias de integração com VS2026. [E1](https://forums.unrealengine.com/t/5-8-4-hotfix-released/2842775).

A 5.9 disponível na consulta é Preview. A própria Epic orienta testar previews em cópias e alerta sobre instabilidade. Não adotá-la como base inicial. A referência a desenvolvimento de UE6 não oferece uma migração garantida do nosso projeto nem justifica aguardar indefinidamente. [E2](https://forums.unrealengine.com/t/unreal-engine-5-9-megajam-preview/2852840).

O usuário confirmou não ter instalado o editor. Componentes do VS não o substituem. Não foi localizado editor pronto nas sondagens padrão; inventário público de instalações vazio. Não houve varredura de unidades, execução de launcher ou acesso a jogos/contas.

## Compatibilidade documentada versus medida

| Item | Evidência | Conclusão |
|---|---|---|
| VS2026 | 18.12 instalada; referenciada oficialmente | Plausível, ainda sem build UE |
| MSVC | 14.52 e14.29 instalados; recomendado14.50 | Mais novo não significa aceito pelo UBT |
| SDK | 10.0.26100.0 instalado/padrão documentado | Alinhado, confirmar seleção efetiva |
| .NET | Runtime10 presente, SDK11 instalado | Não instalar outro SDK sem verificar o empacotado |

Dados recomendados são das notas oficiais 5.8 indexadas: o corpo de algumas URLs de versão não ficou acessível na abertura. Confirmar os arquivos de requisitos da instalação exata antes de fixar ferramentas. Não suprimir erros do UBT nem trocar o toolset atual por suposição. Se14.52 não for aceito, avaliar instalação lado a lado do toolset recomendado com autorização. [E3](https://dev.epicgames.com/documentation/unreal-engine/unreal-engine-5-8-release-notes?lang=es-ES).

Hardware histórico: aproximadamente32GB, RTX3060 e16 processadores lógicos. Não são16 núcleos físicos demonstrados. VRAM segue desconhecida; não inferir pela identificação da GPU. Referência oficial inclui32GB de RAM e8GB de VRAM, mas não prova desempenho deste PC. Consulta CIM de atualização indisponível; histórico não foi apresentado como medição nova. [E4](https://dev.epicgames.com/documentation/unreal-engine/hardware-and-software-specifications-for-unreal-engine).

212,1GiB livres na unidade do projeto foram medidos por .NET. Destino da instalação não foi escolhido e o tamanho depende das opções; conferir antes do download. Registro do SO mostra build26300.9711/26H2 com nome legado divergente do inventário Windows11; não concluir canal ou alterar sistema por isso.

## Ajustes identificados no código — ainda não executados

- Manter regras, dados e progressão separados de objetos da engine. O núcleo usa C++20/STL e exceções; confirmar flags por módulo, capturar falhas na fronteira do adaptador.
- Compilar os fontes selecionados com o ambiente da engine. Não ligar os objetos atuais /MT como se fossem uma biblioteca ABI validada para o editor.
- No primeiro experimento, compilar unidades separadas e controlar PCH/macros; validar configuração de unity build. Não copiar regras de dano/AP para Blueprints.
- Excluir app, main, console, testes e utilitários do módulo de runtime. Compartilhar fontes por referência/etapa reproduzível, evitando duas cópias editáveis divergentes.
- Separar o codec de save do armazenamento Win32 e dos caminhos do console. A arena inicial pode trabalhar em memória; não usar build/saves como caminho definitivo de uma aplicação empacotada.
- Não serializar UObject/material/scene como se fossem o save v1 de progresso. Save permanece contrato próprio.

A documentação oficial descreve configuração por módulos e integração de bibliotecas. As páginas atuais resolveram para documentação5.9: servem para desenhar o ensaio, não para certificar propriedades/ABI da candidata5.8. Validar no UBT da versão instalada. [E5](https://dev.epicgames.com/documentation/en-us/unreal-engine/module-properties-in-unreal-engine), [E6](https://dev.epicgames.com/documentation/en-us/unreal-engine/integrating-third-party-libraries-into-unreal-engine).

## Roteiro priorizado depois da decisão

1. Confirmar versão aprovada, destino, tamanho e termos apresentados pelo fornecedor. Instalar somente editor/binários Windows necessários; sem conteúdo adicional, plugins externos, SDK mobile ou build da engine a partir de fontes por antecipação.
2. Ler Build.version e requisitos da instalação; verificar compilador e runtime aceitos, sem abrir editor ainda. Não alterar ferramentas globais ou desinstalar componentes existentes.
3. Em diretório de avaliação separado, gerar projeto C++ vazio e compilar Win64 Development Editor. Aceite: log de ferramentas usadas, zero modificação no núcleo oficial do projeto.
4. Conectar o núcleo por módulo/adaptador mínimo e executar uma ação com resultado comparável ao standalone, incluindo rejeição sem mutação. Aceite: um único estado autoritativo, sem reimplementação da regra na apresentação.
5. Somente com abertura do editor autorizada: cena vazia, ator/estado mínimo, iniciar/reiniciar, encerrar e repetir. Medir memória/tempo de abertura/compilação; depois testar pacote Win64 próprio. Não medir desempenho gráfico por nossos testes de console.
6. Confirmar escolhas ou registrar incompatibilidade e voltar ao protótipo intacto. VIS-01 só começa após o aceite ENG-01; mobile/lojas/launcher próprio são marcos posteriores.

## Permissões e limites

Esta parte ficou em leituras seletivas e fontes públicas oficiais, com relatórios no projeto. Escolha definitiva, instalação/download, abertura do editor e alteração de toolset exigem decisões específicas, conforme AGENTS.md. Nenhum desses passos foi executado.

A recomendação é versão5.8.4, editor Windows binário, projeto de avaliação isolado e primeiros testes de compilação/ponte. Não promete compatibilidade automática com UE6, gráficos finais, rendimento mobile, termos comerciais futuros ou custo/tamanho de instalação ainda não medidos. A licença da engine deverá ser lida/aceita pelo usuário antes da instalação/distribuição; nenhum compromisso comercial foi assumido aqui.

Resultados estruturados: ../relatorios/atuais/ENG-01.json e ENG-01-matriz.csv. Código e saves existentes preservados; nenhum novo teste funcional necessário nesta entrega documental.

## Instalação manual escolhida pelo usuário

Usuário informou que fará pessoalmente a instalação do editor. Não abrir launcher, iniciar download/instalador, aceitar termos ou modificar componentes por conta própria. Após conclusão, conferir apenas a versão e a pasta fornecidas, sem abrir editor; teste de compilação/abertura permanece etapa específica autorizada. Recomenda-se candidata UE5.8.4 Windows para o experimento; adoção final depende do teste.
