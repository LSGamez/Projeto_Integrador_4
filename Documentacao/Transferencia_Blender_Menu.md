# Atualização do laboratório pelo Blender

Mapa: `/Game/Cenas/Cena_Menu`. Assets importados: `/Game/Laboratorio/ArteBlender`.

## Arte instalada

- Trepadeiras, raízes, samambaias, capim, musgo, folhas, entulho, rachaduras, pisos soltos, reboco e tijolos.
- Doze atores com componentes nativos de instâncias reutilizam as malhas decorativas. As instâncias podem ser editadas no componente `Particulas`, herdado do modelo de instâncias existente.
- Teto com abertura na entrada das raízes, conservando o ator `Teto_01`.
- Oito objetos recebem as cores e manchas gravadas a partir dos materiais do Blender. Suas malhas e origens foram conservadas; a escala da geometria importada foi aplicada na cópia de exportação.
- Os oito objetos compartilham `M_Laboratorio_Gravado`: cor de 2048 × 2048, normal de 1024 × 1024 e propriedades de superfície de 1024 × 1024. A decoração usa o material opaco `M_Natureza`, com cores de vértice.
- A decoração não tem colisão nem Tick. As raízes conservam sombras; as demais peças decorativas não projetam sombras. Nanite, cópia para CPU e geração de campos de distância estão desativados nas novas malhas.
- A luz na entrada do teto não projeta sombras. Três luminárias existentes foram atenuadas para acompanhar a cópia artística.

## Menu e animações

Os botões, câmeras, frascos, Blueprints de navegação e lógica de progresso permanecem os do projeto. Os textos continuam nos widgets da Unreal, incluindo os créditos e a correção de legibilidade em qualidade baixa. Os planos de texto do Blender são uma representação artística; eles não foram importados sobre os widgets.

Os frascos conservam seus Geometry Caches e as animações existentes. A sequência da luz piscando permanece referenciada. Os arquivos das Blueprints do menu e das animações foram comparados antes e depois da importação e não mudaram.

## Organização e retorno

Outliner: `Laboratorio/Natureza` e `Laboratorio/Danos`. Content Browser: `Laboratorio/ArteBlender/Malhas`, `Materiais` e `Texturas`.

O mapa anterior está em `Saved/Backup_Blender_Menu_20261010_092920/Cena_Menu.umap`. Para retornar ao anterior, feche o editor e restaure essa cópia sobre `Content/Cenas/Cena_Menu.umap`. Os assets originais do laboratório permanecem no projeto.

Fonte Blender: `C:/Users/Bruno/Documents/Codex/2026-09-29/consegue-conectar-o-x20/outputs/Menu_Blender_Natureza/Cena_Menu_Natureza.blend`.

Pacote de exportação e relatórios: `outputs/Menu_Blender_Natureza/Retorno_Unreal` na mesma pasta de trabalho. `Instalacao_Unreal.json` registra a importação e `Cena_Salva.json` registra o inventário do mapa reaberto.

As plantas e os danos acrescentam aproximadamente 3.900 triângulos distribuídos por instâncias. Isso não é uma medição de FPS; o custo de iluminação, transparência e resolução continua dependendo das configurações e da máquina alvo.

## Ajuste das manchas — 10/10/2026

As manchas procedurais extras nas quatro paredes e no piso foram retiradas a pedido do usuário. `Parede_01`, `Parede_02`, `Parede_07`, `Parede_09` e `Piso_01` voltaram a usar as malhas e os materiais originais, conservando a sujeira anterior. A cópia de autoria no Blender recebeu a mesma remoção. Os materiais gravados da mobília e o kit de natureza continuam instalados.

O mapa anterior a essa remoção está em `Saved/Backup_Remover_Manchas_Extras_20261010_100422/Cena_Menu.umap`. O relatório desta revisão é `Retorno_Unreal/Remocao_Manchas_Extras.json`. As imagens da transferência inicial foram capturadas antes desse ajuste.
