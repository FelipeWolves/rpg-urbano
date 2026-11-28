# Terminal modular e diagnóstico local — proposta

Estrutura proposta na Unreal: Content/Terminal/Common para contratos visuais e recursos compartilhados; Content/Terminal/Light2D para mapa, cartões e ícones; Content/Terminal/Full3D para mapa, malhas, materiais e áudio próprios. O projeto atual mantém os catálogos locais em data/presentation. A prova de conceito deve definir labels do Asset Manager, dependências de cook e três grupos (base comum + 2D; 3D opcional por plataforma; conteúdo futuro por região). Validar suporte para a configuração real da Unreal, IoStore e cada canal de distribuição. [Asset Manager da Unreal](https://dev.epicgames.com/documentation/unreal-engine/asset-management-in-unreal-engine) · [Chunks](https://dev.epicgames.com/documentation/en-us/unreal-engine/preparing-assets-for-chunking-in-unreal-engine).

## Fluxo de instalação e troca

Oferta local descreve plataforma, versão de build e de catálogo, ID do pacote, dependências, bytes compactados/instalados e hash. Verificar autenticidade de manifesto e compatibilidade antes de baixar. Baixar em área temporária própria, verificar tamanho/hash/assinatura aplicável e ativar atomicamente. A seleção oferece download, progresso, cancelar e acesso rápido 2D. Em falha, preservar pacote ativo e 2D. Trocar em runtime preservando rota/preferências, descarregando mapa e referências antes de ativar o outro. Remoção pelo menu afeta apenas conteúdo opcional validado no diretório do app, após voltar ao 2D e desmontar. Diretório base, saves, pacotes em uso e outros arquivos ficam protegidos. Se a loja gerenciar o pacote, usar seu fluxo aprovado. Testar corrupção, perda de rede, espaço insuficiente, versão incompatível, cancelamento, atualização, baixa memória e remoção.

## Triagem local provisória

| Perfil | RAM | Gráficos | Espaço |
|---|---|---|---|
| Mobile 2D padrão | 3 a 4 GB como alvo inicial | OpenGL ES 3 ou Vulkan no Android; Metal no iOS conforme backend suportado | Base instalada e espaço de operação a medir |
| Mobile 3D opcional | Pelo menos 6 GB totais e pressão de memória aceitável | Vulkan estável ou Metal, com checagem de capacidades | Download temporário + tamanho instalado + margem operacional, a partir do manifesto e medição |
| Windows 3D | 8 GB, 16 GB recomendados, CPU quad-core como alvo | DX11/DX12 ou Vulkan conforme build validado | SSD e reserva a medir |

Avaliar RAM total/disponível, API/capacidades e espaço sem transmitir o diagnóstico. Se algum dado estiver ausente, insuficiente ou indicar alto risco, recomendar 2D. Um checkpoint de desempenho pode corrigir a recomendação; decisão do jogador segue disponível quando compatível. Android 10, iOS 14 e Windows 10/11 são alvos propostos e dependem da matriz de suporte da engine, lojas e dispositivos. API 36 no emulador não certifica esses mínimos. Medir pico de RAM, tempo de carregamento, estabilidade térmica e draw calls. ASTC mobile e BC7 desktop são candidatos a validar por plataforma e perfil. O APK atual inclui os dados locais e ainda não usa entrega modular.
