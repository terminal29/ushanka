#pragma once
#include <optional>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <game/render/Voxel.h>

namespace U {

	struct ChunkPrefab {
		struct ChunkPrefabOptions {
			glm::ivec3 globalPosition;
			std::vector<Voxel> voxels;
		};

		/// Makes a chunk entity with all the necessary components
		/// 
		/// components:
		/// - RenderableChunk
		static entt::entity make(entt::registry& registry, ChunkPrefabOptions options);
	};
}
