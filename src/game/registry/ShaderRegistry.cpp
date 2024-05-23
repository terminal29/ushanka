#include "game/registry/ShaderRegistry.h"
#include "platform/Shader.h"
#include <format>
#include "NativeShaderRegistry.h"

namespace U {
	
	const std::unordered_map<ENamedShader, entt::hashed_string> ShaderRegistry::NamedShaderIDs{
		{ENamedShader::Standard, "Standard"}
	};

	std::unordered_map<entt::hashed_string::hash_type, std::shared_ptr<Shader>> ShaderRegistry::Registry{};

	void ShaderRegistry::initDefaultShaders() {
		NativeShaderRegistry::initDefaultShaders(NamedShaderIDs, Registry);
	}
}