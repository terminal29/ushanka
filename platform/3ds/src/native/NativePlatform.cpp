#include "platform/Platform.h"
#include <3ds.h>

U::Platform::Platform() noexcept
{
}

constexpr U::PlatformType U::Platform::GetPlatformType()
{
    return U::PlatformType::CTR;
}

constexpr const char* U::Platform::GetPlatformName()
{
    return "CTR";
}
