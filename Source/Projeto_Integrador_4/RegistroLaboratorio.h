#pragma once

#include "Blueprint/UserWidget.h"
#include "RegistroLaboratorio.generated.h"

class SScrollBox;

UCLASS()
class PROJETO_INTEGRADOR_4_API URegistroLaboratorio : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Laboratório")
    static void AbrirNoTerminal(AActor* Terminal, int32 ProximaArea);

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;
    virtual FReply NativeOnPreviewKeyDown(const FGeometry& Geometry, const FKeyEvent& Event) override;

private:
    void Selecionar(int32 Indice);
    void Fechar();

    int32 Area = 1;
    int32 Selecionado = 0;
    bool bCursorAnterior = false;
    TSharedPtr<SScrollBox> Leitura;
};
