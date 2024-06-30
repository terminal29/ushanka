#pragma once
#include <entt/entt.hpp>
#include "engine/util/common/Constants.h"
#include <engine/render/Shader.h>
#include <variant>
#include <unordered_map>
#include "engine/render/registry/NamedShader.h"

namespace U::Engine::Render {
	struct ShaderRegistry {
		static const std::unordered_map<ENamedShader, entt::hashed_string> NamedShaderIDs;

		static std::unordered_map<entt::hashed_string::hash_type, std::shared_ptr<Shader>> Registry;

		static void initDefaultShaders();

	};
}