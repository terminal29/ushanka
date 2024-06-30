#pragma once 
#include <glm/vec3.hpp>
#include <entt/entt.hpp>
#include <engine/render/RenderObject.h>

namespace U::Engine::Core {
	class Chunk {
	public:

		using chunk_position_t = glm::ivec3;

		using chunk_id_t = entt::hashed_string::hash_type;

	private:

		const chunk_id_t _id;

		const chunk_position_t _position;

		std::shared_ptr<U::Engine::Render::VoxelStorage> _storage;

		U::Engine::Render::VoxelRenderObject _renderObject;

	public:

		Chunk(chunk_id_t id, chunk_position_t position) :
			_id(id),
			_position(position),
			_renderObject(_storage)
		{

		}

		U::Engine::Render::VoxelRenderObject& getChunkRenderObject() {
			return _renderObject;
		}
	};
}