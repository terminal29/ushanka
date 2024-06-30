#pragma once 
#include <unordered_map>
#include <glad/include/gl.h>
#include <engine/util/common/Constants.h>
#include <engine/render/Shader.h>


namespace U::Native {
	namespace U::Engine::Render {
		class Renderer;
	}

	namespace Native {
		using namespace U::Engine::Render;
		class NativeShader {

			GLuint _shaderProgramObject;

			std::unordered_map<U::Engine::Render::Shader::ENamedShaderUniform, GLint> _uniforms;

		public:
			void compile(
				const std::string& vertexShader,
				const std::string& fragmentShader,
				std::unordered_map<U::Engine::Render::Shader::ENamedShaderUniform, std::string> uniformNames
			);
			void bind() noexcept;
			void unbind() noexcept;

			void setUniformMat4(U::Engine::Render::Shader::ENamedShaderUniform uniform, const glm::fmat4& value) noexcept;
			void setUniformMat3(U::Engine::Render::Shader::ENamedShaderUniform uniform, const glm::fmat3& value) noexcept;
			void setUniformVec3(U::Engine::Render::Shader::ENamedShaderUniform uniform, const glm::fvec3& value) noexcept;
			void setUniformVec4(U::Engine::Render::Shader::ENamedShaderUniform uniform, const glm::fvec4& value) noexcept;

		};
	};
};


	