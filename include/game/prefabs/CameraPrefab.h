#pragma once
#include <optional>
#include <memory>
#include <engine/render/Renderer.h>
#include <engine/components/Transform.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace U::Game::Prefabs {

	/// Makes a camera entity with all the necessary components
	/// 
	/// components:
	/// - Transform
	/// - Camera
	struct CameraPrefab {

		struct CameraOptions {
			std::shared_ptr<U::Engine::Render::Renderer> renderer;
			std::optional<U::Engine::Components::Transform> initialTransform{ std::nullopt };
		};

		static entt::entity make(entt::registry& registry, CameraOptions options);
	};
}