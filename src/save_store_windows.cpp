#define NOMINMAX
#include <windows.h>
#include "rpg/save_store.hpp"
#include <fstream>

namespace rpg {
SavedGame load_save(const std::filesystem::path& path){
    std::ifstream file(path,std::ios::binary);if(!file){throw SaveError("Save ausente ou ilegivel.");}
    std::string bytes(max_save_bytes+1,'\0');file.read(bytes.data(),static_cast<std::streamsize>(bytes.size()));
    const auto size=file.gcount();if(file.bad() || size>static_cast<std::streamsize>(max_save_bytes)){throw SaveError("Save ilegivel ou muito grande.");}
    bytes.resize(static_cast<std::size_t>(size));return decode_save(bytes);
}
void write_save(const std::filesystem::path& path,const SavedGame& game){
    const auto bytes=encode_save(game);
    // Refuse silent replacement of corrupt/unknown-version files.
    if(std::filesystem::exists(path)){static_cast<void>(load_save(path));}
    if(!path.parent_path().empty()){std::filesystem::create_directories(path.parent_path());}
    auto pending=path;pending+=L".pending";
    const auto handle=CreateFileW(pending.c_str(),GENERIC_WRITE,0,nullptr,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,nullptr);
    if(handle==INVALID_HANDLE_VALUE){throw SaveError("Nao foi possivel criar o save temporario; arquivo anterior preservado.");}
    DWORD written=0;
    bool ok=WriteFile(handle,bytes.data(),static_cast<DWORD>(bytes.size()),&written,nullptr)!=0 && written==bytes.size();
    if(ok){ok=FlushFileBuffers(handle)!=0;}
    const bool closed=CloseHandle(handle)!=0;ok=ok && closed;
    if(ok){ok=MoveFileExW(pending.c_str(),path.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)!=0;}
    if(!ok){DeleteFileW(pending.c_str());throw SaveError("Falha ao gravar; save anterior preservado.");}
}
} // namespace rpg
