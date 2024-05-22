#include <platform/Shader.h>
#include <NativeShader.h>

U::Shader::Shader(
	const std::string& vertexShader, 
	const std::string& fragmentShader, 
	std::unordered_map<Shader::ENamedShaderUniform, std::string> uniformNames
	):_impl(std::shared_ptr<U::NativeShader>(new U::NativeShader()))
{
    _impl->compile(vertexShader, fragmentShader, uniformNames);
}

void U::Shader::bind() noexcept {
	_impl->bind();
}

void U::Shader::unbind() noexcept {
	_impl->unbind();
}

void U::Shader::setUniform(ENamedShaderUniform uniform, const glm::mat4& value) noexcept {
	_impl->setUniform(uniform, value);
}

void U::Shader::setUniform(ENamedShaderUniform uniform, const glm::mat3& value) noexcept {
	_impl->setUniform(uniform, value);
}

void U::Shader::setUniform(ENamedShaderUniform uniform, const glm::vec3& value) noexcept {
	_impl->setUniform(uniform, value);
}

void U::Shader::setUniform(ENamedShaderUniform uniform, const glm::vec4& value) noexcept {
	_impl->setUniform(uniform, value);
}


std::shared_ptr<U::NativeShader> U::Shader::getNativeShader() {
	return _impl;
}

U::Shader::~Shader() = default;