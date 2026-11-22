// Shared production orchestration; Windows QA backend never uses normal Saved paths.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include "rpg/visual_session.hpp"
#include "rpg/startup_presentation.hpp"
#include "rpg/persistence.hpp"
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
struct DiskFiles final:storage::Files {
 fs::path root;bool fail_write=false,fail_replace=false,fail_copy=false,fail_directory=false;
 explicit DiskFiles(fs::path path):root(std::move(path)){}
 bool ensure_directory()override{return !fail_directory&&fs::is_directory(root);}
 storage::ReadResult read(std::string_view key,std::size_t limit)override{
  const auto path=root/std::string(key);std::error_code error;if(!fs::exists(path,error)){return {error?storage::ReadState::Failed:storage::ReadState::Missing,{}};}
  const auto size=fs::file_size(path,error);if(error||size>limit){return {storage::ReadState::Failed,{}};}
  try{return {storage::ReadState::Ready,get(path)};}catch(const std::exception&){return {storage::ReadState::Failed,{}};}
 }
 bool copy(std::string_view from,std::string_view to)override{if(fail_copy){return false;}std::error_code error;return fs::copy_file(root/std::string(from),root/std::string(to),fs::copy_options::overwrite_existing,error);}
 bool write(std::string_view key,std::string_view bytes)override{try{put(root/std::string(key),fail_write?bytes.substr(0,bytes.size()/2):bytes);return !fail_write;}catch(const std::exception&){return false;}}
 bool replace(std::string_view target,std::string_view temporary)override{return !fail_replace&&::replace(root/std::string(target),root/std::string(temporary));}
 void discard(std::string_view key)override{std::error_code error;fs::remove(root/std::string(key),error);}
};struct Locked {HANDLE handle;explicit Locked(const fs::path& p):handle(CreateFileW(p.c_str(),GENERIC_READ,0,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr)){check(handle!=INVALID_HANDLE_VALUE,"QA exclusive lock failed");}~Locked(){CloseHandle(handle);}Locked(const Locked&)=delete;Locked& operator=(const Locked&)=delete;};
template<class F>void rejected(F f){bool caught=false;try{f();}catch(const std::exception&){caught=true;}check(caught,"Expected codec rejection");}
int wmain(int argc,wchar_t** argv){try{
 check(argc==2,"QA root required");const fs::path root=fs::absolute(argv[1]);
 check(root.filename().wstring().starts_with(L"run-")&&root.parent_path().filename()==L"qa-save-io"&&fs::is_empty(root),"Fresh QA root required");
 // All file names below are fixed children of the verified, newly created directory.
 const auto file=root/"progress.save",pending=root/"progress.pending";DiskFiles files(root);const storage::Keys progress{"progress.save","progress.pending",{},{}},preferences{"preferences.txt","preferences.tmp","preferences.txt.bak","preferences.invalid"};
 VisualSession original;check(original.select_team({"A2"})==SelectionError::None,"Select fixture team");const auto old=original.export_save();
 storage::save_progress(files,progress,old);VisualSession loaded;loaded.import_save(storage::load_progress(files,progress));check(loaded.team().selected==original.team().selected,"Disk roundtrip");
 std::cout<<"CASE,IO-01,progress disk roundtrip\n";
 VisualSession newer;check(newer.select_team({"A3","A1"})==SelectionError::None,"Replacement team");const auto fresh=newer.export_save();storage::save_progress(files,progress,fresh);check(!fs::exists(pending)&&get(file)==fresh,"Replace commit");
 std::cout<<"CASE,IO-02,replace existing file\n";
 const auto before=loaded.state();const auto beforeBytes=loaded.export_save();const auto unchanged=[&]{const auto current=loaded.state();return current.revision==before.revision&&current.combat==before.combat&&current.progress==before.progress&&loaded.export_save()==beforeBytes;};put(root/"corrupt.save","broken");rejected([&]{loaded.import_save(storage::load_progress(files,{"corrupt.save","corrupt.tmp",{},{}}));});check(unchanged(),"Corruption must preserve memory");
 std::cout<<"CASE,IO-03,corrupt import preserves memory\n";
 put(root/"truncated.save",old.substr(0,old.size()/2));put(root/"oversize.save",std::string(max_save_bytes+1,'x'));rejected([&]{loaded.import_save(storage::load_progress(files,{"truncated.save","truncated.tmp",{},{}}));});rejected([&]{loaded.import_save(storage::load_progress(files,{"oversize.save","oversize.tmp",{},{}}));});check(unchanged(),"Invalid lengths preserve memory");
 std::cout<<"CASE,IO-04,truncated and oversized rejected\n";
 PresentationPreferences prefs;prefs.text_percent=150;prefs.reduced_motion=true;const auto encoded=encode_preferences(prefs);
 const auto primary=root/"preferences.txt",backup=root/"preferences.txt.bak";storage::save_preferences(files,preferences,prefs);storage::save_preferences(files,preferences,prefs);put(primary,"invalid");auto read=storage::load_preferences(files,preferences);check(read.source==PreferenceSource::Backup&&read.primary_invalid&&read.values==prefs,"Backup recovery");
 std::cout<<"CASE,IO-05,preferences recover backup\n";
 put(backup,"invalid too");read=storage::load_preferences(files,preferences);check(read.source==PreferenceSource::Defaults&&read.primary_invalid&&read.values==PresentationPreferences{},"Fallback defaults");
 std::cout<<"CASE,IO-06,both invalid use defaults\n";
 {Locked lock(file);rejected([&]{storage::save_progress(files,progress,old);});}check(get(file)==fresh&&get(pending)==old,"Failed commit preserves target and pending");
 std::cout<<"CASE,IO-07,locked target preserves committed file\n";
 put(pending,"interrupted write");VisualSession restart;restart.import_save(storage::load_progress(files,progress));check(restart.team().selected==newer.team().selected&&get(pending)=="interrupted write","Ignore orphan temporary");check(!fs::exists(root/"progress.save.bak"),"No invented progress backup");
 std::cout<<"CASE,IO-08,pending ignored no automatic progress backup\n";
 put(primary,encoded);put(backup,encoded);{Locked lock(backup);rejected([&]{storage::save_preferences(files,preferences,PresentationPreferences{});});}check(get(primary)==encoded&&get(backup)==encoded,"Failed backup preserves primary");
 std::cout<<"CASE,IO-09,backup copy failure\n";
 const auto parentBefore=root.parent_path();check(!fs::exists(root/"missing.save"),"Missing fixture");rejected([&]{(void)storage::load_progress(files,{"missing.save","missing.tmp",{},{}});});check(root.parent_path()==parentBefore&&get(file)==fresh,"Missing read preserves committed file");
 std::cout<<"CASE,IO-10,missing file handled\n";
 files.fail_write=true;rejected([&]{storage::save_progress(files,progress,old);});check(get(file)==fresh,"Partial progress write preserves primary");rejected([&]{storage::save_preferences(files,preferences,prefs);});check(!fs::exists(root/preferences.temporary)&&get(primary)==encoded,"Preference partial cleanup");files.fail_write=false;
 std::cout<<"CASE,IO-11,partial write failure shared orchestration\n";
 files.fail_replace=true;rejected([&]{storage::save_preferences(files,preferences,PresentationPreferences{});});check(get(primary)==encoded&&!fs::exists(root/preferences.temporary),"Preference commit failure cleanup");files.fail_replace=false;
 std::cout<<"CASE,IO-12,replacement failure shared orchestration\n";
 put(primary,"invalid primary");put(backup,encoded);storage::save_preferences(files,preferences,PresentationPreferences{});check(get(root/preferences.invalid)=="invalid primary"&&get(backup)==encoded&&decode_preferences(get(primary))==PresentationPreferences{},"Quarantine must not overwrite valid backup");
 std::cout<<"CASE,IO-13,invalid primary quarantined backup preserved\n";
 const auto saved=get(file);files.fail_directory=true;rejected([&]{storage::save_progress(files,progress,old);});files.fail_directory=false;check(get(file)==saved,"Directory failure preserves primary");
 std::cout<<"CASE,IO-14,directory failure shared orchestration\n";
 for(const auto& keys: {storage::Keys{"../escape","x",{},{}},storage::Keys{"same","same",{},{}},storage::Keys{"same","SAME",{},{}},storage::Keys{"x","C:escape",{}, {}}}){rejected([&]{storage::save_progress(files,keys,old);});}check(get(file)==saved&&!fs::exists(root.parent_path()/"escape"),"Invalid keys rejected before IO");
 std::cout<<"CASE,IO-15,path and duplicate key guards\n";
 put(primary,std::string(4097,'x'));const auto big=get(primary);rejected([&]{storage::save_preferences(files,preferences,prefs);});check(get(primary)==big&&get(backup)==encoded,"Oversize previous preference preserved");
 std::cout<<"CASE,IO-16,oversize previous preference preserved\nQA_SAVE_IO_RESULT passed=16 total=16\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
