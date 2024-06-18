#pragma once 
#include <game/session/world/chunk/octree/ChunkOctree.h>
#include <glm/vec3.hpp>
#include <entt/entt.hpp>
#include <util/mesher/OctreeMesher.h>

namespace U {
	class Chunk {

	public:
		constexpr static uint16_t size = 256;

		using ChunkId = entt::hashed_string::hash_type;

		virtual ~Chunk() = 0;
		
		ChunkOctree octree;

		OctreeMesher mesher;

	};
}