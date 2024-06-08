#pragma once
#include <cstdint>
#include <array>
#include <cstdint>
#include <map>
#include "game/session/world/chunk/Chunk.h"

namespace U {

	class World {
		
	public:
		using index_t = uint32_t;

		std::map<index_t, std::map<index_t, Chunk>> Chunks;

	};
}