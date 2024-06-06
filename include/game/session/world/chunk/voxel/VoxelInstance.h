#pragma once
#include <game/session/world/chunk/voxel/VoxelType.h>
#include <game/util/data/Vec.h>

namespace U {
	struct VoxelInstance {
		VoxelType type;
		u8Vec3 position;
	};
}