#pragma once
#include "platform/Renderer.h"
#include "platform/Platform.h"
#include <memory>

namespace U {

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
}