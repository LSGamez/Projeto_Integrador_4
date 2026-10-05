# Otimização aplicada — Cena_Menu

Concluída em 05/10/2026 no mapa `/Game/Cenas/Cena_Menu`.

## Antes e depois

| Item | Antes | Depois |
|---|---:|---:|
| Atores no mapa salvo | 210 | 185 |
| Atores de poeira | 26 | 1 com 26 instâncias |
| Slots nas malhas estáticas contadas | 240 | 215 |
| Luzes que projetam sombras | 9 | 3 |
| Controles com Tick na tela principal em repouso | 13 | 0 |
| Triângulos das malhas estáticas, LOD0 | 17.188 | 17.188 |

Slots de material não são draw calls medidos. A contagem de triângulos exclui os três Geometry Caches e não representa apenas a geometria visível por quadro. Instanciar a poeira mantém sua geometria e reduz atores, componentes e trabalho de atualização.

## Alterações

- `Poeira_01` usa um Instanced Static Mesh Component nativo, com 26 instâncias. `M_Poeira_GPU` anima as partículas por World Position Offset; limite de deslocamento de 14 cm e Bounds Scale de 1,2 acomodam o movimento. O ator não usa Tick, colisão ou sombras.
- Os treze controles ativam Tick ao receber alterações e desativam após a animação estabilizar. A oscilação do frasco selecionado continua ativa enquanto a opção está disponível.
- O ambiente usa um timer de 0,05 segundo (20 atualizações por segundo). O gestor continua lendo o cursor por quadro para preservar a resposta da interface.
- Vapor só atualiza quando foi renderizado recentemente. Os três Geometry Caches usam intervalo de Tick de 1/30 segundo e pausam quando saem da câmera, retomando quando visíveis.
- `Luz_01` a `Luz_06` iluminam sem projetar sombras. `Luz_07`, `Luz_08` e a Directional Light mantêm sombras. O ciclo de quatro segundos da luz perto da lousa permanece configurado.
- Interações e navegação permanecem nos quatro Blueprints existentes. O mapa foi reaberto e auditado com a ferramenta temporária de autoria fora de Plugins; não depende dessa ferramenta em execução.

## Verificação

- 16 casos de interação passaram, cobrindo os treze controles, os quatro botões Voltar e confirmações. Cada pressionamento aceito despachou uma ação.
- Hover sustentado por cinco segundos ficou estável, sem repetir o som. O frasco laranja elevou 5 cm. Os controles em repouso ficaram sem Tick.
- As 26 instâncias persistiram após salvar e reabrir o mapa.
- Os três frascos pausaram fora da câmera e retomaram quando visíveis.
- As telas principal, opções e créditos foram renderizadas e inspecionadas na qualidade baixa. As capturas estão em `Documentacao/Validacao_Menu_Otimizado`.

Os testes de entrada enviam eventos ao viewport da engine; não representam cliques físicos pela janela do Windows. Qualidade média e alta foram verificadas com ações temporárias de diagnóstico em PIE, sem executar a troca de renderização que havia provocado falhas de D3D12/memória de vídeo. Os assets salvos conservam suas ações reais. A confirmação de novo jogo foi exercitada sem chamar a API de progresso.

Não foi medido ganho de FPS, tempo de GPU ou memória em um build empacotado. Essas medidas na máquina alvo são necessárias para decidir reduções adicionais de texturas, materiais ou iluminação.

## Onde ajustar

- `BP_ControleLaboratorio → AnimacaoDaInteracao`: sequência de animação; funções `AcordarAnimacao` e `DormirSeEstavel` controlam o Tick.
- `BP_MenuLaboratorio → EfeitosDoAmbiente`: preparação e timer; `AtualizarAmbiente` controla vapor e visibilidade dos frascos. `IntervaloAmbiente` define o intervalo do timer.
- `Content/Laboratorio/Menu/Materiais/M_Poeira_GPU`: movimento de poeira no material.
- Outliner: `Laboratorio/Ambiente/Poeira_01` contém as partículas instanciadas.

## Backup

`C:\Users\Bruno\Projeto_Integrador_4\Saved\Backup_Otimizacao_Menu_20261005_132941`

Inclui o mapa, os assets do menu, a configuração e os geradores anteriores às alterações. A ferramenta temporária de autoria foi arquivada na subpasta `Ferramenta_Editor`.
