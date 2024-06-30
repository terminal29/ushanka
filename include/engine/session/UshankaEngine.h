#pragma once
#include "engine/render/Renderer.h"
#include "engine/platform/Platform.h"
#include <engine/session/Session.h>
#include <memory>

namespace U::Engine::Core {

    class UshankaEngine {

        std::shared_ptr<U::Engine::Platform::Platform> _platform;

        Session _session;

    public:
        /**
         * Inits the engine for this platform
        */
        UshankaEngine(std::shared_ptr<U::Engine::Platform::Platform> platform);

        /**
         * Runs the main game loop
        */
        bool run();
    };
}