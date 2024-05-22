#pragma once
#include <entt/entt.hpp>
#include "util/Constants.h"
#include <platform/Shader.h>
#include <variant>
#include <unordered_map>

namespace U {
	enum class ENamedShader {
		Standard
	};

	extern const std::unordered_map<ENamedShader, entt::hashed_string> NamedShaderIDs;

	extern std::unordered_map<entt::hashed_string::hash_type, std::shared_ptr<Shader>> ShaderRegistry;
	
	void initDefaultShaders();
}