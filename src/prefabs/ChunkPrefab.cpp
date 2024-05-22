#include "prefabs/ChunkPrefab.h"
#include "game/registry/ShaderRegistry.h"
#include "components/RenderableChunk.h"
#include "components/Renderable.h"

entt::entity U::ChunkPrefab::make(entt::registry& registry, U::ChunkPrefab::ChunkPrefabOptions options) {
	auto entity = registry.create();
	PolyRenderable renderable{
		RenderableChunk(options.globalPosition, options.voxels)
	};
	registry.emplace<PolyRenderable>(entity, std::move(renderable));
	return entity;

}