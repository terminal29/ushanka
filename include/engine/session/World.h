#pragma once 
#include <entt/entt.hpp>
#include <engine/session/Chunk.h>

namespace U::Engine::Core {
	
	/// <summary>
	/// Represents a 3d world
	/// 
	/// Holds entities, world-specific systems and data, and manages entities for that world
	/// </summary>
	class World {

		// TODO: subclass to 3dWorld vs UIWorld or something similar
		//std::vector<U::Engine::Session::Chunk> _chunks;

		entt::registry _entities;

	public:
		/// <summary>
		/// Ticks the world
		/// </summary>
		void tick();

	};
}