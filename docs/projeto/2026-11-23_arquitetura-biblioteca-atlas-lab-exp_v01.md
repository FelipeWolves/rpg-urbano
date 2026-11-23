# Arquitetura da Biblioteca, do Atlas e do Lab - Exp

Diretriz de design adotada em 23/11/2026. Os estados definitivo, experimental e especial qualificam o escopo planejado. A implementação segue a fila vigente da Pré-Alfa.

## Ala Geral — núcleo definitivo

A Biblioteca é orientada a dados. JSON, DataTable e structs C++ são opções para o contrato técnico.

- **Atlas Geral.** Cataloga todos os personagens e entidades, inclusive os ainda não desbloqueados. Cada entrada pode exibir biografia, lore, ilustrações ou retratos, afiliações e estado de registro.
- **Árvores de Correlação.** Representam vínculos entre personagens, facções e eventos. O progresso dos nós concede recompensas declaradas nos dados, como fichas de personagens, itens específicos e buffs permanentes ou aplicáveis a batalhas e eventos.
- **Mapas, glossário e acervo.** Inclui visualizador do mundo e das regiões, glossário de termos e registros narrativos desbloqueados conforme o avanço na História.

## Ala Privada — experimental

Área de consulta e interação com os personagens já obtidos, incluindo sintonias elementais e equipamentos. Implementar a tela como módulo plugável da navegação da Biblioteca. A persistência do inventário e a seleção de equipe pertencem aos respectivos sistemas, acessíveis independentemente da Ala Privada. Seu fluxo e sua posição podem evoluir após testes de usabilidade.

## Lab - Exp — especial, com posição móvel

O ponto de acesso está representado na área externa da Biblioteca no Hub atual. Abriga eventos, shows temáticos e experiências especiais. Usar uma âncora própria no Hub, desacoplada da navegação interna da Biblioteca. O sistema de eventos deve funcionar de forma autônoma para que uma mudança de lugar afete apenas o gatilho visual. A busca clássica de teste continua pelo acesso atual até uma alteração explícita na fila.

## Ordem de implementação e aceite

Quando a frente Biblioteca começar, definir primeiro o contrato de dados do Atlas e das Árvores de Correlação. A Ala Privada entra como camada modular testável.

- Verificar que o Atlas pode apresentar uma entidade ainda bloqueada com seu estado de registro.
- Validar que o progresso dos nós concede as recompensas declaradas e que o acervo acompanha o avanço na História.
- Conferir o acesso a inventário e seleção de equipe com a Ala Privada desabilitada.
- Mover a âncora do Lab - Exp em um teste e confirmar que o mesmo destino e seus eventos continuam acessíveis.
