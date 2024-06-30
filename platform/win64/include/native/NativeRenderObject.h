#pragma once 
#include "engine/render/RenderObject.h"

namespace U::Native {
	class NativeRenderObject {
		std::shared_ptr<U::Engine::Render::VoxelStorage> _storage;
		bool _valid = false;

	public:
		NativeRenderObject(std::shared_ptr<U::Engine::Render::VoxelStorage> storage) :
			_storage(storage) {

			reconstruct();
		}

		void invalidate() {
			// Invalidate the render object
			_valid = false;
		}

		void reconstruct(std::shared_ptr<U::Engine::Render::VoxelStorage> storage = nullptr) {
			// Reconstruct the render object
			_storage = storage ? storage : _storage;
			if (!_storage) return;

			// todo: meshing logic
			// todo: vbo/vao creation/reallocation


			// ...
			_valid = true;
		}

	};
}