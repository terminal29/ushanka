#pragma once
#include <functional>

namespace U {
    /**
     * Component that tick() is called each render frame
    */
    class Tickable {

        std::function<void(float)> _onTick;

    public:
        Tickable(std::function<void(float)> onTick);

        void tick(float deltaTime);
    };

}