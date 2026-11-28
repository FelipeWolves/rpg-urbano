# Terminal 3D — câmera por setor e atalho 2D

**Data:** 28/11/2026

**Recorte:** fluxo provisório do RpgEngineProbe no molde atual. Compilado com sucesso em Win64, com reteste em jogo pendente.

Ao entrar no Terminal pelo Hub 3D, a câmera se aproxima pelo setor de origem. Os controles **Esquerda**, **Centro** e **Direita** permitem enquadrar o Terminal por três ângulos. A troca é suave e passa a ser imediata com Movimento Reduzido.

O botão **2D / F2** abre a mesma tela do Terminal 2D e compartilha os bilhetes do TerminalCore. **3D** no rodapé, **Esc** ou **F2** fecham o atalho. A vista 3D preserva o setor e a posição da lista. O botão de abertura recebe um destaque visual quando o jogador retorna. Durante uma sessão aberta pelo Monotrilho, a saída primeiro volta aos laboratórios e depois aos bilhetes do atalho.

## Roteiro de reteste

1. Entrar pelo Hub 3D e passar pelos três setores em janela larga e focada.
2. Em um setor lateral, abrir o atalho por clique e por **F2**. Rolar a lista 2D e fechar pelo rodapé, **Esc** e **F2** em tentativas separadas. Conferir o setor, a posição da lista 3D e o destaque de retorno.
3. Abrir uma sessão do Monotrilho pelo atalho e voltar até a visão 3D.
4. Repetir a transição com Movimento Reduzido. Validar por toque quando a frente Android for retomada.

O atalho usa o cenário provisório. A edição do mapa e o enquadramento com a cena autoral continuam em uma etapa própria.
