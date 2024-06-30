#include "engine/session/UshankaEngine.h"
#include "engine/platform/Platform.h"
#include <iostream>

using namespace U::Engine::Platform;
using namespace U::Engine::Core;

int main()
{
    std::shared_ptr<Platform> platform = std::make_shared<Platform>();

    UshankaEngine engine(platform);
    bool success = engine.run();

    return success ? 0 : 1;
}