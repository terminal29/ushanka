
#include <entt/entt.hpp>
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
    auto playerEntity = PlayerPrefab::make(registry, {});
	auto cameraEntity = CameraPrefab::make(registry, CameraPrefab::CameraOptions{ _renderer });
	auto camera = registry.get<Camera>(cameraEntity);
    while (_renderer->waitForVSync()) {
        _renderer->frameBegin();

        auto renderables = registry.view<PolyRenderable>();
		for (auto entity : renderables) {
			auto& renderable = renderables.get<PolyRenderable>(entity);
			renderable->onRender( registry, entity, camera, *_renderer);
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