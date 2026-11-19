#include "rpg/save_game.hpp"
#include <algorithm>
#include <charconv>
#include <locale>
#include <sstream>

namespace rpg {
namespace {
void validate(const SavedGame& game){
    if(game.version!=1 || (game.profile!=CombatProfile::Team && game.profile!=CombatProfile::Classic) ||
       game.selected.empty() || game.selected.size()>5){throw SaveError("Save: versao, perfil ou equipe invalidos.");}
    std::vector<std::string> seen;
    for(const auto& id:game.selected){
        if(id.empty() || id.size()>64 || !std::ranges::all_of(id,[](unsigned char c){
            return (c>='A' && c<='Z') || (c>='a' && c<='z') || (c>='0' && c<='9') || c=='_' || c=='-';}) ||
           std::ranges::find(seen,id)!=seen.end()){throw SaveError("Save: IDs de equipe invalidos.");}
        seen.push_back(id);
    }
    try{validate_progress(game.progress);}catch(const std::invalid_argument&){throw SaveError("Save: progresso inconsistente.");}
}
struct Reader {
    std::istringstream input;
    explicit Reader(std::string_view data):input(std::string{data}){input.imbue(std::locale::classic());}
    std::string token(){std::string text;if(!(input>>text)){throw SaveError("Save truncado.");}return text;}
    void expect(std::string_view value){if(token()!=value){throw SaveError("Save: campo inesperado.");}}
    std::uint64_t number(){const auto text=token();std::uint64_t value=0;
        const auto [end,error]=std::from_chars(text.data(),text.data()+text.size(),value);
        if(error!=std::errc{} || end!=text.data()+text.size()){throw SaveError("Save: numero invalido.");}return value;}
};
}
std::string encode_save(const SavedGame& game){
    validate(game);std::ostringstream out;out.imbue(std::locale::classic());
    out<<"RPG_URBANO_SAVE 1\nprofile "<<(game.profile==CombatProfile::Team?"team":"classic")
       <<"\nattempt "<<game.progress.attempt<<"\ncredits "<<game.progress.credits<<"\nteam "<<game.selected.size();
    for(const auto& id:game.selected){out<<' '<<id;}
    out<<"\ncompletions "<<game.progress.completions.size()<<'\n';
    for(const auto& c:game.progress.completions){out<<c.attempt<<' '<<(c.winner?(*c.winner==Team::A?"A":"B"):"none")
        <<' '<<(c.reason==EndReason::Elimination?0:c.reason==EndReason::ActionLimit?1:2)<<' '<<c.actions<<' '<<c.credits<<'\n';}
    out<<"end\n";return out.str();
}
SavedGame decode_save(std::string_view data){
    if(data.empty() || data.size()>max_save_bytes){throw SaveError("Save: tamanho invalido.");}
    Reader in(data);in.expect("RPG_URBANO_SAVE");if(in.number()!=1){throw SaveError("Save: versao nao suportada.");}
    SavedGame game;in.expect("profile");const auto profile=in.token();
    if(profile!="team" && profile!="classic"){throw SaveError("Save: perfil invalido.");}
    game.profile=profile=="team"?CombatProfile::Team:CombatProfile::Classic;
    in.expect("attempt");game.progress.attempt=in.number();in.expect("credits");game.progress.credits=in.number();
    in.expect("team");const auto members=in.number();if(members<1 || members>5){throw SaveError("Save: tamanho de equipe invalido.");}
    for(std::uint64_t i=0;i<members;++i){game.selected.push_back(in.token());}
    in.expect("completions");const auto count=in.number();if(count>4096){throw SaveError("Save: historico excedido.");}
    for(std::uint64_t i=0;i<count;++i){LocalCompletion c{};c.attempt=in.number();const auto winner=in.token();
        if(winner=="A"){c.winner=Team::A;}else if(winner=="B"){c.winner=Team::B;}else if(winner!="none"){throw SaveError("Save: vencedor invalido.");}
        const auto reason=in.number();if(reason>2){throw SaveError("Save: motivo invalido.");}c.reason=reason==0?EndReason::Elimination:reason==1?EndReason::ActionLimit:EndReason::TechnicalLimit;
        c.actions=in.number();c.credits=in.number();game.progress.completions.push_back(c);}
    in.expect("end");std::string extra;if(in.input>>extra){throw SaveError("Save: dados adicionais inesperados.");}
    validate(game);return game;
}
} // namespace rpg
