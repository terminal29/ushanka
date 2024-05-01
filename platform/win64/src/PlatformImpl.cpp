#include "../../common/include/Platform.h"
#include <thread>

Platform::Platform()
{
}

const PlatformType Platform::GetPlatformType()
{
    return PlatformType::WIN64;
}

const char* Platform::GetPlatformName()
{
    return "WIN64";
}

void Platform::sleep(uint32_t millis)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}