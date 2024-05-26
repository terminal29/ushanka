#include "platform/Platform.h"
#include <3ds.h>

U::Platform::Platform()
{
}

const U::PlatformType U::Platform::GetPlatformType()
{
    return U::PlatformType::CTR;
}

const char* U::Platform::GetPlatformName()
{
    return "CTR";
}
