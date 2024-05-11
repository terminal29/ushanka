#include "prefabs/CameraPrefab.h"
#include "components/Transform.h"
#include "components/Camera.h"

entt::entity U::CameraPrefab::make(entt::registry& registry, U::CameraPrefab::CameraOptions options)
{
	auto entity = registry.create();
	registry.emplace<Transform>(entity, options.initialTransform.value_or(Transform{}));
	registry.emplace<Camera>(entity, options.renderer);
	return entity;
}
