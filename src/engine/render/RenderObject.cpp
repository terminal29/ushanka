#include <engine/render/RenderObject.h>
#include <native/NativeRenderObject.h>

namespace U::Engine::Render {

	VoxelRenderObject::VoxelRenderObject(std::shared_ptr<const VoxelStorage> storage) :
		_impl{ storage } 
	{}

	void VoxelRenderObject::invalidate() {
		_impl->invalidate();
	}

	void VoxelRenderObject::reconstruct(std::shared_ptr<const VoxelStorage> storage) {
		_impl->reconstruct(storage);
	}

	U::Native::NativeRenderObject& VoxelRenderObject::getNativeObject() {
		return *_impl;
	}

	VoxelRenderObject::VoxelRenderObject(const VoxelRenderObject& other) noexcept = default;
	VoxelRenderObject::VoxelRenderObject(VoxelRenderObject&& other) noexcept = default;
	VoxelRenderObject& VoxelRenderObject::operator=(const VoxelRenderObject& other) noexcept = default;
	VoxelRenderObject& VoxelRenderObject::operator=(VoxelRenderObject&& other) noexcept = default;
	VoxelRenderObject::~VoxelRenderObject() noexcept = default;



}