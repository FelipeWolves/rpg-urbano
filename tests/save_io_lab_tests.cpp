// QA-only Windows filesystem harness. Never used by the game's save adapter.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include "rpg/visual_session.hpp"
#include "rpg/startup_presentation.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace rpg;using namespace rpg::presentation;
namespace fs=std::filesystem;
void check(bool ok,const char* why){if(!ok){throw std::runtime_error(why);}}
void put(const fs::path& path,std::string_view bytes){std::ofstream f(path,std::ios::binary|std::ios::trunc);f.write(bytes.data(),static_cast<std::streamsize>(bytes.size()));f.close();check(!f.fail(),"QA write failed");}
std::string get(const fs::path& path){std::ifstream f(path,std::ios::binary);check(f.good(),"QA read failed");return {std::istreambuf_iterator<char>(f),{}};}
bool replace(const fs::path& file,const fs::path& pending){return MoveFileExW(pending.c_str(),file.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)!=0;}
struct Locked {HANDLE handle;explicit Locked(const fs::path& p):handle(CreateFileW(p.c_str(),GENERIC_READ,0,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr)){check(handle!=INVALID_HANDLE_VALUE,"QA exclusive lock failed");}~Locked(){CloseHandle(handle);}Locked(const Locked&)=delete;Locked& operator=(const Locked&)=delete;};
template<class F>void rejected(F f){bool caught=false;try{f();}catch(const std::exception&){caught=true;}check(caught,"Expected codec rejection");}
int wmain(int argc,wchar_t** argv){try{
 check(argc==2,"QA root required");const fs::path root=fs::absolute(argv[1]);
 check(root.filename().wstring().starts_with(L"run-")&&root.parent_path().filename()==L"qa-save-io"&&fs::is_empty(root),"Fresh QA root required");
 // All file names below are fixed children of the verified, newly created directory.
 const auto file=root/"progress.save",pending=root/"progress.pending";
 VisualSession original;check(original.select_team({"A2"})==SelectionError::None,"Select fixture team");const auto old=original.export_save();
 put(pending,old);check(replace(file,pending),"Initial commit");VisualSession loaded;loaded.import_save(get(file));check(loaded.team().selected==original.team().selected,"Disk roundtrip");
 std::cout<<"CASE,IO-01,progress disk roundtrip\n";
 VisualSession newer;check(newer.select_team({"A3","A1"})==SelectionError::None,"Replacement team");const auto fresh=newer.export_save();put(pending,fresh);check(replace(file,pending)&&!fs::exists(pending)&&get(file)==fresh,"Replace commit");
 std::cout<<"CASE,IO-02,replace existing file\n";
 const auto before=loaded.state();const auto beforeBytes=loaded.export_save();const auto unchanged=[&]{const auto current=loaded.state();return current.revision==before.revision&&current.combat==before.combat&&current.progress==before.progress&&loaded.export_save()==beforeBytes;};put(root/"corrupt.save","broken");rejected([&]{loaded.import_save(get(root/"corrupt.save"));});check(unchanged(),"Corruption must preserve memory");
 std::cout<<"CASE,IO-03,corrupt import preserves memory\n";
 put(root/"truncated.save",old.substr(0,old.size()/2));put(root/"oversize.save",std::string(max_save_bytes+1,'x'));rejected([&]{loaded.import_save(get(root/"truncated.save"));});rejected([&]{loaded.import_save(get(root/"oversize.save"));});check(unchanged(),"Invalid lengths preserve memory");
 std::cout<<"CASE,IO-04,truncated and oversized rejected\n";
 PresentationPreferences prefs;prefs.text_percent=150;prefs.reduced_motion=true;const auto encoded=encode_preferences(prefs);
 const auto primary=root/"preferences.txt",backup=root/"preferences.txt.bak";put(primary,encoded);fs::copy_file(primary,backup);put(primary,"invalid");const auto p=get(primary),b=get(backup);auto read=read_preferences(p,b);check(read.source==PreferenceSource::Backup&&read.primary_invalid&&read.values==prefs,"Backup recovery");
 std::cout<<"CASE,IO-05,preferences recover backup\n";
 put(backup,"invalid too");const auto invalid=get(backup);read=read_preferences(p,invalid);check(read.source==PreferenceSource::Defaults&&read.primary_invalid&&read.values==PresentationPreferences{},"Fallback defaults");
 std::cout<<"CASE,IO-06,both invalid use defaults\n";
 put(pending,old);{Locked lock(file);check(!replace(file,pending),"Exclusive target must fail replacement");}check(get(file)==fresh&&get(pending)==old,"Failed commit preserves target and pending");
 std::cout<<"CASE,IO-07,locked target preserves committed file\n";
 put(pending,"interrupted write");VisualSession restart;restart.import_save(get(file));check(restart.team().selected==newer.team().selected&&get(pending)=="interrupted write","Ignore orphan temporary");check(!fs::exists(root/"progress.save.bak"),"No invented progress backup");
 std::cout<<"CASE,IO-08,pending ignored no automatic progress backup\n";
 put(primary,encoded);put(backup,encoded);{Locked lock(backup);std::error_code error;const bool copied=fs::copy_file(primary,backup,fs::copy_options::overwrite_existing,error);check(!copied&&static_cast<bool>(error),"Backup write must fail under lock");}check(get(primary)==encoded&&get(backup)==encoded,"Failed backup preserves primary");
 std::cout<<"CASE,IO-09,backup copy failure\n";
 const auto parentBefore=root.parent_path();check(!fs::exists(root/"missing.save"),"Missing fixture");rejected([&]{(void)get(root/"missing.save");});check(root.parent_path()==parentBefore&&get(file)==fresh,"Missing read preserves committed file");
 std::cout<<"CASE,IO-10,missing file handled\nQA_SAVE_IO_RESULT passed=10 total=10\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
