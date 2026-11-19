#include "rpg/save_game.hpp"
#include <fstream>
#include <iostream>
#include <locale>
#include <stdexcept>
#include <utility>
namespace {
using namespace rpg;
void check(bool value,const char* message){if(!value){throw std::runtime_error(message);}}
void golden_v1(){
    const std::string golden="RPG_URBANO_SAVE 1\nprofile team\nattempt 3\ncredits 10\nteam 2 A3 A1\ncompletions 3\n1 A 0 1 10\n2 A 1 200 0\n3 none 2 1 0\nend\n";
    const auto data=decode_save(golden);
    check(encode_save(data)==golden && data.selected.front()=="A3" &&
        data.progress.completions[0].reason==EndReason::Elimination &&
        data.progress.completions[1].reason==EndReason::ActionLimit &&
        data.progress.completions[2].reason==EndReason::TechnicalLimit,"v1 wire codes or selection order changed");
}
class Grouping:public std::numpunct<char>{
    char do_thousands_sep()const override{return '_';}
    std::string do_grouping()const override{return "\3";}
};
struct LocaleGuard {std::locale old=std::locale();~LocaleGuard(){std::locale::global(old);}};
void locale_independence(){
    SavedGame data;data.selected={"A1"};data.progress={1000,10,{{1,Team::A,EndReason::Elimination,1000,10}}};
    const auto expected=encode_save(data);LocaleGuard guard;
    std::locale::global(std::locale(std::locale::classic(),new Grouping));
    check(encode_save(data)==expected && decode_save(expected)==data && expected.find("1_000")==std::string::npos,
        "global locale changed save numbers");
}
}
int main(int argc,char** argv){
    const std::pair<const char*,void(*)()> tests[]={{"golden_v1",golden_v1},{"locale_independence",locale_independence}};
    int passed=0;for(const auto& [name,test]:tests){try{test();++passed;std::cout<<"PASS codec "<<name<<'\n';}
        catch(const std::exception& e){std::cerr<<"FAIL codec "<<name<<": "<<e.what()<<'\n';}}
    std::cout<<"CODEC_TESTS_RESULT passed="<<passed<<" total="<<std::size(tests)<<'\n';
    if(argc==2 && passed==static_cast<int>(std::size(tests))){std::ofstream out(argv[1]);out<<"test,status,scope\n";
        for(const auto& [name,test]:tests){static_cast<void>(test);out<<name<<",pass,codec_without_platform_storage\n";}if(!out){return 2;}}
    return passed==static_cast<int>(std::size(tests))?0:1;
}
