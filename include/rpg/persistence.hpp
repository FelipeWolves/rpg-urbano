#pragma once
#include "rpg/save_game.hpp"
#include "rpg/startup_presentation.hpp"
#include <algorithm>
#include <string_view>
namespace rpg::storage {
enum class ReadState { Missing, Ready, Failed };
struct ReadResult {ReadState state=ReadState::Missing;std::string bytes;};
// Backends are rooted by their owner. Keys are single file names, never paths.
struct Files {
 virtual ~Files()=default;
 virtual bool ensure_directory()=0;
 virtual ReadResult read(std::string_view key,std::size_t limit)=0;
 virtual bool copy(std::string_view source,std::string_view destination)=0;
 virtual bool write(std::string_view key,std::string_view bytes)=0;
 virtual bool replace(std::string_view target,std::string_view temporary)=0;
 virtual void discard(std::string_view key)=0;
};
struct Keys {std::string primary,temporary,backup,invalid;};
inline void validate_keys(const Keys& keys,bool preferences){
 const std::string_view names[]={keys.primary,keys.temporary,keys.backup,keys.invalid};
 const std::size_t count=preferences?4:2;
 for(std::size_t i=0;i<count;++i){const auto key=names[i];
  if(key.empty()||key.size()>160||key=="."||key==".."||!std::ranges::all_of(key,[](unsigned char c){return (c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='.'||c=='_'||c=='-';})){throw SaveError("Nome de arquivo de persistencia invalido.");}
  for(std::size_t j=0;j<i;++j){if(std::ranges::equal(key,names[j],[](unsigned char a,unsigned char b){const auto lower=[](unsigned char c){return c>=65&&c<=90?c+32:c;};return lower(a)==lower(b);})){throw SaveError("Arquivos de persistencia devem ser distintos.");}}
 }
}
inline ReadResult bounded_read(Files& files,std::string_view key,std::size_t limit){
 auto result=files.read(key,limit);
 if(result.state==ReadState::Ready && (result.bytes.empty()||result.bytes.size()>limit)){return {ReadState::Failed,{}};}
 return result;
}
inline void save_progress(Files& files,const Keys& keys,std::string_view bytes){
 validate_keys(keys,false);(void)decode_save(bytes);
 if(!files.ensure_directory()){throw SaveError("Nao foi possivel criar a pasta de progresso.");}
 // Existing policy: no backup and no promotion of orphan temporary files.
 if(!files.write(keys.temporary,bytes)||!files.replace(keys.primary,keys.temporary)){
  throw SaveError("Falha ao salvar; progresso em memoria preservado. Verifique permissao/espaco.");
 }
}
inline std::string load_progress(Files& files,const Keys& keys){
 validate_keys(keys,false);auto result=bounded_read(files,keys.primary,max_save_bytes);
 if(result.state!=ReadState::Ready){throw SaveError("Save ausente, vazio, ilegivel ou acima de 1 MiB.");}
 (void)decode_save(result.bytes);return result.bytes;
}
inline void save_preferences(Files& files,const Keys& keys,const presentation::PresentationPreferences& values){
 validate_keys(keys,true);const auto encoded=presentation::encode_preferences(values);
 if(!files.ensure_directory()){throw SaveError("Sem acesso para salvar; alteracao so nesta sessao.");}
 const auto previous=files.read(keys.primary,4096);
 if(previous.state==ReadState::Failed || previous.bytes.size()>4096){throw SaveError("Arquivo anterior preservado; alteracao so nesta sessao.");}
 if(previous.state==ReadState::Ready){
  bool valid=true;try{(void)presentation::decode_preferences(previous.bytes);}catch(const std::invalid_argument&){valid=false;}
  if(!files.copy(keys.primary,valid?keys.backup:keys.invalid)){throw SaveError("Falha ao preservar copia anterior; nao salvo.");}
 }
 if(!files.write(keys.temporary,encoded)||!files.replace(keys.primary,keys.temporary)){
  files.discard(keys.temporary);throw SaveError("Falha de gravacao; alteracao so nesta sessao.");
 }
}
inline presentation::PreferenceRead load_preferences(Files& files,const Keys& keys,presentation::PresentationPreferences defaults={}){
 validate_keys(keys,true);const auto primary=bounded_read(files,keys.primary,4096),backup=bounded_read(files,keys.backup,4096);
 const auto input=[](const ReadResult& r)->std::optional<std::string_view>{if(r.state==ReadState::Missing){return {};}if(r.state==ReadState::Failed){return "invalid";}return r.bytes;};
 return presentation::read_preferences(input(primary),input(backup),defaults);
}
} // namespace rpg::storage
