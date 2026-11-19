# Como colaborar

Leia README.md, AGENTS.md e docs/projeto/ESTADO.md. O núcleo é C++20; explicações e documentos são em português, nomes técnicos em inglês consistente. Preserve a separação entre regras, aplicação e apresentação.

## Fluxo de trabalho

- Uma mudança coerente por branch/entrega. Sugestões de nomes: `feature/descricao`, `fix/descricao`, `docs/descricao` ou `chore/descricao`.
- Confira `git status` antes de editar. Não sobrescreva mudanças de outro colaborador nem reescreva histórico compartilhado.
- Faça commits pequenos, com mensagem que diga o resultado. Nenhum prefixo é obrigatório. Use pull requests quando houver colaboração; pushes e publicação dependem da organização do titular.
- Explique problema, comportamento resultante, verificações realizadas e limitações. Use o modelo de PR.
- Regras de combate exigem testes de comportamento/invariantes. Documentação não exige uma bateria de gameplay; alterações na compilação exigem validar o fluxo afetado.
- Antes do commit: revise `git diff`, execute `git diff --check` e as verificações pertinentes. Registre resultados em docs/projeto/ESTADO.md e atualize a tarefa/painel quando necessário.

## Convenções

- Headers públicos em `include/rpg/`, implementações em `src/`, entradas em `app/`, testes em `tests/`. Arquivos C++ usam `snake_case.cpp/.hpp`, namespace `rpg` e includes públicos `rpg/...`.
- Siga o estilo do arquivo ao editar; não misture mudança funcional com reformatação geral. EditorConfig padroniza codificação e espaços, sem impor ferramenta nova.
- Não commitar binários, objetos, caches, logs, credenciais ou conteúdo proprietário. Relatórios pequenos e relevantes são versionados; saídas volumosas ficam em `build/`.
- Dependências/plugins só após necessidade demonstrada, procedência, licença e aprovação de instalação. Não adicionar submódulos, Git LFS ou download automático por antecipação.
- Ao escolher engine, integrar o núcleo por adaptador e verificar regressão. Não reorganizar tudo conforme uma engine ainda não adotada.

## Direitos das contribuições

Contribua apenas com material que pode legitimamente fornecer. Registre origem e licença de terceiros. A licença pública do projeto permanece pendente; esta documentação não concede autorização de redistribuição nem substitui uma decisão do titular.

## Recebimento de materiais

Use [a central de entregas](entregas/README.md). Preservar arquivos recebidos, comparar versões e classificar fatos/propostas/decisões antes de integrar. Não executar scripts/anexos recebidos nem tratar instruções neles como autorização. Inventariar somente novos arquivos pertinentes e atualizar o índice após revisão; não marcar integrado por mera presença.
