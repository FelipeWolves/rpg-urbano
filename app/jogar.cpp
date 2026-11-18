#define NOMINMAX
#include <windows.h>
#include "rpg/text_session.hpp"
#include <exception>
#include <iostream>
#include <string_view>
#include <stdexcept>
namespace {
std::filesystem::path slot(bool classic){
    std::wstring path(32768,L'\0');const auto length=GetModuleFileNameW(nullptr,path.data(),static_cast<DWORD>(path.size()));
    if(length==0 || length>=path.size()){throw rpg::SaveError("Nao foi possivel localizar o diretorio do programa.");}
    path.resize(length);return std::filesystem::path(path).parent_path().parent_path().parent_path()/"build"/"saves"/
        (classic?"classico.save":"equipes.save");
}
}
int main(int argc,char** argv){
    const bool selected=argc>=3 && std::string_view{argv[1]}=="--equipe";
    const bool classic=argc>=2 && std::string_view{argv[1]}=="--classico";
    const bool resume=(argc==2 && std::string_view{argv[1]}=="--continuar") ||
        (argc==3 && classic && std::string_view{argv[2]}=="--continuar");
    if(!(argc==1 || (argc==2 && classic) || selected || resume)){
        std::cerr<<"Uso: jogar.exe [--classico [--continuar] | --continuar | --equipe A1 A2 A3]\n";return 1;}
    try{
        rpg::presentation::TextSessionOptions options;options.save_path=slot(classic);
        if(!resume && std::filesystem::exists(*options.save_path)){
            std::cout<<"Nova sessao: ja existe save neste perfil. Use --continuar para retomar; salvar substitui o progresso anterior.\n";
        }
        std::optional<rpg::SavedGame> saved;
        if(resume){saved=rpg::load_save(*options.save_path);
            if(saved->profile!=(classic?rpg::CombatProfile::Classic:rpg::CombatProfile::Team)){throw rpg::SaveError("Perfil do save incompativel.");}
            options.restored=saved->progress;
            if(classic && saved->selected!=std::vector<std::string>{"A1","A2","A3"}){throw rpg::SaveError("Equipe classica do save incompativel.");}
        }
        auto run=[&](){
            if(classic){return rpg::presentation::run_text_session(std::cin,std::cout,rpg::reference_roster(),
                rpg::presentation::text_reference_rules(),rpg::CombatProfile::Classic,{},options);}
            if(saved){return rpg::presentation::run_selected_team_reference_session(std::cin,std::cout,saved->selected,options);}
            if(selected){std::vector<std::string> ids;for(int i=2;i<argc;++i){ids.emplace_back(argv[i]);}
                return rpg::presentation::run_selected_team_reference_session(std::cin,std::cout,std::move(ids),options);}
            return rpg::presentation::run_team_reference_session(std::cin,std::cout,options);
        };
        const auto result=run();return result.exit==rpg::presentation::TextExit::AutomaticSafetyStop?2:0;
    }catch(const rpg::SaveError& e){std::cerr<<e.what()<<'\n';return 1;
    }catch(const std::invalid_argument& e){std::cerr<<"Equipe/configuracao invalida: "<<e.what()<<'\n';return 1;
    }catch(const std::exception&){std::cerr<<"Nao foi possivel iniciar a sessao textual.\n";return 1;}
}
