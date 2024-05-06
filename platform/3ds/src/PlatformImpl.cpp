#include "../../common/include/Platform.h"
#include <3ds.h>

Platform::Platform()
{
}

const PlatformType Platform::GetPlatformType()
{
    return PlatformType::CTR;
}

const char* Platform::GetPlatformName()
{
    return "CTR";
}

void Platform::sleep(uint32_t millis)
{
    svcSleepThread(1000000 * millis);
}