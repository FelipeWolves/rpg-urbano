#pragma once
#include "rpg/local_session.hpp"
#include <filesystem>
#include <stdexcept>
namespace rpg {
class SaveError:public std::runtime_error {public:using std::runtime_error::runtime_error;};
struct SavedGame {
    std::uint32_t version=1;
    CombatProfile profile=CombatProfile::Team;
    std::vector<std::string> selected;
    LocalProgress progress;
    bool operator==(const SavedGame&) const = default;
};
[[nodiscard]] std::string encode_save(const SavedGame& game);
[[nodiscard]] SavedGame decode_save(std::string_view data);
[[nodiscard]] SavedGame load_save(const std::filesystem::path& path);
void write_save(const std::filesystem::path& path,const SavedGame& game);
} // namespace rpg
