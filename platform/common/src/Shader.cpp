#include <platform/Shader.h>
#include <NativeShader.h>

U::Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
    : _impl(std::shared_ptr<U::NativeShader>(new U::NativeShader()))
{
    _impl->compile(vertexShader, fragmentShader);
}

void U::Shader::bind() {
	_impl->bind();
}

void U::Shader::unbind() {
	_impl->unbind();
}

void U::Shader::setUniform(U::NamedShaderUniform uniform, const glm::mat4& value) {
	_impl->setUniform(NamedShaderUniforms.at(uniform), value);
}

void U::Shader::setUniform(U::NamedShaderUniform uniform, const glm::mat3& value) {
	_impl->setUniform(NamedShaderUniforms.at(uniform), value);
}

void U::Shader::setUniform(U::NamedShaderUniform uniform, const glm::vec3& value) {
	_impl->setUniform(NamedShaderUniforms.at(uniform), value);
}

void U::Shader::setUniform(U::NamedShaderUniform uniform, const glm::vec4& value) {
	_impl->setUniform(NamedShaderUniforms.at(uniform), value);
}

void U::Shader::drawArrays(const std::vector<vertex_t>& vertexData) {
	_impl->drawArrays(vertexData);
}

std::shared_ptr<U::NativeShader> U::Shader::getNativeShader() {
	return _impl;
}

U::Shader::~Shader() = default;