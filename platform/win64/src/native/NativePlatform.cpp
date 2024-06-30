#include "engine/platform/Platform.h"
#include <thread>

using namespace U::Engine::Platform;


constexpr PlatformType Platform::GetPlatformType()
{
    return PlatformType::WIN64;
}

constexpr const char* Platform::GetPlatformName()
{
    return "WIN64";
}