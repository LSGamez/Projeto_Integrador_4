#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotaDoMapa.generated.h"

class UInstancedStaticMeshComponent;
class UMaterialInterface;
class UStaticMeshComponent;

USTRUCT(BlueprintType)
struct FRotaDeArea
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category="Rota")
    TObjectPtr<AActor> Bandeira;

    UPROPERTY(EditAnywhere, Category="Rota")
    TArray<FVector> Pontos;
};

UCLASS()
class PROJETO_INTEGRADOR_4_API ARotaDoMapa : public AActor
{
    GENERATED_BODY()

public:
    ARotaDoMapa();

    UFUNCTION(BlueprintCallable, Category="Jornada")
    void MostrarArea(int32 AreaDisponivel);

private:
    UPROPERTY(VisibleAnywhere, Category="Rota")
    TObjectPtr<UInstancedStaticMeshComponent> Segmentos;

    // The entries are in campaign order, starting at area 1.
    UPROPERTY(EditInstanceOnly, Category="Rota")
    TArray<FRotaDeArea> Rotas;

    UPROPERTY(EditInstanceOnly, Category="Aparência")
    TObjectPtr<UMaterialInterface> MaterialDisponivel;

    UPROPERTY(EditInstanceOnly, Category="Aparência")
    TObjectPtr<UMaterialInterface> MaterialBloqueado;

    UPROPERTY(EditInstanceOnly, Category="Aparência")
    TObjectPtr<UMaterialInterface> MaterialTrilha;

    int32 UltimaArea = INDEX_NONE;

    void DesenharTrilha(const TArray<FVector>& Pontos);
};
