#pragma once
#include <entt/entt.hpp>
#include "util/Constants.h"
#include <platform/Shader.h>
#include <variant>
#include <unordered_map>
#include "game/registry/NamedShader.h"

namespace U {
	struct ShaderRegistry {
		static const std::unordered_map<ENamedShader, entt::hashed_string> NamedShaderIDs;

		static std::unordered_map<entt::hashed_string::hash_type, std::shared_ptr<Shader>> Registry;

		static void initDefaultShaders();

	};
}