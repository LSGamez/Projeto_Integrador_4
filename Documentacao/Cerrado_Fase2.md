# Cenário de Cerrado — fase 2

## Localização

O cenário novo está no level `/Game/Cenas/Cena_Fase2`, que mantém o mesmo nome usado pelo jogo. Os assets estão em `/Game/Fase2/Cerrado`, com pastas para `Malhas`, `Materiais`, `Texturas` e `Instancias`.

O Landscape antigo, as 105 malhas antigas de ponte/árvores e a folhagem antiga foram retirados desse level. Seus arquivos compartilhados não foram apagados do Content Browser, pois podem ser usados por outras cenas. O cenário novo contém o chão, a trilha, os paredões, a ponte, a flora nativa estilizada e os resíduos de polímeros criados no Blender.

## Organização e custo

| Item | Quantidade |
|---|---:|
| Triângulos do cenário novo, somando as instâncias | 13.804 |
| Elementos do cenário | 795 |
| Malhas reutilizáveis importadas | 301 |
| Objetos de chão/trilha/paredões no Outliner | 72 |
| Elementos em instâncias nativas de Foliage | 723 |
| Grupos de instâncias | 242 |
| Atores totais do level, incluindo a lógica existente | 83 |
| Materiais compartilhados do cenário novo | 4 |
| Texturas de superfície | 128 × 128 e 256 × 256 |

A contagem de triângulos acima exclui os personagens e os recursos internos do céu da Unreal. Os arquivos novos do kit ocupam aproximadamente 6,3 MiB no disco; isso não representa uma medição de memória ou FPS durante o jogo.

As malhas usam LOD 0 de baixa complexidade, sem Nanite, sem lightmap novo e com a resolução de campos de distância zerada. Os materiais de plantas e plásticos são opacos e `Two Sided`. O solo multiplica a textura por `Vertex Color`, enquanto plantas e resíduos usam `Vertex Color` diretamente na cor base.

No Outliner, a pasta `Fase2_Cerrado` separa o chão e os paredões. O ator `Vegetacao_Residuos_Ponte`, na pasta `Instancias`, contém os elementos repetidos. Para selecionar e mover uma instância, abra o modo **Foliage → Select**. As malhas continuam disponíveis individualmente no Content Browser e no Blender.

## Colisão e navegação

- Chão, paredões e peças estruturais da ponte têm colisão estática com a geometria simples existente: `Use Complex Collision As Simple`.
- Árvores e pedras grandes têm colisão. Capim, flora baixa e resíduos pequenos usam `NoCollision`.
- A malha de navegação foi reconstruída, mantendo o volume original da arena da onça.
- A entrada e o controlador da arena foram preservados nas posições originais.
- Em 12/10/2026 foram removidos os limites de descarte de 350 m para árvores/pedras, 90 m para capim e 140 m para detalhes. `Cull Distance` dos tipos de Foliage e as distâncias inicial/final dos componentes estão zeradas, sem um limite manual por distância.
- Instâncias e malhas low poly continuam em uso. A otimização normal da Unreal por enquadramento, oclusão e tamanho na tela continua ativa; elementos muito pequenos ao longe podem ocupar menos de um pixel.

## Iluminação

A iluminação ambiente existente foi ajustada para um SkyLight móvel com intensidade 0,65, cubemap de luz diurna da própria Unreal e resolução 64. A captura contínua está desligada. Não foram adicionados atores de luz extras. A luz direcional existente foi mantida.

## Lógica preservada

O GameMode continua sendo `/Game/Servicos/GM_Polymer`. Foram preservados `BP_Onca`, `BP_BossArenaManager`, `PlayerStart`, a câmera da cena, o volume de navegação e o ator de NavMesh. Os arquivos das Blueprints da onça, do controlador de arena e do GameMode não foram alterados. A troca foi realizada no cenário e em seus materiais.

A versão salva foi reaberta para conferir as contagens, as posições e as configurações das malhas. Não foi executada uma partida para medir desempenho ou testar o combate.

## Cópia de segurança

A versão anterior está em:

`C:\Users\Bruno\Projeto_Integrador_4\Saved\Backup_Fase2_Cerrado_20261012_005806`

Essa pasta contém o `Cena_Fase2.umap` original e seus recursos compartilhados de Landscape. A fonte editável do cenário novo permanece em:

`C:\Users\Bruno\Documents\Codex\2026-09-29\consegue-conectar-o-x20\outputs\Fase2_Cerrado\Fase_2_Cerrado_LowPoly.blend`

O pacote e os registros de instalação ficam na subpasta `Retorno_Unreal` junto dessa fonte. Para enviar a alteração pelo Git, inclua `Content/Cenas/Cena_Fase2.umap`, a pasta `Content/Fase2/Cerrado` e este documento.
