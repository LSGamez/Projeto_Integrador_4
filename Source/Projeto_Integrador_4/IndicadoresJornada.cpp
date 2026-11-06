#include "IndicadoresJornada.h"

#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Framework/Application/SlateApplication.h"
#include "Rendering/DrawElements.h"
#include "Rendering/SlateRenderer.h"
#include "Styling/CoreStyle.h"

void USetaObjetivo::NativeConstruct()
{
    Super::NativeConstruct();
    SetVisibility(ESlateVisibility::HitTestInvisible);
    ForceVolatile(true);
}

int32 USetaObjetivo::NativePaint(const FPaintArgs& Args, const FGeometry& Geometry,
    const FSlateRect& CullingRect, FSlateWindowElementList& Elements, int32 Layer,
    const FWidgetStyle& Style, bool bParentEnabled) const
{
    const FVector2D Size = Geometry.GetLocalSize();
    const double Time = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0;
    const FVector2D Center(Size.X * 0.5, Size.Y * 0.47 + 5.0 * FMath::Sin(Time * 4.0));
    // Two broad arms with diagonal tips, joined into a filled chevron.
    const FVector2D Chevron[] = {
        {-18.0, -5.0}, {-11.0, -12.0}, {0.0, -1.0},
        {11.0, -12.0}, {18.0, -5.0}, {0.0, 13.0}
    };
    const TArray<SlateIndex> Triangles{0, 1, 2, 0, 2, 5, 2, 3, 4, 2, 4, 5};
    const auto Transform = Geometry.GetAccumulatedRenderTransform();
    const FSlateBrush& Brush = *FCoreStyle::Get().GetBrush(TEXT("WhiteBrush"));
    const auto Resource = FSlateApplication::Get().GetRenderer()->GetResourceHandle(Brush);

    auto DrawChevron = [&](int32 DrawLayer, double Scale, double OffsetY, FColor Color)
    {
        TArray<FSlateVertex> Vertices;
        Vertices.Reserve(UE_ARRAY_COUNT(Chevron));
        for (const FVector2D& Point : Chevron)
        {
            const FVector2f Position(Center + Point * Scale + FVector2D(0.0, OffsetY));
            Vertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(
                Transform, Position, FVector2f(0.5f, 0.5f), Color));
        }
        FSlateDrawElement::MakeCustomVerts(Elements, DrawLayer, Resource, Vertices, Triangles, nullptr, 0, 0);
    };

    DrawChevron(Layer, 1.12, 1.0, FColor(38, 27, 17));
    DrawChevron(Layer + 1, 1.0, 0.0, FColor(255, 210, 105));
    return Layer + 1;
}

UDestaqueColetavel::UDestaqueColetavel()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 1.0f / 30.0f;
}

void UDestaqueColetavel::BeginPlay()
{
    Super::BeginPlay();

    AActor* Item = GetOwner();
    TInlineComponentArray<UStaticMeshComponent*> Malhas(Item);
    FBox Limites(ForceInit);
    for (UStaticMeshComponent* Original : Malhas)
    {
        if (!Original->GetStaticMesh() || !Original->IsVisible())
        {
            continue;
        }

        Limites += Original->Bounds.GetBox();
        // Keep the original collision and overlap trigger at the collection point.
        auto* Visual = NewObject<UStaticMeshComponent>(Item);
        Visual->SetMobility(EComponentMobility::Movable);
        Visual->SetStaticMesh(Original->GetStaticMesh());
        Visual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Visual->SetGenerateOverlapEvents(false);
        Visual->SetCanEverAffectNavigation(false);
        for (int32 Slot = 0; Slot < Original->GetNumMaterials(); ++Slot)
        {
            Visual->SetMaterial(Slot, Original->GetMaterial(Slot));
        }

        Visual->SetupAttachment(Original);
        Visual->RegisterComponent();
        Original->SetVisibility(false);
        MalhasFlutuantes.Add(Visual);
    }

    if (Limites.IsValid)
    {
        auto* Seta = NewObject<UWidgetComponent>(Item);
        Seta->SetMobility(EComponentMobility::Movable);
        Seta->SetWidgetSpace(EWidgetSpace::Screen);
        Seta->SetWidgetClass(USetaObjetivo::StaticClass());
        Seta->SetDrawSize(FVector2D(44.0, 42.0));
        Seta->SetPivot(FVector2D(0.5, 1.0));
        Seta->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Seta->SetupAttachment(Item->GetRootComponent());
        Seta->RegisterComponent();
        const FVector Center = Limites.GetCenter();
        Seta->SetWorldLocation(FVector(Center.X, Center.Y, Limites.Max.Z + 30.0));
        Seta->SetCullDistance(2200.0f);
    }
    else
    {
        SetComponentTickEnabled(false);
    }
}

void UDestaqueColetavel::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    constexpr double CicloEmSegundos = 4.5;
    constexpr double AlturaMedia = 18.0;
    constexpr double Amplitude = 9.0;
    const double Fase = GetWorld()->GetTimeSeconds() * (2.0 * UE_PI / CicloEmSegundos);
    const FVector Lift(0.0, 0.0, AlturaMedia + Amplitude * FMath::Sin(Fase));
    for (UStaticMeshComponent* Visual : MalhasFlutuantes)
    {
        const FTransform& Base = Visual->GetAttachParent()->GetComponentTransform();
        Visual->SetRelativeLocation(Base.InverseTransformVector(Lift));
    }
}
