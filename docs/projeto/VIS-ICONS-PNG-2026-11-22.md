# Ícones PNG e preparação do tutorial — 22/11/2026

## Entregue

- 8 habilidades convertidas JPG → PNG RGBA2048x2048, sem escala/corte. Removido apenas branco quase neutro conectado à borda, com ajuste restrito de franja de um pixel. Partes claras internas não são tratadas como fundo global.
- 9 status com alpha, centralizados em384x384, preservando353x307 do desenho original.
- Logo1254x1254 com alpha preservado e retrato1254x1254 copiado para avatar-felipe-v01.png. O retrato mantém fundo colorido, adequado a avatar; não foi segmentado nem convertido em recorte de personagem.
- Derivados válidos: assets/runtime/ui/2026-11-22-v02/. Catálogo: data/presentation/ui-assets-2026-11-22.json. Versão01 foi ensaio descartado por escala de DPI; não está referenciada no catálogo.
- Fontes de imagem em recebidos preservadas. Documentos de mapeamento corrigidos por autorização do usuário para refletir JPG/caminhos/nomes reais e os derivados PNG.

## Verificação

19 derivados conferidos por hash/dimensões/alpha. Nos17ícones, comparação de todos os pixels escuros ou cromáticos protegidos contra a fonte: zero alterações. Branco exterior e antialias neutro adjacente são as únicas regiões elegíveis à mudança. Folhas antes/depois inspecionadas localmente em build/ui-assets-2026-11-22; contraste da A2 inativa continua dependente do tamanho real na HUD. Não houve ajuste artístico ou ampliação de glifo.

PowerShell/.NET local, sem ferramentas externas, internet, geração por IA, editor ou instalação. Scripts preparar-icones-locais.ps1 e verificar-icones-locais.ps1; conversão recusa sobrescrever derivados existentes. Imagens e folhas de contato não entram no Git; transfira-as separadamente para outra cópia do projeto.

## Tutorial

Sangramento inicial no inimigo da demonstração de A1 aprovado. P1 de escolha do debuff foi resolvido; permanecem sequência guiada, variante jovem/IDs, efeitos temporários e persistência específica. Núcleo comprovado com kit atual: 11/11 grupos Pilot, incluindo A1 satisfazendo sua condição sobre alvo já com Sangramento e piloto normal sem status inicial.

O teste usa InitialRuntime, 2turnos e1250pontos do ATK2500 do Felipe atual; não presume esses números para o futuro jovemATK1350. Não altera requires_target_debuff, chance de efeito, imunidade, relógios ou saves. O tutorial narrativo novo e a HUD não foram integrados nesta entrega. Nenhum código runtime alterado, portanto não houve necessidade de compilação Unreal.

## Continuidade

1. Integrar catálogo/imagens no adaptador HUD em entrega própria, mantendo relação de aspecto e molduras mecânicas separadas da arte.
2. Usar decisões restantes do tutorial para montar o encontro guiado; não iniciar evento clássico por inferência.
3. Aceite visual de ícones em tamanho real fica aberto. Demo~28% mantida.

> Continuidade23/11: derivados menores integrados à HUD, dois tutoriais ligados aos menus e conclusão local própria; ver INTEGRACAO-VIS-TUTORIAIS-2026-11-23.md. As indicações anteriores de integração ainda futura são históricas; aceites gráficos continuam pendentes.
