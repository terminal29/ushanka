#pragma once
#include <memory>

namespace U::Engine::Render {

	/// <summary>
	/// Storage class for a voxel "object" 
	/// </summary>
	template <std::size_t SIZE = 32>
	class TVoxelStorage {
		static_assert(SIZE <= UINT8_MAX && "SIZE must be less than uint8 max");
		using voxel_index_t = glm::tvec3<uint8_t>;
		using change_cb_t = std::function<void(const TVoxelStorage<SIZE>& self)>;
		using unsubscribe_t = std::function<void()>;

		constexpr static uint8_t MAX_STORAGE_SIZE = 32;
		constexpr static auto MAX_VOXELS = MAX_STORAGE_SIZE * MAX_STORAGE_SIZE * MAX_STORAGE_SIZE;

	private:
		std::map<voxel_index_t, VoxelType::voxel_tag_t> _voxels;

		std::vector<change_cb_t> _changeWatchers;
	public:
		VoxelType::voxel_tag_t get(voxel_index_t position) const {
			return _voxels.at(position);
		}

		void set(voxel_index_t position, VoxelType::voxel_tag_t type) {
			_voxels[position] = type;
			for (auto& cb : _changeWatchers) {
				cb(*this);
			}
		}

		std::size_t size() const {
			_voxels.size();
		}

		unsubscribe_t onChange(change_cb_t cb) {
			_changeWatchers.push_back(cb);
			return [this, cb]() {
				auto it = std::find(_changeWatchers.begin(), _changeWatchers.end(), cb);
				if (it != _changeWatchers.end()) {
					_changeWatchers.erase(it);
				}
			};
		}
	};

	using VoxelStorage = TVoxelStorage<32>;

	class NativeRenderObject;

	/// <summary>
	/// Generic voxel "render object" class
	/// Generally used to represent a mesh and material in the platform specific rendering system
	/// </summary>
	class VoxelRenderObject {

		std::unique_ptr<NativeRenderObject> _impl;
	
	public:

		/// <summary>
		/// // Constructs a new render object for the given storage
		/// </summary>
		/// <param name="storage">Reference to the storage</param>
		/// <returns></returns>
		VoxelRenderObject(std::shared_ptr<VoxelStorage> storage);

		/// <summary>
		/// Invalidate this render object
		/// 
		/// (will be reconstructed on next call to bind)
		/// </summary>
		void invalidate();

		/// <summary>
		/// Reconstructs the render object using the given storage, or the current storage if nullptr
		/// 
		/// Call manually when the storage has changed if you want
		/// (will get called automatically if invalid when bind is called)
		/// </summary>
		void reconstruct(std::shared_ptr<VoxelStorage> storage = nullptr);

		/// <summary>
		/// Ptr to underlying native object
		/// </summary>
		/// <returns></returns>
		NativeRenderObject* getNativeObject();


	};
}
