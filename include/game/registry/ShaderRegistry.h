#pragma once
#include <entt/entt.hpp>
#include "util/Constants.h"
#include <platform/Shader.h>
#include <variant>

namespace U {
	struct NamedShader {
		static constexpr auto Standard = entt::hashed_string{ "standard" };
	};

	struct ShaderLoader final {
		using result_type = std::shared_ptr<Shader>;

		result_type operator()(result_type shader) const {
			return shader;
		}
	};


	auto ShaderRegistry = entt::resource_cache<std::shared_ptr<Shader>, ShaderLoader>{};
	
	void initDefaultShaders();
}