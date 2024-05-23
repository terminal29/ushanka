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

void U::Shader::setUniformMat4(ENamedShaderUniform uniform, const glm::fmat4& value) noexcept {
	_impl->setUniformMat4(uniform, value);
}

void U::Shader::setUniformMat3(ENamedShaderUniform uniform, const glm::fmat3& value) noexcept {
	_impl->setUniformMat3(uniform, value);
}

void U::Shader::setUniformVec3(ENamedShaderUniform uniform, const glm::fvec3& value) noexcept {
	_impl->setUniformVec3(uniform, value);
}

void U::Shader::setUniformVec4(ENamedShaderUniform uniform, const glm::fvec4& value) noexcept {
	_impl->setUniformVec4(uniform, value);
}


std::shared_ptr<U::NativeShader> U::Shader::getNativeShader() {
	return _impl;
}

U::Shader::~Shader() = default;