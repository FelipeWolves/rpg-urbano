#include "rpg/text_session.hpp"
#include <exception>
#include <iostream>
#include <string_view>
#include <stdexcept>

int main(int argc,char** argv){
    const bool selected=argc>=3 && std::string_view{argv[1]}=="--equipe";
    if(argc!=1 && !(argc==2 && std::string_view{argv[1]}=="--classico") && !selected){
        std::cerr<<"Uso: jogar.exe [--classico | --equipe A1 A2 A3]\n";return 1;}
    try{
        auto run=[&](){
            if(selected){std::vector<std::string> ids;for(int i=2;i<argc;++i){ids.emplace_back(argv[i]);}
                return rpg::presentation::run_selected_team_reference_session(std::cin,std::cout,std::move(ids));}
            if(argc==2){return rpg::presentation::run_text_session(std::cin,std::cout);}
            return rpg::presentation::run_team_reference_session(std::cin,std::cout);
        };
        const auto result=run();
        return result.exit==rpg::presentation::TextExit::AutomaticSafetyStop?2:0;
    }catch(const std::invalid_argument& e){std::cerr<<"Equipe/configuracao invalida: "<<e.what()<<'\n';return 1;
    }catch(const std::exception&){std::cerr<<"Nao foi possivel iniciar a sessao textual.\n";return 1;}
}
