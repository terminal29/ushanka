#pragma once
#include "util/Point.h"
#include "util/Color.h"
#include <entt/entt.hpp>
#include <glm/vec3.hpp>

namespace U {
	struct Voxel {
		glm::ivec3 position;
		RGBColor color;
		entt::hashed_string shaderID;
	};
}