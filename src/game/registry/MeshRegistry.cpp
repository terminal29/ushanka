#include "game/registry/MeshRegistry.h"

namespace U {
	void initDefaultMeshes() {
		MeshRegistry.load(NamedMesh::Cube, std::make_shared<Mesh>(std::vector<vertex_t>{
			// front
			{-1.0f, -1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f },
			{ 1.0f, -1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f },
			{ 1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f },
			{-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f }
		}));
	}
}