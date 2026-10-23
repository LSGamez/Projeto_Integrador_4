# Retomar uma luta

Implementacao de 22/10/2026.

## Como usar

O menu agora tem **Continuar**. Ele consulta a ultima fase salva e a reabre. A vida atual e maxima do jogador e do chefe, suas posicoes e rotacoes sao restauradas. **Novo Experimento** continua usando a confirmacao de Novo Jogo e limpa o estado da luta.

O ator `Salvamento_Luta` foi colocado em Cena_Fase1, Cena_Fase2, Cena_Fase3_Nova, Cena_Fase4_Nova e Cena_Menu, na pasta Sistema/Salvamento do Outliner. No menu ele atende apenas ao botao Continuar; nao inicia o timer de captura.

## Blueprints

`Content/Blueprints/Save/BP_SalvamentoLuta` possui quatro grupos:

1. **InicializarReferencias**: espera as referencias do personagem e do boss, guarda os componentes AC_HealthSystem e pausa a fase enquanto carrega o save. O ator nao pode salvar antes de concluir esta etapa.
2. **CapturarVidaEPosicoes**: le Current Health e Max Health dos componentes reais, monta EstadoLuta e chama Gravar Luta. Acionado por alteracao de vida, timer de 0,25 segundo e EndPlay. O arquivo so e regravado quando os dados mudam.
3. **RestaurarVidaEPosicoes**: aplica os maximos antes das vidas atuais, restaura as posicoes sem alterar a escala dos modelos e libera o timer apenas apos confirmar a restauracao. Um registro com jogador morto inicia nova tentativa com valores iniciais, em vez de restaurar a morte indefinidamente.
4. **ContinuarEInformarFalhas**: abre o mapa confirmado, restaura entrada de gameplay e encaminha falhas ao terminal do menu. Uma falha de carregamento preserva o save, sem autorizar sua substituicao pelos valores iniciais.

Em **AC_HealthSystem**, os Set de Current Health notificam Capturar Estado imediatamente. Em **BP_MenuLaboratorio**, AcoesDoTerminal recebeu o caso `continue`; as demais acoes foram preservadas. O botao fica bloqueado enquanto aguarda a consulta e e liberado em caso de erro.

## Transporte e copia local

`Source/Projeto_Integrador_4/CombateHttpActor` oferece serializacao, HTTP e gravacao local para esses nos. Usa uma requisicao por vez, no maximo um novo envio por segundo, e conserva o ultimo estado pendente para repetir apos falha. Sem Event Tick.

Os arquivos ficam em Saved/Combate. A substituicao por arquivo temporario preserva o ultimo arquivo completo. O fechamento normal tambem solicita uma captura; o HTTP nao precisa terminar antes da saida, porque Continuar pode usar a copia local mais recente da mesma partida. Uma interrupcao abrupta usa o ultimo arquivo cuja escrita terminou.

Novo Jogo troca a identificacao da partida no servidor. Saves atrasados da partida anterior recebem HTTP 409. A copia local e limpa apos a confirmacao de Novo Jogo.

## Banco e publicacao

API no repositorio Unreal-API, commit d7b48ee, com migration automatica 004. Tabelas sessao_save e combate_fase; rotas GET/PUT /api/progresso/{fase}/combate e GET /api/progresso/continuar-combate. Continua sendo um save compartilhado do jogador 1.

A API compilou sem erros e foi enviada ao GitHub. O envio nao comprova a conclusao do deploy no Railway. O modulo Unreal e os Blueprints compilaram. A ferramenta de autoria foi retirada do projeto e nao e dependencia do jogo. Nao foram executados testes de partida, fechamento ou retomada nesta alteracao.

Uma build .exe antiga nao recebe estes arquivos automaticamente: e necessario empacotar uma nova versao do jogo. Este salvamento nao e um replay do frame: ataques, projeteis e montagens em andamento nao sao serializados. O inventario e as receitas continuam com a integracao anterior; esta entrega trata do estado da luta.


## Verificação posterior — 22/10/2026

A referência do chefe foi corrigida para usar a classe de cada fase. Foram testadas gravação e restauração da vida em novas sessões de Play na fase 1, usando API local isolada (tatu 77/100 e Ema 41/100). O menu também foi testado com e sem save. Detalhes em `Ajustes_Tatu_e_Continuar.md`.

A verificação da API publicada encontrou HTTP 404 nas rotas novas de combate, enquanto inventário responde 200. A publicação no Railway ainda precisa ser concluída; o painel solicitou login. O GitHub já contém o código das rotas.
