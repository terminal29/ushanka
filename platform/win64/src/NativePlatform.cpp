#include "platform/Platform.h"
#include <thread>
using namespace U;

Platform::Platform() noexcept
{
}

constexpr PlatformType Platform::GetPlatformType()
{
    return PlatformType::WIN64;
}

constexpr const char* Platform::GetPlatformName()
{
    return "WIN64";
}