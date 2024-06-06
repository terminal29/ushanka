#pragma once
#include <game/session/world/chunk/voxel/VoxelType.h>
#include <entt/entt.hpp>

namespace U {
	using namespace entt::literals;
	class VoxelRegistry {
		constexpr static auto VoxelTypes = {
			
			VoxelType({
				.tag = "stone"_hs,
				.renderType = VoxelType::RenderType::CUBE,
				.isTransparent = false,
				.useSidedTexture = false,
				.textureTags = "stone"_hs
			}),
			VoxelType({
				.tag = "dirt"_hs,
				.renderType = VoxelType::RenderType::CUBE,
				.isTransparent = false,
				.useSidedTexture = false,
				.textureTags = "dirt"_hs
			}),
			VoxelType({
				.tag = "grass"_hs,
				.renderType = VoxelType::RenderType::CUBE,
				.isTransparent = false,
				.useSidedTexture = true,
				.textureTags = std::array<VoxelType::texture_tag_t, 6>{
					"grass_top"_hs,
					"dirt"_hs,
					"grass_side"_hs,
					"grass_side"_hs,
					"grass_side"_hs,
					"grass_side"_hs
				}
			}),
			VoxelType({
				.tag = "bedrock"_hs,
				.renderType = VoxelType::RenderType::CUBE,
				.isTransparent = false,
				.useSidedTexture = false,
				.textureTags = "bedrock"_hs
			})
		};
	};

	VoxelRegistry GlobalVoxelRegistry;
}