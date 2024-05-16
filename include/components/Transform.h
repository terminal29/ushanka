#pragma once 
#include <glm/glm.hpp>

namespace U {

	struct Transform {
		glm::vec3 position{ 0,0,0 };
		glm::vec3 rotation{ 0,0,0 };
		glm::vec3 scale{ 1,1,1 };

		Transform();

		Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

		Transform(const Transform& other);

	};
}