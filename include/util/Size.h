#pragma once
#include <cstdint>

struct Size {
    constexpr Size() { }
    constexpr Size(uint32_t width, uint32_t height)
        : width(width)
        , height(height)
    {
    }
    uint32_t width {};
    uint32_t height {};
};