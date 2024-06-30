#include <platform/RenderObject.h>
#include <NativeRenderObject.h>

namespace U::Engine::Render {

	RenderObject::RenderObject(std::shared_ptr<VoxelStorage> storage) {
		_impl = std::make_unique<NativeRenderObject>(storage);
	}

	void RenderObject::invalidate() {
		_impl->invalidate();
	}

	void RenderObject::reconstruct(std::shared_ptr<VoxelStorage> storage) {
		_impl->reconstruct(storage);
	}

	NativeRenderObject* RenderObject::getNativeObject() {
		return _impl.get(); 
	}

}