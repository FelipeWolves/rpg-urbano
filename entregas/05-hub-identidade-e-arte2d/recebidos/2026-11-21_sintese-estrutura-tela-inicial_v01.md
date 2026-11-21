# Síntese Documental — Estrutura da Tela Inicial do Jogo (`VIS / HUB`)
**Projeto:** Contos de um Lobo na Cidade (Codinome LOBO / *Mundus Lupis Disco*)  
**Versão de referência:** 0.1 | **Data:** 21 de novembro de 2026  
**Status na entrega:** `DECISÃO DESEJADA` (visão consolidada de produto) + `PROPOSTA` (expansões futuras)  
**Referência visual anexa:** `BG-tela-inicial-Prototipo-02/` (8 variantes limpas `bg_home_*.png` + 1 gabarito indicativo de layout)

---

## 1. Objetivo e Escopo deste Documento
Registrar as decisões conceituais e espaciais estabelecidas até 21/11/2026 para a tela inicial (Hub) do protótipo de *Contos de um Lobo na Cidade*, consolidando:
- Organização panorâmica inspirada na navegabilidade de *Dislyte*, com identidade visual e literária própria.
- Mapeamento dos **7 pontos principais de acesso** na cena urbana.
- Distinção clara entre **o que é estrutura espacial consolidada** e **o que permanece como expansão futura** (sem exigir implementação imediata além dos contratos já vigentes `ENT-04`, `ENT-05`, `ENT-11`, `ENT-13` e `AUT-01` a `AUT-04`).

---

## 2. Diretrizes Gerais de Ambientação e Interface
A tela principal representa uma **cidade humana contemporânea, realista e funcional**, na qual pontos específicos dão acesso aos sistemas, atividades e histórias do universo.

| Elemento | Diretriz Estabelecida | Classificação |
|---|---|---|
| **Arquitetura** | Urbana contemporânea combinada com edifícios históricos | `DECISÃO DESEJADA` |
| **Identidade Visual** | Realismo cinematográfico, sem estética excessivamente futurista | `DECISÃO DESEJADA` |
| **Atmosfera** | Cidade viva, com pessoas, vegetação, estabelecimentos e iluminação funcional | `DECISÃO DESEJADA` |
| **Fantasia** | Elementos simbólicos e sobrenaturais discretos (sem mundos sobrenaturais misturados abertamente à paisagem urbana) | `DECISÃO DESEJADA` |
| **Perspectiva** | Vista panorâmica com pan horizontal / âncora responsiva (`ENT-05`), permitindo identificar diferentes áreas interativas | `DECISÃO DESEJADA` |
| **Separação UI × Fundo** | Indicadores, botões e painéis são widgets interativos separados da arte de fundo (nunca embutidos nos `.png` de background) | `DECISÃO DESEJADA` |
| **Paleta e Tipografia Runtime** | Preservar no runtime atual os tons em azul noturno, preto e prata e fontes `Cormorant Garamond` + `Lora` (`ENT-13`) | `DECISÃO DESEJADA` |
| **Música e Expansibilidade** | Música como parte central da identidade; layout preparado para receber novos locais/eventos sem reconstruir o Hub | `DECISÃO DESEJADA` |

---

## 3. Organização Funcional dos 7 Pontos de Acesso (Panorama)
A composição visual aprovada (com **Lab - Exp / Biblioteca / Praça** à esquerda, **Terminal** ao centro e **Arena PvP / Pub** à direita) estrutura sete pontos principais de interação:

| Local | Função Principal | Localização na Composição | Situação no Protótipo Atual |
|---|---|---|---|
| **Terminal** | Hub de deslocamentos e destinos (por modal de transporte) | Edifício contemporâneo central | Conceito atualizado (`DECISÃO DESEJADA`); seleção de modais/destinos em definição |
| **Arena PvP** | Batalhas entre jogadores | Região sob o viaduto ferroviário, à direita | Mapeado; bloqueado/futuro (`PROPOSTA`) |
| **Pub** | Música, convivência e Quadro Negro de Caçadas | Rua rebaixada, no canto inferior direito | Mapeado; bloqueado/futuro (`PROPOSTA`) |
| **Praça** | Lojas, consumíveis, armas, itens e trocas | Área comercial à esquerda | Mapeado; economia/lojas bloqueadas/futuras (`PROPOSTA`) |
| **Games** | Minijogos, recompensas, buffs e futuro *DJ Contest* | Estabelecimento recreativo próximo à Praça | Mapeado; bloqueado/futuro (`ENT-14` / `PROPOSTA`) |
| **Lab - Exp** | Eventos especiais, temporários e protótipo do *Modo Clássico* | Região próxima à Biblioteca, à esquerda | Mapeado; ativo apenas conforme recorte de evento da demo (`ENT-05` / `ENT-14`) |
| **Biblioteca** | Atlas, registros dos 4 Livros, glossário, relações e progressão | Edifício histórico à esquerda (entalhe de lobo uivando com uma asa) | Mapeado; bloqueado/futuro (`PROPOSTA`) |

### Detalhamento por Local
- **3.1. Terminal Multimodal:** Deixa de ser um botão direto para História ou Provações. Ao selecioná-lo, abrirá uma tela secundária de escolha de **modal de transporte**, encaminhando o jogador ao destino correspondente (mundos, áreas de batalha, regiões). *Correspondência exata entre modais e destinos: `EM ABERTO`.*
- **3.2. Arena PvP:** Posicionada sob o viaduto ferroviário à direita. *Necessidade de tela intermediária: `EM ABERTO`.*
- **3.3. Pub:** Entrada abaixo do nível da rua por passagem rebaixada, placa de madeira de inspiração medieval com inscrição *Pub* e elementos mágicos discretos. Interior previsto com bar, piano de madeira para músicas das histórias, espaço para futuros instrumentos e **Quadro Negro de Caçadas** (com atendente para Chamados/missões).
- **3.4. Praça:** Centro comercial (consumíveis, armas, utilidades e trocas). *Economia, moedas, estoques e desbloqueios: `EM ABERTO`.*
- **3.5. Games:** Atividades recreativas e minijogos com recompensas/buffs; hospedará futuramente o **modo musical inspirado no DJ Contest** (`ENT-14`), separado do Pub.
- **3.6. Lab - Exp:** Espaço para eventos especiais/temporários, incluindo o protótipo futuro do **Modo Clássico** de batalhas por turnos (`ENT-14`). Localização próxima à Biblioteca (com faixas/painéis de exposição), sujeita a ajuste futuro.
- **3.7. Biblioteca:** Sistema de Atlas e conhecimento abrangendo os **4 volumes atuais de *Contos de um Lobo na Cidade***, livros futuros e obras menores (personagens, glossários, cenas, acontecimentos, relacionamentos e árvores de correlação, com recompensas por conclusão de registros). Fachada com entalhe de **cabeça de lobo uivando para cima acompanhada por uma única asa**.

---

## 4. Variantes Visuais de Tempo e Clima (`BG-tela-inicial-Prototipo-02`)
Foram produzidas **8 variantes limpas (sem texto)** da mesma composição urbana, acompanhadas de **1 gabarito indicativo** com exemplos de posicionamento dos textos clicáveis:

| Variante | Condição Ambiental / Iluminação |
|---|---|
| **Amanhecer** | Luz inicial do sol |
| **Manhã ensolarada** | Aproximadamente 9h, céu limpo |
| **Tempestade forte** | Nuvens escuras, raios, trovões e granizo |
| **Chuva diurna** | Chuva comum, sem tempestade severa |
| **Noite com lua minguante** | Céu limpo e estrelado |
| **Noite com lua cheia** | Céu limpo e lua cheia |
| **Noite sem lua** | Céu limpo, sem lua visível |
| **Noite chuvosa** | Céu totalmente encoberto (sem estrelas nem Via Láctea), com chuva e poças naturais irregulares com reflexos coerentes |

- **Diretriz de Iluminação Noturna:** Predominantemente branca ou levemente azulada, evitando excesso de luz amarelada; luz lunar branca e natural.
- **Convenção de Nomenclatura:** `bg_home_[horario]_[clima]_[variante].png` (ex.: `bg_home_morning_sunny.png`, `bg_home_day_rain.png`, `bg_home_night_clear_full_moon.png`, `bg_home_night_rain_overcast.png`).

---

## 5. Funcionalidades Adiadas, Reservadas ou em Aberto
| Funcionalidade | Situação Atual |
|---|---|
| **União** | Reservada para possível tela alternativa futura associada a conteúdo literário posterior |
| **Show Temático** | Adiado, sem função definitiva |
| **Provações** | Poderá ser acessada futuramente por destinos do Terminal (vínculo final `EM ABERTO`) |
| **História jogável** | Prevista como conteúdo narrativo, acesso definitivo em organização |
| **Outros mundos** | Associado ao sistema de viagens do Terminal (destinos/regras `EM ABERTO`) |
| **Clima e horário automáticos** | Possibilidade futura (`ENT-13`); não vinculada à implementação inicial |
| **Modo Clássico** | Previsto inicialmente como evento no *Lab - Exp* (`ENT-14`) |
| **DJ Contest** | Previsto como futura atividade principal na área *Games* (`ENT-14`) |

---

## 6. Nota de Sincronização com a Fila de Desenvolvimento
- Este documento consolida a visão arquitetural e visual atual do Hub sem invalidar o recorte piloto fechado (`ENT-11`) nem reescrever as cores/fontes executáveis já validadas (`ENT-13`).
- Os locais ainda não contratados devem permanecer como **estados bloqueados / não-interativos ou informativos ("Em breve")** no protótipo até a abertura de seus respectivos blocos de engenharia.