#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombateHttpActor.generated.h"

USTRUCT(BlueprintType)
struct PROJETO_INTEGRADOR_4_API FEstadoLuta
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) double VidaJogador = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) double VidaMaximaJogador = 100;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FTransform TransformJogador;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString BossId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) double VidaBoss = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) double VidaMaximaBoss = 100;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FTransform TransformBoss;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool EmCombate = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool BossFuria = false;
};

// Transporte HTTP e copia local. Captura/restauracao da vida ficam no Blueprint.
UCLASS()
class PROJETO_INTEGRADOR_4_API ACombateHttpActor : public AActor
{
    GENERATED_BODY()
public:
    ACombateHttpActor();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Save da luta")
    FString UrlApi = TEXT("https://unreal-api-x1kl.onrender.com/api/progresso");
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Save da luta") FString BossIdDaFase;
    UPROPERTY(BlueprintReadOnly, Category="Save da luta") bool DadosProntos = false;
    UFUNCTION(BlueprintCallable, Category="Save da luta") void CarregarLuta(FString Fase);
    UFUNCTION(BlueprintCallable, Category="Save da luta") void ConfirmarRestauracao();
    UFUNCTION(BlueprintCallable, Category="Save da luta") void GravarLuta(FEstadoLuta Estado);
    UFUNCTION(BlueprintCallable, Category="Save da luta") void ContinuarLuta();
    UFUNCTION(BlueprintCallable, Category="Save da luta") void ConsultarContinuar();
    UFUNCTION(BlueprintImplementableEvent, Category="Save da luta") void AoConsultarContinuar(bool Existe);
    UFUNCTION(BlueprintImplementableEvent, Category="Save da luta") void AoCarregarLuta(const FEstadoLuta& Estado, bool TemEstado);
    UFUNCTION(BlueprintImplementableEvent, Category="Save da luta") void AoContinuarLuta(const FString& Fase);
    UFUNCTION(BlueprintImplementableEvent, Category="Save da luta") void AoFalharLuta(const FString& Mensagem);
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Save da luta") void CapturarEstado();
    UFUNCTION(BlueprintPure, Category="Save da luta") static bool FaseDeCombate(const FString& Fase);
    UFUNCTION(BlueprintCallable, Category="Save da luta", meta=(WorldContext="Contexto"))
    static void NotificarMudancaVida(UObject* Contexto);
    static void LimparCacheLocal();
private:
    FString FaseAtual, Sessao, JsonPendente;
    int64 UltimaRevisao = 0;
    double ProximoEnvio = 0;
    bool Enviando = false, Carregando = false, AvisouErro = false;
    bool TemUltimoEstado = false;
    FEstadoLuta UltimoEstado;
    void EnviarPendente();
    void ConsultarUltimaLuta(bool Abrir);
};
