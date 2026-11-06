#pragma once

#define LOCTEXT_NAMESPACE "RegistrosLaboratorio"

namespace RegistrosLaboratorio
{
    struct FRegistro
    {
        int32 Area;
        FText Titulo;
        FText Texto;
    };

    const FRegistro Pesquisa[] = {
        {1, LOCTEXT("Pesquisa1Titulo", "O que sobrou dos antigos"), LOCTEXT("Pesquisa1",
            "Para entender o que está acontecendo com meu corpo, preciso entender como tudo começou. A floresta de pedra é o que restou daqueles que vieram antes, mas foi o plástico que mostrou o que eles realmente queriam. Garrafas de água, pneus, vasilhas e tantas outras coisas perdidas por aí revelam o deus que eles veneravam.\n\n"
            "Mas esse deus se voltou contra eles. Pequenos pedaços dele, chamados de microplásticos, começaram a se espalhar pela água, pelo ar e pela comida, até dominar seus corpos. Ainda não entendo como isso funciona, mas conheço os resultados: infecção, violência e morte.\n\n"
            "Ainda não sei como parar isso. Mas tenho muito a perder se não fizer nada.")},
        {1, LOCTEXT("Pesquisa2Titulo", "O plástico no sangue"), LOCTEXT("Pesquisa2",
            "Acho que estou entendendo melhor como isso funciona. Os microplásticos estão em tudo: rios, flores e lares. Chegam à nossa mesa pelos alimentos. Dali, passam para o estômago e depois para o sangue. Mas o corpo não sabe com o que está lutando e não consegue destruir aquilo.\n\n"
            "Eu e minha filha estamos doentes, e tudo indica que a causa é a mesma. Minhas pesquisas indicam que o plástico carrega venenos aderidos a ele. Ao tentar resistir, as células se desgastam mais depressa do que conseguem se reparar. Nos livros antigos, encontrei algo parecido nos relatos sobre radiação. Ainda não sei se é coincidência.\n\n"
            "Preciso continuar estudando. O tempo não está a meu favor. Ontem minhas mãos tremeram enquanto eu desenhava o esboço de uma lança. Deve ser o cansaço. Também tenho comido pouco, mas isso passa. Refiz o desenho três vezes e, na última, esqueci o que ia anotar na margem. Preciso alcançar meus alvos antes que seja tarde demais.")},
        {2, LOCTEXT("Pesquisa3Titulo", "Amostras da ema"), LOCTEXT("Pesquisa3",
            "Venci a ema. Quando ela caiu, colhi sangue, penas e material do chão ao redor. Levei as três amostras para o laboratório e encontrei plástico em todas. O material dentro das células do sangue estava partido em pedaços, como nos relatos antigos sobre os efeitos da radiação. Agora sei que a semelhança não é coincidência: nos dois casos, a célula perde a capacidade de se reparar. Cada erro pode passar para a célula seguinte.\n\n"
            "As partículas menores são as piores. Passam por onde as maiores não passam e chegam a lugares que eu achava protegidos. O plástico não precisa ser radioativo para ferir assim. Basta permanecer ali, e o corpo gasta o que tem tentando se defender.\n\n"
            "Preciso encontrar algo que ajude o corpo a eliminar o plástico ou a reparar o estrago que ele deixou. Anotei tudo duas vezes porque não lembrava de já ter escrito. Meus dedos mal se fecham em torno da caneta. Preciso ir mais rápido.")},
        {3, LOCTEXT("Pesquisa4Titulo", "A descoberta na saliva"), LOCTEXT("Pesquisa4",
            "Venci a onça. Quando ela caiu, colhi uma amostra de saliva. No laboratório encontrei algo que não esperava. Junto ao plástico havia pequenos seres vivos, e alguns deles se alimentavam dele. Não comem tudo e trabalham devagar, mas conseguem quebrar certos plásticos até que não restem pedaços.\n\n"
            "Isso muda o que eu pensava. Eu achava que o corpo não podia fazer nada contra o plástico, mas a natureza está tentando aprender sozinha. Esses seres produzem a substância que usam para quebrá-lo. É ela que eu preciso entender. Se eu conseguir que o corpo a produza, talvez ele consiga se limpar sozinho.\n\n"
            "Ainda há um problema: a substância só age sobre alguns tipos de plástico, e eu não sei se é segura dentro de um corpo vivo. Escrevi o nome dela errado duas vezes antes de acertar. As letras estão embaçando no fim da página. Preciso testar antes que a visão falhe de vez.")},
        {4, LOCTEXT("Pesquisa5Titulo", "Uma instrução para o corpo"), LOCTEXT("Pesquisa5",
            "Venci o jacaré. Quando ele caiu, colhi sangue. No laboratório, encontrei a mesma substância que vi na boca da onça, agora circulando dentro dele. O corpo do jacaré aprendeu a produzi-la sozinho. Algo nele mudou, e essa mudança o ajuda a eliminar o plástico. Isso me diz que um corpo pode receber uma instrução e passar a fabricar a própria defesa.\n\n"
            "Se isso for possível, não preciso esperar uma mudança natural que talvez nunca chegue. Posso entregar a instrução pronta, escrita numa mensagem e envolta em uma bolha de gordura que leva o recado até o fígado. As células de lá leem a mensagem, produzem a substância e a liberam no sangue, onde ela encontra o plástico. A mensagem desaparece depressa. A substância dura alguns dias, e depois o corpo precisa de outra dose.\n\n"
            "Isso me preocupa, porque preparar doses exige tempo, e o tempo é o que mais me falta. Hoje as palavras escaparam de mim no meio de uma frase, e precisei olhar para a página para lembrar o que queria escrever. Preciso fazer a primeira dose funcionar antes de pensar nas outras.")},
        {5, LOCTEXT("Pesquisa6Titulo", "A última dose"), LOCTEXT("Pesquisa6",
            "Venci a cobra. Agora tenho tudo de que precisava, mas só o bastante para uma dose. Não há material para fazer duas, e o tempo para buscar mais já acabou. Se a dose funcionar, o corpo recebe a instrução e passa a produzir a própria defesa por alguns dias, como o jacaré aprendeu a fazer.\n\n"
            "Escolhi dar a dose à Sofia. Sei que também estou doente e sei o que isso quer dizer. Se eu a usar em mim, vou ganhar alguns dias, e ela não ganha nada. Se eu a usar nela, meu trabalho não termina comigo. Ela seguirá meu legado e vai terminar o que eu não consegui.\n\n"
            "Minhas mãos mal seguram a seringa, e preciso olhar para a página para saber onde estou. Não vou escrever mais. O que resta fazer é o que eu vim fazer.")}
    };

    const FRegistro Sofia[] = {
        {1, LOCTEXT("Sofia1Titulo", "Os primeiros sinais"), LOCTEXT("Sofia1",
            "Registro da Sofia. Quero deixar escrito como isso começa, porque, na primeira vez, ninguém percebe. Quem adoece de polimerose não sente nada nos primeiros dias. O plástico entra pela água e pela comida, passa para o sangue e fica. O corpo segue como se nada tivesse acontecido.\n\n"
            "Depois vêm os sinais pequenos: um cansaço que o sono não cura, uma tosse seca que vai e volta e um apetite que diminui. Sofia tem os três e diz que é só uma fase. Quero acreditar nela.\n\n"
            "Vou anotar tudo o que ela sentir, dia após dia. Se eu entender como a doença avança, talvez consiga ficar um passo à frente dela.")},
        {2, LOCTEXT("Sofia2Titulo", "As noites de Sofia"), LOCTEXT("Sofia2",
            "A doença mudou de fase. O corpo percebeu o plástico e começou a lutar, mas não consegue eliminá-lo. A luta nunca termina. É isso que cansa: o corpo inteiro trabalhando sem parar, como uma casa com todas as luzes acesas.\n\n"
            "A tosse de Sofia agora vem à noite. Ela sente frio quando a casa está quente, dorme mais do que de costume e o estômago recusa comida. Os venenos presos ao plástico também interferem no que regula o corpo, e por isso tudo parece acontecer fora de hora.\n\n"
            "Ainda consigo fazê-la acreditar que é só um resfriado. Sei que não é, mas não tenho coragem de dizer.")},
        {3, LOCTEXT("Sofia3Titulo", "O que já dá para ver"), LOCTEXT("Sofia3",
            "A doença passou do que se sente para o que se vê. As células de Sofia começaram a errar ao se reparar, e cada erro passa para a célula seguinte. O corpo continua trabalhando, mas passa a trabalhar torto.\n\n"
            "Os machucados dela demoram a fechar. Há manchas novas na pele, e o cabelo fica nas minhas mãos quando a penteio. As roupas que antes ficavam curtas agora ficam largas.\n\n"
            "Ela me perguntou se vai ficar diferente. Eu disse que não. Dessa vez, dizer isso doeu.")},
        {4, LOCTEXT("Sofia4Titulo", "Memória e comportamento"), LOCTEXT("Sofia4",
            "A doença chegou à cabeça. O que o plástico e os venenos fazem ao corpo também alcança aquilo que pensa e sente. É por isso que gente calma pode se tornar violenta sem entender o motivo. Sofia ainda não chegou a esse ponto, mas está perto.\n\n"
            "Ontem ela atirou um copo na parede por uma coisa pequena. Depois chorou por muito tempo sem saber dizer por quê. As mãos dela tremem como as minhas e, às vezes, ela esquece o meu nome por alguns segundos.\n\n"
            "Eu não a culpo. É a doença que fala quando ela perde o controle. Preciso me lembrar disso nos dias em que ela me ferir.")},
        {5, LOCTEXT("Sofia5Titulo", "Antes que o tempo acabe"), LOCTEXT("Sofia5",
            "Sofia quase não acorda mais. A doença chegou à fase em que já não ataca um lugar de cada vez. O fígado, os pulmões, o intestino e a cabeça falham juntos, e cada falha piora as outras. O corpo deixa de se defender, como se tivesse desistido.\n\n"
            "Ela respira com esforço, e a pele está fria e pálida. Às vezes abre os olhos e me olha, e eu não sei se me vê. Quem chega aqui, pelo que li, não volta sozinho.\n\n"
            "Mas eu tenho a dose. Vou fazer isso agora, antes que o tempo dela também acabe.")}
    };

    TArrayView<const FRegistro> Lista()
    {
        // Both sections retain the order of the original journal.
        static const TArray<FRegistro> Todos = [] {
            TArray<FRegistro> Registros;
            Registros.Append(Pesquisa, UE_ARRAY_COUNT(Pesquisa));
            Registros.Append(Sofia, UE_ARRAY_COUNT(Sofia));
            return Registros;
        }();
        return Todos;
    }
}

#undef LOCTEXT_NAMESPACE
