#pragma once
#include <cstdint>

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

    /**
     * sleeps current thread 
    */
    void sleep(uint32_t millis);
};