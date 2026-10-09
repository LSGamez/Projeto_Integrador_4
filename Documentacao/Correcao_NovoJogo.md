# Novo jogo e banco — verificação e correção

Projeto `Projeto_Integrador_4`, mapa `Cena_Menu`, 08/10/2026.

## O que foi observado

A falha original não se repetiu durante a verificação. A API real respondeu HTTP 200 às consultas de progresso, inventário e itens coletados. Um início de novo jogo real recebeu HTTP 200 e abriu `/Game/Cenas/Cena_Fase1`; o personagem `BP_Tatu` foi criado e as consultas da fase terminaram.

O fluxo anterior dependia de um botão antigo oculto para chamar o serviço. Se a API falhasse, `BP_MenuHttpActor` não encaminhava o erro ao terminal; o menu permanecia aguardando até uma mensagem genérica após 20 segundos. A requisição HTTP tinha limite de 15 segundos. Havia também registros de acesso a referências vazias nas listas de textos do menu.

Não há evidência suficiente para atribuir a falha relatada a uma interrupção específica do banco. Uma resposta mais lenta que o limite anterior podia bloquear o início; o limite foi ampliado e o tratamento de falha foi corrigido.

## Alterações instaladas

- `BP_MenuLaboratorio → AcoesDoTerminal → IniciarExperimento`: Get Actor Of Class → Cast ProgressHttpActor → Is Valid → estado de espera → NovoJogo (`Cena_Fase1`, vida 100). A operação não depende mais do clique no botão antigo oculto.
- `ExperimentoFalhou`: libera a espera e mostra a mensagem recebida. Uma nova tentativa é permitida imediatamente após a falha.
- `BP_MenuHttpActor → InicioEFalhaDoExperimento`: `AoFalharNovoJogo` encaminha a mensagem ao gestor. `AoIniciarNovoJogo` restaura Game Only, oculta o cursor, configura captura de gameplay e chama Open Level após a confirmação da API.
- O serviço existente `ProgressHttpActor` continua responsável pelo transporte HTTP em C++. Acrescentei `UrlApi` editável e `TempoLimiteNovoJogo`, além de mensagens que distinguem HTTP de interrupção de conexão. Navegação, confirmações e tratamento visual continuam em nós de Blueprint.
- `TempoLimiteNovoJogo = 45` segundos. A guarda do menu usa esse limite mais cinco segundos, em vez de uma espera fixa de 20 segundos.
- Cinco laços do menu verificam Is Valid antes de acessar os atores; as listas salvas foram conferidas.
- Os treze controles, as 26 instâncias de poeira e as otimizações de atualização permanecem presentes. Nenhuma nova classe de Blueprint de runtime foi criada.

## Testes após a correção

Os testes enviam ações ao Blueprint em PIE com NullRHI; não são cliques físicos pela janela do Windows e não medem desempenho gráfico.

1. Resposta HTTP 500 de servidor local: o erro foi exibido e o menu liberado em cerca de 0,05 segundo.
2. Tempo limite de um segundo configurado somente na instância de teste: o menu foi liberado após cerca de um segundo.
3. Nova tentativa com confirmação local: `Cena_Fase1` abriu, `BP_Tatu` foi criado e as consultas reais de inventário/coletados terminaram.

Nos testes posteriores, `UrlApi` foi alterada para `127.0.0.1` antes de criar a requisição, somente em PIE. Três POSTs foram recebidos pelo servidor local. O mapa salvo conserva a URL de produção e limite de 45 segundos. O mapa foi reaberto sem a ferramenta temporária de autoria e suas referências foram verificadas.

## Ocorrência no teste inicial

O redirecionamento inicial tentou alterar uma requisição que já estava em processamento. A engine rejeitou a alteração; o POST foi enviado à API real e executou o reinício de novo jogo, que limpa inventários e coletados de todas as fases.

Restaurei pelo endpoint de progresso o registro principal consultado antes do teste: `Cena_Lobby`, vida 100, checkpoint nulo e itens vazios. Uma consulta posterior confirmou esse estado. O inventário e os coletados de `Cena_Fase1` estavam vazios antes do teste. Não havia uma cópia anterior dos inventários/coletados das demais fases; não é possível afirmar que esses dados foram recuperados.

Os scripts de teste anteriores foram ajustados para impedir POSTs de produção quando exercitam a confirmação. O teste atual usa `UrlApi` local antes do envio.

## Onde ajustar

- URL e espera: selecione `BP_MenuHttpActor` no Outliner e procure a categoria **Progresso**, ou abra os Class Defaults desse Blueprint. A URL salva é `https://unreal-api-production.up.railway.app/api/progresso`.
- Início e mensagens: `BP_MenuLaboratorio → AcoesDoTerminal`.
- Confirmação da API e abertura da fase: `BP_MenuHttpActor → InicioEFalhaDoExperimento`.
- Transporte: `Source/Projeto_Integrador_4/ProgressHttpActor.h` e `.cpp`.

Os relatórios JSON estão em `Documentacao/Verificacao_NovoJogo`. A ferramenta temporária de autoria está arquivada; o jogo não depende dela.

Backup de mapa, Blueprints e fontes anteriores: `C:\Users\Bruno\Projeto_Integrador_4\Saved\Backup_NovoJogo_20261008_201925`. Restaurar as fontes do módulo requer recompilação.
