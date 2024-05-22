#include "prefabs/PlayerPrefab.h"
#include "components/Transform.h"
#include "components/Controller.h"
#include "components/Renderable.h"
#include "components/RenderableMesh.h"
#include "game/registry/ShaderRegistry.h"

entt::entity U::PlayerPrefab::make(entt::registry& registry, U::PlayerPrefab::PlayerOptions options) {
	auto entity = registry.create();
	registry.emplace<Transform>(entity, options.initialTransform.value_or(Transform{}));
	registry.emplace<Controller>(entity);
	return entity;

}