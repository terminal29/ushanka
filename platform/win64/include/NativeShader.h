#pragma once 
#include <glad/include/gl.h>

namespace U {
	class Renderer;

	class NativeShader {
		GLuint _shaderProgramObject;

	public:
		void compile(const std::string& vertexShader, const std::string& fragmentShader);
		void bind();
		void unbind();
	};
}