# Terminal 2D — roteiro do piloto com dados simulados

**Data:** 28/11/2026

**Estado:** piloto implementado e compilado em Win64. A prévia normal continua com os cinco bilhetes e o rodapé já aceitos.

O piloto exercita o mesmo catálogo do TerminalCore com a arte provisória. Os exemplos de saldo, custo e bônus são exclusivos de QA. O embarque real da prévia permanece restrito ao Monotrilho e às sessões locais existentes.

## Cenários do piloto

| Caso | Dados simulados | Resultado esperado |
| --- | --- | --- |
| Base | Catálogo real, sem dados de economia | Monotrilho disponível. Trem Regional, Ônibus, Metrô e Loja de Conveniência exibem ⛔ Em breve. Rotas está ativa. Trilhas e Expedição estão bloqueadas. |
| Rótulo longo | Loja de Conveniência e descrições longas em português | Nome e indicação de disponibilidade ficam legíveis em janela estreita. A lista rola mantendo acesso ao rodapé. |
| Saldo ausente | Dados de economia ainda não carregados | Área de saldo oculta até haver dados. Nenhum valor da imagem de referência aparece como saldo do jogador. |
| Saldo técnico | Ouro 1.250, gemas 32, notas musicais 7 | Valores aparecem apenas na visualização identificada como QA. Uma abertura de rota ou retorno não altera o saldo. |
| Rota futura sem acesso | Metrô ilustrativo exige 20 notas, jogador de QA tem 7 | Indisponibilidade e motivo visíveis na variante de teste. O clique não inicia sessão nem grava save. |
| Bônus ilustrativo | Ônibus recebe indicador de recompensa de teste | O indicador tem rótulo textual compreensível. O dado não altera drop, recompensa ou combate. |

## Como validar

- **Leitura:** informar a disponibilidade por texto e símbolo, além da cor. Conferir contraste sobre as cinco cores e usar como referência 4,5:1 para texto comum e 3:1 para texto grande. Verificar acentos e nomes completos.
- **Entrada:** um clique ou toque abre o Monotrilho. Arrastar a lista não embarca. Bilhetes e abas bloqueados permanecem sem ação e com estado legível. Verificar alvos de interação de pelo menos 48 px no protótipo e registrar o tamanho percebido no aparelho.
- **Foco:** caso o piloto adote widgets navegáveis, dar foco visível a Voltar e às ações disponíveis, em ordem previsível. Uma indicação textual deve continuar disponível para os itens bloqueados.
- **Enquadramento:** conferir janela larga e estreita, retrato, áreas seguras e Movimento Reduzido. Voltar continua acessível sem sobreposição ao conteúdo.
- **Retorno:** manter na fila a regressão com batalha e tutorial, retorno à mesma página, novo embarque na primeira página e toque no Android. O teste com mouse da seta Voltar já foi aceito nos moldes 2D e 3D.

## Dependências

A montagem do piloto pode reutilizar o catálogo, a arte atual e os recursos locais. A tela final depende da arte em camadas, dos ícones e dos contratos de economia e recompensas. Trilhas e Expedição abrem telas próprias em etapa futura.

- [Mapa do TerminalCore](2026-11-28_terminalcore-mapa-navegacao_v01.md)
- [Referência visual do Terminal 2D](2026-11-27_terminal-2d-referencia-visual_v01.md)

## Abertura da variante de QA no Windows

Abra o RpgEngineProbe em modo jogo e entre no Terminal pelo Hub panorâmico. Use `-TerminalQA=Saldo` para exibir os três saldos fictícios, o requisito ilustrativo do Metrô e o bônus ilustrativo do Ônibus. Use `-TerminalQA=Base` para ver a identificação de QA com a área de saldo oculta. Ao abrir pelo atalho habitual, vale a prévia normal.

```powershell
& 'D:\Unreal Engine\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe' 'F:\Testes de jogos\Contos de Um Lobo - Jogo\rpg-urbano\experiments\unreal\RpgEngineProbe\RpgEngineProbe.uproject' -game -windowed -Presentation=Mobile -TerminalQA=Saldo
```

Para conferir a lista em tela larga, troque por `-Presentation=Desktop`. Os dados de QA são usados apenas no desenho da tela. O embarque do Monotrilho usa o fluxo existente.

**Verificado:** PNG atual de 2000 × 3000, espaço da faixa no recorte focado em três tamanhos simulados e compilação Win64. **Pendente:** inspeção visual em execução, navegação completa e toque.
