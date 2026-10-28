# Menu de pausa

Adicionado em 27/10/2026. Interface e lógica em Blueprints, com classes base da própria Unreal. Não acrescenta código C++ de runtime ao projeto. Preferências são locais; ao sair da fase, o botão solicita a captura pelo sistema de combate já existente.

## Uso

- Esc durante a fase: abrir e pausar.
- Voltar ao jogo: fechar e retomar.
- Som: ajustar o volume geral de 0 a 100%.
- Gráficos: escolher Baixa, Média ou Alta, correspondendo a 0/1/2 no Game User Settings, como no menu existente.
- Voltar ao menu inicial: sair da fase e abrir `/Game/Cenas/Cena_Menu`.
- Esc em Som/Gráficos: voltar à tela principal da pausa. Esc novamente: retomar.

No Play dentro do editor, Esc também é o atalho padrão de Stop da Unreal. Para usar Esc sem esse conflito, execute em Standalone Game ou altere o atalho Stop nas preferências do editor. O atalho do editor não foi modificado por esta alteração.

## Assets e lógica

Pasta `/Game/Interface/Pausa`:

- `WBP_Pausa`: aparência, botões, foco, estado de pausa e preferências. As funções estão separadas por responsabilidade e com comentários.
- `BP_MenuPausa`: cria uma única janela, habilita a entrada de Esc mesmo durante a pausa e limpa o estado ao remover o personagem. No Cena_Menu, a entrada de pausa fica desabilitada para preservar a navegação do terminal.
- `SG_Preferencias`: volume local. Slot `PreferenciasPolimerose`, na pasta padrão Saved/SaveGames. Grava ao voltar/fechar, sem escrita contínua pelo slider.
- `SM_Preferencias`: Sound Mix do volume geral sobre `/Engine/EngineSounds/Master`, incluindo classes filhas. O mix permanece como base para preservar o volume durante transições.

`BP_Tatu` recebeu um Child Actor Component chamado `MenuPausa`, apontando para `BP_MenuPausa`. As fases que usam o tatu recebem o menu automaticamente. As malhas, dano, HUD e lógica de rolagem não foram editados. Os mapas não foram modificados.

A abertura guarda o estado anterior do cursor, sincroniza os controles, adiciona a janela e pausa. Se outro sistema já pausou o jogo, o menu não toma posse dessa pausa. O fechamento restaura o cursor e o foco do jogo. Nenhum Event Tick, captura 3D ou desfoque foi acrescentado.

O botão `BotaoMenuInicial` chama a função Blueprint `VoltarMenuInicial`, organizada da esquerda para a direita: solicitar a captura do estado da luta pelo sistema existente, chamar `DefinirPausa(false)` para salvar preferências e liberar a pausa/entrada, depois `OpenLevel` com `/Game/Cenas/Cena_Menu`. Não chama Novo Jogo nem apaga o save. A captura registra a cópia local de acordo com as regras já existentes do sistema de combate; a transição não aguarda uma confirmação de sincronização remota. O painel recebeu espaço para quatro botões.

## Conferência e entrega

A compilação das Blueprints, o vínculo com o tatu e as dependências dos assets foram conferidos. A prévia visual foi renderizada pelo editor. Não foi executado teste de Play nesta alteração.

A ferramenta temporária usada para escrever os grafos é removida do projeto ao concluir. Para compartilhar, enviar também os novos assets de Content/Interface/Pausa e o BP_Tatu atualizado. Não é necessário enviar a ferramenta de autoria.

Evidências no diretório de trabalho do Codex: `outputs/MenuPausa_20261027`.

A última reabertura, após corrigir a execução da leitura do mapa, compilou os assets sem avisos de nós podados e sem erros de Blueprint. A ferramenta temporária foi retirada e a compilação do alvo Editor passou.

Na atualização de 27/10/2026 com `VoltarMenuInicial`, a prévia visual foi conferida com os quatro botões. A reabertura sem a ferramenta temporária compilou `WBP_Pausa`, `BP_MenuPausa` e `BP_Tatu`, confirmou a função, o caminho do mapa e o foco do widget, sem erros/avisos de Blueprint. O alvo Editor também recompilou com sucesso. A transição ainda não foi executada em Play. Evidências: `outputs/Pausa_VoltarMenu_20261027` no diretório de trabalho do Codex.
