#include "rpg/text_session.hpp"
#include <exception>
#include <iostream>

int main(){
    try{const auto result=rpg::presentation::run_text_session(std::cin,std::cout);
        return result.exit==rpg::presentation::TextExit::AutomaticSafetyStop?2:0;
    }catch(const std::exception&){std::cerr<<"Nao foi possivel iniciar a sessao textual.\n";return 1;}
}
