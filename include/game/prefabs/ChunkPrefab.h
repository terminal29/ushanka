#pragma once
#include <optional>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <engine/render/voxel/Voxel.h>

namespace U::Engine::Prefabs {

	struct ChunkPrefab {
		struct ChunkPrefabOptions {
			glm::ivec3 globalPosition;
			std::vector<U::Engine::Render::Voxel::Voxel> voxels;
		};

		/// Makes a chunk entity with all the necessary components
		/// 
		/// components:
		/// - RenderableChunk
		static entt::entity make(entt::registry& registry, ChunkPrefabOptions options);
	};
}
