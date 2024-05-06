#include "include/game/Game.h"
#include "platform/common/include/Platform.h"
#include <iostream>

int main()
{
    std::shared_ptr<Platform> platform = std::make_shared<Platform>();

    Game game(platform);
    bool success = game.run();

    return success ? 0 : 1;
}