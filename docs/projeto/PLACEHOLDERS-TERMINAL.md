# Placeholders, seletor e escala do Terminal

22/11/2026. Retorno do usuário após fc31f3d: vídeo e skip corretos, batalha do Felipe até o fim sem crash e recargas corretas; esferas ausentes e seletor escuro/fino nos dois cenários.

## Diagnóstico e ajustes

A captura fornecida mostra SetStaticMesh recusado por mobilidade estática nos atores criados em execução. As esferas agora recebem Movable antes de SetStaticMesh. Componentes e raiz do marcador também têm mobilidade explícita antes do registro/movimento.

Seletor ampliado de 3 para 8 unidades de largura e de 1 para 2,5 de espessura. Material emissivo local da própria engine M_Emissive_Color, parâmetro Color conferido no asset; cor âmbar. Referência explícita no construtor para descoberta de dependência. Continua sendo geometria no chão, sem debug renderer nem plano sobre a silhueta. O material de ferramentas é provisório: validar cook/Shipping e substituir por material autoral de produção quando autorizado; nenhuma validação visual ou cook foi feita aqui.

Novo terminal_lab_setup deriva do cenário de equipe com FunnelV1. PV máximos multiplicados por 100 (8.000/10.000/14.000), ATK por 25 (375/500/625). Assim é possível observar mais turnos sem alterar fórmulas, duração, porcentagem de escudo, habilidades ou teto de ações. Escudos próprios passam a 80/100/140 pontos, ainda 1% da vida máxima; a faixa proporcional não é artificialmente ampliada. team_reference_setup pequeno continua intacto para laboratório e regressões. Piloto Felipe e perfil clássico preservados. Isso é balanceamento de teste, não aprovação de balanceamento final.

## Mensagens do editor

- SetStaticMesh com mobilidade estática: aviso relevante, tratado nesta correção.
- Controle de revisão desabilitado: estado da integração de controle de versão do editor; não prova falha no Git externo. Não habilitado/alterado.
- Localização desabilitada: texto exato não consta nas capturas entregues; sem diagnóstico ou alteração por inferência. Não impede por si só registrar os resultados funcionais relatados.

## Aceites e retestes

Aceites informados: cliques únicos, hover, vídeo/skip, conclusão da batalha Felipe sem crash e recargas no cenário testado. Não reabrir por rotina. Novo reteste: três esferas na Arena local, seis no Terminal; seletor âmbar mais espesso; PV ampliados e painel de escudo. Estabilidade só confirmada nos testes realizados pelo usuário, não garantia global.

Testes nativos Pilot 10/10 e alternância 2/2, incluindo nova escala e preservação do baseline pequeno. Build/stage em PLACEHOLDERS-TERMINAL.json. Editor detectado aberto: compilação aguardou confirmação de fechamento pelo usuário. Nenhuma tela ou jogo foi aberto pelo assistente; Android/rede/instalações continuam fora do escopo. Demo ~25% mantida.
Compilação aprovada em 45,46 s; stage66 verificado, sem cook. Saves v1 não guardam PV/AP/status em andamento, portanto não foi criada migração ou apagado save para a escala visual.
