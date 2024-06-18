#pragma once 
#include <game/session/world/chunk/octree/ChunkOctree.h>
#include <platform/RenderObject.h>

namespace U {
	/// <summary>
	/// Provides meshing capability for an octree
	/// </summary>
	class OctreeMesher {
		/// <summary>
		/// Ref to octree
		/// </summary>
		std::shared_ptr<ChunkOctree> _octree;
		
		/// <summary>
		/// Ref to render object
		/// </summary>
		std::shared_ptr<RenderObject> _renderObject;

	public:
		OctreeMesher(std::shared_ptr<ChunkOctree> octree);


	};
}