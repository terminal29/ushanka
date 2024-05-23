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

		void setUniformMat4(Shader::ENamedShaderUniform uniform, const glm::fmat4& value) noexcept;
		void setUniformMat3(Shader::ENamedShaderUniform uniform, const glm::fmat3& value) noexcept;
		void setUniformVec3(Shader::ENamedShaderUniform uniform, const glm::fvec3& value) noexcept;
		void setUniformVec4(Shader::ENamedShaderUniform uniform, const glm::fvec4& value) noexcept;

	};
}