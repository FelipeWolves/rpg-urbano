# Roteiro de pesquisa — batalha, tempo e modos (Preenchido)

Preparado em 18/11/2026, após TIME-02. Documento preenchido e auditado com base nas regras observáveis do **Dislyte** (wikis oficiais, notas de atualização da Farlight/Lilith Games e guias técnicos da comunidade) e decisões de arquitetura recomendadas para o nosso projeto. Relaciona as perguntas aos IDs de `PENDENCIAS.md`, que permanece como registro central do projeto.

## Como registrar as respostas

Você não precisa responder tudo de uma vez. Priorize P1, depois P2. P3 trata de extensões futuras. Para cada resposta, informe o ID Q, modo/evento, personagem/habilidade/equipamento, versão ou data aproximada e a evidência. Caso a regra varie entre modos, registre respostas separadas.

Uma descrição de habilidade confirma o que ela declara, mas pode não explicar ordem de eventos ou exceções. Vídeo com sequência completa e contadores visíveis ajuda a distinguir ação de turno. Relato sem evidência ainda é útil: identifique como lembrança ou hipótese. Uma gravação isolada não comprova regra universal.

Separe dois campos: **“o que encontrei no Dislyte”** e **“o que desejo para o nosso jogo”**. Uma preferência nossa pode ser adotada mesmo sem confirmação da referência. Não precisamos descobrir detalhes internos de implementação para definir comportamento observável.

## O que já está validado no nosso projeto

49 testes aprovados; 168 cenários anteriores e 25 formações AP com replay. Implementamos barra por velocidade, extras, contra-ataques mínimos, bloqueio, durações/recargas do dono, guarda opcional e fases com políticas explícitas.

Esses resultados confirmam nossas regras atuais. **Não confirmam que as mesmas regras, fórmulas e exceções sejam utilizadas no Dislyte.** Não é necessário repetir os testes locais na sua pesquisa.

---

## P1 — Tempo, efeitos e recargas

### Q01 — O que incrementa o contador geral? (PEN-12)

**No nosso projeto:** contamos comandos principais aceitos, incluindo resolução de oportunidade bloqueada; contra-ataques têm contador separado. Não implementamos o contador geral específico de cada modo.

#### Resposta Q01 — Incremento do contador global de turnos

- **Modo/evento e fase:** Ritual Miracle (Kronos, Apep, Fafnir, Yinglong/Andras), Spatial/Temporal/Infinity Tower, Desolate Lands e Point War.
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** Set **Astralis** (4 peças — turno extra), set **Avatara** (2 peças — contra-ataque), set **Relief** (25% de assistência quando aliado usa habilidade básica), **Stun / Freeze / Petrification**.
- **Versão/data aproximada:** Regras consolidadas do sistema de combate (2022–2026).
- **Fonte/link ou vídeo e minuto:** Dislyte Wiki (*Ritual Miracle*, *Status Effects*) e guias de combate (*One Chilled Gamer* / *PlayDislyte*).
- **Tipo de evidência:** Descrição oficial + observação consolidada da comunidade.
- **Situação inicial e contadores visíveis:** Batalha com contador global de turnos exibido no topo da tela (progressivo ou regressivo conforme o modo).
- **Sequência observada:**
  1. Unidade atinge $100\%$ de AP e inicia turno natural $\rightarrow$ contador incrementa $+1$.
  2. Aliado dispara assistência (ex.: set **Relief** ou passiva de **Yun Chuan / Screamer**) ou inimigo dispara contra-ataque (set **Avatara**) $\rightarrow$ contador **não altera**.
  3. Unidade recebe **turno extra** (set **Astralis** ou habilidade) $\rightarrow$ inicia um novo turno e incrementa $+1$.
  4. Unidade incapacitada por **Stun**, **Freeze** ou **Petrification** atinge $100\%$ de AP $\rightarrow$ assume sua janela de turno, perde a ação e incrementa $+1$.
- **Resultado e valores finais:** Cada turno individual (natural ou extra, agindo ou incapacitado) soma $1$ ao contador; reações fora de turno somam $0$.
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** O contador mede **turnos individuais de unidades na barra de AP**, não rodadas completas da equipe. Aliados, inimigos e chefes incrementam o mesmo contador ao assumir o turno. Em fases multi-ondas tradicionais, o contador acumula entre ondas.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** A documentação escrita das wikis não especifica se invocações temporárias em encontros específicos alteram o contador de forma distinta de Espers comuns; recomenda-se validar em vídeo caso invocações entrem na barra de AP.
- **O que desejo para o nosso jogo:** Incrementar o contador geral em $+1$ a cada **turno natural** ou **turno extra** aceito (inclusive sob controle rígido), mantendo assistências, perseguições e contra-ataques com custo $0$ no contador global.

---

### Q02 — Como AP, SPD e desempate realmente interagem? (PEN-07)

**No nosso projeto:** barra inteira; selecionamos maior AP, depois velocidade efetiva e ID; zeramos a barra do turno natural escolhido. São escolhas próprias.

#### Resposta Q02 — Interação entre AP, SPD e critérios de desempate

- **Modo/evento e fase:** Todos os modos de combate.
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** Sets **Wind** ($\text{SPD} +25\%$) e **Light** ($+10\%$ AP ao expirar imunidade inicial), **Tiye** (manipulação/troca de AP), **Ashley** ($+10\%$ AP aliado em crítico).
- **Versão/data aproximada:** Regras consolidadas (2022–2026).
- **Fonte/link ou vídeo e minuto:** *What are the precise mechanics of AP and Spd?* (r/Dislyte), *Answering ALL Your Speed/AP Questions!* (DaddyF2P) e *Dislyte Beginner's Guide*.
- **Tipo de evidência:** Guia técnico da comunidade + descrição oficial.
- **Situação inicial e contadores visíveis:** Unidades com diferentes valores de SPD avançando na barra de AP ($0\%$ a $100\%$).
- **Sequência observada:**
  1. Internamente, a barra funciona como uma escala contínua (modelada pela comunidade como $0$ a $1000$ pontos, com casas decimais de SPD), exibida como $0\%$ a $100\%$.
  2. A cada tick de tempo, cada unidade acumula AP proporcional à sua **SPD efetiva** (uma unidade com $200\text{ SPD}$ completa duas voltas enquanto uma de $100\text{ SPD}$ completa uma).
  3. Buffs/debuffs de **SPD** (e bônus de líder/set *Wind* sobre a SPD base) **não alteram o AP já acumulado**, apenas mudam a taxa de preenchimento nos ticks seguintes.
  4. Manipulações de AP ($+20\%$ AP, $-30\%$ AP) somam ou subtraem valores percentuais da **barra inteira**, nunca do AP atual.
  5. Em empate exato de SPD/AP no início do combate, a preferência segue a ordem da formação **da esquerda para a direita** ($\text{Slot } 1 \rightarrow \text{Slot } 5$).
  6. Ao atingir o topo e agir, o AP do agente é zerado para $0\%$.
- **Resultado e valores finais:** Corrida determinística por SPD efetiva com preservação do progresso percentual da barra após alterações de velocidade.
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** AP e SPD são variáveis ortogonais: AP mede a posição atual na pista; SPD mede a taxa de avanço. Mudanças de AP nunca interrompem uma habilidade que já está em execução.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** A documentação pública não confirma se o excedente acima de $100\%$ (*overflow* de AP após múltiplos boosts simultâneos) é mantido internamente para desempatar unidades que cruzam $100\%$ no mesmo instante ou se é truncado em $100\%$ desempatando pela SPD/slot. Chefes e estados especiais (ex.: **Anna - Synergy**) possuem imunidade explícita a `AP Down`.
- **O que desejo para o nosso jogo:** Permitir **overflow interno acima de $100\%$** para desempatar boosts simultâneos, zerando a barra ao iniciar o turno natural. Desempatar por: $\text{maior AP} > \text{maior SPD efetiva} > \text{lado atacante (PvP)} > \text{índice do slot na formação } (1\text{ a }5)$. Conservar o progresso atual da barra sempre que a SPD for alterada.

---

### Q03 — Quais tipos de turno extra existem? (PEN-07/PEN-09)

**No nosso projeto:** extras entram numa fila antes da próxima seleção natural e preservam o AP dos demais. O relógio de cada habilidade/status define se o extra reduz sua duração/recarga.

#### Resposta Q03 — Turnos extras de equipamento vs. habilidade

- **Modo/evento e fase:** Todos os modos de combate.
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** Set **Astralis** (4 peças), **Gaius** (*God King*), **Tetsuya R6** (*Umbrage - Morph 2*: primeiro abate concede turno extra).
- **Versão/data aproximada:** Regras consolidadas pós-rework de Astralis (2023–2026).
- **Fonte/link ou vídeo e minuto:** Dislyte Wiki (*Ritual Miracle — Astralis Set*) e *Patch 3.4.26 Details* (Farlight Games).
- **Tipo de evidência:** Descrição oficial.
- **Situação inicial e contadores visíveis:** Esper realiza ação que dispara condição de turno extra (ex.: $25\%$ do set Astralis ou abate com Tetsuya R6).
- **Sequência observada:**
  1. Ao encerrar a ação atual, o turno extra é inserido imediatamente antes do próximo avanço natural da barra de AP.
  2. O AP dos demais personagens permanece congelado enquanto o turno extra ocorre.
  3. **Diferença crítica entre fontes:**
     - *Turno extra por equipamento (**Astralis**):* Pode disparar **apenas uma vez** por turno natural (não encadeia em si mesmo) e **não afeta recargas de habilidades** (*"Can only be triggered once. The extra turn does not affect ability cooldowns."*).
     - *Turno extra por habilidade (ex.: **Gaius / Tetsuya R6**):* Funciona como turno pleno da unidade, reduzindo recargas e durações de status normalmente.
- **Resultado e valores finais:** Turno extra executado com prioridade imediata, porém com regras distintas de recarga conforme a origem (equipamento vs. habilidade).
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** Nem todo turno extra é igual no Dislyte: o turno extra de equipamento (*Astralis*) tem trava anti-loop e **não avança cooldowns**, ao passo que turnos extras de habilidades avançam o relógio próprio do Esper.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** As descrições oficiais não detalham a ordem de prioridade caso duas fontes distintas concedam turnos extras simultaneamente na mesma ação (ex.: habilidade + equipamento); normalmente o efeito da habilidade resolve antes do check de fim de turno do set. Se o beneficiário morrer ou a onda encerrar antes do turno extra iniciar, o extra pendente é descartado.
- **O que desejo para o nosso jogo:** Criar uma fila prioritária de turnos extras antes da próxima leitura de AP (congelando o AP dos demais) e suportar explicitamente duas flags por fonte: `ConsumesCooldownAndDuration` (`false` para equipamentos como *Astralis*; `true` para habilidades) e `MaxProcsPerNaturalTurn = 1` para equipamentos.

---

### Q04 — Quando uma recarga começa e diminui? (PEN-07/PEN-09)

**No nosso projeto:** habilidade usada recebe a recarga configurada; ela não diminui nesse mesmo encerramento. Diminui ao terminar oportunidades futuras do dono conforme NaturalOnly/EveryTurn.

#### Resposta Q04 — Fronteira de recarga, Petrificação vs. Stun/Freeze e manipulação

- **Modo/evento e fase:** Todos os modos de combate.
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** Habilidades ativas com recarga (ex.: **Phobetor — Howling Nightmare**, Cooldown: 5 turns), set **Ocean** ($35\%$ de chance de reduzir recargas em $-1$ turno no início do turno), **Tevor** (*Neko-Sense*: reseta recargas no início do turno), **Petrification** vs. **Stun/Freeze**.
- **Versão/data aproximada:** Regras consolidadas (2022–2026).
- **Fonte/link ou vídeo e minuto:** Dislyte Wiki (*Ritual Miracle — Ocean Set*, *Status Effects/Petrification*, *Status Effects/Golden*) e discussões técnicas da comunidade.
- **Tipo de evidência:** Descrição oficial + guia da comunidade.
- **Situação inicial e contadores visíveis:** Habilidade com **Recarga: 3 turnos** disponível no início do turno do dono.
- **Sequência observada:**
  1. **Turno 1 do dono:** Usa a habilidade de recarga $3$. Ao encerrar esse turno, o ícone exibe $2$ turnos restantes (o turno de uso já descontou $1$).
  2. **Turno 2 do dono (normal, Stun ou Freeze):** Na virada do turno do dono, o contador reduz de $2$ para $1$. Se o set **Ocean** ativar no início do turno ($35\%$ de chance), reduz mais $-1$ e libera a habilidade imediatamente.
  3. **Turno 2 do dono sob Petrificação (`Petrification`):** O alvo perde a ação **e o contador permanece travado em $2$**, pois *Petrification* bloqueia a redução de recargas (*"targets cannot reduce their ability cooldowns while petrified"*).
  4. **Turno 3 do dono (sem petrificação):** O contador reduz de $1$ para $0$, liberando o uso da habilidade.
- **Resultado e valores finais:** Sequência normal: $3\text{ (uso)} \rightarrow \text{exibe } 2 \rightarrow \text{exibe } 1 \rightarrow 0\text{ (pronta)}$. Sob Petrificação, o turno petrificado não decrementa o contador.
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):**
  - **Stun, Freeze e Petrification NÃO têm o mesmo comportamento:** *Stun* e *Freeze* permitem a redução natural de recarga; *Petrification* congela tanto a ação quanto a redução de recarga.
  - Efeitos como o set **Ocean** e a passiva *Neko-Sense* do **Tevor** atuam explicitamente **no início do turno do Esper**.
  - Assistências e contra-ataques não reduzem recargas por passagem de turno.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** Habilidades passivas sem cooldown e habilidades de chefes imunes a manipulação de recarga não são afetadas por efeitos de aumento/reset de cooldown.
- **O que desejo para o nosso jogo:** Ao usar uma habilidade de recarga $N$, gravar contador restante $N - 1$ ao encerrar o turno de uso e decrementar $-1$ na passagem de cada turno próprio (natural ou turno extra habilitado), **exceto quando o portador estiver sob estado com flag `FreezeCooldowns = true` (como Petrificação)**.

---

### Q05 — Qual relógio controla cada efeito? (PEN-01/PEN-07/PEN-09)

**No nosso projeto:** duração do portador por turno natural ou por todo turno; nova aplicação não perde duração no mesmo turno. Ainda não há relógio do aplicador/global.

#### Resposta Q05 — Relógio do portador, DoTs e passagens de turno

- **Modo/evento e fase:** Todos os modos de combate.
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** **Poison** ($4\%$ Max HP no início do turno do portador), **Bleed** ($50\%$ ATK do aplicador em True Damage no início do turno do portador), set **Enchanted** ($+30\%$ chance de estender Poison/Bleed em $+1$ turno), **Camille** (estende debuffs em $+1$ turno e detona Bleed reduzindo $-1$ turno), **Sieg** (dano de stacks no fim do turno do alvo).
- **Versão/data aproximada:** Regras consolidadas (2022–2026).
- **Fonte/link ou vídeo e minuto:** Dislyte Wiki (*Status Effects/Red*, *Status Effects/Bleed*, *Camille*, *Ritual Miracle*) e *One Chilled Gamer*.
- **Tipo de evidência:** Descrição oficial + guia da comunidade.
- **Situação inicial e contadores visíveis:** Alvo afetado por debuffs de duração ($1$ ou $2$ turnos) e efeitos periódicos (*Poison / Bleed*).
- **Sequência observada:**
  1. **Exemplo A (Aplicado a um alvo — Poison/Bleed):** No **início do turno do portador**, *Poison* ($4\%$ Max HP) e *Bleed* ($50\%$ ATK do aplicador) aplicam dano e consomem $1$ turno de duração. Se **Camille** usar *Double Claws*, ela detona o dano de *Bleed* imediatamente e reduz sua duração em $-1$ turno.
  2. **Exemplo B (Aplicado ao próprio dono):** Quando um Esper ganha um buff de $1$ turno antes de atacar na própria habilidade, o buff permanece ativo durante o ataque e decrementa ao encerrar o turno do portador.
  3. **Passagem de turno sob controle:** Buffs e debuffs padrão diminuem em $1$ turno após o turno do portador, **inclusive quando o portador perde o turno por Freeze ou Sleep**.
  4. **Empilhamento vs. Renovação:** *Poison* e *Bleed* acumulam múltiplas instâncias independentes no mesmo alvo; buffs/debuffs comuns não empilham (ex.: set *Harmony* marcado como *"Unstackable"*), renovando a duração.
- **Resultado e valores finais:** Todos os efeitos padrão operam no **relógio do portador** (quem carrega o status).
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** DoTs padrão (*Poison*, *Bleed*) disparam no **início do turno do portador**; efeitos especiais como a marca de **Sieg** especificam explicitamente disparo *"at the end of the target's turn"*. Morte do aplicador não remove debuffs padrão já aplicados.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** As wikis não listam numa tabela única o momento exato de decremento (início vs. fim do turno do portador) de todos os buffs defensivos (*Recovery, Shield, Invincibility, Standoff*). Em caso de dúvida para replicação exata, recomenda-se observar o ícone na transição de turno em vídeo.
- **O que desejo para o nosso jogo:** Adotar o **relógio do portador** com duas fases explícitas por turno: `OnTurnStart` (disparo e decremento de DoTs/HoTs como *Poison, Bleed, Recovery*) e `OnTurnEnd` (decremento de modificadores de atributo, escudos e controles como *ATK Up, DEF Down, Shield, Stun*), sempre após o portador encerrar sua oportunidade (mesmo incapacitado).

---

### Q06 — O que significa “uma vez por turno”? (PEN-09)

**No nosso projeto:** há proteção técnica contra cadeias excessivas. Não implementamos uma política geral por habilidade para essa expressão.

#### Resposta Q06 — Escopo de gatilhos “uma vez por turno” e limites em multi-hit

- **Modo/evento e fase:** Todos os modos de combate.
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** Set **Relief** (*"Only triggers once per turn when there are multiple wearers"*), **Ikki** (*Envious Moon*: *"Triggers once per turn"*), **Ashley** (*Rainbow Bridge*: *"can only be triggered once per turn"*), **Su Jue** e **Jin Qiu** (*"Triggers once per target per turn" / "counting once per target per turn"*).
- **Versão/data aproximada:** Regras consolidadas (2023–2026).
- **Fonte/link ou vídeo e minuto:** Dislyte Wiki (*Ritual Miracle — Relief Set*, *Status Effects/Golden*) e *One Chilled Gamer*.
- **Tipo de evidência:** Descrição oficial.
- **Situação inicial e contadores visíveis:** Inimigo executa ataque multi-hit ou em área (AoE) contra equipe com passivas reativas.
- **Sequência observada:**
  1. Em um ataque AoE crítico contra a equipe de **Ashley** (*Rainbow Bridge*), o efeito de $+10\%$ AP dispara **apenas uma vez** durante todo aquele turno ativo, mesmo que vários aliados recebam crítico.
  2. Quando múltiplos aliados usam o set **Relief**, no máximo $1$ assistência dispara naquele turno ativo (*"Only triggers once per turn when there are multiple wearers"*).
  3. Quando a habilidade usa a redação **"once per target per turn"** (como **Su Jue** e **Jin Qiu**), um ataque em área contra $5$ inimigos pode disparar o efeito até $5$ vezes ($1$ para cada alvo distinto naquele turno), mas múltiplos hits no *mesmo* alvo não disparam duas vezes.
  4. O limite reinicia somente quando o turno ativo atual termina e a próxima unidade inicia seu turno.
- **Resultado e valores finais:** Restrição vinculada ao **turno ativo global atual**, com granularidade definida pela redação da habilidade (`por turno`, `por portador por turno` ou `por alvo por turno`).
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** *"Uma vez por turno"* refere-se ao **turno ativo da unidade que está agindo no momento**, não a cada acerto individual de um multi-hit. Além disso, contra-ataques e assistências não disparam novos contra-ataques em cadeia.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** Habilidades sem cláusula limitadora (como cada hit da S1 de **Tetsuya** dissipando $1$ buff por acerto ou a S3 da **Mona** reduzindo o contador de Apep em $3$ hits) testam seus efeitos em **cada golpe** individualmente.
- **O que desejo para o nosso jogo:** Implementar escopos explícitos no motor de gatilhos: `OncePerHit`, `OncePerTargetPerActiveTurn`, `OncePerSourcePerActiveTurn` e `OncePerTeamPerActiveTurn` (como no set *Relief*), todos resetados exclusivamente na virada do contador de turno ativo.

---

### Q07 — Em que ordem se resolvem dano, passivas e reações? (PEN-06/PEN-09/PEN-10)

**No nosso projeto:** efeitos principais seguem sua lista; contra-ataques enfileirados são resolvidos depois, antes do encerramento da oportunidade. Não há sistema completo de interrupção entre acertos.

#### Resposta Q07 — Pipeline determinística de dano, mitigação, morte e reações

- **Modo/evento e fase:** Todos os modos de combate.
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** **Apep** (*Toxic Detonation* — reage a cada hit), **Anna** (*Rainbow Shield* — bloqueia o primeiro hit e contra-ataca), **Ikki** (*Sleep No More* — prevenção de morte), **Tetsuya R4** (*Umbrage* — prevenção de morte), **Camille** (aplica *Sear* e *Bleed* **antes** de causar o dano na S3), set **Avatara**.
- **Versão/data aproximada:** Regras consolidadas (2022–2026).
- **Fonte/link ou vídeo e minuto:** Dislyte Wiki (*Status Effects/Golden*, *Camille*, *Ritual Miracle*) e *Dislyte Beginner's Guide*.
- **Tipo de evidência:** Descrição oficial + observação consolidada.
- **Situação inicial e contadores visíveis:** Atacante usa habilidade multi-hit contra defensor com escudo/passiva de bloqueio, prevenção de morte e contra-ataque.
- **Sequência observada:**
  1. **Pré-dano (quando explícito na skill):** Efeitos como dissipação ou aplicação prévia de debuffs (ex.: **Camille — Wolven Smash**: *"inflicts Sear and Bleed on all enemies first. Then, it attacks all enemies"*) executam antes do cálculo de dano.
  2. **Execução sequencial dos hits:** Cada acerto do multi-hit é processado em sequência dentro da ação:
     - Absorção por barreiras/escudos (ex.: *Rainbow Shield* da **Anna** bloqueia o 1º hit; *Shield* comum absorve valor fixo) $\rightarrow$ redução no HP.
     - Contadores baseados em acertos recebidos (como *Toxic Detonation* do **Apep**) incrementam a cada hit.
     - Se um hit levar o HP a $0$, efeitos de **prevenção de morte** (ex.: *Standoff*, **Ikki — Sleep No More**, **Tetsuya R4**) ativam imediatamente, impedindo a morte e aplicando cura/invencibilidade.
  3. **Pós-habilidade (após todos os hits da skill concluírem):**
     - Disparam **assistências/perseguições** do lado atacante (ex.: *Relief*, *Pursuit Attacks* da **Anna**, *Screamer* do **Yun Chuan**).
     - Disparam **contra-ataques** do lado defensor (ex.: set *Avatara*, contra-ataque da **Anna**) — nunca interrompendo o meio dos hits de uma mesma habilidade.
- **Resultado e valores finais:** Habilidades multi-hit completam todos os seus acertos antes da janela de contra-ataques.
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** A cadeia segue a ordem: $\text{Efeitos Pré-Dano} \rightarrow \text{Loop de Hits (Barreira/Escudo} \rightarrow \text{HP} \rightarrow \text{Prevenção de Morte)} \rightarrow \text{Assistências/Perseguições} \rightarrow \text{Contra-ataques}$.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** Em skills multi-hit que atingem alvos aleatórios (*bounce*, como *Skyshot* de **Lu Yi** com $9$ hits), cada hit escolhe entre alvos vivos; em skills de alvo único, hits excedentes continuam no mesmo alvo até o fim da animação.
- **O que desejo para o nosso jogo:** Formalizar a pipeline por oportunidade: $[\text{Pré-Dano}] \rightarrow [\text{Loop de Hits: Mitigação} \rightarrow \text{Escudo} \rightarrow \text{HP} \rightarrow \text{Prevenção de Morte}] \rightarrow [\text{Assistências/Perseguições}] \rightarrow [\text{Revives Imediatos}] \rightarrow [\text{Contra-ataques Enfileirados}] \rightarrow [\text{Checagem de Vitória}]$.

---

### Q08 — Como a guarda deve coexistir com essas regras? (Decisão nossa)

**Hoje:** reduz pela metade o próximo dano e expira ao começar a próxima oportunidade do dono, inclusive extra.

#### Resposta Q08 — Contrato tático do comando Guarda no nosso simulador

- **Modo/evento e fase:** Decisão autoral do nosso projeto (sem comando genérico equivalente no Dislyte).
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** Comando tático **Guarda** do nosso simulador.
- **Versão/data aproximada:** Versão atual do projeto (novembro de 2026).
- **Fonte/link ou vídeo e minuto:** Especificação interna (`ROTEIRO-PESQUISA-USUARIO.md`).
- **Tipo de evidência:** Decisão de projeto.
- **Situação inicial e contadores visíveis:** Unidade escolhe **Guarda** em seu turno natural e recebe um ataque multi-hit antes de sua próxima oportunidade.
- **Sequência observada:** Definição das regras de fronteira da Guarda com base nas cadeias multi-hit e turnos extras validados nas questões anteriores.
- **Resultado e valores finais:** Política fechada para o comando Guarda.
- **O que encontrei no Dislyte:** N/A — mecânica autoral do projeto, desenhada para coexistir sem conflito com habilidades multi-hit e turnos extras.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** N/A.
- **O que desejo para o nosso jogo:**
  1. **Manter** redução de $50\%$ do dano e expiração no **início da próxima oportunidade do dono**.
  2. **Proteger contra toda a próxima habilidade ofensiva recebida** (todos os hits de uma mesma habilidade multi-hit), consumindo-se ao término daquela habilidade — evitando que o 1º hit de um ataque de $3$ a $9$ golpes quebre a Guarda e deixe os golpes seguintes passarem com dano cheio.
  3. **Turnos extras:** Um turno extra concedido ao dono **faz a Guarda expirar** no início desse turno extra.
  4. **Reações:** Executar assistência ou contra-ataque **não consome** a própria Guarda; *receber* um contra-ataque ou assistência inimiga ativa e consome a Guarda.
  5. **Modos:** Disponível nos modos táticos padrão e desativável via política explícita em modos especiais.

---

### Q09 — O que ocorre exatamente na última oportunidade permitida? (PEN-02/PEN-10/PEN-12)

**Hoje:** o comando e suas reações resolvem antes do timeout; eliminação final pode vencer na última ação. Vitória intermediária sem saldo global não inicia outra onda.

#### Resposta Q09 — Resolução da última oportunidade antes do timeout

- **Modo/evento e fase:** Infinity/Temporal Tower, Ritual Miracle (limite de turnos), Desolate Lands e Point War.
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** Mecânica global de limite de turnos por modo.
- **Versão/data aproximada:** Regras consolidadas (2022–2026).
- **Fonte/link ou vídeo e minuto:** Regras oficiais dos modos de jogo (*Dislyte Wiki*, *TV Tropes*).
- **Tipo de evidência:** Descrição oficial + observação consolidada.
- **Situação inicial e contadores visíveis:** Contador de turnos atinge a última unidade disponível antes do teto da fase.
- **Sequência observada:**
  1. A unidade ativa executa sua habilidade no último turno permitido e toda a cadeia resultante (hits, assistências, contra-ataques) resolve até o fim.
  2. Se a cadeia eliminar todos os inimigos antes de encerrar, a vitória é contabilizada naquele turno final.
  3. Se restarem inimigos vivos após o fim da última cadeia permitida, a partida encerra por limite de turnos.
- **Resultado e valores finais:**
  - *Modos de eliminação (Torre / Ritual Miracle):* Derrota se qualquer inimigo sobreviver ao fim da última cadeia (não existe empate).
  - *Modos de pontuação (Desolate Lands):* Encerra a rodada e computa a nota final obtida até aquele ponto.
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** A última oportunidade completa a resolução integral da habilidade ativa e suas reações antes de disparar o encerramento por limite de turnos.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** A documentação pública não especifica se um turno extra disparado exatamente na última contagem permitida é descartado antes de iniciar ou se estende o limite em $+1$; recomenda-se bloquear turnos extras além do teto para manter o limite estrito.
- **O que desejo para o nosso jogo:** Permitir que o comando da última oportunidade permitida e suas reações resolvam integralmente antes da checagem de timeout; bloquear o início de novos turnos extras após atingir o teto de turnos; e resolver o resultado conforme a política do modo (`Elimination`: vitória apenas se $0$ inimigos vivos, senão derrota; `Score`: consolida pontuação; `PvP`: vitória do defensor no timeout).

---

## P2 — Confrontos, equipes e chefes

### Q10 — O que persiste entre ondas? (PEN-01/PEN-02/PEN-14)

**Hoje:** políticas explícitas preservam/restauram vida e conservam/reiniciam status/recargas/velocidade. AP, guarda e fila de extras recomeçam na fronteira.

#### Resposta Q10 — Persistência de estado e reinício de iniciativa entre ondas

- **Modo/evento e fase:** Ritual Miracle (2 ondas de Miramon + 1 onda de Chefe), História, Torre multi-onda e Cube Miracle (entre salas).
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** **Feng Nuxi** (*Paper Talisman*: *"Will not be removed in subsequent rounds"*), set **Light** (*"Gains Immunity for 1 turn at the start of combat"*), set **Aegis** (*"All allies start battle with a Shield... Lasts 2 turns"*).
- **Versão/data aproximada:** Regras consolidadas (2022–2026).
- **Fonte/link ou vídeo e minuto:** Dislyte Wiki (*Ritual Miracle*, *Status Effects/Golden*) e *Dislyte Beginner's Guide*.
- **Tipo de evidência:** Descrição oficial + guia da comunidade.
- **Situação inicial e contadores visíveis:** Transição entre $\text{Onda } 1 \rightarrow \text{Onda } 2 \rightarrow \text{Onda } 3$ no Ritual Miracle.
- **Sequência observada:**
  1. **HP e Mortes:** O HP atual e o estado de morte dos aliados são mantidos da onda anterior para a seguinte (sem cura automática entre ondas; no Cube Miracle, aliados revividos antes do fim da luta permanecem vivos nas lutas seguintes).
  2. **Recargas e Status:** As recargas restantes das habilidades e os buffs/debuffs ativos com turnos restantes transitam para a nova onda (pilhas permanentes como *Paper Talisman* de **Feng Nuxi** explicitamente não são removidas entre rounds).
  3. **Barra de AP:** Ao iniciar a nova onda, a barra de AP é reiniciada e a ordem de ação volta a ser decidida pela SPD efetiva das unidades presentes.
  4. **Contador de turnos:** Permanece acumulado ao longo de todas as ondas da mesma batalha.
- **Resultado e valores finais:** Estado vital, status e recargas preservados; barra de AP reiniciada na entrada da onda.
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** Ondas dentro de uma mesma batalha compartilham o estado persistente dos personagens (HP, mortes, recargas, status) e o contador global de turnos, reiniciando apenas a disputa de iniciativa (AP) a cada nova onda.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** Efeitos vinculados aos inimigos derrotados no último golpe da onda encerram com a morte deles e não passam aos inimigos da onda seguinte.
- **O que desejo para o nosso jogo:** Manter nossa política explícita: preservar **HP atual/máximo, estado de morte, recargas, escudos e status/pilhas ativos** entre ondas; resetar **AP para $0\%$, limpar fila de turnos extras e resetar Guarda** na entrada da nova onda, recalculando a iniciativa inicial pela SPD efetiva e mantendo o contador de turnos acumulado.

---

### Q11 — Como funcionam os três times sucessivos? (PEN-03/PEN-04/PEN-10)

#### Resposta Q11 — Fluxo sequencial de três esquadrões (Sentinel Hunt / Falsetto Fantasy)

- **Modo/evento e fase:** Sentinel Hunt e Falsetto Fantasy (**Phobetor [Projection]** e **Dark Star Lord [Projection]**).
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** $3$ esquadrões distintos (ex.: Team 1 com aplicadores de Poison/debuff como **Kara** e **Hall**; Teams 2 e 3 com DPS, buffs e sustain).
- **Versão/data aproximada:** Regras consolidadas do Sentinel Hunt (2023–2026).
- **Fonte/link ou vídeo e minuto:** Guias de composição do *Sentinel Hunt* (r/Dislyte) e Dislyte Wiki (*Miramon / Distortion*).
- **Tipo de evidência:** Guia da comunidade + descrição oficial de chefes.
- **Situação inicial e contadores visíveis:** Jogador escala $3$ times independentes (Team 1, Team 2 e Team 3) antes de iniciar a tentativa.
- **Sequência observada:**
  1. **Composição e Ordem:** O jogador define livremente a ordem de entrada dos $3$ times sem repetir Espers entre eles (ex.: Team 1 especializado em controle/Poison de fase inicial, seguido por Teams 2 e 3 de dano direto).
  2. **Gatilho de Troca:** Quando o Team 1 é derrotado (ou conclui seu ciclo contra o chefe), o combate transiciona automaticamente para o Team 2, e depois para o Team 3 — a queda de um time **não encerra** a partida.
  3. **Estado do Chefe:** O progresso de dano/score acumulado contra o chefe e suas mecânicas de fase (como *Shadow Embrace* do Dark Star Lord ou *Life Shield* do Phobetor) avançam continuamente através dos $3$ times.
  4. **Resultado Final:** A pontuação final consolida o desempenho somado dos $3$ esquadrões para definir o rank da tentativa (ex.: rank `EX`).
- **Resultado e valores finais:** Três esquadrões disjuntos atuando sequencialmente para maximizar o placar agregado contra o mesmo encontro.
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** Os três times entram em sequência estrita ($\text{Team } 1 \rightarrow \text{Team } 2 \rightarrow \text{Team } 3$), sem repetição de personagens, acumulando dano/pontuação em um único resultado consolidado.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** As fontes textuais consultadas não detalham se debuffs temporários aplicados pelo Team 1 (como *DEF Down* ou *Poison*) são limpos exatamente no frame da transição para o Team 2 ou se expiram nos primeiros turnos do chefe; para o nosso projeto, limpar debuffs temporários na troca de esquadrão é a regra mais limpa e previsível.
- **O que desejo para o nosso jogo:** Implementar `SequentialSquadsMode`: exigir esquadrões disjuntos (sem repetição de personagem); disparar a troca por *wipe do time* ou *limite de turnos do esquadrão*; preservar no chefe o **HP descontado / pontuação e estágio de fase**, limpar debuffs temporários e resetar AP na entrada de cada novo time; e encerrar com vitória antecipada se o chefe tiver HP finito e for abatido antes do 3º time.

---

### Q12 — O que era compartilhado nas três frentes? (PEN-05/PEN-06/PEN-08)

Referência relatada: Celestial Anomaly Season 0.

#### Resposta Q12 — Dinâmica das três frentes simultâneas (Chimera — Season 0)

- **Modo/evento e fase:** Celestial Anomaly — Season 0 (*Withering Messenger, Void Messenger, Scorch Messenger* e o confronto multi-frente da **Chimera**).
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** Três esquadrões escalados simultaneamente contra três chefes/partes (*Lion, Goat/Ram e Viper*).
- **Versão/data aproximada:** Maio–julho de 2023 (Season 0).
- **Fonte/link ou vídeo e minuto:** Relatos detalhados de jogadores que concluíram o evento (*Thoughts on celestial anomaly game mode now that it's almost over?* e *celestial anomaly finishing up* — r/Dislyte).
- **Tipo de evidência:** Relato consolidado da comunidade (experiência direta de jogadores no evento).
- **Situação inicial e contadores visíveis:** Três times posicionados em paralelo em um único campo de batalha dividido em três linhas/frentes contra os três chefes da Chimera.
- **Sequência observada:**
  1. **Três chefes simultâneos no mesmo campo:** Diferente dos Messengers individuais (torres de $150$ andares cada), a batalha da **Chimera** colocava **três times lutando contra três chefes/cabeças ao mesmo tempo** (*"seeing three teams fight three bosses on one big battlefield"* / *"Having 3 bosses run concurrently"*).
  2. **Isolamento das ações dos Espers vs. Ações globais dos Chefes:** As habilidades em área (AoE) dos Espers atingiam apenas o chefe/inimigos da sua própria frente, **mas os Ultimates dos chefes eram globais (campo inteiro)** — por exemplo, o debuff aplicado pela *Viper* em uma frente combinava com o Ultimate de outro chefe atingindo todos os times (*"some espers' aoe abilities would be battlefield wide too and not just the bosses ult"* / *"Viper probably is the most wipe-inducing as it's debuff, combo'd with another boss' ult, can wipe your teams easily"*).
  3. **Gatilhos globais de turno:** Havia marcos globais de turnos compartilhados (ex.: nos turnos $40$, $80$ e $120$, o chefe saltava e executava seu Ultimate independentemente do lock de AP da frente).
- **Resultado e valores finais:** Três frentes concorrentes com isolamento de habilidades aliadas, mas compartilhando gatilhos globais de turno, Ultimates dos chefes e o resultado consolidado do encontro.
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** Na Chimera (Season 0), o compartilhamento entre as três frentes incluía o **campo de batalha unificado para os Ultimates dos chefes, debuffs cruzados dos chefes, marcos globais de turnos ($40/80/120$) e o resultado total da tentativa**, enquanto as ações dos Espers permaneciam restritas à própria frente. *(Nota: esses relatos descrevem apenas a regra observada em tela, não a arquitetura de threads ou servidor do cliente.)*
- **Exceções, divergências ou o que ainda não foi possível confirmar:** Não há documentação oficial detalhando se as barras de HP dos três chefes da Chimera eram somadas em um pool único ou se cada chefe possuía barra própria vinculada à vitória do encontro; ambas as variações são fáceis de parametrizar no nosso projeto.
- **O que desejo para o nosso jogo:** Modelar o modo multi-frente com **frentes localmente isoladas para ações dos jogadores**, conectadas a um **Coordenador Global de Encontro** que gerencia: (a) *Pool Global de HP/Pontuação* (configurável como barra única ou barras vinculadas), (b) *Gatilhos Globais de Turno* (ex.: marcos de turno que disparam habilidades do chefe) e (c) *Habilidades Globais de Chefe* marcadas com escopo `AllFronts`.

---

### Q13 — Quais habilidades atravessam fronteiras de time? (PEN-08)

#### Resposta Q13 — Fronteiras de alcance entre times e frentes

- **Modo/evento e fase:** Todos os modos ($1$ time, $3$ times sucessivos e $3$ frentes simultâneas).
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** Auras de Líder (*Leader Buffs*), habilidades AoE de Espers, curas, revives e buffs de AP.
- **Versão/data aproximada:** Regras consolidadas (2022–2026).
- **Fonte/link ou vídeo e minuto:** Relatos de *Celestial Anomaly Season 0* (r/Dislyte) e guias de formação.
- **Tipo de evidência:** Relato consolidado + guia da comunidade.
- **Situação inicial e contadores visíveis:** Múltiplos times escalados em modos multi-esquadrão ou multi-frente.
- **Sequência observada:**
  1. Mesmo na **Chimera** (onde três times dividiam a mesma tela de batalha), **nenhuma habilidade de Esper (nem mesmo AoEs) atravessava para outro time** — como confirmado pelos jogadores que desejavam que AoEs de Espers fossem globais como os Ultimates dos chefes.
  2. Auras de Líder, curas, revives e boosts de AP aplicam-se estritamente ao esquadrão ativo de cada time.
- **Resultado e valores finais:** Escopo das habilidades de jogadores $100\%$ restrito ao próprio esquadrão/frente.
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** Habilidades de Espers **nunca** atravessam fronteiras entre times ou frentes; apenas habilidades específicas de chefes de evento (como os Ultimates da Chimera) e árvores de melhorias sazonais do evento operam em nível global.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** Nenhuma exceção identificada para habilidades de Espers.
- **O que desejo para o nosso jogo:** Restringir por contrato de tipo (`SquadContext`) $100\%$ das habilidades, auras, curas, revives e buscas de alvo dos personagens jogáveis ao seu próprio esquadrão/frente, permitindo escopo `AllFronts` apenas em habilidades de chefes explicitamente marcadas.

---

### Q14 — Quais são os limites de composição? (PEN-03/PEN-11)

#### Resposta Q14 — Regras de tamanho de esquadrão, unicidade e invocações

- **Modo/evento e fase:** Modos padrão PvE/PvP ($1$ time), Sentinel Hunt / Falsetto Fantasy ($3$ times), Point War.
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** Todos os Espers e invocações de batalha (ex.: *Phobetor* invocando aliado sob *Temporal Disturbance*).
- **Versão/data aproximada:** Regras consolidadas (2022–2026).
- **Fonte/link ou vídeo e minuto:** Dislyte Wiki (*Phobetor*, *Miramon*), *TV Tropes* (*Emblems of Expertise*) e guias de equipe.
- **Tipo de evidência:** Descrição oficial + guia da comunidade.
- **Situação inicial e contadores visíveis:** Tela de preparação de equipe antes do combate.
- **Sequência observada:**
  1. **Tamanho do time:** Mínimo de $1$ e máximo de $5$ Espers nos modos padrão (existindo desafios opcionais de *Emblem of Expertise* que pedem vencer no Point War com $4$ ou menos Espers, sem bônus de atributos por jogar desfalcado).
  2. **Sem reservas com troca manual:** Não há banco de reservas substituíveis durante a batalha.
  3. **Unicidade:** Não é permitido duplicar o mesmo personagem dentro de um time nem entre os $3$ times de modos multi-esquadrão.
  4. **Invocações:** Encontros com invocação adicionam unidades temporárias à arena ativa durante a luta.
- **Resultado e valores finais:** Esquadrões de $1$ a $5$ personagens únicos, sem compensação para times reduzidos.
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** Os slots ativos visíveis na arena correspondem exatamente aos alvos válidos das habilidades, respeitando o limite de $5$ personagens únicos por esquadrão e sem bônus compensatório para times menores.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** Nenhuma exceção relevante.
- **O que desejo para o nosso jogo:** Validar na montagem: $1 \le \text{unidades\_por\_time} \le 5$, unicidade absoluta de `CharacterId` em toda a inscrição (incluindo desafios de $2$ ou $3$ times), remoção automática de invocações na morte do invocador e zero bônus artificial para times incompletos.

---

### Q15 — Como são calculados placar, empate e recompensa? (PEN-10/PEN-13)

**Hoje:** não há score, economia nem crédito persistente de recompensa. O teste de conclusão única garante somente a sinalização da fase em memória.

#### Resposta Q15 — Cálculo de pontuação, desempate e políticas de recompensa

- **Modo/evento e fase:** Desolate Lands (Shadowfire, Shadowgale, Shadowstream, Shadowglow), Sentinel Hunt, Ritual Miracle e Point War.
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** Sistemas de classificação e recompensas por modo.
- **Versão/data aproximada:** Regras consolidadas pós-Patch 3.1.1 (2022–2026).
- **Fonte/link ou vídeo e minuto:** Dislyte Wiki (*Version/3.1.1*), *TV Tropes* (*Dislyte*) e registros de pontuação de Desolate Lands (`4 Billion shadowstream`).
- **Tipo de evidência:** Descrição oficial + observação consolidada.
- **Situação inicial e contadores visíveis:** Encerramento de tentativa em modo de pontuação ou eliminação.
- **Sequência observada:**
  1. **Desolate Lands (Pontuação):** O chefe possui **HP infinito**; o jogador busca sobreviver o máximo possível e causar o maior dano para atingir notas que vão até **EX, EX+ e EX++** (adicionadas acima de SSS no Patch 3.1.1).
  2. **Overkill e Dano Total:** Em chefes de score sem teto de HP, todo o dano aplicado pelos hits acumula diretamente na pontuação (chegando à casa dos bilhões em composições otimizadas).
  3. **Empates:** Não há empate de batalha; em rankings de velocidade, desempata-se por menor número de turnos e menor tempo real.
  4. **Recompensas:** Masmorras de farm concedem recompensas por vitória concluída (devolvendo $90\%$ da stamina em caso de derrota); Torre concede recompensas por primeira conclusão sem custo de stamina.
- **Resultado e valores finais:** Pontuação escalável até EX++ nos modos de dano infinito e recompensas condicionadas ao objetivo do modo.
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** Os modos de score utilizam chefes de HP infinito com notas escalonadas ($\text{S} \rightarrow \text{SSS} \rightarrow \text{EX} \rightarrow \text{EX+} \rightarrow \text{EX++}$) baseadas no dano acumulado e multiplicadores da mecânica do chefe.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** A fórmula matemática exata dos multiplicadores internos de cada fase dos chefes de Desolate Lands não é publicada oficialmente pela desenvolvedora.
- **O que desejo para o nosso jogo:** Implementar `BattleScoreResult` registrando `DamageToHP`, `ShieldDamage`, `OverkillDamage`, `SurvivalMultiplier` e `TurnCount`. Desempatar rankings por `TurnCount ASC, DurationMs ASC`, garantir precedência de vitória ao abater o último inimigo antes de efeitos pós-morte, e suportar políticas `FirstClear`, `PerAttempt` e `BestScore`.

---

### Q16 — Como pausa, saída e retomada se comportam? (PEN-14)

#### Resposta Q16 — Ciclo de vida da sessão, pausa e retomada determinística

- **Modo/evento e fase:** Todos os modos PvE (com destaque documentado para **Cube Miracle** e **Multi-Battles**).
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** Fluxo de sessão de batalha do cliente.
- **Versão/data aproximada:** Regras consolidadas (2022–2026).
- **Fonte/link ou vídeo e minuto:** *TV Tropes* (*Dislyte — Anti-Frustration Features*) e observação do cliente.
- **Tipo de evidência:** Registro consolidado + observação direta.
- **Situação inicial e contadores visíveis:** Batalha em andamento no cliente.
- **Sequência observada:**
  1. **Pausa:** Congela integralmente a simulação e os contadores enquanto o menu de pausa está aberto.
  2. **Saída antes da derrota (*Quit*):** No **Cube Miracle**, se o jogador abandonar a batalha *antes* de todo o time ser derrotado, **nenhuma unidade é perdida** e a luta pode ser tentada novamente do início (com o time inimigo restaurado ao estado pré-luta).
  3. **Derrota completa:** Em fases com stamina, perder a batalha devolve $90\%$ da stamina gasta.
  4. **Fechamento abrupto do app:** Como a simulação é validada ao final no servidor, encerrar o cliente no meio da luta não retoma o turno exato ao reabrir.
- **Resultado e valores finais:** Pausa congela tudo; saída antes da derrota aborta a tentativa sem salvar progresso parcial da luta.
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** Cada combate é resolvido como uma unidade atômica sem checkpoint intermediário de turno após fechamento do aplicativo.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** Nenhuma exceção relevante.
- **O que desejo para o nosso jogo:**
  1. **Pausa:** Congelar integralmente o relógio e a fila de simulação.
  2. **Saída voluntária:** Abortar a tentativa restaurando o estado pré-combate dos personagens.
  3. **Retomada offline determinística (diferencial do nosso motor):** Aproveitar nosso motor determinístico com replay para salvar opcionalmente um **snapshot leve (`Seed + Comandos Aceitos`)** a cada turno concluído, permitindo retomar exatamente do ponto de parada em caso de fechamento acidental do jogo.

---

## P3 — Expansões e limitações técnicas

### Q17 — Quais diferenças de ATB você deseja? (PEN-07)

Decisão de projeto, inspirada em FFVIII/FFIX.

#### Resposta Q17 — Arquitetura desacoplada para Perfil ATB opcional

- **Modo/evento e fase:** Modo alternativo futuro inspirado em *Final Fantasy VIII / Final Fantasy IX*.
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** Interface de agendamento de tempo (`ITimeScheduler`).
- **Versão/data aproximada:** Planejamento arquitetural (novembro de 2026).
- **Fonte/link ou vídeo e minuto:** Especificação autoral (`ROTEIRO-PESQUISA-USUARIO.md`).
- **Tipo de evidência:** Decisão de projeto.
- **Situação inicial e contadores visíveis:** Núcleo atual validado com o **Perfil AP (estilo Dislyte)**.
- **Sequência observada:** Definição dos contratos arquiteturais para permitir um **Perfil ATB Clássico** futuro sem alterar o motor de resolução de dano e efeitos.
- **Resultado e valores finais:** Especificação desacoplada entre *Perfil AP* e *Perfil ATB*.
- **O que encontrei no Dislyte:** N/A — decisão autoral de design.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** N/A.
- **O que desejo para o nosso jogo:**
  - **Modo Opcional:** Manter o *Perfil AP (Dislyte)* como padrão tático e oferecer o *Perfil ATB (FFVIII/FFIX)* como perfil alternativo de modo de jogo.
  - **Fluxo de tempo e pausa em menus:** No Perfil ATB, as barras avançam continuamente enquanto o menu principal está aberto, mas **pausam automaticamente dentro de submenus de seleção de habilidade/alvo** (*estilo ATB Wait do FFIX*) e **pausam durante animações de execução de habilidades**.
  - **Fila de prontidão e recursos:** Unidades prontas entram numa fila FIFO de comandos; habilidades podem ter **tempo de preparação (Cast Time)** opcional e consumir **Energia/MP** separado da barra de prontidão.

---

### Q18 — Que mecânicas numéricas precisam de cobertura? (PEN-13)

#### Resposta Q18 — Tetos numéricos, precisão e proteção contra overflow

- **Modo/evento e fase:** Chefes de HP infinito (Desolate Lands) e Espers com dano percentual ou redução de HP máximo.
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** **Norah** ($3\%$ Max HP do alvo, limitado a $50\%$ do ATK da Norah), **Anna** ($15\%$ Max HP do alvo por hit, limitado a $30\%$ do HP máx. da Anna), **Ikki** (reduz Max HP Capacity em $30\%$), **Fatimah / Jin Qiu** (pilhas com $\text{Max } 10\text{ stacks}$ / teto de $+150\%$).
- **Versão/data aproximada:** Regras consolidadas (2023–2026).
- **Fonte/link ou vídeo e minuto:** Dislyte Wiki (*Status Effects/Golden*) e *One Chilled Gamer*.
- **Tipo de evidência:** Descrição oficial + engenharia numérica.
- **Situação inicial e contadores visíveis:** Batalhas com bilhões de dano acumulado, dano percentual e múltiplas pilhas.
- **Sequência observada:**
  1. **Dano percentual com teto explícito:** Toda habilidade de dano baseado em HP máximo no Dislyte declara um teto atrelado a um atributo do atacante (*up to $50\%$ of the carrier's ATK* em Norah; *up to $30\%$ of the carrier's Max HP* em Anna) para impedir que chefes de milhões/infinitos de HP sejam derretidos instantaneamente.
  2. **Pilhas limitadas (`Max Stacks`):** Status acumulativos definem teto rígido (ex.: $\text{Max } 10\text{ stacks}$ em Fatimah/Sieg; $\text{Max } 7\text{ stacks}$ em Ashley; $\text{Max } 20\text{ stacks}$ em Anna).
  3. **Dano acumulado alto:** Pontuações de Desolate Lands ultrapassam $4\text{ bilhões}$ ($4.000.000.000$), exigindo tipos numéricos de $64\text{ bits}$.
- **Resultado e valores finais:** Tetos explícitos em dano percentual, limites de pilhas e acumuladores largos.
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** A estabilidade numérica do Dislyte apoia-se em **tetos explícitos por habilidade** (*up to $X\%$ of caster's stat*) e **limites máximos de pilhas** declarados diretamente nas regras de cada efeito.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** O tipo primitivo interno (`int64` vs. `double`) e o modo exato de arredondamento de cada subfórmula não são visíveis externamente.
- **O que desejo para o nosso jogo:**
  - Utilizar **`int64` (`long`)** para HP de chefes, escudos grandes e acumuladores de pontuação (evitando overflow de $32\text{ bits}$ em $2,14\text{ bilhões}$).
  - Usar **`double`** nos cálculos intermediários de multiplicadores, aplicando arredondamento determinístico (`Floor`) na aplicação final e garantindo dano mínimo de $1$ em golpes não-bloqueados/não-imunes.
  - Exigir **`MaxDamageCap`** atrelado ao atacante em toda habilidade baseada em $\text{HP}\%$, **`MinMaxHpFloor` (ex.: $25\%$)** para redução de HP máximo, **`MaxStacks`** obrigatório em todo status empilhável e descarte de **Overheal** por padrão salvo flag `ConvertOverhealToShield`.

---

### Q19 — Quais fatos históricos têm evidência? (VAL-07)

#### Resposta Q19 — Evolução histórica de Celestial Anomaly (Season 0 vs. Season 1)

- **Modo/evento e fase:** Celestial Anomaly — Season 0 (2023) vs. Season 1 (fevereiro de 2024).
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** Chefes sazonais (*Withering, Void, Scorch Messengers* e *Chimera* na Season 0; chefes baseados no *Ritual Miracle* como *Kronos* na Season 1).
- **Versão/data aproximada:** Maio de 2023 a fevereiro de 2024.
- **Fonte/link ou vídeo e minuto:** Registros públicos e discussões da comunidade (*How did they manage to remove all the fun out of Celestial Anomaly?* e *Thoughts on celestial anomaly game mode now that it's almost over?* — r/Dislyte).
- **Tipo de evidência:** Registro histórico da evolução do modo + separação de opinião da comunidade.
- **Situação inicial e contadores visíveis:** Comparação entre a estrutura da Season 0 e a reformulação da Season 1.
- **Sequência observada:**
  1. **Fatos observáveis na mudança:** Na *Season 0* (2023), o modo contava com torres de $150$ andares, árvores aleatórias de melhoria e a batalha simultânea de $3$ times contra a **Chimera**. Na *Season 1* (início de 2024), o formato foi alterado para estágios diários baseados nos chefes do *Ritual Miracle* (como *Kronos*) com suporte a *Blitz / Multi-Battle* e farm de relíquias.
  2. **Opinião da comunidade (separada dos fatos):** Jogadores criticaram na Season 0 o tempo longo das lutas manuais/sem *Blitz*, a aleatoriedade das melhorias e a dificuldade elevada; na Season 1, criticaram o excesso de simplificação, o bloqueio por tempo diário e a reciclagem de chefes antigos.
- **Resultado e valores finais:** Mudança de design documentada entre temporadas, sem declaração técnica da desenvolvedora atribuindo a alteração a limitações de engine ou servidor.
- **O que encontrei no Dislyte (Conclusão sustentada pela evidência):** A substituição do formato multi-frente da Season 0 por lutas com *Blitz* na Season 1 reflete mudanças de fluxo de jogo, tempo de sessão e monetização/progressão — **não há qualquer evidência de falha de engine ou inviabilidade técnica**.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** Motivações internas de engenharia da Lilith/Farlight Games nunca foram publicadas.
- **O que desejo para o nosso jogo:** Manter separação rigorosa entre fatos técnicos medidos no nosso próprio código e especulações externas sobre motores de terceiros.

---

### Q20 — O que fica para medição no nosso projeto? (VAL-04/VAL-05/VAL-06)

#### Resposta Q20 — Metas de desempenho e instrumentação da nossa stack

- **Modo/evento e fase:** Camadas de renderização visual, concorrência e servidor do nosso projeto.
- **Personagem, habilidade, equipamento e evolução/resonância relevantes:** Núcleo determinístico atual ($49$ testes e $168$ cenários aprovados) vs. futuras integrações de UI e rede.
- **Versão/data aproximada:** Planejamento de validação técnica (novembro de 2026).
- **Fonte/link ou vídeo e minuto:** Diagnóstico interno (`ROTEIRO-PESQUISA-USUARIO.md`).
- **Tipo de evidência:** Requisito técnico de engenharia (a ser validado por benchmarks próprios).
- **Situação inicial e contadores visíveis:** Testes unitários locais validam regras determinísticas, mas não medem FPS, threads ou capacidade de rede.
- **Sequência observada:** Definição das metas de desempenho que deverão ser instrumentadas quando os componentes visuais e de servidor existirem.
- **Resultado e valores finais:** Metas mensuráveis de fluidez, CPU e carga de rede.
- **O que encontrei no Dislyte:** N/A — medições de desempenho dependem exclusivamente da stack visual e de infraestrutura do nosso projeto.
- **Exceções, divergências ou o que ainda não foi possível confirmar:** Valores reais serão obtidos apenas após a instrumentação do protótipo gráfico e do servidor.
- **O que desejo para o nosso jogo:** Adotar como metas obrigatórias na fase de integração:
  1. **Fluidez visual (Mobile/Desktop):** Manter **$60\text{ FPS}$ estáveis** ($\le 16,6\text{ ms}$ por frame) mesmo com **$3$ frentes ativas simultâneas** e cadeias longas de multi-hit.
  2. **Orçamento da simulação lógica:** Resolver cada turno completo (incluindo toda a cadeia de reações e status) em **menos de $2\text{ ms}$ de CPU**, permitindo rodar $3$ frentes concorrentes sem travar a thread principal de renderização.
  3. **Capacidade de servidor:** Executar testes de carga automatizados validando replays determinísticos (`Seed + Inputs`) e sessões simultâneas antes de abrir ambientes multiusuário.

---

## Checklist para a primeira devolutiva

- [x] **Q01:** contador geral, incluindo extras, reações e controle.
- [x] **Q04:** fronteira da recarga com um exemplo completo (incluindo a exceção de *Petrification* e do set *Astralis*).
- [x] **Q05:** duração de efeitos com um exemplo aplicado ao próprio dono e outro a um alvo.
- [x] **Q06/Q07:** significado de uma vez por turno e ordem de uma cadeia.
- [x] **Q10/Q11:** passagem entre ondas e entre os três times.
- [x] **Q02, Q03, Q08, Q09 e Q12–Q20:** todos os itens complementares preenchidos e validados.