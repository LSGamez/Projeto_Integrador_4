# Apresentação do laboratório em Blueprints

Projeto: `C:/Users/Bruno/Projeto_Integrador_4`

Mapa: `/Game/Cenas/Cena_Menu`

No Content Browser, abra `Laboratorio > Menu > Blueprints`.

## Ordem para apresentar

| Blueprint | Gráfico | O que explicar |
|---|---|---|
| `BP_MenuLaboratorio` | `Inicializacao` | BeginPlay configura o cursor, os cliques e a qualidade inicial. |
| `BP_MenuLaboratorio` | `AcoesDoTerminal` | O clique chega ao Switch on String, que escolhe a ação do botão. |
| `BP_MenuLaboratorio` | `NavegacaoECameras` | Set View Target with Blend movimenta a câmera com Ease In Out; os cliques ficam bloqueados durante a transição. |
| `BP_MenuLaboratorio` | `EntradaDoMouse` | Left Mouse Button consulta o alvo atual e chama ProcessarClique uma vez. |
| `BP_ControleLaboratorio` | `DefinirHover` | A entrada altera o estado e toca o som uma vez; chamadas repetidas não reiniciam o efeito. |
| `BP_ControleLaboratorio` | `AnimacaoDaInteracao` | FInterpTo suaviza o hover; funções separadas controlam materiais, elevação e avanço dos botões. |
| `BP_MenuLaboratorio` | `QualidadeDosGraficos` | Game User Settings aplica e salva Desempenho, Equilibrado ou Qualidade. |
| `BP_TextoLaboratorio` | `Inicializacao` | O texto 3D serve de prévia no editor. No jogo, o ator atualiza um Widget Component em espaço de tela. |
| `WBP_TextoLaboratorio` | `TextoLegivel` | Set Text, Set Font e Set Color and Opacity mantêm os textos legíveis mesmo no Low. |
| `BP_MenuLaboratorio` | `EfeitosDoAmbiente` | O fluxo chama AnimarPoeira e AnimarVapor. Abra as funções para ver os loops e os cálculos organizados. |

Todos esses gráficos têm comentários em português. Os atores de comportamento herdam de `Actor`, e o widget herda de `UserWidget` da Unreal. A lógica do menu está nos nós; não são Blueprints filhos das antigas classes de menu em C++.

## Demonstração no Play

1. Abrir `Cena_Menu` e iniciar Play.
2. Clicar em Configurações e mostrar a transição de câmera.
3. Passar o cursor sobre os frascos e selecionar Desempenho.
4. Mostrar que os rótulos continuam legíveis no Low.
5. Usar Voltar e abrir Créditos / Arquivos.
6. Mostrar os integrantes e o orientador na lousa.
7. Voltar ao terminal; Novo Jogo abre uma confirmação. Cancelar para não substituir o progresso salvo.

## Materiais e animações

O sinal ativo do monitor usa 22 barras animadas, em um único plano de dois triângulos. Abra `Materiais/TV/M_Sinal_Barras` no editor de materiais para mostrar Time, Sine, Floor, Frac e máscaras. O material usa nós visuais, sem código Custom/HLSL.

As animações dos líquidos importadas do Blender continuam sendo Geometry Caches. A luz da lousa mantém seu Level Sequence de quatro segundos. São recursos de animação da Unreal, separados dos Event Graphs do menu.

## Escopo da conversão

A lógica visual e interativa nova do laboratório foi convertida para Blueprints e UMG. A integração com a API de progresso que já existia no projeto continua usando `ProgressHttpActor` em C++. O evento `IniciarPeloMenu` encaminha o clique ao fluxo original de Novo Jogo.

Uma descrição fiel para a apresentação é: **“O menu do laboratório usa Blueprints e UMG, com integração ao sistema de progresso existente.”**

As classes anteriores `LabMenuDirector`, `LabMenuControl` e `LabMenuHUD` foram retiradas do módulo do jogo e preservadas em `Saved/Backup_Conversao_Blueprints/Source_Menu_Cpp`. A ferramenta que gerou os gráficos é exclusiva do editor e não faz parte da lógica executada pelo menu.

## Backup

`Saved/Backup_Conversao_Blueprints` contém o mapa anterior e os arquivos substituídos. Para migrar o laboratório, use Migrate no mapa com suas dependências. A integração de progresso também precisa do módulo original ou de uma integração equivalente no projeto de destino.


## Organização e TV restaurada

São quatro Blueprints. O vapor e a visibilidade dos frascos são geridos pelo BP_MenuLaboratorio; a poeira usa instâncias e animação no material. Os gráficos possuem blocos com comentários, execução alinhada e cálculos abaixo dos eventos. As variáveis estão agrupadas por câmeras, interface, interação, áudio e ambiente.

A TV usa fundo azul petróleo, painéis visíveis, texto verde claro e barras animadas. Os elementos acompanham o monitor por vínculo de parentesco. Set Input Mode Game and UI, cursor visível e Capture During Mouse Down encaminham o primeiro pressionamento ao menu.

Abra AcoesDoTerminal ou NavegacaoECameras na lista GRAPHS; o Construction Script permanece sem lógica de jogo.


## Hover e funções organizadas

No `BP_ControleLaboratorio`, `AnimacaoDaInteracao` apresenta a sequência principal. As funções `AtualizarDestaque`, `AtualizarMateriais`, `LevantarFrasco` e `AvancarBotao` mostram cada etapa separadamente.

- `Elevacao`: interpola apenas o cursor. Todos os frascos sobem 5 cm, inclusive o selecionado.
- `Destaque`: controla o brilho da seleção e do cursor, sem manter o frasco elevado.
- `AvancarBotao`: percorre as peças e chama `AnimarElementoBotao`.
- `AnimarElementoBotao`: posição → escala → texto, em uma linha de execução.
- `CalcularAvancoBotao`: até 6 cm na direção da câmera.
- `CalcularEscalaBotao`: fator máximo 1,055. O texto UMG acompanha com Set Render Scale.
- A área de clique fica fixa; os textos são Hit Test Invisible.

No `BP_MenuLaboratorio`, `EfeitosDoAmbiente` prepara um timer de 0,05 segundo. `AtualizarAmbiente` chama `AnimarVapor` e `AtualizarAnimacoesVisiveis`. Vapor tem funções separadas para fase, deslocamento, escala e orientação. A poeira usa `Poeira_01` com 26 instâncias e movimento no material `M_Poeira_GPU`.

Não foram adicionadas classes de Blueprint nem malhas para esses efeitos. Abra as funções na seção FUNCTIONS, além dos gráficos da seção GRAPHS.

Backup anterior a essas alterações: `Saved/Backup_Interacoes_BP_20261003_094503`.

## Cursor estável e clique único

A leitura do mouse é centralizada no `BP_MenuLaboratorio`:

1. `AtualizarCursor`: consulta o canal `MenuInteracao` uma vez por quadro.
2. `ResolverControleDoMouse`: Get Hit Result Under Cursor By Channel → Break Hit Result → Cast BP_ControleLaboratorio.
3. `AtualizarAlvoCursor`: muda o destaque somente quando muda o botão sob o mouse.
4. `DefinirHover`: ignora entradas repetidas e toca o som apenas quando passa de repouso para hover.
5. `EntradaDoMouse` → `ProcessarClique`: consulta a posição atual do cursor e executa uma única ação por pressão.

Os eventos nativos On Begin/End Cursor Over e On Clicked estão desativados neste menu, para evitar duas fontes alterando o estado. As peças animadas não participam do canal de interação; o Box Component mantém sua posição fixa. O efeito de avanço e ampliação continua ativo.

Em Project Settings → Collision há o canal de trace `MenuInteracao` (GameTraceChannel2, resposta padrão Ignore) e o perfil `MenuInterativa` (Query Only, bloqueia somente esse canal). Os volumes AreaInterativa usam esse perfil. Na configuração atual, o canal corresponde a TraceTypeQuery3.

Ao migrar para outro projeto, copie também essas duas configurações de Collision do DefaultEngine.ini. O Migrate dos assets não leva arquivos de configuração. Se o canal GameTraceChannel2 já estiver ocupado no destino, crie um canal livre e ajuste o trace em ResolverControleDoMouse e o perfil das áreas para o canal correspondente.

A lógica continua nos nós de Blueprint; não foi adicionado código C++ de menu ao jogo.


## Correção da captura do primeiro clique — 03/10/2026

Em `BP_MenuLaboratorio → Inicializacao`, a entrada agora usa:

- **Set Input Mode Game And UI**: Mouse Lock Mode = Do Not Lock, Hide Cursor During Capture = false, Flush Input = false.
- **Set Viewport Mouse Capture Mode = Capture During Mouse Down**.
- **Set Focus to Game Viewport**, seguido de Enable Input.

Não configure No Capture neste menu. Na versão 5.7, FSceneViewport::OnMouseButtonDown ignora IE_Pressed em um viewport de jogo sem captura quando o modo é NoCapture. O caminho de duplo clique é diferente, o que explicava o comportamento observado.

O processamento continua no grafo `EntradaDoMouse`, usando Left Mouse Button / Pressed → ProcessarClique. A função possui duas variáveis para diagnóstico: `CliquesProcessados` conta ações aceitas e `UltimaAcao` mostra o nome da última ação. Selecione a instância em execução no Debug Filter para observar esses valores.

O bloqueio de 1,25 segundo durante a transição de câmera permanece intencional. Clique no destino após a câmera parar. A área clicável continua fixa e independente da animação de hover.

O relatório Verificacao_Clique_Unico.json identifica os testes e suas limitações. Os testes automatizados chamam o processamento de entrada do viewport com a captura liberada antes de cada pressionamento; não representam cliques físicos pela janela do Windows. A confirmação de novo jogo é testada sem chamar a API de progresso.


### Limites da verificação de entrada

Os botões de qualidade média e alta são verificados com ações temporárias de diagnóstico apenas na instância PIE. Isso confirma a área atingida e um único despacho por pressionamento, sem aplicar a troca de renderização que provocou falhas D3D12/memória de vídeo. Os assets salvos conservam quality1 e quality2 e suas ligações a AplicarQualidade. A qualidade baixa é exercitada normalmente. A captura de janela do Windows também falhou, portanto não houve validação ponta a ponta com cliques físicos.


## Atualizações sob demanda — 05/10/2026

`BP_ControleLaboratorio → AnimacaoDaInteracao` termina em `DormirSeEstavel`. Após posição e destaque chegarem ao destino, o controle desativa Tick. `AcordarAnimacao` é chamada quando muda hover, tela ou seleção. O frasco selecionado continua oscilando enquanto estiver disponível.

No gestor, o cursor continua atualizando por quadro. O timer `AtualizarAmbiente` atualiza a cada 0,05 segundo; `AtualizarAnimacoesVisiveis` pausa e retoma os Geometry Caches conforme visibilidade recente. Os controles continuam usando a área de clique fixa e o mesmo processamento de clique único.

Há quatro Blueprints; `Poeira_01` é um Actor nativo com Instanced Static Mesh Component. A ferramenta de autoria não precisa estar instalada no projeto. Veja `Otimizacao_Menu_Aplicada.md` e os relatórios JSON para números e limites dos testes.


## Novo jogo e tratamento de erro — 08/10/2026

`BP_MenuLaboratorio → AcoesDoTerminal → IniciarExperimento` chama `NovoJogo` diretamente no serviço `BP_MenuHttpActor`. A referência ao botão antigo não é usada para iniciar a API. `ExperimentoFalhou` libera a espera e atualiza a mensagem do terminal.

No Blueprint de serviço já existente, `InicioEFalhaDoExperimento` organiza os dois callbacks: confirmação → entrada de gameplay → Open Level; falha → mensagem no gestor. A espera HTTP padrão é 45 segundos; URL e intervalo são editáveis na categoria Progresso. O transporte permanece no módulo C++ HTTP existente.

Consulte `Correcao_NovoJogo.md` para os testes, a verificação da API e a ocorrência do reinício real do save no teste inicial.
