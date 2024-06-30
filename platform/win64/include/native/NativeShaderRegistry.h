#pragma once 
#include <unordered_map>
#include <entt/entt.hpp>
#include "platform/Shader.h"
#include "game/registry/NamedShader.h"

namespace U {
	struct NativeShaderRegistry {
		static void initDefaultShaders(
			const std::unordered_map<ENamedShader, entt::hashed_string>& ids,
			std::unordered_map<entt::hashed_string::hash_type, std::shared_ptr<Shader>>& registry
		);
	};
}