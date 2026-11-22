# REG-01-B — funil integrado por modelo explícito

22/11/2026. CombatModel::FunnelV1 opt-in em BattleRules, Legacy continua padrão para cenários anteriores. Stats efetivos explícitos na Definition; ausência de DEF configurada não inventa um atributo de produto. Block padrão do modelo novo 3%, crítico configurável (multiplicador padrão150%); seed/revisões/hits/alvos fornecem chaves ao RNG v1.

Escopo: Invencibilidade→Miss→Block→crit→mitigação→escudo→HP, preservando Defesa floor e absorção integral sem dano residual inventado. A fórmula DEF recebida (1800/(570+DEF efetiva após ignore)) e DamageMod de bônus/redução são representados por fatores racionais; Miss usa75% conforme ficha recebida e não gera carga de atingido. True Damage ignora DEF/redução/Defesa/Block e crit normal, mas mantém bônus de dano e respeito a escudo/invencibilidade. Catálogo de DoTs ainda não é generalizado por essa função.

Produto de fatores mantém inteiros naturais internos em base2^32, limitado a16 fatores; piso e saturação ocorrem no resultado final. Isso evita erro de saturar antes de redução/Defesa. DEF e multiplicadores não usam double. Intermediários de até cerca de1071bits são suportados pelo limite explícito, sem biblioteca externa; não é parser de aritmética arbitrária de usuário.

Event.hit opcional carrega outcome, dano tentado, absorção, dano no HP e overkill. Só o hit original recebe o relatório; reflexão/explosão legadas não herdam seu outcome. Falha inicial de assinatura/chamada detectada pelo compilador foi corrigida antes dos testes. Canais são separados, eventos anteriores mantêm hit nulo no Legacy. Counters novos exigem contato elegível; reflexão legado foi preservada inclusive nos casos de zero da Defesa.

Dez grupos de pipeline aprovados: exemplo8910 da ficha, True Damage/bônus/invencibilidade, piso/Defesa, Miss, produto com redução após valor grande, cadeias largas e casos pequenos, integração de evento/escudo/block, replay e Legacy preservado. Regressão geral completa aprovada e Editor compilado18ações, saída0; log build/reg01-b-build.log. Windows não comprova replay ARM/iOS, não houve engine em execução nem player.

Próximo REG-03: controles, debuffs e revalidação de reações; depois passiva e piloto Felipe. Novas decisões Bleed/Poison/slots/cargas registradas em documentos próprios. Stats/elementos de produto, equipamentos, todas as famílias DoT e score ainda não são inferidos deste modelo.
