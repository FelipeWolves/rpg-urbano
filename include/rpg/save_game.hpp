#pragma once
#include "rpg/local_session.hpp"
#include <cstddef>
#include <stdexcept>
namespace rpg {
inline constexpr std::size_t max_save_bytes=1024*1024;
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
} // namespace rpg
