#pragma once
#include <cstdint>

namespace U {

    enum class PlatformType {
        CTR,
        WIN64
    };

    class Platform {
    public:
        Platform() noexcept;

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