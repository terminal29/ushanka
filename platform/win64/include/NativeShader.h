#pragma once 
#include <glad/include/gl.h>
#include <util/Constants.h>
#include <platform/Shader.h>

namespace U {
	class Renderer;
	class NativeRenderer;

	class NativeShader {
		friend class NativeRenderer;
		GLuint _shaderProgramObject;

		std::unordered_map<Shader::ENamedShaderUniform, GLint> _uniforms;

	public:
		void compile(
			const std::string& vertexShader, 
			const std::string& fragmentShader,
			std::unordered_map<Shader::ENamedShaderUniform, std::string> uniformNames
			);
		void bind() noexcept;
		void unbind() noexcept;

		void setUniform(Shader::ENamedShaderUniform uniform, const glm::mat4& value) noexcept;
		void setUniform(Shader::ENamedShaderUniform uniform, const glm::mat3& value) noexcept;
		void setUniform(Shader::ENamedShaderUniform uniform, const glm::vec3& value) noexcept;
		void setUniform(Shader::ENamedShaderUniform uniform, const glm::vec4& value) noexcept;

	};
}