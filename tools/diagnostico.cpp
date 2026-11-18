#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <algorithm>
#include <array>
#include <concepts>
#include <iostream>
#include <numeric>
#include <span>

#ifndef _WIN64
#error This diagnostic must be built for Windows x64.
#endif

static_assert(sizeof(void*) == 8);
static_assert(__cplusplus >= 202002L);

template <std::integral T>
constexpr T sum(std::span<const T> values) {
    return std::accumulate(values.begin(), values.end(), T{});
}

int main() {
    std::array values{7, 2, 5, 3};
    std::ranges::sort(values);
    constexpr std::array expected{2, 3, 5, 7};
    if (values != expected || sum<int>(values) != 17) {
        std::cerr << "TOOLCHAIN_FAIL standard_library\n";
        return 1;
    }

    SYSTEM_INFO system{};
    GetNativeSystemInfo(&system);
    if (system.dwNumberOfProcessors == 0) {
        std::cerr << "TOOLCHAIN_FAIL windows_sdk_call\n";
        return 2;
    }

    std::cout << "TOOLCHAIN_OK cxx=" << __cplusplus
              << " msvc=" << _MSC_FULL_VER
              << " pointer_bits=" << sizeof(void*) * 8
              << " sum=" << sum<int>(values) << '\n';
    return 0;
}
