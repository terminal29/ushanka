#pragma once
#include <cstdint>

namespace U {

    enum class PlatformType {
        CTR,
        WIN64
    };

    class Platform {
    public:
        Platform();

        /**
         * Gets enum type of this platform
        */
        const PlatformType GetPlatformType();

        /**
         * Gets nice name for this platform
        */
        const char* GetPlatformName();
    };

}