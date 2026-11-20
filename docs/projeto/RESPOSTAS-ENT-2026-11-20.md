# Respostas ENT-01–15 — decisões recebidas

20/11/2026. Complemento autoritativo do usuário à revisão das12 entregas. Fonte original preservada em entregas/00-entrada-geral/recebidos/2026-11-20_respostas-ent01-ent15_v01.md, sem alteração. **Os 15 pontos estão respondidos quanto às decisões de produto; não significa implementação, teste ou contrato técnico completo.** Esta atualização prevalece sobre as alternativas/conflitos da revisão anterior nos temas decididos.

## Matriz vigente

| Ponto | Decisão recebida | Consequência na fila / estado |
|---|---|---|
| ENT-01 / REG-01 | Bloqueio completo por hit:3% base,Michel10% total,teto50% geral/100% especial. Mitigação percentual separada30–50% quando aplicável, combinada comDEF/Defesa | Respondido; novo sistema futuro, não alterar cenários antigos. Block técnico legado ainda significa impossibilitado de agir, não bloqueio probabilístico já implementado |
| ENT-02 / HAB/tutorial | FelipeA2 passiva, não selecionável; referência nova Postura de Vigília. TutorialA1, golpe recebido ativaA2, depoisA3 | Conflito ativa/passiva resolvido. Revisar ficha consolidada antes do kit: nome/gatilho de cargas/escudo mudaram em relação ao texto anterior |
| ENT-03 / ESC/SIS-01 | StandardShield1%/3naturais/mínimo1 intacto; Filhote15%/2naturais é gerador especializado futuroSilverShield | Não generalizar15% ao escudo padrão. Coexistência/permissões do tipo especializado precisam de contrato próprio |
| ENT-04 / VIS/HUB | ESC/Voltar:fechar painel→cancelar alvo→pausa→Retomar ou Abandonar. Abandono sem vitória/recompensa, preservando progresso salvo. Seleção/confirmar em2passos;hold só consulta | AUT-03/08 podem especificar novo fluxo. Retorno ao hub que antes preservava a mesma tentativa não é abandono; ação explícita precisa diferenciar os dois comportamentos |
| ENT-05 / HUB | Mesma imagem; foco no humano/dispositivo,Batalha; pan horizontal e/ou âncora responsiva; Evento alcançável por pan ou atalho seguro | Não exigir2 imagens nem ambos os objetos na mesma janela recortada. AUT-02 pode testar pan/atalho sem deformação; composição final requer aceite |
| ENT-06 / REG-03 | CarrierTurnEnd só natural. Supressão bloqueia reativas/auto-revive,exceto mortes já enfileiradas e auras estruturais do chefe. Freeze também bloqueiaPursuit; oportunidade perdida decrementa duração | Orienta família de controles; precisa ordenar aplicação/remoção e captura de eventos comR04. Não alterar retrospectivamente relógios de efeitos históricos fora do novo contrato |
| ENT-07 / SIS-02 | Piso1 em golpe comum não anulado;Defesa aplica floor(dano/2) depois, permitindo0. TrueDamage ignoraDEF/mitigação percentual/Defesa, mas respeita escudo. FunilImunidade→Miss→Block→Crítico→Mitigação→Escudo→HP | Conflito de piso/Defesa resolvido. Novo pipeline ainda não codificado; não basta mudar ordem do resolvedor atual sem contrato de chance/debuffs/RNG |
| ENT-08 / WAVE/score | Entre ondas:curar vivos30%MaxHP comfloor/cap; não reviver;remover buffs comuns;preservar especiais/auras/cargas/CD conforme nova política. Chefe respeitaR04. Scorefloor(efetivo+25%overkill)+bônus,int64/teto/idempotência | É extensão/mudança explícita de política a implementar em bloco próprio; não declarar WAVE atual já comcura. Preservar remoçãoESC e definir categorias dos efeitos antes de migração |
| ENT-09 / HUB/AUD | Primeira exibiçãohold0,8s;rever1clique;retoma pausado;6ºcanalMídia ou provisórioMúsica. IntroWatchedtrue ao terminar/skip,não porfalha | AUT-10/11 liberados para especificar esses estados; preferências/flag devem funcionar localmente sem conta no laboratório |
| ENT-10 / economia | Estamina+1/min,teto100–300;DJ entrega pacotes diretos,não acelera regeneração. Duplicação anterior obsoleta | Divergência textual resolvida; valor exato do teto/progressão é parametrização futura, não economia implementada |
| ENT-11 / demo | SóFelipe jogável completo contraEco de Dissonância,naarenaTerraço daVigília;demais4nomes no catálogo narrativo | Recorte da primeira entrega artística/kit/tutorial fechado; não bloquearMVP por5 modelos/kits |
| ENT-12 / CIC | Progresso:nuvem validada pelo servidor prevalece;preferências locais por dispositivo;sem snapshot noSavev1/MVP | Frase incompleta complementada; política cloud futura. “Mais recente” deve ser revisão/transação do servidor,não relógio enviado pelo cliente |
| ENT-13 / VIS/HUB | Preservar runtime/paleta e fontes atuais,semrecoloração imediata. Temas narrativos/jogador/evento ficam para depois | AUT-07 pode preparar opções sem aplicar paleta nova; preservarJSON atual inclusive acentosmagenta/ciano/âmbar/carmesim. Descrição textual não reescreve cores executáveis |
| ENT-14 / clássico/DJ | FuturoDefender+15%SP;ATBWaitpausa submenus/Activecontinua;rankings separados por3/4/5pistas | Registrado apenas como especificação futura;Defesa atual não ganhaSP,ATB/DJ não existentes não ficam habilitados por configuração |
| ENT-15 / produção | Servidor emiteSeed/AttemptID;cliente envia comandos/inputs;servidor reexecuta antes de recompensa/ranking | Direção de autoridade registrada;nenhumbackend ou anticheat implementado. Seed/replay exigem validação de entradas,versões e transações; não garantem segurança isoladamente |

## Detalhes técnicos para fechar no bloco afetado

As escolhas principais não precisam ser perguntadas novamente. Antes de implementar, formalizar apenas os detalhes abaixo; sem questionário ou aprovação pendente agora:

1. **Ficha de Felipe — regra principal respondida:** [Postura de Vigília](FELIPE-A2-POSTURA-VIGILIA.md):1carga por aliado atingido por habilidade, limite pela capacidade do modo, consumo total, A1 no autor e StandardShield no aliado vivo com menor percentualHP incluindo Felipe. Fórmula/duração/quantidades já definidas; somente casos de borda enumerados no contrato aguardam formalização para implementação.
2. **Dano/controles — ponto2 respondido:** [bloqueio/mitigação/True Damage](BLOQUEIO-MITIGACAO-DANO-PONTO2.md): Block3% (Michel10% total), tetos50%/100% explícito; mitigação só por fonte, aditiva até50%; True Damage e DoT não bloqueáveis. True Damage ignora DEF/mitigação/Defender, respeitando invencibilidade/escudo. Restam em contratos separados chances/debuffs/RNG, famílias DoT e bordas de controles; não repetir elegibilidade ou origem da mitigação.
3. **Ondas/escudos:** quais estados são comuns/especiais nos dados,ordemcura/limpeza,inimigos/aliados afetados,AP e demais políticas preservadas. SilverShield precisa permissões/coexistência e regra dereaplicação;cor não deve herdar comportamento implicitamente. Score precisa teto/bônus e momento dofloor (acúmulo ouporhit) definidos pelo modo.
4. **Interface/mídia:** foco/teclado e suspensão da automação na pausa;abandonar não aciona recompensa nem abre nova tentativa sem intenção. Mapear flagprimeiraabertura para perfil local;distinguir revermanual e retryapóserro. Rotação/áudio/ciclo devida exigem teste por plataforma.

## Retomada sem repetir análise

Quando o usuário informar disponibilidade de limite: AUT-01→02→03→04 com recursos atuais; AUT-07/08 seguem as decisões de pausa/mídia sem habilitar serviços ou menus não implementados. Inventariar somente arquivos que mudaram. SDK/NDK/JDK/AVD continuam pendentes da conferência documentada em ANDROID-DEPENDENCIAS.md; este adendo não informa versões/caminhos novos nem autoriza instalações.

ESC/R04 e testes históricos permanecem válidos no escopo anterior. Novas regras (curaentreondas,bloqueio,TrueDamage,SP) deverão ganhar implementação/versão e regressão próprias. Seletor continua aceito. Nenhum código,catálogo executável,save ou recurso original foi modificado nesta recepção;percentuais mantidos.

Complemento ponto3: dois trilhos,basecrítica e escala/teto definidos em [FUNIL-ESCALA-NUMERICA-PONTO3.md](FUNIL-ESCALA-NUMERICA-PONTO3.md). Substitui a indefinição desses temas;equaçãoACC/RESIST completa,elegibilidadeporMiss/Block e tratamento acima do teto ficam por formalizar no bloco técnico.
