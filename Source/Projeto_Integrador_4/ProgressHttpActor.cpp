#include "ProgressHttpActor.h"

#include "HttpModule.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

namespace
{
    FString DescreverFalhaApi(const FString& Operacao, const FHttpResponsePtr& Response, bool Sucesso)
    {
        if (!Response.IsValid())
        {
            return FString::Printf(TEXT("Sem resposta do servidor ao %s. Confira a conexao e tente novamente."), *Operacao);
        }
        if (!Sucesso)
        {
            return FString::Printf(TEXT("A conexao foi interrompida ao %s. Tente novamente."), *Operacao);
        }
        return FString::Printf(TEXT("O servidor respondeu HTTP %d ao %s. Tente novamente."), Response->GetResponseCode(), *Operacao);
    }
}

FString AProgressHttpActor::MontarUrl(const FString& Caminho) const
{
    FString Base = UrlApi.TrimStartAndEnd();
    while (Base.RemoveFromEnd(TEXT("/"))) {}
    return Base + Caminho;
}

void AProgressHttpActor::NovoJogo(FString FaseInicial, int32 VidaInicial)
{
    if (IniciandoNovoJogo)
    {
        return;
    }

    if (CarregandoFase)
    {
        NotificarFalhaNovoJogo(TEXT("Aguarde o carregamento da fase terminar."));
        return;
    }

    FaseInicial.TrimStartAndEndInline();
    if (FaseInicial.IsEmpty() || VidaInicial <= 0)
    {
        NotificarFalhaNovoJogo(TEXT("Informe a fase inicial e uma vida inicial maior que zero."));
        return;
    }

    IniciandoNovoJogo = true;

    TSharedPtr<FJsonObject> Dados = MakeShared<FJsonObject>();
    Dados->SetStringField(TEXT("faseInicial"), FaseInicial);
    Dados->SetNumberField(TEXT("vidaInicial"), VidaInicial);

    FString TextoJson;
    TSharedRef<TJsonWriter<>> Escritor = TJsonWriterFactory<>::Create(&TextoJson);
    FJsonSerializer::Serialize(Dados.ToSharedRef(), Escritor);

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Pedido =
        FHttpModule::Get().CreateRequest();

    Pedido->SetURL(MontarUrl(TEXT("/novo-jogo")));
    Pedido->SetVerb(TEXT("POST"));
    Pedido->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Pedido->SetContentAsString(TextoJson);
    Pedido->SetTimeout(FMath::Clamp(TempoLimiteNovoJogo, 1.0f, 90.0f));

    Pedido->OnProcessRequestComplete().BindWeakLambda(
        this,
        [this, FaseInicial](FHttpRequestPtr Request, FHttpResponsePtr Response, bool Sucesso)
        {
            if (!Sucesso || !Response.IsValid()
                || Response->GetResponseCode() < 200 || Response->GetResponseCode() >= 300)
            {
                NotificarFalhaNovoJogo(DescreverFalhaApi(TEXT("iniciar o novo jogo"), Response, Sucesso));
                return;
            }

            IniciandoNovoJogo = false;
            UE_LOG(LogTemp, Display, TEXT("Novo jogo confirmado pela API. Fase inicial: %s"), *FaseInicial);
            // Abra o nivel neste evento, depois que o banco confirmar a limpeza.
            AoIniciarNovoJogo(FaseInicial);
        }
    );

    if (!Pedido->ProcessRequest())
    {
        NotificarFalhaNovoJogo(TEXT("Nao foi possivel enviar o pedido de novo jogo."));
    }
}

void AProgressHttpActor::NotificarFalhaNovoJogo(const FString& Mensagem)
{
    IniciandoNovoJogo = false;
    UE_LOG(LogTemp, Error, TEXT("%s"), *Mensagem);
    AoFalharNovoJogo(Mensagem);
}

void AProgressHttpActor::CarregarFase(FString Fase)
{
    if (IniciandoNovoJogo)
    {
        UE_LOG(LogTemp, Warning, TEXT("Aguarde a confirmacao do novo jogo antes de carregar a fase."));
        return;
    }

    if (CarregandoFase)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ja existe um carregamento de fase em andamento."));
        return;
    }

    Fase.TrimStartAndEndInline();
    if (Fase.IsEmpty())
    {
        NotificarFalhaCarregamento(TEXT("Informe o nome da fase."));
        return;
    }

    CarregandoFase = true;
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Pedido =
        FHttpModule::Get().CreateRequest();

    Pedido->SetURL(MontarUrl(FString::Printf(
        TEXT("/%s/inventario"),
        *FGenericPlatformHttp::UrlEncode(Fase)
    )));
    Pedido->SetVerb(TEXT("GET"));
    Pedido->SetTimeout(15.0f);

    // O callback deixa de ser executado se este ator for destruido.
    Pedido->OnProcessRequestComplete().BindWeakLambda(
        this,
        [this, Fase](FHttpRequestPtr Request, FHttpResponsePtr Response, bool Sucesso)
        {
            if (!Sucesso || !Response.IsValid()
                || Response->GetResponseCode() < 200 || Response->GetResponseCode() >= 300)
            {
                NotificarFalhaCarregamento(DescreverFalhaApi(TEXT("carregar o inventario"), Response, Sucesso));
                return;
            }

            TSharedPtr<FJsonObject> Dados;
            TSharedRef<TJsonReader<>> Leitor =
                TJsonReaderFactory<>::Create(Response->GetContentAsString());

            int32 ItemA = 0;
            int32 ItemB = 0;
            int32 Bugigangas = 0;

            if (!FJsonSerializer::Deserialize(Leitor, Dados) || !Dados.IsValid()
                || !Dados->TryGetNumberField(TEXT("itemA"), ItemA)
                || !Dados->TryGetNumberField(TEXT("itemB"), ItemB)
                || !Dados->TryGetNumberField(TEXT("bugigangas"), Bugigangas)
                || ItemA < 0 || ItemB < 0 || Bugigangas < 0)
            {
                NotificarFalhaCarregamento(TEXT("A API retornou um inventario invalido."));
                return;
            }

            CarregarColetadosDaFase(Fase, ItemA, ItemB, Bugigangas);
        }
    );

    if (!Pedido->ProcessRequest())
    {
        NotificarFalhaCarregamento(TEXT("Nao foi possivel iniciar a consulta do inventario."));
    }
}

void AProgressHttpActor::CarregarColetadosDaFase(
    FString Fase, int32 ItemA, int32 ItemB, int32 Bugigangas)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Pedido =
        FHttpModule::Get().CreateRequest();

    Pedido->SetURL(MontarUrl(FString::Printf(
        TEXT("/%s/coletados"),
        *FGenericPlatformHttp::UrlEncode(Fase)
    )));
    Pedido->SetVerb(TEXT("GET"));
    Pedido->SetTimeout(15.0f);

    Pedido->OnProcessRequestComplete().BindWeakLambda(
        this,
        [this, Fase, ItemA, ItemB, Bugigangas](
            FHttpRequestPtr Request, FHttpResponsePtr Response, bool Sucesso)
        {
            if (!Sucesso || !Response.IsValid()
                || Response->GetResponseCode() < 200 || Response->GetResponseCode() >= 300)
            {
                NotificarFalhaCarregamento(DescreverFalhaApi(TEXT("consultar os itens coletados"), Response, Sucesso));
                return;
            }

            TArray<TSharedPtr<FJsonValue>> Lista;
            TSharedRef<TJsonReader<>> Leitor =
                TJsonReaderFactory<>::Create(Response->GetContentAsString());

            if (!FJsonSerializer::Deserialize(Leitor, Lista))
            {
                NotificarFalhaCarregamento(TEXT("A API retornou uma lista de itens invalida."));
                return;
            }

            TArray<FString> IdsColetados;
            for (const TSharedPtr<FJsonValue>& Valor : Lista)
            {
                FString Id;
                if (!Valor.IsValid() || !Valor->TryGetString(Id) || Id.IsEmpty())
                {
                    NotificarFalhaCarregamento(TEXT("A lista retornada pela API contem um ID invalido."));
                    return;
                }
                IdsColetados.AddUnique(Id);
            }

            CarregandoFase = false;
            UE_LOG(LogTemp, Display,
                TEXT("Fase carregada: %s | ItemA=%d | ItemB=%d | Bugigangas=%d | Coletados=%d"),
                *Fase, ItemA, ItemB, Bugigangas, IdsColetados.Num());

            // Os dados so sao aplicados depois das duas consultas terem sucesso.
            AoCarregarFase(ItemA, ItemB, Bugigangas, IdsColetados);
        }
    );

    if (!Pedido->ProcessRequest())
    {
        NotificarFalhaCarregamento(TEXT("Nao foi possivel iniciar a consulta dos itens coletados."));
    }
}

void AProgressHttpActor::NotificarFalhaCarregamento(const FString& Mensagem)
{
    CarregandoFase = false;
    UE_LOG(LogTemp, Error, TEXT("%s"), *Mensagem);
    AoFalharCarregamento(Mensagem);
}

void AProgressHttpActor::CarregarProgresso()
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Pedido =
        FHttpModule::Get().CreateRequest();

    Pedido->SetURL(MontarUrl());
    Pedido->SetVerb(TEXT("GET"));

    Pedido->OnProcessRequestComplete().BindLambda(
        [](FHttpRequestPtr Request, FHttpResponsePtr Response, bool Sucesso)
        {
            if (Sucesso && Response.IsValid()
                && Response->GetResponseCode() >= 200 && Response->GetResponseCode() < 300)
            {
                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT("Progresso carregado: %s"),
                    *Response->GetContentAsString()
                );
            }
            else
            {
                UE_LOG(
                    LogTemp,
                    Error,
                    TEXT("Nao foi possivel carregar o progresso.")
                );
            }
        }
    );

    Pedido->ProcessRequest();
}

void AProgressHttpActor::SalvarProgresso(
    FString Fase,
    FString Checkpoint,
    int32 Vida,
    FString Itens)
{
    if (IniciandoNovoJogo)
    {
        UE_LOG(LogTemp, Warning, TEXT("O save esta sendo reiniciado. Aguarde antes de salvar progresso."));
        return;
    }

    Fase.TrimStartAndEndInline();
    TSharedPtr<FJsonObject> Dados = MakeShared<FJsonObject>();

    Dados->SetStringField(TEXT("fase"), Fase);
    Dados->SetStringField(TEXT("checkpoint"), Checkpoint);
    Dados->SetNumberField(TEXT("vida"), Vida);
    Dados->SetStringField(TEXT("itens"), Itens);

    FString TextoJson;
    TSharedRef<TJsonWriter<>> Escritor =
        TJsonWriterFactory<>::Create(&TextoJson);

    FJsonSerializer::Serialize(Dados.ToSharedRef(), Escritor);

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Pedido =
        FHttpModule::Get().CreateRequest();

    Pedido->SetURL(MontarUrl());
    Pedido->SetVerb(TEXT("PUT"));
    Pedido->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Pedido->SetContentAsString(TextoJson);

    Pedido->OnProcessRequestComplete().BindLambda(
        [](FHttpRequestPtr Request, FHttpResponsePtr Response, bool Sucesso)
        {
            if (Sucesso && Response.IsValid()
                && Response->GetResponseCode() >= 200 && Response->GetResponseCode() < 300)
            {
                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT("Resposta ao salvar progresso: %s"),
                    *Response->GetContentAsString()
                );
            }
            else
            {
                UE_LOG(
                    LogTemp,
                    Error,
                    TEXT("Nao foi possivel salvar o progresso.")
                );
            }
        }
    );

    Pedido->ProcessRequest();
}

void AProgressHttpActor::RegistrarColeta(
    FString Fase,
    FString IdItem,
    int32 ItemA,
    int32 ItemB,
    int32 Bugigangas)
{
    if (IniciandoNovoJogo)
    {
        UE_LOG(LogTemp, Warning, TEXT("O save esta sendo reiniciado. Aguarde antes de registrar uma coleta."));
        return;
    }

    Fase.TrimStartAndEndInline();
    IdItem.TrimStartAndEndInline();
    TSharedPtr<FJsonObject> Dados = MakeShared<FJsonObject>();

    Dados->SetStringField(TEXT("fase"), Fase);
    Dados->SetStringField(TEXT("idItem"), IdItem);
    Dados->SetNumberField(TEXT("itemA"), ItemA);
    Dados->SetNumberField(TEXT("itemB"), ItemB);
    Dados->SetNumberField(TEXT("bugigangas"), Bugigangas);

    FString TextoJson;
    TSharedRef<TJsonWriter<>> Escritor =
        TJsonWriterFactory<>::Create(&TextoJson);

    FJsonSerializer::Serialize(Dados.ToSharedRef(), Escritor);

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Pedido =
        FHttpModule::Get().CreateRequest();

    Pedido->SetURL(MontarUrl(TEXT("/coletar")));
    Pedido->SetVerb(TEXT("POST"));
    Pedido->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Pedido->SetContentAsString(TextoJson);

    Pedido->OnProcessRequestComplete().BindLambda(
        [](FHttpRequestPtr Request, FHttpResponsePtr Response, bool Sucesso)
        {
            if (Sucesso && Response.IsValid()
                && Response->GetResponseCode() >= 200 && Response->GetResponseCode() < 300)
            {
                UE_LOG(
                    LogTemp,
                    Display,
                    TEXT("Resposta ao registrar coleta: %s"),
                    *Response->GetContentAsString()
                );
            }
            else
            {
                UE_LOG(
                    LogTemp,
                    Error,
                    TEXT("Nao foi possivel registrar a coleta.")
                );
            }
        }
    );

    Pedido->ProcessRequest();
}
