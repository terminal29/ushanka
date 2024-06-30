#include "NativeShaderRegistry.h"

namespace U {
	void NativeShaderRegistry::initDefaultShaders(
		const std::unordered_map<ENamedShader, entt::hashed_string>& ids,
		std::unordered_map<entt::hashed_string::hash_type, std::shared_ptr<Shader>>& registry) {
		std::unordered_map<Shader::ENamedShaderUniform, std::string> uniforms{
				{Shader::ENamedShaderUniform::ModelMatrix, "ModelMatrix"},
				{Shader::ENamedShaderUniform::ViewMatrix, "ViewMatrix"},
				{Shader::ENamedShaderUniform::ProjectionMatrix, "ProjectionMatrix"},
				{Shader::ENamedShaderUniform::NormalMatrix, "NormalMatrix"},
				{Shader::ENamedShaderUniform::AmbientLightColor, "AmbientLightColor"},
				{Shader::ENamedShaderUniform::SunDirection, "SunDirection"},
				{Shader::ENamedShaderUniform::SunColor, "SunColor"},
				{Shader::ENamedShaderUniform::Texture0, "Texture0"}
		};

		// todo: runtime shader compilation for 3ds
		auto defaultshader = std::shared_ptr<Shader>(
			new Shader("", "", uniforms));

		const auto id = ids.at(ENamedShader::Standard);
		registry.insert_or_assign(id.value(), defaultshader);
	}
}