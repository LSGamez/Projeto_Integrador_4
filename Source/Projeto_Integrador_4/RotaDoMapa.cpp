#include "RotaDoMapa.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ARotaDoMapa::ARotaDoMapa()
{
    Segmentos = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("SegmentosDaTrilha"));
    SetRootComponent(Segmentos);
    Segmentos->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Segmentos->SetGenerateOverlapEvents(false);
    Segmentos->SetCanEverAffectNavigation(false);
    Segmentos->SetCastShadow(false);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Forma(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    Segmentos->SetStaticMesh(Forma.Object);
}

void ARotaDoMapa::MostrarArea(int32 AreaDisponivel)
{
    if (UltimaArea == AreaDisponivel)
    {
        return;
    }

    UltimaArea = AreaDisponivel;
    Segmentos->ClearInstances();
    const int32 RotaAtiva = AreaDisponivel - 1;
    for (int32 Index = 0; Index < Rotas.Num(); ++Index)
    {
        if (!IsValid(Rotas[Index].Bandeira))
        {
            continue;
        }

        TInlineComponentArray<UStaticMeshComponent*> Malhas(Rotas[Index].Bandeira);
        for (UStaticMeshComponent* Malha : Malhas)
        {
            if (Malha->GetFName() == TEXT("Cube"))
            {
                Malha->SetMaterial(0, Index == RotaAtiva ? MaterialDisponivel : MaterialBloqueado);
                break;
            }
        }
    }

    if (Rotas.IsValidIndex(RotaAtiva))
    {
        DesenharTrilha(Rotas[RotaAtiva].Pontos);
    }
}

void ARotaDoMapa::DesenharTrilha(const TArray<FVector>& Pontos)
{
    Segmentos->SetMaterial(0, MaterialTrilha);
    constexpr double Espacamento = 65.0;
    double DistanciaAteSegmento = Espacamento * 0.5;
    for (int32 Index = 1; Index < Pontos.Num(); ++Index)
    {
        const FVector Inicio = Pontos[Index - 1];
        const FVector Trecho = Pontos[Index] - Inicio;
        const double Distancia = Trecho.Length();
        if (Distancia < UE_KINDA_SMALL_NUMBER)
        {
            continue;
        }

        const FVector Direcao = Trecho / Distancia;
        const FRotator Rotacao = Direcao.Rotation();
        while (DistanciaAteSegmento < Distancia)
        {
            const FVector Posicao = Inicio + Direcao * DistanciaAteSegmento;
            const FVector Escala(0.36, 0.13, 0.035);
            Segmentos->AddInstance(FTransform(Rotacao, Posicao, Escala), true);
            DistanciaAteSegmento += Espacamento;
        }
        DistanciaAteSegmento -= Distancia;
    }
}
