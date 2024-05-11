#pragma once
#include <optional>
#include <memory>
#include <platform/Renderer.h>
#include <components/Transform.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace U {

	/// Makes a camera entity with all the necessary components
	/// 
	/// components:
	/// - Transform
	/// - Camera
	struct CameraPrefab {

		struct CameraOptions {
			std::shared_ptr<Renderer> renderer;
			std::optional<Transform> initialTransform{ std::nullopt };
		};

		static entt::entity make(entt::registry& registry, CameraOptions options);
	};
}