#include "platform/Platform.h"
#include <thread>
using namespace U;

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