#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"
#include "IndicadoresJornada.generated.h"

class UStaticMeshComponent;

UCLASS()
class PROJETO_INTEGRADOR_4_API USetaObjetivo : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& Geometry,
        const FSlateRect& CullingRect, FSlateWindowElementList& Elements, int32 Layer,
        const FWidgetStyle& Style, bool bParentEnabled) const override;
};

UCLASS(ClassGroup=(Jornada), meta=(BlueprintSpawnableComponent))
class PROJETO_INTEGRADOR_4_API UDestaqueColetavel : public UActorComponent
{
    GENERATED_BODY()

public:
    UDestaqueColetavel();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY(Transient)
    TArray<TObjectPtr<UStaticMeshComponent>> MalhasFlutuantes;
};
