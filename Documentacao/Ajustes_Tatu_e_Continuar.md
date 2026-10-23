# Tatu e botão Continuar — 22/10/2026

## Onde ajustar nas Blueprints

- `BP_Tatu > RolamentoSuave`: direção e curva de velocidade da rolagem.
- `BP_Tatu > MovimentoRolamento` (Timeline): aceleração, deslocamento e desaceleração em 0,82 s.
- `BP_Tatu > RestaurarAposRolamento`: restaura atrito, freio, velocidade e entrada do jogador ao concluir ou interromper a animação.
- `BP_Tatu > RolamentoAoEncerrar`: libera o bloqueio ao encerrar a fase.
- Categoria `Rolamento > VelocidadeRolamento`: pico nominal de 950 cm/s. A direção usa a entrada de movimento e, parado, a frente do personagem.
- Montages usados: `Modelos/Personagem/TatuPersonagem/Montage/AM_Tatu_roll` (Rate Scale 1,35), `AM_Tatu_Attack_A` e `AM_Tatu_Attack_B` (1,25).

O Launch Character de 4500 foi substituído pela curva de velocidade. O Character Movement continua responsável pelas colisões e pela gravidade. O eixo vertical não é zerado pela curva. O dano dos golpes e os notifies existentes foram mantidos.

## Disponibilidade do Continuar

1. `BP_SalvamentoLuta > InicializarReferencias`: no menu, consulta se existe uma partida; nas fases, prepara as referências para salvar a luta.
2. `ConsultarPartidaNoMenu`: recebe a resposta e comunica ao menu.
3. `BP_MenuLaboratorio > DisponibilidadeDoContinuar`: habilita Continuar e escolhe o layout de quatro ou cinco opções.
4. `BP_ControleLaboratorio > ReposicionarLinhaDoMenu`: restaura a escala de repouso, reposiciona os elementos e reconstrói as posições usadas no hover.
5. Sem save, Continuar fica oculto e sem colisão; as outras opções ocupam o espaço e a numeração é atualizada.

A consulta HTTP e a leitura do cache local ficam em `CombateHttpActor`. A consulta de disponibilidade não abre uma fase. O clique em Continuar usa o fluxo de abertura já existente.

## Referência do chefe

Os chefes não herdam todos de BP_Boss. O serviço de save usa uma referência Actor e a variável editável `ClasseBossDaFase`, configurada em cada mapa: Ema, Onça, Jacaré e Cobra. A vida continua sendo lida do AC_HealthSystem.

## Comparação medida em Play no editor

Entrada real pelas Enhanced Input Actions, simulação com passo fixo de 1/60 s. Estes números medem tempo e deslocamento da lógica, não FPS da máquina.

| Medida | Antes | Depois |
|---|---:|---:|
| Alcance da rolagem no ensaio | 545,6 cm | 532,4 cm |
| Deslocamento nos primeiros 0,10 s | 445,2 cm | 13,7 cm |
| Primeiro acerto após início da animação A | cerca de 0,45 s | cerca de 0,36 s |

O movimento passou a se distribuir durante o giro. Os danos observados continuaram sendo 5 no golpe A e 10 no B.

As evidências detalhadas e os resultados dos cenários adicionais ficam no diretório de trabalho do Codex, `outputs/Tatu_Menu_20261022`. Os testes HTTP usam um servidor local separado para não alterar o banco de produção. A ferramenta temporária usada para editar os grafos não é uma dependência do jogo.

### Cenários concluídos

- Rolagem em movimento e parado: concluída, com entrada liberada e Max Walk Speed restaurado para 600 cm/s.
- Interrupção da montage: não deixa o jogador preso na rolagem.
- Parede de teste: o personagem parou antes da superfície; não atravessou o obstáculo.
- Menu sem save, com save e ao remover o save: visibilidade, colisão, posições e numeração verificadas.
- Abertura do menu com save existente: Continuar apareceu automaticamente.
- Dano aplicado pelo sistema da Unreal, gravação local e nova sessão de Play: tatu restaurado com 77/100 e Ema com 41/100.
- Referências e componente de vida dos quatro chefes conferidos nos respectivos mapas. O ensaio de fechar e reabrir foi executado na fase 1.

Os saves criados nos testes foram retirados de Saved/Combate ao terminar. O servidor local foi encerrado. As alterações de URL usadas nos ensaios não foram salvas nos mapas.

## Pendência do servidor publicado

Na verificação por GET em 22/10/2026, a API de produção respondeu HTTP 200 para `/api/progresso/Cena_Fase1/inventario`, mas HTTP 404 para `/api/progresso/Cena_Fase1/combate` e `/api/progresso/continuar-combate`. Os testes locais não comprovam o salvamento no Railway. O código das rotas já está no GitHub no commit `d7b48ee`; falta confirmar a publicação dessa versão no serviço. O painel do Railway pediu login, solicitado ao usuário.

A auditoria final carregou e compilou as quatro Blueprints envolvidas sem a ferramenta temporária de autoria. A URL de produção foi preservada nos assets.


## Correção da direção da rolagem — 22/10/2026

O pino `Direcao` da chamada `IniciarRolamentoSuave` ainda recebia `Get Actor Forward Vector`, o que fazia a rolagem acompanhar para onde o personagem olhava. Essa ligação foi substituída pela função pura `ObterDirecaoDeRolamento`, criada na própria `BP_Tatu`.

A função escolhe, nesta ordem:

1. `Get Pending Movement Input Vector`: entrada de movimento acumulada no quadro atual.
2. `Get Last Movement Input Vector`: última entrada consumida pelo movimento.
3. `Get Velocity`: velocidade atual, se não houver entrada.
4. `Get Actor Forward Vector`: alternativa quando o personagem está parado.

A saída passa por `Normal2D` e entra em `IniciarRolamentoSuave > Direcao`. Os vetores de movimento do Pawn já estão em coordenadas do mundo, acompanhando os controles existentes. A direção é guardada antes do bloqueio temporário da entrada durante a rolagem. A normalização horizontal evita aumentar a velocidade nas diagonais.

A curva da Timeline, a velocidade nominal, os montages e o dano dos ataques não foram alterados nesta correção.

Verificação em Play no editor: nove casos passaram (frente, trás, laterais, quatro diagonais e parado), com o personagem olhando para uma direção fixa. A direção e a velocidade durante a rolagem acompanharam a entrada esperada e os controles foram liberados ao terminar. Evidências: `outputs/DirecaoRolamento_20261022/direction_tests.json` no diretório de trabalho do Codex. Os testes usaram uma API local isolada e seus saves foram retirados do projeto ao concluir.


## Orientação da animação com câmera travada — 22/10/2026

A trava do alvo mantém o Controller e a cápsula voltados para o boss. O deslocamento já seguia a entrada de movimento, mas o modelo continuava com a orientação da cápsula e, por isso, a animação parecia sempre rolar para a frente.

A correção está na `BP_Tatu`, nos grafos `OrientacaoVisualDaRolagem` e `RestaurarOrientacaoVisual`:

1. Depois de guardar `DirecaoRolamento`, o evento `OrientarVisualParaRolamento` guarda a rotação relativa do Mesh e o modo anterior de herança da rotação.
2. Calcula a orientação visual correspondente à direção da esquiva, preservando o alinhamento original do modelo em relação ao personagem.
3. Usa `Set Absolute` apenas para a rotação do Mesh e aplica `Set World Rotation`. A localização e a escala continuam com os modos anteriores.
4. A câmera e a cápsula continuam com a trava existente. A câmera é filha de SpringArm/CollisionCylinder, e não do Mesh.
5. `FinalizarRolamentoSuave` chama `RestaurarVisualDepoisRolamento` para devolver a rotação relativa e o modo anteriores. O caminho é compartilhado pelo término, interrupção e encerramento da fase.

A animação original é reaproveitada. Nenhum arquivo de animação, montage, curva de velocidade ou código da trava do boss foi alterado nesta correção.

### Verificação

Foi acionada a própria `IA_LookIn` para selecionar a Ema. Antes da correção, o deslocamento lateral estava correto, mas o modelo permanecia olhando para o boss. Depois da correção, passaram onze cenários: quatro direções, quatro diagonais, parado, interrupção e câmera livre. Durante as esquivas, a orientação do modelo e a velocidade acompanharam a direção escolhida; após o término, a orientação original foi restaurada e o alvo continuou selecionado nos casos com trava. Os ensaios usaram uma API local isolada e um piso temporário, sem salvar esses objetos no mapa. Os saves de teste foram retirados ao terminar.

Evidências no diretório de trabalho do Codex: `outputs/RolamentoLock_20261022/lock_tests.json`.

## Consolidação da Blueprint e caminhada com alvo travado — 22/10/2026

Esta seção substitui a organização dos grafos descrita nas duas correções anteriores. A lógica continua na própria Blueprint.

### Onde editar

- `Content/Blueprints/Personagem/BP_Tatu > GerenciarRolamento`: uma única função com três blocos numerados e nós de execução alinhados da esquerda para a direita. Os getters simples ficam próximos de seus consumidores; os cálculos puros sem uso foram removidos.
- O parâmetro `Operacao` escolhe `0 = iniciar`, `1 = atualizar` ou `2 = finalizar`. `VelocidadeDaCurva` recebe o valor da Timeline na atualização.
- `BP_Tatu > Rolamento`: contém apenas a Timeline `MovimentoRolamento`, suas chamadas para atualizar/finalizar e a finalização no EndPlay. A Timeline precisa ficar no Event Graph da Unreal; a lógica executada por ela está na função.
- `Content/Modelos/Personagem/TatuPersonagem/ABP_TatuPersonagem > Event Graph`: cálculo da variável `Velocidade` usada pelo `Blend_tatu`.

Os seis grafos/funções auxiliares anteriores de direção, início, restauração e orientação foram incorporados em `GerenciarRolamento` e removidos da lista. As funções originais do personagem foram preservadas.

### Passo a passo do rolamento

1. O fluxo existente da entrada `IA_Dodge`, após suas condições de esquiva, chama a operação 0. Ela escolhe a direção pela entrada pendente, última entrada, velocidade ou frente do personagem, normalizando no plano horizontal.
2. Guarda a orientação do modelo e os ajustes de movimento; orienta apenas o Mesh na direção da esquiva. A cápsula e a câmera continuam com a trava do boss.
3. Prepara o movimento, bloqueia temporariamente a entrada e inicia a Timeline existente de 0,82 s.
4. Cada atualização da Timeline chama a operação 1, que aplica a curva à velocidade horizontal e preserva a componente vertical, a gravidade e as colisões.
5. O término, a interrupção da montage e o EndPlay chamam a operação 2. Uma guarda evita finalizar duas vezes. A função restaura o Mesh, atrito, frenagem, velocidade máxima e entrada.

### Causa e correção da caminhada lateral

Antes, `Velocidade` recebia somente o produto escalar entre a velocidade do personagem e sua direção frontal. Com a câmera travada e movimento lateral, esse valor ficava perto de zero, levando o Blend Space para a pose parada enquanto a cápsula se deslocava.

Agora, o módulo da velocidade horizontal (`VSizeXY`) determina a intensidade da caminhada. O produto escalar escolhe somente o sinal: a caminhada para trás é usada quando a projeção frontal é menor que -25% da velocidade horizontal. Essa margem evita alternar para a animação de trás por uma pequena projeção negativa durante o movimento lateral.

São reaproveitados `Tatu_walk`, `Tatu_back` e `Tatu_idle`. Não foram criados clipes específicos de strafe nem alterados os arquivos das animações ou do Blend Space nesta etapa. A correção elimina o uso indevido da pose parada durante o deslocamento lateral; não implementa um sistema de apoio dos pés no chão.

### Verificação desta etapa

- Onze casos de rolagem aprovados: frente, trás, duas laterais, quatro diagonais, parado, interrupção e câmera livre. Direção, velocidade e orientação visual acompanharam a entrada; o estado original foi restaurado ao terminar.
- Cinco casos de caminhada aprovados com a Ema selecionada pela própria `IA_LookIn`: frente, trás, direita, esquerda e parado.
- Nas duas laterais, a velocidade real e a variável da animação ficaram em aproximadamente 600 cm/s. O cálculo anterior teria produzido aproximadamente -16 cm/s no mesmo enquadramento. O movimento do pé entre amostras confirmou que o ciclo estava ativo; parado, a velocidade da animação retornou a zero.
- Os testes usaram entrada Enhanced Input, passo de simulação de 1/60 s, API local isolada e piso temporário. Não medem FPS nem alteram o banco publicado. Os mapas não foram salvos durante os ensaios.

Evidências no diretório de trabalho do Codex: `outputs/LocomocaoTatu_20261022/roll_regression.json`, `walk_tests.json`, `installation.json` e `final_audit.json`.

A auditoria final reabriu o projeto e compilou as cinco Blueprints envolvidas sem a ferramenta temporária de autoria. Os assets não dependem dela. A compilação do alvo Editor passou, a URL de produção foi preservada e os saves de teste foram retirados.

## Revisão visual de GerenciarRolamento — 22/10/2026

A função foi novamente organizada para leitura em blocos menores, substituindo as três faixas muito largas da versão anterior. Continua sendo uma única função, com oito comentários numerados em duas colunas:

1. Escolher a direção.
2. Guardar e orientar o modelo.
3. Guardar os ajustes de movimento.
4. Preparar o movimento e iniciar a Timeline.
5. Aplicar a velocidade da curva.
6. Restaurar o modelo e parar a Timeline.
7. Restaurar os ajustes de movimento.
8. Zerar o movimento horizontal e liberar a entrada.

Os nós existentes foram aproximados e alinhados. Reroutes encaminham as ligações longas de execução pelas margens dos blocos; o cálculo da direção segue da esquerda para a direita. Não foram adicionadas funções, variáveis de gameplay ou novas ações à execução.

A ferramenta de autoria comparou as conexões lógicas e os valores dos pinos antes e depois, ignorando somente posições, comentários e reroutes. As assinaturas permaneceram iguais. As dimensões dos widgets reais do editor foram usadas para conferir sobreposição entre nós. As imagens de inspeção e o relatório ficam no diretório de trabalho do Codex em `outputs/OrganizacaoRolamento_20261022`.

Ao concluir esta revisão, o projeto foi recompilado e os assets foram reabertos sem a ferramenta temporária. A auditoria de carregamento e compilação passou. Nenhum ensaio de gameplay adicional foi executado nesta revisão exclusivamente visual.
