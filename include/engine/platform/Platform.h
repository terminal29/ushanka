#pragma once
#include <cstdint>

namespace U::Engine::Platform {

    enum class PlatformType {
        CTR,
        WIN64
    };

    class Platform {
    public:
        constexpr Platform() = default;

        /**
         * Gets enum type of this platform
        */
        constexpr PlatformType GetPlatformType();

        /**
         * Gets nice name for this platform
        */
        constexpr const char* GetPlatformName();
    };

}