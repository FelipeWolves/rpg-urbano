# VIS-01C — fluxo3×3 verificado no pacote; aceite visual parcial

19/11/2026. **✅ Fluxo funcional3×3 aprovado fora do editor. ⚠️ VIS-01B/C integral ainda aberto.** Não é aceite de todos os modos, formações, resoluções ou acabamento.

## Execução

Pacote Development/Win64 revisado em `build/p/dcecd963/archive/Windows`. Novo stage de28 entradas, sem substituir o pacote básico anterior. Dois empacotamentos nesta entrega passaram com saída0. A primeira inspeção apontou contraste/etiquetas sobrepostas e texto de habilidade extrapolando o botão; foram corrigidos altura/contraste e separação de nome/estado, com nova compilação e pacote. Logs e primeira tentativa preservados nos JSONs INICIAL.

A versão revisada foi inspecionada em1280×720, -culture=en, sem editor gráfico. Testes manuais por clique no aplicativo, com observação a cada ação. Não houve automação por comandos dentro da interface nem modificação de HP/resultado para acelerar o teste.

## Matriz observada

| Caso | Resultado |
|---|---|
| Cena/lados/câmera/rótulos | Formas simples em lados opostos, vida e ator acompanhando a posição |
| Alvo aliado com ataque | Recusado, ator e ações inalterados na primeira versão; mesmo caminho de núcleo preservado |
| Habilidade de posição2 | Executada, dano refletido no estado visual e resposta automática do inimigo |
| Recarga | Número separado do estado de condição, com básica disponível |
| Passiva pura A2 | Não selecionável; clique não avança a batalha |
| Condição A3 | Inicialmente bloqueada, liberada ao atingir40/80HP; proteção em si próprio aplicou escudo |
| Derrota de unidade | HP zero e esfera ocultada; rótulo zero preservado |
| Resultado | Team A wins após20 ações aceitas, sendo14 manuais registradas e6 respostas automáticas |
| Entrada após vitória | Clique na antiga área de habilidade não alterou resultado/contadores |
| Reinício após vitória | Unidades restauradas, ator A3, oportunidade1 e ações0; VIS_RESET no log |
| Encerramento | Alt+F4, janela desapareceu, bootstrap e processo principal com saída0 |

Log final sem linhas Error:/Fatal/Critical ou Warning:. Sem marcadores de requisição de telemetria procurados; isso não comprova ausência de tráfego. Rede/firewall inalterados conforme decisão do usuário. Dados e preferências de teste separados em build/vis-runtime; sem uso de saves do protótipo. Não foi feita auditoria integral de escritas do sistema.

## Pendências antes do aceite integral

1. Texto de HP/escudo ainda se aproxima ou sobrepõe no rótulo ativo A2 com140/140HP. Separar linhas ou reservar largura medida.
2. Nomes longos foram abreviados para caber. Falta descrição completa/motivo específico, sem tornar passivas selecionáveis.
3. A UI ainda permite apontar alvos candidatos inválidos e depende da recusa do núcleo; aprimorar indicação e explicação, mantendo a autoridade do núcleo.
4. Formação1×1 e5×5, resoluções menores/maiores e toque não foram inspecionados. A cena hoje cria seis esferas fixas; generalizar a criação a partir do elenco antes desses testes.
5. Não demonstrados replay visual, catálogo amplo, animações, efeitos, seleção de equipe, saves, SEQ/PAR ou localização final. Formas e materiais são provisórios; não são personagens do jogo de referência.

## Evidências e limites

- [Build revisado](../relatorios/atuais/VIS-01C-BUILD.json).
- [Execução e matriz resumida](../relatorios/atuais/VIS-01C-RUNTIME.json).
- [Saída do processo principal](../relatorios/atuais/VIS-01C-PROCESSO.json).
- [Build inicial](../relatorios/atuais/VIS-01C-BUILD-INICIAL.json) e [inspeção inicial](../relatorios/atuais/VIS-01C-RUNTIME-INICIAL.json).

Relatórios ENG-01 anteriores preservados. Script de empacotamento agora aceita nome restrito de relatório para não substituir resultados do pacote básico. Compilação manteve avisos MSVC/C4864 e cook manteve avisos ACL já conhecidos, sem alterar/suprimir a engine. A HUD usa Canvas, não copia arte/interface proprietária e não instala plugins.

213 testes gerais e6 do adaptador permanecem evidência standalone de VIS-01A; não foram repetidos nesta entrega porque regras/adaptador não mudaram. Os cliques observados são a evidência de integração desta cena. Não é benchmark de desempenho, validação de Windows limpo, distribuição pública, mobile ou idioma português. Inglês no ambiente não restringe localização futura do produto.

Próxima entrega recomendada: corrigir legibilidade e criação de unidades baseada no elenco, validar1/3/5 e redimensionamento, então fechar VIS-01B/C no escopo declarado. Não avançar automaticamente para arte/áudio/produção enquanto este aceite permanecer parcial.
