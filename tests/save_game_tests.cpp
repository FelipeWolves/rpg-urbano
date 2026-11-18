#define NOMINMAX
#include <windows.h>
#include "rpg/save_game.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>
namespace {
using namespace rpg;
std::filesystem::path directory;
void check(bool value,const char* message){if(!value){throw std::runtime_error(message);}}
template<class F>void rejected(F f){bool caught=false;try{f();}catch(const std::exception&){caught=true;}check(caught,"invalid save accepted");}
std::vector<Definition> duel(){return {{"A",Team::A,10,10,200},{"B",Team::B,10,10,100}};}
SavedGame example(){SavedGame d;d.profile=CombatProfile::Classic;d.selected={"A1","A3"};
    d.progress={3,10,{{1,Team::A,EndReason::Elimination,1,10},{3,std::nullopt,EndReason::ActionLimit,2,0}}};return d;}
std::string changed(std::string text,const std::string& from,const std::string& to){const auto at=text.find(from);
    check(at!=std::string::npos,"test marker missing");text.replace(at,from.size(),to);return text;}
void roundtrip(){const auto d=example();const auto encoded=encode_save(d);
    check(decode_save(encoded)==d && encode_save(decode_save(encoded))==encoded,"save roundtrip/canonical output differs");
    auto empty=d;empty.progress=LocalProgress{};empty.profile=CombatProfile::Team;check(decode_save(encode_save(empty))==empty,"fresh team roundtrip failed");}
void syntax_and_bounds(){const auto text=encode_save(example());
    const std::pair<std::string,std::string> cases[]={{"SAVE 1","SAVE 2"},{"profile classic","profile unknown"},
        {"attempt 3","attempt -3"},{"attempt 3","attempt +3"},{"attempt 3","attempt 18446744073709551616"},
        {"team 2","team 6"},{"completions 2","completions 4097"},{"1 A 0","1 X 0"},{"1 A 0","1 A 99"}};
    for(const auto& [from,to]:cases){rejected([&]{static_cast<void>(decode_save(changed(text,from,to)));});}
    for(std::size_t i=0;i<text.size()-4;++i){rejected([&]{static_cast<void>(decode_save(std::string_view{text}.substr(0,i)));});}
    rejected([&]{static_cast<void>(decode_save(text+"garbage"));});
    rejected([&]{static_cast<void>(decode_save(std::string(1024*1024+1,'x')));});
}
void semantic_validation(){
    auto d=example();d.progress.credits=20;rejected([&]{static_cast<void>(encode_save(d));});
    d=example();d.progress.completions[1].attempt=1;rejected([&]{static_cast<void>(encode_save(d));});
    d=example();d.progress.completions[0].credits=999;rejected([&]{static_cast<void>(encode_save(d));});
    d=example();d.progress.completions[0].actions=0;rejected([&]{static_cast<void>(encode_save(d));});
    d=example();d.progress.completions[0].winner.reset();rejected([&]{static_cast<void>(encode_save(d));});
    d=example();d.progress.completions[0].reason=EndReason::TechnicalLimit;rejected([&]{static_cast<void>(encode_save(d));});
    d=example();d.progress.attempt=std::numeric_limits<std::uint64_t>::max();rejected([&]{static_cast<void>(encode_save(d));});
    d=example();d.selected={"A1","A1"};rejected([&]{static_cast<void>(encode_save(d));});
    d=example();d.selected={"A 1"};rejected([&]{static_cast<void>(encode_save(d));});
}
void restore_once(){
    LocalSession first(duel(),{});check(first.apply({"A",ActionType::Attack,"B"}).accepted(),"save win setup failed");
    SavedGame saved{1,CombatProfile::Classic,{"A"},first.progress()};const auto disk=decode_save(encode_save(saved));
    LocalSession resumed(duel(),{});resumed.restore_progress(disk.progress);
    check(resumed.progress().attempt==2 && resumed.progress().credits==10 && !resumed.settle() &&
        resumed.battle().battle().accepted_actions()==0 && resumed.battle().battle().find("B")->health==10,"restore replayed result or battle state");
    const auto prior=resumed.progress();rejected([&]{resumed.restore_progress(disk.progress);});check(resumed.progress()==prior,"repeat restore mutated progress");
    check(resumed.apply({"A",ActionType::Attack,"B"}).accepted() && resumed.progress().credits==20 &&
        resumed.progress().completions[1].attempt==2,"new attempt failed after restore");
    LocalSession third(duel(),{});third.restore_progress(resumed.progress());check(third.progress().attempt==3 && third.progress().credits==20 &&
        !third.settle(),"second load duplicated settlement");
}
void rejected_restore_atomic(){
    LocalSession fresh(duel(),{});const auto old=fresh.progress();const auto battle=fresh.battle().snapshot();auto bad=example().progress;bad.credits=999;
    rejected([&]{fresh.restore_progress(bad);});check(fresh.progress()==old && fresh.battle().snapshot()==battle,"invalid restore mutated session");
    auto exhausted=LocalProgress{};exhausted.attempt=std::numeric_limits<std::uint64_t>::max()-1;
    rejected([&]{fresh.restore_progress(exhausted);});check(fresh.progress()==old,"exhausted ID restore mutated session");
    check(fresh.apply({"A",ActionType::Guard,{}}).accepted(),"live restore setup failed");const auto ongoing=fresh.battle().snapshot();
    rejected([&]{fresh.restore_progress(example().progress);});check(fresh.battle().snapshot()==ongoing,"load modified active battle");
}
void files_and_pending(){
    const auto path=directory/"normal.save";auto d=example();write_save(path,d);check(load_save(path)==d,"file roundtrip failed");
    d.progress.attempt=4;write_save(path,d);check(load_save(path)==d,"atomic replacement failed");
    auto bad=d;bad.progress.credits=999;rejected([&]{write_save(path,bad);});check(load_save(path)==d,"invalid data replaced prior save");
    auto pending=path;pending+=L".pending";{std::ofstream busy(pending);busy<<"occupied";}
    rejected([&]{write_save(path,d);});check(load_save(path)==d && std::filesystem::exists(pending),"pending collision changed existing files");
    const auto missing=directory/"absent.save";rejected([&]{static_cast<void>(load_save(missing));});check(!std::filesystem::exists(missing),"load created missing save");
}
void locked_and_unknown_file(){
    const auto path=directory/"locked.save";const auto d=example();write_save(path,d);
    const auto handle=CreateFileW(path.c_str(),GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);
    check(handle!=INVALID_HANDLE_VALUE,"file lock setup failed");bool caught=false;
    try{write_save(path,d);}catch(const std::exception&){caught=true;}CloseHandle(handle);
    check(caught && load_save(path)==d,"replacement failure damaged prior save");
    const auto future=directory/"future.save";const auto text=changed(encode_save(d),"SAVE 1","SAVE 99");
    {std::ofstream out(future,std::ios::binary);out<<text;}
    rejected([&]{write_save(future,d);});std::ifstream in(future,std::ios::binary);const std::string preserved((std::istreambuf_iterator<char>(in)),{});
    check(preserved==text,"unsupported version overwritten");
}
}
int main(int argc,char** argv){
    directory=std::filesystem::path(argc==2?argv[1]:"saves-testes.csv").parent_path()/
        ("save-tests-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::filesystem::create_directories(directory);
    const std::pair<const char*,void(*)()> tests[]={{"roundtrip",roundtrip},{"syntax_bounds",syntax_and_bounds},{"semantics",semantic_validation},
        {"restore_once",restore_once},{"restore_atomic",rejected_restore_atomic},{"files_pending",files_and_pending},{"locked_future",locked_and_unknown_file}};
    int passed=0;for(const auto& [name,test]:tests){try{test();++passed;std::cout<<"PASS save "<<name<<'\n';}
        catch(const std::exception& e){std::cerr<<"FAIL save "<<name<<": "<<e.what()<<'\n';}}
    std::cout<<"SAVE_TESTS_RESULT passed="<<passed<<" total="<<std::size(tests)<<'\n';
    if(argc==2 && passed==static_cast<int>(std::size(tests))){std::ofstream out(argv[1]);out<<"test,status,scope\n";
        for(const auto& [name,test]:tests){static_cast<void>(test);out<<name<<",pass,versioned_between_battle_progress_save\n";}if(!out){return 2;}}
    return passed==static_cast<int>(std::size(tests))?0:1;
}
