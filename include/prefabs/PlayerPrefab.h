#pragma once
#include <optional>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <components/Transform.h>

namespace U {

	struct PlayerPrefab {
		struct PlayerOptions {
			std::optional<Transform> initialTransform{ std::nullopt };
		};

		/// Makes a player entity with all the necessary components
		/// 
		/// components:
		/// - Transform
		/// - Controller
		static entt::entity make(entt::registry& registry, PlayerOptions options);
	};
}
