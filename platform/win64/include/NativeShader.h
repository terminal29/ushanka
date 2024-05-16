#pragma once 
#include <glad/include/gl.h>
#include <util/Constants.h>

namespace U {
	class Renderer;

	class NativeShader {
		GLuint _shaderProgramObject;

		std::map<NamedShaderUniform, GLint> _uniforms;
		std::map<NamedShaderParam, GLint> _params;

	public:
		void compile(const std::string& vertexShader, const std::string& fragmentShader);
		void bind();
		void unbind();

		void setUniform(const std::string& name, const glm::mat4& value);
		void setUniform(const std::string& name, const glm::mat3& value);
		void setUniform(const std::string& name, const glm::vec3& value);
		void setUniform(const std::string& name, const glm::vec4& value);

		void drawArrays(const std::vector<vertex_t>& vertexData);
	};
}