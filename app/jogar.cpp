#include "rpg/text_session.hpp"
#include <exception>
#include <iostream>
#include <string_view>

int main(int argc,char** argv){
    if(argc>2 || (argc==2 && std::string_view{argv[1]}!="--classico")){
        std::cerr<<"Uso: jogar.exe [--classico]\n";return 1;}
    try{const auto result=argc==2?rpg::presentation::run_text_session(std::cin,std::cout):
        rpg::presentation::run_team_reference_session(std::cin,std::cout);
        return result.exit==rpg::presentation::TextExit::AutomaticSafetyStop?2:0;
    }catch(const std::exception&){std::cerr<<"Nao foi possivel iniciar a sessao textual.\n";return 1;}
}
