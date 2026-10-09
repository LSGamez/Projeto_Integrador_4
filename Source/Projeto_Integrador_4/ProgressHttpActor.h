#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProgressHttpActor.generated.h"

UCLASS()
class PROJETO_INTEGRADOR_4_API AProgressHttpActor : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progresso")
    FString UrlApi = TEXT("https://unreal-api-production.up.railway.app/api/progresso");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progresso", meta = (ClampMin = "1.0", ClampMax = "90.0"))
    float TempoLimiteNovoJogo = 45.0f;

    // Limpa o save pela API. O evento de sucesso pode abrir a fase inicial.
    UFUNCTION(BlueprintCallable, Category = "Progresso")
    void NovoJogo(FString FaseInicial, int32 VidaInicial);

    UFUNCTION(BlueprintImplementableEvent, Category = "Progresso")
    void AoIniciarNovoJogo(const FString& FaseInicial);

    UFUNCTION(BlueprintImplementableEvent, Category = "Progresso")
    void AoFalharNovoJogo(const FString& Mensagem);

    UPROPERTY(BlueprintReadOnly, Category = "Progresso")
    bool IniciandoNovoJogo = false;

    // Consulta o inventario e os IDs coletados da fase.
    UFUNCTION(BlueprintCallable, Category = "Progresso")
    void CarregarFase(FString Fase);

    // Implemente este evento no BP_ProgressHttpActor para aplicar o save.
    UFUNCTION(BlueprintImplementableEvent, Category = "Progresso")
    void AoCarregarFase(
        int32 ItemA,
        int32 ItemB,
        int32 Bugigangas,
        const TArray<FString>& IdsColetados
    );

    UFUNCTION(BlueprintImplementableEvent, Category = "Progresso")
    void AoFalharCarregamento(const FString& Mensagem);

    UPROPERTY(BlueprintReadOnly, Category = "Progresso")
    bool CarregandoFase = false;

    UFUNCTION(BlueprintCallable, Category = "Progresso")
    void CarregarProgresso();

    UFUNCTION(BlueprintCallable, Category = "Progresso")
    void SalvarProgresso(
        FString Fase,
        FString Checkpoint,
        int32 Vida,
        FString Itens
    );

    UFUNCTION(BlueprintCallable, Category = "Progresso")
    void RegistrarColeta(
        FString Fase,
        FString IdItem,
        int32 ItemA,
        int32 ItemB,
        int32 Bugigangas
    );

private:
    FString MontarUrl(const FString& Caminho = TEXT("")) const;
    void CarregarColetadosDaFase(
        FString Fase,
        int32 ItemA,
        int32 ItemB,
        int32 Bugigangas
    );

    void NotificarFalhaCarregamento(const FString& Mensagem);
    void NotificarFalhaNovoJogo(const FString& Mensagem);
};
