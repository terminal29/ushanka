#include "engine/render/registry/ShaderRegistry.h"
#include "engine/render/Shader.h"
#include <format>
#include "native/NativeShaderRegistry.h"

namespace U::Engine::Render {
	
	const std::unordered_map<ENamedShader, entt::hashed_string> ShaderRegistry::NamedShaderIDs{
		{ENamedShader::Standard, "Standard"}
	};

	std::unordered_map<entt::hashed_string::hash_type, std::shared_ptr<Shader>> ShaderRegistry::Registry{};

	void ShaderRegistry::initDefaultShaders() {
		NativeShaderRegistry::initDefaultShaders(NamedShaderIDs, Registry);
	}
}