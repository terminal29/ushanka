
#include "game/Game.h"
#include <chrono>
#include <cmath>
#include <entt/entt.hpp>
#include <iostream>


#define GLM_ENABLE_EXPERIMENTAL
#include "components/Renderable.h"
#include "game/registry/ShaderRegistry.h"
#include "glm/gtx/string_cast.hpp"
#include "platform/Renderer.h"
#include "platform/Timer.h"
#include "prefabs/CameraPrefab.h"
#include "prefabs/ChunkPrefab.h"
#include "prefabs/PlayerPrefab.h"
#include <numbers>

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
    ShaderRegistry::initDefaultShaders();

    // Make a chunk "entity"
    std::vector<Voxel> voxels;
    voxels.push_back({ { 0, 0, 0 }, { 255, 0, 0 }, ShaderRegistry::NamedShaderIDs.at(ENamedShader::Standard) });
    ChunkPrefab::ChunkPrefabOptions opts;
    opts.globalPosition = { 0, 0, 0 };
    opts.voxels = voxels;
    auto chunkEntity = ChunkPrefab::make(registry, opts);

    // Make a camera "entity" so we can render something
    auto cameraEntity = CameraPrefab::make(registry, CameraPrefab::CameraOptions { _renderer });
    auto camera = registry.get<Camera>(cameraEntity);
    GlobalTimer.start();
    while (_renderer->waitForVSync()) {
        GlobalTimer.tick();
        _renderer->frameBegin();

        // Render everything that implements PolyRenderable
        auto renderables = registry.view<PolyRenderable>();
        for (auto entity : renderables) {
            auto& renderable = renderables.get<PolyRenderable>(entity);
            renderable->onRender(registry, entity, camera, *_renderer);
        }

        // rotate camera for debugging
        const float rotation = std::numbers::pi * GlobalTimer.msSinceStart / 1000.f;
        //std::cout << "rotation " << rotation << "\n";
        camera.setPosition(glm::fvec3(4 * std::sin(rotation), 2 * std::cos(rotation/4), 4 * std::cos(rotation)));
        camera.setTarget(glm::fvec3(0, 0, 0));
        //std::cout << "camera position " << glm::to_string(camera.getPosition()) << "\n";

        /*auto tickables = registry.view<PolyTickable>();
		for (auto entity : tickables) {
			auto& tickable = tickables.get<PolyTickable>(entity);
			tickable->onTick(registry, entity);
		}*/

        _renderer->frameEnd();
    }

    return success;
}