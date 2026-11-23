#include "ProbeArena.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "ImageUtils.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
UTexture2D* AProbeArenaHUD::UiTexture(const FString& id)const{const auto* entry=UiTextures.Find(id);return entry?entry->Get():nullptr;}
void AProbeArenaHUD::LoadUiResources(){
 UiTextures.Empty();HubLabelHover.Empty();
 HubLabelFont=NewObject<UFont>(this);HubLabelFont->FontCacheType=EFontCacheType::Runtime;HubLabelFont->LegacyFontSize=20;
 const FString font=FPaths::EngineContentDir()/TEXT("Slate/Fonts/Roboto-Light.ttf");
 HubLabelFont->GetMutableInternalCompositeFont()=FCompositeFont(FName(TEXT("Regular")),font,EFontHinting::Default,EFontLoadingPolicy::LazyLoad);
 const FString root=FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir()/TEXT("Presentation"));
 FString text;const FString manifest=root/TEXT("data/presentation/ui-runtime.json");if(IFileManager::Get().FileSize(*manifest)>256*1024||!FFileHelper::LoadFileToString(text,*manifest)){UE_LOG(LogTemp,Warning,TEXT("UI runtime catalog unavailable; fallbacks retained"));return;}
 TSharedPtr<FJsonObject> json;if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(text),json)||!json.IsValid()){return;}
 const TArray<TSharedPtr<FJsonValue>>* resources=nullptr;if(!json->TryGetArrayField(TEXT("Resources"),resources)||!resources||resources->Num()>32){return;}
 for(const auto& value:*resources){const auto item=value->AsObject();if(!item){continue;}FString id,path;double width=0,height=0;if(!item->TryGetStringField(TEXT("Id"),id)||!item->TryGetStringField(TEXT("Path"),path)||!item->TryGetNumberField(TEXT("Width"),width)||!item->TryGetNumberField(TEXT("Height"),height)){continue;}
  const FString full=FPaths::ConvertRelativePathToFull(root/path);if(!FPaths::IsUnderDirectory(full,root)||width<1||height<1||width>512||height>512){continue;}
  const int64 bytes=IFileManager::Get().FileSize(*full);if(bytes<1||bytes>4*1024*1024){continue;}auto* texture=FImageUtils::ImportFileAsTexture2D(full);if(texture&&texture->GetSizeX()==width&&texture->GetSizeY()==height){UiTextures.Add(id,texture);}
 }
 if(auto* logo=UiTexture(TEXT("logo-lobo-v01"))){HubLogo=logo;}
}
void AProbeArenaHUD::DrawTutorialLesson(){
 TArray<FString> lines;for(const auto& line:Tutorial->lesson()){lines.Add(UTF8_TO_TCHAR(line.c_str()));}if(Tutorial->finished()){lines.Add(TutorialSaveStatus);}
 const int32 action=DrawInfoPanel(TEXT("A BOLA PERDIDA — TUTORIAL"),lines,HubPanelPage,false,TEXT(""),TEXT("Continuar"));
 if(action==1){Tutorial->acknowledge();HubPanelPage=0;Pointer.cancel_until_release();NextAutomatic=GetWorld()->GetTimeSeconds()+0.5;}
}
void AProbeArenaHUD::DrawClassicSearch(){
 using namespace rpg::presentation;const auto state=Search.state();
 DrawRect(FLinearColor(0.015f,0.025f,0.06f),0,0,Canvas->SizeX,Canvas->SizeY);
 const float x=24,y=80,w=FMath::Max(48.f,Canvas->SizeX-48);
 DrawText(TEXT("LAB - EXP | A BOLA PERDIDA"),FColor::Cyan,x,24,HubLabelFont);
 DrawText(TEXT("Evento classico de teste — objetivo de busca; sem combate ou recompensas."),FColor::White,x,52,HubBodyFont,0.8f);
 const auto bar=[&](float top,int value,const FString& name,FLinearColor color){DrawRect(FLinearColor(0.05f,0.07f,0.12f),x,top,w,24);DrawRect(color,x,top,w*value/100.f,24);DrawText(FString::Printf(TEXT("%s: %d%%"),*name,value),FColor::White,x+4,top+2,HubBodyFont);};
 bar(y,state.search,TEXT("BUSCA"),FLinearColor(0.05f,0.6f,0.7f));bar(y+44,state.storm,TEXT("TEMPESTADE"),FLinearColor(0.6f,0.12f,0.2f));
 DrawText(FString::Printf(TEXT("Acoes: %u/200 | Concentracao: %s"),state.actions,state.concentrated?TEXT("pronta (+25)"):TEXT("inativa")),FColor::White,x,y+82,HubBodyFont);
 const auto button=[&](int row,const FString& label){Rect rect{x,y+125+row*58,w,48};const bool hover=contains(rect,Input.x,Input.y);DrawRect(hover?FLinearColor(0.1f,0.3f,0.4f):HubPanel,rect.x,rect.y,rect.width,rect.height);float tw=0,th=0;GetTextSize(label,tw,th,HubBodyFont);DrawText(label,FColor::White,x+10,rect.y+10,HubBodyFont,FMath::Min(1.f,(w-20)/FMath::Max(1.f,tw)));return pointer_clicked(Input,rect);};
 if(state.dialogue){
  TArray<FString> text={TEXT("METADE DO CAMINHO"),TEXT("Um homem percebe uma presenca luminosa. O jovem Lobo alerta sobre a tempestade e brinca sobre construir um barco."),TEXT("Busca e tempestade ficam pausadas neste dialogo.")};
  if(DrawInfoPanel(TEXT("Encontro na Terra"),text,HubPanelPage,false,TEXT(""),TEXT("Continuar"))==1){Search.acknowledge();HubPanelPage=0;Pointer.cancel_until_release();}return;
 }
 if(state.outcome==SearchOutcome::Running){
  if(button(0,TEXT("Voo de reconhecimento — busca +25"))){Search.act(state.revision,SearchAction::Reconnaissance);if(Search.state().outcome==SearchOutcome::Recovered){RecordTutorialCompletion(true);}Pointer.cancel_until_release();return;}
  if(button(1,TEXT("Concentrar sentidos — proxima busca +25"))){Search.act(state.revision,SearchAction::Concentrate);Pointer.cancel_until_release();return;}
  if(button(2,TEXT("Avancar pela tempestade — tempestade -20"))){Search.act(state.revision,SearchAction::Advance);Pointer.cancel_until_release();return;}
  DrawText(TEXT("Toda acao avanca a tempestade em15. Concentracao nao acumula."),FColor::White,x,y+305,HubBodyFont,0.75f);
  if(button(4,TEXT("Abandonar e voltar ao Lab - Exp"))){Search.abandon();SearchActive=false;Pointer.cancel_until_release();}return;
 }
 DrawText(state.outcome==SearchOutcome::Recovered?TEXT("BOLA RECUPERADA! O Lobo retorna as nuvens. A chuva durou45dias."):TEXT("A tempestade interrompeu a busca. Tente novamente."),FColor::Cyan,x,y+118,HubBodyFont,0.8f);
 DrawText(TutorialSaveStatus,FColor::White,x,y+164,HubBodyFont,0.7f);
 if(button(2,TEXT("Nova tentativa"))){Search.reset();Pointer.cancel_until_release();return;}
 if(state.outcome==SearchOutcome::Recovered && button(4,TEXT("Salvar conclusao novamente"))){RecordTutorialCompletion(true);Pointer.cancel_until_release();return;}
 if(button(3,TEXT("Voltar ao Lab - Exp"))){SearchActive=false;Pointer.cancel_until_release();}
}
