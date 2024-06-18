#pragma once 
#include <memory>
#include <game/session/world/chunk/Chunk.h>

namespace U {
	class ChunkRenderer {
		std::shared_ptr<Chunk> _chunk;


	public:
		ChunkRenderer(std::shared_ptr<Chunk> chunk);
	};
}