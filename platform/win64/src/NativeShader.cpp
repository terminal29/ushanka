#include "glad/include/gl.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <platform/Shader.h>
#include <utility>
#include <NativeShader.h>

void U::NativeShader::compile(
	const std::string& vertexShader,
	const std::string& fragmentShader,
	std::unordered_map<Shader::ENamedShaderUniform, std::string> uniformNames) {

	int success;
	char infoLog[512];
	// make vertex shader
	const char* vertexShaderCStr = vertexShader.c_str();
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderObject, 1, &vertexShaderCStr, NULL);
	glCompileShader(vertexShaderObject);
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderObject, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
		std::terminate();
	}

	// make frament shader
	const char* fragmentShaderCStr = fragmentShader.c_str();
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderObject, 1, &fragmentShaderCStr, NULL);
	glCompileShader(fragmentShaderObject);
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderObject, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
		std::terminate();
	}

	// make program
	_shaderProgramObject = glCreateProgram();

	glAttachShader(_shaderProgramObject, vertexShaderObject);
	glAttachShader(_shaderProgramObject, fragmentShaderObject);
	glLinkProgram(_shaderProgramObject);
	glGetProgramiv(_shaderProgramObject, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(_shaderProgramObject, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
			<< infoLog << std::endl;
		std::terminate();
	}

	for (auto& [name, uniform] : uniformNames) {
		GLint uniformLocation = glGetUniformLocation(_shaderProgramObject, uniform.c_str());
		if (uniformLocation == -1) {
			std::cout << "ERROR::SHADER::UNIFORM::NOT_FOUND\n"
				<< uniform << std::endl;
			std::terminate();
		}
		_uniforms.insert_or_assign(name, uniformLocation);
	}

	// cleanup vertex & fragment shaders
	glDeleteShader(vertexShaderObject);
	glDeleteShader(fragmentShaderObject);
}

void U::NativeShader::bind() noexcept
{
	glUseProgram(_shaderProgramObject);
}

void U::NativeShader::unbind() noexcept
{
	glUseProgram(0);
}

void U::NativeShader::setUniform(Shader::ENamedShaderUniform uniform, const glm::mat4& value) noexcept
{
	glUniformMatrix4fv(_uniforms[uniform], 1, GL_FALSE, glm::value_ptr(value));
}

void U::NativeShader::setUniform(Shader::ENamedShaderUniform uniform, const glm::mat3& value) noexcept
{
	glUniformMatrix3fv(_uniforms[uniform], 1, GL_FALSE, glm::value_ptr(value));
}

void U::NativeShader::setUniform(Shader::ENamedShaderUniform uniform, const glm::vec3& value) noexcept
{
	glUniform3fv(_uniforms[uniform], 1, glm::value_ptr(value));
}

void U::NativeShader::setUniform(Shader::ENamedShaderUniform uniform, const glm::vec4& value) noexcept
{
	glUniform4fv(_uniforms[uniform], 1, glm::value_ptr(value));
}
