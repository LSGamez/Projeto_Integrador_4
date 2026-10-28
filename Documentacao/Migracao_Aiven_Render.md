# Hospedagem da API e banco — 27/10/2026

A API foi publicada no Render e usa o MySQL da Aiven. O jogo acessa a API por HTTPS; a senha do banco fica somente na variável `ConnectionStrings__BancoJogo` da hospedagem da API.

## Endereço no jogo

`https://unreal-api-x1kl.onrender.com/api/progresso`

Os valores padrão de `UrlApi` em `ProgressHttpActor.h` e `CombateHttpActor.h` foram atualizados. As consultas iniciais e o início de novo jogo aguardam até 90 segundos para permitir o despertar da instância gratuita. O transporte de combate mantém a cópia local já existente.

## Conferência da API

Consultas realizadas em 27/10/2026, sem alterar registros:

| Rota após `/api/progresso` | Resultado |
| --- | --- |
| `/Cena_Fase1/inventario` | HTTP 200; itemA=0, itemB=0, bugigangas=1 |
| `/Cena_Fase1/combate` | HTTP 200; estado=null |
| `/continuar-combate` | HTTP 200; fase vazia e revisão 0 |

O estado de combate vazio é esperado após a importação parcial: o arquivo `Polimerose_Aiven_Import.sql` contém a estrutura base, as migrações 002–004 e somente o inventário/itens coletados da Fase 1 encontrados no backup local de 28/09/2026. Ele não é um backup integral do antigo Railway. O antigo estado de luta não foi recuperado.

O endereço `/` retornar 404 não indica falha da API, pois não há uma página inicial definida.

## Uso e distribuição

O plano gratuito do Render suspende a API após inatividade; a primeira consulta pode levar cerca de um minuto. Após mudar os fontes, é necessário recompilar o módulo do projeto. Para distribuir o jogo já empacotado, gere outro pacote; o executável antigo conserva o endereço anterior.

O alvo `Projeto_Integrador_4Editor Win64 Development` foi recompilado com sucesso após as alterações. A conferência HTTP acima não substitui um teste completo de Novo Jogo, coleta e retomada da luta dentro do jogo; esse teste em Play ainda não foi executado.
