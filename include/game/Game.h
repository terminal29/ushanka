#pragma once
#include "platform/common/include/Platform.h"
#include "platform/common/include/Renderer.h"
#include <memory>

class Game {
    std::shared_ptr<Platform> _platform;
    std::shared_ptr<Renderer> _renderer;

public:
    /**
     * Inits the game for this platform
    */
    Game(std::shared_ptr<Platform> platform);

    /**
     * Runs the main game loop
    */
    bool run();
};