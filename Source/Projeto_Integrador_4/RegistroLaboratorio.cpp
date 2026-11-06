#include "RegistroLaboratorio.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

#include "RegistrosLaboratorio.inl"

#define LOCTEXT_NAMESPACE "RegistroLaboratorio"

namespace
{
    const FLinearColor Fundo(0.026f, 0.047f, 0.049f);
    const FLinearColor Barra(0.042f, 0.080f, 0.082f);
    const FLinearColor Dourado(0.95f, 0.74f, 0.34f);
    const FLinearColor Texto(0.92f, 0.92f, 0.86f);
}

void URegistroLaboratorio::AbrirNoTerminal(AActor* Terminal, int32 ProximaArea)
{
    if (!IsValid(Terminal))
    {
        return;
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(Terminal, 0);
    const APawn* Jogador = PC ? PC->GetPawn() : nullptr;
    constexpr double Alcance = 240.0;
    if (!Jogador || FVector::DistSquared(Jogador->GetActorLocation(), Terminal->GetActorLocation()) > FMath::Square(Alcance))
    {
        return;
    }

    TArray<UUserWidget*> Abertos;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(Terminal, Abertos, StaticClass(), true);
    if (!Abertos.IsEmpty())
    {
        return;
    }

    auto* Painel = CreateWidget<URegistroLaboratorio>(PC);
    if (!Painel)
    {
        return;
    }
    Painel->Area = FMath::Clamp(ProximaArea, 1, 5);
    Painel->bCursorAnterior = PC->bShowMouseCursor;
    Painel->SetIsFocusable(true);
    Painel->AddToViewport(40);
    PC->bShowMouseCursor = true;

    FInputModeUIOnly Modo;
    Modo.SetWidgetToFocus(Painel->TakeWidget());
    Modo.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(Modo);
}

TSharedRef<SWidget> URegistroLaboratorio::RebuildWidget()
{
    using namespace RegistrosLaboratorio;

    const FSlateBrush* Branco = FCoreStyle::Get().GetBrush(TEXT("WhiteBrush"));
    TSharedRef<SVerticalBox> Indice = SNew(SVerticalBox);
    const auto Registros = Lista();
    for (int32 I = 0; I < Registros.Num(); ++I)
    {
        if (I == 0 || I == UE_ARRAY_COUNT(Pesquisa))
        {
            Indice->AddSlot().AutoHeight().Padding(0, I == 0 ? 0 : 18, 0, 7)
            [
                SNew(STextBlock)
                .Text(I == 0 ? LOCTEXT("Pesquisa", "DIÁRIO DE PESQUISA") : LOCTEXT("Sofia", "REGISTROS DA SOFIA"))
                .Font(FCoreStyle::GetDefaultFontStyle(TEXT("Bold"), 13))
                .ColorAndOpacity(Dourado)
            ];
        }

        const FRegistro& Registro = Registros[I];
        if (Registro.Area > Area)
        {
            continue;
        }

        Indice->AddSlot().AutoHeight().Padding(0, 0, 0, 3)
        [
            SNew(SButton)
            .ContentPadding(FMargin(9, 7))
            .ButtonColorAndOpacity_Lambda([this, I] {
                return FSlateColor(Selecionado == I ? FLinearColor(0.24f, 0.39f, 0.38f) : FLinearColor(0.10f, 0.17f, 0.17f));
            })
            .OnClicked_Lambda([this, I] {
                Selecionar(I);
                return FReply::Handled();
            })
            [
                SNew(STextBlock)
                .Text(Registro.Titulo)
                .Font(FCoreStyle::GetDefaultFontStyle(TEXT("Medium"), 15))
                .ColorAndOpacity(Texto)
                .AutoWrapText(true)
            ]
        ];
    }

    return SNew(SOverlay)
        + SOverlay::Slot()
        [
            SNew(SBorder).BorderImage(Branco).BorderBackgroundColor(FLinearColor(0, 0, 0, .67f))
        ]
        + SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).Padding(24)
        [
            SNew(SScaleBox).Stretch(EStretch::ScaleToFit).StretchDirection(EStretchDirection::DownOnly)
            [
                SNew(SBox).WidthOverride(1080).HeightOverride(670)
                [
                    SNew(SBorder).BorderImage(Branco).BorderBackgroundColor(Dourado).Padding(1)
                    [
                        SNew(SBorder).BorderImage(Branco).BorderBackgroundColor(Fundo).Padding(0)
                        [
                            SNew(SVerticalBox)
                            + SVerticalBox::Slot().AutoHeight()
                            [
                                SNew(SBorder).BorderImage(Branco).BorderBackgroundColor(Barra).Padding(FMargin(26, 16))
                                [
                                    SNew(SHorizontalBox)
                                    + SHorizontalBox::Slot().FillWidth(1).VAlign(VAlign_Center)
                                    [
                                        SNew(STextBlock).Text(LOCTEXT("Titulo", "Terminal do laboratório"))
                                        .Font(FCoreStyle::GetDefaultFontStyle(TEXT("Bold"), 28)).ColorAndOpacity(Texto)
                                    ]
                                    + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
                                    [
                                        SNew(SButton).ContentPadding(FMargin(14, 7))
                                        .OnClicked_Lambda([this] { Fechar(); return FReply::Handled(); })
                                        [
                                            SNew(STextBlock).Text(LOCTEXT("Fechar", "Fechar  ·  Esc"))
                                            .Font(FCoreStyle::GetDefaultFontStyle(TEXT("Medium"), 15))
                                        ]
                                    ]
                                ]
                            ]
                            + SVerticalBox::Slot().FillHeight(1).Padding(18, 18, 22, 20)
                            [
                                SNew(SHorizontalBox)
                                + SHorizontalBox::Slot().AutoWidth()
                                [
                                    SNew(SBox).WidthOverride(246)
                                    [
                                        SNew(SBorder).BorderImage(Branco).BorderBackgroundColor(Barra).Padding(12)
                                        [
                                            SNew(SScrollBox)
                                            + SScrollBox::Slot()[Indice]
                                        ]
                                    ]
                                ]
                                + SHorizontalBox::Slot().FillWidth(1).Padding(30, 7, 5, 3)
                                [
                                    SAssignNew(Leitura, SScrollBox)
                                    + SScrollBox::Slot().Padding(0, 0, 22, 22)
                                    [
                                        SNew(SVerticalBox)
                                        + SVerticalBox::Slot().AutoHeight().Padding(0, 0, 0, 10)
                                        [
                                            SNew(STextBlock)
                                            .Text_Lambda([this] {
                                                return Selecionado < UE_ARRAY_COUNT(Pesquisa)
                                                    ? LOCTEXT("GrupoPesquisa", "Diário de pesquisa")
                                                    : LOCTEXT("GrupoSofia", "Registros da Sofia");
                                            })
                                            .Font(FCoreStyle::GetDefaultFontStyle(TEXT("Medium"), 15)).ColorAndOpacity(Dourado)
                                        ]
                                        + SVerticalBox::Slot().AutoHeight().Padding(0, 0, 0, 20)
                                        [
                                            SNew(STextBlock).Text_Lambda([this] { return Lista()[Selecionado].Titulo; })
                                            .Font(FCoreStyle::GetDefaultFontStyle(TEXT("Bold"), 30)).ColorAndOpacity(Texto)
                                            .AutoWrapText(true)
                                        ]
                                        + SVerticalBox::Slot().AutoHeight()
                                        [
                                            SNew(STextBlock).Text_Lambda([this] { return Lista()[Selecionado].Texto; })
                                            .Font(FCoreStyle::GetDefaultFontStyle(TEXT("Regular"), 19)).ColorAndOpacity(Texto)
                                            .AutoWrapText(true).LineHeightPercentage(1.22f)
                                        ]
                                    ]
                                ]
                            ]
                        ]
                    ]
                ]
            ]
        ];
}

void URegistroLaboratorio::Selecionar(int32 Indice)
{
    Selecionado = Indice;
    if (Leitura.IsValid())
    {
        Leitura->ScrollToStart();
    }
}

FReply URegistroLaboratorio::NativeOnPreviewKeyDown(const FGeometry& Geometry, const FKeyEvent& Event)
{
    if (Event.GetKey() == EKeys::Escape || Event.GetKey() == EKeys::Gamepad_FaceButton_Right)
    {
        Fechar();
        return FReply::Handled();
    }
    return Super::NativeOnPreviewKeyDown(Geometry, Event);
}

void URegistroLaboratorio::Fechar()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->bShowMouseCursor = bCursorAnterior;
        PC->SetInputMode(FInputModeGameOnly());
    }
    RemoveFromParent();
}

void URegistroLaboratorio::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    Leitura.Reset();
}

#undef LOCTEXT_NAMESPACE
