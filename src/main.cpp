#include "game/Game.h"
#include "platform/Platform.h"
#include <iostream>

using namespace U;

int main()
{
    std::shared_ptr<Platform> platform = std::make_shared<Platform>();

    Game game(platform);
    bool success = game.run();

    return success ? 0 : 1;
}