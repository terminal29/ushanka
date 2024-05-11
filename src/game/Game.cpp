
#include <entt/entt.hpp>
#include "components/Square.h"
#include "game/Game.h"
#include "platform/Renderer.h"
#include "prefabs/CameraPrefab.h"
#include "prefabs/PlayerPrefab.h"
#include "components/Renderable.h"

using namespace U;


Game::Game(std::shared_ptr<Platform> platform)
    : _platform(platform)
    , _renderer(std::make_shared<Renderer>(platform))
{
}

entt::registry registry;

bool Game::run()
{
    bool success = true;
    auto player = PlayerPrefab::make(registry, {});
	auto camera = CameraPrefab::make(registry, CameraPrefab::CameraOptions{ _renderer });
    while (_renderer->wait()) {
        _renderer->frameBegin();

        auto renderables = registry.view<PolyRenderable>();
		for (auto entity : renderables) {
			auto& renderable = renderables.get<PolyRenderable>(entity);
			renderable->onRender( registry, entity, *_renderer);
		}

        /*auto tickables = registry.view<PolyTickable>();
		for (auto entity : tickables) {
			auto& tickable = tickables.get<PolyTickable>(entity);
			tickable->onTick(registry, entity);
		}*/

        _renderer->frameEnd();
    }

    return success;
}