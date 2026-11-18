# Revisão de habilidades recebidas — R01

Recebido em 18/11/2026: “Dislyte — estado das habilidades, ressonâncias e patches até 18 de novembro de 2026”. Análise documental para HAB-01; sem nova pesquisa web, execução do jogo ou alteração de regras. Original mantido no anexo; não copiar roster/progressão pessoal para dados do jogo original.

## Confiança e versão

O relatório informa 3.4.49 como ativo e 3.4.50 como anunciado para 21/11. **São afirmações recebidas, não verificadas independentemente nesta entrega.** O texto não inclui URLs individuais ou citações recuperáveis para suas fichas; “confirmado” no documento não transforma uma regra em evidência primária verificada no projeto.

Guardar separadamente data da observação, patch ativo relatado, anúncio futuro e versão da ficha. Não misturar eventual mudança de Tevor com a ficha anterior, nem programar atualização automática. Quando uma regra específica for necessária, pedir sua fonte/descrição exata; não refazer uma investigação de todo o jogo agora.

O próprio material ressalva fichas incompletas de Ikki/Toland e partes de Victor/Nuxi, além de divergência de duração na proteção de Nuxi R6. Esses pontos continuam incertos; não preencher por memória ou usar números no balanceamento autoral.

## Contribuições para HAB-01

| Família extraída do relato | Exemplos mencionados | Consequência de projeto |
|---|---|---|
| Posição exclusivamente passiva | Yi An, Wu You, Petros | Não criar comando clicável; declarar gatilho e estado |
| Ativa com componente passivo | Maria, Everett, Cecilia | Modelo deve comportar os dois componentes na mesma posição |
| Passiva com recarga interna | Petros, Raven, Jiang Jiuli | Ativa/passiva e possuir recarga são dimensões distintas |
| Recarga protegida de alterações | Ming Shuo, Cecilia e outras fichas | Separar decremento natural de modificações externas; não implementar exceção global por nome |
| Gatilho de início de batalha | Yi An e configurações de estados iniciais | Janela única por tentativa; reset reconstitui, não duplica a aplicação |
| Gatilho por turno/ação/hit | Unas, Maria, Wu You, Ikki | Identificar dono, tipo de turno e limite por ação/alvo, sem usar rodada como substituto |
| Componentes de progressão | Ressonância, Ascension e Divinate | Configuração efetiva versionada; não aplicar R2/R6 a variante-base |
| Estado que substitui comportamento | Victor, Jiang Jiuli, Wu You | Mudanças de modo/conjunto exigem contrato, não só troca de ícone |
| AP com operação própria | Troca descrita para Maria | Trocar, absorver, aumentar e reduzir AP não devem ser confundidos |
| Morte/prevenção/ressurreição | Anna, Cecilia, Ji Qingyang | Distinguir prevenção, auto-revive próprio, revive de aliados e disparo pós-morte |
| Modificadores e dano | Moroyama, Tevor e outros | Fórmulas condicionais, crítico, dano verdadeiro e overkill ainda não são catálogo implementado |

Os exemplos servem como pistas de cobertura, não fichas aprovadas ou conteúdo a copiar. “Ultimate” não implica ataque: há cura, proteção, controle e condições. Três posições não garantem três escolhas manuais.

## Correspondência com o motor atual

Já existem AP/SPD, recargas, flags de extras, Counter/Reflect/DeathExplosion, prevenção, auto-revive próprio e escudos no perfil testado. Isso não equivale às habilidades completas descritas no relatório.

Em especial: nosso AutoRevive não implementa a habilidade completa de Cecilia que revive aliados; Counter básico não é automaticamente uso de S1/S2 de outro personagem; escudo absorvente genérico não equivale a bloquear primeiro hit e gerar estados como os descritos para Anna. Não declarar personagens reproduzidos porque existem efeitos com nomes semelhantes.

## Ordem prática mantida

1. HAB-01 parte 1: perfis padrão/clássico e posições, componentes ativo/passivo/híbrido; ações disponíveis e informações passivas separadas. Preservar clássico e não criar passivas fictícias.
2. Parte 2: escolher uma passiva original simples, preferencialmente um gatilho único de início de batalha com efeito já suportado. Isso é recomendação de escopo, não decisão de kit do usuário.
3. Depois: limites por dono/alvo, recarga interna/protegida e condições, uma família por entrega. Variantes de progressão e estados complexos permanecem extensões.
4. Manual e automático consultam a mesma configuração efetiva; básica não tem caminho alternativo para burlar condições.

Nenhum motivo para reabrir R04 ou repetir os 155 testes nesta revisão. Detalhes externos conflitantes não alteram silenciosamente o contrato próprio.

## Poucas evidências a pedir quando forem necessárias

- Para implementar um gatilho inspirado numa categoria, basta uma descrição específica com versão/data e fonte identificável; não exigir todos os personagens.
- Para uma passiva complexa, esclarecer a quem pertence “uma vez por turno” e se reage a extra/Counter/assistência.
- Para comparar progressão, separar ficha-base da modificação e registrar quais campos ela realmente altera.

Não é necessário continuar extraindo o chat inteiro. Este documento já fornece variedade suficiente para orientar o modelo das posições. Fichas completas serão úteis apenas nos casos que decidirmos estudar.

## Registro de entrega

Só documentos atualizados; Jogar.cmd e o protótipo continuam iguais. Sem nova bateria de testes, coleta de conta ou acesso a serviços. Próximo HAB-01 parte 1.

SHA-256 do anexo recebido: C28CBB3001CAECE747885C2F24680B863B6E44AD2C535DD34531504763519A69
