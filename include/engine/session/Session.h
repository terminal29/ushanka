#pragma once
#include "engine/session/World.h";

namespace U::Engine::Core {
	/// <summary>
	/// Represents a gameplay session
	/// 
	/// Similar to a "world" but doesnt have to be a 3d world, could be a menu or something else
	/// 
	/// </summary>
	class Session {
		std::vector<std::shared_ptr<World>> _worlds;

		std::shared_ptr<World> _currentWorld;

		

	public:
		
		/// <summary>
		/// Inits a session and sets the given world as current
		/// </summary>
		/// <param name="world"></param>
		Session(std::shared_ptr<World> world) : _worlds{ world }, _currentWorld(world) {}

		/// <summary>
		/// Initializes a session with multiple worlds and sets the first one as current
		/// </summary>
		/// <param name="worlds"></param>
		Session(std::vector<std::shared_ptr<World>> worlds) : _worlds(worlds) {
			if (worlds.size() == 0) throw std::exception("no worlds given to session!");
			_currentWorld = worlds[0];
		}

		/// <summary>
		/// Ticks the session
		/// 
		/// Triggers current world tick
		/// </summary>
		void tick();

		/// <summary>
		/// Adds a world to the session
		/// </summary>
		/// <param name="world"></param>
		/// <returns></returns>
		bool addWorld(std::shared_ptr<World> world);



	};
}