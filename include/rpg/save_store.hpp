#pragma once
#include "rpg/save_game.hpp"
#include <filesystem>
namespace rpg {
// Implemented by a platform adapter. The save codec itself does not access files.
[[nodiscard]] SavedGame load_save(const std::filesystem::path& path);
void write_save(const std::filesystem::path& path,const SavedGame& game);
} // namespace rpg
