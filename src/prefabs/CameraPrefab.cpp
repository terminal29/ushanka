#include "game/prefabs/PlayerPrefab.h"
#include "engine/components/Transform.h"
#include "engine/components/Renderable.h"
#include "engine/components/Camera.h"
#include "engine/render/registry/ShaderRegistry.h"
#include "entt/entt.hpp"

using namespace U::Engine::Components;
using namespace U::Game::Prefabs;


entt::entity CameraPrefab::make(entt::registry& registry, CameraPrefab::CameraOptions options)
{
	auto entity = registry.create();
	registry.emplace<Transform>(entity, options.initialTransform.value_or(Transform{}));
	registry.emplace<Camera>(entity, options.renderer);
	return entity;
}
