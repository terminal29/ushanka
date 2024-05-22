#include "components/RenderableChunk.h"

namespace U {
	RenderableChunk::RenderableChunk(glm::ivec3 globalPosition, std::vector<Voxel> voxels) :_globalPosition(globalPosition), _voxels(voxels)
	{
	}
	void RenderableChunk::onRender(entt::registry& registry, entt::entity entity, Camera& camera, Renderer& renderer) {
		renderer.drawVoxels(camera, _globalPosition, _voxels);
	}

}