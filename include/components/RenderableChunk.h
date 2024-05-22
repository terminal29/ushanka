#pragma once
#include <vector>
#include "game/render/Voxel.h"
#include <platform/Renderer.h>
#include <components/Camera.h>


namespace U {
	class RenderableChunk {
		glm::ivec3 _globalPosition;
		std::vector<Voxel> _voxels;

	public:
		RenderableChunk(glm::ivec3 globalPosition, std::vector<Voxel> voxels);

		void onRender(entt::registry& registry, entt::entity entity, Camera& camera, Renderer& renderer);
	};
}