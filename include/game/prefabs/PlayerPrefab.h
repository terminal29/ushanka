#pragma once
#include <optional>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <engine/components/Transform.h>

namespace U::Game::Prefabs {

	struct PlayerPrefab {
		struct PlayerOptions {
			std::optional<U::Engine::Components::Transform> initialTransform{ std::nullopt };
		};

		/// Makes a player entity with all the necessary components
		/// 
		/// components:
		/// - Transform
		static entt::entity make(entt::registry& registry, PlayerOptions options);
	};
}
