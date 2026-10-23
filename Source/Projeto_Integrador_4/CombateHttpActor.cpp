#include "CombateHttpActor.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"

namespace {
FString Pasta() { return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Combate")); }
FString Arquivo(const FString& Fase) { return FPaths::Combine(Pasta(), Fase + TEXT(".json")); }
FString Texto(const TSharedRef<FJsonObject>& O) { FString S; FJsonSerializer::Serialize(O,TJsonWriterFactory<>::Create(&S)); return S; }
TSharedPtr<FJsonObject> Ler(const FString& S) { TSharedPtr<FJsonObject> O; FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(S), O); return O; }
TSharedPtr<FJsonObject> LerArquivo(const FString& Path) { FString S; return FFileHelper::LoadFileToString(S,*Path) ? Ler(S) : nullptr; }
bool Atomico(const FString& Path, const FString& S) {
    IFileManager::Get().MakeDirectory(*Pasta(),true);
    FString Temp=Path+TEXT(".tmp");
    return FFileHelper::SaveStringToFile(S,*Temp,FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM)
        && IFileManager::Get().Move(*Path,*Temp,true,true);
}
TSharedRef<FJsonObject> Posicao(FVector P) {
    auto O=MakeShared<FJsonObject>();O->SetNumberField(TEXT("x"),P.X);O->SetNumberField(TEXT("y"),P.Y);O->SetNumberField(TEXT("z"),P.Z);return O;
}
TSharedRef<FJsonObject> Serializar(const FEstadoLuta& E,const FString& Sessao,int64 Rev) {
    auto O=MakeShared<FJsonObject>();O->SetStringField(TEXT("sessao"),Sessao);O->SetNumberField(TEXT("revisao"),double(Rev));
    O->SetNumberField(TEXT("vidaJogador"),E.VidaJogador);O->SetNumberField(TEXT("vidaMaximaJogador"),E.VidaMaximaJogador);
    O->SetObjectField(TEXT("posicaoJogador"),Posicao(E.TransformJogador.GetLocation()));O->SetNumberField(TEXT("rotacaoJogador"),E.TransformJogador.Rotator().Yaw);
    O->SetStringField(TEXT("bossId"),E.BossId);O->SetNumberField(TEXT("vidaBoss"),E.VidaBoss);O->SetNumberField(TEXT("vidaMaximaBoss"),E.VidaMaximaBoss);
    O->SetObjectField(TEXT("posicaoBoss"),Posicao(E.TransformBoss.GetLocation()));O->SetNumberField(TEXT("rotacaoBoss"),E.TransformBoss.Rotator().Yaw);
    O->SetBoolField(TEXT("emCombate"),E.EmCombate);O->SetBoolField(TEXT("bossFuria"),E.BossFuria);return O;
}
bool Vida(double V,double M) { return FMath::IsFinite(V)&&FMath::IsFinite(M)&&M>0&&M<=1e9&&V>=0&&V<=M; }
bool Valido(const FEstadoLuta& E) {return Vida(E.VidaJogador,E.VidaMaximaJogador)&&Vida(E.VidaBoss,E.VidaMaximaBoss)&&!E.BossId.IsEmpty()
    &&!E.TransformJogador.ContainsNaN()&&!E.TransformBoss.ContainsNaN();}
bool Parse(const TSharedPtr<FJsonObject>& O,FEstadoLuta& E) {
    if(!O.IsValid())return false;
    const TSharedPtr<FJsonObject>* P=nullptr;const TSharedPtr<FJsonObject>* B=nullptr;double Y=0,BY=0;
    if(!O->TryGetNumberField(TEXT("vidaJogador"),E.VidaJogador)||!O->TryGetNumberField(TEXT("vidaMaximaJogador"),E.VidaMaximaJogador)
        ||!O->TryGetNumberField(TEXT("vidaBoss"),E.VidaBoss)||!O->TryGetNumberField(TEXT("vidaMaximaBoss"),E.VidaMaximaBoss)
        ||!O->TryGetStringField(TEXT("bossId"),E.BossId)||!O->TryGetObjectField(TEXT("posicaoJogador"),P)||!O->TryGetObjectField(TEXT("posicaoBoss"),B)
        ||!O->TryGetNumberField(TEXT("rotacaoJogador"),Y)||!O->TryGetNumberField(TEXT("rotacaoBoss"),BY))return false;
    double X1,Y1,Z1,X2,Y2,Z2;
    if(!(*P)->TryGetNumberField(TEXT("x"),X1)||!(*P)->TryGetNumberField(TEXT("y"),Y1)||!(*P)->TryGetNumberField(TEXT("z"),Z1)
        ||!(*B)->TryGetNumberField(TEXT("x"),X2)||!(*B)->TryGetNumberField(TEXT("y"),Y2)||!(*B)->TryGetNumberField(TEXT("z"),Z2))return false;
    E.TransformJogador=FTransform(FRotator(0,Y,0),FVector(X1,Y1,Z1));E.TransformBoss=FTransform(FRotator(0,BY,0),FVector(X2,Y2,Z2));
    O->TryGetBoolField(TEXT("emCombate"),E.EmCombate);O->TryGetBoolField(TEXT("bossFuria"),E.BossFuria);return Valido(E);
}
bool Sucesso(const FHttpResponsePtr& R,bool S) {return S&&R.IsValid()&&R->GetResponseCode()>=200&&R->GetResponseCode()<300;}
FString Base(const FString& URL) {FString S=URL.TrimStartAndEnd();while(S.RemoveFromEnd(TEXT("/"))){}return S;}
}
ACombateHttpActor::ACombateHttpActor(){PrimaryActorTick.bCanEverTick=false;}
bool ACombateHttpActor::FaseDeCombate(const FString& F){return F==TEXT("Cena_Fase1")||F==TEXT("Cena_Fase2")||F==TEXT("Cena_Fase3_Nova")||F==TEXT("Cena_Fase4_Nova");}
void ACombateHttpActor::NotificarMudancaVida(UObject* Contexto){
    UWorld* W=GEngine->GetWorldFromContextObject(Contexto,EGetWorldErrorMode::ReturnNull);
    if(W)for(TActorIterator<ACombateHttpActor> I(W);I;++I){I->CapturarEstado();break;}
}
void ACombateHttpActor::LimparCacheLocal(){
    for(const FString& F: {TEXT("Cena_Fase1"),TEXT("Cena_Fase2"),TEXT("Cena_Fase3_Nova"),TEXT("Cena_Fase4_Nova"),TEXT("UltimaLuta")})
        IFileManager::Get().Delete(*Arquivo(F),false,true);
}
void ACombateHttpActor::CarregarLuta(FString Fase){
    if(Carregando||!FaseDeCombate(Fase))return;
    FaseAtual=Fase;Carregando=true;DadosProntos=false;
    auto Pedido=FHttpModule::Get().CreateRequest();Pedido->SetURL(Base(UrlApi)+TEXT("/")+Fase+TEXT("/combate"));Pedido->SetVerb(TEXT("GET"));Pedido->SetTimeout(12);
    Pedido->OnProcessRequestComplete().BindWeakLambda(this,[this](FHttpRequestPtr Q,FHttpResponsePtr R,bool OK){
        Carregando=false;auto Local=LerArquivo(Arquivo(FaseAtual));auto Root=Sucesso(R,OK)?Ler(R->GetContentAsString()):nullptr;
        TSharedPtr<FJsonObject> Escolhido;const TSharedPtr<FJsonObject>* Remoto=nullptr;FEstadoLuta Estado;double Rev=0;
        if(Root.IsValid()&&Root->TryGetStringField(TEXT("sessao"),Sessao)) {
            Root->TryGetObjectField(TEXT("estado"),Remoto);
            if(Remoto)Escolhido=*Remoto;
            double RRev=0,LRev=0;FString LS;
            if(Escolhido.IsValid())Escolhido->TryGetNumberField(TEXT("revisao"),RRev);
            if(Local.IsValid()&&Local->TryGetStringField(TEXT("sessao"),LS)&&LS==Sessao&&Local->TryGetNumberField(TEXT("revisao"),LRev)&&LRev>=RRev)Escolhido=Local;
        }else if(Local.IsValid()&&Local->TryGetStringField(TEXT("sessao"),Sessao)){
            Escolhido=Local;UE_LOG(LogTemp,Warning,TEXT("SaveCombate: servidor indisponivel; retomando copia local."));
        }else{AoFalharLuta(TEXT("Nao foi possivel carregar a luta. Confira a conexao e reabra a fase; o save nao foi sobrescrito."));return;}
        bool Tem=Parse(Escolhido,Estado);
        if(Escolhido.IsValid()&&!Tem){AoFalharLuta(TEXT("O save da luta esta invalido. Ele foi preservado para recuperacao."));return;}
        if(Tem){Escolhido->TryGetNumberField(TEXT("revisao"),Rev);UltimaRevisao=int64(Rev);}
        AoCarregarLuta(Estado,Tem);
    });
    if(!Pedido->ProcessRequest()){Carregando=false;AoFalharLuta(TEXT("Nao foi possivel consultar o save da luta."));}
}
void ACombateHttpActor::ConfirmarRestauracao(){DadosProntos=!Sessao.IsEmpty();}
void ACombateHttpActor::GravarLuta(FEstadoLuta E){
    if(!DadosProntos||!Valido(E))return;
    bool Mudou=!TemUltimoEstado||E.VidaJogador!=UltimoEstado.VidaJogador||E.VidaBoss!=UltimoEstado.VidaBoss
        ||E.VidaMaximaJogador!=UltimoEstado.VidaMaximaJogador||E.VidaMaximaBoss!=UltimoEstado.VidaMaximaBoss
        ||E.BossFuria!=UltimoEstado.BossFuria||E.EmCombate!=UltimoEstado.EmCombate
        ||!E.TransformJogador.GetLocation().Equals(UltimoEstado.TransformJogador.GetLocation(),25)
        ||!E.TransformBoss.GetLocation().Equals(UltimoEstado.TransformBoss.GetLocation(),25)
        ||!E.TransformJogador.GetRotation().Equals(UltimoEstado.TransformJogador.GetRotation(),.025f);
    if(Mudou){
        UltimaRevisao=FMath::Max(UltimaRevisao+1,FDateTime::UtcNow().ToUnixTimestamp()*1000+FDateTime::UtcNow().GetMillisecond());
        auto O=Serializar(E,Sessao,UltimaRevisao);O->SetStringField(TEXT("fase"),FaseAtual);FString J=Texto(O);
        if(!Atomico(Arquivo(FaseAtual),J)){if(!AvisouErro){AvisouErro=true;AoFalharLuta(TEXT("Nao foi possivel gravar a copia local da luta."));}return;}
        Atomico(Arquivo(TEXT("UltimaLuta")),J);JsonPendente=J;UltimoEstado=E;TemUltimoEstado=true;
    }
    EnviarPendente();
}
void ACombateHttpActor::EnviarPendente(){
    if(Enviando||JsonPendente.IsEmpty()||FPlatformTime::Seconds()<ProximoEnvio)return;
    Enviando=true;ProximoEnvio=FPlatformTime::Seconds()+1;FString Enviado=JsonPendente;
    auto Pedido=FHttpModule::Get().CreateRequest();Pedido->SetURL(Base(UrlApi)+TEXT("/")+FaseAtual+TEXT("/combate"));Pedido->SetVerb(TEXT("PUT"));Pedido->SetHeader(TEXT("Content-Type"),TEXT("application/json"));Pedido->SetContentAsString(Enviado);Pedido->SetTimeout(8);
    Pedido->OnProcessRequestComplete().BindWeakLambda(this,[this,Enviado](FHttpRequestPtr Q,FHttpResponsePtr R,bool OK){
        Enviando=false;
        if(Sucesso(R,OK)){if(JsonPendente==Enviado)JsonPendente.Empty();AvisouErro=false;}
        else if(R.IsValid()&&R->GetResponseCode()==409){DadosProntos=false;AoFalharLuta(TEXT("A partida foi reiniciada em outra sessao. Reabra a fase antes de continuar."));}
        else{ProximoEnvio=FPlatformTime::Seconds()+3;if(!AvisouErro){AvisouErro=true;UE_LOG(LogTemp,Warning,TEXT("SaveCombate: gravado localmente; sincronizacao pendente."));}}
    });
    if(!Pedido->ProcessRequest()){Enviando=false;ProximoEnvio=FPlatformTime::Seconds()+3;}
}
void ACombateHttpActor::ContinuarLuta(){ConsultarUltimaLuta(true);}
void ACombateHttpActor::ConsultarContinuar(){ConsultarUltimaLuta(false);}
void ACombateHttpActor::ConsultarUltimaLuta(bool Abrir){
    if(Carregando)return;Carregando=true;
    auto Pedido=FHttpModule::Get().CreateRequest();Pedido->SetURL(Base(UrlApi)+TEXT("/continuar-combate"));Pedido->SetVerb(TEXT("GET"));Pedido->SetTimeout(12);
    Pedido->OnProcessRequestComplete().BindWeakLambda(this,[this,Abrir](FHttpRequestPtr Q,FHttpResponsePtr R,bool OK){
        Carregando=false;auto Local=LerArquivo(Arquivo(TEXT("UltimaLuta")));auto Remoto=Sucesso(R,OK)?Ler(R->GetContentAsString()):nullptr;
        FString Fase,LS,RS,LF;double LR=0,RR=0;FEstadoLuta E;
        if(Remoto.IsValid()){Remoto->TryGetStringField(TEXT("sessao"),RS);Remoto->TryGetStringField(TEXT("fase"),Fase);Remoto->TryGetNumberField(TEXT("revisao"),RR);}
        if(Local.IsValid()&&Parse(Local,E)&&E.VidaJogador>0&&Local->TryGetStringField(TEXT("sessao"),LS)
            &&Local->TryGetStringField(TEXT("fase"),LF)&&Local->TryGetNumberField(TEXT("revisao"),LR)
            &&(!Remoto.IsValid()||(LS==RS&&LR>=RR)))Fase=LF;
        if(!FaseDeCombate(Fase)){if(Abrir)AoFalharLuta(TEXT("Nao ha luta salva para continuar. Use Iniciar experimento."));else AoConsultarContinuar(false);return;}
        if(Abrir)AoContinuarLuta(Fase);else AoConsultarContinuar(true);
    });
    if(!Pedido->ProcessRequest()){Carregando=false;if(Abrir)AoFalharLuta(TEXT("Nao foi possivel consultar a ultima luta."));else AoConsultarContinuar(false);}
}
