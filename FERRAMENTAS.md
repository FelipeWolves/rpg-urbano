# Ferramentas — AMB-01 validada

## Situação atual em 18/11/2026

O usuário escolheu e instalou manualmente **Visual Studio Community 2026**. A proposta histórica de Build Tools VS2022 está superada. Nenhuma instalação ou download foi feito pelo agente.

| Componente | Versão medida |
|---|---|
| Visual Studio | 18.12.1; instalação 18.12.12412.261; estável |
| MSVC selecionado pelo VsDevCmd | 14.52.37025 |
| Windows SDK selecionado | 10.0.26100.0 |
| Alvo | Windows x64, C++20 |

**Comando validado na raiz:** `.\scripts\verificar-ambiente.ps1`.

tools/diagnostico.cpp compilou, vinculou e executou com sucesso. Verificou concepts, std::span, std::ranges, ordenação/soma, ponteiros de 64 bits e chamada de API do Windows via SDK. Resultado: `TOOLCHAIN_OK cxx=202002 msvc=195237029 pointer_bits=64 sum=17`; código de saída zero. /W4 /WX ativos, sem avisos ou erros. Isso valida as ferramentas, não um motor de combate.

AMBIENTE.json contém versões/resultado. Objetos, executável, wrapper .cmd, temporários e log ficam em build/diagnostico/, ignorado pelo Git. Ambiente x64 inicializado pelo VsDevCmd em processo filho; sem alteração de PATH global. Compilação com runtime estático /MT. Não foi aberta ou instalada engine, nem escrito gameplay.

Próximo bloco vigente em ESTADO.md; COM-01 e TIME-01 já usam este ambiente. Não repetir o diagnóstico sem mudança de ambiente ou falha relevante. Validar compatibilidade com a engine quando escolhida.

---

## Histórico — FUN-02 antes da instalação manual

Verificação seletiva realizada em 18/11/2026. Nenhum programa instalado, nenhuma engine aberta e nenhum código de gameplay escrito.

## Disponível e confirmado

| Item | Resultado |
|---|---|
| Windows | Windows 11 Pro |
| RAM física | 34.144.575.488 bytes, aproximadamente 31,8 GiB |
| Processadores lógicos | 16 |
| GPU enumerada | NVIDIA GeForce RTX 3060, driver informado 32.0.16.2114 |
| Git | Disponível; repositório local main, sem remoto ou primeiro commit |
| .NET | Runtimes Microsoft.NETCore.App 8.0.34/9.0.23 e Microsoft.WindowsDesktop.App 9.0.23 |
| .NET SDK | Nenhum retornado por `dotnet --list-sdks` |

AdapterRAM retornado por WMI não foi usado para inferir VRAM: o campo pode truncar capacidades modernas. Capacidade dedicada, suporte a recursos gráficos específicos e desempenho do editor não foram validados. Identificar uma RTX 3060 não substitui medir desempenho do protótipo.

## Não encontrado / não confirmado

- `cl`, `clang++`, `g++`, `cmake`, `ninja` e `vswhere` não encontrados no PATH.
- Diretórios padrão de Visual Studio, instalador vswhere, CMake, LLVM, MSYS2 e includes de Windows SDK não encontrados nos caminhos consultados.
- Nenhum editor Unreal utilizável confirmado no PATH, nas instalações padrão e nos registros específicos consultados. Um registro antigo foi encontrado sem editor presente; não é evidência de engine pronta. Seu destino não foi usado ou incluído no projeto.
- Não houve busca em todas as unidades. Instalações portáteis ou em caminhos personalizados continuam possíveis; um caminho fornecido pelo usuário pode ser verificado antes de instalar.

As primeiras leituras do registro foram bloqueadas pelo ambiente restrito. As leituras aprovadas fora dessa restrição confirmaram o inventário seletivo. Isso não envolveu alteração de registro ou execução de jogos, launchers ou editores.

## Caminho mínimo definido

Núcleo em **C++20**, independente da Unreal, com biblioteca padrão. Compilar para Windows x64 usando MSVC e SDK do Windows. Sem CMake, Ninja, framework de testes externo ou IDE completos como requisitos do primeiro combate. Não mudar para C# ou PowerShell apenas porque um runtime está disponível: isso criaria uma etapa de integração diferente da proposta de núcleo C++.

Proposta histórica superada: Build Tools for Visual Studio 2022 linha 17.14, MSVC v143 e Windows SDK. O usuário optou por VS2026. Não executar a proposta antiga. A engine futura deverá revalidar o toolset instalado, sem garantia de compatibilidade permanente.

Não há estimativa confiável do tamanho do download antes de consultar a seleção real do instalador. Aprovar a instalação e sua origem Microsoft antes de baixar/executar. Não aceitar alteração automática de seleção ou reinicialização sem informar o usuário. Validar assinatura do instalador e componentes escolhidos antes da execução.

## Verificação após instalação — plano histórico, agora concluído

1. Identificar a instalação pelo vswhere instalado pela Microsoft.
2. Inicializar o ambiente x64 com o script de desenvolvimento fornecido pela própria instalação, apenas na sessão; não editar PATH global manualmente.
3. Compilar um programa diagnóstico mínimo próprio com `/std:c++20 /EHsc /W4 /WX`, usando somente biblioteca padrão. Esse programa verifica compilação, link e execução local; não implementa combate.
4. Manter objetos/executável em build/ e verificar código de saída e resultado esperado.
5. Somente depois registrar o comando efetivamente validado e liberar COM-01.

Esse plano foi executado em AMB-01 por verificar-ambiente.ps1, descrito no início. O script falha se faltar ferramenta e não instala nada. A primeira versão do núcleo e seu verificador poderão ser compilados diretamente, sem sistema de build adicional.

## Comandos de diagnóstico efetivamente executados

`dotnet --list-sdks` e `dotnet --list-runtimes`; descoberta PowerShell de comandos e caminhos padrão; consultas somente de leitura de GPU/Windows e registros de instalação de engine; `git status` na fundação. A consulta .NET foi feita com opt-out de telemetria na sessão. Nenhuma compilação ou teste de C++ foi possível nesta entrega.

## Fontes oficiais consultadas

- [Microsoft — ferramentas C++ pela linha de comando](https://learn.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=msvc-170): Build Tools pode ser usado sem instalar Visual Studio IDE; ambiente depende dos scripts do toolset.
- [Epic — configuração e compatibilidade do Visual Studio](https://dev.epicgames.com/documentation/en-us/unreal-engine/setting-up-visual-studio-development-environment-for-cplusplus-projects-in-unreal-engine): matriz por versão da Unreal e requisitos de SDK/toolchain. Revalidar ao escolher a engine.

## Resultado histórico de FUN-02

Na conclusão de FUN-02 o ambiente ainda não compilava. Essa condição foi resolvida pela instalação manual e AMB-01 descritas no início. COM-01 ainda não começou.
