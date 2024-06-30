#pragma once
#include <entt/entt.hpp>
#include <variant>
#include <array>
#include <cstdint>

namespace U::Engine::Render::Voxel {
	struct VoxelType {
		using voxel_tag_t = entt::hashed_string::hash_type;
		using texture_tag_t = entt::hashed_string::hash_type;

		enum class RenderType : uint8_t {
			CUBE = 0
		};

		struct VoxelTypeOptions {
			const voxel_tag_t tag;
			const RenderType renderType;
			const bool isTransparent;
			const bool useSidedTexture;
			// top bottom north south east west
			const std::variant<texture_tag_t, std::array<texture_tag_t, 6>> textureTags;
		};

		VoxelTypeOptions options;

		constexpr VoxelType(VoxelTypeOptions options) noexcept : options(options) {};

	};
}