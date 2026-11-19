#pragma once
namespace rpg_probe {
struct Result {
    bool passed=false;
    int checkpoints=0;
    int failures=0;
    int accepted=0;
    int credits=0;
};
// No Unreal types or OS I/O cross this boundary. Exceptions stay inside.
Result Run() noexcept;
}
