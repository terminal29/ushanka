#include <chrono> // chrono::system_clock
#include <ctime> // localtime
#include <iomanip> // put_time
#include <iostream>
#include <sstream> // stringstream
#include <string> // string
#include <thirdparty/entt/entt.hpp>

#include "include/components/Square.h"
#include "include/game/Game.h"
#include "platform/common/include/Renderer.h"
#include "platform/common/include/Camera.h"

std::string return_current_time_and_date()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

Game::Game(std::shared_ptr<Platform> platform)
    : _platform(platform)
    , _renderer(std::make_shared<Renderer>(platform))
{
}

entt::registry registry;

bool Game::run()
{
    bool success = true;
    const auto squareEntity = registry.create();
    registry.emplace<Square>(squareEntity, RGBColor(255, 0, 0));

    auto camera = _renderer->getActiveCamera();

    while (_renderer->wait()) {
        _renderer->frameBegin();

        unsigned long milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        float hue = -(milliseconds_since_epoch / 100) % 360 / 360.0f;
        RGBColor squareColor = HSLToRGB(hue, 1.0, 0.5);

        auto view = registry.view<Square>();
        for (auto [entity, square] : view.each()) {
            square.render(*_renderer);
            square._color = squareColor;
        }

        _renderer->frameEnd();
    }

    return success;
}