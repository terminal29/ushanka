#pragma once 
#include <glm/vec3.hpp>
#include <entt/entt.hpp>

namespace U {
	class Chunk {

	public:
		constexpr static uint8_t size = 256;

		using ChunkId = entt::hashed_string::hash_type;

		virtual ~Chunk() = 0;

	};
}