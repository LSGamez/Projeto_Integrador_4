> Este documento registra o inventário anterior às mudanças. Para o estado atual, consulte [Otimizacao_Menu_Aplicada.md](Otimizacao_Menu_Aplicada.md).

# Auditoria de otimização — Cena_Menu

Inspeção do mapa salvo em 05/10/2026. Não altera os assets e não mede FPS.

## Inventário

- 210 atores no mapa, incluindo câmeras, luzes, textos e controles.
- 17.188 triângulos LOD0 em malhas estáticas de atores não ocultos no mapa salvo. Exclui os três Geometry Caches dos frascos e malhas geradas durante a execução. Não representa a contagem efetivamente visível por quadro.
- Terminal_01: 416 triângulos e quatro slots de material. Sinal_01: plano de dois triângulos com material animado.
- 240 slots de materiais nas malhas estáticas contadas. Slots não equivalem a draw calls medidas; visibilidade, batching e passes de iluminação alteram o custo real.
- Dez luzes móveis: nove com Cast Shadows habilitado; oito delas são Rect Lights, uma é Directional Light. A luz pontual do terminal não projeta sombras.
- Três Geometry Caches para os frascos; cada arquivo ocupa cerca de 0,95 MB em disco. Isso não mede seu consumo de RAM ou GPU.
- 14 atores Blueprint com Tick habilitado: gestor e treze controles. O código dos controles atualiza materiais e transformações a cada quadro, inclusive sem uma guarda geral por Disponivel.
- 26 elementos de poeira e quatro de vapor atualizados pelo gestor.
- A pasta Content/Laboratorio contém 23,21 MB em arquivos .uasset, incluindo arquivos que podem não ser usados por este mapa. Não é tamanho do jogo empacotado nem VRAM consumida.

## Avaliação

A geometria é leve. A cena ainda tem oportunidades de otimização nas sombras dinâmicas, na quantidade de seções de material e no trabalho contínuo das animações. Sem um perfil de CPU/GPU não é possível afirmar qual deles limita o FPS.

## Prioridades propostas

1. Medir tempo de GPU e CPU com a tela principal, opções e créditos em um build do jogo na máquina alvo.
2. Rever Cast Shadows e alcance das luzes; manter sombras onde fazem diferença no visual.
3. Suspender atualizações de controles e efeitos invisíveis ou estabilizados, preservando transições e retorno do hover.
4. Consolidar materiais compatíveis e avaliar instanciamento de objetos repetidos.
5. Medir o custo dos Geometry Caches; se necessário, substituir deformações simples dos líquidos por materiais ou animações mais econômicas.

As falhas de renderização/memória de vídeo registradas nos testes de troca de qualidade são uma limitação da validação. Elas não demonstram, por si só, que a quantidade de polígonos é o gargalo.

Referência: https://dev.epicgames.com/documentation/en-us/unreal-engine/shadowing-in-unreal-engine
