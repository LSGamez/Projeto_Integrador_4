# Menu do laboratório em Blueprints

Projeto: `C:/Users/Bruno/Projeto_Integrador_4`

Mapa: `/Game/Cenas/Cena_Menu`

## Implementação atual

O menu usa Blueprints reais em `/Game/Laboratorio/Menu/Blueprints`: `BP_MenuLaboratorio`, `BP_ControleLaboratorio`, `BP_TextoLaboratorio` e `WBP_TextoLaboratorio`.

Os Event Graphs implementam cliques, câmeras, qualidade gráfica, sons, seleção dos frascos, reação do líquido e movimento da poeira/vapor. Os textos usam UMG em espaço de tela, com resolução independente da redução do cenário no Low. Os gráficos têm comentários em português.

As antigas classes de menu em C++ foram arquivadas. A API de progresso original continua usando C++, chamada pelo Blueprint original de Novo Jogo.

Veja `Apresentacao_Blueprints.md` para a ordem de apresentação e o escopo da conversão.

## Edição

O ator `Menu_Laboratorio` configura as câmeras, os sons e o tempo de transição. Os atores `Botao_XX` configuram ações, volumes clicáveis e referências de frascos e indicadores. Os atores de texto têm as variáveis `Texto`, `Fonte` e `Cor`.

As câmeras são `Camera_Menu`, `Camera_Opcoes` e `Camera_Creditos`. As transições usam 1,25 s com Ease In Out. A luz da lousa mantém o ciclo de quatro segundos.

Os líquidos continuam usando Geometry Cache; o sinal do monitor usa barras animadas em um plano de dois triângulos. `Kit_Menu_Laboratorio.blend` mantém as novas malhas editáveis.

## Sinal do monitor

O sinal ativo usa `M_Sinal_Barras`, em `Laboratorio/Menu/Materiais/TV`: 22 barras desenhadas em um único plano por nós Time, Sine, Floor, Frac e máscaras. São dois triângulos, sem código Custom/HLSL. O atlas anterior continua disponível no projeto.

Fonte: [Lissajous animation.gif](https://commons.wikimedia.org/wiki/File:Lissajous_animation.gif), de Krishnavedala, sob [CC BY-SA 3.0](https://creativecommons.org/licenses/by-sa/3.0/). A atribuição está em `Content/ThirdPartyNotices/Sinal_Fonte_Licenca.txt`, incluído no empacotamento.

## Créditos

Alice Araújo, Bruno Figueiredo, Luan Santana, Lucas Oliveira, Rodrigo Mota e Yuri Rodrigues. Orientador: Eduardo Souto.

## Conferência

Os Blueprints foram compilados e o menu foi exercitado em Play: navegação, hover, seleção de qualidade Low, câmeras e confirmação/cancelamento de novo jogo. O teste não confirmou Novo Jogo nem alterou o progresso da API. A qualidade anterior foi restaurada.

`Teste_Blueprints.json` registra as verificações. `BP_Menu_Principal.png`, `BP_Opcoes_Low.png` e `BP_Creditos_Low.png` foram capturadas durante o Play.

## Backup

`Saved/Backup_Conversao_Blueprints` preserva o mapa anterior, o código de menu substituído e os assets anteriores. A migração para outro projeto deve incluir o mapa e suas dependências, além da integração de progresso existente.



## Organização da TV

Peças no Outliner: Laboratorio/Menu/TV. Materiais no Content Browser: Laboratorio/Menu/Materiais/TV. Fundo azul-petróleo, quatro painéis visíveis e barras animadas, seguindo a segunda referência. A lógica e a organização das quatro Blueprints foram mantidas.
