> **Relato mais recente — 22/11:** abertura do vídeo confirmada pelo usuário com um clique. RET-MEDIA-01 fica aceito nesse recorte; não comprova backend mobile ou todos os fluxos de pausa/skip. Demais botões, menus e batalha continuam exigindo dois cliques. Skip sem feedback visível: novo reteste RET-MEDIA-02.
>
> **Correção complementar:** trocar NoCapture por CaptureDuringMouseDown, sem prender/ocultar cursor. A engine só encaminha o pressionamento primário quando há captura, captura temporária ou modo permanente incluindo primeiro clique. O polling do controlador dependia desse encaminhamento. Acrescentados hover e progresso de 0,8 s ao skip. RET-INPUT-01 e RET-MEDIA-02 continuam pendentes de teste real do usuário; compilação não é aceite.
# Retestes do usuário — entrada e vídeo

22/11/2026. Relato após piloto Felipe: sistema funcional, mas cliques continuam estranhos; batalha exige duplos cliques e não permite trocar alvos; vídeo também exige duplo clique para funcionar.

| ID | Estado | Conferência posterior do usuário |
|---|---|---|
| RET-INPUT-01 | REABERTO / não aceito | Escolher habilidade de alvo único; selecionar inimigoA; trocar paraB antes de confirmar; confirmar se seleção acompanha o clique e se o comando exige clique adicional inesperado. Separar habilidade em área, que atinge todos, da seleção visual |
| RET-MEDIA-01 | ACEITO pelo usuário: início com 1 clique | Primeiro Rever abertura após iniciar; fechar/rever novamente; anotar se primeiro acesso exige dois cliques e se os seguintes usam um |

Se possível registrar atalho/cenário, modo janela/PIE, resolução/escala e sequência. Não é necessário gravar vídeo ou investigar implementação. Aceite anterior do alinhamento do ponteiro não comprova estes fluxos. O primeiro clique da mídia deixa de ser considerado aparentemente resolvido: permanece pendência confirmada pelo novo relato.

Atualização posterior nesta mesma data: investigação e correções de código autorizadas em VIS/ENG/HUB P0; testes nativos aprovados. Retestes permanecem REABERTOS e não aceitos visualmente. Ver VIS-ENG-HUB-P0-CORRECOES.md; compilação não substitui reprodução real.
