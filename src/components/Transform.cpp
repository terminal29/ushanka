#include "components/Transform.h"

U::Transform::Transform() : position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(glm::vec3(1.0f))
{
}

U::Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: position(position), rotation(rotation), scale(scale)
{
}

U::Transform::Transform(const Transform& other)
{
	position = other.position;
	rotation = other.rotation;
	scale = other.scale;
}


