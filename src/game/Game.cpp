
#include <entt/entt.hpp>
#include "game/Game.h"
#include "platform/Renderer.h"
#include "prefabs/CameraPrefab.h"
#include "prefabs/PlayerPrefab.h"
#include "prefabs/ChunkPrefab.h"
#include "components/Renderable.h"
#include "game/registry/ShaderRegistry.h"

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
	initDefaultShaders();
	
	// Make a chunk "entity"
	std::vector<Voxel> voxels;
	for (int x = 0; x < 16; x++) {
		for (int y = 0; y < 16; y++) {
			for (int z = 0; z < 16; z++) {
				voxels.push_back({ {x, y, z}, {255, 0, 0}, NamedShaderIDs.at(ENamedShader::Standard) });
			}
		}
	}
	ChunkPrefab::ChunkPrefabOptions opts;
	opts.globalPosition = { 0, 0, 0 };
	opts.voxels = voxels;
	auto chunkEntity = ChunkPrefab::make(registry, opts);

	// Make a camera "entity" so we can render something
	auto cameraEntity = CameraPrefab::make(registry, CameraPrefab::CameraOptions{ _renderer });
	auto camera = registry.get<Camera>(cameraEntity);
    while (_renderer->waitForVSync()) {
        _renderer->frameBegin();

		// Render everything that implements PolyRenderable
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