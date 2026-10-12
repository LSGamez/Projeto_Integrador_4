# Cenário de Cerrado — fase 2

## Localização

O cenário atual está no level `/Game/Cenas/Cena_Fase2`, que mantém o mesmo nome usado pelo jogo. A revisão sem árvores verdes usa assets em `/Game/Fase2/Cerrado_Revisado`, com pastas para `Malhas`, `Materiais`, `Texturas` e `Instancias`. A pasta `/Game/Fase2/Cerrado` guarda os assets da instalação anterior, que foram retirados desse level.

O Landscape antigo, as 105 malhas antigas de ponte/árvores e a folhagem antiga foram retirados desse level. Seus arquivos compartilhados não foram apagados do Content Browser, pois podem ser usados por outras cenas. O cenário novo contém o chão, a trilha, os paredões, a ponte, a flora nativa estilizada e os resíduos de polímeros criados no Blender.

## Organização e custo

| Item | Quantidade |
|---|---:|
| Triângulos do cenário atual, somando as instâncias | 10.286 |
| Elementos do cenário | 1.050 |
| Malhas reutilizáveis importadas | 278 |
| Objetos de chão/trilha/paredões no Outliner | 72 |
| Elementos em instâncias nativas de Foliage | 978 |
| Grupos de instâncias | 206 |
| Atores totais do level, incluindo a lógica existente | 83 |
| Materiais compartilhados do cenário novo | 4 |
| Texturas de superfície | 128 × 128 e 512 × 512 |

A contagem de triângulos acima exclui os personagens e os recursos internos do céu da Unreal. Não foi medida memória ou taxa de quadros durante uma partida.

As malhas usam LOD 0 de baixa complexidade, sem Nanite, sem lightmap novo e com a resolução de campos de distância zerada. Os materiais de plantas e plásticos são opacos e `Two Sided`. O solo usa diretamente `T_Solo_Cerrado_Vermelho_Verde`, com `sRGB=False`, sampler `Linear Color` e UVs normalizadas. Nos materiais `M_Cerrado_Nativo` e `M_Plastico_Gasto`, os canais RGB de `Vertex Color` são convertidos de sRGB para linear antes da cor base, usando a função `sRGBToLinear` da Engine. Isso corresponde aos valores sRGB exportados no FBX e evita cores clareadas. A textura linear do solo não recebe essa conversão novamente.

Na revisão atual, foram retiradas as 111 árvores verdes da versão Blender para que o usuário acrescente árvores prontas da Unreal. Permanecem 11 ipês amarelos, 10 ipês roxos, 8 árvores secas e 650 moitas de capim. As duas pequenas trilhas e os ipês maiores indicam pontos de referência para itens da bugiganga no acesso e na saída da ponte; não foi criada programação de coleta desses itens.

No Outliner, a pasta `Fase2_Cerrado` separa o chão e os paredões. O ator `Vegetacao_Residuos_Ponte`, na pasta `Instancias`, contém os elementos repetidos. Para selecionar e mover uma instância, abra o modo **Foliage → Select**. As malhas continuam disponíveis individualmente no Content Browser e no Blender.

## Colisão e navegação

- Chão, paredões e peças estruturais da ponte têm colisão estática com a geometria simples existente: `Use Complex Collision As Simple`.
- Árvores e pedras grandes têm colisão. Capim, flora baixa e resíduos pequenos usam `NoCollision`.
- A malha de navegação foi reconstruída, mantendo o volume original da arena da onça.
- A entrada e o controlador da arena foram preservados nas posições originais.
- Em 12/10/2026 foram removidos os limites de descarte de 350 m para árvores/pedras, 90 m para capim e 140 m para detalhes. `Cull Distance` dos tipos de Foliage e as distâncias inicial/final dos componentes estão zeradas, sem um limite manual por distância.
- Instâncias e malhas low poly continuam em uso. A otimização normal da Unreal por enquadramento, oclusão e tamanho na tela continua ativa; elementos muito pequenos ao longe podem ocupar menos de um pixel.

## Iluminação

A iluminação ambiente existente foi ajustada para um SkyLight móvel com intensidade 0,65, cubemap de luz diurna da própria Unreal e resolução 64. A captura contínua está desligada. Não foram adicionados atores de luz extras. Na correção das cores, a luz direcional foi calibrada para intensidade 5. As configurações originais de exposição e do volume de pós-processamento permanecem em uso; o volume auxiliar de exposição usado durante o ajuste foi removido.

A correção de materiais e iluminação foi salva em 12/10/2026. Os registros e prévias atuais ficam em `outputs/Fase2_Cerrado/Correcao_Cores`, junto à fonte Blender: `Correcao_Cores.json`, `Cerrado_Cores_Corrigidas.png` e `Ponte_Cores_Corrigidas.png`. A geometria permanece com 1.050 elementos e 10.286 triângulos. O Blender e a Unreal usam renderização e iluminação diferentes; a correção trata a interpretação das cores e reduz o excesso de brilho, sem prometer imagens idênticas entre os programas.

## Lógica preservada

O GameMode continua sendo `/Game/Servicos/GM_Polymer`. Foram preservados `BP_Onca`, `BP_BossArenaManager`, `PlayerStart`, a câmera da cena, o volume de navegação e o ator de NavMesh. Os arquivos das Blueprints da onça, do controlador de arena e do GameMode não foram alterados. A troca foi realizada no cenário e em seus materiais.

Antes de salvar a revisão, foram conferidas as contagens, a ausência de componentes do cenário anterior e as referências dos atores de jogabilidade. O relatório confirmou 1.050 elementos, 10.286 triângulos e distâncias de descarte zeradas. Não foi executada uma partida para medir desempenho ou testar o combate.

## Cópia de segurança

A versão anterior está em:

`C:\Users\Bruno\Projeto_Integrador_4\Saved\Backup_Fase2_Sem_Arvores_20261012_131438`

Essa pasta contém o `Cena_Fase2.umap` imediatamente anterior à substituição e os relatórios da instalação anterior. A cópia original do Landscape permanece no backup histórico `Backup_Fase2_Cerrado_20261012_005806`. A fonte editável do cenário atual permanece em:

`C:\Users\Bruno\Documents\Codex\2026-09-29\consegue-conectar-o-x20\outputs\Fase2_Cerrado\Fase_2_Cerrado_LowPoly.blend`

O pacote e os registros da revisão atual ficam na subpasta `Retorno_Unreal_Revisao_Sem_Arvores` junto dessa fonte. `Retorno_Unreal` registra a instalação anterior. Para enviar a revisão pelo Git, inclua `Content/Cenas/Cena_Fase2.umap`, a pasta `Content/Fase2/Cerrado_Revisado` e este documento. Os assets anteriores não foram apagados.
